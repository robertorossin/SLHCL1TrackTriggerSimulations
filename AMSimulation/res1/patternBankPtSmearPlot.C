#define patternBankPtSmear_cxx
#include "patternBankPtSmear.h"
#include <TH2.h>
#include <TF1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TGraphErrors.h>
#include <TProfile.h>
#include <TString.h>
#include <TObjArray.h>
#include <TText.h>
#include "TFitResult.h"
#include <TFitResultPtr.h>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>


struct patternS {
	double meanPt   ;
	double rmsPt    ;
	double meanPhi  ;
	double rmsPhi   ;
	double frequency;
	int    chargeSum;

	bool operator<(const patternS& a) const
	{
		return meanPt < a.meanPt;
	}

};

long unsigned int patternBankPtSmear::ReadPercentile(TString sTree) {
	std::cout << sTree.Data() << std::endl;
	if (percentile==95) {
//		TString sLog(sTree.ReplaceAll(".root",".log"));
		if(sTree.Contains("patternBank_SingleMuon_sf0p7_nz1_tt27_pt2_first50M_SL6_.root")) return 292600;
		if(sTree.Contains("patternBank_sf0p7_nz1_tt27_pt2_400M.root")) return 282800;
		if(sTree.Contains("patternBank_sf0p7_nz1_tt27_pt3_400M.root")) return 141500;
		if(sTree.Contains("patternBank_SingleMuon_sf1_nz1_tt27_pt2_first50M_SL6_.root"  )) return 103000;
		if(sTree.Contains("patternBank_sf1_nz1_tt27_pt2_SingleMuonFlatOneOverPt0p0005To0p5_tt27NoTest_4M.root"  )) return 110900;
		if(sTree.Contains("patternBank_sf1_nz1_tt27_pt2_400M.root"  )) return 101700;
		if(sTree.Contains("patternBank_sf1_nz1_tt27_pt3_400M.root"  )) return 57500;
		if(sTree.Contains("patternBank_sf2_nz1_tt27_pt2_5.0M.root"  )) return 19000;
		if(sTree.Contains("patternBank_sf2_nz1_tt27_pt3_5.0M.root"  )) return 12400;
	}
	if (percentile==99) {
		if(sTree.Contains("patternBank_SingleMuon_sf0p7_nz1_tt27_pt2_first50M_SL6_.root")) return 478400;
		if(sTree.Contains("patternBank_sf0p7_nz1_tt27_pt2_400M.root")) return 449800;
		if(sTree.Contains("patternBank_sf0p7_nz1_tt27_pt3_400M.root")) return 214900;
		if(sTree.Contains("patternBank_SingleMuon_sf1_nz1_tt27_pt2_first50M_SL6_.root"  )) return 159600;
		if(sTree.Contains("patternBank_sf1_nz1_tt27_pt2_400M.root"  )) return 155600;
		if(sTree.Contains("patternBank_sf1_nz1_tt27_pt3_400M.root"  )) return  84200;
		if(sTree.Contains("patternBank_sf2_nz1_tt27_pt2_5.0M.root"  )) return  27000;
		if(sTree.Contains("patternBank_sf2_nz1_tt27_pt3_5.0M.root"  )) return  17100;
	}
	if (percentile==100) return 1E9;
	return 0;
}

