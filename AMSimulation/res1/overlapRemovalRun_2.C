#define overlapRemoval_cxx
#include "overlapRemoval.h"
#include <TH2.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLatex.h>
#include <TVector3.h>
#include <TLine.h>
#include <TArc.h>
#include <TLegend.h>

#include <iostream>
#include <fstream>
#include <iomanip> // for setw

void genTrackPropagate(double *genTrackPt_Phi0_CotTheta, double r, double *genTrackXYZ);
void loadTT27ModuleList(vector <unsigned> &v);
void loadTT35ModuleList(vector <unsigned> &v);
void loadTT43ModuleList(vector <unsigned> &v);
void findFactors(unsigned num, unsigned &f1, unsigned &f2);

void overlapRemoval::Loop(Long64_t nMax,bool makePlots,float minPt,TString sProc,bool makeRX,int iTT)
{

	if (fChain == 0) return;

//	TFile* fFrames = new TFile("/home/rossin/Dropbox/TT/Work/figures_stubOverlapRemoval/stubOverlapHistosFramesOnlyTree_0p2_0p2_0p2_0p2_0p2_0p2.root"); TString outDir("/home/rossin/Dropbox/TT/Work/figures_stubOverlapRemoval/");
	TFile* fFrames = new TFile("/home/rossin/Dropbox/TT/Work/figures_stubOverlapRemoval/stubOverlapHistosFramesOnlyTree_0p8_0p8_0p6_0p8_0p6_0p5.root"); TString outDir("/home/rossin/Dropbox/TT/Work/figures_stubOverlapRemoval/0p8_0p8_0p6_0p8_0p6_0p5/");

//	TTree* tmodule = (TTree*)fFrames->Get("tmodule");

//  // Declaration of leaf types
//  Int_t           id;
//  Int_t           layer;
//  Int_t           x1;
//  Int_t           y1;
//  Int_t           x2;
//  Int_t           y2;
//
//  // List of branches
//  TBranch        *b_id   ;   //!
//  TBranch        *b_layer;   //!
//  TBranch        *b_x1   ;   //!
//  TBranch        *b_y1   ;   //!
//  TBranch        *b_x2   ;   //!
//  TBranch        *b_y2   ;   //!
//
//  tmodule->SetBranchAddress("id", &id, &b_id);
//  tmodule->SetBranchAddress("layer", &layer, &b_layer);
//  tmodule->SetBranchAddress("x1", &x1, &b_x1);
//  tmodule->SetBranchAddress("y1", &y1, &b_y1);
//  tmodule->SetBranchAddress("x2", &x2, &b_x2);
//  tmodule->SetBranchAddress("y2", &y2, &b_y2);
//
//  Long64_t tentries = tmodule->GetEntriesFast();
//
//  std::map<int,vector<int> >           map_moduleFrames;
//  std::map<int,vector<int> >::iterator map_moduleFrames_it;
//
//	for (Long64_t jentry=0; jentry<tentries;jentry++) {
//		Long64_t ientry = tmodule->LoadTree(jentry);
//		b_id   ->GetEntry(jentry);
//		b_layer->GetEntry(jentry);
//		b_x1   ->GetEntry(jentry);
//		b_y1   ->GetEntry(jentry);
//    b_x2   ->GetEntry(jentry);
//    b_y2   ->GetEntry(jentry);
//
//    vector <int> v;
//    v.push_back(layer);
//    v.push_back(x1   );
//    v.push_back(y1   );
//    v.push_back(x2   );
//    v.push_back(y2   );
//
//    map_moduleFrames.insert(make_pair(id,v));
//	}
//	std::cout << "Loaded " << map_moduleFrames.size() << " overlapping areas." << std::endl;

	vector <unsigned> vModuleId;
	if (iTT==27) loadTT27ModuleList(vModuleId);
	if (iTT==35) loadTT35ModuleList(vModuleId);
	if (iTT==43) loadTT43ModuleList(vModuleId);
	std::cout<< vModuleId.size() << std::endl;
	std::map<unsigned,unsigned> moduleIdMap;
	const unsigned nMod = vModuleId.size();
//	TH2* h2stubLocalCoordAll [nMod];
//	TH2* h2stubLocalCoordDupl[nMod];
//	TH2* h2stubLocalCoordEff[nMod];
	TGraph* gStubEtaPhi               [6];
	TGraph* gStubXY                   [5];
	TGraph* gStubEtaPhi_OverlapRemoved[6];
	TGraph* gStubRZ                    ;
	TGraph* gStubXYBarrel              ;
//	unsigned nModules[6] = {0,0,0,0,0,0};
	int nBinTIB = 960/2;
	int nBinTOB = 1016/2;
	for (unsigned iMod=0; iMod<vModuleId.size(); ++iMod) {
		moduleIdMap[vModuleId[iMod]]=iMod;
//
//		char hName [100];
//		char hName2[100];
//		short int iLay = TMath::Floor(vModuleId[iMod]/10000.)-5;
////		++nModules[iLay];
//		sprintf(hName ,"h2stubLocalCoordAll_ModuleId%d",vModuleId[iMod]);
//		sprintf(hName2,"h2stubLocalCoordDupl_ModuleId%d",vModuleId[iMod]);
//		if (iLay<3) {
//			h2stubLocalCoordAll [iMod] = new TH2F(hName ,hName ,nBinTIB,0,960,32,0,32);
//			h2stubLocalCoordDupl[iMod] = new TH2F(hName2,hName2,nBinTIB,0,960,32,0,32);
//		}
//		else        {
//			h2stubLocalCoordAll [iMod] = new TH2F(hName ,hName ,nBinTOB,0,1016,2,0,2);
//			h2stubLocalCoordDupl[iMod] = new TH2F(hName2,hName2,nBinTOB,0,1016,2,0,2);
//		}
	}
//	for (unsigned iLay=0; iLay<6; ++iLay) std::cout << nModules[iLay] << "\t" ;
//	std::cout << std::endl;

//
//	TFile* fRadii = 0; bool isfOpen = false;
//	fRadii = new TFile("barrelRadii.root");
//	isfOpen = fRadii->IsOpen();
//	if (!isfOpen) {
//		cout << "ERROR. Not able to load the radii file. Exiting..." << endl;
//		return;
//	}
//	TH1* hRadii = (TH1S*) fRadii->Get("hRadii");
//	if (!hRadii) {
//		std::cout << "ERROR loading hRadii. Exiting..." << std::endl;
//	}
//	TCanvas* cRadii = new TCanvas("cRadii","cRadii",80,80,900,900);
//	hRadii->DrawCopy();
//

	// external Radii for each layer. from the hRadii plot
	vector <float> vBarrelRadii;
	vBarrelRadii.push_back(24.3  );
	vBarrelRadii.push_back(37.125);
	vBarrelRadii.push_back(52.1  );
	vBarrelRadii.push_back(69.775);
	vBarrelRadii.push_back(89.925);
	vBarrelRadii.push_back(109.125);

//	TH1* h1StubTrackResiduals       [6][3];
//	TH1* h1StubTrackResidualsXYZ[6][3];
//	TH1* h1StubTrackResidualsXYZDupl[6][3];
	TString sXYZ[3];
	sXYZ[0] = TString("X");
	sXYZ[1] = TString("Y");
	sXYZ[2] = TString("Z");

	TH2* h2stubLocalCoord    [6];
	TH2* h2stubLocalCoordDisk[5];

	vector <float> vStubEta              [6];
	vector <float> vStubPhi              [6];
//	vector <float> vStubEtaOverlapRemoved[6];
//	vector <float> vStubPhiOverlapRemoved[6];
	vector <float> vStubXDisk            [5];
	vector <float> vStubYDisk            [5];
	vector <float> vStubR                ;
	vector <float> vStubZ                ;
	vector <float> vStubXBarrel          ;
	vector <float> vStubYBarrel          ;

	TH1* hStubEta              [6];
	TH1* hStubPhi              [6];
//	TH1* hStubEtaOverlapRemoved[6];
//	TH1* hStubPhiOverlapRemoved[6];
	TH1* hStubRDisk            [5];
	TH1* hStubPhiDisk          [5];

	for (unsigned iLay=0; iLay<6; ++iLay) {
		char hName[100];
		sprintf(hName,"h2stubLocalCoord_Layer%d",iLay);
		if (iLay<3) h2stubLocalCoord[iLay] = new TH2D(hName,hName,nBinTIB,0,960,32,0,32);
		else        h2stubLocalCoord[iLay] = new TH2D(hName,hName,nBinTOB,0,1016,2,0,2);

		sprintf(hName,"h1StubEta_Layer_%d"               ,iLay);
		if (iTT==27) hStubEta              [iLay] = new TH1F(hName,hName,200,-0.2,1.0);
		if (iTT==35) hStubEta              [iLay] = new TH1F(hName,hName,200, 0.5,1.5);
		if (iTT==43) hStubEta              [iLay] = new TH1F(hName,hName,200, -10,10);
//		sprintf(hName,"h1StubEta_Layer_%d_OverlapRemoved",iLay);
//		if (iTT==27) hStubEtaOverlapRemoved[iLay] = new TH1F(hName,hName,200,-0.2,1.0);
//		if (iTT==35) hStubEtaOverlapRemoved[iLay] = new TH1F(hName,hName,200, 0.5,1.5);
//		if (iTT==43) hStubEtaOverlapRemoved[iLay] = new TH1F(hName,hName,200, -10,10);
		sprintf(hName,"h1StubPhi_Layer_%d"               ,iLay);
		hStubPhi              [iLay] = new TH1F(hName,hName,500, 0.6 ,1.8);
//		sprintf(hName,"h1StubPhi_Layer_%d_OverlapRemoved",iLay);
//		hStubPhiOverlapRemoved[iLay] = new TH1F(hName,hName,500, 0.6 ,1.8);

//		for (unsigned iXYZ=0; iXYZ<3; ++iXYZ) {
//			sprintf(hName,"h1StubTrackResiduals_Layer%d_%s",iLay,sXYZ[iXYZ].Data());
//			h1StubTrackResiduals[iLay][iXYZ] = new TH1F(hName,hName,200,-2,2);
//			sprintf(hName,"h1StubTrackResidualsXYZ_Layer%d_%s",iLay,sXYZ[iXYZ].Data());
//			h1StubTrackResidualsXYZ[iLay][iXYZ] = new TH1F(hName,hName,200,-2,2);
//			sprintf(hName,"h1StubTrackResidualsXYZDupl_Layer%d_%s",iLay,sXYZ[iXYZ].Data());
//			h1StubTrackResidualsXYZDupl[iLay][iXYZ] = new TH1F(hName,hName,200,-2,2);
//		}
	}

	vector <float> vrPS = {
			245.5,
			264.4,
			284.8,
			306.8,
			330.5,
			355.9,
			383.4,
			412.9,
			444.8,
			479.1,
			516.0,
			555.8,
			598.6
	};

	vector <float> vr2S = {
			598.6 ,
			642.2 ,
			685.0 ,
			729.6 ,
			773.4 ,
			815.3 ,
			857.0 ,
			880.0,
			941.6 ,
			990.0 ,
			1022.5,
			1040.0,
			1100.0
	};
	vector <double> vrPScm;
	vector <double> vr2Scm;
	for (unsigned i=0; i<vrPS.size(); ++i) vrPScm.push_back(vrPS[i]/10);
	for (unsigned i=0; i<vr2S.size(); ++i) vr2Scm.push_back(vr2S[i]/10);

//	for (unsigned i=0; i<vrPScm.size(); ++i) std::cout << vrPScm.at(i) << ","  ;
//	for (unsigned i=1; i<vr2Scm.size(); ++i) std::cout << vr2Scm.at(i) << ","  ;
//	std::cout << std::endl;

	const int nPhi = 48;


	TH1F* h1OccupancyVsR_PS_Disk[5];
	TH1F* h1OccupancyVsR_2S_Disk[5];
	TH2F* h2Occupancy_PS_Disk   [5];
	TH2F* h2Occupancy_2S_Disk   [5];

	double pi = TMath::Pi();

	for (unsigned iLay=0; iLay<5; ++iLay) {
		char hName[100];
		sprintf(hName,"h2stubLocalCoord_Layer%d",iLay+11);
		h2stubLocalCoordDisk[iLay] = new TH2D(hName,hName,nBinTIB,0,960,32,0,32);
		sprintf(hName,"h1StubRDisk_Layer_%d"               ,iLay+11);
		hStubRDisk             [iLay] = new TH1F(hName,hName,1000,20,120);
		sprintf(hName,"h1StubPhi_Layer_%d"               ,iLay+11);
		hStubPhiDisk           [iLay] = new TH1F(hName,hName,1000, 0.6 ,1.8);
		sprintf(hName,"h1OccupancyVsR_PS_Disk_Layer_%d",iLay+11);
		h1OccupancyVsR_PS_Disk[iLay] = new TH1F(hName,hName,vrPScm.size()-1,&(vrPScm[0]));
		sprintf(hName,"h1OccupancyVsR_2S_Disk_Layer_%d",iLay+11);
		h1OccupancyVsR_2S_Disk[iLay] = new TH1F(hName,hName,vr2Scm.size()-1,&(vr2Scm[0]));
		sprintf(hName,"h2Occupancy_PS_Disk_Layer_%d",iLay+11);
		h2Occupancy_PS_Disk[iLay] = new TH2F(hName,hName,vrPScm.size()-1,&(vrPScm[0]),nPhi*8,-pi,pi);
		sprintf(hName,"h2Occupancy_2S_Disk_Layer_%d",iLay+11);
		h2Occupancy_2S_Disk[iLay] = new TH2F(hName,hName,vr2Scm.size()-1,&(vr2Scm[0]),nPhi*8,-pi,pi);
	}



	TH1* h1StubRadius = new TH1F("h1StubRadius","h1StubRadius",4800,0,120);

	int counter = 0;

	Long64_t nentries = fChain->GetEntriesFast();
	nentries = TMath::Min(nentries,nMax);

	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
//		if (!genParts_pt->size()   ) continue;
//		if (genParts_pt->size()!=1   ) continue;

//		vector <float> vStubsR[6];
//		float aaDeltaXYZCuts[6][3] = {
//				{0.3,0.2,0.4},
//				{0.4,0.3,0.5},
//				{0.6,0.4,0.6},
//				{0.9,0.6,3.0},
//				{1.2,0.8,4.0},
//				{1.4,1.0,4.0}
//		};

//		double genTrackPt_Phi0[2] = {-genParts_pt->at(0)*genParts_charge->at(0), genParts_phi->at(0)};
//		if (genTrackPt_Phi0[0]<minPt) continue;

		std::map<short,stub> stubMapClean;

//		if (TTStubs_r->size()>6) std::cout << jentry << "\t" <<  TTStubs_r->size() << std::endl;

		for (unsigned iStub=0; iStub<TTStubs_r->size(); ++iStub) {

			unsigned moduleId = TTStubs_modId   ->at(iStub);
			std::map<unsigned,unsigned>::iterator itMod = moduleIdMap.find(moduleId);
			if (itMod == moduleIdMap.end()) continue; // does not belong to TT

			short int stub_layer = TMath::Floor(TTStubs_modId   ->at(iStub)/10000.)-5;
			if (stub_layer<0 || stub_layer>10) continue; // look only in the barrel and forward
			bool isBarrel = true;
			if (stub_layer>5) isBarrel = false;


			double stubLocalX = TTStubs_coordx->at(iStub);
			double stubLocalY = TTStubs_coordy->at(iStub);
			if (isBarrel) h2stubLocalCoord[stub_layer]->Fill(stubLocalX,stubLocalY);
			bool stubToBeKept = true;

//			if (iTT==27) {
//				map_moduleFrames_it = map_moduleFrames.find(moduleId);
//				if (map_moduleFrames_it == map_moduleFrames.end()) {
//					continue;
//				}
//				else {
//					int x1 = map_moduleFrames_it->second[1];
//					int y1 = map_moduleFrames_it->second[2];
//					int x2 = map_moduleFrames_it->second[3];
//					int y2 = map_moduleFrames_it->second[4];
//					if      (stubLocalX < x1) stubToBeKept = false;
//					else if (stubLocalX > x2) stubToBeKept = false;
//					else if (stubLocalY < y1) stubToBeKept = false;
//					else if (stubLocalY > y2) stubToBeKept = false;
//				}
//			}

			double stubRadius = TTStubs_r->at(iStub);
			if (isBarrel) h1StubRadius->Fill(stubRadius);
			double stubGlobalX = TTStubs_x->at(iStub);
			double stubGlobalY = TTStubs_y->at(iStub);
			double stubGlobalZ = TTStubs_z->at(iStub);

			TVector3 stub3(stubGlobalX,stubGlobalY,stubGlobalZ);
			float stubEta = stub3.Eta();
			float stubPhi = stub3.Phi();

			//			if (stubGlobalX<0 || stubGlobalY<0) continue; // First quadrant Patch. just to speed up the plotting

//			if ((moduleId)%2==0) continue;

			if (!isBarrel) {
				if (stubRadius<vr2S[0]/10) {
					h1OccupancyVsR_PS_Disk[stub_layer-6]->Fill(stubRadius);
					h2Occupancy_PS_Disk   [stub_layer-6]->Fill(stubRadius,stubPhi);
				}
				else                       {
					h1OccupancyVsR_2S_Disk[stub_layer-6]->Fill(stubRadius);
					h2Occupancy_2S_Disk   [stub_layer-6]->Fill(stubRadius,stubPhi);
				}
			}

			if (isBarrel) {
				vStubEta[stub_layer].push_back(stubEta);
				vStubPhi[stub_layer].push_back(stubPhi);
				hStubEta[stub_layer]->Fill(stubEta);
				hStubPhi[stub_layer]->Fill(stubPhi);
				vStubXBarrel        .push_back(stubGlobalX);
				vStubYBarrel        .push_back(stubGlobalY);
//				if (stubToBeKept) {
//					vStubEtaOverlapRemoved[stub_layer].push_back(stubEta);
//					vStubPhiOverlapRemoved[stub_layer].push_back(stubPhi);
//					hStubEtaOverlapRemoved[stub_layer]->Fill(stubEta);
//					hStubPhiOverlapRemoved[stub_layer]->Fill(stubPhi);
//				}
			}
			else {
				vStubXDisk  [stub_layer-6].push_back(stubGlobalX);
				vStubYDisk  [stub_layer-6].push_back(stubGlobalY);
				hStubRDisk  [stub_layer-6]->Fill(stubRadius);
				hStubPhiDisk[stub_layer-6]->Fill(stubPhi);
			}
			vStubZ.push_back(stubGlobalZ);
			vStubR.push_back(stubRadius );

//			double genTrackXYZ[3];
//			genTrackPropagate(genTrackPt_Phi0,TTStubs_r->at(iStub)/100.,genTrackXYZ);
//			double genPart_CotTheta = TMath::SinH(genParts_eta->at(0));
//			genTrackXYZ[0] += genParts_vx->at(0)/100.;
//			genTrackXYZ[1] += genParts_vy->at(0)/100.;
//			genTrackXYZ[2]  = stubRadius*genPart_CotTheta/100.+genParts_vz->at(0)/100.;
//
//			double stubTrackResiduals[3];
//			stubTrackResiduals[0] = stubGlobalX-genTrackXYZ[0]*100.;
//			stubTrackResiduals[1] = stubGlobalY-genTrackXYZ[1]*100.;
//			stubTrackResiduals[2] = stubGlobalZ-genTrackXYZ[2]*100.;
//
//			bool keepStub = true;
//			for (unsigned iXYZ=0; iXYZ<3; ++iXYZ) {
//				h1StubTrackResiduals[stub_layer][iXYZ]->Fill(stubTrackResiduals[iXYZ]);
//				if (TMath::Abs(stubTrackResiduals[iXYZ])>aaDeltaXYZCuts[stub_layer][iXYZ]) {
//					keepStub = false;
//					continue;
//				}
//			}
//			if (keepStub) {
//				for (unsigned iXYZ=0; iXYZ<3; ++iXYZ) h1StubTrackResidualsXYZ[stub_layer][iXYZ]->Fill(stubTrackResiduals[iXYZ]);
//			}
//			else continue;
//
//			double stubResidualXY2 = stubTrackResiduals[0]*stubTrackResiduals[0]+stubTrackResiduals[1]*stubTrackResiduals[1];
//			short stubLayerSublayer = hRadii->GetBinContent(hRadii->FindBin(stubRadius));
////			if (!stubLayerSublayer) {
////				cout << stubRadius << "\t" << hRadii->FindBin(stubRadius) << "\t" << stubLayerSublayer << endl;
////				return;
////			}
//			std::map<short,stub>::iterator it = stubMapClean.find(stubLayerSublayer);
//			if (it!=stubMapClean.end()) { // if there is already a stub in the sublayer
//				if (stubResidualXY2 < it->second.deltaXY) { // check whether it is closer to the track.
//					stubMapClean.erase(it); // if so remove the other one
//				}
//			}
//			stubMapClean.insert(std::make_pair(stubLayerSublayer,stub(iStub,stub_layer,(int)(stubLayerSublayer)%10,stubRadius,stubResidualXY2))); // insert the stub. if there is another one is not the closet one. it will not be inserted
//
//
//			//		   std::cout << setw(7) << iStub << setw(15)
//			//				   <<  100*genTrackXYZ[0] << setw(15) <<  100*genTrackXYZ[1]  << setw(15) <<  100*genTrackXYZ[2]  << setw(15)
//			//				   <<  TTStubs_r->at(iStub)*TMath::Cos(TTStubs_phi->at(iStub))  << setw(15)
//			//				   <<  TTStubs_r->at(iStub)*TMath::Sin(TTStubs_phi->at(iStub))  << setw(15)
//			//        		   <<  TTStubs_z->at(iStub) << std::endl;
		} // end looping over stubs
//
//		vector<unsigned> vStubsSubLayer[6];
//
//		for (std::map<short,stub>::iterator it = stubMapClean.begin(); it!=stubMapClean.end(); ++it) {
//			vStubsSubLayer[it->second.layer].push_back(it->second.sublayer);
//		}
//		for (unsigned iLay=0; iLay<6; ++iLay) { // sorting each vector. innermost sublayer will be in [0]
//			std::sort(vStubsSubLayer[iLay].begin(),vStubsSubLayer[iLay].end());
//		}
//
//		for (std::map<short,stub>::iterator it = stubMapClean.begin(); it!=stubMapClean.end(); ++it) {
////			if (stubMapClean.size()>48) std::cout << stubMapClean.size() << "\t"
////					<< it->second.iStub << "\t"
////					<< it->second.layer << "."
////					<< it->second.sublayer << "\t"
////					<< it->second.r << "\t"
////					<< TTStubs_coordx->at(it->second.iStub) << "\t"
////					<< TTStubs_coordy->at(it->second.iStub) << "\t"
////					<< it->second.deltaXY << "\t"
////					<< std::endl;
//			unsigned iStub = it->second.iStub;
//
//			std::map<unsigned,unsigned>::iterator itMod = moduleIdMap.find(TTStubs_modId->at(iStub));
//			if (itMod==moduleIdMap.end()) continue;
//			unsigned iMod = moduleIdMap.find(TTStubs_modId->at(iStub))->second;
//			h2stubLocalCoordAll[iMod]->Fill(TTStubs_coordx->at(iStub),TTStubs_coordy->at(iStub));
//			// Filling dupl histo if and only if the hit is in the innermost sublayer
//			if (vStubsSubLayer[it->second.layer][0]==it->second.sublayer) h2stubLocalCoordDupl[iMod]->Fill(TTStubs_coordx->at(iStub),TTStubs_coordy->at(iStub));
//
////    OLD WAY. getting regions to remove.
////			if (vStubsSubLayer[it->second.layer].size()>1) {
////				if (vStubsSubLayer[it->second.layer][0]<it->second.sublayer) h2stubLocalCoordDupl[iMod]->Fill(TTStubs_coordx->at(iStub),TTStubs_coordy->at(iStub));
////			}
//		}


		if (jentry%100000==0 ) {
			std::cout << "Processing event " << jentry << setw(15) <<  std::endl;
		}
	}
