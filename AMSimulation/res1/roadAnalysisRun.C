#define roadAnalysis_cxx
#include "roadAnalysis.h"
#include <TH2.h>
#include <TF1.h>
#include <TProfile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TMath.h>
#include <TArc.h>

//#include "/home/rossin/Dropbox/TT/CMSSW_6_2_0_SLHC12_patch1_SL6/src/SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTRoad.h"
#include <iostream>
#include <map>

struct TTRoad {
    unsigned patternRef;
    unsigned tower;
    unsigned nstubs;
    std::vector<unsigned> superstripIds;  // necessary?
    std::vector<std::vector<unsigned> > stubRefs;  // stubRefs[superstrip i][stub j]
    int missingLayer;
};

class trackLTF{
public:
	   float        pt;
	   float        eta;
	   float        rinv;
	   float        invPt;
	   float        phi0;
	   float        cottheta;
	   float        z0;
	   float        d0;
	   float        chi2;
	   int          ndof;
	   float        chi2_phi;
	   float        chi2_z;
	   bool         isGhost;
	   int          tpId;
	   int          synTpId;
	   unsigned int tower;
	   unsigned int hitBits;
	   unsigned int ptSegment;
	   unsigned int roadRef;
	   unsigned int combRef;
	   unsigned int patternRef;
	   vector<unsigned int> stubRefs;
};

// Calculate how many SS are in common (returning an integer from 0 to 6)
bool isSibling(std::vector<unsigned> lhs, std::vector<unsigned> rhs, unsigned nSS) {
	unsigned nCommon = 0;
	assert(lhs.size()==rhs.size());
	for (unsigned iSS=0; iSS<lhs.size(); ++iSS) {
		if (lhs.at(iSS)==rhs.at(iSS)) ++nCommon;
	}
	return (nCommon>=nSS);
}

double chi2pdf(Double_t *xx, Double_t *par)
{
//	std::cout << par[0] << " " << par[1] << std::endl;
   double x =xx[0]*par[0]; //reduced chi2
   return par[0]*par[1]/(TMath::Power(2,par[0]/2.0)*TMath::Gamma(par[0]/2.0)) * TMath::Power(x,par[0]/2.0-1)*TMath::Exp(-x/2);
}


