#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternGenerator.h"


static const unsigned MAX_TRIGGERTOWER = 6 * 8;

void PatternGenerator::uniquifyPatterns() {
    goodPatterns_.clear();

    PatternIdIndexMap patternIdMap;  // key: patternId, value: index in goodPatterns_
    for (unsigned i=0; i<allPatterns_.size(); ++i) {
        const Pattern& patt = allPatterns_.at(i);
        const Pattern::pattern8_t& pattId = patt.getPatternId();

        PatternIdIndexMap::const_iterator found = patternIdMap.find(pattId);
        if (found == patternIdMap.end()) {  // if not exist, insert the pattern
            goodPatterns_.push_back(patt);
            patternIdMap.insert(std::make_pair(pattId, goodPatterns_.size()-1) );

        } else {  // if exist, increase the frequency, update DC bit
            uint32_t index = found->second;
            goodPatterns_.at(index).touch();

            // Also update hit bit
            for (int l=0; l<nLayers_; ++l) {
                uint16_t hitBit = goodPatterns_.at(index).getHitBit(l);
                hitBit |= patt.getHitBit(l);
                goodPatterns_.at(index).setHitBit(l, hitBit);
            }
        }
    }
}


// _____________________________________________________________________________
// Read and parse the trigger tower csv file
int PatternGenerator::readTriggerTowerFile(TString src) {
    Pattern::vuint32_t values;
    if (src.EndsWith(".csv")) {
        if (verbose_)  std::cout << Info() << "Opening " << src << std::endl;

        unsigned i = 0;
        std::string line;
        ifstream ifs(src.Data());
        while (std::getline(ifs, line)) {
            std::istringstream iss(line);
            std::string issline;

            while (std::getline(iss, issline, ',')) {  // split by commas
                if (i != 0)  // skip the first line
                    values.push_back(std::stoi(issline));
            }
            ++i;
        }
    }

    if (values.empty()) {
        std::cout << Error() << "Failed to read the trigger tower file: " << src << std::endl;
        return 1;
    }

    uint32_t triggerTowerId = 0;
    for (unsigned i=0; i<values.size(); ++i) {
        if (i != 0) {  // skip the first index
            if (values.at(i-1) <= 6 && values.at(i) <= 8) {  // eta_idx, phi_idx
                triggerTowerId = values.at(i-1) * 10 + values.at(i);
                triggerTowerMap_.insert(std::make_pair(triggerTowerId, Pattern::vuint32_t()));
            } else if (values.at(i) > 10000) {
                triggerTowerMap_[triggerTowerId].push_back(values.at(i));
            }
        }
    }

    if (triggerTowerMap_.size() != MAX_TRIGGERTOWER) {
        std::cout << Error() << "Failed to get all trigger towers from the trigger tower file: " << src << std::endl;
        return 1;
    }
    return 0;
}

