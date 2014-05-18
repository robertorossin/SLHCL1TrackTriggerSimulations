#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternGenerator.h"


static const unsigned MAX_NLAYERS = 8;
static const unsigned MAX_NTOWERS = 6 * 8;


// _____________________________________________________________________________
// Read and parse the trigger tower csv file
int PatternGenerator::readTriggerTowerFile(TString src) {
    std::vector<unsigned> values;
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

    unsigned triggerTowerId = 0;
    for (unsigned i=0; i<values.size(); ++i) {
        if (i != 0) {  // skip the first index
            if (values.at(i-1) <= 6 && values.at(i) <= 8) {  // eta_idx, phi_idx
                triggerTowerId = values.at(i-1) * 10 + values.at(i);
                triggerTowerMap_.insert(std::make_pair(triggerTowerId, std::vector<unsigned>()));
            } else if (values.at(i) > 10000) {
                triggerTowerMap_[triggerTowerId].push_back(values.at(i));
            }
        }
    }

    if (triggerTowerMap_.size() != MAX_NTOWERS) {
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

    } else if (src.EndsWith(".txt")) {
        TFileCollection* fc = new TFileCollection("fileinfolist", "", src);
        if (chain_->AddFileInfoList((TCollection*) fc->GetList()) )  // 1 if successful, 0 otherwise
            return 0;
    }

    std::cout << Error() << "Input source should be either a .root file or a .txt file." << std::endl;
    return 1;
}