void roadAnalysis::Loop(unsigned int nEv,TString pName, bool savePlots)
{

   TFile* fPatterns;
   if (pName==TString("SingleMuonTest_PU0_tt27_sf1_nz4")) {
	   fPatterns = new TFile("/data/rossin/EOS/Neutrino_PU140_tt27_sf1_nz4_pt3_20151107/patternBank_tt27_sf1_nz4_pt3_100M.root");
   }
   else if ("SingleMuonTest_PU0_tt27_scs1_nz4") {
	   fPatterns = new TFile("/data/rossin/EOS/candleStick/patternBank_tt27_scs1_nz4_pt3_300M.root");
   }
   else if ("SingleMuonTest_PU0_tt27_scs1V1_nz4") {
	   fPatterns = new TFile("/data/rossin/EOS/candleStick/patternBank_tt27_scs1V1_nz4_pt3_300M.root");
   }
   else {
	   std::cout << "ERROR. No valid file selected." << std::endl;
	   return;
   }
   TTree *patternBank       = (TTree*)fPatterns->Get("patternBank");
   unsigned short patternFreq = 0;
   patternBank->SetBranchAddress("frequency",&patternFreq);
   if (patternBank== 0) return;
   vector <unsigned short> vPatternFreq; // the key is the patternRef. filled with Frequencies (=popularities)
   Long64_t nPatterns = patternBank->GetEntriesFast();
   Long64_t nbp = 0;
   for (Long64_t jpatt=0; jpatt<nPatterns;++jpatt) {
      patternBank->GetEntry(jpatt);
//      if (ientry < 0) break;
//      nbp = fChain->GetEntry(jpatt);
      if (jpatt%100000==0) std::cout << "Loading pattern " << jpatt << std::endl;
      vPatternFreq.push_back(patternFreq);
   }


//   TTree *patternAttributes = (TTree*)fPatterns->Get("patternAttributes");
//   float patternPhi0 = 0;
//   patternAttributes->SetBranchAddress("phi_mean",&patternPhi0);

   TString splotDir("/home/rossin/Dropbox/TT/Work/figures_roadAnalysis/");
   if (savePlots) cout << "Saving plots in " << splotDir.Data() <<endl;
   TH1F    * hroadSiblings   [7];
   TH1F    * hroadMinChi2    [7];
   TH2F    * h2roadChi2VSPop [7];
   TProfile*  proadChi2VSPop [7];
   int nBinChi2=200;
   for (unsigned imiss=0; imiss<7; ++imiss) {
	   char hname[100];
	   if (imiss==0) sprintf(hname,"hroadSiblings_missLayer_None");
	   else          sprintf(hname,"hroadSiblings_missLayer_%d",(int)imiss-1);
	   hroadSiblings  [imiss] = new TH1F(hname,hname,200,-100,100);
	   if (imiss==0) sprintf(hname,"hroadMinChi2_missLayer_None");
	   else          sprintf(hname,"hroadMinChi2_missLayer_%d",(int)imiss-1);
	   hroadMinChi2   [imiss] = new TH1F(hname,hname,nBinChi2,0,20);
	   if (imiss==0) sprintf(hname,"h2roadChi2VSPop_missLayer_None");
	   else          sprintf(hname,"h2roadChi2VSPop_missLayer_%d",(int)imiss-1);
	   h2roadChi2VSPop[imiss] = new TH2F    (hname,hname,200,0,200,100,0,10);
	   if (imiss==0) sprintf(hname, "proadChi2VSPop_missLayer_None");
	   else          sprintf(hname, "proadChi2VSPop_missLayer_%d",(int)imiss-1);
	    proadChi2VSPop[imiss] = new TProfile(hname,hname,200,0,200,    0,100);
   }
   TH1F* hmissingLayer           = new TH1F("hmissingLayer"          ,"hmissingLayer"          ,7,-1,6);
   TH1F* hsiblingsByMissingLayer = new TH1F("hsiblingsByMissingLayer","hsiblingsByMissingLayer",7,-1,6);

   if (fChain == 0) return;
   Long64_t nentries = min(fChain->GetEntriesFast(),(long long)nEv);
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      if (jentry%5000==0) std::cout << "Processing event " << jentry << std::endl;

      std::multimap<unsigned,trackLTF> mapRoadTrack; // the key is the patternRef. a road (=pattern) will contain multiple tracks
      // Fill the fitted tracks info
      for (unsigned ift=0; ift<AMTTTracks_invPt->size(); ++ift) {
    	  trackLTF ft;
    	  ft.invPt      = AMTTTracks_invPt     ->at(ift);
    	  ft.phi0       = AMTTTracks_phi0      ->at(ift);
    	  ft.pt         = AMTTTracks_pt        ->at(ift);
    	  ft.patternRef = AMTTTracks_patternRef->at(ift);
    	  ft.stubRefs   = AMTTTracks_stubRefs  ->at(ift);
    	  ft.chi2       = AMTTTracks_chi2      ->at(ift);
    	  ft.ndof       = AMTTTracks_ndof      ->at(ift);
    	  mapRoadTrack.insert(std::make_pair(ft.patternRef,ft));
      }

	  unsigned nRoads = AMTTRoads_nstubs->size();
	  for (unsigned int iRoads=0; iRoads<nRoads; ++iRoads) { // LOOPING OVER ROADS in EVENT
	  	TTRoad road;
	  	road.stubRefs       = AMTTRoads_stubRefs     ->at(iRoads);
	  	road.superstripIds  = AMTTRoads_superstripIds->at(iRoads);
	  	road.nstubs         = AMTTRoads_nstubs       ->at(iRoads);
	  	road.patternRef     = AMTTRoads_patternRef   ->at(iRoads);
	  	road.missingLayer   = -1;
	  	double patternInvPt = AMTTRoads_patternInvPt ->at(iRoads);
	  	for (unsigned int iSS=0; iSS<road.superstripIds.size();++iSS) { // Looping over SS in ROAD
	  		unsigned int ssID = road.superstripIds.at(iSS);
	  		unsigned int nStubs = road.stubRefs.at(iSS).size();
	  		if (!nStubs) road.missingLayer=iSS;
	  	}
	  	for (unsigned int jRoads=iRoads+1; jRoads<nRoads; ++jRoads) { // LOOPING OVER ROADS in EVENT
	  		bool isSibl = isSibling(road.superstripIds, AMTTRoads_superstripIds->at(jRoads), 5);
	  		if (isSibl) {
	  			int deltaSS = 0;
	  			for (unsigned iSS=0; iSS<road.superstripIds.size(); ++iSS) {
	  				deltaSS += road.superstripIds.at(iSS)-AMTTRoads_superstripIds->at(jRoads).at(iSS);
	  			}
	  			hroadSiblings[road.missingLayer+1]->Fill(deltaSS%100);
	  			hsiblingsByMissingLayer->Fill(road.missingLayer);
	  		}
	  	}
	    std::multimap<unsigned,trackLTF>::iterator it_mapRoadTrack;
	    double roadMinChi2 = 9999;
	    for (it_mapRoadTrack=mapRoadTrack.equal_range(road.patternRef).first; it_mapRoadTrack!=mapRoadTrack.equal_range(road.patternRef).second; ++it_mapRoadTrack) {
	    	double roadChi2 = (*it_mapRoadTrack).second.chi2/(*it_mapRoadTrack).second.ndof;
	    	roadMinChi2 = roadChi2<roadMinChi2 ? roadChi2 : roadMinChi2;
	    }
	    hroadMinChi2[road.missingLayer+1]->Fill(roadMinChi2);
	  	hmissingLayer->Fill(road.missingLayer);

	  	h2roadChi2VSPop[road.missingLayer+1]->Fill(vPatternFreq.at(road.patternRef),roadMinChi2);
	  	 proadChi2VSPop[road.missingLayer+1]->Fill(vPatternFreq.at(road.patternRef),roadMinChi2);
	  }
   }
