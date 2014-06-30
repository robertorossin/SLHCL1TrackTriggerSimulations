#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/StubCleaner.h"

#ifndef ROOT_Math_GenVector_eta
#include "Math/GenVector/eta.h"
#endif

static const unsigned MIN_NGOODSTUBS = 3;

bool sortByFloat(const std::pair<unsigned, float>& lhs, const std::pair<unsigned, float>& rhs) {
    return lhs.second < rhs.second;
}


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
    std::vector<float> *          vb_x          = 0;
    std::vector<float> *          vb_y          = 0;
    std::vector<float> *          vb_z          = 0;
    std::vector<float> *          vb_r          = 0;
    std::vector<float> *          vb_phi        = 0;
    std::vector<float> *          vb_coordx     = 0;
    std::vector<float> *          vb_coordy     = 0;
    std::vector<float> *          vb_roughPt    = 0;
    std::vector<unsigned> *       vb_iModCols   = 0;
    std::vector<unsigned> *       vb_iModRows   = 0;
    std::vector<unsigned> *       vb_modId      = 0;
    std::vector<unsigned> *       vb_nhits      = 0;
    std::vector<float> *          vb_simPt      = 0;
    std::vector<float> *          vb_simEta     = 0;
    std::vector<float> *          vb_simPhi     = 0;
    std::vector<int> *            vb_trkId      = 0;

    chain_->SetBranchStatus("*"                 , 0);
    chain_->SetBranchStatus("TTStubs_x"         , 1);
    chain_->SetBranchStatus("TTStubs_y"         , 1);
    chain_->SetBranchStatus("TTStubs_z"         , 1);
    chain_->SetBranchStatus("TTStubs_r"         , 1);
    chain_->SetBranchStatus("TTStubs_phi"       , 1);
    chain_->SetBranchStatus("TTStubs_coordx"    , 1);
    chain_->SetBranchStatus("TTStubs_coordy"    , 1);
    chain_->SetBranchStatus("TTStubs_roughPt"   , 1);
    chain_->SetBranchStatus("TTStubs_iModCols"  , 1);
    chain_->SetBranchStatus("TTStubs_iModRows"  , 1);
    chain_->SetBranchStatus("TTStubs_modId"     , 1);
    chain_->SetBranchStatus("TTStubs_nhits"     , 1);
    chain_->SetBranchStatus("TTStubs_simPt"     , 1);
    chain_->SetBranchStatus("TTStubs_simEta"    , 1);
    chain_->SetBranchStatus("TTStubs_simPhi"    , 1);
    chain_->SetBranchStatus("TTStubs_trkId"     , 1);

    chain_->SetBranchAddress("TTStubs_x"        , &(vb_x));
    chain_->SetBranchAddress("TTStubs_y"        , &(vb_y));
    chain_->SetBranchAddress("TTStubs_z"        , &(vb_z));
    chain_->SetBranchAddress("TTStubs_r"        , &(vb_r));
    chain_->SetBranchAddress("TTStubs_phi"      , &(vb_phi));
    chain_->SetBranchAddress("TTStubs_coordx"   , &(vb_coordx));
    chain_->SetBranchAddress("TTStubs_coordy"   , &(vb_coordy));
    chain_->SetBranchAddress("TTStubs_roughPt"  , &(vb_roughPt));
    chain_->SetBranchAddress("TTStubs_iModCols" , &(vb_iModCols));
    chain_->SetBranchAddress("TTStubs_iModRows" , &(vb_iModRows));
    chain_->SetBranchAddress("TTStubs_modId"    , &(vb_modId));
    chain_->SetBranchAddress("TTStubs_nhits"    , &(vb_nhits));
    chain_->SetBranchAddress("TTStubs_simPt"    , &(vb_simPt));
    chain_->SetBranchAddress("TTStubs_simEta"   , &(vb_simEta));
    chain_->SetBranchAddress("TTStubs_simPhi"   , &(vb_simPhi));
    chain_->SetBranchAddress("TTStubs_trkId"    , &(vb_trkId));

    // In addition, keep genParticle info
    chain_->SetBranchStatus("genParts_pt"       , 1);
    chain_->SetBranchStatus("genParts_eta"      , 1);
    chain_->SetBranchStatus("genParts_phi"      , 1);

    // Set up TTreeFormula for event selection
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
    int nPassed = 0;
    int nKept = 0;
    int curTree = chain_->GetTreeNumber();
    for (int ievt=0; ievt<nEvents_; ++ievt) {
        Long64_t local_entry = chain_->LoadTree(ievt);  // for TChain
        if (local_entry < 0)  break;
        if (chain_->GetTreeNumber() != curTree) {  // for TTreeFormula
            curTree = chain_->GetTreeNumber();
            ttf_event->UpdateFormulaLeaves();
        }
        chain_->GetEntry(ievt);

        unsigned nstubs = vb_modId->size();
        if (verbose_>1 && ievt%20000==0)  std::cout << Debug() << Form("... Processing event: %7i, keeping: %7i", ievt, nKept) << std::endl;
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
        bool require = (nstubs >= MIN_NGOODSTUBS);
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

        // Apply pt, eta, phi requirements
        bool sim = (po.minPt  <= simPt  && simPt  <= po.maxPt  &&
                    po.minEta <= simEta && simEta <= po.maxEta &&
                    po.minPhi <= simPhi && simPhi <= po.maxPhi);
        if (!sim && filter_)
            keep = false;

        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " simPt: " << simPt << " simEta: " << simEta << " simPhi: " << simPhi << " keep? " << keep << std::endl;

        // _____________________________________________________________________
        // Remove multiple stubs in one layer
        std::vector<std::pair<unsigned, float> > vec_index_minDEta;
        for (unsigned l=0; (l<nstubs) && keep; ++l) {
            int trkId = vb_trkId->at(l);  // check sim info
            if (trkId == good_trkId) {
                float stub_eta = ROOT::Math::Impl::Eta_FromRhoZ(vb_r->at(l), vb_z->at(l));
                //float stub_phi = std::atan2(vb_y->at(l), vb_x->at(l));
                float dEta = std::abs(simEta - stub_eta);
                vec_index_minDEta.push_back(std::make_pair(l, dEta));
            }
        }

        // Sort: smallest deltaEta to largest
        std::sort(vec_index_minDEta.begin(), vec_index_minDEta.end(), sortByFloat);

        std::vector<unsigned> goodLayerStubs(nLayers_, 999999);
        for (std::vector<std::pair<unsigned, float> >::const_iterator it=vec_index_minDEta.begin();
             it!=vec_index_minDEta.end(); ++it) {
            unsigned l = it->first;
            float dEta = it->second;

            unsigned moduleId = vb_modId->at(l);
            unsigned layer = decodeLayer(moduleId);
            unsigned mlayer = layerMap_.at(layer);  // mapping of layer --> mlayer must exist

            if (goodLayerStubs.at(mlayer) == 999999) {
                goodLayerStubs.at(mlayer) = l;  // for each mlayer, takes the stub with minDEta to simTrack
            } else {
                unsigned l_old = goodLayerStubs.at(mlayer);
                unsigned moduleId_old = vb_modId->at(l_old);
                unsigned layer_old = decodeLayer(moduleId_old);
                if (layer > layer_old && dEta < 0.1) {
                    // in case of layer merging, take the stub further away in z,
                    // as it provides larger lever arm. However, endcap provides
                    // worse resolution
                    goodLayerStubs.at(mlayer) = l;
                }
            }
        }

        // _____________________________________________________________________
        // Now make keep-or-ignore decision per stub
        unsigned ngoodstubs = 0;
        for (unsigned l=0; (l<nstubs) && keep; ++l) {
            bool keepstub = true;

            unsigned moduleId = vb_modId->at(l);
            unsigned nhits = vb_nhits->at(l);
            assert(nhits > 0);
            if (verbose_>2)  std::cout << Debug() << "... ... stub: " << l << " moduleId: " << moduleId << " trkId: " << vb_trkId->at(l) << " # hits: " << nhits << std::endl;

            unsigned count = std::count(goodLayerStubs.begin(), goodLayerStubs.end(), l);
            if (!count)  // do not keep even if filter_ is false
                keepstub = false;

            if (keepstub) {
                // Keep and move it to be the 'ngoodstubs' element
                vb_x->at(ngoodstubs)          = vb_x->at(l);
                vb_y->at(ngoodstubs)          = vb_y->at(l);
                vb_z->at(ngoodstubs)          = vb_z->at(l);
                vb_r->at(ngoodstubs)          = vb_r->at(l);
                vb_phi->at(ngoodstubs)        = vb_phi->at(l);
                vb_coordx->at(ngoodstubs)     = vb_coordx->at(l);
                vb_coordy->at(ngoodstubs)     = vb_coordy->at(l);
                vb_roughPt->at(ngoodstubs)    = vb_roughPt->at(l);
                vb_iModCols->at(ngoodstubs)   = vb_iModCols->at(l);
                vb_iModRows->at(ngoodstubs)   = vb_iModRows->at(l);
                vb_modId->at(ngoodstubs)      = vb_modId->at(l);
                vb_nhits->at(ngoodstubs)      = vb_nhits->at(l);
                vb_simPt->at(ngoodstubs)      = vb_simPt->at(l);
                vb_simEta->at(ngoodstubs)     = vb_simEta->at(l);
                vb_simPhi->at(ngoodstubs)     = vb_simPhi->at(l);
                vb_trkId->at(ngoodstubs)      = vb_trkId->at(l);
                ++ngoodstubs;
            }
            if (verbose_>2)  std::cout << Debug() << "... ... stub: " << l << " keep? " << keepstub << std::endl;
        }

        // Check again min # of layers
        require = (ngoodstubs >= MIN_NGOODSTUBS);
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
        vb_coordx   ->resize(ngoodstubs);
        vb_coordy   ->resize(ngoodstubs);
        vb_roughPt  ->resize(ngoodstubs);
        vb_iModCols ->resize(ngoodstubs);
        vb_iModRows ->resize(ngoodstubs);
        vb_modId    ->resize(ngoodstubs);
        vb_nhits    ->resize(ngoodstubs);
        vb_simPt    ->resize(ngoodstubs);
        vb_simEta   ->resize(ngoodstubs);
        vb_simPhi   ->resize(ngoodstubs);
        vb_trkId    ->resize(ngoodstubs);

        ++nKept;
        ttree->Fill();
    }
    if (verbose_)  std::cout << Info() << "Processed " << nEvents_ << " events, kept " << nKept << ", passed " << nPassed << std::endl;

    assert(ttree->GetEntries() == nKept);
    tfile->Write();
    delete ttf_event;
    delete ttree;
    delete tfile;

    return 0;
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
