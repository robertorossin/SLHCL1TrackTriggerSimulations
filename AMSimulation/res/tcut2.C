#include "../interface/HelperMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TChain.h"
#include "TTreeFormula.h"

void tcut2(int nEvents=-1,
           const char *output = "out.root",
           const char *input = "",
           const char *suffix = ".root",
           const char *prefix = "results") {

    //TFile* infile = TFile::Open(input);
    //TTree* intree = (TTree*) infile->Get("ntupler/tree");

    //TString cut = "genParts_pt[0]>2 && abs(genParts_eta[0])<2.2 && Sum$(TTStubs_trkId==1 && abs(atan2(TTStubs_r,TTStubs_z)-atan2(genParts_pt[0],genParts_pz[0]))<0.05 && abs(deltaPhi(atan2(TTStubs_y,TTStubs_x),genParts_phi[0]))<0.03)>=Sum$(TTStubs_trkId==1)-2";
    TString cut = "genParts_pt[0]>20 && abs(genParts_eta[0])<1.0";

    TChain* chain_ = new TChain("ntupler/tree");
    chain_->Add("/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_ntuple/SingleMuMinus_OneOverPt_vz0_20140420/ntuple_1_1_Y0t.root");
    //chain_->Add("/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_ntuple/SingleMuPlus_OneOverPt_vz0_20140420/ntuple_1_1_EAj.root");

    TTreeFormula* ttf_event = new TTreeFormula("ttf_event", cut, chain_);
    
    TFile* outfile = TFile::Open(output, "RECREATE");
    outfile->mkdir("ntupler")->cd();
    TTree *outtree = (TTree*) chain_->CloneTree(0); // Do no copy the data yet

    if (nEvents==-1)  nEvents = chain_->GetEntriesFast();
    int curTree = chain_->GetTreeNumber();
    for (Long64_t ievt=0; ievt<nEvents; ++ievt) {
        Long64_t local_entry = chain_->LoadTree(ievt);  // for TChain
        if (local_entry < 0)  break;
        if (chain_->GetTreeNumber() != curTree) {  // for TTreeFormula
            curTree = chain_->GetTreeNumber();
            ttf_event->UpdateFormulaLeaves();
        }
        chain_->GetEntry(ievt);

        int ndata = ttf_event->GetNdata();
        bool keep = ttf_event->EvalInstance();
        if (!keep)
            continue;

        outtree->Fill();
    }

    outfile->Write();
    delete outtree;
    delete outfile;
    //delete intree;
    //delete infile;
    delete chain_;
}