//   TCanvas* cmissingLayer = new TCanvas("cmissingLayer","cmissingLayer",0,0,700,700);
//   hmissingLayer->SetMinimum(0);
//   hmissingLayer->Draw();
   TCanvas* croadSiblings = new TCanvas("croadSiblings","croadSiblings", 50, 50,1200,800);
   croadSiblings->Divide(3,3);
   TCanvas* croadMinChi2  = new TCanvas("croadMinChi2" ,"croadMinChi2" ,100,100,1200,800);
   croadMinChi2 ->Divide(3,3);
   TCanvas* croadMinChi2VSPop  = new TCanvas("croadMinChi2VSPop" ,"croadMinChi2VSPop" ,150,150,1200,800);
   croadMinChi2VSPop ->Divide(3,3);
   for (unsigned imiss=0; imiss<7; ++imiss) {
	   if (imiss) croadSiblings->cd(imiss);
	   else       croadSiblings->cd(7);
	   hroadSiblings[imiss]->Draw();
	   if (imiss) croadMinChi2 ->cd(imiss);
	   else       croadMinChi2 ->cd(7);
	   gPad->SetLogy();
	   hroadMinChi2 [imiss]->Draw();
	   TF1 *f1 = new TF1("chi2pdf",chi2pdf,0,10,2); //par[0] is ndof, par[1] is normalization
	   if (imiss) f1->SetParameter(0,6); // 5/6
	   else       f1->SetParameter(0,8); // 6/6
	   f1->SetParameter(1,hroadMinChi2 [imiss]->Integral(0,nBinChi2+1,"width"));
	   std::cout << hroadMinChi2 [imiss]->GetName() << " " << hroadMinChi2 [imiss]->Integral(0,201,"width") << std::endl;
	   f1->SetLineColor(2);
	   f1->DrawClone("same");

	   if (imiss) croadMinChi2VSPop ->cd(imiss);
	   else       croadMinChi2VSPop ->cd(7);
	   gPad->SetLogz();
	   h2roadChi2VSPop[imiss]->Draw("colz0");
	    proadChi2VSPop[imiss]->SetMarkerStyle(20);
	    proadChi2VSPop[imiss]->SetMarkerSize(0.3);
	    proadChi2VSPop[imiss]->SetMarkerColor(1);
	    proadChi2VSPop[imiss]->SetLineColor(1);
	    proadChi2VSPop[imiss]->Draw("same");
   }
   croadSiblings->cd(8);
   hsiblingsByMissingLayer->SetMinimum(0);
   hsiblingsByMissingLayer->Draw();
   croadSiblings->cd(9);
   hmissingLayer->SetMinimum(0);
   hmissingLayer->Draw();

   if (savePlots) {
	   TString ext[2] = {".pdf",".png"};
	   for (unsigned iext=0; iext<2; ++iext) {
		   croadSiblings     ->SaveAs(splotDir+pName+TString("/")+TString("hroadSiblings")     +TString("_")+pName+ext[iext]);
		   croadMinChi2      ->SaveAs(splotDir+pName+TString("/")+TString( "hroadMinChi2")     +TString("_")+pName+ext[iext]);
		   croadMinChi2VSPop ->SaveAs(splotDir+pName+TString("/")+TString("h2roadMinChi2VSPop")+TString("_")+pName+ext[iext]);
	   }
   }
}

void roadAnalysisRun(bool savePlots=false,unsigned int nEv=9999999) {
	gStyle->SetOptStat(111111);
//	TString fName("/data/rossin/EOS/SingleMuonTest_PU0_tt27_sf1_nz4_pt3_ml5_20150511/tracks_LTF_SingleMuonTest_PU0_tt27_sf1_nz4_pt3_5oo6_20150511.root"); TString pName("SingleMuonTest_PU0_tt27_sf1_nz4");
	TString fName("/data/rossin/EOS/candleStick/tracks_LTF_SingleMuonTest_PU0_tt27_scs1_nz4_pt3_5oo6_95c_chi100_300M.root"); TString pName("SingleMuonTest_PU0_tt27_scs1_nz4");
//	TString fName("/data/rossin/EOS/candleStick/tracks_LTF_SingleMuonTest_PU0_tt27_scs1V1_nz4_pt3_5oo6_95c_chi100_300M.root"); TString pName("SingleMuonTest_PU0_tt27_scs1V1_nz4");

	roadAnalysis s(fName);
	s.Loop(nEv,pName,savePlots);
	return;

}