// _____________________________________________________________________________
// Read the input ntuples
int PatternGenerator::readFile(TString src) {
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
// Select one stub per layer, one hit per stub
int PatternGenerator::readAndFilterTree(TString out_tmp) {
    if (!out_tmp.EndsWith(".root")) {
        std::cout << Error() << "Output filename must be .root" << std::endl;
        return 1;
    }
    out_tmp.ReplaceAll(".root", "_tmp.root");

    // Do not do GetEntries() as it conflicts with CloneTree() when reading from XROOTD
    //Long64_t nentries = chain_->GetEntries();
    //if (nentries <= 0) {
    //    std::cout << Error() << "Input source has zero entry." << std::endl;
    //    return 1;
    //}
    //
    //if (nEvents_ > (int) nentries)
    //    nEvents_ = nentries;
    if (verbose_)  std::cout << Info() << "Reading " << nEvents_ << " events; recreating " << out_tmp << " after event filtering." << std::endl;

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

    // Set up TTreeFormula for event selection
    chain_->SetBranchStatus("genParts_pt"       , 1);
    chain_->SetBranchStatus("genParts_eta"      , 1);
    chain_->SetBranchStatus("genParts_phi"      , 1);
    chain_->SetBranchStatus("genParts_pz"       , 1);
    TTreeFormula* ttf_event = new TTreeFormula("ttf_event", eventSelect_, chain_);
    //ttf_event->SetQuickLoad(1);

    // For writing
    TFile* tfile = TFile::Open(out_tmp, "RECREATE");
    tfile->mkdir("ntupler")->cd();

    TTree* ttree = (TTree*) chain_->CloneTree(0); // Do not copy the data yet
    // The clone should not delete any shared i/o buffers.
    ResetDeleteBranches(ttree);


    // _________________________________________________________________________
    // Loop over all events
    Long64_t nPassed = 0;
    int curTree = chain_->GetTreeNumber();
    for (Long64_t ievt=0; ievt<nEvents_; ++ievt) {
        Long64_t local_entry = chain_->LoadTree(ievt);  // for TTreeFormula
        if (local_entry < 0)  break;
        if (chain_->GetTreeNumber() != curTree) {
            curTree = chain_->GetTreeNumber();
            ttf_event->UpdateFormulaLeaves();
        }
        chain_->GetEntry(ievt);

        unsigned nstubs = vb_modId->size();
        if (verbose_>1 && ievt%1000==0)  std::cout << Debug() << Form("... Processing event: %7lld, keeping: %7lld", ievt, nPassed) << std::endl;
        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " # stubs: " << nstubs << " [# patterns: " << allPatterns_.size() << "]" << std::endl;

        if (!nstubs) {  // skip if no stub
            if (!filter_) {  // keep all events regardless
                ++nPassed;
                ttree->Fill();
            }
            continue;
        }

        bool keep = true;

        // Check event selection
        int ndata = ttf_event->GetNdata();
        if (ndata && filter_)
            keep = (ttf_event->EvalInstance() != 0);

        // Check min # of layers
        bool require = ((int) nstubs >= nLayers_);
        if (!require && filter_)
            keep = false;

        // Check sim info
        // Take the sim-matched particle info from the outermost stub
        float simPt = vb_simPt->back();
        float simEta = vb_simEta->back();
        float simPhi = vb_simPhi->back();
        int trkId = vb_trkId->back();
        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " simPt: " << simPt << " simEta: " << simEta << " simPhi: " << simPhi << " trkId: " << trkId << std::endl;

        bool sim = (trkId == 1 &&
                    po.minPt  <= simPt  && simPt  <= po.maxPt  &&
                    po.minEta <= simEta && simEta <= po.maxEta &&
                    po.minPhi <= simPhi && simPhi <= po.maxPhi);
        if (!sim && filter_)
            keep = false;

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
        if (verbose_>2) {
            std::cout << Debug() << "... evt: " << ievt << " goodLayers: ";
            std::copy(goodLayers.begin(), goodLayers.end(), std::ostream_iterator<uint32_t>(std::cout, " "));
            std::cout << std::endl;
            std::cout << Debug() << "... evt: " << ievt << " goodLayerModules: ";
            std::copy(goodLayerModules.begin(), goodLayerModules.end(), std::ostream_iterator<uint32_t>(std::cout, " "));
            std::cout << std::endl;
        }

        // Check again min # of layers
        require = ((int) goodLayers.size() >= nLayers_);
        if (!require && filter_)
            keep = false;

        // Find layers with at least one sim-matched hit
        // If more than one sim-matched hit in a layer, take only the first hit
        unsigned ngoodstubs = 0;
        for (unsigned l=0; (l<nstubs) && keep; ++l) {
            bool keepstub = true;

            unsigned moduleId = vb_modId->at(l);
            unsigned nhits = vb_nhits->at(l);
            assert(nhits > 0);
            if (verbose_>2)  std::cout << Debug() << "... ... stub: " << l << " moduleId: " << moduleId << " trkId: " << vb_trkId->at(l) << " # hits: " << nhits << std::endl;

            // Check module id
            unsigned count = std::count(goodLayerModules.begin(), goodLayerModules.end(), moduleId);
            if (!count)  // do not keep even if filter_ is false
                keepstub = false;
            else  // keep and invalidate the next stub with identical moduleId
                goodLayerModules.erase(std::remove(goodLayerModules.begin(), goodLayerModules.end(), moduleId), goodLayerModules.end());

            // Check sim info of stub
            sim = (vb_trkId->at(l) == trkId);
            if (!sim && filter_)
                keepstub = false;

            for (unsigned m=0; (m<nhits) && keepstub; ++m) {
                bool keephit = true;

                if (verbose_>2)  std::cout << Debug() << "... ... ... hit: " << m << " col: " << vb_hitCols->at(l).at(m) << " row: " << vb_hitRows->at(l).at(m) << " trkId: " << vb_hitTrkIds->at(l).at(m) << std::endl;

                // Check sim info of hit
                sim = (vb_hitTrkIds->at(l).at(m) == trkId);
                if (!sim && filter_)
                    keephit = false;

                if (keephit) {  // Only need one hit
                    // Keep and move it to be the first element
                    vb_hitCols->at(l).at(0)    = vb_hitCols->at(l).at(m);
                    vb_hitRows->at(l).at(0)    = vb_hitRows->at(l).at(m);
                    vb_hitTrkIds->at(l).at(0)  = vb_hitTrkIds->at(l).at(m);
                    break;
                }
            }
            // Only need one hit
            vb_hitCols->at(l)  .resize(1);
            vb_hitRows->at(l)  .resize(1);
            vb_hitTrkIds->at(l).resize(1);
            vb_nhits->at(l)    = 1;

            // Check sim info of hit
            if (!sim && filter_)
                keepstub = false;

            if (keepstub) {
                // Keep and move it to be the 'ngoodstubs' element
                vb_iModCols->at(ngoodstubs)   = vb_iModCols->at(l);
                vb_iModRows->at(ngoodstubs)   = vb_iModRows->at(l);
                vb_modId->at(ngoodstubs)      = vb_modId->at(l);
                vb_nhits->at(ngoodstubs)      = vb_nhits->at(l);
                vb_hitCols->at(ngoodstubs)    = vb_hitCols->at(l);
                vb_hitRows->at(ngoodstubs)    = vb_hitRows->at(l);
                vb_hitTrkIds->at(ngoodstubs)  = vb_hitTrkIds->at(l);
                vb_simPt->at(ngoodstubs)      = vb_simPt->at(l);
                vb_simEta->at(ngoodstubs)     = vb_simEta->at(l);
                vb_simPhi->at(ngoodstubs)     = vb_simPhi->at(l);
                vb_trkId->at(ngoodstubs)      = vb_trkId->at(l);
                ++ngoodstubs;
            }
            if (verbose_>2)  std::cout << Debug() << "... ... stub: " << l << " keep? " << keepstub << std::endl;
        }

        // Check again min # of layers
        require = ((int) ngoodstubs >= nLayers_);
        if (!require && filter_)
            keep = false;
        if (!keep)  //  do not keep any stub
            ngoodstubs = 0;

        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " keep? " << keep << std::endl;

        vb_iModCols ->resize(ngoodstubs);
        vb_iModRows ->resize(ngoodstubs);
        vb_modId    ->resize(ngoodstubs);
        vb_nhits    ->resize(ngoodstubs);
        vb_hitCols  ->resize(ngoodstubs);
        vb_hitRows  ->resize(ngoodstubs);
        vb_hitTrkIds->resize(ngoodstubs);
        vb_simPt    ->resize(ngoodstubs);
        vb_simEta   ->resize(ngoodstubs);
        vb_simPhi   ->resize(ngoodstubs);
        vb_trkId    ->resize(ngoodstubs);

        ++nPassed;
        ttree->Fill();
    }
    if (verbose_)  std::cout << Info() << "Processed " << nEvents_ << " events, kept " << nPassed << " events." << std::endl;

    assert(ttree->GetEntries() == nPassed);
    tfile->Write();
    delete ttf_event;
    delete ttree;
    delete tfile;
    return 0;
}

// Testing
int PatternGenerator::readAndFilterTree2(TString out_tmp) {
    Long64_t nentries = chain_->GetEntries();

    for (Long64_t ievt=0; ievt<nentries; ++ievt) {
        // do something
    }

    return 0;
}


// _____________________________________________________________________________
// Make the patterns
int PatternGenerator::makeTree(TString out_tmp) {
    if (!out_tmp.EndsWith(".root")) {
        std::cout << Error() << "Output filename must be .root" << std::endl;
        return 1;
    }

    out_tmp.ReplaceAll(".root", "_tmp.root");
    chain_->Add(out_tmp);
    Long64_t nentries = chain_->GetEntries();
    assert(nentries > 0);

    if (nEvents_ > (int) nentries)
        nEvents_ = nentries;
    if (verbose_)  std::cout << Info() << "Reading " << nEvents_ << " events from " << out_tmp << std::endl;

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

    allPatterns_.clear();
    if (nEvents_ >= (Long64_t) allPatterns_.max_size()) {
        std::cout << Error() << "Number of events is more than the max_size of a std::vector." << std::endl;
        return 1;
    }


    // _________________________________________________________________________
    // Loop over all events
    Long64_t nPassed = 0;
    for (Long64_t ievt=0; ievt<nEvents_; ++ievt) {
        chain_->GetEntry(ievt);
        unsigned nstubs = vb_modId->size();
        if (verbose_>1 && ievt%1000==0)  std::cout << Debug() << Form("... Processing event: %7lld, keeping: %7lld", ievt, nPassed) << std::endl;
        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " # stubs: " << nstubs << " [# patterns: " << allPatterns_.size() << "]" << std::endl;

        if (!nstubs) {  // skip if no stub
            continue;
        }

        // Loop over reconstructed stubs
        Pattern::vuint64_t goodHitIds;
        Pattern::vuint16_t goodHitBits;
        for (unsigned l=0; l<nstubs; ++l) {
            unsigned moduleId = vb_modId->at(l);
            unsigned ncols = vb_iModCols->at(l);
            unsigned nrows = vb_iModRows->at(l);
            unsigned nhits = vb_nhits->at(l);
            assert(nhits == 1);
            int col = vb_hitCols->at(l).at(0);
            int row = vb_hitRows->at(l).at(0);

            // Set to lower resolution according to nSubLadders and nSubModules
            col /= (ncols / std::min((int) ncols, po.nSubLadders));
            row /= (nrows / std::min((int) nrows, po.nSubModules));
            uint64_t hitId = Pattern::encodeHitId(moduleId, col, row);
            uint16_t hitBit = 1 << 0;

            // Use DCBits
            Pattern::encodeDCBit(nDCBits_, hitId, hitBit);

            if (verbose_>2)  std::cout << Debug() << "... ... stub: " << l << " moduleId: " << moduleId << " trkId: " << vb_trkId->at(l) << " # hits: " << nhits << std::endl;
            if (verbose_>2)  std::cout << Debug() << "... ... stub: " << l << " #0 hitId: " << hitId << " hitBit: " << hitBit << " col: " << col << " row: " << row << " trkId: " << vb_hitTrkIds->at(l).at(0) << std::endl;

            goodHitIds.push_back(hitId);
            goodHitBits.push_back(hitBit);
        }
        if (verbose_>2) {
            std::cout << Debug() << "... evt: " << ievt << " # goodHitIds: " << goodHitIds.size() << " {";
            std::copy(goodHitIds.begin(), goodHitIds.end(), std::ostream_iterator<uint64_t>(std::cout, " "));
            std::cout << "}" << std::endl;
        }


        // Initialize a pattern
        uint32_t hash = hashFileEvent(chain_->GetCurrentFile()->GetName(), v_event);  // FIXME: currently not working, will switch to use generator seed instead
        Pattern patt(nLayers_, nDCBits_, hash);

        // Only consider up to the first nLayers_
        for (int l=0; l<nLayers_; ++l) {
            if (l==(int) goodHitIds.size())  break;  // when filter_ is false, goodHitIds.size() can be less than nLayers_
            patt.setHitId(l, goodHitIds.at(l));
            patt.setHitBit(l, goodHitBits.at(l));
        }

        patt.ready();

        for (int l=0; l<nLayers_; ++l) {
            uint64_t hitId = patt.getHitId(l);
            uint16_t hitBit = patt.getHitBit(l);
            if (hitId == 0)  continue;  // avoid zero
            assert(hitBit > 0);

            //HitIdBoolMap::const_iterator found = hitIdMap_.find(hitId);
            HitIdShortMap::const_iterator found = hitIdMap_.find(hitId);
            if (found == hitIdMap_.end()) {  // if not exist, insert the hitId
                //std::shared_ptr<bool> abool(new bool(false));
                //hitIdMap_.insert(std::make_pair(hitId, abool) );

                std::shared_ptr<uint16_t> ashort(new uint16_t(0));
                hitIdMap_.insert(std::make_pair(hitId, ashort) );
            }

            if (verbose_>2)  std::cout << Debug() << "... ... bin: " << l << " hitId: " << hitId << " hitBit: " << hitBit << " # hitIds: " << hitIdMap_.size() << std::endl;
            if (verbose_>2)  std::cout << Debug() << "... ... bin: " << l << " lay: " << Pattern::decodeHitLayer(hitId) << " lad: " << Pattern::decodeHitLadder(hitId) << " mod: " << Pattern::decodeHitModule(hitId) << " col: " << Pattern::decodeHitSubLadder(hitId) << " row: " << Pattern::decodeHitSubModule(hitId) << std::endl;
        }

        ++nPassed;
        allPatterns_.push_back(patt);  // save the patterns
    }

    // Keep only unique patterns
    uniquifyPatterns();  // do this after allPatterns_ is populated
    for (unsigned i=0; i<goodPatterns_.size(); ++i) {
        const Pattern& patt = goodPatterns_.at(i);
        const Pattern::pattern8_t& pattId = patt.getPatternId();
        if (verbose_>2) {
            std::cout << Debug() << "... patt: " << i << " ";
            std::copy(pattId.begin(), pattId.end(), std::ostream_iterator<uint64_t>(std::cout, " "));
            std::cout << " freq: " << patt.frequency() << std::endl;
        }
    }

    if (verbose_>1)  std::cout << Debug() << "Identified " << hitIdMap_.size() << " possible 'hitId's." << std::endl;
    if (verbose_)  std::cout << Info() << "Made " << goodPatterns_.size() << " final patterns, out of " << allPatterns_.size() << " inclusive, full-resolution patterns." << std::endl;

    std::sort(goodPatterns_.begin(), goodPatterns_.end(), sortByFrequency);
    return 0;
}

// _____________________________________________________________________________
// Output patterns into a TTree
int PatternGenerator::writeTree(TString out) {
    if (!out.EndsWith(".root")) {
        std::cout << Error() << "Output filename must be .root" << std::endl;
        return 1;
    }

    if (nPatterns_ > (int) goodPatterns_.size())
        nPatterns_ = goodPatterns_.size();
    if (verbose_)  std::cout << Info() << "Recreating " << out << " with " << nPatterns_ << " patterns." << std::endl;
    TFile* tfile = TFile::Open(out, "RECREATE");
    TTree* ttree = new TTree("patternBank", "");

    uint32_t nLayers = nLayers_;
    uint32_t nDCBits = nDCBits_;
    uint32_t hash;
    uint32_t frequency;
    uint64_t pattern[nLayers_];
    uint16_t bit[nLayers_];

    ttree->Branch("nLayers", &nLayers, "nLayers/i");            // i for UInt_t
    ttree->Branch("nDCBits", &nDCBits, "nDCBits/i");            // i for UInt_t
    ttree->Branch("hash", &hash, "hash/i");                     // i for UInt_t
    ttree->Branch("frequency", &frequency, "frequency/i");      // i for UInt_t
    ttree->Branch("pattern", pattern, "pattern[nLayers]/l");    // l for ULong64_t
    ttree->Branch("bit", bit, "bit[nLayers]/s");                // s for UShort_t

    // _________________________________________________________________________
    // Loop over good patterns
    for (int ievt=0; ievt<nPatterns_; ++ievt) {
        if (verbose_>1 && ievt%10000==0)  std::cout << Debug() << Form("... Writing event: %7i", ievt) << std::endl;

        const Pattern& patt = goodPatterns_.at(ievt);
        for (unsigned l=0; l<nLayers; ++l) {
            pattern[l] = patt.getHitId(l);
            bit[l] = patt.getHitBit(l);
        }
        hash = patt.hash();
        frequency = patt.frequency();

        ttree->Fill();
    }
    assert(ttree->GetEntries() == nPatterns_);

    TTree* ttree2 = new TTree("triggerTower", "");
    std::map<uint32_t, Pattern::vuint32_t> * ptr_triggerTowerMap_ = &triggerTowerMap_;
    ttree2->Branch("triggerTowerMap", ptr_triggerTowerMap_);
    ttree2->Fill();
    assert(ttree2->GetEntries() == 1);

    tfile->Write();
    delete ttree;
    delete ttree2;
    delete tfile;
    return 0;
}

// _____________________________________________________________________________
// Main driver
int PatternGenerator::run(TString src, TString out, TString layout) {
    int exitcode = 0;
    Timing(1);

    exitcode = readTriggerTowerFile(layout);
    if (exitcode)  return exitcode;
    Timing();

    exitcode = readFile(src);
    if (exitcode)  return exitcode;
    Timing();

    exitcode = readAndFilterTree(out);
    //exitcode = readAndFilterTree2(out);
    if (exitcode)  return exitcode;
    Timing();

    if (verbose_)  std::cout << Info() << "Closing input source." << std::endl;
    chain_->Reset();
    exitcode = makeTree(out);
    if (exitcode)  return exitcode;
    Timing();

    exitcode = writeTree(out);
    if (exitcode)  return exitcode;
    Timing();

    chain_->Reset();
    return exitcode;
}
