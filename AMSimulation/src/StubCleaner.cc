#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/StubCleaner.h"

#ifndef ROOT_Math_GenVector_eta
#include "Math/GenVector/eta.h"
#endif

static const unsigned MIN_NLAYERS = 3;


// _____________________________________________________________________________
// Read the input ntuples
int StubCleaner::readFile(TString src) {
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
// If an event fails selection, all stubs are removed
int StubCleaner::cleanStubs(TString out) {
    if (!out.EndsWith(".root")) {
        std::cout << Error() << "Output filename must be .root" << std::endl;
        return 1;
    }

    // Do not do GetEntries() as it conflicts with CloneTree() when reading from XROOTD
    //Long64_t nentries = chain_->GetEntries();
    //if (nentries <= 0) {
    //    std::cout << Error() << "Input source has zero entry." << std::endl;
    //    return 1;
    //}
    //
    //if (nEvents_ > (int) nentries)
    //    nEvents_ = nentries;
    if (verbose_)  std::cout << Info() << "Reading " << nEvents_ << " events; recreating " << out << " after stub cleaning." << std::endl;

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

    // Set up TTreeFormula for event selection
    //chain_->SetBranchStatus("genParts_pt"       , 1);
    //chain_->SetBranchStatus("genParts_eta"      , 1);
    //chain_->SetBranchStatus("genParts_phi"      , 1);
    //chain_->SetBranchStatus("genParts_pz"       , 1);
    TTreeFormula* ttf_event = new TTreeFormula("ttf_event", eventSelect_, chain_);
    //ttf_event->SetQuickLoad(1);

    // For writing
    TFile* tfile = TFile::Open(out, "RECREATE");
    tfile->mkdir("ntupler")->cd();

    TTree* ttree = (TTree*) chain_->CloneTree(0); // Do not copy the data yet
    // The clone should not delete any shared i/o buffers.
    ResetDeleteBranches(ttree);


    // _________________________________________________________________________
    // Loop over all events
    Long64_t nPassed = 0;
    Long64_t nKept = 0;
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
        if (verbose_>1 && ievt%20000==0)  std::cout << Debug() << Form("... Processing event: %7lld, keeping: %7lld", ievt, nKept) << std::endl;
        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " # stubs: " << nstubs << " [# passed: " << nPassed << "]" << std::endl;

        if (!nstubs) {  // skip if no stub
            ++nKept;
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
        goodLayers.erase(std::unique(goodLayers.begin(), goodLayers.end()), goodLayers.end() );  // it's sorted and unique from here on

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
            auto found = std::find(goodLayers.begin(), goodLayers.end(), mlayer);  // find the first occurence
            if (found == goodLayers.end()) {
                std::cout << Error() << "'mlayer' should always be found in goodLayers!" << std::endl;

            } else {  // there exist two different layers with the same mlayer
                const unsigned gl_found = found - goodLayers.begin();  // first occurence
                if (gl_found == gl) {
                    continue;  // do nothing
                } else {  // if this is not the first occurence
                    if (minDR_for_goodLayerModules.at(gl) < 0.1) {
                        // Take this occurence since it's further away, thereby providing larger lever arm
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
        erase999999(goodLayers);
        erase999999(goodLayerModules);
        erase999(minDR_for_goodLayerModules);
        erase999999(goodLayerCols);
        erase999999(goodLayerRows);
        erase999(minDEta_for_goodLayerCols);
        erase999(minDPhi_for_goodLayerRows);
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
            unsigned count = std::count(goodLayers.begin(), goodLayers.end(), mlayer);
            const unsigned gl_found = std::find(goodLayers.begin(), goodLayers.end(), mlayer) - goodLayers.begin();
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

        if (keep)
            ++nPassed;
        else //  do not keep any stub
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

        ++nKept;
        ttree->Fill();
    }
    if (verbose_)  std::cout << Info() << "Processed " << nEvents_ << " events, kept " << nKept << " events, passed " << nPassed << std::endl;

    assert(ttree->GetEntries() == nKept);
    tfile->Write();
    delete ttf_event;
    delete ttree;
    delete tfile;

    if (verbose_)  std::cout << Info() << "Replacing input source." << std::endl;
    chain_->Reset();
    chain_->Add(out);

    return 0;
}

// _____________________________________________________________________________
// Private functions
void StubCleaner::erase999999(std::vector<id_type>& vec) {
    vec.erase(std::remove(vec.begin(), vec.end(), 999999), vec.end());
}

void StubCleaner::erase999999(std::vector<int>& vec) {
    vec.erase(std::remove(vec.begin(), vec.end(), 999999), vec.end());
}

void StubCleaner::erase999(std::vector<float>& vec) {
    vec.erase(std::remove(vec.begin(), vec.end(), 999.), vec.end());
}


// _____________________________________________________________________________
// Main driver
int StubCleaner::run(TString out, TString src) {
    gROOT->ProcessLine("#include <vector>");  // how is it not loaded?

    int exitcode = 0;
    Timing(1);

    exitcode = readFile(src);
    if (exitcode)  return exitcode;
    Timing();

    exitcode = cleanStubs(out);
    if (exitcode)  return exitcode;
    Timing();

    chain_->Reset();
    return exitcode;
}
