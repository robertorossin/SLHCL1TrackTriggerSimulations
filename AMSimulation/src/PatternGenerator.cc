#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternGenerator.h"

#ifndef ROOT_Math_GenVector_eta
#include "Math/GenVector/eta.h"
#endif

static const unsigned MAX_NTOWERS = 6 * 8;
static const unsigned MIN_NLAYERS = 3;


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

    triggerTowerMap_.clear();
    unsigned triggerTowerId = 0;
    for (unsigned i=0; i<values.size(); ++i) {
        if (i == 0)  continue;  // skip the first index
        if (values.at(i-1) <= 6 && values.at(i) <= 8) {  // eta_idx, phi_idx
            triggerTowerId = (values.at(i-1)-1) * 8 + (values.at(i)-1);  // mapped to 0-47
            triggerTowerMap_.insert(std::make_pair(triggerTowerId, std::vector<unsigned>()));
        } else if (values.at(i) > 10000) {
            triggerTowerMap_.at(triggerTowerId).push_back(values.at(i));
        }
    }

    if (triggerTowerMap_.size() != MAX_NTOWERS) {
        std::cout << Error() << "Failed to get all trigger towers from the trigger tower file: " << src << std::endl;
        return 1;
    }

    // Prepare the reversed trigger tower map
    triggerTowerMapReversed_.clear();
    for (auto it: triggerTowerMap_) {  // loop over trigger tower map
        for (auto it2: it.second) {  // loop over the moduleIds in the tower
            auto found = triggerTowerMapReversed_.find(it2);
            if (found == triggerTowerMapReversed_.end()) {  // if moduleId is not yet in the reversed map
                std::vector<unsigned> triggerTowerIds;
                triggerTowerIds.push_back(it.first);  // push back the towerId
                triggerTowerMapReversed_.insert(std::make_pair(it2, triggerTowerIds));
            } else {
                found->second.push_back(it.first);  // push back the towerId
            }
        }
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
        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " # stubs: " << nstubs << " [# passed: " << nPassed << "]" << std::endl;

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
        bool require = (nstubs >= MIN_NLAYERS);
        if (!require && filter_)
            keep = false;

        // Check sim info
        // Take the sim-matched particle info from the outermost stub
        const int good_trkId = 1;
        float simPt = vb_simPt->back();
        float simEta = vb_simEta->back();
        float simPhi = vb_simPhi->back();
        for (unsigned l=0; (l<nstubs) && keep; ++l) {
            unsigned ll = (nstubs-1) - l;  // reverse iteration order
            int trkId = vb_trkId->at(ll);
            if (trkId == good_trkId) {
                simPt = vb_simPt->at(ll);
                simEta = vb_simEta->at(ll);
                simPhi = vb_simPhi->at(ll);
                break;
            }
        }

        bool sim = (po.minPt  <= simPt  && simPt  <= po.maxPt  &&
                    po.minEta <= simEta && simEta <= po.maxEta &&
                    po.minPhi <= simPhi && simPhi <= po.maxPhi);
        if (!sim && filter_)
            keep = false;

        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " simPt: " << simPt << " simEta: " << simEta << " simPhi: " << simPhi << " keep? " << keep << std::endl;

        // _____________________________________________________________________
        // Build a list of goodLayers that is unique
        std::vector<id_type> goodLayers;  // the contents will be modified later on, beware!
        for (unsigned l=0; (l<nstubs) && keep; ++l) {
            unsigned moduleId = vb_modId->at(l);
            id_type layer = decodeLayer(moduleId);
            int trkId = vb_trkId->at(l);  // check sim info
            if (trkId == good_trkId) {
                goodLayers.push_back(layer);
            }
        }
        std::sort(goodLayers.begin(), goodLayers.end());
        goodLayers.erase(std::unique(goodLayers.begin(), goodLayers.end()), goodLayers.end() );

        // Build a list of goodLayerModules -- for each goodLayer, find one goodLayerModule
        // A goodLayerModule is the one that minimizes the difference in deltaR(module,simTrack)
        std::vector<id_type> goodLayerModules(goodLayers.size(), 0);
        std::vector<float> minDR_for_goodLayerModules(goodLayers.size(), 999.);
        for (unsigned gl=0; (gl<goodLayers.size()) && keep; ++gl) {
            for (unsigned l=0; (l<nstubs) && keep; ++l) {
                unsigned moduleId = vb_modId->at(l);
                id_type layer = decodeLayer(moduleId);
                if (layer != goodLayers.at(gl))  continue;

                int trkId = vb_trkId->at(l);
                if (trkId == good_trkId) {
                    float stub_eta = ROOT::Math::Impl::Eta_FromRhoZ(vb_r->at(l), vb_z->at(l));
                    float stub_phi = std::atan2(vb_y->at(l), vb_x->at(l));
                    float dR = deltaR(simEta, simPhi, stub_eta, stub_phi);
                    if (minDR_for_goodLayerModules.at(gl) > dR) {
                        minDR_for_goodLayerModules.at(gl) = dR;
                        goodLayerModules.at(gl) = moduleId;
                    }
                }
            }
        }  // end loop over goodLayers

        // Now repeat building the list of goodLayerModules, but at strip level
        std::vector<int> goodLayerCols(goodLayers.size(), 0);
        std::vector<int> goodLayerRows(goodLayers.size(), 0);
        std::vector<float> minDEta_for_goodLayerCols(goodLayers.size(), 999.);
        std::vector<float> minDPhi_for_goodLayerRows(goodLayers.size(), 999.);
        for (unsigned gl=0; (gl<goodLayers.size()) && keep; ++gl) {
            for (unsigned l=0; (l<nstubs) && keep; ++l) {
                bool keepstub = true;
                unsigned moduleId = vb_modId->at(l);
                unsigned nhits = vb_nhits->at(l);
                if (moduleId != goodLayerModules.at(gl))  continue;

                // First, for dEta
                int trkId = vb_trkId->at(l);
                for (unsigned m=0; (m<nhits) && keepstub; ++m) {
                    int col = vb_hitCols->at(l).at(m);
                    //int row = vb_hitRows->at(l).at(m);
                    //int trkId = vb_hitTrkIds->at(l).at(m);
                    if (trkId == good_trkId) {
                        float hit_x = vb_hitXs->at(l).at(m);
                        float hit_y = vb_hitYs->at(l).at(m);
                        float hit_z = vb_hitZs->at(l).at(m);
                        float hit_rho = std::sqrt(hit_x*hit_x + hit_y*hit_y);
                        float hit_eta = ROOT::Math::Impl::Eta_FromRhoZ(hit_rho, hit_z);
                        float dEta = std::abs(simEta - hit_eta);
                        if (minDEta_for_goodLayerCols.at(gl) > dEta) {
                            minDEta_for_goodLayerCols.at(gl) = dEta;
                            goodLayerCols.at(gl) = col;
                        }
                    }
                }

                // Second, for dPhi
                for (unsigned m=0; (m<nhits) && keepstub; ++m) {
                    int col = vb_hitCols->at(l).at(m);
                    if (col != goodLayerCols.at(gl))  continue;

                    int row = vb_hitRows->at(l).at(m);
                    //int trkId = vb_hitTrkIds->at(l).at(m);
                    if (trkId == good_trkId) {
                        float hit_x = vb_hitXs->at(l).at(m);
                        float hit_y = vb_hitYs->at(l).at(m);
                        float hit_phi = std::atan2(hit_y, hit_x);
                        float dPhi = std::abs(deltaPhi(simPhi, hit_phi));
                        if (minDPhi_for_goodLayerRows.at(gl) > dPhi) {
                            minDPhi_for_goodLayerRows.at(gl) = dPhi;
                            goodLayerRows.at(gl) = row;
                        }
                    }
                }
            }  // end loop over stubs
        }  // end loop over goodLayers

        if (verbose_>2) {
            std::cout << Debug() << "... evt: " << ievt << " # goodLayers: " << goodLayers.size() << std::endl;
            for (unsigned gl=0; (gl<goodLayers.size()) && keep; ++gl) {
                std::cout << Debug() << "... ... layer: " << goodLayers.at(gl) << " moduleId: " << goodLayerModules.at(gl) << " minDR: " << minDR_for_goodLayerModules.at(gl) << " col: " << goodLayerCols.at(gl) << " row: " << goodLayerRows.at(gl) << " minDEta: " << minDEta_for_goodLayerCols.at(gl) << " minDPhi: " << minDPhi_for_goodLayerRows.at(gl) << std::endl;
            }
        }

        // Decide the merging of layers -- goodLayers will use mlayer instead of layer after this!
        // Remember goodLayers is already sorted from smaller to larger value
        for (unsigned gl=0; (gl<goodLayers.size()) && keep; ++gl) {
            id_type layer = goodLayers.at(gl);
            id_type mlayer = layerMap_.at(layer);  // mapping of layer --> mlayer must exist
            goodLayers.at(gl) = mlayer;  // replace layer with mlayer
            auto found = std::find(goodLayers.begin(), goodLayers.end(), mlayer);  // only search up to index gl-1
            if (found == goodLayers.end()) {
                std::cout << Error() << "'mlayer' should always be found in goodLayers!" << std::endl;

            } else {  // there exist two different layers with the same mlayer
                const unsigned gl_found = found - goodLayers.begin();  // first occurence
                if (gl_found == gl) {
                    continue;  // do nothing
                } else {  // if this is not the first occurence
                    if (minDR_for_goodLayerModules.at(gl) < 0.1) {
                        // take this occurence since it's further away, thereby providing larger lever arm
                        goodLayerModules.at(gl_found)           = goodLayerModules.at(gl);
                        minDR_for_goodLayerModules.at(gl_found) = minDR_for_goodLayerModules.at(gl);
                        goodLayerCols.at(gl_found)              = goodLayerCols.at(gl);
                        goodLayerRows.at(gl_found)              = goodLayerRows.at(gl);
                        minDEta_for_goodLayerCols.at(gl_found)  = minDEta_for_goodLayerCols.at(gl);
                        minDPhi_for_goodLayerRows.at(gl_found)  = minDPhi_for_goodLayerRows.at(gl);
                    }
                    // Remove these occurences
                    goodLayers.at(gl)                 = 999999;
                    goodLayerModules.at(gl)           = 999999;
                    minDR_for_goodLayerModules.at(gl) = 999.;
                    goodLayerCols.at(gl)              = 999999;
                    goodLayerRows.at(gl)              = 999999;
                    minDEta_for_goodLayerCols.at(gl)  = 999.;
                    minDPhi_for_goodLayerRows.at(gl)  = 999.;
                }
            }
        }  // end loop over goodLayers

        // Erase the 999's
        goodLayers.erase(std::remove(goodLayers.begin(), goodLayers.end(), 999999), goodLayers.end() );
        goodLayerModules.erase(std::remove(goodLayerModules.begin(), goodLayerModules.end(), 999999), goodLayerModules.end() );
        minDR_for_goodLayerModules.erase(std::remove(minDR_for_goodLayerModules.begin(), minDR_for_goodLayerModules.end(), 999.), minDR_for_goodLayerModules.end() );
        goodLayerCols.erase(std::remove(goodLayerCols.begin(), goodLayerCols.end(), 999999), goodLayerCols.end() );
        goodLayerRows.erase(std::remove(goodLayerRows.begin(), goodLayerRows.end(), 999999), goodLayerRows.end() );
        minDEta_for_goodLayerCols.erase(std::remove(minDEta_for_goodLayerCols.begin(), minDEta_for_goodLayerCols.end(), 999.), minDEta_for_goodLayerCols.end() );
        minDPhi_for_goodLayerRows.erase(std::remove(minDPhi_for_goodLayerRows.begin(), minDPhi_for_goodLayerRows.end(), 999.), minDPhi_for_goodLayerRows.end() );

        {   // Sanity check
            if (verbose_>2) {
                std::cout << Debug() << "... evt: " << ievt << " layers after merging: ";
                std::copy(goodLayers.begin(), goodLayers.end(), std::ostream_iterator<id_type>(std::cout, " "));
                std::cout << " check sizes: " << goodLayerModules.size() << " " << minDR_for_goodLayerModules.size() << " " << goodLayerCols.size() << " " << goodLayerRows.size() << " " << minDEta_for_goodLayerCols.size() << " " << minDPhi_for_goodLayerRows.size() << std::endl;
            }

            unsigned ngoodlayers = goodLayers.size();
            assert(ngoodlayers == goodLayerModules.size() && ngoodlayers == minDR_for_goodLayerModules.size() &&
                   ngoodlayers == goodLayerCols.size() && ngoodlayers == goodLayerRows.size() && ngoodlayers == minDEta_for_goodLayerCols.size() && ngoodlayers == minDPhi_for_goodLayerRows.size());
        }

        // _____________________________________________________________________
        // Check again min # of layers
        require = (goodLayers.size() >= MIN_NLAYERS);
        if (!require && filter_)
            keep = false;

        // Now make keep-or-ignore decisions
        unsigned ngoodstubs = 0;
        for (unsigned l=0; (l<nstubs) && keep; ++l) {
            bool keepstub = true;

            unsigned moduleId = vb_modId->at(l);
            unsigned nhits = vb_nhits->at(l);
            assert(nhits > 0);
            if (verbose_>2)  std::cout << Debug() << "... ... stub: " << l << " moduleId: " << moduleId << " trkId: " << vb_trkId->at(l) << " # hits: " << nhits << std::endl;

            // Check sim info of stub
            // This is redundant now
            sim = (vb_trkId->at(l) == good_trkId);
            if (!sim && filter_)
                keepstub = false;

            // Check module id
            id_type layer = decodeLayer(moduleId);
            id_type mlayer = layerMap_.at(layer);
            const unsigned gl_found = std::find(goodLayers.begin(), goodLayers.end(), mlayer) - goodLayers.begin();
            unsigned count = std::count(goodLayers.begin(), goodLayers.end(), mlayer);
            if (sim) {
                assert(count == 1);
                count = (goodLayerModules.at(gl_found) == moduleId);
            }
            if (!count)  // do not keep even if filter_ is false
                keepstub = false;

            for (unsigned m=0; (m<nhits) && keepstub; ++m) {
                bool keephit = true;

                int col = vb_hitCols->at(l).at(m);
                int row = vb_hitRows->at(l).at(m);
                if (verbose_>2)  std::cout << Debug() << "... ... ... hit: " << m << " col: " << col << " row: " << row << " trkId: " << vb_hitTrkIds->at(l).at(m) << std::endl;

                // Check sim info of hit
                // This is redundant now
                sim = (vb_hitTrkIds->at(l).at(m) == good_trkId);
                if (!sim && filter_)
                    keephit = false;

                // Check row and col
                count = (goodLayerCols.at(gl_found) == col) && (goodLayerRows.at(gl_found) == row);
                if (!count)  // do not keep even if filter_ is false
                    keephit = false;

                if (keephit) {
                    // Keep and move it to be the first element
                    vb_hitCols->at(l).at(0)    = vb_hitCols->at(l).at(m);
                    vb_hitRows->at(l).at(0)    = vb_hitRows->at(l).at(m);
                    vb_hitTrkIds->at(l).at(0)  = vb_hitTrkIds->at(l).at(m);
                    vb_hitXs->at(l).at(0)      = vb_hitXs->at(l).at(m);
                    vb_hitYs->at(l).at(0)      = vb_hitYs->at(l).at(m);
                    vb_hitZs->at(l).at(0)      = vb_hitZs->at(l).at(m);
                    break;  // Only need one hit
                }
            }
            // Only need one hit
            vb_hitCols->at(l)  .resize(1);
            vb_hitRows->at(l)  .resize(1);
            vb_hitTrkIds->at(l).resize(1);
            vb_hitXs->at(l)    .resize(1);
            vb_hitYs->at(l)    .resize(1);
            vb_hitZs->at(l)    .resize(1);
            vb_nhits->at(l)    = 1;

            if (!count)  // do not keep even if filter_ is false
                keepstub = false;

            // Check sim info of hit
            // This is redundant now
            if (!sim && filter_)
                keepstub = false;

            if (keepstub) {
                // Keep and move it to be the 'ngoodstubs' element
                vb_x->at(ngoodstubs)          = vb_x->at(l);
                vb_y->at(ngoodstubs)          = vb_y->at(l);
                vb_z->at(ngoodstubs)          = vb_z->at(l);
                vb_r->at(ngoodstubs)          = vb_r->at(l);
                vb_phi->at(ngoodstubs)        = vb_phi->at(l);
                vb_roughPt->at(ngoodstubs)    = vb_roughPt->at(l);
                vb_iModCols->at(ngoodstubs)   = vb_iModCols->at(l);
                vb_iModRows->at(ngoodstubs)   = vb_iModRows->at(l);
                vb_modId->at(ngoodstubs)      = vb_modId->at(l);
                vb_nhits->at(ngoodstubs)      = vb_nhits->at(l);
                vb_hitCols->at(ngoodstubs)    = vb_hitCols->at(l);
                vb_hitRows->at(ngoodstubs)    = vb_hitRows->at(l);
                vb_hitTrkIds->at(ngoodstubs)  = vb_hitTrkIds->at(l);
                vb_hitXs->at(ngoodstubs)      = vb_hitXs->at(l);
                vb_hitYs->at(ngoodstubs)      = vb_hitYs->at(l);
                vb_hitZs->at(ngoodstubs)      = vb_hitZs->at(l);
                vb_simPt->at(ngoodstubs)      = vb_simPt->at(l);
                vb_simEta->at(ngoodstubs)     = vb_simEta->at(l);
                vb_simPhi->at(ngoodstubs)     = vb_simPhi->at(l);
                vb_trkId->at(ngoodstubs)      = vb_trkId->at(l);
                ++ngoodstubs;

                // No need any stub in this 'mlayer' anymore
                //goodLayers.at(gl_found)       = 999999;
                goodLayerModules.at(gl_found) = 999999;
                goodLayerRows.at(gl_found)    = 999999;
                goodLayerCols.at(gl_found)    = 999999;
            }
            if (verbose_>2)  std::cout << Debug() << "... ... stub: " << l << " keep? " << keepstub << std::endl;
        }

        // Check again min # of layers
        require = (ngoodstubs >= MIN_NLAYERS);

        if (!require && filter_)
            keep = false;
        if (!keep)  //  do not keep any stub
            ngoodstubs = 0;

        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " keep? " << keep << std::endl;

        vb_x        ->resize(ngoodstubs);
        vb_y        ->resize(ngoodstubs);
        vb_z        ->resize(ngoodstubs);
        vb_r        ->resize(ngoodstubs);
        vb_phi      ->resize(ngoodstubs);
        vb_roughPt  ->resize(ngoodstubs);
        vb_iModCols ->resize(ngoodstubs);
        vb_iModRows ->resize(ngoodstubs);
        vb_modId    ->resize(ngoodstubs);
        vb_nhits    ->resize(ngoodstubs);
        vb_hitCols  ->resize(ngoodstubs);
        vb_hitRows  ->resize(ngoodstubs);
        vb_hitTrkIds->resize(ngoodstubs);
        vb_hitXs    ->resize(ngoodstubs);
        vb_hitYs    ->resize(ngoodstubs);
        vb_hitZs    ->resize(ngoodstubs);
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
            // If there is a valid hit, but moduleId does not exist in any
            // trigger tower (due to cables, connections, or just too forward),
            // we drop them
            if (po.requireTriggerTower && triggerTowerMapReversed_.find(moduleId) == triggerTowerMapReversed_.end())
                continue;

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

        // Fake superstrip mechanism is carried out here

        // This map uses the fact that the keys in a C++ map are sorted
        std::map<unsigned, unsigned> mlayerToSuperstripMap;  // key: mlayer, value: index of superstrip in goodSuperstrips
        for (unsigned i=0; i<goodSuperstrips.size(); ++i) {
            addr_type ssId = goodSuperstrips.at(i).id();
            id_type layer = decodeSuperstripLayer(ssId);
            id_type mlayer = layerMap_.at(layer);
            if (mlayerToSuperstripMap.find(mlayer) == mlayerToSuperstripMap.end()) {  // not found
                mlayerToSuperstripMap.insert(std::make_pair(mlayer, i));
            } else {
                std::cout << Error() << "There is supposed to be one unique superstrip per layer at this stage" << std::endl;
            }
        }

        // Now find mlayer that is lacking a superstrip
        int nFakeSuperstripsInThisPattern = 0;
        for (std::map<unsigned, unsigned>::const_iterator it=layerMap_.begin(); it!=layerMap_.end(); ++it) {
            id_type mlayer = it->second;
            if (mlayerToSuperstripMap.find(mlayer) == mlayerToSuperstripMap.end()) {
                mlayerToSuperstripMap[it->second] = goodSuperstrips.size();
                nFakeSuperstripsInThisPattern += 1;
            }
        }

        // Insert fake superstrips
        std::vector<TTSuperstrip> goodAndFakeSuperstrips;
        for (std::map<unsigned, unsigned>::const_iterator it=mlayerToSuperstripMap.begin(); it!=mlayerToSuperstripMap.end(); ++it) {
            if (it->second == goodSuperstrips.size()) {  // create a fake superstrip
                addr_type ssId = encodeFakeSuperstripId();
                bit_type ssBit = 1 << 0;
                TTSuperstrip ss(ssId, ssBit);
                goodAndFakeSuperstrips.push_back(ss);

            } else {
                const TTSuperstrip& ss = goodSuperstrips.at(it->second);
                goodAndFakeSuperstrips.push_back(ss);
            }
        }

        if (verbose_>2) {
            std::cout << Debug() << "... evt: " << ievt << " # goodAndFakeSuperstrips: " << goodAndFakeSuperstrips.size() << " {";
            std::copy(goodAndFakeSuperstrips.begin(), goodAndFakeSuperstrips.end(), std::ostream_iterator<TTSuperstrip>(std::cout, " "));
            std::cout << "}" << std::endl;
        }

        // Check the required number of layers and fake superstrips
        if (nFakeSuperstripsInThisPattern > nFakeSuperstrips_)
            nFakeSuperstripsInThisPattern = nFakeSuperstrips_;
        bool require = ((int) goodSuperstrips.size() + nFakeSuperstripsInThisPattern >= nLayers_);
        if (!require && filter_)
            continue;

        // Create a pattern
        unsigned hash = hashThisEvent(v_event, vb_simPhi->back());  // FIXME: will switch to use generator seed instead
        TTPattern patt(hash, goodAndFakeSuperstrips);
        allPatterns_.push_back(patt);  // save the pattern

        ++nPassed;
    }

    // Keep only unique patterns
    uniquifyPatterns();  // do this after allPatterns_ is populated

    // Erase patterns not fully contained in a trigger tower
    for (std::vector<TTPattern>::iterator it=goodPatterns_.begin(); (it!=goodPatterns_.end()) && po.requireTriggerTower; ) {
        if (isFullyContainedInTriggerTower(*it)) {
            ++it;
        } else {
            it = goodPatterns_.erase(it);
        }
    }

    if (verbose_>2) {
        for (unsigned i=0; i<goodPatterns_.size(); ++i) {
            const TTPattern& patt = goodPatterns_.at(i);
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
    std::map<unsigned, std::vector<unsigned> > * ptr_triggerTowerMapReversed_ = &triggerTowerMapReversed_;
    ttree2->Branch("triggerTowerMapReversed", ptr_triggerTowerMapReversed_);
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
    if (nLayers_ <= 5) {
        std::cout << Warning() << "Does not really support nLayers = " << nLayers_ << ". Setting for nLayers = 6 is used." << std::endl;
    }

    // Hardcoded layer information
    if (nLayers_ <= 6) {
        layerMap_ = std::map<id_type, id_type> {
            {5,5}, {6,6}, {7,7}, {8,8}, {9,9}, {10,10},
            {11,10}, {12,9}, {13,8}, {14,7}, {15,6},
            {18,10}, {19,9}, {20,8}, {21,7}, {22,6}
        };
    } else if (nLayers_ == 7) {
        layerMap_ = std::map<id_type, id_type> {
            {5,5}, {6,6}, {7,7}, {8,8}, {9,9}, {10,10},
            {11,11}, {12,10}, {13,9}, {14,8}, {15,7},
            {18,11}, {19,10}, {20,9}, {21,8}, {22,7}
        };
    } else {  // >= 8
        layerMap_ = std::map<id_type, id_type> {
            {5,5}, {6,6}, {7,7}, {8,8}, {9,9}, {10,10},
            {11,11}, {12,12}, {13,10}, {14,9}, {15,8},
            {18,11}, {19,12}, {20,10}, {21,9}, {22,8}
        };
    }
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

// Remove patterns not fully contained in a trigger tower
bool PatternGenerator::isFullyContainedInTriggerTower(const TTPattern& patt) {
    std::map<unsigned, unsigned> triggerTowerIdCounts;
    unsigned moduleIdCount = 0;
    for(auto it: patt.id()) {  // loop over superstripIds in a pattern
        id_type moduleId = decodeSuperstripAndEncodeModuleId(it);  // retrieve the moduleId
        auto found = triggerTowerMapReversed_.find(moduleId);
        if (found == triggerTowerMapReversed_.end()) {  // if moduleId not in the map
            // do nothing
        } else {
            for (auto it2: found->second) {  // loop over towerIds containing the module
                auto found2 = triggerTowerIdCounts.find(it2);
                if (found2 == triggerTowerIdCounts.end()) {  // if not yet found, insert 1
                    triggerTowerIdCounts.insert(std::make_pair(it2, 1));
                } else {  // if found, just increment the counter
                    found2->second += 1;
                }
            }
            moduleIdCount += 1;
        }
        //std::cout << "moduleId: " << moduleId << " " << moduleIdCount << " " << found->second.front() << std::endl;
    }

    // Loop over the counters in the map
    for (auto it: triggerTowerIdCounts) {
        if ((int) it.second == (int) moduleIdCount) {
            return true;
        }
    }
    return false;
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