void patternBankPtSmear::Loop(TString sTree, int makePlots)
{
//   In a ROOT session, you can do:
//      Root > .L patternBankPtSmear.C
//      Root > patternBankPtSmear t
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

   std::vector <patternS> pattS;

//   std::multimap <double, std::pair< std::pair<unsigned int, int>, double> > ptmap;

//   TString sSSID(sTree);
//   sSSID.ReplaceAll(".root","_ssID_pT.root");
//   TFile *f = TFile::Open(TString("/data/rossin/EOS/")+sSSID,"RECREATE");
//   if (!f) { return; }
   std::vector <unsigned int > AMTTRoads_superstripIds;
//   double ssPt, ssRms;
//   TTree *t = new TTree("tSSID","pattern ssID InvPt and InvPtRms");
//   t->Branch("superstripIds"   ,&AMTTRoads_superstripIds);
//   t->Branch("superstripMeanInvPt",&ssPt );
//   t->Branch("superstripRmsInvPt" ,&ssRms);

   long unsigned int nPercentile = ReadPercentile(sTree);

   Long64_t nentries = TMath::Min((long unsigned int)fChain->GetEntriesFast(),nPercentile);

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
//      if (jentry<10) printf("meanPt: %7.4f\n", meanPt);
      // if (Cut(ientry) < 0) continue;
//      ptmap.insert(std::make_pair(TMath::Sign(meanPt, (double)chargeSum), std::make_pair( std::make_pair(frequency, chargeSum), rmsPt)));
      patternS p;
      p.meanPt    = TMath::Sign(meanPt, (double)chargeSum);
      p.rmsPt     = rmsPt  ;
      p.meanPhi   = meanPhi;
      p.rmsPhi    = rmsPhi ;
      p.frequency = frequency;
      p.chargeSum = chargeSum;
      pattS.push_back(p);

//      ssPt  = p.meanPt;
//      ssRms = p.rmsPt ;
      AMTTRoads_superstripIds.clear();
      for (unsigned int iSS=0; iSS<superstripIds->size(); ++iSS) {
    	  AMTTRoads_superstripIds.push_back(superstripIds->at(iSS));
      }
//      t->Fill();
   }
