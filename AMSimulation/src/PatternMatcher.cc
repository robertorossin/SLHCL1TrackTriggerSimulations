#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternMatcher.h"

static const unsigned MAX_NPATTERNS = 30000000;  // 30M = only 1/8 of design goal

static const unsigned MAX_NLAYERS = pattern_type().size();  // ought to be 8

static const unsigned MAX_NKEYS = (1<<21)-1;  // 2097151


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

        // For reading
        typedef unsigned char unsigned8;
        typedef unsigned short unsigned16;
        //typedef unsigned long long unsigned64;
        //typedef ULong64_t unsigned64;
        //unsigned8 frequency;
        std::vector<unsigned>   * superstripIds = 0;
        std::vector<unsigned16> * superstripBits = 0;

        //ttree->SetBranchAddress("frequency", &frequency);
        ttree->SetBranchAddress("superstripIds", &superstripIds);
        ttree->SetBranchAddress("superstripBits", &superstripBits);

        Long64_t nentries = ttree->GetEntries();
        if (nentries > maxPatterns_)
            nentries = maxPatterns_;
        if (nentries > MAX_NPATTERNS) {
            std::cout << Error() << "Number of patterns exceeds the allocated amount! " << nentries << " vs. " << MAX_NPATTERNS << std::endl;
            return 1;
        }
        if (verbose_)  std::cout << Info() << "Reading " << nentries << " patterns." << std::endl;

        // _____________________________________________________________________
        // Loop over all patterns
        assert(ptr_allPatternIds_ == 0 && ptr_allPatternBits_ == 0);
        ptr_allPatternIds_ = new addr_type[MAX_NPATTERNS * MAX_NLAYERS];
        ptr_allPatternBits_ = new bit_type[MAX_NPATTERNS * MAX_NLAYERS];
        size_allPatternIds_ = 0;
        size_allPatternBits_ = 0;

        for (Long64_t ievt=0; ievt<nentries; ++ievt) {
            ttree->GetEntry(ievt);
            if (verbose_>2) {
                std::cout << Debug() << "... patt: " << ievt << " # superstripIds: " << superstripIds->size() << " # superstripBits: " << superstripBits->size() << " # allPatternIds: " << size_allPatternIds_ << " # allPatternBits: " << size_allPatternBits_ << std::endl;
                std::cout << Debug() << "... ... " << superstripIds->at(0) << " " << superstripIds->at(1) << " " << superstripIds->at(2) << " " << superstripIds->at(3) << " " << superstripIds->at(4) << " " << superstripIds->at(5) << " " << superstripIds->at(6) << " " << superstripIds->at(7) << std::endl;
            }
            assert(superstripIds->size() == MAX_NLAYERS && superstripBits->size() == MAX_NLAYERS);

            std::copy(superstripIds->begin(), superstripIds->begin()+MAX_NLAYERS, ptr_allPatternIds_ + size_allPatternIds_*MAX_NLAYERS);
            size_allPatternIds_ += 1;

            std::copy(superstripBits->begin(), superstripBits->begin()+MAX_NLAYERS, ptr_allPatternBits_ + size_allPatternBits_*MAX_NLAYERS);
            size_allPatternBits_ += 1;
        }

        if ((Long64_t) size_allPatternIds_ != nentries || (Long64_t) size_allPatternBits_ != nentries) {
            std::cout << Error() << "Failed to read all patterns from the root file: " << src << std::endl;
            return 1;
        }

        delete ttree;
        delete tfile;
    }

    if (ptr_allPatternIds_ == 0 || ptr_allPatternBits_ == 0) {
        std::cout << Error() << "Pattern bank pointer is NULL." << std::endl;
        return 1;
    }
    return 0;
}


// _____________________________________________________________________________
int PatternMatcher::loadPatterns() {
    unsigned nentries = size_allPatternIds_;
    assert(nentries>0);

    // _________________________________________________________________________
    // Loop over all patterns

    ssIdHashMap_.clear();
    ssIdHashMap_.resize(MAX_NKEYS);
    for (unsigned ievt=0; ievt<nentries; ++ievt) {

        for (unsigned l=0; l<MAX_NLAYERS; ++l) {
            addr_type ssId = ptr_allPatternIds_[ievt*MAX_NLAYERS + l];
            if (ssId == 0)  continue;  // avoid zero

            key_type hash = hashSuperstripId(ssId);
            ssIdHashMap_.at(hash).push_back(ievt);  // ievt is used as the id of a pattern
        }

        if (verbose_>2) {
            std::cout << Debug() << "... patt: " << ievt << " ";
            for (unsigned l=0; l<MAX_NLAYERS; ++l) {
                std::cout << ptr_allPatternIds_[ievt*MAX_NLAYERS + l] << " ";
            }
            std::cout << std::endl;
        }
    }

    if (verbose_)  std::cout << Info() << "Loaded the patterns into memory." << std::endl;
    return 0;
}


