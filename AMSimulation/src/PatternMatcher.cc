#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternMatcher.h"


static const unsigned MAX_NLAYERS = 10;

#include "TBranchElement.h"
void ResetDeleteBranches(TTree* tree)
{
    TObjArray* branches = tree->GetListOfBranches();
    Int_t nb = branches->GetEntriesFast();
    for (Int_t i = 0; i < nb; ++i) {
        TBranch* br = (TBranch*) branches->UncheckedAt(i);
        if (br->InheritsFrom(TBranchElement::Class())) {
            ((TBranchElement*) br)->ResetDeleteObject();
        }
    }
}

void PatternMatcher::resetMap() {
    for (HitIdBoolMap::const_iterator it=hitIdMap_.begin(); it!=hitIdMap_.end(); ++it)
        *(it->second) = false;
}


// _____________________________________________________________________________
// Read from the pattern bank
int PatternMatcher::readPatterns(TString src) {
    if (src.EndsWith(".root")) {
        if (verbose_)  std::cout << Info() << "Opening " << src << std::endl;
        TFile* tfile = TFile::Open(src);
        TTree* ttree = (TTree*) tfile->Get("patternBank");

        int nLayers;
        UInt_t hash;
        UInt_t frequency;
        ULong64_t pattern[MAX_NLAYERS];
        UChar_t bit[MAX_NLAYERS];

        ttree->SetBranchAddress("nLayers", &nLayers);
        ttree->SetBranchAddress("hash", &hash);
        ttree->SetBranchAddress("frequency", &frequency);
        ttree->SetBranchAddress("pattern", pattern);
        ttree->SetBranchAddress("bit", bit);

        Long64_t nentries = ttree->GetEntries();
        if (verbose_)  std::cout << Info() << "Reading " << nentries << " patterns." << std::endl;

        for (Long64_t i=0; i<nentries; ++i) {
            ttree->GetEntry(i);
            if (nLayers != nLayers_) {
                std::cout << Error() << "Conflict in nLayers setting: " << nLayers_ << std::endl;
                return 1;
            }

            Pattern patt(nLayers, hash);  // initialize a new pattern
            for (int l=0; l<nLayers; ++l) {
                patt.setHit(l, pattern[l], NULL);
                patt.setBit(l, bit[l]);
            }
            patterns_.push_back(patt);
        }

        if (patterns_.empty()) {
            std::cout << Error() << "Failed to read pattern bank from the root file: " << src << std::endl;
            return 1;
        } else if ((int) patterns_.size() != nentries) {
            std::cout << Error() << "Failed to read all patterns from the root file: " << src << std::endl;
        }

        delete ttree;
        delete tfile;
    }
    return 0;
}

// _____________________________________________________________________________
// FIXME: use a minimal perfect hash table?
int PatternMatcher::putPatterns() {
    unsigned nentries = patterns_.size();
    if (nentries <= 0) {
        std::cout << Error() << "Pattern bank has zero entry." << std::endl;
        return 1;
    }

    for (unsigned i=0; i<nentries; ++i) {
        Pattern& patt = patterns_.at(i);  // pass by reference
        const Pattern::vuint64_t& hitIds = patt.getHitIds();

        for (unsigned l=0; l<hitIds.size(); ++l) {
            uint64_t hitId = hitIds.at(l);
            if (!hitId)  // avoid zero
                continue;

            HitIdBoolMap::const_iterator found = hitIdMap_.find(hitId);
            if (found == hitIdMap_.end()) {
                std::shared_ptr<bool> abool(new bool(false));
                hitIdMap_.insert(std::make_pair(hitId, abool) );
                patt.setHit(l, hitId, abool.get());  // the address of the bool

            } else {
                patt.setHit(l, hitId, found->second.get());  // the address of the bool
            }
        }
        if (verbose_>2) {
            const Pattern::pattern8_t& pattId = patt.getPatternId();
            std::cout << Debug() << "... patt: " << i << " ";
            std::copy(pattId.begin(), pattId.end(), std::ostream_iterator<uint64_t>(std::cout, " "));
            std::cout << std::endl;
        }
    }
    if (verbose_>1)  std::cout << Debug() << "Identified " << hitIdMap_.size() << " possible 'hitId's." << std::endl;
    //if (verbose_>2) {
    //    unsigned idebug=0;
    //    for (HitIdBoolMap::const_iterator it=hitIdMap_.begin(); it!=hitIdMap_.end(); ++it) {
    //        std::cout << Debug() << "... hitId: " << idebug << " " << it->first << std::endl;
    //        ++idebug;
    //    }
    //}
    if (verbose_)  std::cout << Info() << "Loaded the patterns into memory." << std::endl;
    return 0;
}