//   f->Write();
//   delete f;

   printf("Nentries: %lu\t\tNpercentile[%d]: %ld\n", pattS.size(), percentile, nPercentile);
   if (nPercentile==0) return;
   unsigned int maxminPopularity[2] = {pattS.at(0).frequency , pattS.at(nentries-1).frequency};

   std::sort(pattS.begin(), pattS.end());


   const unsigned long int Npatt = pattS.size();

   float *vmeanPt      =new  float[Npatt];
   float *vrmsPt       =new  float[Npatt];
   float *vRelrmsPt    =new  float[Npatt];
   float *vmeanPhi     =new  float[Npatt];
   float *vrmsPhi      =new  float[Npatt];
   float *vRelrmsPhi   =new  float[Npatt];
   float *vfrequency   =new  float[Npatt];
   float *vcount       =new  float[Npatt];
   float *vdummy       =new  float[Npatt];
   int   *vcharge      =new    int[Npatt];
   float *vchargeMisID =new  float[Npatt];


   float fFreqRange = 1/3.;
   TString sTitle("");
   TString sName("");
   TObjArray* obj = sTree.Tokenize("_");
   if (obj->GetEntries()>=6) {
  	 char c[100];

  	 if (sTree.Contains("sf0p7")) sTitle=TString("Fountain - SF=0.7 - ");
  	 if (sTree.Contains("sf1"  )) sTitle=TString("Fountain - SF=1.0 - ");
  	 if (sTree.Contains("sf2"  )) sTitle=TString("Fountain - SF=2.0 - ");
  	 if (sTree.Contains("nz1"  )) sTitle=sTitle+TString("1 Z - ");
  	 if (sTree.Contains("pt2"  )) {
  		 sTitle=sTitle+TString("pT.gt.2 - ");
  		 fFreqRange = 0.5;
  	 }
  	 if (sTree.Contains("pt3"  )) {
  		 sTitle=sTitle+TString("pT.gt.3 - ");
  		 fFreqRange = 1/3.0;
  	 }
     sprintf(c,"%d%% coverage", percentile);
  	 sTitle=sTitle+TString(c);
//
  	 sName=sName+TString(((TObjString*)(obj->At(1)))->String()+"_");
  	 sName=sName+TString(((TObjString*)(obj->At(2)))->String()+"_");
  	 sName=sName+TString(((TObjString*)(obj->At(4)))->String()+"_");
     sprintf(c,"perc%d_", percentile);
  	 sName=sName+TString(c);
   }
   TString cTemp(sName.ReplaceAll("/","_"));
   sName=cTemp;

   const unsigned int nFreqBin = 1000;
   TH1D* hPattFreq = new TH1D("hPattFreq","hPattFreq", nFreqBin, -fFreqRange, fFreqRange);
   float *vxPattFreq   = new float[Npatt];
   float *vyPattFreq   = new float[Npatt];

   const unsigned int nRMSBin = 100000;
   TProfile* prRMS    = new TProfile("prRMS","prRMS", nRMSBin/100, -fFreqRange, fFreqRange,""); // Error bar is RMS
   TH2*      h2RMS    = new TH2D    ("h2RMS","h2RMS", nRMSBin, -fFreqRange, fFreqRange,100,0,0.01); // Error bar is RMS
   TProfile* prRMSphi = new TProfile("prRMSphi","prRMSphi", nRMSBin/100, 0.75, 1.6,""); // Error bar is RMS
   TH2*      h2RMSphi = new TH2D    ("h2RMSphi","h2RMSphi", nRMSBin, 0.75, 1.6,100,0,0.004); // Error bar is RMS
   TProfile* prRMSphiVSpT = new TProfile("prRMSphiVSpT","prRMSphiVSpT", nRMSBin/100, -fFreqRange, fFreqRange,""); // Error bar is RMS
   TH2*      h2RMSphiVSpT = new TH2D    ("h2RMSphiVSpT","h2RMSphiVSpT", nRMSBin, -fFreqRange, fFreqRange,100,0,0.004); // Error bar is RMS

   double subRange=0.02;
   long unsigned int nPattSubRange = 0;
   for (unsigned long int ipatt=0; ipatt<Npatt; ++ipatt) {

  	 vmeanPt     [ipatt] =     pattS.at(ipatt).meanPt   ;
  	 vrmsPt      [ipatt] =     pattS.at(ipatt). rmsPt   ;
  	 vRelrmsPt   [ipatt] =     pattS.at(ipatt). rmsPt /fabs(vmeanPt     [ipatt])    ;
  	 vmeanPhi    [ipatt] =     pattS.at(ipatt).meanPhi  ;
  	 vrmsPhi     [ipatt] =     pattS.at(ipatt). rmsPhi  ;
  	 vRelrmsPhi  [ipatt] =     pattS.at(ipatt). rmsPhi/fabs(vmeanPhi    [ipatt])    ;
  	 vfrequency  [ipatt] =     pattS.at(ipatt).frequency;
  	 vcharge     [ipatt] =     pattS.at(ipatt).chargeSum;
  	 vchargeMisID[ipatt] =   0.5-fabs(vcharge[ipatt])/(2.0*vfrequency[ipatt]);
  	 vcount      [ipatt] =     (ipatt);
  	 vdummy      [ipatt] =     (0);
  	 prRMS   ->Fill(vmeanPt     [ipatt], vrmsPt [ipatt]);
  	 h2RMS   ->Fill(vmeanPt     [ipatt], vrmsPt [ipatt]);
  	 prRMSphi->Fill(vmeanPhi    [ipatt], vrmsPhi[ipatt]);
  	 h2RMSphi->Fill(vmeanPhi    [ipatt], vrmsPhi[ipatt]);
  	 prRMSphiVSpT->Fill(vmeanPt    [ipatt], vrmsPhi[ipatt]);
  	 h2RMSphiVSpT->Fill(vmeanPt    [ipatt], vrmsPhi[ipatt]);
  	 if (fabs(vmeanPt     [ipatt])<fFreqRange-subRange) ++nPattSubRange;
   }


   long unsigned int iPattSubRange = 0;
   for (unsigned long int ipatt=0; ipatt<Npatt; ++ipatt) {
	   if (fabs(vmeanPt     [ipatt])>fFreqRange-subRange) continue;
	   hPattFreq->Fill(vmeanPt     [ipatt]);
	   float x          =-(fFreqRange-subRange)+iPattSubRange*2*(fFreqRange-subRange)/nPattSubRange;
	   vxPattFreq[iPattSubRange]= x;
	   vyPattFreq[iPattSubRange]= x - vmeanPt[ipatt];
	   ++iPattSubRange;
   }

   float     relrms_pt10GeV=0;
   unsigned int ic=0;
   for (unsigned long int ipatt=Npatt-1; ipatt>0; --ipatt) {
  	 if (vmeanPt[ipatt]>0.11 || vmeanPt[ipatt]<0.09) continue;
  	 relrms_pt10GeV+=vRelrmsPt   [ipatt];
  	 ++ic;
   }
	 relrms_pt10GeV/=ic;

	 char cc[100];
   //   TGraphErrors *gr = new TGraphErrors(iCount, vmeanPt, vrmsPt, vdummy, vdummy);