// _____________________________________________________________________________
// Select one stub per layer, one hit per stub.
// It is not supposed to throw away any event yet.
int PatternGenerator::filterHits(TString out_tmp) {
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
        Long64_t local_entry = chain_->LoadTree(ievt);  // for TChain
        if (local_entry < 0)  break;
        if (chain_->GetTreeNumber() != curTree) {  // for TTreeFormula
            curTree = chain_->GetTreeNumber();
            ttf_event->UpdateFormulaLeaves();
        }
        chain_->GetEntry(ievt);

        unsigned nstubs = vb_modId->size();
        if (verbose_>1 && ievt%1000==0)  std::cout << Debug() << Form("... Processing event: %7lld, keeping: %7lld", ievt, nPassed) << std::endl;
        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " # stubs: " << nstubs << " [# patterns: " << allPatterns_.size() << "]" << std::endl;

        if (!nstubs) {  // skip if no stub
            ++nPassed;
            ttree->Fill();
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
        std::vector<id_type> goodLayers;  // NOTE: stores mlayer, not layer
        std::vector<id_type> goodLayerModules;
        for (unsigned l=0; (l<nstubs) && keep; ++l) {
            unsigned ll = (nstubs-1) - l;  // reverse iteration order
            unsigned moduleId = vb_modId->at(ll);
            id_type layer = decodeLayer(moduleId);
            id_type mlayer = layerMap_.at(layer);  // mapping of layer --> mlayer must exist

            unsigned count = std::count(goodLayers.begin(), goodLayers.end(), mlayer);
            if (!count) {  // no module with mlayer
                goodLayers.push_back(mlayer);
                goodLayerModules.push_back(moduleId);
            }
        }
        if (verbose_>2) {
            std::cout << Debug() << "... evt: " << ievt << " # goodLayers: " << goodLayers.size() << " {";
            std::copy(goodLayers.begin(), goodLayers.end(), std::ostream_iterator<id_type>(std::cout, " "));
            std::cout << "}" << std::endl;
            std::cout << Debug() << "... evt: " << ievt << " # goodLayerModules: " << goodLayerModules.size() << " {";
            std::copy(goodLayerModules.begin(), goodLayerModules.end(), std::ostream_iterator<id_type>(std::cout, " "));
            std::cout << "}" << std::endl;
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

                if (keephit) {
                    // Keep and move it to be the first element
                    vb_hitCols->at(l).at(0)    = vb_hitCols->at(l).at(m);
                    vb_hitRows->at(l).at(0)    = vb_hitRows->at(l).at(m);
                    vb_hitTrkIds->at(l).at(0)  = vb_hitTrkIds->at(l).at(m);
                    break;  // Only need one hit
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

    if (verbose_)  std::cout << Info() << "Replacing input source." << std::endl;
    chain_->Reset();
    chain_->Add(out_tmp);

    return 0;
}


// _____________________________________________________________________________
// Make the patterns
int PatternGenerator::makePatterns() {
    Long64_t nentries = chain_->GetEntries();
    assert(nentries > 0);

    if (nEvents_ > (int) nentries)
        nEvents_ = nentries;
    if (verbose_)  std::cout << Info() << "Reading " << nEvents_ << " events" << std::endl;

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
        std::vector<TTSuperstrip> goodSuperstrips;
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
            addr_type ssId = encodeSuperstripId(moduleId, col, row);
            bit_type ssBit = 1 << 0;

            // Use DCBit
            encodeDCBit(nDCBits_, ssId, ssBit);

            // Create a superstrip
            TTSuperstrip ss(ssId, ssBit);
            goodSuperstrips.push_back(ss);

            if (verbose_>2)  std::cout << Debug() << "... ... stub: " << l << " moduleId: " << moduleId << " trkId: " << vb_trkId->at(l) << " # hits: " << nhits << std::endl;
            if (verbose_>2)  std::cout << Debug() << "... ... stub: " << l << " ssId: " << ssId << " ssBit: " << ssBit << " col: " << col << " row: " << row << " trkId: " << vb_hitTrkIds->at(l).at(0) << std::endl;
        }
        if (verbose_>2) {
            std::cout << Debug() << "... evt: " << ievt << " # goodSuperstrips: " << goodSuperstrips.size() << " {";
            std::copy(goodSuperstrips.begin(), goodSuperstrips.end(), std::ostream_iterator<TTSuperstrip>(std::cout, " "));
            std::cout << "}" << std::endl;
        }


        // Create a pattern
        unsigned hash = hashThisEvent(v_event, vb_simPhi->back());  // FIXME: will switch to use generator seed instead
        goodSuperstrips.resize(nLayers_);
        TTPattern patt(hash, goodSuperstrips);
        allPatterns_.push_back(patt);  // save the pattern

        ++nPassed;
    }

    // Keep only unique patterns
    uniquifyPatterns();  // do this after allPatterns_ is populated
    for (unsigned i=0; i<goodPatterns_.size(); ++i) {
        const TTPattern& patt = goodPatterns_.at(i);
        if (verbose_>2) {
            std::cout << Debug() << "... patt: " << i << " " << patt << std::endl;
        }
    }

    if (verbose_)  std::cout << Info() << "Made " << goodPatterns_.size() << " final patterns, out of " << allPatterns_.size() << " inclusive, full-resolution patterns." << std::endl;

    std::sort(goodPatterns_.begin(), goodPatterns_.end(), sortByFrequency);
    return 0;
}


// _____________________________________________________________________________
// Output patterns into a TTree
int PatternGenerator::writePatterns(TString out) {
    if (!out.EndsWith(".root")) {
        std::cout << Error() << "Output filename must be .root" << std::endl;
        return 1;
    }

    if (maxPatterns_ > (int) goodPatterns_.size())
        maxPatterns_ = goodPatterns_.size();
    if (verbose_)  std::cout << Info() << "Recreating " << out << " with " << maxPatterns_ << " patterns." << std::endl;
    TFile* tfile = TFile::Open(out, "RECREATE");
    TTree* ttree = new TTree("patternBank", "");

    typedef unsigned short unsigned16;
    //typedef unsigned long long unsigned64;
    typedef ULong64_t unsigned64;
    std::auto_ptr<unsigned>                   hash            (new unsigned(0));
    std::auto_ptr<unsigned>                   frequency       (new unsigned(0));
    std::auto_ptr<std::vector<unsigned64> >   superstripIds   (new std::vector<unsigned64>());
    std::auto_ptr<std::vector<unsigned16> >   superstripBits  (new std::vector<unsigned16>());

    ttree->Branch("hash"            , &(*hash));
    ttree->Branch("frequency"       , &(*frequency));
    ttree->Branch("superstripIds"   , &(*superstripIds));
    ttree->Branch("superstripBits"  , &(*superstripBits));

    // _________________________________________________________________________
    // Loop over good patterns
    for (int ievt=0; ievt<maxPatterns_; ++ievt) {
        if (verbose_>1 && ievt%10000==0)  std::cout << Debug() << Form("... Writing event: %7i", ievt) << std::endl;

        const TTPattern& patt = goodPatterns_.at(ievt);
        *hash = patt.hash();
        *frequency = patt.frequency();

        superstripIds->clear();
        superstripBits->clear();
        const std::vector<TTSuperstrip> superstrips = patt.getSuperstrips();
        for (unsigned i=0; i<superstrips.size(); ++i) {
            const TTSuperstrip& superstrip = superstrips.at(i);
            superstripIds->push_back(superstrip.id());
            superstripBits->push_back(superstrip.bit());
        }

        ttree->Fill();
    }
    assert(ttree->GetEntries() == maxPatterns_);

    // Also save the triggerTower
    TTree* ttree2 = new TTree("triggerTower", "");
    std::map<unsigned, std::vector<unsigned> > * ptr_triggerTowerMap_ = &triggerTowerMap_;
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
// Private functions

// Make a map to merge layers in barrel and in endcap
void PatternGenerator::makeLayerMap() {
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

// Make unique patterns from 'allPatterns_' and put them into 'goodPatterns_'
void PatternGenerator::uniquifyPatterns() {
    assert(!allPatterns_.empty());
    goodPatterns_.clear();

    std::map<pattern_type, unsigned> patternIdMap;  // key: patternId, value: index in goodPatterns_
    for (unsigned i=0; i<allPatterns_.size(); ++i) {
        const TTPattern& patt = allPatterns_.at(i);
        const pattern_type& pattId = patt.id();

        auto found = patternIdMap.find(pattId);
        if (found == patternIdMap.end()) {  // if not exist, insert the pattern
            goodPatterns_.push_back(patt);
            patternIdMap.insert(std::make_pair(pattId, goodPatterns_.size()-1) );

        } else {
            // If exist, merge them
            // merge(...) will update the DC bit and increase the frequency
            unsigned index = found->second;
            goodPatterns_.at(index).merge(patt);
        }
    }
    //allPatterns_.clear();
}


// _____________________________________________________________________________
// Main driver
int PatternGenerator::run(TString out, TString src, TString layout) {
    gROOT->ProcessLine("#include <vector>");  // how is it not loaded?

    int exitcode = 0;
    Timing(1);

    exitcode = readTriggerTowerFile(layout);
    if (exitcode)  return exitcode;
    Timing();

    exitcode = readFile(src);
    if (exitcode)  return exitcode;
    Timing();

    exitcode = filterHits(out);
    if (exitcode)  return exitcode;
    Timing();

    exitcode = makePatterns();
    if (exitcode)  return exitcode;
    Timing();

    exitcode = writePatterns(out);
    if (exitcode)  return exitcode;
    Timing();

    chain_->Reset();
    return exitcode;
}
