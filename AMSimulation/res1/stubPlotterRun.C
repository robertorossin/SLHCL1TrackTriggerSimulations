#define stubPlotter_cxx
#include "stubPlotter.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TMath.h>
#include <TArc.h>

#include "/home/rossin/Dropbox/TT/CMSSW_6_2_0_SLHC12_patch1_SL6/src/SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTRoad.h"
#include <iostream>
#include <map>

class trackParticle{
public:
	   float   eta;
	   float   phi;
	   float   pt;
	   float   vx;
	   float   vy;
	   float   vz;
	   int     charge;
	   bool    iSprimary;
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
};

void loadTT27ModuleList(vector <unsigned> &v);

void stubPlotter::Loop(unsigned int iEv,TString pName, bool makePlots)
{
//   In a ROOT session, you can do:
//      Root > .L stubPlotter.C
//      Root > stubPlotter t
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
   Long64_t nbytes = 0, nb = 0;

   vector <double> vStubsX;
   vector <double> vStubsY;
   vector <unsigned> vStubsLayer;
   vector <double> vRoadStubsX;
   vector <double> vRoadStubsY;
   vector <unsigned> vModuleId;
   vector <double> vRoadInvPt;
   vector <float>  vRoadPhi0;
   vector <unsigned>  vRoadPatternRef;
   loadTT27ModuleList(vModuleId);

   TFile* fPatterns = new TFile("/data/rossin/EOS/Neutrino_PU140_tt27_sf1_nz4_pt3_20151107/patternBank_tt27_sf1_nz4_pt3_100M.root");
   TTree *patternAttributes = (TTree*)fPatterns->Get("patternAttributes");
   float patternPhi0 = 0;
   patternAttributes->SetBranchAddress("phi_mean",&patternPhi0);

   TH1F* hnStubsPerLayer[6];
   char hName[100];
   for (unsigned iLay=0; iLay<6; ++iLay) {
	   sprintf(hName,"hnStubsLayer_%d",iLay);
	   hnStubsPerLayer[iLay] = new TH1F(hName,hName,300,0,300);
   }

   vector <trackParticle> vParticles;
   vector <trackLTF>      vTracksLTF;

   unsigned nStubsTOT = 0;
   unsigned nStubsInRoads = 0;
   unsigned int nRoads = 0;
   unsigned long evCounter=0;
   unsigned evNumber=0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {

//	   if (evCounter<iEv) continue;
	   if (evCounter>iEv) break;
//	   std::cout << jentry << "\t" << evCounter << std::endl;
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      if (jentry%1000==0) std::cout << "Processing event " << jentry << std::endl;
      // if (Cut(ientry) < 0) continue;

      vParticles .clear();
      vTracksLTF .clear();

      vStubsX    .clear();
      vStubsY    .clear();
      vStubsLayer.clear();
      vStubsLayer.resize(6,0);
      vRoadStubsX.clear();
      vRoadStubsY.clear();
      vRoadInvPt .clear();
      vRoadPhi0  .clear();
      vRoadPatternRef  .clear();

      // Fill the fitted tracks info
      for (unsigned ift=0; ift<AMTTTracks_invPt->size(); ++ift) {
    	  trackLTF ft;
    	  ft.invPt      = AMTTTracks_invPt     ->at(ift);
    	  ft.phi0       = AMTTTracks_phi0      ->at(ift);
    	  ft.pt         = AMTTTracks_pt        ->at(ift);
    	  ft.patternRef = AMTTTracks_patternRef->at(ift);
    	  vTracksLTF.push_back(ft);
      }

      // Fill the track particle info
      for (unsigned itp=0; itp<trkParts_pt->size(); ++itp) {
    	  trackParticle tp;
    	  double err=0.002;
    	  if (TMath::Abs(trkParts_pt    ->at(itp)) < 3.0  ) continue;
    	  if (TMath::Abs(trkParts_pt    ->at(itp)) > 7.  && !pName.Contains("Neutrino")) continue;
    	  if ( trkParts_phi    ->at(itp)>TMath::Pi()/2+err) continue;
    	  if ( trkParts_phi    ->at(itp)<TMath::Pi()/4-err) continue;
    	  if ( trkParts_eta    ->at(itp)< -err*80         ) continue;
    	  if ( trkParts_eta    ->at(itp)> 4.4/6+err*80    ) continue;
    	  if (!trkParts_primary->at(itp)                  ) continue;
    	  if (!trkParts_charge ->at(itp)                  ) continue;
    	  tp.pt       =trkParts_pt     ->at(itp);
    	  tp.charge   =trkParts_charge ->at(itp);
    	  tp.phi      =trkParts_phi    ->at(itp);
    	  tp.iSprimary=trkParts_primary->at(itp);
    	  vParticles.push_back(tp);
      }
      if (!vParticles.size()) continue;

	  nStubsTOT = TTStubs_x->size();
	  for (unsigned int iStub=0; iStub<nStubsTOT; ++iStub) { // looping on stubs 1
		  double stub_x1    = TTStubs_x    ->at(iStub);
		  double stub_y1    = TTStubs_y    ->at(iStub);
		  unsigned int stub_moduleId = TTStubs_modId->at(iStub);
		  for (unsigned int iMod=0; iMod<vModuleId.size(); ++iMod) {
			  if (vModuleId.at(iMod)!=stub_moduleId) continue;
			  else {
				  vStubsX.push_back(stub_x1);
				  vStubsY.push_back(stub_y1);
				  int layer = TMath::Floor(stub_moduleId/10000)-5;
				  if (layer<0 || layer>5) {
					  std::cout << "What the heck. It should not happen for TT27!" << std::endl;
					  std::cout <<stub_moduleId<< "\t" <<  layer << std::endl;
					  return;
				  }
				  ++vStubsLayer[layer];
				  break;
			  }
		  } //looping over moduleIDs
	  }// looping on stubs 1

//	  for (unsigned iLay=0; iLay<6; ++iLay) {
//		  hnStubsPerLayer[iLay]->Fill(vStubsLayer.at(iLay));
//
//	  }
//	  continue;

  	std::map <unsigned, unsigned> map_stubsInRoads;
  	std::map <unsigned, unsigned>::iterator it_map_stubs_InRoads;

	  nRoads = AMTTRoads_nstubs->size();
	  for (unsigned int iRoads=0; iRoads<nRoads; ++iRoads) { // LOOPING OVER ROADS in EVENT
	  	slhcl1tt::TTRoad road;
	  	road.stubRefs       = AMTTRoads_stubRefs     ->at(iRoads);
	  	road.superstripIds  = AMTTRoads_superstripIds->at(iRoads);
	  	road.nstubs         = AMTTRoads_nstubs       ->at(iRoads);
	  	double patternInvPt = AMTTRoads_patternInvPt ->at(iRoads);
	  	unsigned patternRef = AMTTRoads_patternRef   ->at(iRoads);
	  	vRoadInvPt.push_back(patternInvPt);
	  	patternAttributes->GetEntry(patternRef);
	  	vRoadPhi0.push_back(patternPhi0);
	  	vRoadPatternRef.push_back(patternRef);

	  	for (unsigned int iSS=0; iSS<road.superstripIds.size();++iSS) { // Looping over SS in ROAD
	  		unsigned int ssID = road.superstripIds.at(iSS);
	  		unsigned int nStubs = road.stubRefs.at(iSS).size();
	  		for (unsigned int iStub=0; iStub<nStubs; ++iStub) { // looping on stubs 1
	  			unsigned int stubID = road.stubRefs.at(iSS).at(iStub);
	  			short int stub_layer = TMath::Floor(TTStubs_modId   ->at(stubID)/10000.)-5;
	  			double stub_bend = TTStubs_trigBend->at(stubID);
	  			double stub_x1    = TTStubs_x    ->at(stubID);
	  			double stub_y1    = TTStubs_y    ->at(stubID);
	  			double stub_z1    = TTStubs_z    ->at(stubID);
	  			unsigned int stub_moduleId = TTStubs_modId->at(stubID);

	  			for (unsigned int iMod=0; iMod<vModuleId.size(); ++iMod) {
	  				if (vModuleId.at(iMod)!=stub_moduleId) continue;
	  				it_map_stubs_InRoads = map_stubsInRoads.find(stubID);
	  				if (it_map_stubs_InRoads == map_stubsInRoads.end()) {
	  					vRoadStubsX.push_back(stub_x1);
	  					vRoadStubsY.push_back(stub_y1);
	  					map_stubsInRoads.insert(make_pair(stubID,iSS));
	  					break;
	  				}
	  			} //looping over moduleIDs
	  		} // looping over stubs
	  	} //looping over SS in road
	  } //looping over roads
	  if (!vRoadStubsX.size()) {
	  	std::cout << "No road fired!!" << std::endl;
	  }
	  else {
	  	++evCounter;
	  	std::cout << "Processing event " << jentry  << "\t Found " << nStubsTOT << "\tstubs and " << map_stubsInRoads.size() << "\tin " << nRoads << "\troads." << std::endl;
	  	evNumber = jentry;
	  	nStubsInRoads = map_stubsInRoads.size();
	  }
   } //looping over entries

//   TCanvas* cStubsPerLayer = new TCanvas("cStubsPerLayer","cStubsPerLayer",0,0,1000,1000);
//   cStubsPerLayer->Divide(3,2);
//   for (unsigned iLay=0; iLay<6; ++iLay) {
//	   cStubsPerLayer->cd(iLay+1);
//	   hnStubsPerLayer[iLay]->DrawCopy();
//	   TLegend* tl = new TLegend(0.4,0.6,0.9,0.9);
//	   char cc[100];
//	   sprintf(cc,"mean = %4.1lf",hnStubsPerLayer[iLay]->GetMean());
//	   tl->AddEntry(hnStubsPerLayer[iLay],cc,"l");
//	   sprintf(cc,"RMS = %4.1lf",hnStubsPerLayer[iLay]->GetRMS());
//	   tl->AddEntry(hnStubsPerLayer[iLay],cc,"l");
//	   tl->Draw();
//   }
   char cc[100];
//   sprintf(cc,"/home/rossin/Dropbox/TT/DOCS/20151111_Luciano_Rossin_AM_Status/nStubsPerLayerAll%s",pName.Data());
//   cStubsPerLayer->SaveAs(TString(cc)+TString(".png"));

   double markerSize = 0.8;

   TGraph* grStubsXY_all   = new TGraph( vStubsX    .size(),&(vStubsX    [0]),&(vStubsY    [0]));
   grStubsXY_all->SetTitle("Stubs in Tower");
   grStubsXY_all->GetXaxis()->SetTitle("X [cm]");
   grStubsXY_all->GetYaxis()->SetTitle("Y [cm]");

   TGraph* grStubsXY_roads = new TGraph( vRoadStubsX.size(),&(vRoadStubsX[0]),&(vRoadStubsY[0]));
   TCanvas* cStubsXY_all   = new TCanvas("cStubsXY_all","cStubsXY_all",0,0,1000,1000);
//   cStubsXY_all->Divide(2,2);
//   cStubsXY_all->cd(1);
   grStubsXY_all->SetMarkerStyle(20);
   grStubsXY_all->SetMarkerSize (markerSize);
   grStubsXY_all->DrawClone("AP");
   gPad->SetGrid();
   TArc arc;
   for (unsigned itp=0; itp<vParticles.size(); ++itp) {
	   trackParticle tp = vParticles.at(itp);
	   std::cout << "Track particle[" << itp << "] pt/phi = " << tp.pt << "\t" << tp.phi << std::endl;

	   double R = 100*tp.pt/(0.3*3.8);
	   double alpha = vParticles.at(itp).phi-vParticles.at(itp).charge*TMath::Pi()/2;
	   double c0x = R*TMath::Cos(alpha);
	   double c0y = R*TMath::Sin(alpha);
	   double phiMin = TMath::Pi()/2;
	   double phiMax = TMath::Pi()+alpha;
	   if (vParticles.at(itp).charge<0) {
		   phiMin = alpha - TMath::Pi();
		   phiMax = TMath::Pi();
	   }
	   arc.SetFillStyle(0);
	   arc.SetLineWidth(0.3);
	   arc.SetLineColor(2);
	   cStubsXY_all->cd(1);
	   arc.DrawArc(c0x,c0y,R,phiMin*180/TMath::Pi(),phiMax*180/TMath::Pi(),"only");
//	   arc.DrawArc(c0x,c0y,R,phiMin*180/TMath::Pi(),phiMax*180/TMath::Pi(),"only");
   }
   TLegend* l = new TLegend(0.4,0.8,0.94,0.94);
   sprintf(cc,"Tot # stubs = %d",(int)vStubsX    .size());
   l->AddEntry(grStubsXY_all,cc,"p");
   sprintf(cc,"Tot # tracking-part [Pt>3GeV/c] = %d",(int)vParticles.size());
   TLegendEntry* tle = l->AddEntry(&arc,cc,"l");
   tle->SetLineColor(2);
   l->DrawClone();
   sprintf(cc,"/home/rossin/Dropbox/TT/DOCS/20151111_Luciano_Rossin_AM_Status/stubsXYall%s_%02d",pName.Data(),iEv);
   if (makePlots) {
	   cStubsXY_all->SaveAs(TString(cc)+TString(".pdf"));
	   cStubsXY_all->SaveAs(TString(cc)+TString(".png"));
   }

//   cStubsXY_all->cd(2);

   TCanvas* cStubsXY_roads   = new TCanvas("cStubsXY_roads","cStubsXY_roads",0,0,1000,1000);
   grStubsXY_all->SetTitle("Stubs in Tower after AM");
   grStubsXY_all->Draw("AP");
   gPad->SetGrid();
   grStubsXY_all->SetMarkerColor(0);
   grStubsXY_all->SetLineColor(1);
   grStubsXY_roads->SetMarkerColor(1);
   grStubsXY_roads->SetMarkerStyle(20);
   grStubsXY_roads->SetMarkerSize (markerSize);
   grStubsXY_roads->Draw("P");

   TLegend* l2 = new TLegend(0.4,0.8,0.9,0.9);
   sprintf(cc,"# stubs after AM = %d",(int)vRoadStubsX.size());
   l2->AddEntry(grStubsXY_roads,cc,"p");
   l2->Draw();
   sprintf(cc,"/home/rossin/Dropbox/TT/DOCS/20151111_Luciano_Rossin_AM_Status/stubsXYroads%s_%02d",pName.Data(),iEv);
   if (makePlots) {
  	 cStubsXY_roads->SaveAs(TString(cc)+TString(".pdf"));
  	 cStubsXY_roads->SaveAs(TString(cc)+TString(".png"));
   }

   for (unsigned int iTrack=0; iTrack<vTracksLTF.size(); ++iTrack) {
	   trackLTF track = vTracksLTF.at(iTrack);
	   std::cout << "Track [" << iTrack << "] pt/phi = " << track.pt << "\t" << track.phi0 << std::endl;

	   double R = TMath::Abs(100*track.pt/(0.3*3.8));
	   float  charge = TMath::Sign(1.0f,track.invPt);
	   double alpha = track.phi0-charge*TMath::Pi()/2;
	   double c0x = R*TMath::Cos(alpha);
	   double c0y = R*TMath::Sin(alpha);
	   double phiMin = TMath::Pi()/2;
	   double phiMax = TMath::Pi()+alpha;
	   if (charge<0) {
		   phiMin = alpha - TMath::Pi();
		   phiMax = TMath::Pi();
	   }
//	   TArc arc;
	   arc.SetFillStyle(0);
	   arc.SetLineWidth(0.1);
	   arc.SetLineColor(4);
//	   cStubsXY_all->cd(2);
	   arc.DrawArc(c0x,c0y,R,phiMin*180/TMath::Pi(),phiMax*180/TMath::Pi(),"only");
//	   cStubsXY_all->cd(1);
//	   arc.DrawArc(c0x,c0y,R,phiMin*180/TMath::Pi(),phiMax*180/TMath::Pi(),"only");
   }

   cStubsXY_roads->cd();
   grStubsXY_all->SetTitle("Fitted tracks");
   cStubsXY_roads->Update();
   TLegend* l3 = new TLegend(0.4,0.8,0.9,0.9);
   sprintf(cc,"# stubs after AM = %d",(int)vRoadStubsX.size());
   l3->AddEntry(grStubsXY_roads,cc,"p");
//   sprintf(cc,"Tot # tracks [Pt>3GeV/c] = %d",(int)vTracksLTF.size());
//   l3->AddEntry(&arc,cc,"l");
   l3->Draw();
   sprintf(cc,"/home/rossin/Dropbox/TT/DOCS/20151111_Luciano_Rossin_AM_Status/stubsXYroads_and_tracks%s_%02d",pName.Data(),iEv);
   if (makePlots) {
  	 cStubsXY_roads->SaveAs(TString(cc)+TString(".pdf"));
  	 cStubsXY_roads->SaveAs(TString(cc)+TString(".png"));
   }


   std::cout << "Tot # Roads  = " << nRoads << std::endl;
   std::cout << "Tot # Tracks = " << vTracksLTF.size() << std::endl;
   TH1F* hRoadInvPt      = new TH1F("hRoadInvPt","hRoadInvPt",200,-0.5,0.5);
   TH1F* hRoadPatternRef = new TH1F("hRoadPatternRef","hRoadPatternRef",1900,0,1900000);
   TH2F* h2RoadInvPt_Phi0 = new TH2F("h2RoadInvPt_Phi0","h2RoadInvPt_Phi0",200,-0.5,0.5,100,TMath::Pi()/4,TMath::Pi()/2);
   for (unsigned int iRoad=0; iRoad<vRoadInvPt.size(); ++iRoad) { // LOOPING OVER ROADS
	   std::cout << "Road [" << iRoad << "] patternId/invPt/phi = " << vRoadPatternRef.at(iRoad) << "\t" << vRoadInvPt.at(iRoad) << "\t" << vRoadPhi0.at(iRoad) <<  std::endl;
	   for (unsigned int iTrack=0; iTrack<vTracksLTF.size(); ++iTrack) {
		   if (vRoadPatternRef.at(iRoad) != vTracksLTF.at(iTrack).patternRef) continue;
		   std::cout << "Track[" << iTrack << "] patternId/invPt/phi = " << vTracksLTF.at(iTrack).patternRef << "\t" << vTracksLTF.at(iTrack).invPt << "\t" << vTracksLTF.at(iTrack).phi0 <<  std::endl;
	   }
	   hRoadInvPt      ->Fill(vRoadInvPt     .at(iRoad));
	   hRoadPatternRef ->Fill(vRoadPatternRef.at(iRoad));
	   h2RoadInvPt_Phi0->Fill(vRoadInvPt     .at(iRoad),vRoadPhi0.at(iRoad));
   }

   //   cStubsXY_all->cd(3);
//   hRoadPatternRef->DrawCopy();
//   TLegend* tl = new TLegend(0.4,0.6,0.95,0.95);
////   sprintf(cc,"evNumber = %d", evNumber);
////   tl->AddEntry(hRoadPatternRef,cc,"l");
//   sprintf(cc,"nStubs = %d", nStubsInRoads);
//   tl->AddEntry(hRoadPatternRef,cc,"l");
//   sprintf(cc,"nRoads = %d", (int)vRoadInvPt.size());
//   tl->AddEntry(hRoadPatternRef,cc,"l");
//   sprintf(cc,"nTracks = %d", (int)vTracksLTF.size());
//   tl->AddEntry(hRoadPatternRef,cc,"l");
//   sprintf(cc,"<popularity> = %7.0lf", hRoadPatternRef->GetMean());
//   tl->AddEntry(hRoadPatternRef,cc,"l");
//   tl->Draw();

   TCanvas* croadsInvPtPhi= new TCanvas("croadsInvPtPhi","croadsInvPtPhi",0,0,1000,1000);
   h2RoadInvPt_Phi0->SetTitle("Roads: #phi_{0} VS 1/Pt");
   h2RoadInvPt_Phi0->GetXaxis()->SetTitle("1/Pt [c/GeV]");
   h2RoadInvPt_Phi0->GetYaxis()->SetTitle("#phi_{0} [rad]");
   h2RoadInvPt_Phi0->DrawCopy("colz0");
   gPad->SetGrid();
   sprintf(cc,"/home/rossin/Dropbox/TT/DOCS/20151111_Luciano_Rossin_AM_Status/roadsInvPtPhi_%s_%02d",pName.Data(),iEv);
   if (makePlots) {
  	 croadsInvPtPhi->SaveAs(TString(cc)+TString(".pdf"));
  	 croadsInvPtPhi->SaveAs(TString(cc)+TString(".png"));
   }

//   sprintf(cc,"/home/rossin/Dropbox/TT/DOCS/20151111_Luciano_Rossin_AM_Status/stubsXYall%s_%02d",pName.Data(),iEv);
//   if (makePlots) {
//	   cStubsXY_all->SaveAs(TString(cc)+TString(".pdf"));
//	   cStubsXY_all->SaveAs(TString(cc)+TString(".png"));
//   }

//   grStubsXY_roads->Draw("P same");
//   TCanvas* cStubsXY_roads   = new TCanvas("cStubsXY_roads","cStubsXY_roads",0,0,1000,1000);
//   grStubsXY_all->Draw("AP");
//   grStubsXY_all->SetMarkerColor(0);
//   grStubsXY_roads->SetMarkerColor(1);
//   grStubsXY_roads->SetMarkerStyle(20);
//   grStubsXY_roads->SetMarkerSize (1.0);
//   grStubsXY_roads->Draw("P");
//   cStubsXY_roads->SaveAs("/home/rossin/Dropbox/TT/DOCS/20151111_Luciano_Rossin_AM_Status/stubsXYroads.pdf");
//   cStubsXY_roads->SaveAs("/home/rossin/Dropbox/TT/DOCS/20151111_Luciano_Rossin_AM_Status/stubsXYroads.png");
}