//   TGraphErrors *gr = new TGraphErrors(Npatt, vcount, vmeanPt, vdummy, vrmsPt);
   TGraphErrors *gr = new TGraphErrors(Npatt, vcount, vmeanPt, vdummy, vrmsPt);
   TCanvas* c = new TCanvas();
   c->SetTitle(sTitle);
   gr->SetTitle("Charge/Pt VS patternID");
   gr->SetTitle("<charge/Pt> VS patternID");

   c->SetGridx();
   c->SetGridy();
   gr->GetXaxis()->SetTitle("pattern ID (charge/Pt sorted)");
   gr->GetYaxis()->SetTitle("<charge/Pt> [c/GeV]");
   gr->Draw("APZ");
   TF1 *f1 = new TF1("f1", "[0] +[1]*x", 0, Npatt);
   TF1 *f3 = new TF1("f3", "[0] +[1]*x +[2]*x*x +[3]*x*x*x", 0, Npatt);
   TF1 *f5p = new TF1("f5p", "[0] +[1]*x +[2]*x*x +[3]*x*x*x +[4]*x*x*x*x +[5]*x*x*x*x*x+[6]*(x<[7])", 0, Npatt);
   f1->SetParameters(0,0);
   f3->SetParameters(0,0);
   f5p->FixParameter(7,Npatt/2.);
   TFitResultPtr r = gr->Fit("f3","SR");
//   TF1 *f11 = new TF1("f11", "[0] +[1]*x"                    ,  0, Npatt);
   TF1 *f13 = new TF1("f13", "[0] +[1]*x +[2]*x*x +[3]*x*x*x",  0, Npatt);
   TF1 *f15p = new TF1("f15p", "[0] +[1]*x +[2]*x*x +[3]*x*x*x +[4]*x*x*x*x +[5]*x*x*x*x*x+[6]*(x<[7])",  0, Npatt);
   f13->SetParameters(r->Parameter(0),r->Parameter(1),r->Parameter(2),r->Parameter(3));
   f15p->SetParameters(r->Parameter(0),r->Parameter(1),r->Parameter(2),r->Parameter(3),r->Parameter(4),r->Parameter(5),r->Parameter(6),r->Parameter(7));
   f13->SetNpx(1000);
//   f13->Draw("LSAME");
   sprintf(cc,"# of patterns@%d%% = %ld",percentile,Npatt);
   TText TT1;
   TT1.DrawTextNDC(0.15,0.75,cc);
   TText TT2;
   sprintf(cc,"Track multiplicity in (least:most)");
//   TT2.DrawTextNDC(0.35,0.20,cc);
   TText TT3;
   sprintf(cc,"populated patterns: (%4u:%5u)",maxminPopularity[1],maxminPopularity[0]);
//   TT3.DrawTextNDC(0.35,0.15,cc);

   if (makePlots) c->SaveAs(TString("figs/patternVS1_over_pt_")+sName+TString(".png"));

   for (unsigned long int ipatt=0; ipatt<Npatt; ++ipatt) {
	   vyPattFreq[ipatt]=(vmeanPt[ipatt]-f3->Eval(ipatt));
	   vxPattFreq[ipatt]=ipatt;
   }