//	fRadii->Close();

	std::cout << "Total # modules in TT: " << moduleIdMap.size() << std::endl;


//	TCanvas* cstubTrackResiduals = new TCanvas("cstubTrackResiduals","cstubTrackResiduals",0,0,900,900);
//	cstubTrackResiduals->Divide(6,3);
//	int count=0;
//	for (unsigned iXYZ=0; iXYZ<3; ++iXYZ) {
//		for (unsigned iLay=0; iLay<6; ++iLay) {
//			cstubTrackResiduals->cd(++count);
//			gPad->SetLogy();
//			h1StubTrackResiduals[iLay][iXYZ]->DrawCopy();
//			h1StubTrackResidualsXYZ[iLay][iXYZ]->SetLineColor(2);
//			h1StubTrackResidualsXYZ[iLay][iXYZ]->DrawCopy("same");
//		}
//	}

//	TCanvas* cStubRadius = new TCanvas("cStubRadius","cStubRadius",50,50,1700,900);
//	cStubRadius->SetLogy();
//	h1StubRadius->DrawCopy();
//	if (makePlots) {
//		char cc[100];
//		sprintf(cc,"stubRadii");
//		cStubRadius->SaveAs(outDir+TString(cc)+TString(".C"));
//		cStubRadius->SaveAs(outDir+TString(cc)+TString(".pdf"));
//		cStubRadius->SaveAs(outDir+TString(cc)+TString(".png"));
//	}


//	TCanvas* c2stubLocalCoord = new TCanvas("c2stubLocalCoord","c2stubLocalCoord",50,0,900,900);
//	c2stubLocalCoord->Divide (3,2);
//	for (unsigned iLay=0; iLay<6; ++iLay) {
//		c2stubLocalCoord->cd(iLay+1);
//		h2stubLocalCoord[iLay]->DrawCopy("colz0");
//	}


