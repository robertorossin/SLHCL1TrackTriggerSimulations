#define stubsDistributions_cxx
#include "stubsDistributions.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>

#include <iostream>
#include <fstream>
#include <iomanip> // for setw
#include <map>
#include <vector>


void stubsDistributions::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L stubsDistributions.C
//      Root > stubsDistributions t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

//   TStopwatch twatch;
//   twatch.Start();

   const unsigned nBinEta = 200;
   const unsigned nBinPhi = 200;

   const double minEta = 0.0;
   const double maxEta = 0.7;
   const double minPhi = 0.8;
   const double maxPhi = 1.5;

   const unsigned nLayers = 6;
   TH2D* h2StubEtaPhi[nLayers];
   for (unsigned iLay=0; iLay<nLayers; ++iLay) {
	   char cc[100];
	   sprintf(cc,"h2StubEtaPhi_Layer_%d",iLay);
	   h2StubEtaPhi[iLay] = new TH2D(cc,cc,nBinEta,minEta,maxEta,nBinPhi,minPhi,maxPhi);
   }

   Long64_t nbytes = 0, nb = 0, nCount = 0;
      for (Long64_t jentry=0; jentry<nentries;jentry++) {
//   for (Long64_t jentry=0; jentry<10000000;jentry++) {
	   Long64_t ientry = LoadTree(jentry);
	   if (ientry < 0) break;
	   nb = fChain->GetEntry(jentry);   nbytes += nb;

	   if (!genParts_pt->size()   ) continue;
	   if ( genParts_pt->at(0)<200) continue;

	   ++nCount;
	   for (unsigned imodId=0; imodId<TTStubs_modId->size(); ++imodId) {
		   short int stub_layer = TMath::Floor(TTStubs_modId   ->at(imodId)/10000.)-5;
		   //    	  std::cout << stub_layer << "\t";
		   h2StubEtaPhi[stub_layer]->Fill(TTStubs_eta->at(imodId),TTStubs_phi->at(imodId));
	   }
	   //      std::cout << std::endl;
	   //      short int stub_layer = TMath::Floor(TTStubs_modId   ->at(stubID)/10000.)-5;


	   if (jentry%1000000==0 || nCount%50000==0) {
		   std::cout << "Processing event " << setw(10) << nCount << "/" << jentry << setw(15) <<  std::endl;
	   }
   }
   TCanvas* cStubEtaPhi[nLayers];
   for (unsigned iLay=0; iLay<nLayers; ++iLay) {
	   char cc [100];
	   char cc2[100];
	   sprintf(cc,"cStubEtaPhi_Layer_%d_pt200",iLay);
	   cStubEtaPhi[iLay] = new TCanvas(cc,cc,0,0,900,900);
	   cStubEtaPhi[iLay]->cd();
	   h2StubEtaPhi[iLay]->DrawCopy("colz0");
	   sprintf(cc2,"/home/rossin/Dropbox/TT/Work/figures_stubCleaning/%s.pdf",cc);
	   cStubEtaPhi[iLay]->SaveAs(cc2);
   }


}

void stubsDistributionPlots() {
	gStyle->SetOptStat(0);
//	const unsigned nC=30;
//	int colors[nC];
//	for (unsigned i=0; i<nC; ++i) colors[i]=10+i;
//	gStyle->SetPalette(nC,colors);
	stubsDistributions s;
	s.Loop();
	return;

}
