#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternMatcher.h"


static const unsigned MAX_NLAYERS = 10;

void PatternMatcher::resetMap() {
    for (HitIdShortMap::const_iterator it=hitIdMap_.begin(); it!=hitIdMap_.end(); ++it)
        *(it->second) = 0;
}


// _____________________________________________________________________________
// Read from the pattern bank
int PatternMatcher::readPatterns(TString src) {
    if (src.EndsWith(".root")) {
        if (verbose_)  std::cout << Info() << "Opening " << src << std::endl;
        TFile* tfile = TFile::Open(src);
        TTree* ttree = (TTree*) tfile->Get("patternBank");
        if (ttree == 0) {
            std::cout << Error() << "Unable to retrieve the TTree." << std::endl;
        }

        UInt_t nLayers;
        UInt_t nDCBits;
        UInt_t hash;
        UInt_t frequency;
        ULong64_t pattern[MAX_NLAYERS];
        UShort_t bit[MAX_NLAYERS];

        ttree->SetBranchAddress("nLayers", &nLayers);  // redundant
        ttree->SetBranchAddress("nDCBits", &nDCBits);  // redundant
        ttree->SetBranchAddress("hash", &hash);
        ttree->SetBranchAddress("frequency", &frequency);
        ttree->SetBranchAddress("pattern", pattern);
        ttree->SetBranchAddress("bit", bit);

        Long64_t nentries = ttree->GetEntries();
        if (verbose_)  std::cout << Info() << "Reading " << nentries << " patterns." << std::endl;

        // _____________________________________________________________________
        // Loop over all patterns
        for (Long64_t ievt=0; ievt<nentries; ++ievt) {
            ttree->GetEntry(ievt);
            if ((int) nLayers != nLayers_ || (int) nDCBits != nDCBits_) {
                std::cout << Error() << "Conflict in nLayers setting: " << nLayers_ << " or nDCBits setting: " << nDCBits_ << std::endl;
                return 1;
            }

            Pattern patt(nLayers, nDCBits, hash);  // initialize a new pattern
            for (unsigned l=0; l<nLayers; ++l) {
                patt.setHitId(l, pattern[l]);
                patt.setHitBit(l, bit[l]);
            }
            patt.ready();
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
    uint32_t nentries = patterns_.size();
    if (nentries == 0) {
        std::cout << Error() << "Pattern bank has zero entry." << std::endl;
        return 1;
    }

    // _________________________________________________________________________
    // Loop over all patterns
    for (uint32_t ievt=0; ievt<nentries; ++ievt) {
        Pattern& patt = patterns_.at(ievt);  // pass by reference

        for (int l=0; l<nLayers_; ++l) {
            uint64_t hitId = patt.getHitId(l);
            if (hitId == 0)  continue;  // avoid zero

            //HitIdBoolMap::const_iterator found = hitIdMap_.find(hitId);
            HitIdShortMap::const_iterator found = hitIdMap_.find(hitId);
            if (found == hitIdMap_.end()) {  // if not exist, insert the hitId
                //std::shared_ptr<bool> abool(new bool(false));
                //hitIdMap_.insert(std::make_pair(hitId, abool) );
                //patt.setHitAddress(l, abool.get());  // the address of the bool

                std::shared_ptr<uint16_t> ashort(new uint16_t(0));
                hitIdMap_.insert(std::make_pair(hitId, ashort) );
                patt.setHitAddress(l, ashort.get());  // the address of the short int

            } else {
                patt.setHitAddress(l, found->second.get());  // the address of the short int
            }
        }
        if (verbose_>2) {
            const Pattern::pattern8_t& pattId = patt.getPatternId();
            std::cout << Debug() << "... patt: " << ievt << " ";
            std::copy(pattId.begin(), pattId.end(), std::ostream_iterator<uint64_t>(std::cout, " "));
            std::cout << " freq: " << patt.frequency() << std::endl;
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


int PatternMatcher::putPatternsFast() {
    uint32_t nentries = patterns_.size();
    if (nentries == 0) {
        std::cout << Error() << "Pattern bank has zero entry." << std::endl;
        return 1;
    }

    // _________________________________________________________________________
    // Loop over all patterns
    for (uint32_t ievt=0; ievt<nentries; ++ievt) {
        Pattern& patt = patterns_.at(ievt);  // pass by reference

        for (int l=0; l<nLayers_; ++l) {
            uint64_t hitId = patt.getHitId(l);
            uint16_t hitBit = patt.getHitBit(l);
            if (hitId == 0)  continue;  // avoid zero

            // Using unordered_multimap, so collision is ok
            uint16_32_pair apair(hitBit, ievt);
            hitIdMapFast_.insert(std::make_pair(hitId, apair) );
        }
        if (verbose_>2) {
            const Pattern::pattern8_t& pattId = patt.getPatternId();
            std::cout << Debug() << "... patt: " << ievt << " ";
            std::copy(pattId.begin(), pattId.end(), std::ostream_iterator<uint64_t>(std::cout, " "));
            std::cout << " freq: " << patt.frequency() << std::endl;
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
    if (src.EndsWith(".root")) {
        if (verbose_)  std::cout << Info() << "Opening " << src << std::endl;
        if (chain_->Add(src) )  // 1 if successful, 0 otherwise
            return 0;

    //} else if (src.EndsWith(".txt")) {
    //    std::string line;
    //    ifstream ifs(src);
    //    while (std::getline(ifs,line)) {
    //        TString tstring(line);
    //        tstring.ReplaceAll(" ","");  // strip white space
    //        if (!tstring.BeginsWith("#") && tstring.EndsWith(".root")) {
    //            if (verbose_)  std::cout << Info() << "Opening " << tstring << std::endl;
    //            chain_->Add(tstring);
    //        }
    //    }
    //    ifs.close();

    } else if (src.EndsWith(".txt")) {
        TFileCollection* fc = new TFileCollection("fileinfolist", "", src);
        if (chain_->AddFileInfoList((TCollection*) fc->GetList()) )  // 1 if successful, 0 otherwise
            return 0;
    }

    std::cout << Error() << "Input source should be either a .root file or a .txt file." << std::endl;
    return 1;
}

// _____________________________________________________________________________
// Do pattern recognition
int PatternMatcher::readAndMakeTree(TString out_tmp) {
    if (!out_tmp.EndsWith(".root")) {
        std::cout << Error() << "Output filename must be .root" << std::endl;
        return 1;
    }
    out_tmp.ReplaceAll(".root", "_tmp.root");

    Long64_t nentries = chain_->GetEntries();
    if (nentries <= 0) {
        std::cout << Error() << "Input source has zero entry." << std::endl;
        return 1;
    }

    if (nEvents_ > (int) nentries)
        nEvents_ = nentries;
    if (verbose_)  std::cout << Info() << "Reading " << nEvents_ << " events; recreating " << out_tmp << " with pattern matching decisions." << std::endl;

    // For reading
    std::vector<float> *              vb_x                = 0;
    std::vector<float> *              vb_y                = 0;
    std::vector<float> *              vb_z                = 0;
    std::vector<float> *              vb_r                = 0;
    std::vector<float> *              vb_phi              = 0;
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
    chain_->SetBranchStatus("TTStubs_x"        , 1);
    chain_->SetBranchStatus("TTStubs_y"        , 1);
    chain_->SetBranchStatus("TTStubs_z"        , 1);
    chain_->SetBranchStatus("TTStubs_r"        , 1);
    chain_->SetBranchStatus("TTStubs_phi"      , 1);
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
    chain_->SetBranchAddress("TTStubs_x"        , &(vb_x));
    chain_->SetBranchAddress("TTStubs_y"        , &(vb_y));
    chain_->SetBranchAddress("TTStubs_z"        , &(vb_z));
    chain_->SetBranchAddress("TTStubs_r"        , &(vb_r));
    chain_->SetBranchAddress("TTStubs_phi"      , &(vb_phi));
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
    TFile* tfile = TFile::Open(out_tmp, "RECREATE");
    tfile->mkdir("ntupler")->cd();

    TTree* ttree = (TTree*) chain_->CloneTree(0); // Do not copy the data yet
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

    // _________________________________________________________________________
    // Loop over all events
    Long64_t nPassed = 0;
    for (Long64_t ievt=0; ievt<nEvents_; ++ievt) {
        chain_->GetEntry(ievt);
        unsigned nstubs = vb_modId->size();
        if (verbose_>1 && ievt%1000==0)  std::cout << Debug() << Form("... Processing event: %7lld, triggering: %7lld", ievt, nPassed) << std::endl;
        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " # stubs: " << nstubs << " [# patterns: " << patterns_.size() << "]" << std::endl;

        // Reset
        *vb_trivial = false;
        *vb_passed = false;
        vb_modPassed->clear();
        vb_hitPasseds->clear();

        if (!nstubs) {  // skip if no stub
            ttree->Fill();
            continue;
        }

        ////////////////////////////////////////////////////////////////////////
        // This block is used to reduce timing
        bool keep = true;

        // Build a list of goodLayers and goodLayerModules that are unique
        // Work from the outermost stub
        Pattern::vuint32_t goodLayers;  // stores mlayer, not layer
        Pattern::vuint32_t goodLayerModules;
        for (unsigned l=0; (l<nstubs) && keep; ++l) {
            unsigned ll = (nstubs-1) - l;  // reverse iteration order
            unsigned moduleId = vb_modId->at(ll);
            uint32_t layer = Pattern::decodeLayer(moduleId);
            uint32_t mlayer = layerMap_.at(layer);  // mapping of layer --> mlayer must exist

            unsigned count = std::count(goodLayers.begin(), goodLayers.end(), mlayer);
            if (!count) {
                goodLayers.push_back(mlayer);
                goodLayerModules.push_back(moduleId);
            }
        }

        // Check again min # of layers
        if ((int) goodLayers.size() < nLayers_ - po.nMisses) {
            ttree->Fill();
            continue;
        }
        ////////////////////////////////////////////////////////////////////////

        // Loop over reconstructed stubs
        for (unsigned l=0; l<nstubs; ++l) {
            unsigned moduleId = vb_modId->at(l);
            unsigned ncols = vb_iModCols->at(l);
            unsigned nrows = vb_iModRows->at(l);
            unsigned nhits = vb_nhits->at(l);
            assert(nhits > 0);
            int col = 0;
            int row = 0;

            if (verbose_>2)  std::cout << Debug() << "... ... stub: " << l << " moduleId: " << moduleId << " trkId: " << vb_trkId->at(l) << " # hits: " << nhits << std::endl;

            for (unsigned m=0; m<nhits; ++m) {
                col = vb_hitCols->at(l).at(m);
                row = vb_hitRows->at(l).at(m);

                // Set to lower resolution according to nSubLadders and nSubModules
                col /= (ncols / std::min((int) ncols, po.nSubLadders));
                row /= (nrows / std::min((int) nrows, po.nSubModules));
                uint64_t hitId = Pattern::encodeHitId(moduleId, col, row);
                uint16_t hitBit = 1 << 0;

                // Use DCBits
                Pattern::encodeDCBit(nDCBits_, hitId, hitBit);

                HitIdShortMap::iterator found = hitIdMap_.find(hitId);
                if (found == hitIdMap_.end()) {
                    // ignore it

                } else {
                    *(found->second) |= hitBit;  // logical OR
                }
                if (verbose_>2)  std::cout << Debug() << "... ... ... hit: " << m << " hitId: " << hitId << " hitBit: " << hitBit << " col: " << col << " row: " << row << " ncols: " << ncols << " nrows: " << nrows << " trkId: " << vb_hitTrkIds->at(l).at(m) << std::endl;
                if (verbose_>2)  std::cout << Debug() << "... ... ... hit: " << m << " " << ((found == hitIdMap_.end()) ? "NOT FOUND" : "FOUND") << std::endl;
            }
        }


        // _____________________________________________________________________
        // Loop over patterns (the most time consuming part)
        if (patterns_.empty()) {
            std::cout << Error() << "Patterns are not yet loaded!" << std::endl;
            return 1;
        }

        std::vector<Pattern::vuint64_t> goodRoads;  // allow hit sharing
        Pattern::vuint32_t goodRoads_hash;
        unsigned nPatterns = patterns_.size();
        for (unsigned i=0; i<nPatterns; ++i) {
            const Pattern& patt = patterns_.at(i);

            Pattern::vuint64_t goodHitIds;  // not the finest granularity, but OK for our purpose
            for (int l=0; l<nLayers_; ++l) {
                //const bool* abool = patt.getHitAddress(l);
                //if (abool && *abool) { // if the boolean is true
                //    goodHitIds.push_back(hitIds.at(l));
                //}
                ////if (verbose_>2)  std::cout << Debug() << "... ... addr: " << l << " bool: " << (abool && *abool) << std::endl;
                const uint16_t* ashort = patt.getHitAddress(l);
                if (ashort && ((*ashort) & patt.getHitBit(l)) ) { // if the bit is matched
                    goodHitIds.push_back(patt.getHitId(l));
                }
                if (verbose_>2)  std::cout << Debug() << "... ... addr: " << l << " short: " << ((*ashort) & patt.getHitBit(l)) << std::endl;
            }

            if ((int) goodHitIds.size() >= (nLayers_ - po.nMisses)) {  // FIXME: implement majority logic here
                goodRoads.push_back(goodHitIds);
                goodRoads_hash.push_back(patt.hash());
            }
            if (verbose_>2) {
                const Pattern::pattern8_t& pattId = patt.getPatternId();
                std::cout << Debug() << "... patt: " << i << " ";
                std::copy(pattId.begin(), pattId.end(), std::ostream_iterator<uint64_t>(std::cout, " "));
                std::cout << std::endl;
                std::cout << Debug() << "... patt: " << i << " # goodHitIds: " << goodHitIds.size() << " # goodRoads: " << goodRoads.size() << std::endl;
            }
        }

        // Remember to reset the map
        resetMap();

        // Write
        if (!goodRoads.empty()) {
            uint32_t hash = hashFileEvent("patternBank_tmp.root", v_event);  // FIXME: currently not working, will switch to use generator seed instead
            unsigned hash_count = std::count(goodRoads_hash.begin(), goodRoads_hash.end(), hash);
            *vb_trivial = (hash_count > 0);
            *vb_passed = true;
            ++nPassed;
        }

        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " being written." << std::endl;
        vb_modPassed->resize(nstubs, false);
        vb_hitPasseds->resize(nstubs, std::vector<bool>());
        for (unsigned l=0; l<nstubs; ++l) {
            unsigned moduleId = vb_modId->at(l);
            unsigned ncols = vb_iModCols->at(l);
            unsigned nrows = vb_iModRows->at(l);
            unsigned nhits = vb_nhits->at(l);
            assert(nhits > 0);
            int col = 0;
            int row = 0;

            vb_hitPasseds->at(l).resize(nhits, false);
            for (unsigned m=0; m<nhits; ++m) {
                col = vb_hitCols->at(l).at(m);
                row = vb_hitRows->at(l).at(m);

                // Set to lower resolution according to nSubLadders and nSubModules
                col /= (ncols / std::min((int) ncols, po.nSubLadders));
                row /= (nrows / std::min((int) nrows, po.nSubModules));
                uint64_t hitId = Pattern::encodeHitId(moduleId, col, row);
                uint16_t hitBit = 1 << 0;

                // Use DCBits
                Pattern::encodeDCBit(nDCBits_, hitId, hitBit);

                for (unsigned ll=0; ll<goodRoads.size(); ++ll) {
                    unsigned count = std::count(goodRoads.at(ll).begin(), goodRoads.at(ll).end(), hitId);

                    if (count > 0) {
                        vb_modPassed->at(l) = true;
                        vb_hitPasseds->at(l).at(m) = true;
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


int PatternMatcher::readAndMakeTreeFast(TString out_tmp) {
    if (!out_tmp.EndsWith(".root")) {
        std::cout << Error() << "Output filename must be .root" << std::endl;
        return 1;
    }

    out_tmp.ReplaceAll(".root", "_tmp.root");
    Long64_t nentries = chain_->GetEntries();
    if (nentries <= 0) {
        std::cout << Error() << "Input source has zero entry." << std::endl;
        return 1;
    }

    if (nEvents_ > (int) nentries)
        nEvents_ = nentries;
    if (verbose_)  std::cout << Info() << "Reading " << nEvents_ << " events; recreating " << out_tmp << " with pattern matching decisions." << std::endl;

    // For reading
    std::vector<float> *              vb_x                = 0;
    std::vector<float> *              vb_y                = 0;
    std::vector<float> *              vb_z                = 0;
    std::vector<float> *              vb_r                = 0;
    std::vector<float> *              vb_phi              = 0;
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
    chain_->SetBranchStatus("TTStubs_x"        , 1);
    chain_->SetBranchStatus("TTStubs_y"        , 1);
    chain_->SetBranchStatus("TTStubs_z"        , 1);
    chain_->SetBranchStatus("TTStubs_r"        , 1);
    chain_->SetBranchStatus("TTStubs_phi"      , 1);
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
    chain_->SetBranchAddress("TTStubs_x"        , &(vb_x));
    chain_->SetBranchAddress("TTStubs_y"        , &(vb_y));
    chain_->SetBranchAddress("TTStubs_z"        , &(vb_z));
    chain_->SetBranchAddress("TTStubs_r"        , &(vb_r));
    chain_->SetBranchAddress("TTStubs_phi"      , &(vb_phi));
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
    TFile* tfile = TFile::Open(out_tmp, "RECREATE");
    tfile->mkdir("ntupler")->cd();

    TTree* ttree = (TTree*) chain_->CloneTree(0); // Do not copy the data yet
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

    // _________________________________________________________________________
    // Loop over all events
    Long64_t nPassed = 0;
    for (Long64_t ievt=0; ievt<nEvents_; ++ievt) {
        chain_->GetEntry(ievt);
        unsigned nstubs = vb_modId->size();
        if (verbose_>1 && ievt%1000==0)  std::cout << Debug() << Form("... Processing event: %7lld, triggering: %7lld", ievt, nPassed) << std::endl;
        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " # stubs: " << nstubs << " [# patterns: " << patterns_.size() << "]" << std::endl;

        // Reset
        *vb_trivial = false;
        *vb_passed = false;
        vb_modPassed->clear();
        vb_hitPasseds->clear();

        if (!nstubs) {  // skip if no stub
            ttree->Fill();
            continue;
        }

        ////////////////////////////////////////////////////////////////////////
        // This block is used to reduce timing
        bool keep = true;

        // Build a list of goodLayers and goodLayerModules that are unique
        // Work from the outermost stub
        Pattern::vuint32_t goodLayers;  // stores mlayer, not layer
        Pattern::vuint32_t goodLayerModules;
        for (unsigned l=0; (l<nstubs) && keep; ++l) {
            unsigned ll = (nstubs-1) - l;  // reverse iteration order
            unsigned moduleId = vb_modId->at(ll);
            uint32_t layer = Pattern::decodeLayer(moduleId);
            uint32_t mlayer = layerMap_.at(layer);  // mapping of layer --> mlayer must exist

            unsigned count = std::count(goodLayers.begin(), goodLayers.end(), mlayer);
            if (!count) {
                goodLayers.push_back(mlayer);
                goodLayerModules.push_back(moduleId);
            }
        }

        // Check again min # of layers
        if ((int) goodLayers.size() < nLayers_ - po.nMisses) {
            ttree->Fill();
            continue;
        }
        ////////////////////////////////////////////////////////////////////////

        if (patterns_.empty() || hitIdMapFast_.empty()) {
            std::cout << Error() << "Patterns are not yet loaded!" << std::endl;
            return 1;
        }

        std::map<uint32_t, Pattern::vuint64_t>  foundPatternMap;

        // Loop over reconstructed stubs
        for (unsigned l=0; l<nstubs; ++l) {
            unsigned moduleId = vb_modId->at(l);
            unsigned ncols = vb_iModCols->at(l);
            unsigned nrows = vb_iModRows->at(l);
            unsigned nhits = vb_nhits->at(l);
            assert(nhits > 0);
            int col = 0;
            int row = 0;

            if (verbose_>2)  std::cout << Debug() << "... ... stub: " << l << " moduleId: " << moduleId << " trkId: " << vb_trkId->at(l) << " # hits: " << nhits << std::endl;

            for (unsigned m=0; m<nhits; ++m) {
                col = vb_hitCols->at(l).at(m);
                row = vb_hitRows->at(l).at(m);

                // Set to lower resolution according to nSubLadders and nSubModules
                col /= (ncols / std::min((int) ncols, po.nSubLadders));
                row /= (nrows / std::min((int) nrows, po.nSubModules));
                uint64_t hitId = Pattern::encodeHitId(moduleId, col, row);
                uint16_t hitBit = 1 << 0;

                // Use DCBits
                Pattern::encodeDCBit(nDCBits_, hitId, hitBit);

                auto found = hitIdMapFast_.equal_range(hitId);
                for (auto it=found.first; it!=found.second; ++it) {  // loop over all found hits in the hitIdMapFast_
                    if (hitBit & it->second.first) {  // match the hitBit

                        auto found2 = foundPatternMap.find(it->second.second);
                        if (found2 != foundPatternMap.end()) {  // found the pattern number in the foundPatternMap
                            found2->second.push_back(hitId);
                        } else {
                            Pattern::vuint64_t hitIds;
                            hitIds.push_back(hitId);
                            foundPatternMap.insert(std::make_pair(it->second.second, hitIds));
                        }
                    }
                }
                if (verbose_>2)  std::cout << Debug() << "... ... ... hit: " << m << " hitId: " << hitId << " hitBit: " << hitBit << " col: " << col << " row: " << row << " ncols: " << ncols << " nrows: " << nrows << " trkId: " << vb_hitTrkIds->at(l).at(m) << std::endl;
                //if (verbose_>2)  std::cout << Debug() << "... ... ... hit: " << m << " " << ((found == hitIdMap_.end()) ? "NOT FOUND" : "FOUND") << std::endl;
            }
        }

        std::vector<Pattern::vuint64_t> goodRoads;  // allow hit sharing
        Pattern::vuint32_t goodRoads_hash;
        for (auto it: foundPatternMap) {
            if ((int) it.second.size() >= (nLayers_ - po.nMisses)) {
                goodRoads.push_back(it.second);
                uint32_t hash = patterns_.at(it.first).hash();
                goodRoads_hash.push_back(hash);
            }
        }

        // Write
        if (!goodRoads.empty()) {
            uint32_t hash = hashFileEvent("patternBank_tmp.root", v_event);  // FIXME: currently not working, will switch to use generator seed instead
            unsigned hash_count = std::count(goodRoads_hash.begin(), goodRoads_hash.end(), hash);
            *vb_trivial = (hash_count > 0);
            *vb_passed = true;
            ++nPassed;
        }

        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " being written." << std::endl;
        vb_modPassed->resize(nstubs, false);
        vb_hitPasseds->resize(nstubs, std::vector<bool>());
        for (unsigned l=0; l<nstubs; ++l) {
            unsigned moduleId = vb_modId->at(l);
            unsigned ncols = vb_iModCols->at(l);
            unsigned nrows = vb_iModRows->at(l);
            unsigned nhits = vb_nhits->at(l);
            assert(nhits > 0);
            int col = 0;
            int row = 0;

            vb_hitPasseds->at(l).resize(nhits, false);
            for (unsigned m=0; m<nhits; ++m) {
                col = vb_hitCols->at(l).at(m);
                row = vb_hitRows->at(l).at(m);

                // Set to lower resolution according to nSubLadders and nSubModules
                col /= (ncols / std::min((int) ncols, po.nSubLadders));
                row /= (nrows / std::min((int) nrows, po.nSubModules));
                uint64_t hitId = Pattern::encodeHitId(moduleId, col, row);
                uint16_t hitBit = 1 << 0;

                // Use DCBits
                Pattern::encodeDCBit(nDCBits_, hitId, hitBit);

                for (unsigned ll=0; ll<goodRoads.size(); ++ll) {
                    unsigned count = std::count(goodRoads.at(ll).begin(), goodRoads.at(ll).end(), hitId);

                    if (count > 0) {
                        vb_modPassed->at(l) = true;
                        vb_hitPasseds->at(l).at(m) = true;
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

// _____________________________________________________________________________
// Merge input source and results
int PatternMatcher::writeTree(TString out) {
    if (!out.EndsWith(".root")) {
        std::cout << Error() << "Output filename must be .root" << std::endl;
        return 1;
    }

    TString out_tmp = out;
    out_tmp.ReplaceAll(".root", "_tmp.root");
    Long64_t nentries = chain_->GetEntries();
    assert(nentries > 0);

    TFile* tfile_tmp = TFile::Open(out_tmp);
    TTree* ttree_tmp = (TTree*) tfile_tmp->Get("ntupler/tree");
    assert(ttree_tmp != 0);

    if (nEvents_ > (int) nentries)
        nEvents_ = nentries;
    if (verbose_)  std::cout << Info() << "Recreating " << out << " with " << nEvents_ << " events, merging input source and " << out_tmp << std::endl;

    //chain_->ResetBranchAddresses();
    chain_->SetBranchStatus("*", 1);
    //std::vector<unsigned> *           vb_modId            = 0;
    //chain_->SetBranchAddress("TTStubs_modId"    , &(vb_modId));

    // For merging
    bool                              vb_trivial          = 0;
    bool                              vb_passed           = 0;
    std::vector<bool> *               vb_modPassed        = 0;
    std::vector<std::vector<bool> > * vb_hitPasseds       = 0;

    ttree_tmp->SetBranchAddress("TTStubs_trivial"   , &(vb_trivial));
    ttree_tmp->SetBranchAddress("TTStubs_passed"    , &(vb_passed));
    ttree_tmp->SetBranchAddress("TTStubs_modPassed" , &(vb_modPassed));
    ttree_tmp->SetBranchAddress("TTStubs_hitPasseds", &(vb_hitPasseds));

    // For writing
    TFile* tfile = TFile::Open(out, "RECREATE");
    tfile->mkdir("ntupler")->cd();

    TTree* ttree = (TTree*) chain_->CloneTree(0); // Do not copy the data yet
    // The clone should not delete any shared i/o buffers.
    ResetDeleteBranches(ttree);

    ttree->Branch("TTStubs_trivial"   , &(vb_trivial));  // a trivial match happens when an event is matched by the pattern generated from itself
    ttree->Branch("TTStubs_passed"    , &(vb_passed));
    ttree->Branch("TTStubs_modPassed" , &(vb_modPassed));
    ttree->Branch("TTStubs_hitPasseds", &(vb_hitPasseds));

    // _________________________________________________________________________
    // Loop over all events
    for (Long64_t ievt=0; ievt<nEvents_; ++ievt) {
        chain_->GetEntry(ievt);
        ttree_tmp->GetEntry(ievt);
        if (verbose_>1 && ievt%10000==0)  std::cout << Debug() << Form("... Writing event: %7lld", ievt) << std::endl;

        ttree->Fill();
    }

    assert(ttree->GetEntries() == nEvents_);
    tfile->Write();
    delete ttree;
    delete tfile;
    delete ttree_tmp;
    delete tfile_tmp;
    return 0;
}

// _____________________________________________________________________________
// Main driver
int PatternMatcher::run(TString src, TString out, TString bank) {
    gROOT->ProcessLine("#include <vector>");  // how is it not loaded?

    int exitcode = 0;
    bool useFastAlgo = true;
    Timing(1);

    exitcode = readPatterns(bank);
    if (exitcode)  return exitcode;
    Timing();

    if (useFastAlgo) {
        exitcode = putPatternsFast();
    } else {
        exitcode = putPatterns();
    }
    if (exitcode)  return exitcode;
    Timing();

    exitcode = readFile(src);
    if (exitcode)  return exitcode;
    Timing();

    if (useFastAlgo) {
        exitcode = readAndMakeTreeFast(out);  // this reads and updates the tree with AM decision
    } else {
        exitcode = readAndMakeTree(out);  // this reads and updates the tree with AM decision
    }
    if (exitcode)  return exitcode;
    Timing();

    // Apparently ROOT doesn't let you loop over a TChain twice
    // Before merging results, chain_ has to be closed and read again
    if (verbose_)  std::cout << Info() << "Closing input source and reopening it." << std::endl;
    chain_->Reset();
    exitcode = readFile(src);
    if (exitcode)  return exitcode;
    Timing();

    // Finally write
    exitcode = writeTree(out);
    if (exitcode)  return exitcode;
    Timing();

    chain_->Reset();
    return exitcode;
}