//   TGraphErrors *grRel = new TGraphErrors(Npatt, vmeanPt, vdummy, vdummy, vRelrmsPt);
//   TCanvas* cRel = new TCanvas();
//   cRel->SetTitle(sTitle);
//   grRel->SetTitle(sTitle);
//   cRel->SetGridx();
//   cRel->SetGridy();
//   grRel->GetXaxis()->SetRangeUser(-0.5,0.5);
//   grRel->GetYaxis()->SetRangeUser(-0.4,0.4);
//   grRel->GetXaxis()->SetTitle("<charge/Pt>  [c/GeV]");
//   grRel->GetYaxis()->SetTitle("relative rms(1/Pt)");
//   grRel->Draw("APZ");
//	 sprintf(cc,"<relRms>@10 GeV/c = %3.3f",relrms_pt10GeV);
//   TText TT;
//   TT.DrawTextNDC(0.55,0.75,cc);
//   if (makePlots) cRel->SaveAs(TString("figs/relativeRmsVS1_over_pt_")+sName+TString(".png"));

  TCanvas* cCharge = new TCanvas();
  cCharge->SetGridx();
  cCharge->SetGridy();
  cCharge->SetLogy ();
  TGraphErrors *grCharge = new TGraphErrors(Npatt, vmeanPt, vchargeMisID, vdummy, vdummy);
  cCharge->SetTitle(sTitle);
  grCharge->SetTitle("charge MisID rate VS <charge/Pt>");
  grCharge->GetXaxis()->SetRangeUser(-0.02,0.02);
  grCharge->GetXaxis()->SetTitle("<charge/Pt> [c/GeV]");
  grCharge->GetYaxis()->SetTitle("charge MisID rate");
  grCharge->SetMarkerStyle(20);
  grCharge->SetMarkerSize(0.4);
  grCharge->Draw("APZ");
  if (makePlots) cCharge->SaveAs(TString("figs/chargeMisIDVS1_over_pt_")+sName+TString(".png"));

  TCanvas* cFreq = new TCanvas();
  cFreq->cd();
  hPattFreq->SetTitle(sTitle);
  cFreq->SetTitle("Pattern Frequency VS <charge/Pt>");
  hPattFreq->GetXaxis()->SetTitle("<charge/Pt> [c/GeV]");
  hPattFreq->GetYaxis()->SetTitle("Pattern Frequency - charge/Pt binned");
  hPattFreq->GetYaxis()->SetTitleOffset(1.4);
  hPattFreq->DrawCopy();
  if (makePlots) cFreq->SaveAs(TString("figs/patternFrequency_")+sName+TString(".png"));

  TCanvas* cLinBias = new TCanvas();
  cLinBias->SetGridx();
  cLinBias->SetGridy();
  TGraph *grLinBias = new TGraph(Npatt, vxPattFreq, vyPattFreq);
  cLinBias->SetTitle(sTitle);
  grLinBias->SetTitle("Polinomial fit - residual bias");
  grLinBias->GetXaxis()->SetTitle("pattern ID (charge/Pt sorted)");
  grLinBias->GetYaxis()->SetTitle("#Delta(<charge/Pt>) [c/GeV]");
  grLinBias->GetYaxis()->SetTitleOffset(1.4);
//  grLinBias->GetXaxis()->SetRangeUser(-fFreqRange, fFreqRange);
//  grLinBias->GetYaxis()->SetRangeUser(-0.1, 0.1);
  grLinBias->Draw("APZ");
  if (makePlots) cLinBias->SaveAs(TString("figs/patternLinearBias_")+sName+TString(".png"));

  TCanvas* cprRMS = new TCanvas();
  cprRMS->SetGridx();
  cprRMS->SetGridy();
  cprRMS->SetTitle(sTitle);
  prRMS ->SetTitle("rms(1/Pt) VS <charge/Pt>");
  prRMS->SetMarkerStyle(20);
  prRMS->SetMarkerSize(0.4);
  prRMS->SetMarkerColor(2);
  prRMS->SetLineColor(0);
  prRMS->GetXaxis()->SetTitle("<charge/Pt> [c/GeV]");
  prRMS->GetYaxis()->SetTitle("rms(1/Pt)  [c/GeV]");
  prRMS->GetYaxis()->SetTitleOffset(1.4);
  prRMS->DrawCopy();
  if (makePlots) cprRMS->SaveAs(TString("figs/rmsVS1_over_pt_")+sName+TString(".png"));

  TCanvas* ch2RMS = new TCanvas();
  ch2RMS->SetGridx();
  ch2RMS->SetGridy();
  ch2RMS->SetTitle(sTitle);