//	TFile* fOutHistos = new TFile(outDir+TString("stubOverlapHistos")+TString(".root"),"recreate");


//	char cMinPt[10];
//	sprintf(cMinPt,"_minPt%02.0f",minPt);

//	unsigned ih2stubLocalCoordAll=0;
//	TCanvas* c2stubLocalCoordByModule[6];
//	for (unsigned iLay=0; iLay<6; ++iLay) {
//		char cc[100];
//		sprintf(cc,"c2stubLocalCoordByModule_Layer%d",iLay);
//		c2stubLocalCoordByModule[iLay] = new TCanvas(cc,cc,50,0,1700,900);
//		unsigned nx, ny;
//		findFactors(nModules[iLay], nx,ny);
//		cout << iLay << "\t" << nModules[iLay] << "\t" << nx << ":" << ny << endl;
//		c2stubLocalCoordByModule[iLay]->Divide(nx,ny);
//		for (unsigned iMod=0; iMod<nModules[iLay]; ++iMod) {
//			c2stubLocalCoordByModule[iLay]->cd(iMod+1);
//			h2stubLocalCoordAll[ih2stubLocalCoordAll]->Write();
//			h2stubLocalCoordAll[ih2stubLocalCoordAll++]->DrawCopy("colz0");
//		}
//		if (makePlots) {
//			sprintf(cc,"stubLocalCoord_Layer%d",iLay);
//			c2stubLocalCoordByModule[iLay]->SaveAs(outDir+TString(cc)+TString(cMinPt)+TString(".C"));
//			c2stubLocalCoordByModule[iLay]->SaveAs(outDir+TString(cc)+TString(cMinPt)+TString(".pdf"));
//			c2stubLocalCoordByModule[iLay]->SaveAs(outDir+TString(cc)+TString(cMinPt)+TString(".png"));
//		}
//	}

//	unsigned ih2stubLocalCoordDupl=0;
//	TCanvas* c2stubLocalCoordByModuleDupl[6];
//	for (unsigned iLay=0; iLay<6; ++iLay) {
//		char cc[100];
//		sprintf(cc,"c2stubLocalCoordByModuleDupl_Layer%d",iLay);
//		c2stubLocalCoordByModuleDupl[iLay] = new TCanvas(cc,cc,50,0,1700,900);
//		unsigned nx, ny;
//		findFactors(nModules[iLay], nx,ny);
////		cout << iLay << "\t" << nModules[iLay] << "\t" << nx << ":" << ny << endl;
//		c2stubLocalCoordByModuleDupl[iLay]->Divide(nx,ny);
//		for (unsigned iMod=0; iMod<nModules[iLay]; ++iMod) {
//			c2stubLocalCoordByModuleDupl[iLay]->cd(iMod+1);
//			h2stubLocalCoordDupl[ih2stubLocalCoordDupl]->Write();
//			h2stubLocalCoordDupl[ih2stubLocalCoordDupl++]->DrawCopy("colz0");
//		}
//		if (makePlots) {
//			sprintf(cc,"stubOverlapLocalCoords_Layer%d",iLay);
//			c2stubLocalCoordByModuleDupl[iLay]->SaveAs(outDir+TString(cc)+TString(cMinPt)+TString(".C"));
//			c2stubLocalCoordByModuleDupl[iLay]->SaveAs(outDir+TString(cc)+TString(cMinPt)+TString(".pdf"));
//			c2stubLocalCoordByModuleDupl[iLay]->SaveAs(outDir+TString(cc)+TString(cMinPt)+TString(".png"));
//		}
//	}

//	unsigned ih2stubLocalCoordDuplEff=0;
//	TCanvas* c2stubLocalCoordByModuleDuplEff[6];
//	for (unsigned iLay=0; iLay<6; ++iLay) {
//		char cc[100];
//		sprintf(cc,"c2stubLocalCoordByModuleDuplEff_Layer%d",iLay);
//		c2stubLocalCoordByModuleDuplEff[iLay] = new TCanvas(cc,cc,50,0,1700,900);
//		unsigned nx, ny;
//		findFactors(nModules[iLay], nx,ny);
////		cout << iLay << "\t" << nModules[iLay] << "\t" << nx << ":" << ny << endl;
//		c2stubLocalCoordByModuleDuplEff[iLay]->Divide(nx,ny);
//		for (unsigned iMod=0; iMod<nModules[iLay]; ++iMod) {
//			c2stubLocalCoordByModuleDuplEff[iLay]->cd(iMod+1);
//			TString hName(h2stubLocalCoordDupl[ih2stubLocalCoordDuplEff]->GetName());
//			hName.ReplaceAll(TString("Dupl_"),TString("Eff_"));
//			h2stubLocalCoordEff[ih2stubLocalCoordDuplEff] = (TH2F*) h2stubLocalCoordDupl[ih2stubLocalCoordDuplEff]->Clone(hName);
//			h2stubLocalCoordEff[ih2stubLocalCoordDuplEff]->SetTitle(hName);
//			h2stubLocalCoordEff[ih2stubLocalCoordDuplEff]->Divide(h2stubLocalCoordAll[ih2stubLocalCoordDuplEff]);
//			h2stubLocalCoordEff[ih2stubLocalCoordDuplEff]->GetZaxis()->SetRangeUser(0,1);
//			h2stubLocalCoordEff[ih2stubLocalCoordDuplEff]->Write();
//			h2stubLocalCoordEff[ih2stubLocalCoordDuplEff++]->DrawCopy("colz0");
//		}
//		if (makePlots) {
//			sprintf(cc,"stubOverlapLocalCoordsEff_Layer%d",iLay);
//			c2stubLocalCoordByModuleDuplEff[iLay]->SaveAs(outDir+TString(cc)+TString(cMinPt)+TString(".root"));
//			c2stubLocalCoordByModuleDuplEff[iLay]->SaveAs(outDir+TString(cc)+TString(cMinPt)+TString(".C"));
//			c2stubLocalCoordByModuleDuplEff[iLay]->SaveAs(outDir+TString(cc)+TString(cMinPt)+TString(".pdf"));
//			c2stubLocalCoordByModuleDuplEff[iLay]->SaveAs(outDir+TString(cc)+TString(cMinPt)+TString(".png"));
//		}
//	}

	float phi_magic = 0.18;