void loadTT27ModuleList(vector <unsigned> &v) { // this is sector 4,4 in mersi's DB
	double moduleL[] = {50230,50231,50232,50233,50234,50235,50236,50237,50330,50331,50332,50333,50334,50335,50336,50337,50430,50431,50432,50433,50434,50435,50436,50437,
			60326,60327,60328,60329,60330,60331,60332,60333,60334,60335,60426,60427,60428,60429,60430,60431,60432,60433,60434,60435,60526,60527,60528,60529,60530,60531,
			60532,60533,60534,60535,60626,60627,60628,60629,60630,60631,60632,60633,60634,60635,
			70426,70427,70428,70429,70430,70431,70432,70433,70434,70435,70436,70437,70526,70527,70528,70529,70530,70531,70532,70533,70534,70535,70536,70537,70626,70627,
			70628,70629,70630,70631,70632,70633,70634,70635,70636,70637,70726,70727,70728,70729,70730,70731,70732,70733,70734,70735,70736,70737,70826,70827,70828,70829,
			70830,70831,70832,70833,70834,70835,70836,70837,70926,70927,70928,70929,70930,70931,70932,70933,70934,70935,70936,70937,
			80611,80612,80613,80614,80615,80616,80617,80711,80712,80713,80714,80715,80716,80717,80811,80812,80813,80814,80815,80816,80817,80911,80912,80913,80914,80915,
			80916,80917,81011,81012,81013,81014,81015,81016,81017,81111,81112,81113,81114,81115,81116,81117,81211,81212,81213,81214,81215,81216,81217,
			90711,90712,90713,90714,90715,90716,90717,90718,90719,90811,90812,90813,90814,90815,90816,90817,90818,90819,90911,90912,90913,90914,90915,90916,90917,90918,
			90919,91011,91012,91013,91014,91015,91016,91017,91018,91019,91111,91112,91113,91114,91115,91116,91117,91118,91119,91211,91212,91213,91214,91215,91216,91217,
			91218,91219,91311,91312,91313,91314,91315,91316,91317,91318,91319,91411,91412,91413,91414,91415,91416,91417,91418,91419,91511,91512,91513,91514,91515,91516,
			91517,91518,91519,91611,91612,91613,91614,91615,91616,91617,91618,91619,
			100811,100812,100813,100814,100815,100816,100817,100818,100819,100820,100911,100912,100913,100914,100915,100916,100917,100918,100919,100920,101011,101012,
			101013,101014,101015,101016,101017,101018,101019,101020,101111,101112,101113,101114,101115,101116,101117,101118,101119,101120,101211,101212,101213,101214,
			101215,101216,101217,101218,101219,101220,101311,101312,101313,101314,101315,101316,101317,101318,101319,101320,101411,101412,101413,101414,101415,101416,
			101417,101418,101419,101420,101511,101512,101513,101514,101515,101516,101517,101518,101519,101520,101611,101612,101613,101614,101615,101616,101617,101618,
			101619,101620,101711,101712,101713,101714,101715,101716,101717,101718,101719,101720,101811,101812,101813,101814,101815,101816,101817,101818,101819,101820,
			101911,101912,101913,101914,101915,101916,101917,101918,101919,101920,102011,102012,102013,102014,102015,102016,102017,102018,102019,102020,102111,102112,
			102113,102114,102115,102116,102117,102118,102119,102120};
	unsigned sizeOfArray = sizeof(moduleL)/sizeof(moduleL[0]);
	for (unsigned iMod=0; iMod<sizeOfArray; ++iMod) v.push_back(moduleL[iMod]);
	return;
}