// _____________________________________________________________________________
// Read the input ntuples
int PatternMatcher::readFile(TString src) {  // currently it's a carbon copy from PatternGenerator
    if (src == "EMPTY") {
        //TString out = "dummy.root";
        //if (verbose_)  std::cout << Info() << "Recreating " << out << " with 10000 events." << std::endl;
        //createDummy(out, 10000);
        //chain_->Add(out);

    } else if (src.EndsWith(".root")) {
        if (verbose_)  std::cout << Info() << "Opening " << src << std::endl;
        chain_->Add(src);

    } else if (src.EndsWith(".txt")) {
        std::string line;
        ifstream ifs(src);
        while (std::getline(ifs,line)) {
            TString tstring(line);
            if (!tstring.BeginsWith("#") && tstring.EndsWith(".root")) {
                if (verbose_)  std::cout << Info() << "Opening " << tstring << std::endl;
                chain_->Add(tstring);
            }
        }
        ifs.close();

    } else {
        std::cout << Error() << "Input source should be either a .root file or a .txt file." << std::endl;
        return 1;
    }
    return 0;
}

// _____________________________________________________________________________
// Do pattern recognition
int PatternMatcher::readAndWriteTree(TString out) {
    Long64_t nentries = chain_->GetEntries();
    if (nentries <= 0) {
        std::cout << Error() << "Input source has zero entry." << std::endl;
        return 1;
    }

    if (nEvents_ > (int) nentries)
        nEvents_ = nentries;
    if (verbose_)  std::cout << Info() << "Reading " << nEvents_ << " events; recreating " << out << " with pattern matching decisions." << std::endl;

    // For reading
    std::vector<unsigned> *           vb_iModCols         = 0;
    std::vector<unsigned> *           vb_iModRows         = 0;
    std::vector<unsigned> *           vb_modId            = 0;
    std::vector<unsigned> *           vb_nhits            = 0;
    std::vector<std::vector<int> > *  vb_hitCols          = 0;
    std::vector<std::vector<int> > *  vb_hitRows          = 0;
    std::vector<std::vector<int> > *  vb_hitTrkIds        = 0;
    std::vector<float> *              vb_simPt            = 0;
    std::vector<float> *              vb_simEta           = 0;
    std::vector<float> *              vb_simPhi           = 0;
    std::vector<int> *                vb_trkId            = 0;
    unsigned                          v_event             = 0;

    chain_->SetBranchStatus("*", 0);
    chain_->SetBranchStatus("TTStubs_iModCols" , 1);
    chain_->SetBranchStatus("TTStubs_iModRows" , 1);
    chain_->SetBranchStatus("TTStubs_modId"    , 1);
    chain_->SetBranchStatus("TTStubs_nhits"    , 1);
    chain_->SetBranchStatus("TTStubs_hitCols"  , 1);
    chain_->SetBranchStatus("TTStubs_hitRows"  , 1);
    chain_->SetBranchStatus("TTStubs_hitTrkIds", 1);
    chain_->SetBranchStatus("TTStubs_simPt"    , 1);
    chain_->SetBranchStatus("TTStubs_simEta"   , 1);
    chain_->SetBranchStatus("TTStubs_simPhi"   , 1);
    chain_->SetBranchStatus("TTStubs_trkId"    , 1);
    chain_->SetBranchStatus("event"            , 1);

    // Unfortunately, very different semantics for Branch(...) vs SetBranchAddress(...)
    chain_->SetBranchAddress("TTStubs_iModCols" , &(vb_iModCols));
    chain_->SetBranchAddress("TTStubs_iModRows" , &(vb_iModRows));
    chain_->SetBranchAddress("TTStubs_modId"    , &(vb_modId));
    chain_->SetBranchAddress("TTStubs_nhits"    , &(vb_nhits));
    chain_->SetBranchAddress("TTStubs_hitCols"  , &(vb_hitCols));
    chain_->SetBranchAddress("TTStubs_hitRows"  , &(vb_hitRows));
    chain_->SetBranchAddress("TTStubs_hitTrkIds", &(vb_hitTrkIds));
    chain_->SetBranchAddress("TTStubs_simPt"    , &(vb_simPt));
    chain_->SetBranchAddress("TTStubs_simEta"   , &(vb_simEta));
    chain_->SetBranchAddress("TTStubs_simPhi"   , &(vb_simPhi));
    chain_->SetBranchAddress("TTStubs_trkId"    , &(vb_trkId));
    chain_->SetBranchAddress("event"            , &(v_event));

    // For writing
    TFile* tfile = TFile::Open(out, "RECREATE");
    tfile->mkdir("ntupler");
    tfile->cd("ntupler");

    TTree *ttree = chain_->CloneTree(0); // Do no copy the data yet
    // The clone should not delete any shared i/o buffers.
    ResetDeleteBranches(ttree);

    std::auto_ptr<bool>                             vb_trivial        (new bool(false));
    std::auto_ptr<bool>                             vb_passed         (new bool(false));
    std::auto_ptr<std::vector<bool> >               vb_modPassed      (new std::vector<bool>());
    std::auto_ptr<std::vector<std::vector<bool> > > vb_hitPasseds     (new std::vector<std::vector<bool> >());

    ttree->Branch("TTStubs_trivial"   , &(*vb_trivial));  // a trivial match happens when an event is matched by the pattern generated from itself
    ttree->Branch("TTStubs_passed"    , &(*vb_passed));
    ttree->Branch("TTStubs_modPassed" , &(*vb_modPassed));
    ttree->Branch("TTStubs_hitPasseds", &(*vb_hitPasseds));


    Long64_t nPassed = 0;
    for (Long64_t ievt=0; ievt<nEvents_; ++ievt) {
        chain_->GetEntry(ievt);
        unsigned nstubs = vb_modId->size();
        if (verbose_>1 && ievt%1000==0)  std::cout << Debug() << Form("... Processing event: %7lld, triggering: %7lld", ievt, nPassed) << std::endl;
        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " # stubs: " << nstubs << " [# patterns: " << patterns_.size() << "]." << std::endl;
        //if (!nstubs)  // skip if no stub
        //    continue;

        // Loop over reconstructed stubs
        for (unsigned l=0; l<nstubs; ++l) {
            unsigned moduleId = vb_modId->at(l);
            unsigned ncols = vb_iModCols->at(l);
            unsigned nrows = vb_iModRows->at(l);
            unsigned nhits = vb_nhits->at(l);
            int trkId = vb_trkId->at(l);
            int col = 0;
            int row = 0;
            assert(nhits > 0);
            if (verbose_>2)  std::cout << Debug() << "... ... stub: " << l << " moduleId: " << moduleId << " trkId: " << trkId << " # hits: " << nhits << std::endl;

            for (unsigned ii=0; ii<nhits; ++ii) {
                col = vb_hitCols->at(l).at(ii);
                row = vb_hitRows->at(l).at(ii);
                if (verbose_>2)  std::cout << Debug() << "... ... ... hit: " << ii << " col: " << col << " row: " << row << " sim: " << (vb_hitTrkIds->at(l).at(ii)==trkId) << std::endl;

                // Set to lower resolution according to nSubLadders and nSubModules
                col /= (ncols / std::min((int) ncols, nSubLadders_));
                row /= (nrows / std::min((int) nrows, nSubModules_));

                uint64_t hitId = Pattern::encodeHitId(moduleId, col, row);
                HitIdBoolMap::iterator found = hitIdMap_.find(hitId);
                if (found == hitIdMap_.end()) {
                    // ignore it

                } else {
                    *(found->second) = true;
                }
                if (verbose_>2)  std::cout << Debug() << "... ... ... hit: " << ii << " hitId: " << hitId << " col: " << col << " row: " << row << " ncols: " << ncols << " nrows: " << nrows << std::endl;
                if (verbose_>2)  std::cout << Debug() << "... ... ... hit: " << ii << " " << ((found == hitIdMap_.end()) ? "NOT FOUND" : "FOUND") << std::endl;
            }
        }


        // Loop over patterns
        if (patterns_.empty()) {
            std::cout << Error() << "Patterns are not yet loaded!" << std::endl;
            return 1;
        }

        std::vector<Pattern::vuint64_t> goodRoads;  // allow hit sharing
        std::vector<Pattern::vuint32_t> goodRoads_hash;
        unsigned nPatterns = patterns_.size();
        for (unsigned i=0; i<nPatterns; ++i) {
            Pattern& patt = patterns_.at(i);  // pass by reference
            const Pattern::vuint64_t& hitIds = patt.getHitIds();
            const std::vector<const bool*>& hitAddresses = patt.getHitAddresses();
            assert(hitIds.size() == hitAddresses.size());
            if (verbose_>2) {
                const Pattern::pattern8_t& pattId = patt.getPatternId();
                std::cout << Debug() << "... patt: " << i << " ";
                std::copy(pattId.begin(), pattId.end(), std::ostream_iterator<uint64_t>(std::cout, " "));
                std::cout << std::endl;
            }

            Pattern::vuint64_t goodHitIds;  // not the finest granularity, but OK for our purpose
            Pattern::vuint32_t goodHitIds_hash;
            for (unsigned l=0; l<hitAddresses.size(); ++l) {
                const bool* abool = hitAddresses.at(l);
                if (abool && *abool) { // if the boolean is true
                    goodHitIds.push_back(hitIds.at(l));
                    goodHitIds_hash.push_back(patt.hash());
                }
                //if (verbose_>2)  std::cout << Debug() << "... ... addr: " << l << " bool: " << (abool && *abool) << std::endl;
            }

            if ((int) goodHitIds.size() >= (nLayers_-0)) {  // FIXME: implement majority logic here
                goodRoads.push_back(goodHitIds);
                goodRoads_hash.push_back(goodHitIds_hash);
            }
            if (verbose_>2)  std::cout << Debug() << "... patt: " << i << " # goodHitIds: " << goodHitIds.size() << " # goodRoads: " << goodRoads.size() << std::endl;
        }
        if (!goodRoads.empty())
            ++nPassed;

        // Remember to reset the map
        resetMap();


        // Write
        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " being written." << std::endl;

        uint32_t hash = hashFileEvent(chain_->GetCurrentFile()->GetName(), v_event);
        vb_modPassed->resize(nstubs, false);
        vb_hitPasseds->resize(nstubs, std::vector<bool>());
        for (unsigned l=0; l<nstubs; ++l) {
            unsigned moduleId = vb_modId->at(l);
            unsigned ncols = vb_iModCols->at(l);
            unsigned nrows = vb_iModRows->at(l);
            unsigned nhits = vb_nhits->at(l);
            int col = 0;
            int row = 0;
            assert(nhits > 0);

            vb_hitPasseds->at(l).resize(nhits, false);
            for (unsigned ii=0; ii<nhits; ++ii) {
                col = vb_hitCols->at(l).at(ii);
                row = vb_hitRows->at(l).at(ii);

                // Set to lower resolution according to nSubLadders and nSubModules
                col /= (ncols / std::min((int) ncols, nSubLadders_));
                row /= (nrows / std::min((int) nrows, nSubModules_));
                uint64_t hitId = Pattern::encodeHitId(moduleId, col, row);

                for (unsigned ll=0; ll<goodRoads.size(); ++ll) {
                    unsigned count = std::count(goodRoads.at(ll).begin(), goodRoads.at(ll).end(), hitId);
                    unsigned count_hash = std::count(goodRoads_hash.at(ll).begin(), goodRoads_hash.at(ll).end(), hash);
                    if (count > 0) {
                        *vb_trivial = (count_hash > 0);
                        *vb_passed = true;
                        vb_modPassed->at(l) = true;
                        vb_hitPasseds->at(l).at(ii) = true;
                        break;
                    } else {
                        // do nothing
                    }
                }
            }
        }

        ttree->Fill();
    }
    if (verbose_)  std::cout << Info() << "Processed " << nEvents_ << " events, triggered on " << nPassed << " events." << std::endl;

    assert(ttree->GetEntries() == nEvents_);
    tfile->Write();
    delete ttree;
    delete tfile;

    return 0;
}


//int PatternMatcher::writeTree(TString out) {
//    // obsolete
//    return 0;
//}


// Main driver
int PatternMatcher::run(TString src, TString out, TString bank) {
    int exitcode = 0;
    TStopwatch timer;
    if (doTiming_)  timer.Start();

    exitcode = readPatterns(bank);
    if (exitcode)  return exitcode;
    if (doTiming_) { timer.Stop(); timer.Continue(); timer.Print(); }

    exitcode = putPatterns();
    if (exitcode)  return exitcode;
    if (doTiming_) { timer.Stop(); timer.Continue(); timer.Print(); }

    exitcode = readFile(src);
    if (exitcode)  return exitcode;
    if (doTiming_) { timer.Stop(); timer.Continue(); timer.Print(); }

    exitcode = readAndWriteTree(out);  // this reads and updates the tree with AM decision
    if (exitcode)  return exitcode;
    if (doTiming_) { timer.Stop(); timer.Continue(); timer.Print(); }

    //exitcode = writeTree(out);
    //if (exitcode)  return exitcode;

    return exitcode;
}