//	float phi_magic = 0.;
	double const R = 100*minPt/(0.3*3.8);

	TCanvas* cRX       [11][2];
	TCanvas* cRX_EtaPhi[6]   ;
	if (makeRX) {
		for (unsigned iLay=0; iLay<6; ++iLay) {
			if (!vStubEta[iLay].size()) {
				std::cout << "WARNING. No stubs in barrel layer " << iLay << std::endl;
			}
			else {
				gStubEtaPhi               [iLay] = new TGraph(vStubEta              [iLay].size(),&(vStubEta              [iLay][0]),&(vStubPhi              [iLay][0]));
				char cc[90];
				sprintf(cc,"Stubs #phi VS #eta - Layer %d - TT%d",iLay,iTT);
				gStubEtaPhi               [iLay]-> SetTitle(cc);
				sprintf(cc,"cRX_Layer_%d_TT%d",iLay,iTT);
				cRX[iLay][0] = new TCanvas(cc,cc,0,0,1000,1000);
				gStubEtaPhi               [iLay]->GetXaxis()->SetTitle("Stub #eta");
				gStubEtaPhi               [iLay]->GetYaxis()->SetTitle("Stub #phi");
				gStubEtaPhi               [iLay]-> Draw("AP");
				TLine l;
				l.SetLineColor(3);
				l.SetLineWidth(2);
				float phiTrack = -TMath::ACos(vBarrelRadii.at(iLay)/(2*R))+(TMath::Pi()-phi_magic);
				std::cout << iLay << "\t" << R << "\t" << vBarrelRadii.at(iLay) << "\t" << TMath::ACos(vBarrelRadii.at(iLay)/(2*R)) << "\t" << phiTrack  << "\t";
				l.DrawLine(-1,phiTrack,3,phiTrack);
				phiTrack = TMath::ACos(vBarrelRadii.at(iLay)/(2*R))+(-TMath::Pi()/4+phi_magic);
				std::cout << phiTrack << std::endl;
				l.DrawLine(-1,phiTrack,3,phiTrack);
				l.SetLineColor(5);
				l.SetLineStyle(2);
				phiTrack = TMath::ACos(vBarrelRadii.at(iLay)/(2*R))+(phi_magic);
				l.DrawLine(-1,phiTrack,3,phiTrack);
				phiTrack = -TMath::ACos(vBarrelRadii.at(iLay)/(2*R))+(3*TMath::Pi()/4-phi_magic);
				l.DrawLine(-1,phiTrack,3,phiTrack);

//				l.DrawLine(gStubEtaPhi[iLay]->GetXaxis()->GetXmin(),0.0         ,gStubEtaPhi[iLay]->GetXaxis()->GetXmax(),0.0         );
//				l.DrawLine(gStubEtaPhi[iLay]->GetXaxis()->GetXmin(),1.5707963268,gStubEtaPhi[iLay]->GetXaxis()->GetXmax(),1.5707963268);
				if (makePlots) {
					cRX[iLay][0]-> SaveAs(outDir+TString(cc)+sProc+TString(".pdf"));
					cRX[iLay][0]-> SaveAs(outDir+TString(cc)+sProc+TString(".png"));
				}

//				if (iTT==27) {
//				gStubEtaPhi_OverlapRemoved[iLay] = new TGraph(vStubEtaOverlapRemoved[iLay].size(),&(vStubEtaOverlapRemoved[iLay][0]),&(vStubPhiOverlapRemoved[iLay][0]));
//				sprintf(cc,"Stubs #phi VS #eta - Layer %d - TT%d - Overlap removed",iLay,iTT);
//				gStubEtaPhi_OverlapRemoved[iLay]-> SetTitle(cc);
//				sprintf(cc,"cRX_Layer_%d_TT%d_OverlapRemoved",iLay,iTT);
//				cRX[iLay][1] = new TCanvas(cc,cc,0,0,1000,1000);
//				gStubEtaPhi_OverlapRemoved[iLay]->GetXaxis()->SetTitle("Stub #eta");
//				gStubEtaPhi_OverlapRemoved[iLay]->GetYaxis()->SetTitle("Stub #phi");
//				gStubEtaPhi_OverlapRemoved[iLay]-> Draw("AP");
//				if (makePlots) {
//					cRX[iLay][1]-> SaveAs(outDir+TString(cc)+sProc+TString(".pdf"));
//					cRX[iLay][1]-> SaveAs(outDir+TString(cc)+sProc+TString(".png"));
//				}
//				}
			}
		}

		TLegend* tlPS;
		TLegend* tl2S;
		TCanvas* cOccupancyDiskPS   = new TCanvas("cOccupancyDiskPS"  ,"cOccupancyDiskPS"  ,0,0,1000,1000); cOccupancyDiskPS->Divide(3,2);
		TCanvas* cOccupancyDisk2S   = new TCanvas("cOccupancyDisk2S"  ,"cOccupancyDisk2S"  ,0,0,1000,1000); cOccupancyDisk2S->Divide(3,2);
		TCanvas* cOccupancyDiskPS2D = new TCanvas("cOccupancyDiskPS2D","cOccupancyDiskPS2D",0,0,1000,1000); cOccupancyDiskPS2D->Divide(3,2);
		TCanvas* cOccupancyDisk2S2D = new TCanvas("cOccupancyDisk2S2D","cOccupancyDisk2S2D",0,0,1000,1000); cOccupancyDisk2S2D->Divide(3,2);
		vector <float> vDiskZ = {128.494003,153.294998,182.654007,217.408005,258.549988};
		vector <float> vTowerEta = {0,2.2/3,4.4/3,2.2};
		for (unsigned iLay=0; iLay<5; ++iLay) { //disks

			char cc[100];
			tlPS = new TLegend(0.25,0.82,0.95,0.95);
			cOccupancyDiskPS->cd(iLay+1);
			h1OccupancyVsR_PS_Disk[iLay]->SetXTitle("R [cm]");
			h1OccupancyVsR_PS_Disk[iLay]->DrawCopy();
			sprintf(cc,"Tot occup. PS L%d = %5.0lf",iLay+11,h1OccupancyVsR_PS_Disk[iLay]->Integral());
			tlPS->AddEntry(h1OccupancyVsR_PS_Disk[iLay],cc,"L");
			tlPS->Draw();
			cOccupancyDisk2S->cd(iLay+1);
			h1OccupancyVsR_2S_Disk[iLay]->SetXTitle("R [cm]");
			h1OccupancyVsR_2S_Disk[iLay]->DrawCopy();
			tl2S = new TLegend(0.25,0.82,0.95,0.95);
			sprintf(cc,"Tot occup. 2S L%d = %5.0lf",iLay+11,h1OccupancyVsR_2S_Disk[iLay]->Integral());
			tl2S->AddEntry(h1OccupancyVsR_2S_Disk[iLay],cc,"L");
			tl2S->Draw();

			cOccupancyDiskPS2D->cd(iLay+1);
			h2Occupancy_PS_Disk[iLay]->SetXTitle("R [cm]");
			h2Occupancy_PS_Disk[iLay]->SetYTitle("#phi [rad]");
			h2Occupancy_PS_Disk[iLay]->Draw("colz");
			cOccupancyDisk2S2D->cd(iLay+1);
			h2Occupancy_2S_Disk[iLay]->SetXTitle("R [cm]");
			h2Occupancy_2S_Disk[iLay]->SetYTitle("#phi [rad]");
			h2Occupancy_2S_Disk[iLay]->Draw("colz");

			if (!vStubXDisk[iLay].size()) {
				std::cout << "WARNING. No stubs in disk " << iLay+11 << std::endl;
			}
			else {
				gStubXY[iLay] = new TGraph(vStubXDisk[iLay].size(),&(vStubXDisk[iLay][0]),&(vStubYDisk[iLay][0]));
				char cc[90];
				sprintf(cc,"Stubs Y vs X - Layer %d - TT %d",iLay+11,iTT);
				gStubXY[iLay]-> SetTitle(cc);
				sprintf(cc,"cRX_Layer_%d_TT%d",iLay+11,iTT);
				cRX[iLay+6][0] = new TCanvas(cc,cc,0,0,1000,1000);
				gStubXY[iLay]->GetXaxis()->SetTitle("Stub X");
				gStubXY[iLay]->GetYaxis()->SetTitle("Stub Y");
				gStubXY[iLay]-> Draw("AP");
				float xmin = gStubXY[iLay]->GetXaxis()->GetXmin();
				float xmax = gStubXY[iLay]->GetXaxis()->GetXmax();
				float ymin = gStubXY[iLay]->GetYaxis()->GetXmin();
				float ymax = gStubXY[iLay]->GetYaxis()->GetXmax();
				if (xmax-xmin > ymax-ymin) ymin = ymax-(xmax-xmin);
				else                       xmax = xmin+ymax-ymin;
				cout << iLay << "\t" << xmin << "\t" << xmax << "\t" << ymin << "\t" << ymax << std::endl;
				gStubXY[iLay]->GetXaxis()->SetLimits(xmin,xmax);
				gStubXY[iLay]->SetMinimum(ymin);
				gStubXY[iLay]->SetMaximum(ymax);
				gStubXY[iLay]-> Draw("AP");
				cRX[iLay+6][0]->Update();
				TLine tlTower;
				tlTower.SetLineColor(1);
				tlTower.SetLineStyle(2);
				tlTower.SetLineWidth(4);
				tlTower.DrawLine(0,0,0,150);
				tlTower.DrawLine(0,0,150,150);
				TArc genPart;
				genPart.SetFillStyle(0);
				genPart.SetLineColor(3);
				genPart.SetLineWidth(2);
				genPart.DrawArc(-R*TMath::Cos(phi_magic),R*TMath::Sin(phi_magic),R,-phi_magic*2*90/TMath::Pi(),90,"only");
				genPart.DrawArc(R*TMath::Cos(-TMath::Pi()/4+phi_magic),R*TMath::Sin(-TMath::Pi()/4+phi_magic),R,90,(3*TMath::Pi()/4+phi_magic)*180/TMath::Pi(),"only");

				genPart.SetFillStyle(0);
				genPart.SetLineColor(5);
				genPart.SetLineStyle(2);
				genPart.SetLineWidth(2);
//				genPart.DrawArc(R*TMath::Cos(-phi_magic),R*TMath::Sin(phi_magic),R,-phi_magic*2*90/TMath::Pi(),90,"only");
//				genPart.DrawArc(R*TMath::Cos(-TMath::Pi()/4+phi_magic),R*TMath::Sin(-TMath::Pi()/4+phi_magic),R,90,(3*TMath::Pi()/4+phi_magic)*180/TMath::Pi(),"only");
				genPart.DrawArc(R*TMath::Cos(phi_magic),R*TMath::Sin(phi_magic),R,90,(TMath::Pi()+phi_magic)*180/TMath::Pi(),"only");
				genPart.DrawArc(R*TMath::Cos(3*TMath::Pi()/4-phi_magic),R*TMath::Sin(3*TMath::Pi()/4-phi_magic),R,(7*TMath::Pi()/4-phi_magic)*180/TMath::Pi(),360,"only");
				int iEta = 0;
				if (iTT == 35) iEta = 1;
				if (iTT == 43) iEta = 2;
				double Rm = (vDiskZ.at(iLay)-5.0)/TMath::SinH(vTowerEta.at(iEta+1)); //minR @ layer iLay. Z Reduced by 5cm
				genPart.SetLineColor(3);
				genPart.DrawArc(0,0,Rm,20,120,"only");
				Rm = (vDiskZ.at(iLay)+5.0)/TMath::SinH(vTowerEta.at(iEta));  //maxR @ layer iLay. Z Increased by 5cm
				genPart.DrawArc(0,0,Rm,0,140,"only");

				TLine l;
				l.SetLineColor(2);
				TLine l2S;
				l2S.SetLineColor(4);
				float rPSmin = 24;
				float rPSmax = 60;
				float r2Smax = 110;
				float phimin = -TMath::Pi()/(nPhi*8);
				float deltaphi = 2*TMath::Pi()/(nPhi*8);

				for (int iphi=0; iphi<nPhi*8; ++iphi) {
					float phi = phimin+iphi*deltaphi;
					float c = TMath::Cos(phi);
					float s = TMath::Sin(phi);
					l  .DrawLine(rPSmin*c,rPSmin*s,rPSmax*c,rPSmax*s);
					l2S.DrawLine(rPSmax*c,rPSmax*s,r2Smax*c,r2Smax*s);
				}

				TArc circle;
				circle.SetLineColor(2);
				circle.SetFillStyle(0);
				TArc circle2S;
				circle2S.SetLineColor(4);
				circle2S.SetFillStyle(0);
				for (unsigned iR=0; iR<vrPS.size(); ++iR) {
					circle.DrawArc(0,0,vrPS.at(iR)/10,0,360);
				}
				for (unsigned iR=0; iR<vr2S.size(); ++iR) {
					circle2S.DrawArc(0,0,vr2S.at(iR)/10,0,360);
				}

				if (makePlots) {
					cRX[iLay+6][0]->SaveAs(outDir+TString(cc)+sProc+TString(".pdf"));
					cRX[iLay+6][0]->SaveAs(outDir+TString(cc)+sProc+TString(".png"));
				}
			}
		} //end disks

		TCanvas* cXY_OccupanciesBarrel = new TCanvas("cXY_OccupanciesBarrel","cXY_OccupanciesBarrel",0,0,1000,1000);
		gStubXYBarrel  = new TGraph(vStubXBarrel.size(),&(vStubXBarrel[0]),&(vStubYBarrel[0]));
		char cc[100];
		sprintf(cc,"Stubs Y vs X");
		gStubXYBarrel->SetTitle(cc);
		gStubXYBarrel->GetXaxis()->SetTitle("Stub X");
		gStubXYBarrel->GetYaxis()->SetTitle("Stub Y");
		gStubXYBarrel->Draw("AP");
		gStubXYBarrel->GetXaxis()->SetLimits(-25,95);
		gStubXYBarrel->SetMinimum(0);
		gStubXYBarrel->SetMaximum(120);
		gStubXYBarrel->Draw("AP");
		cXY_OccupanciesBarrel->Update();
		TArc genPart;
		genPart.SetFillStyle(0);
		genPart.SetLineColor(3);
		genPart.SetLineWidth(2);
		genPart.DrawArc(-R*TMath::Cos(phi_magic),R*TMath::Sin(phi_magic),R,-phi_magic*2*90/TMath::Pi(),90,"only");
		genPart.DrawArc(R*TMath::Cos(-TMath::Pi()/4+phi_magic),R*TMath::Sin(-TMath::Pi()/4+phi_magic),R,90,(3*TMath::Pi()/4+phi_magic)*180/TMath::Pi(),"only");

		genPart.SetFillStyle(0);
		genPart.SetLineColor(5);
		genPart.SetLineStyle(2);
		genPart.SetLineWidth(2);
		genPart.DrawArc(R*TMath::Cos(phi_magic),R*TMath::Sin(phi_magic),R,90,(TMath::Pi()+phi_magic)*180/TMath::Pi(),"only");
		genPart.DrawArc(R*TMath::Cos(3*TMath::Pi()/4-phi_magic),R*TMath::Sin(3*TMath::Pi()/4-phi_magic),R,(7*TMath::Pi()/4-phi_magic)*180/TMath::Pi(),360,"only");

		TLine tl;
		tl.SetLineColor(1);
		tl.SetLineStyle(2);
		tl.SetLineWidth(4);
		tl.DrawLine(0,0,150,150);
		tl.DrawLine(0,0,  0,150);

		TCanvas* cRZ_Occupancies = new TCanvas("cRZ_Occupancy","cRZ_Occupancy",0,0,1000,1000);
		gStubRZ        = new TGraph(vStubZ.size(),&(vStubZ[0]),&(vStubR[0]));
//		char cc[100];
		sprintf(cc,"Stubs R vs Z");
		gStubRZ-> SetTitle(cc);
		gStubRZ->GetXaxis()->SetTitle("Stub Z");
		gStubRZ->GetYaxis()->SetTitle("Stub R");
		gStubRZ        ->Draw("AP");
		gStubRZ->GetXaxis()->SetLimits(-20,280);
		gStubRZ        ->Draw("AP");
		cRZ_Occupancies->Update();
//		TLine tl;
//		tl.SetLineColor(1);
//		tl.SetLineWidth(4);
		tl.SetLineColor(3);
		tl.SetLineStyle(1);
		tl.SetLineWidth(2);
		float max_z = 5;
		float max_rho = 110;
		if (iTT==27) {
			tl.DrawLine(-max_z,0,max_rho*TMath::SinH(vTowerEta.at(0)),max_rho);
			tl.DrawLine( max_z,0,max_rho*TMath::SinH(vTowerEta.at(1)),max_rho  );
			tl.SetLineColor(5);
			tl.SetLineStyle(2);
			tl.DrawLine(-max_z,0,max_rho*TMath::SinH(vTowerEta.at(1)),max_rho);
		}
		if (iTT==35) {
			tl.DrawLine(-max_z,0,max_rho*TMath::SinH(vTowerEta.at(1)),max_rho);
			tl.DrawLine( max_z,0,max_rho*TMath::SinH(vTowerEta.at(2)),max_rho  );
			tl.SetLineColor(5);
			tl.SetLineStyle(2);
			tl.DrawLine( max_z,0,max_rho*TMath::SinH(vTowerEta.at(1)),max_rho  );
			tl.DrawLine(-max_z,0,max_rho*TMath::SinH(vTowerEta.at(2)),max_rho);
		}
		if (iTT==43) {
			tl.DrawLine(-max_z,0,max_rho*TMath::SinH(vTowerEta.at(2)),max_rho);
			tl.DrawLine( max_z,0,max_rho*TMath::SinH(vTowerEta.at(3)),max_rho  );
			tl.SetLineColor(5);
			tl.SetLineStyle(2);
			tl.DrawLine( max_z,0,max_rho*TMath::SinH(vTowerEta.at(2)),max_rho  );
		}

//		tl.DrawLine( 5,0,280+5,280/TMath::SinH(2.2)  );
//		tl.DrawLine(-5,0,(130-5)*TMath::SinH(4.4/3),130);
//		tl.DrawLine( 5,0,(130+5)*TMath::SinH(4.4/3),130);
//		tl.DrawLine(-5,0,(130-5)*TMath::SinH(2.2/3),130);

		if (makePlots) {
			char cc[100];
			sprintf(cc,"cOccupancyDiskPS_TT%d",iTT);
			cOccupancyDiskPS->SaveAs(outDir+TString(cc)+sProc+TString(".png"));
			sprintf(cc,"cOccupancyDisk2S_TT%d",iTT);
			cOccupancyDisk2S->SaveAs(outDir+TString(cc)+sProc+TString(".png"));
			sprintf(cc,"cOccupancyDiskPS2D_TT%d",iTT);
			cOccupancyDiskPS2D->SaveAs(outDir+TString(cc)+sProc+TString(".png"));
			sprintf(cc,"cOccupancyDisk2S2D_TT%d",iTT);
			cOccupancyDisk2S2D->SaveAs(outDir+TString(cc)+sProc+TString(".png"));
			sprintf(cc,"cXY_OccupanciesBarrel");
			cXY_OccupanciesBarrel->SaveAs(outDir+TString(cc)+sProc+TString(".png"));
			sprintf(cc,"cRZ_Occupancy_TT%d",iTT);
			cRZ_Occupancies->SaveAs(outDir+TString(cc)+sProc+TString(".png"));
		}

//		for (unsigned iLay=0; iLay<6; ++iLay) {
//			char cc[90];
//			char cc2[90];
//			sprintf(cc,"cRX_EtaPhi_Layer_%d_TT%d",iLay,iTT);
//			sprintf(cc2,"Stubs #eta and #phi - Layer_%d - TT %d",iLay,iTT);
//			cRX_EtaPhi[iLay] = new TCanvas(cc,cc2,0,0,1000,500);
//			cRX_EtaPhi[iLay]->Divide(2,1);
//			cRX_EtaPhi[iLay]->cd(1);
//			hStubEta              [iLay]->GetXaxis()->SetTitle("Stub #eta");
//			hStubEta              [iLay]->Draw();
//			hStubEtaOverlapRemoved[iLay]->SetLineColor(2);
//			hStubEtaOverlapRemoved[iLay]->Draw("same");
//			cRX_EtaPhi[iLay]->cd(2);
//			hStubPhi              [iLay]->GetXaxis()->SetTitle("Stub #phi");
//			hStubPhi              [iLay]->Draw();
//			hStubPhiOverlapRemoved[iLay]->SetLineColor(2);
//			hStubPhiOverlapRemoved[iLay]->Draw("same");
//			if (makePlots) {
//				cRX_EtaPhi[iLay]->SaveAs(outDir+TString(cc)+sProc+TString(".pdf"));
//				cRX_EtaPhi[iLay]->SaveAs(outDir+TString(cc)+sProc+TString(".png"));
//			}
//		}
	}