//  h2RMS->SetLeftMargin(0.3);
  h2RMS ->SetTitle("rms(charge/Pt) VS <charge/Pt>");
//  h2RMS->SetMarkerStyle(20);
//  h2RMS->SetMarkerSize(0.4);
//  h2RMS->SetMarkerColor(2);
  h2RMS->GetXaxis()->SetTitle("<charge/Pt> [c/GeV]");
  h2RMS->GetYaxis()->SetTitle("rms(charge/Pt)  [c/GeV]");
  h2RMS->GetYaxis()->SetTitleOffset(1.4);
  h2RMS->DrawCopy("col");
  prRMS->SetLineColor(0);
  prRMS->DrawCopy("same");
  if (makePlots) ch2RMS->SaveAs(TString("figs/h2rmsVS1_over_pt_")+sName+TString(".png"));

//  TCanvas* ch2RMSphi = new TCanvas();
//  ch2RMSphi->SetGridx();
//  ch2RMSphi->SetGridy();
//  ch2RMSphi->SetTitle(sTitle);
//  h2RMSphi ->SetTitle("rms(#phi_{0}) VS #phi_{0}");
//  h2RMSphi->GetXaxis()->SetTitle("#phi_{0}  [rad]");
//  h2RMSphi->GetYaxis()->SetTitle("rms(#phi_{0})  [rad]");
//  h2RMSphi->GetYaxis()->SetTitleOffset(1.4);
//  h2RMSphi->DrawCopy("col");
//  prRMSphi->SetMarkerStyle(20);
//  prRMSphi->SetMarkerSize(0.4);
//  prRMSphi->SetMarkerColor(2);
//  prRMSphi->SetLineColor(0);
//  prRMSphi->DrawCopy("same");
//  if (makePlots) ch2RMSphi->SaveAs(TString("figs/h2rmsPhiVSphi_")+sName+TString(".png"));

  TCanvas* ch2RMSphiVSpT = new TCanvas();
  ch2RMSphiVSpT->SetGridx();
  ch2RMSphiVSpT->SetGridy();
  ch2RMSphiVSpT->SetTitle(sTitle);
  h2RMSphiVSpT ->SetTitle("rms(#phi_{0}) VS <charge/Pt>");
  h2RMSphiVSpT->GetXaxis()->SetTitle("<charge/Pt> [c/GeV]");
  h2RMSphiVSpT->GetYaxis()->SetTitle("rms(#phi_{0})  [rad]");
  h2RMSphiVSpT->GetYaxis()->SetTitleOffset(1.4);
  h2RMSphiVSpT->DrawCopy("col");
  prRMSphiVSpT ->SetTitle("rms(#phi_{0}) VS <charge/Pt>");
  prRMSphiVSpT->GetXaxis()->SetTitle("<charge/Pt> [c/GeV]");
  prRMSphiVSpT->GetYaxis()->SetTitle("rms(#phi_{0})  [rad]");
  prRMSphiVSpT->GetYaxis()->SetTitleOffset(1.4);
  prRMSphiVSpT->SetMarkerStyle(20);
  prRMSphiVSpT->SetMarkerSize(0.4);
  prRMSphiVSpT->SetMarkerColor(2);
  prRMSphiVSpT->SetLineColor(0);
  prRMSphiVSpT->DrawCopy("same");
  if (makePlots) ch2RMSphiVSpT->SaveAs(TString("figs/h2rmsPhiVSInvPt_")+sName+TString(".png"));

}

void patternBankPtSmearPlot(TString sTree, int perc=99, int makePlots=1) {

	gStyle->SetOptStat(0);
	const unsigned nC=30;
	int colors[nC];
	for (unsigned i=0; i<nC; ++i) colors[i]=10+i;
	gStyle->SetPalette(nC,colors);
	patternBankPtSmear p(sTree);
	p.percentile=perc;
	p.Loop(sTree,makePlots);
	return;
}