void stubPlotterRun(unsigned int iEv=1,bool makePlots=false) {
	gStyle->SetOptStat(0);
//	TString fName("/data/rossin/EOS/SinglePionTest_PU0_tt27_sf1_nz4_pt3_ml5_20150511/roads_SinglePionTest_tt27_PU0_sf1_nz4_pt3_5oo6_95c_10k.root"); TString pName("SinglePionTest_PU0");
//	TString fName("/data/rossin/EOS/SinglePionTest_PU0_tt27_sf1_nz4_pt3_ml5_20150511/results_LTF_SinglePionTest_tt27_PU0_sf1_nz4_pt3_5oo6_95c_10k.root"); TString pName("SinglePionTest_PU0");
//	TString fName("/data/rossin/EOS/Neutrino_PU140_tt27_sf1_nz4_pt3_20151107/roads_Neutrino_PU140_tt27_sf1_nz4_pt3_5oo6_95c_14k.root"); TString pName("PU140");
	TString fName("/data/rossin/EOS/Neutrino_PU140_tt27_sf1_nz4_pt3_20151107/tracks_LTF_Neutrino_PU140_tt27_sf1_nz4_pt3_5oo6_95c_14k.root"); TString pName("PU140");
//	TString fName("/data/rossin/EOS/Neutrino_PU200_tt27_sf1_nz4_pt3_20151029/roads_Neutrino_PU200_tt27_sf1_nz4_pt3_5oo6_95c.root"); TString pName("PU200");
	stubPlotter s(fName);
	s.Loop(iEv,pName,makePlots);
	return;
}