//	fOutHistos->Close();

	//*********************************************

//	bool makeHistoRadii = false;
//	if (makeHistoRadii) {
//	vector <double> vRadii;
//	vRadii.push_back(0);
//	bool hasEntries = false;
//	for (int iBin=1; iBin<h1StubRadius->GetXaxis()->GetNbins()+1; ++iBin) {
//		if ((hasEntries && !h1StubRadius->GetBinContent(iBin)) || (!hasEntries && h1StubRadius->GetBinContent(iBin))) {
//			std::cout << h1StubRadius->GetBinLowEdge(iBin) << ",";
//			vRadii.push_back(h1StubRadius->GetBinLowEdge(iBin));
//			hasEntries = not(hasEntries);
//		}
//	}
//	vRadii.push_back(120);
//	TFile* fRadiiw = 0; isfOpen = false;
//	fRadiiw = new TFile("barrelRadii.root","RECREATE");
//	isfOpen = fRadiiw->IsOpen();
//	if (!isfOpen) {
//		cout << "ERROR. Not able to load the radii file. Exiting..." << endl;
//		return;
//	}
//
//	TH1* hRadiiw = new TH1S("hRadii","hRadii",vRadii.size()-1,&vRadii[0]);
//	unsigned iCount = 0;
//	for (unsigned iBin=0; iBin<vRadii.size()-1; ++iBin) {
//		if (iBin%2==0) {
//			hRadiiw->SetBinContent(iBin+1,0);
//		}
//		else {
//			unsigned iSubLayer=1+iCount%4;
//			unsigned iLayer=iCount/4;
//			hRadiiw->SetBinContent(iBin+1,iLayer*10+iSubLayer);
//			++iCount;
//		}
//	}
//	hRadiiw->Write();
//	fRadiiw->Close();
//	}
	return;
}

void overlapRemovalRun_2(Long64_t nMax=1000000,bool makePlots=false, bool makeRX=false, int TT=27, float minPt=2) {
	gStyle->SetOptStat(0);
	TString sProc("_SingleMuon");
	TString fName;
		if (TT==27) {
			fName="/data/rossin/EOS/singleMuNoTest2_2000/SingleMuonFlatOneOverPt0p0005To0p5_tt27NoTest_cfi_py_GEN_SIM_DIGI_L1TrackTrigger_2Msum.root";
			fName="/data/rossin/EOS/SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3/SingleMuonTest_tt27_PU0_20150408/SingleMuonTest_tt27_PU0_ntuple_Pt0p2_6p5M.root";
			fName="/data/rossin/EOS/SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3/SingleMuonTest_tt27_PU0_20150408/SingleMuonTest_tt27_PU0_ntuple_Pt3_6M.root";
//			fName="/data/rossin/EOS/Neutrino_PU140_20150408ext2/Neutrino_PU140_ntuple_1k.root"; 	sProc="_PU140";
		}
		if (TT==35) {
			fName="/data/rossin/EOS/SingleMuon_tt35_20150918/SingleMuon_tt35_ntuple_2M.root";
			fName="/data/rossin/EOS/Neutrino_PU140_20150408ext2/Neutrino_PU140_ntuple_1k.root"; 	sProc="_PU140";
		}
		if (TT==43) {
			fName="/data/rossin/EOS/SingleMuon_tt43_20150918/SingleMuon_tt43_ntuple_2M.root";
			fName="/data/rossin/EOS/Neutrino_PU140_20150408ext2/Neutrino_PU140_ntuple_1k.root"; 	sProc="_PU140";
		}

	overlapRemoval s(fName);
	s.Loop(nMax,makePlots,minPt,sProc,makeRX,TT);
	return;

}
void genTrackPropagate(double *genTrackPt_Phi0, double r, double *genTrackXY) {
	double Pt       = genTrackPt_Phi0[0];
	double Phi0     = genTrackPt_Phi0[1];
	double rho = Pt/(0.3*3.8);
	double phi = TMath::ASin(r/(2*rho))+Phi0;

	genTrackXY[0] = r*TMath::Cos(phi);
	genTrackXY[1] = r*TMath::Sin(phi);
}

