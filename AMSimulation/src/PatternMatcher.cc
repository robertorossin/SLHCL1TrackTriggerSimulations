#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternMatcher.h"


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
        typedef unsigned short unsigned16;
        //typedef unsigned long long unsigned64;
        typedef ULong64_t unsigned64;
        unsigned hash;
        unsigned frequency;
        std::vector<unsigned64> * superstripIds = 0;
        std::vector<unsigned16> * superstripBits = 0;

        ttree->SetBranchAddress("hash", &hash);
        ttree->SetBranchAddress("frequency", &frequency);
        ttree->SetBranchAddress("superstripIds", &superstripIds);
        ttree->SetBranchAddress("superstripBits", &superstripBits);

        Long64_t nentries = ttree->GetEntries();
        if (verbose_)  std::cout << Info() << "Reading " << nentries << " patterns." << std::endl;

        // _____________________________________________________________________
        // Loop over all patterns
        for (Long64_t ievt=0; ievt<nentries; ++ievt) {
            ttree->GetEntry(ievt);
            assert(superstripIds->size() == superstripBits->size());
            unsigned nsuperstrips = superstripIds->size();

            std::vector<TTSuperstrip> superstrips;
            for (unsigned i=0; i<nsuperstrips; ++i) {
                TTSuperstrip ss(superstripIds->at(i), superstripBits->at(i));
                superstrips.push_back(ss);
            }

            TTPattern patt(hash, superstrips);
            patterns_.push_back(patt);
        }

        if ((int) patterns_.size() != nentries) {
            std::cout << Error() << "Failed to read all patterns from the root file: " << src << std::endl;
            return 1;
        }

        delete ttree;
        delete tfile;
    }

    if (patterns_.empty()) {
        std::cout << Error() << "Pattern bank has zero entry." << std::endl;
        return 1;
    }
    return 0;
}


