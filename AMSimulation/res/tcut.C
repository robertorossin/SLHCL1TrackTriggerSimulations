#include "../interface/HelperMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"

void tcut(const char *output,
          const char *input,
          const char *suffix = ".root",
          const char *prefix = "results") {

    TFile* infile = TFile::Open(input);
    TTree* intree = (TTree*) infile->Get("ntupler/tree");
    
    TString cut = "genParts_pt[0]>2 && abs(genParts_eta[0])<2.2 && Sum$(TTStubs_trkId==1 && abs(atan2(TTStubs_r,TTStubs_z)-atan2(genParts_pt[0],genParts_pz[0]))<0.05 && abs(deltaPhi(atan2(TTStubs_y,TTStubs_x),genParts_phi[0]))<0.03)>=Sum$(TTStubs_trkId==1)-2";
    
    TFile* outfile = TFile::Open(output, "RECREATE");
    outfile->mkdir("ntupler")->cd();
    TTree* outtree = (TTree*) intree->CopyTree(cut);
    outfile->Write();
    delete outtree;
    delete outfile;
    delete intree;
    delete infile;
}