// _____________________________________________________________________________
// Read the input ntuples
int PatternMatcher::readFile(TString src) {
    if (src.EndsWith(".root")) {
        if (verbose_)  std::cout << Info() << "Opening " << src << std::endl;
        if (chain_->Add(src) )  // 1 if successful, 0 otherwise
            return 0;

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
int PatternMatcher::makeRoads() {
    if (verbose_)  std::cout << Info() << "Reading " << nEvents_ << " events" << std::endl;

    // For reading
    std::vector<float> *                vb_x              = 0;
    std::vector<float> *                vb_y              = 0;
    std::vector<float> *                vb_z              = 0;
    std::vector<float> *                vb_r              = 0;
    std::vector<float> *                vb_phi            = 0;
    std::vector<float> *                vb_roughPt        = 0;
    std::vector<unsigned> *             vb_iModCols       = 0;
    std::vector<unsigned> *             vb_iModRows       = 0;
    std::vector<unsigned> *             vb_modId          = 0;
    std::vector<unsigned> *             vb_nhits          = 0;
    std::vector<std::vector<int> > *    vb_hitCols        = 0;
    std::vector<std::vector<int> > *    vb_hitRows        = 0;
    std::vector<std::vector<int> > *    vb_hitTrkIds      = 0;
    std::vector<std::vector<float> > *  vb_hitXs          = 0;
    std::vector<std::vector<float> > *  vb_hitYs          = 0;
    std::vector<std::vector<float> > *  vb_hitZs          = 0;
    std::vector<float> *                vb_simPt          = 0;
    std::vector<float> *                vb_simEta         = 0;
    std::vector<float> *                vb_simPhi         = 0;
    std::vector<int> *                  vb_trkId          = 0;

    chain_->SetBranchStatus("*", 0);
    chain_->SetBranchStatus("TTStubs_x"        , 1);
    chain_->SetBranchStatus("TTStubs_y"        , 1);
    chain_->SetBranchStatus("TTStubs_z"        , 1);
    chain_->SetBranchStatus("TTStubs_r"        , 1);
    chain_->SetBranchStatus("TTStubs_phi"      , 1);
    chain_->SetBranchStatus("TTStubs_roughPt"  , 1);
    chain_->SetBranchStatus("TTStubs_iModCols" , 1);
    chain_->SetBranchStatus("TTStubs_iModRows" , 1);
    chain_->SetBranchStatus("TTStubs_modId"    , 1);
    chain_->SetBranchStatus("TTStubs_nhits"    , 1);
    chain_->SetBranchStatus("TTStubs_hitCols"  , 1);
    chain_->SetBranchStatus("TTStubs_hitRows"  , 1);
    chain_->SetBranchStatus("TTStubs_hitTrkIds", 1);
    chain_->SetBranchStatus("TTStubs_hitXs"    , 1);
    chain_->SetBranchStatus("TTStubs_hitYs"    , 1);
    chain_->SetBranchStatus("TTStubs_hitZs"    , 1);
    chain_->SetBranchStatus("TTStubs_simPt"    , 1);
    chain_->SetBranchStatus("TTStubs_simEta"   , 1);
    chain_->SetBranchStatus("TTStubs_simPhi"   , 1);
    chain_->SetBranchStatus("TTStubs_trkId"    , 1);

    chain_->SetBranchAddress("TTStubs_x"        , &(vb_x));
    chain_->SetBranchAddress("TTStubs_y"        , &(vb_y));
    chain_->SetBranchAddress("TTStubs_z"        , &(vb_z));
    chain_->SetBranchAddress("TTStubs_r"        , &(vb_r));
    chain_->SetBranchAddress("TTStubs_phi"      , &(vb_phi));
    chain_->SetBranchAddress("TTStubs_roughPt"  , &(vb_roughPt));
    chain_->SetBranchAddress("TTStubs_iModCols" , &(vb_iModCols));
    chain_->SetBranchAddress("TTStubs_iModRows" , &(vb_iModRows));
    chain_->SetBranchAddress("TTStubs_modId"    , &(vb_modId));
    chain_->SetBranchAddress("TTStubs_nhits"    , &(vb_nhits));
    chain_->SetBranchAddress("TTStubs_hitCols"  , &(vb_hitCols));
    chain_->SetBranchAddress("TTStubs_hitRows"  , &(vb_hitRows));
    chain_->SetBranchAddress("TTStubs_hitTrkIds", &(vb_hitTrkIds));
    chain_->SetBranchAddress("TTStubs_hitXs"    , &(vb_hitXs));
    chain_->SetBranchAddress("TTStubs_hitYs"    , &(vb_hitYs));
    chain_->SetBranchAddress("TTStubs_hitZs"    , &(vb_hitZs));
    chain_->SetBranchAddress("TTStubs_simPt"    , &(vb_simPt));
    chain_->SetBranchAddress("TTStubs_simEta"   , &(vb_simEta));
    chain_->SetBranchAddress("TTStubs_simPhi"   , &(vb_simPhi));
    chain_->SetBranchAddress("TTStubs_trkId"    , &(vb_trkId));

    allRoads_.clear();

    // Make sure the map has already been set up
    assert(size_allPatternIds_ > 0);
    assert(ssIdHashMap_.size() == MAX_NKEYS);


    // _________________________________________________________________________
    // Loop over all events
    Long64_t nPassed = 0;
    for (Long64_t ievt=0; ievt<nEvents_; ++ievt) {
        Long64_t local_entry = chain_->LoadTree(ievt);  // for TChain
        if (local_entry < 0)  break;
        chain_->GetEntry(ievt);

        unsigned nstubs = vb_modId->size();
        if (verbose_>1 && ievt%1000==0)  std::cout << Debug() << Form("... Processing event: %7lld, triggering: %7lld", ievt, nPassed) << std::endl;
        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " # stubs: " << nstubs << " [# roads: " << allRoads_.size() << "]" << std::endl;

        if (!nstubs) {  // skip if no stub
            allRoads_.push_back(std::vector<TTRoad>());
            continue;
        }

        std::map<unsigned, std::vector<TTHit> >  foundPatternMap;  // key: index of pattern

        // Loop over reconstructed stubs
        for (unsigned l=0; l<nstubs; ++l) {
            unsigned moduleId = vb_modId->at(l);
            unsigned ncols = vb_iModCols->at(l);
            unsigned nrows = vb_iModRows->at(l);
            unsigned nhits = vb_nhits->at(l);
            if (nrows == 960 || nrows == 1016)
                nrows = 1024;
            assert(nhits > 0 && (ncols == 2 || ncols == 32) && nrows == 1024);
            int col = 0;
            int row = 0;

            //float x = vb_x->at(l);
            //float y = vb_y->at(l);
            //float z = vb_z->at(l);
            float pt = vb_roughPt->at(l);

            if (verbose_>2)  std::cout << Debug() << "... ... stub: " << l << " moduleId: " << moduleId << " trkId: " << vb_trkId->at(l) << " # hits: " << nhits << std::endl;

            for (unsigned m=0; m<nhits; ++m) {
                col = vb_hitCols->at(l).at(m);
                row = vb_hitRows->at(l).at(m);

                // Set to lower resolution according to nSubLadders and nSubModules
                col /= (ncols / std::min((int) ncols, po.nSubLadders));
                row /= (nrows / std::min((int) nrows, po.nSubModules));
                addr_type ssId = encodeSuperstripId(moduleId, col, row);
                bit_type ssBit = 1 << 0;

                // Use DCBits
                encodeDCBit(nDCBits_, ssId, ssBit);

                float x = vb_hitXs->at(l).at(m);
                float y = vb_hitYs->at(l).at(m);
                float z = vb_hitZs->at(l).at(m);

                key_type hash = hashSuperstripId(ssId);
                for (auto it=ssIdHashMap_.at(hash).begin(); it!=ssIdHashMap_.at(hash).end(); ++it) {  // loop over all possible patterns
                    auto found = foundPatternMap.find(*it);  // find by index of pattern
                    if (found != foundPatternMap.end()) {  // found
                        found->second.emplace_back(x, y, z, pt, ssId, ssBit);  // construct a TTHit in place

                    } else {
                        std::vector<TTHit> hits;
                        hits.emplace_back(x, y, z, pt, ssId, ssBit);  // construct a TTHit in place
                        foundPatternMap.insert(std::make_pair(*it, hits));
                    }
                }
                if (verbose_>2) {
                    std::cout << Debug() << "... ... ... hit: " << m << " ssId: " << ssId << " ssBit: " << ssBit << " col: " << col << " row: " << row << " trkId: " << vb_hitTrkIds->at(l).at(m) << std::endl;
                    std::cout << Debug() << "... ... ... hit: " << m << " hash: " << hash << " # values: " << ssIdHashMap_.at(hash).size() << std::endl;
                }
            }
        }

        std::vector<TTRoad> roadsInThisEvent;
        if (! foundPatternMap.empty()) {
            for (auto it: foundPatternMap) {
                const TTPattern& patt = makeTTPattern(it.first);
                pattern_type pattId = patt.id();
                pattern_bit_type pattBit = patt.bit();
                const std::vector<TTHit>& hits = it.second;

                count_type count = 0, count_l = 0;
                for (unsigned l=0; l<pattId.size(); ++l) {
                    count_l = 0;
                    if (pattId.at(l) == encodeFakeSuperstripId()) {
                        count_l ++;

                    } else {
                        for (unsigned m=0; m<hits.size(); ++m) {
                            if ((pattId.at(l) == hits.at(m).superstripId() ) &&
                                (pattBit.at(l) & hits.at(m).superstripBit()) ) {
                                count_l ++;
                            }
                        }
                    }

                    if (count_l > 0)
                        count ++;
                }

                if (count >= (nLayers_ - po.nMisses)) {  // if number of hits more than the requirement
                    TTRoad road(pattId, hits);
                    roadsInThisEvent.push_back(road);
                }
            }

            if (! roadsInThisEvent.empty())
                ++nPassed;
        }
        allRoads_.push_back(roadsInThisEvent);
    }

    if (verbose_)  std::cout << Info() << "Processed " << nEvents_ << " events, triggered on " << nPassed << " events." << std::endl;

    clearHashMap();
    return 0;
}


// _____________________________________________________________________________
// Output roads into a TTree
int PatternMatcher::writeRoads(TString out) {
    if (!out.EndsWith(".root")) {
        std::cout << Error() << "Output filename must be .root" << std::endl;
        return 1;
    }

    unsigned nentries = allRoads_.size();
    if (verbose_)  std::cout << Info() << "Recreating " << out << " with " << nentries << " events." << std::endl;
    TFile* tfile = TFile::Open(out, "RECREATE");
    tfile->mkdir("ntupler")->cd();
    TTree* ttree = new TTree("tree", "");

    typedef unsigned char unsigned8;
    typedef unsigned short unsigned16;
    //typedef unsigned long long unsigned64;
    //typedef ULong64_t unsigned64;
    std::auto_ptr<std::vector<std::vector<unsigned> > >   vr_patternIds       (new std::vector<std::vector<unsigned> >());
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitXs            (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitYs            (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitZs            (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitXErrors       (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitYErrors       (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitZErrors       (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<int> > >        vr_hitCharges       (new std::vector<std::vector<int> >());
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitPts           (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<unsigned> > >   vr_hitSuperstripIds (new std::vector<std::vector<unsigned> >());
    std::auto_ptr<std::vector<std::vector<unsigned16> > > vr_hitSuperstripBits(new std::vector<std::vector<unsigned16> >());

    ttree->Branch(prefixRoad_ + "patternIds"        + suffix_, &(*vr_patternIds));
    ttree->Branch(prefixRoad_ + "hitXs"             + suffix_, &(*vr_hitXs));
    ttree->Branch(prefixRoad_ + "hitYs"             + suffix_, &(*vr_hitYs));
    ttree->Branch(prefixRoad_ + "hitZs"             + suffix_, &(*vr_hitZs));
    ttree->Branch(prefixRoad_ + "hitXErrors"        + suffix_, &(*vr_hitXErrors));
    ttree->Branch(prefixRoad_ + "hitYErrors"        + suffix_, &(*vr_hitYErrors));
    ttree->Branch(prefixRoad_ + "hitZErrors"        + suffix_, &(*vr_hitZErrors));
    ttree->Branch(prefixRoad_ + "hitCharges"        + suffix_, &(*vr_hitCharges));
    ttree->Branch(prefixRoad_ + "hitPts"            + suffix_, &(*vr_hitPts));
    ttree->Branch(prefixRoad_ + "hitSuperstripIds"  + suffix_, &(*vr_hitSuperstripIds));
    ttree->Branch(prefixRoad_ + "hitSuperstripBits" + suffix_, &(*vr_hitSuperstripBits));

    // _________________________________________________________________________
    // Loop over all roads
    for (unsigned ievt=0; ievt<nentries; ++ievt) {
        if (verbose_>1 && ievt%10000==0)  std::cout << Debug() << Form("... Writing event: %7u", ievt) << std::endl;
        vr_patternIds      ->clear();
        vr_hitXs           ->clear();
        vr_hitYs           ->clear();
        vr_hitZs           ->clear();
        vr_hitXErrors      ->clear();
        vr_hitYErrors      ->clear();
        vr_hitZErrors      ->clear();
        vr_hitCharges      ->clear();
        vr_hitPts          ->clear();
        vr_hitSuperstripIds->clear();
        vr_hitSuperstripBits->clear();

        const std::vector<TTRoad>& roadsInThisEvent = allRoads_.at(ievt);
        unsigned nroads = roadsInThisEvent.size();

        for (unsigned i=0; i<nroads; ++i) {
            if ((int) i >= maxRoads_)  break;

            std::vector<unsigned>   patternIds;
            std::vector<float>      hitXs;
            std::vector<float>      hitYs;
            std::vector<float>      hitZs;
            std::vector<float>      hitXErrors;
            std::vector<float>      hitYErrors;
            std::vector<float>      hitZErrors;
            std::vector<int>        hitCharges;
            std::vector<float>      hitPts;
            std::vector<unsigned>   hitSuperstripIds;
            std::vector<unsigned16> hitSuperstripBits;

            const TTRoad& road = roadsInThisEvent.at(i);
            const pattern_type&       pattId = road.patternId();
            const std::vector<TTHit>& hits   = road.getHits();

            for (unsigned j=0; j<pattId.size(); ++j) {
                patternIds.push_back(pattId.at(j));
            }

            for (unsigned j=0; j<hits.size(); ++j) {
                if ((int) j >= maxHits_)  break;

                const TTHit& hit = hits.at(j);
                hitXs.push_back(hit.x());
                hitYs.push_back(hit.y());
                hitZs.push_back(hit.z());
                hitXErrors.push_back(hit.xError());
                hitYErrors.push_back(hit.yError());
                hitZErrors.push_back(hit.zError());
                hitCharges.push_back(hit.charge());
                hitPts.push_back(hit.pt());
                hitSuperstripIds.push_back(hit.superstripId());
                hitSuperstripBits.push_back(hit.superstripBit());
            }

            vr_patternIds       ->push_back(patternIds);
            vr_hitXs            ->push_back(hitXs);
            vr_hitYs            ->push_back(hitYs);
            vr_hitZs            ->push_back(hitZs);
            vr_hitXErrors       ->push_back(hitXErrors);
            vr_hitYErrors       ->push_back(hitYErrors);
            vr_hitZErrors       ->push_back(hitZErrors);
            vr_hitCharges       ->push_back(hitCharges);
            vr_hitPts           ->push_back(hitPts);
            vr_hitSuperstripIds ->push_back(hitSuperstripIds);
            vr_hitSuperstripBits->push_back(hitSuperstripBits);
        }

        ttree->Fill();
        assert(vr_hitXs->size() == allRoads_.at(ievt).size());
    }
    assert(ttree->GetEntries() == (int) allRoads_.size());

    tfile->Write();
    delete ttree;
    delete tfile;
    return 0;
}

// _____________________________________________________________________________
// Private functions

TTPattern PatternMatcher::makeTTPattern(unsigned index) const {
    std::vector<TTSuperstrip> superstrips;
    for (unsigned l=0; l<MAX_NLAYERS; ++l) {
        TTSuperstrip ss(ptr_allPatternIds_[index*MAX_NLAYERS + l], ptr_allPatternBits_[index*MAX_NLAYERS + l]);
        superstrips.push_back(ss);
    }
    TTPattern patt(superstrips);
    return patt;
}

void PatternMatcher::clearHashMap() {
    for (auto i: ssIdHashMap_) {
        i.clear();
        std::vector<unsigned>().swap(i); // free memory
    }
    ssIdHashMap_.clear();
    std::vector<std::vector<unsigned> >().swap(ssIdHashMap_);  // free memory
}


// _____________________________________________________________________________
// Main driver
int PatternMatcher::run(TString out, TString src, TString bank) {
    gROOT->ProcessLine("#include <vector>");  // how is it not loaded?

    int exitcode = 0;
    Timing(1);

    exitcode = readPatterns(bank);
    if (exitcode)  return exitcode;
    Timing();

    exitcode = loadPatterns();
    if (exitcode)  return exitcode;
    Timing();

    exitcode = readFile(src);
    if (exitcode)  return exitcode;
    Timing();

    exitcode = makeRoads();
    if (exitcode)  return exitcode;
    Timing();

    exitcode = writeRoads(out);
    if (exitcode)  return exitcode;
    Timing();

    chain_->Reset();
    return exitcode;
}