// _____________________________________________________________________________
int PatternMatcher::loadPatterns() {
    unsigned nentries = patterns_.size();
    assert(nentries>0);

    // _________________________________________________________________________
    // Loop over all patterns
    for (unsigned ievt=0; ievt<nentries; ++ievt) {
        const TTPattern& patt = patterns_.at(ievt);

        for (int l=0; l<nLayers_; ++l) {
            addr_type ssId = patt.getSuperstripId(l);
            bit_type ssBit = patt.getSuperstripBit(l);
            if (ssId == 0)  continue;  // avoid zero

            // Using unordered_multimap, so collision is ok
            bit_n_index_pair apair(ssBit, ievt);
            ssIdMapFast_.insert(std::make_pair(ssId, apair) );
        }
        if (verbose_>2) {
            std::cout << Debug() << "... patt: " << ievt << " " << patt << std::endl;
        }
    }

    if (verbose_>1)  std::cout << Debug() << "Identified " << ssIdMapFast_.size() << " possible 'ssId's." << std::endl;
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
    unsigned                            v_event           = 0;

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
    chain_->SetBranchStatus("event"            , 1);

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
    chain_->SetBranchAddress("event"            , &(v_event));


    // _________________________________________________________________________
    // Loop over all events
    allRoads_.clear();
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

        ////////////////////////////////////////////////////////////////////////
        // This block is used to reduce timing -- commented out for now as I'm not sure how to deal with fake superstrips
        //bool keep = true;
        //
        //// Build a list of goodLayers that is unique
        //std::vector<id_type> goodLayers;  // stores mlayer, not layer!
        //for (unsigned l=0; (l<nstubs) && keep; ++l) {
        //    unsigned moduleId = vb_modId->at(l);
        //    id_type layer = decodeLayer(moduleId);
        //    id_type mlayer = layerMap_.at(layer);
        //    goodLayers.push_back(mlayer);
        //}
        //std::sort(goodLayers.begin(), goodLayers.end());
        //goodLayers.erase(std::unique(goodLayers.begin(), goodLayers.end()), goodLayers.end() );
        //
        //// Check again min # of layers
        //// Add '-2' to account for possible fake superstrips in the pattern
        //if ((int) goodLayers.size() < nLayers_ - po.nMisses - 2) {
        //    allRoads_.push_back(std::vector<TTRoad>());
        //    continue;
        //}
        ////////////////////////////////////////////////////////////////////////

        if (patterns_.empty() || ssIdMapFast_.empty()) {
            std::cout << Error() << "Patterns are not yet loaded!" << std::endl;
            return 1;
        }

        std::map<unsigned, std::vector<TTHit> >  foundPatternMap;  // key: index of pattern in patterns_

        // Loop over reconstructed stubs
        for (unsigned l=0; l<nstubs; ++l) {
            unsigned moduleId = vb_modId->at(l);
            unsigned ncols = vb_iModCols->at(l);
            unsigned nrows = vb_iModRows->at(l);
            unsigned nhits = vb_nhits->at(l);
            assert(nhits > 0);
            int col = 0;
            int row = 0;

            float x = vb_x->at(l);  // FIXME: change to hit position
            float y = vb_y->at(l);
            float z = vb_z->at(l);
            float pt = vb_roughPt->at(l);  // can we calculate rough pt for each hit?

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

                //x = vb_hitXs->at(l).at(m);
                //y = vb_hitYs->at(l).at(m);
                //z = vb_hitZs->at(l).at(m);

                auto found = ssIdMapFast_.equal_range(ssId);  // find by superstrip id
                for (auto it=found.first; it!=found.second; ++it) {  // loop over all found superstrips in the ssIdMapFast_
                    if (ssBit & it->second.first) {  // match the hitBit

                        auto found2 = foundPatternMap.find(it->second.second);  // find by index of pattern in pattern_
                        if (found2 != foundPatternMap.end()) {  // found the pattern number in the foundPatternMap
                            std::vector<TTHit>& hits = found2->second;
                            TTHit hit(x, y, z, pt, ssId);
                            hits.push_back(hit);

                        } else {
                            std::vector<TTHit> hits;
                            TTHit hit(x, y, z, pt, ssId);
                            hits.push_back(hit);

                            foundPatternMap.insert(std::make_pair(it->second.second, hits));
                        }
                    }
                }
                if (verbose_>2)  std::cout << Debug() << "... ... ... hit: " << m << " ssId: " << ssId << " ssBit: " << ssBit << " col: " << col << " row: " << row << " trkId: " << vb_hitTrkIds->at(l).at(m) << std::endl;
            }
        }

        std::vector<TTRoad> roadsInThisEvent;
        if (! foundPatternMap.empty()) {
            for (auto it: foundPatternMap) {
                pattern_type pattId = patterns_.at(it.first).id();
                int nFakeSuperstrips = std::count(pattId.begin(), pattId.end(), encodeFakeSuperstripId());

                if ((int) it.second.size() >= (nLayers_ - nFakeSuperstrips - po.nMisses)) {  // if number of hits more than the requirement
                    unsigned hash = hashThisEvent(v_event, vb_simPhi->back());  // FIXME: will switch to use generator seed instead
                    const std::vector<TTHit>& hits = it.second;

                    TTRoad road(hash, pattId, hits);
                    roadsInThisEvent.push_back(road);
                }
            }

            if (! roadsInThisEvent.empty())
                ++nPassed;
        }
        allRoads_.push_back(roadsInThisEvent);
    }

    if (verbose_)  std::cout << Info() << "Processed " << nEvents_ << " events, triggered on " << nPassed << " events." << std::endl;

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

    //typedef unsigned long long unsigned64;
    typedef ULong64_t unsigned64;
    std::auto_ptr<std::vector<unsigned> >                 vr_hash            (new std::vector<unsigned>());
    std::auto_ptr<std::vector<std::vector<unsigned64> > > vr_patternIds      (new std::vector<std::vector<unsigned64> >());
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitXs           (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitYs           (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitZs           (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitXErrors      (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitYErrors      (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitZErrors      (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<int> > >        vr_hitCharges      (new std::vector<std::vector<int> >());
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitPts          (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<unsigned64> > > vr_hitSuperstripIds(new std::vector<std::vector<unsigned64> >());

    ttree->Branch(prefixRoad_ + "hash"             + suffix_, &(*vr_hash));
    ttree->Branch(prefixRoad_ + "patternIds"       + suffix_, &(*vr_patternIds));
    ttree->Branch(prefixRoad_ + "hitXs"            + suffix_, &(*vr_hitXs));
    ttree->Branch(prefixRoad_ + "hitYs"            + suffix_, &(*vr_hitYs));
    ttree->Branch(prefixRoad_ + "hitZs"            + suffix_, &(*vr_hitZs));
    ttree->Branch(prefixRoad_ + "hitXErrors"       + suffix_, &(*vr_hitXErrors));
    ttree->Branch(prefixRoad_ + "hitYErrors"       + suffix_, &(*vr_hitYErrors));
    ttree->Branch(prefixRoad_ + "hitZErrors"       + suffix_, &(*vr_hitZErrors));
    ttree->Branch(prefixRoad_ + "hitCharges"       + suffix_, &(*vr_hitCharges));
    ttree->Branch(prefixRoad_ + "hitPts"           + suffix_, &(*vr_hitPts));
    ttree->Branch(prefixRoad_ + "hitSuperstripIds" + suffix_, &(*vr_hitSuperstripIds));

    // _________________________________________________________________________
    // Loop over all roads
    for (unsigned ievt=0; ievt<nentries; ++ievt) {
        if (verbose_>1 && ievt%10000==0)  std::cout << Debug() << Form("... Writing event: %7u", ievt) << std::endl;
        vr_hash            ->clear();
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

        const std::vector<TTRoad>& roadsInThisEvent = allRoads_.at(ievt);
        unsigned nroads = roadsInThisEvent.size();

        for (unsigned i=0; i<nroads; ++i) {
            if ((int) i >= maxRoads_)  break;

            std::vector<unsigned64> patternIds;
            std::vector<float>      hitXs;
            std::vector<float>      hitYs;
            std::vector<float>      hitZs;
            std::vector<float>      hitXErrors;
            std::vector<float>      hitYErrors;
            std::vector<float>      hitZErrors;
            std::vector<int>        hitCharges;
            std::vector<float>      hitPts;
            std::vector<unsigned64> hitSuperstripIds;

            const TTRoad& road = roadsInThisEvent.at(i);
            unsigned                  hash   = road.hash();
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
            }

            vr_hash            ->push_back(hash);
            vr_patternIds      ->push_back(patternIds);
            vr_hitXs           ->push_back(hitXs);
            vr_hitYs           ->push_back(hitYs);
            vr_hitZs           ->push_back(hitZs);
            vr_hitXErrors      ->push_back(hitXErrors);
            vr_hitYErrors      ->push_back(hitYErrors);
            vr_hitZErrors      ->push_back(hitZErrors);
            vr_hitCharges      ->push_back(hitCharges);
            vr_hitPts          ->push_back(hitPts);
            vr_hitSuperstripIds->push_back(hitSuperstripIds);
        }

        ttree->Fill();
        assert(vr_hash->size() == allRoads_.at(ievt).size());
    }
    assert(ttree->GetEntries() == (int) allRoads_.size());

    tfile->Write();
    delete ttree;
    delete tfile;
    return 0;
}


// _____________________________________________________________________________
// Private functions

// Make a map to merge layers in barrel and in endcap
void PatternMatcher::makeLayerMap() {
    // Hardcoded layer information
    if (nLayers_ <= 6) {
        layerMap_ = std::map<id_type, id_type> {
            {5,5}, {6,6}, {7,7}, {8,8}, {9,9}, {10,10},
            {11,11}, {12,10}, {13,9}, {14,8}, {15,7},
            {18,11}, {19,10}, {20,9}, {21,8}, {22,7}
        };
    } else {  // otherwise it's not merged
        layerMap_ = std::map<id_type, id_type> {
            {5,5}, {6,6}, {7,7}, {8,8}, {9,9}, {10,10},
            {11,11}, {12,12}, {13,13}, {14,14}, {15,15},
            {18,11}, {19,12}, {20,13}, {21,14}, {22,15}
        };
    };
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