void loadTT27ModuleList(vector <unsigned> &v) { // this is sector 4,4 in mersi's DB
//	double moduleL[] = {50230,50231,50232,50233,50234,50235,50236,50330,50331,50332,50333,50334,50335,50336,50337,50430,50431,50432,50433,50434,50435,50436,60326,60327,60328,60329,60330,60331,60332,60333,60334,60335,60426,60427,60428,60429,60430,60431,60432,60433,60434,60526,60527,60528,60529,60530,60531,60532,60533,60534,60535,60626,60627,60628,60629,60630,60631,60632,60633,60634,70426,70427,70428,70429,70430,70431,70432,70433,70434,70435,70436,70526,70527,70528,70529,70530,70531,70532,70533,70534,70535,70536,70537,70626,70627,70628,70629,70630,70631,70632,70633,70634,70635,70636,70726,70727,70728,70729,70730,70731,70732,70733,70734,70735,70736,70737,70826,70827,70828,70829,70830,70831,70832,70833,70834,70835,70836,80611,80612,80613,80614,80615,80616,80617,80711,80712,80713,80714,80715,80716,80717,80811,80812,80813,80814,80815,80816,80817,80911,80912,80913,80914,80915,80916,80917,81011,81012,81013,81014,81015,81016,81017,81111,81112,81113,81114,81115,81116,81117,81211,81212,81213,81214,81215,81216,81217,90711,90712,90713,90714,90715,90716,90717,90718,90719,90811,90812,90813,90814,90815,90816,90817,90818,90819,90911,90912,90913,90914,90915,90916,90917,90918,90919,91011,91012,91013,91014,91015,91016,91017,91018,91019,91111,91112,91113,91114,91115,91116,91117,91118,91119,91211,91212,91213,91214,91215,91216,91217,91218,91219,91311,91312,91313,91314,91315,91316,91317,91318,91319,91411,91412,91413,91414,91415,91416,91417,91418,91419,91511,91512,91513,91514,91515,91516,91517,91518,91519,91611,91612,91613,91614,91615,91616,91617,91618,91619,100811,100812,100813,100814,100815,100816,100817,100818,100819,100820,100911,100912,100913,100914,100915,100916,100917,100918,100919,100920,101011,101012,101013,101014,101015,101016,101017,101018,101019,101020,101111,101112,101113,101114,101115,101116,101117,101118,101119,101120,101211,101212,101213,101214,101215,101216,101217,101218,101219,101220,101311,101312,101313,101314,101315,101316,101317,101318,101319,101320,101411,101412,101413,101414,101415,101416,101417,101418,101419,101420,101511,101512,101513,101514,101515,101516,101517,101518,101519,101520,101611,101612,101613,101614,101615,101616,101617,101618,101619,101620,101711,101712,101713,101714,101715,101716,101717,101718,101719,101720,101811,101812,101813,101814,101815,101816,101817,101818,101819,101820,101911,101912,101913,101914,101915,101916,101917,101918,101919,101920,102011,102012,102013,102014,102015,102016,102017,102018,102019,102020,102111,102112,102113,102114,102115,102116,102117,102118,102119,102120};
	double moduleL[] = {
			50230,50231,50232,50233,50234,50235,50236,50237,50330,50331,50332,50333,50334,50335,50336,50337,50430,50431,50432,50433,50434,50435,50436,50437,
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

void loadTT35ModuleList(vector <unsigned> &v) { // this is sector 5,4 and 5,2 in mersi's DB
	double moduleL[] = {50234,50235,50236,50237,50238,50239,50240,50241,50242,50243,50335,50336,50337,50338,50339,50340,50341,50342,50343,50344,50345,50434,50435,50436,50437,50438,50439,50440,50441,50442,50443,60333,60334,60335,60336,60337,60338,60339,60340,60341,60342,60343,60344,60345,60346,60433,60434,60435,60436,60437,60438,60439,60440,60441,60442,60443,60444,60445,60533,60534,60535,60536,60537,60538,60539,60540,60541,60542,60543,60544,60545,60546,60633,60634,60635,60636,60637,60638,60639,60640,60641,60642,60643,60644,60645,70435,70436,70437,70438,70439,70440,70441,70442,70443,70444,70445,70446,70447,70448,70449,70450,70451,70535,70536,70537,70538,70539,70540,70541,70542,70543,70544,70545,70546,70547,70548,70549,70550,70551,70552,70635,70636,70637,70638,70639,70640,70641,70642,70643,70644,70645,70646,70647,70648,70649,70650,70651,70735,70736,70737,70738,70739,70740,70741,70742,70743,70744,70745,70746,70747,70748,70749,70750,70751,70752,70835,70836,70837,70838,70839,70840,70841,70842,70843,70844,70845,70846,70847,70848,70849,70850,70851,80617,80618,80619,80620,80621,80622,80623,80717,80718,80719,80720,80721,80722,80723,80817,80818,80819,80820,80821,80822,80823,80917,80918,80919,80920,80921,80922,80923,81017,81018,81019,81020,81021,81022,81023,81117,81118,81119,81120,81121,81122,81123,81217,81218,81219,81220,81221,81222,81223,90719,90720,90721,90722,90723,90818,90819,90820,90821,90822,90823,90919,90920,90921,90922,90923,91018,91019,91020,91021,91022,91023,91119,91120,91121,91122,91123,91218,91219,91220,91221,91222,91223,91319,91320,91321,91322,91323,91418,91419,91420,91421,91422,91423,91519,91520,91521,91522,91523,91618,91619,91620,91621,91622,91623,100820,100821,100822,100823,100920,100921,100922,100923,101020,101021,101022,101023,101120,101121,101122,101123,101220,101221,101222,101223,101320,101321,101322,101323,101420,101421,101422,101423,101520,101521,101522,101523,101620,101621,101622,101623,101720,101721,101722,101723,101820,101821,101822,101823,101920,101921,101922,101923,102020,102021,102022,102023,102120,102121,102122,102123,110905,110906,110907,110908,110909,110910,110911,110912,111006,111007,111008,111009,111010,111011,111012,111013,111106,111107,111108,111109,111110,111111,111112,111113,111114,111115,111116,111207,111208,111209,111210,111211,111212,111213,111214,111215,111216,111217,111307,111308,111309,111310,111311,111312,111313,111314,111315,111316,111317,111318,111319,111407,111408,111409,111410,111411,111412,111413,111414,111415,111416,111417,111418,111419,111420,111421,121006,121007,121008,121009,121010,121011,121012,121013,121106,121107,121108,121109,121110,121111,121112,121113,121114,121115,121116,121207,121208,121209,121210,121211,121212,121213,121214,121215,121216,121217,121307,121308,121309,121310,121311,121312,121313,121314,121315,121316,121317,121318,121319,121407,121408,121409,121410,121411,121412,121413,121414,121415,121416,121417,121418,121419,121420,121421,131207,131208,131209,131210,131211,131212,131213,131214,131215,131216,131217,131307,131308,131309,131310,131311,131312,131313,131314,131315,131316,131317,131318,131319,131407,131408,131409,131410,131411,131412,131413,131414,131415,131416,131417,131418,131419,131420,131421,141407,141408,141409,141410,141411,141412,141413,141414,141415,141416,141417,141418,141419,141420,141421};
//              			50234,50235,50236,50237,50238,50239,50240,50241,50242,50243,50335,50336,50337,50338,50339,50340,50341,50342,50343,50344,50345,50434,50435,50436,50437,50438,50439,50440,50441,50442,50443,60333,60334,60335,60336,60337,60338,60339,60340,60341,60342,60343,60344,60345,60346,60433,60434,60435,60436,60437,60438,60439,60440,60441,60442,60443,60444,60445,60533,60534,60535,60536,60537,60538,60539,60540,60541,60542,60543,60544,60545,60546,60633,60634,60635,60636,60637,60638,60639,60640,60641,60642,60643,60644,60645,70435,70436,70437,70438,70439,70440,70441,70442,70443,70444,70445,70446,70447,70448,70449,70450,70451,70535,70536,70537,70538,70539,70540,70541,70542,70543,70544,70545,70546,70547,70548,70549,70550,70551,70552,70635,70636,70637,70638,70639,70640,70641,70642,70643,70644,70645,70646,70647,70648,70649,70650,70651,70735,70736,70737,70738,70739,70740,70741,70742,70743,70744,70745,70746,70747,70748,70749,70750,70751,70752,70835,70836,70837,70838,70839,70840,70841,70842,70843,70844,70845,70846,70847,70848,70849,70850,70851,80617,80618,80619,80620,80621,80622,80623,80717,80718,80719,80720,80721,80722,80723,80817,80818,80819,80820,80821,80822,80823,80917,80918,80919,80920,80921,80922,80923,81017,81018,81019,81020,81021,81022,81023,81117,81118,81119,81120,81121,81122,81123,81217,81218,81219,81220,81221,81222,81223,90719,90720,90721,90722,90723,90818,90819,90820,90821,90822,90823,90919,90920,90921,90922,90923,91018,91019,91020,91021,91022,91023,91119,91120,91121,91122,91123,91218,91219,91220,91221,91222,91223,91319,91320,91321,91322,91323,91418,91419,91420,91421,91422,91423,91519,91520,91521,91522,91523,91618,91619,91620,91621,91622,91623,100820,100821,100822,100823,100920,100921,100922,100923,101020,101021,101022,101023,101120,101121,101122,101123,101220,101221,101222,101223,101320,101321,101322,101323,101420,101421,101422,101423,101520,101521,101522,101523,101620,101621,101622,101623,101720,101721,101722,101723,101820,101821,101822,101823,101920,101921,101922,101923,102020,102021,102022,102023,102120,102121,102122,102123,110905,110906,110907,110908,110909,110910,110911,110912,111006,111007,111008,111009,111010,111011,111012,111013,111106,111107,111108,111109,111110,111111,111112,111113,111114,111115,111116,111207,111208,111209,111210,111211,111212,111213,111214,111215,111216,111217,111307,111308,111309,111310,111311,111312,111313,111314,111315,111316,111317,111318,111319,111407,111408,111409,111410,111411,111412,111413,111414,111415,111416,111417,111418,111419,111420,111421,121006,121007,121008,121009,121010,121011,121012,121013,121106,121107,121108,121109,121110,121111,121112,121113,121114,121115,121116,121207,121208,121209,121210,121211,121212,121213,121214,121215,121216,121217,121307,121308,121309,121310,121311,121312,121313,121314,121315,121316,121317,121318,121319,121407,121408,121409,121410,121411,121412,121413,121414,121415,121416,121417,121418,121419,121420,121421,131207,131208,131209,131210,131211,131212,131213,131214,131215,131216,131217,131307,131308,131309,131310,131311,131312,131313,131314,131315,131316,131317,131318,131319,131407,131408,131409,131410,131411,131412,131413,131414,131415,131416,131417,131418,131419,131420,131421,141407,141408,141409,141410,141411,141412,141413,141414,141415,141416,141417,141418,141419,141420,141421};
//			50234,50235,50236,50237,50238,50239,50240,50241,50242,50243,50244,50334,50335,50336,50337,50338,
//			50339,50340,50341,50342,50343,50344,50345,50434,50435,50436,50437,50438,50439,50440,50441,50442,50443,50444,
//			60333,60334,60335,60336,60337,60338,60339,60340,60341,60342,60343,60344,60345,60346,60432,60433,60434,60435,60436,
//			60437,60438,60439,60440,60441,60442,60443,60444,60445,60533,60534,60535,60536,60537,60538,60539,60540,60541,
//			60542,60543,60544,60545,60546,60632,60633,60634,60635,60636,60637,60638,60639,60640,60641,60642,60643,60644,60645,
//			70435,70436,70437,70438,70439,70440,70441,70442,70443,70444,70445,70446,70447,70448,70449,70450,70451,70452,70535,
//			70536,70537,70538,70539,70540,70541,70542,70543,70544,70545,70546,70547,70548,70549,70550,70551,70552,70553,70635,
//			70636,70637,70638,70639,70640,70641,70642,70643,70644,70645,70646,70647,70648,70649,70650,70651,70652,70735,70736,
//			70737,70738,70739,70740,70741,70742,70743,70744,70745,70746,70747,70748,70749,70750,70751,70752,70753,70835,70836,
//			70837,70838,70839,70840,70841,70842,70843,70844,70845,70846,70847,70848,70849,70850,70851,70852,70935,70936,70937,
//			70938,70939,70940,70941,70942,70943,70944,70945,70946,70947,70948,70949,70950,70951,70952,70953,
//			80617,80618,80619,80620,80621,80622,80623,80717,80718,80719,80720,80721,80722,80723,80817,80818,80819,80820,80821,
//			80822,80823,80917,80918,80919,80920,80921,80922,80923,81017,81018,81019,81020,81021,81022,81023,81117,81118,81119,
//			81120,81121,81122,81123,81217,81218,81219,81220,81221,81222,81223,
//			90719,90720,90721,90722,90723,90818,90819,90820,90821,90822,90823,90919,90920,90921,90922,90923,91018,91019,91020,
//			91021,91022,91023,91119,91120,91121,91122,91123,91218,91219,91220,91221,91222,91223,91319,91320,91321,91322,91323,
//			91418,91419,91420,91421,91422,91423,91519,91520,91521,91522,91523,91618,91619,91620,91621,91622,91623,
//			100020,100021,100022,100023,100120,100121,100122,100123,100220,100221,100222,100223,106520,106521,106522,106523,
//			106620,106621,106622,106623,106720,106721,106722,106723,106820,106821,106822,106823,106920,106921,106922,106923,
//			107020,107021,107022,107023,107120,107121,107122,107123,107220,107221,107222,107223,107320,107321,107322,107323,
//			107420,107421,107422,107423,107520,107521,107522,107523,
//			110906,110907,110908,110909,110910,110911,110912,110913,111007,111008,111009,111010,111011,111012,111013,111014,
//			111107,111108,111109,111110,111111,111112,111113,111114,111115,111116,111117,111208,111209,111210,111211,111212,
//			111213,111214,111215,111216,111217,111218,111308,111309,111310,111311,111312,111313,111314,111315,111316,111317,
//			111318,111319,111320,111408,111409,111410,111411,111412,111413,111414,111415,111416,111417,111418,111419,111420,
//			111421,111422,
//			121007,121008,121009,121010,121011,121012,121013,121014,121107,121108,121109,121110,121111,121112,
//			121113,121114,121115,121116,121117,121208,121209,121210,121211,121212,121213,121214,121215,121216,121217,121218,
//			121308,121309,121310,121311,121312,121313,121314,121315,121316,121317,121318,121319,121320,121408,121409,121410,
//			121411,121412,121413,121414,121415,121416,121417,121418,121419,121420,121421,121422,
//			131208,131209,131210,131211,
//			131212,131213,131214,131215,131216,131217,131218,131308,131309,131310,131311,131312,131313,131314,131315,131316,
//			131317,131318,131319,131320,131408,131409,131410,131411,131412,131413,131414,131415,131416,131417,131418,131419,
//			131420,131421,131422,
//			141408,141409,141410,141411,141412,141413,141414,141415,141416,141417,141418,141419,141420,141421,141422
//	};
	unsigned sizeOfArray = sizeof(moduleL)/sizeof(moduleL[0]);
	for (unsigned iMod=0; iMod<sizeOfArray; ++iMod) v.push_back(moduleL[iMod]);
	return;
}

void loadTT43ModuleList(vector <unsigned> &v) {

	double moduleL[] = 	{50241,50242,50243,50244,50245,50246,50247,50248,50249,50250,50251,50252,50253,50254,50255,50256,50257,50258,50259,50342,50343,50344,50345,50346,50347,50348,50349,50350,50351,50352,50353,50354,50355,50356,50357,50358,50359,50360,50361,50362,50441,50442,50443,50444,50445,50446,50447,50448,50449,50450,50451,50452,50453,50454,50455,50456,50457,50458,50459,60344,60345,60346,60347,60348,60349,60350,60351,60352,60353,60354,60443,60444,60445,60446,60447,60448,60449,60450,60451,60452,60453,60454,60544,60545,60546,60547,60548,60549,60550,60551,60552,60553,60554,60643,60644,60645,60646,60647,60648,60649,60650,60651,60652,60653,60654,70450,70451,70452,70453,70551,70552,70553,70650,70651,70652,70653,70751,70752,70753,70850,70851,70852,70853,110002,110003,110004,110102,110103,110104,110105,110203,110204,110205,110206,110303,110304,110305,110306,110403,110404,110405,110406,110407,110504,110505,110506,110507,110508,110604,110605,110606,110607,110608,110704,110705,110706,110707,110708,110709,110804,110805,110806,110807,110808,110809,110905,110906,110907,110908,110909,110910,110911,110912,120203,120204,120205,120206,120303,120304,120305,120306,120403,120404,120405,120406,120407,120504,120505,120506,120507,120508,120604,120605,120606,120607,120608,120704,120705,120706,120707,120708,120709,120804,120805,120806,120807,120808,120809,120905,120906,120907,120908,120909,120910,120911,120912,121006,121007,121008,121009,121010,121011,121012,121013,121107,121109,121111,121113,121115,130303,130304,130305,130306,130403,130404,130405,130406,130407,130504,130505,130506,130507,130508,130604,130605,130606,130607,130608,130704,130705,130706,130707,130708,130709,130804,130805,130806,130807,130808,130809,130905,130906,130907,130908,130909,130910,130911,130912,131006,131007,131008,131009,131010,131011,131012,131013,131106,131107,131108,131109,131110,131111,131112,131113,131114,131115,131116,131207,131208,131209,131210,131211,131212,131213,131214,131215,131216,131217,140504,140505,140506,140507,140508,140604,140605,140606,140607,140608,140704,140705,140706,140707,140708,140709,140804,140805,140806,140807,140808,140809,140905,140906,140907,140908,140909,140910,140911,140912,141006,141007,141008,141009,141010,141011,141012,141013,141106,141107,141108,141109,141110,141111,141112,141113,141114,141115,141116,141207,141208,141209,141210,141211,141212,141213,141214,141215,141216,141217,141307,141308,141309,141310,141311,141312,141313,141314,141315,141316,141317,141318,141319,141407,141408,141409,141410,141411,141412,141413,141414,141415,141416,141417,141418,141419,141420,141421,150804,150805,150806,150807,150808,150809,150905,150906,150907,150908,150909,150910,150911,150912,151006,151007,151008,151009,151010,151011,151012,151013,151106,151107,151108,151109,151110,151111,151112,151113,151114,151115,151116,151207,151208,151209,151210,151211,151212,151213,151214,151215,151216,151217,151307,151308,151309,151310,151311,151312,151313,151314,151315,151316,151317,151318,151319,151407,151408,151409,151410,151411,151412,151413,151414,151415,151416,151417,151418,151419,151420,151421};
//			50241,50242,50243,50244,50245,50246,50247,50248,50249,50250,50251,50252,50253,50254,50255,50256,50257,50258,50259,50342,50343,50344,50345,50346,50347,50348,50349,50350,50351,50352,50353,50354,50355,50356,50357,50358,50359,50360,50361,50362,50441,50442,50443,50444,50445,50446,50447,50448,50449,50450,50451,50452,50453,50454,50455,50456,50457,50458,50459,60344,60345,60346,60347,60348,60349,60350,60351,60352,60353,60354,60443,60444,60445,60446,60447,60448,60449,60450,60451,60452,60453,60454,60544,60545,60546,60547,60548,60549,60550,60551,60552,60553,60554,60643,60644,60645,60646,60647,60648,60649,60650,60651,60652,60653,60654,70450,70451,70452,70453,70551,70552,70553,70650,70651,70652,70653,70751,70752,70753,70850,70851,70852,70853,110002,110003,110004,110102,110103,110104,110105,110203,110204,110205,110206,110303,110304,110305,110306,110403,110404,110405,110406,110407,110504,110505,110506,110507,110508,110604,110605,110606,110607,110608,110704,110705,110706,110707,110708,110709,110804,110805,110806,110807,110808,110809,110905,110906,110907,110908,110909,110910,110911,110912,120203,120204,120205,120206,120303,120304,120305,120306,120403,120404,120405,120406,120407,120504,120505,120506,120507,120508,120604,120605,120606,120607,120608,120704,120705,120706,120707,120708,120709,120804,120805,120806,120807,120808,120809,120905,120906,120907,120908,120909,120910,120911,120912,121006,121007,121008,121009,121010,121011,121012,121013,121107,121109,121111,121113,121115,130303,130304,130305,130306,130403,130404,130405,130406,130407,130504,130505,130506,130507,130508,130604,130605,130606,130607,130608,130704,130705,130706,130707,130708,130709,130804,130805,130806,130807,130808,130809,130905,130906,130907,130908,130909,130910,130911,130912,131006,131007,131008,131009,131010,131011,131012,131013,131106,131107,131108,131109,131110,131111,131112,131113,131114,131115,131116,131207,131208,131209,131210,131211,131212,131213,131214,131215,131216,131217,140504,140505,140506,140507,140508,140604,140605,140606,140607,140608,140704,140705,140706,140707,140708,140709,140804,140805,140806,140807,140808,140809,140905,140906,140907,140908,140909,140910,140911,140912,141006,141007,141008,141009,141010,141011,141012,141013,141106,141107,141108,141109,141110,141111,141112,141113,141114,141115,141116,141207,141208,141209,141210,141211,141212,141213,141214,141215,141216,141217,141307,141308,141309,141310,141311,141312,141313,141314,141315,141316,141317,141318,141319,141407,141408,141409,141410,141411,141412,141413,141414,141415,141416,141417,141418,141419,141420,141421,150804,150805,150806,150807,150808,150809,150905,150906,150907,150908,150909,150910,150911,150912,151006,151007,151008,151009,151010,151011,151012,151013,151106,151107,151108,151109,151110,151111,151112,151113,151114,151115,151116,151207,151208,151209,151210,151211,151212,151213,151214,151215,151216,151217,151307,151308,151309,151310,151311,151312,151313,151314,151315,151316,151317,151318,151319,151407,151408,151409,151410,151411,151412,151413,151414,151415,151416,151417,151418,151419,151420,151421};
//			50241,50242,50243,50244,50245,50246,50247,50248,50249,50250,50251,50252,50253,50254,50255,50256,50257,50258,50259,50260,50342,50343,50344,50345,50346,50347,50348,50349,50350,50351,50352,50353,50354,50355,50356,50357,50358,50359,50360,50361,50362,50441,50442,50443,50444,50445,50446,50447,50448,50449,50450,50451,50452,50453,50454,50455,50456,50457,50458,50459,50460,60344,60345,60346,60347,60348,60349,60350,60351,60352,60353,60354,60443,60444,60445,60446,60447,60448,60449,60450,60451,60452,60453,60454,60544,60545,60546,60547,60548,60549,60550,60551,60552,60553,60554,60643,60644,60645,60646,60647,60648,60649,60650,60651,60652,60653,60654,70450,70451,70452,70453,70551,70552,70553,70650,70651,70652,70653,70751,70752,70753,70850,70851,70852,70853,110002,110003,110004,110102,110103,110104,110105,110203,110204,110205,110206,110303,110304,110305,110306,110403,110404,110405,110406,110407,110504,110505,110506,110507,110508,110604,110605,110606,110607,110608,110704,110705,110706,110707,110708,110709,110804,110805,110806,110807,110808,110809,110905,110906,110907,110908,110909,110910,110911,110912,120102,120104,120203,120204,120205,120206,120303,120304,120305,120306,120403,120404,120405,120406,120407,120504,120505,120506,120507,120508,120604,120605,120606,120607,120608,120704,120705,120706,120707,120708,120709,120804,120805,120806,120807,120808,120809,120905,120906,120907,120908,120909,120910,120911,120912,121006,121007,121008,121009,121010,121011,121012,121013,121107,121109,121111,121113,121115,130303,130304,130305,130306,130403,130404,130405,130406,130407,130504,130505,130506,130507,130508,130604,130605,130606,130607,130608,130704,130705,130706,130707,130708,130709,130804,130805,130806,130807,130808,130809,130905,130906,130907,130908,130909,130910,130911,130912,131006,131007,131008,131009,131010,131011,131012,131013,131106,131107,131108,131109,131110,131111,131112,131113,131114,131115,131116,131207,131208,131209,131210,131211,131212,131213,131214,131215,131216,131217,140504,140505,140506,140507,140508,140604,140605,140606,140607,140608,140704,140705,140706,140707,140708,140709,140804,140805,140806,140807,140808,140809,140905,140906,140907,140908,140909,140910,140911,140912,141006,141007,141008,141009,141010,141011,141012,141013,141106,141107,141108,141109,141110,141111,141112,141113,141114,141115,141116,141207,141208,141209,141210,141211,141212,141213,141214,141215,141216,141217,141307,141308,141309,141310,141311,141312,141313,141314,141315,141316,141317,141318,141319,141407,141408,141409,141410,141411,141412,141413,141414,141415,141416,141417,141418,141419,141420,141421,150804,150805,150806,150807,150808,150809,150905,150906,150907,150908,150909,150910,150911,150912,151006,151007,151008,151009,151010,151011,151012,151013,151106,151107,151108,151109,151110,151111,151112,151113,151114,151115,151116,151207,151208,151209,151210,151211,151212,151213,151214,151215,151216,151217,151307,151308,151309,151310,151311,151312,151313,151314,151315,151316,151317,151318,151319,151407,151408,151409,151410,151411,151412,151413,151414,151415,151416,151417,151418,151419,151420,151421};
//			50241,50242,50243,50244,50245,50246,50247,50248,50249,50250,50251,50252,50253,50254,50255,50256,50257,50258,50259,50260,50342,50343,50344,50345,50346,50347,50348,50349,50350,50351,50352,50353,50354,50355,50356,50357,50358,50359,50360,50361,50362,50441,50442,50443,50444,50445,50446,50447,50448,50449,50450,50451,50452,50453,50454,50455,50456,50457,50458,50459,50460,60344,60345,60346,60347,60348,60349,60350,60351,60352,60353,60354,60442,60443,60444,60445,60446,60447,60448,60449,60450,60451,60452,60453,60454,60544,60545,60546,60547,60548,60549,60550,60551,60552,60553,60554,60642,60643,60644,60645,60646,60647,60648,60649,60650,60651,60652,60653,60654,70449,70450,70451,70452,70453,70550,70551,70552,70553,70649,70650,70651,70652,70653,70750,70751,70752,70753,70849,70850,70851,70852,70853,70950,70951,70952,70953,110001,110002,110003,110004,110102,110103,110104,110105,110202,110203,110204,110205,110206,110302,110303,110304,110305,110306,110403,110404,110405,110406,110407,110503,110504,110505,110506,110507,110508,110509,110603,110604,110605,110606,110607,110608,110609,110703,110704,110705,110706,110707,110708,110709,110710,110803,110804,110805,110806,110807,110808,110809,110810,110904,110905,110906,110907,110908,110909,110910,110911,110912,110913,120102,120104,120202,120203,120204,120205,120206,120302,120303,120304,120305,120306,120403,120404,120405,120406,120407,120503,120504,120505,120506,120507,120508,120509,120603,120604,120605,120606,120607,120608,120609,120703,120704,120705,120706,120707,120708,120709,120710,120803,120804,120805,120806,120807,120808,120809,120810,120904,120905,120906,120907,120908,120909,120910,120911,120912,120913,121004,121005,121006,121007,121008,121009,121010,121011,121012,121013,121014,121015,121105,121106,121107,121108,121109,121110,121111,121112,121113,121114,121115,121116,121117,130302,130303,130304,130305,130306,130403,130404,130405,130406,130407,130503,130504,130505,130506,130507,130508,130509,130603,130604,130605,130606,130607,130608,130609,130703,130704,130705,130706,130707,130708,130709,130710,130803,130804,130805,130806,130807,130808,130809,130810,130904,130905,130906,130907,130908,130909,130910,130911,130912,130913,131004,131005,131006,131007,131008,131009,131010,131011,131012,131013,131014,131015,131105,131106,131107,131108,131109,131110,131111,131112,131113,131114,131115,131116,131117,131205,131206,131207,131208,131209,131210,131211,131212,131213,131214,131215,131216,131217,131218,140503,140504,140505,140506,140507,140508,140509,140603,140604,140605,140606,140607,140608,140609,140703,140704,140705,140706,140707,140708,140709,140710,140803,140804,140805,140806,140807,140808,140809,140810,140904,140905,140906,140907,140908,140909,140910,140911,140912,140913,141004,141005,141006,141007,141008,141009,141010,141011,141012,141013,141014,141015,141105,141106,141107,141108,141109,141110,141111,141112,141113,141114,141115,141116,141117,141205,141206,141207,141208,141209,141210,141211,141212,141213,141214,141215,141216,141217,141218,141306,141307,141308,141309,141310,141311,141312,141313,141314,141315,141316,141317,141318,141319,141320,141321,141406,141407,141408,141409,141410,141411,141412,141413,141414,141415,141416,141417,141418,141419,141420,141421,141422,150803,150804,150805,150806,150807,150808,150809,150810,150904,150905,150906,150907,150908,150909,150910,150911,150912,150913,151004,151005,151006,151007,151008,151009,151010,151011,151012,151013,151014,151015,151105,151106,151107,151108,151109,151110,151111,151112,151113,151114,151115,151116,151117,151205,151206,151207,151208,151209,151210,151211,151212,151213,151214,151215,151216,151217,151218,151306,151307,151308,151309,151310,151311,151312,151313,151314,151315,151316,151317,151318,151319,151320,151321,151406,151407,151408,151409,151410,151411,151412,151413,151414,151415,151416,151417,151418,151419,151420,151421,151422};
//			50241,50242,50243,50244,50245,50246,50247,50248,50249,50250,50251,50252,50253,50254,50255,50256,50257,50258,50259,50260,50342,50343,50344,50345,50346,50347,50348,50349,50350,50351,50352,50353,50354,50355,50356,50357,50358,50359,50360,50361,50362,50441,50442,50443,50444,50445,50446,50447,50448,50449,50450,50451,50452,50453,50454,50455,50456,50457,50458,50459,50460,60344,60345,60346,60347,60348,60349,60350,60351,60352,60353,60354,60443,60444,60445,60446,60447,60448,60449,60450,60451,60452,60453,60454,60544,60545,60546,60547,60548,60549,60550,60551,60552,60553,60554,60643,60644,60645,60646,60647,60648,60649,60650,60651,60652,60653,60654,70351,70352,70353,70450,70451,70452,70453,70551,70552,70553,70650,70651,70652,70653,70751,70752,70753,70850,70851,70852,70853,70951,70952,70953,110001,110002,110003,110004,110102,110103,110104,110105,110202,110203,110204,110205,110206,110207,110302,110303,110304,110305,110306,110307,110402,110403,110404,110405,110406,110407,110408,110503,110504,110505,110506,110507,110508,110509,110603,110604,110605,110606,110607,110608,110609,110703,110704,110705,110706,110707,110708,110709,110710,110803,110804,110805,110806,110807,110808,110809,110810,110904,110905,110906,110907,110908,110909,110910,110911,110912,110913,110914,120102,120104,120202,120203,120204,120205,120206,120207,120302,120303,120304,120305,120306,120307,120402,120403,120404,120405,120406,120407,120408,120503,120504,120505,120506,120507,120508,120509,120603,120604,120605,120606,120607,120608,120609,120703,120704,120705,120706,120707,120708,120709,120710,120803,120804,120805,120806,120807,120808,120809,120810,120904,120905,120906,120907,120908,120909,120910,120911,120912,120913,120914,121004,121005,121006,121007,121008,121009,121010,121011,121012,121013,121014,121015,121105,121107,121109,121111,121113,121115,121117,130302,130303,130304,130305,130306,130307,130402,130403,130404,130405,130406,130407,130408,130503,130504,130505,130506,130507,130508,130509,130603,130604,130605,130606,130607,130608,130609,130703,130704,130705,130706,130707,130708,130709,130710,130803,130804,130805,130806,130807,130808,130809,130810,130904,130905,130906,130907,130908,130909,130910,130911,130912,130913,130914,131004,131005,131006,131007,131008,131009,131010,131011,131012,131013,131014,131015,131104,131105,131106,131107,131108,131109,131110,131111,131112,131113,131114,131115,131116,131117,131118,131205,131206,131207,131208,131209,131210,131211,131212,131213,131214,131215,131216,131217,131218,131219,140503,140504,140505,140506,140507,140508,140509,140603,140604,140605,140606,140607,140608,140609,140703,140704,140705,140706,140707,140708,140709,140710,140803,140804,140805,140806,140807,140808,140809,140810,140904,140905,140906,140907,140908,140909,140910,140911,140912,140913,140914,141004,141005,141006,141007,141008,141009,141010,141011,141012,141013,141014,141015,141104,141105,141106,141107,141108,141109,141110,141111,141112,141113,141114,141115,141116,141117,141118,141205,141206,141207,141208,141209,141210,141211,141212,141213,141214,141215,141216,141217,141218,141219,141305,141306,141307,141308,141309,141310,141311,141312,141313,141314,141315,141316,141317,141318,141319,141320,141321,141322,141405,141406,141407,141408,141409,141410,141411,141412,141413,141414,141415,141416,141417,141418,141419,141420,141421,141422,141423,150803,150804,150805,150806,150807,150808,150809,150810,150904,150905,150906,150907,150908,150909,150910,150911,150912,150913,150914,151004,151005,151006,151007,151008,151009,151010,151011,151012,151013,151014,151015,151104,151105,151106,151107,151108,151109,151110,151111,151112,151113,151114,151115,151116,151117,151118,151205,151206,151207,151208,151209,151210,151211,151212,151213,151214,151215,151216,151217,151218,151219,151305,151306,151307,151308,151309,151310,151311,151312,151313,151314,151315,151316,151317,151318,151319,151320,151321,151322,151405,151406,151407,151408,151409,151410,151411,151412,151413,151414,151415,151416,151417,151418,151419,151420,151421,151422,151423};
//		double moduleL[] = { // this is sector 6,4 and 6,2 in mersi's DB
//				50240,50241,50242,50243,50244,50245,50246,50247,50248,50249,50250,50251,50252,50253,50254,50255,50256,50257,50258
//				,50259,50260,50342,50343,50344,50345,50346,50347,50348,50349,50350,50351,50352,50353,50354,50355,50356,50357,50358
//				,50359,50360,50361,50362,50440,50441,50442,50443,50444,50445,50446,50447,50448,50449,50450,50451,50452,50453,50454
//				,50455,50456,50457,50458,50459,50460
//				,60344,60345,60346,60347,60348,60349,60350,60351,60352,60353,60354,60442,60443,60444,60445,60446,60447,60448,60449
//				,60450,60451,60452,60453,60454,60544,60545,60546,60547,60548,60549,60550,60551,60552,60553,60554,60642,60643,60644
//				,60645,60646,60647,60648,60649,60650,60651,60652,60653,60654
//				,70449,70450,70451,70452,70453,70551,70552,70553,70649,70650,70651,70652,70653,70751,70752,70753,70849,70850,70851
//				,70852,70853,70951,70952,70953
//				,110002,110003,110004,110005,110103,110104,110105,110106,110203,110204,110205,110206,110207,110208,110303,110304
//				,110305,110306,110307,110404,110405,110406,110407,110408,110504,110505,110506,110507,110508,110509,110604,110605
//				,110606,110607,110608,110609,110705,110706,110707,110708,110709,110710,110805,110806,110807,110808,110809,110810
//				,110906,110907,110908,110909,110910,110911,110912,110913
//				,120203,120204,120205,120206,120207,120208,120303,120304,120305,120306,120307,120404,120405,120406,120407,120408
//				,120504,120505,120506,120507,120508,120509,120604,120605,120606,120607,120608,120609,120705,120706,120707,120708
//				,120709,120710,120805,120806,120807,120808,120809,120810,120906,120907,120908,120909,120910,120911,120912,120913
//				,121007,121008,121009,121010,121011,121012,121013,121014,121107,121108,121109,121110,121111,121112,121113,121114
//				,121115,121116,121117
//				,130303,130305,130307,130404,130405,130406,130407,130408,130504,130505,130506,130507,130508,130509,130604,130605
//				,130606,130607,130608,130609,130705,130706,130707,130708,130709,130710,130805,130806,130807,130808,130809,130810
//				,130906,130907,130908,130909,130910,130911,130912,130913,131007,131008,131009,131010,131011,131012,131013,131014
//				,131107,131108,131109,131110,131111,131112,131113,131114,131115,131116,131117,131208,131209,131210,131211,131212
//				,131213,131214,131215,131216,131217,131218
//				,140505,140507,140509,140604,140605,140606,140607,140608,140609,140705,140706,140707,140708,140709,140710,140805
//				,140806,140807,140808,140809,140810,140906,140907,140908,140909,140910,140911,140912,140913,141007,141008,141009
//				,141010,141011,141012,141013,141014,141107,141108,141109,141110,141111,141112,141113,141114,141115,141116,141117
//				,141208,141209,141210,141211,141212,141213,141214,141215,141216,141217,141218,141308,141309,141310,141311,141312
//				,141313,141314,141315,141316,141317,141318,141319,141320,141408,141409,141410,141411,141412,141413,141414,141415
//				,141416,141417,141418,141419,141420,141421,141422
//				,150805,150806,150807,150808,150809,150810,150906,150907,150908,150909,150910,150911,150912,150913,151007,151008
//				,151009,151010,151011,151012,151013,151014,151107,151108,151109,151110,151111,151112,151113,151114,151115,151116
//				,151117,151208,151209,151210,151211,151212,151213,151214,151215,151216,151217,151218,151308,151309,151310,151311
//				,151312,151313,151314,151315,151316,151317,151318,151319,151320,151408,151409,151410,151411,151412,151413,151414
//				,151415,151416,151417,151418,151419,151420,151421,151422};

//	double moduleL[] = { // this is JF tower, built out of 2.0GeV outgoing tracks
//			50241,50242,50243,50244,50245,50246,50247,50248,50249,50250,50251,50252,50253,50254,50255,50256,50257,50258
//			,50259,50260,50342,50343,50344,50345,50346,50347,50348,50349,50350,50351,50352,50353,50354,50355,50356,50357,50358
//			,50359,50360,50361,50362,50441,50442,50443,50444,50445,50446,50447,50448,50449,50450,50451,50452,50453,50454,50455
//			,50456,50457,50458,50459,50460
//			,60344,60345,60346,60347,60348,60349,60350,60351,60352,60353,60354,60443,60444,60445,60446,60447,60448,60449,60450
//			,60451,60452,60453,60454,60544,60545,60546,60547,60548,60549,60550,60551,60552,60553,60554,60643,60644,60645,60646
//			,60647,60648,60649,60650,60651,60652,60653,60654
//			,70450,70451,70452,70453,70551,70552,70553,70650,70651,70652,70653,70751,70752,70753,70850,70851,70852,70853
//			,110002,110003,110004,110102,110103,110104,110105,110203,110204,110205,110206,110303,110304,110305,110306,110403
//			,110404,110405,110406,110407,110504,110505,110506,110507,110508,110604,110605,110606,110607,110608,110704,110705
//			,110706,110707,110708,110709,110804,110805,110806,110807,110808,110809,110905,110906,110907,110908,110909,110910
//			,110911,110912
//			,120102,120104,120203,120204,120205,120206,120303,120304,120305,120306,120403,120404,120405,120406,120407,120504
//			,120505,120506,120507,120508,120604,120605,120606,120607,120608,120704,120705,120706,120707,120708,120709,120804
//			,120805,120806,120807,120808,120809,120905,120906,120907,120908,120909,120910,120911,120912,121006,121007,121008
//			,121009,121010,121011,121012,121013,121107,121109,121111,121113,121115
//			,130303,130304,130305,130306,130403,130404,130405,130406,130407,130504,130505,130506,130507,130508,130604,130605
//			,130606,130607,130608,130704,130705,130706,130707,130708,130709,130804,130805,130806,130807,130808,130809,130905
//			,130906,130907,130908,130909,130910,130911,130912,131006,131007,131008,131009,131010,131011,131012,131013,131106
//			,131107,131108,131109,131110,131111,131112,131113,131114,131115,131116,131207,131208,131209,131210,131211,131212
//			,131213,131214,131215,131216,131217
//			,140504,140505,140506,140507,140508,140604,140605,140606,140607,140608,140704,140705,140706,140707,140708,140709
//			,140804,140805,140806,140807,140808,140809,140905,140906,140907,140908,140909,140910,140911,140912,141006,141007
//			,141008,141009,141010,141011,141012,141013,141106,141107,141108,141109,141110,141111,141112,141113,141114,141115
//			,141116,141207,141208,141209,141210,141211,141212,141213,141214,141215,141216,141217,141307,141308,141309,141310
//			,141311,141312,141313,141314,141315,141316,141317,141318,141319,141407,141408,141409,141410,141411,141412,141413
//			,141414,141415,141416,141417,141418,141419,141420,141421
//			,150804,150805,150806,150807,150808,150809,150905,150906,150907,150908,150909,150910,150911,150912,151006,151007
//			,151008,151009,151010,151011,151012,151013,151106,151107,151108,151109,151110,151111,151112,151113,151114,151115
//			,151116,151207,151208,151209,151210,151211,151212,151213,151214,151215,151216,151217,151307,151308,151309,151310
//			,151311,151312,151313,151314,151315,151316,151317,151318,151319,151407,151408,151409,151410,151411,151412,151413
//			,151414,151415,151416,151417,151418,151419,151420,151421
//	};
//	double moduleL[] = { // this is sector 6,4 and 6,2 in mersi's DB
//			50240,50241,50242,50243,50244,50245,50246,50247,50248,50249,50250,50251,50252,50253,50254,50255,50256,50257,50258
//			,50259,50260,50342,50343,50344,50345,50346,50347,50348,50349,50350,50351,50352,50353,50354,50355,50356,50357,50358
//			,50359,50360,50361,50362,50440,50441,50442,50443,50444,50445,50446,50447,50448,50449,50450,50451,50452,50453,50454
//			,50455,50456,50457,50458,50459,50460
//			,60344,60345,60346,60347,60348,60349,60350,60351,60352,60353,60354,60442,60443,60444,60445,60446,60447,60448,60449
//			,60450,60451,60452,60453,60454,60544,60545,60546,60547,60548,60549,60550,60551,60552,60553,60554,60642,60643,60644
//			,60645,60646,60647,60648,60649,60650,60651,60652,60653,60654
//			,70449,70450,70451,70452,70453,70551,70552,70553,70649,70650,70651,70652,70653,70751,70752,70753,70849,70850,70851
//			,70852,70853,70951,70952,70953
//			,110002,110003,110004,110005,110103,110104,110105,110106,110203,110204,110205,110206,110207,110208,110303,110304
//			,110305,110306,110307,110404,110405,110406,110407,110408,110504,110505,110506,110507,110508,110509,110604,110605
//			,110606,110607,110608,110609,110705,110706,110707,110708,110709,110710,110805,110806,110807,110808,110809,110810
//			,110906,110907,110908,110909,110910,110911,110912,110913
//			,120203,120204,120205,120206,120207,120208,120303,120304,120305,120306,120307,120404,120405,120406,120407,120408
//			,120504,120505,120506,120507,120508,120509,120604,120605,120606,120607,120608,120609,120705,120706,120707,120708
//			,120709,120710,120805,120806,120807,120808,120809,120810,120906,120907,120908,120909,120910,120911,120912,120913
//			,121007,121008,121009,121010,121011,121012,121013,121014,121107,121108,121109,121110,121111,121112,121113,121114
//			,121115,121116,121117
//			,130303,130305,130307,130404,130405,130406,130407,130408,130504,130505,130506,130507,130508,130509,130604,130605
//			,130606,130607,130608,130609,130705,130706,130707,130708,130709,130710,130805,130806,130807,130808,130809,130810
//			,130906,130907,130908,130909,130910,130911,130912,130913,131007,131008,131009,131010,131011,131012,131013,131014
//			,131107,131108,131109,131110,131111,131112,131113,131114,131115,131116,131117,131208,131209,131210,131211,131212
//			,131213,131214,131215,131216,131217,131218
//			,140505,140507,140509,140604,140605,140606,140607,140608,140609,140705,140706,140707,140708,140709,140710,140805
//			,140806,140807,140808,140809,140810,140906,140907,140908,140909,140910,140911,140912,140913,141007,141008,141009
//			,141010,141011,141012,141013,141014,141107,141108,141109,141110,141111,141112,141113,141114,141115,141116,141117
//			,141208,141209,141210,141211,141212,141213,141214,141215,141216,141217,141218,141308,141309,141310,141311,141312
//			,141313,141314,141315,141316,141317,141318,141319,141320,141408,141409,141410,141411,141412,141413,141414,141415
//			,141416,141417,141418,141419,141420,141421,141422
//			,150805,150806,150807,150808,150809,150810,150906,150907,150908,150909,150910,150911,150912,150913,151007,151008
//			,151009,151010,151011,151012,151013,151014,151107,151108,151109,151110,151111,151112,151113,151114,151115,151116
//			,151117,151208,151209,151210,151211,151212,151213,151214,151215,151216,151217,151218,151308,151309,151310,151311
//			,151312,151313,151314,151315,151316,151317,151318,151319,151320,151408,151409,151410,151411,151412,151413,151414
//			,151415,151416,151417,151418,151419,151420,151421,151422
//	};
	unsigned sizeOfArray = sizeof(moduleL)/sizeof(moduleL[0]);
	for (unsigned iMod=0; iMod<sizeOfArray; ++iMod) v.push_back(moduleL[iMod]);
	return;
}

void findFactors(unsigned num, unsigned &f1, unsigned &f2) {
	f1=1;
	f2=1;
	unsigned sqroot = TMath::CeilNint(TMath::Sqrt(num));
	for (unsigned if1=sqroot; if1<num+1; ++if1) {
		if (num%if1==0) {
			f1=if1;
			f2=num/if1;
			return;
		}
	}
	return;
}


