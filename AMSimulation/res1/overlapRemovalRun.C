#define overlapRemoval_cxx
#include "overlapRemoval.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLatex.h>

#include <iostream>
#include <fstream>
#include <iomanip> // for setw

void genTrackPropagate(double *genTrackPt_Phi0_CotTheta, double r, double *genTrackXYZ);
void loadTT27ModuleList(vector <unsigned> &v);
void findFactors(unsigned num, unsigned &f1, unsigned &f2);

void overlapRemoval::Loop(Long64_t nMax,bool makePlots,float minPt,TString sBeamDisplaced)
{

	if (fChain == 0) return;

	TString outDir("/home/rossin/Dropbox/TT/Work/figures_stubOverlapRemoval/");

	vector <unsigned> vModuleId;
	loadTT27ModuleList(vModuleId);
//	std::cout<< vModuleId.size() << std::endl;
	std::map<unsigned,unsigned> moduleIdMap;
	const unsigned nMod = vModuleId.size();
	TH2* h2stubLocalCoordAll [nMod];
	TH2* h2stubLocalCoordDupl[nMod];
	TH2* h2stubLocalCoordEff[nMod];
	unsigned nModules[6] = {0,0,0,0,0,0};
	int nBinTIB = 960/2;
	int nBinTOB = 1016/2;
	for (unsigned iMod=0; iMod<vModuleId.size(); ++iMod) {
		moduleIdMap[vModuleId[iMod]]=iMod;

		char hName [100];
		char hName2[100];
		short int iLay = TMath::Floor(vModuleId[iMod]/10000.)-5;
		++nModules[iLay];
		sprintf(hName ,"h2stubLocalCoordAll_ModuleId%d",vModuleId[iMod]);
		sprintf(hName2,"h2stubLocalCoordDupl_ModuleId%d",vModuleId[iMod]);
		if (iLay<3) {
			h2stubLocalCoordAll [iMod] = new TH2F(hName ,hName ,nBinTIB,0,960,32,0,32);
			h2stubLocalCoordDupl[iMod] = new TH2F(hName2,hName2,nBinTIB,0,960,32,0,32);
		}
		else        {
			h2stubLocalCoordAll [iMod] = new TH2F(hName ,hName ,nBinTOB,0,1016,2,0,2);
			h2stubLocalCoordDupl[iMod] = new TH2F(hName2,hName2,nBinTOB,0,1016,2,0,2);
		}
	}
	for (unsigned iLay=0; iLay<6; ++iLay) std::cout << nModules[iLay] << "\t" ;
	std::cout << std::endl;


	TFile* fRadii = 0; bool isfOpen = false;
	fRadii = new TFile("barrelRadii.root");
	isfOpen = fRadii->IsOpen();
	if (!isfOpen) {
		cout << "ERROR. Not able to load the radii file. Exiting..." << endl;
		return;
	}
	TH1* hRadii = (TH1S*) fRadii->Get("hRadii");
	if (!hRadii) {
		std::cout << "ERROR loading hRadii. Exiting..." << std::endl;
	}
	TCanvas* cRadii = new TCanvas("cRadii","cRadii",80,80,900,900);
	hRadii->DrawCopy();


	Long64_t nentries = fChain->GetEntriesFast();
	nentries = TMath::Min(nentries,nMax);

	TH1* h1StubTrackResiduals       [6][3];
	TH1* h1StubTrackResidualsXYZ[6][3];
	TH1* h1StubTrackResidualsXYZDupl[6][3];
	TString sXYZ[3];
	sXYZ[0] = TString("X");
	sXYZ[1] = TString("Y");
	sXYZ[2] = TString("Z");

	TH2* h2stubLocalCoord[6];

	for (unsigned iLay=0; iLay<6; ++iLay) {
		char hName[100];
		sprintf(hName,"h2stubLocalCoord_Layer%d",iLay);
		if (iLay<3) h2stubLocalCoord[iLay] = new TH2D(hName,hName,nBinTIB,0,960,32,0,32);
		else        h2stubLocalCoord[iLay] = new TH2D(hName,hName,nBinTOB,0,1016,2,0,2);
		for (unsigned iXYZ=0; iXYZ<3; ++iXYZ) {
			sprintf(hName,"h1StubTrackResiduals_Layer%d_%s",iLay,sXYZ[iXYZ].Data());
			h1StubTrackResiduals[iLay][iXYZ] = new TH1F(hName,hName,200,-2,2);
			sprintf(hName,"h1StubTrackResidualsXYZ_Layer%d_%s",iLay,sXYZ[iXYZ].Data());
			h1StubTrackResidualsXYZ[iLay][iXYZ] = new TH1F(hName,hName,200,-2,2);
			sprintf(hName,"h1StubTrackResidualsXYZDupl_Layer%d_%s",iLay,sXYZ[iXYZ].Data());
			h1StubTrackResidualsXYZDupl[iLay][iXYZ] = new TH1F(hName,hName,200,-2,2);
		}
	}

	TH1* h1StubRadius = new TH1F("h1StubRadius","h1StubRadius",4800,0,120);

	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		if (!genParts_pt->size()   ) continue;
		if (genParts_pt->size()!=1   ) continue;

		vector <float> vStubsR[6];
		float aaDeltaXYZCuts[6][3] = {
				{0.3,0.2,0.4},
				{0.4,0.3,0.5},
				{0.6,0.4,0.6},
				{0.9,0.6,3.0},
				{1.2,0.8,4.0},
				{1.4,1.0,4.0}
		};

		double genTrackPt_Phi0[2] = {-genParts_pt->at(0)*genParts_charge->at(0), genParts_phi->at(0)};
		if (genTrackPt_Phi0[0]<minPt) continue;

		std::map<short,stub> stubMapClean;

//		if (TTStubs_r->size()>6) std::cout << jentry << "\t" <<  TTStubs_r->size() << std::endl;

		for (unsigned iStub=0; iStub<TTStubs_r->size(); ++iStub) {

			std::map<unsigned,unsigned>::iterator itMod = moduleIdMap.find(TTStubs_modId->at(iStub));
//			if (itMod == moduleIdMap.end()) continue; // does not belong to TT27

			short int stub_layer = TMath::Floor(TTStubs_modId   ->at(iStub)/10000.)-5;
			if (stub_layer<0 || stub_layer>5) continue; // look only in the barrel

			unsigned moduleId = TTStubs_modId   ->at(iStub);
//			moduleIdMap[moduleId] = moduleId;

			double stubRadius = TTStubs_r->at(iStub);
			h1StubRadius->Fill(stubRadius);

			double stubLocalX = TTStubs_coordx->at(iStub);
			double stubLocalY = TTStubs_coordy->at(iStub);
			h2stubLocalCoord[stub_layer]->Fill(stubLocalX,stubLocalY);

			double genTrackXYZ[3];
			genTrackPropagate(genTrackPt_Phi0,TTStubs_r->at(iStub)/100.,genTrackXYZ);
			double genPart_CotTheta = TMath::SinH(genParts_eta->at(0));
			genTrackXYZ[0] += genParts_vx->at(0)/100.;
			genTrackXYZ[1] += genParts_vy->at(0)/100.;
			genTrackXYZ[2]  = stubRadius*genPart_CotTheta/100.+genParts_vz->at(0)/100.;

			double stubTrackResiduals[3];
			stubTrackResiduals[0] = TTStubs_x->at(iStub)-genTrackXYZ[0]*100.;
			stubTrackResiduals[1] = TTStubs_y->at(iStub)-genTrackXYZ[1]*100.;
			stubTrackResiduals[2] = TTStubs_z->at(iStub)-genTrackXYZ[2]*100.;

			bool keepStub = true;
			for (unsigned iXYZ=0; iXYZ<3; ++iXYZ) {
				h1StubTrackResiduals[stub_layer][iXYZ]->Fill(stubTrackResiduals[iXYZ]);
				if (TMath::Abs(stubTrackResiduals[iXYZ])>aaDeltaXYZCuts[stub_layer][iXYZ]) {
					keepStub = false;
					continue;
				}
			}
			if (keepStub) {
				for (unsigned iXYZ=0; iXYZ<3; ++iXYZ) h1StubTrackResidualsXYZ[stub_layer][iXYZ]->Fill(stubTrackResiduals[iXYZ]);
			}
			else continue;

			double stubResidualXY2 = stubTrackResiduals[0]*stubTrackResiduals[0]+stubTrackResiduals[1]*stubTrackResiduals[1];
			short stubLayerSublayer = hRadii->GetBinContent(hRadii->FindBin(stubRadius));
//			if (!stubLayerSublayer) {
//				cout << stubRadius << "\t" << hRadii->FindBin(stubRadius) << "\t" << stubLayerSublayer << endl;
//				return;
//			}
			std::map<short,stub>::iterator it = stubMapClean.find(stubLayerSublayer);
			if (it!=stubMapClean.end()) { // if there is already a stub in the sublayer
				if (stubResidualXY2 < it->second.deltaXY) { // check whether it is closer to the track.
					stubMapClean.erase(it); // if so remove the other one
				}
			}
			stubMapClean.insert(std::make_pair(stubLayerSublayer,stub(iStub,stub_layer,(int)(stubLayerSublayer)%10,stubRadius,stubResidualXY2))); // insert the stub. if there is another one is not the closet one. it will not be inserted


			//		   std::cout << setw(7) << iStub << setw(15)
			//				   <<  100*genTrackXYZ[0] << setw(15) <<  100*genTrackXYZ[1]  << setw(15) <<  100*genTrackXYZ[2]  << setw(15)
			//				   <<  TTStubs_r->at(iStub)*TMath::Cos(TTStubs_phi->at(iStub))  << setw(15)
			//				   <<  TTStubs_r->at(iStub)*TMath::Sin(TTStubs_phi->at(iStub))  << setw(15)
			//        		   <<  TTStubs_z->at(iStub) << std::endl;
		} // end looping over stubs

		vector<unsigned> vStubsSubLayer[6];

		for (std::map<short,stub>::iterator it = stubMapClean.begin(); it!=stubMapClean.end(); ++it) {
			vStubsSubLayer[it->second.layer].push_back(it->second.sublayer);
		}
		for (unsigned iLay=0; iLay<6; ++iLay) { // sorting each vector. innermost sublayer will be in [0]
			std::sort(vStubsSubLayer[iLay].begin(),vStubsSubLayer[iLay].end());
		}

		for (std::map<short,stub>::iterator it = stubMapClean.begin(); it!=stubMapClean.end(); ++it) {
//			if (stubMapClean.size()>48) std::cout << stubMapClean.size() << "\t"
//					<< it->second.iStub << "\t"
//					<< it->second.layer << "."
//					<< it->second.sublayer << "\t"
//					<< it->second.r << "\t"
//					<< TTStubs_coordx->at(it->second.iStub) << "\t"
//					<< TTStubs_coordy->at(it->second.iStub) << "\t"
//					<< it->second.deltaXY << "\t"
//					<< std::endl;
			unsigned iStub = it->second.iStub;

			std::map<unsigned,unsigned>::iterator itMod = moduleIdMap.find(TTStubs_modId->at(iStub));
			if (itMod==moduleIdMap.end()) continue;
			unsigned iMod = moduleIdMap.find(TTStubs_modId->at(iStub))->second;
			h2stubLocalCoordAll[iMod]->Fill(TTStubs_coordx->at(iStub),TTStubs_coordy->at(iStub));
			// Filling dupl histo if and only if the hit is in the innermost sublayer
			if (vStubsSubLayer[it->second.layer][0]==it->second.sublayer) h2stubLocalCoordDupl[iMod]->Fill(TTStubs_coordx->at(iStub),TTStubs_coordy->at(iStub));

//    OLD WAY. getting regions to remove.
//			if (vStubsSubLayer[it->second.layer].size()>1) {
//				if (vStubsSubLayer[it->second.layer][0]<it->second.sublayer) h2stubLocalCoordDupl[iMod]->Fill(TTStubs_coordx->at(iStub),TTStubs_coordy->at(iStub));
//			}
		}


		if (jentry%100000==0 ) {
			std::cout << "Processing event " << jentry << setw(15) <<  std::endl;
		}
	}
//	fRadii->Close();

	std::cout << "Total # modules in TT27: " << moduleIdMap.size() << std::endl;


	TCanvas* cstubTrackResiduals = new TCanvas("cstubTrackResiduals","cstubTrackResiduals",0,0,900,900);
	cstubTrackResiduals->Divide(6,3);
	int count=0;
	for (unsigned iXYZ=0; iXYZ<3; ++iXYZ) {
		for (unsigned iLay=0; iLay<6; ++iLay) {
			cstubTrackResiduals->cd(++count);
			gPad->SetLogy();
			h1StubTrackResiduals[iLay][iXYZ]->DrawCopy();
			h1StubTrackResidualsXYZ[iLay][iXYZ]->SetLineColor(2);
			h1StubTrackResidualsXYZ[iLay][iXYZ]->DrawCopy("same");
		}
	}

	TCanvas* cStubRadius = new TCanvas("cStubRadius","cStubRadius",50,50,1700,900);
	cStubRadius->SetLogy();
	h1StubRadius->DrawCopy();
	if (makePlots) {
		char cc[100];
		sprintf(cc,"stubRadii");
		cStubRadius->SaveAs(outDir+TString(cc)+TString(".C"));
		cStubRadius->SaveAs(outDir+TString(cc)+TString(".pdf"));
		cStubRadius->SaveAs(outDir+TString(cc)+TString(".png"));
	}


	TCanvas* c2stubLocalCoord = new TCanvas("c2stubLocalCoord","c2stubLocalCoord",50,0,900,900);
	c2stubLocalCoord->Divide (3,2);
	for (unsigned iLay=0; iLay<6; ++iLay) {
		c2stubLocalCoord->cd(iLay+1);
		h2stubLocalCoord[iLay]->DrawCopy("colz0");
	}


	TFile* fOutHistos = new TFile(outDir+TString("stubOverlapHistos")+TString(sBeamDisplaced)+TString(".root"),"recreate");


	char cMinPt[10];
	sprintf(cMinPt,"_minPt%02.0f",minPt);

	unsigned ih2stubLocalCoordAll=0;
	TCanvas* c2stubLocalCoordByModule[6];
	for (unsigned iLay=0; iLay<6; ++iLay) {
		char cc[100];
		sprintf(cc,"c2stubLocalCoordByModule%s_Layer%d",TString(sBeamDisplaced).Data(),iLay);
		c2stubLocalCoordByModule[iLay] = new TCanvas(cc,cc,50,0,1700,900);
		unsigned nx, ny;
		findFactors(nModules[iLay], nx,ny);
		cout << iLay << "\t" << nModules[iLay] << "\t" << nx << ":" << ny << endl;
		c2stubLocalCoordByModule[iLay]->Divide(nx,ny);
		for (unsigned iMod=0; iMod<nModules[iLay]; ++iMod) {
			c2stubLocalCoordByModule[iLay]->cd(iMod+1);
			h2stubLocalCoordAll[ih2stubLocalCoordAll]->Write();
			h2stubLocalCoordAll[ih2stubLocalCoordAll++]->DrawCopy("colz0");
		}
		if (makePlots) {
			sprintf(cc,"stubLocalCoord_Layer%d",iLay);
			c2stubLocalCoordByModule[iLay]->SaveAs(outDir+TString(cc)+TString(sBeamDisplaced)+TString(cMinPt)+TString(".C"));
			c2stubLocalCoordByModule[iLay]->SaveAs(outDir+TString(cc)+TString(sBeamDisplaced)+TString(cMinPt)+TString(".pdf"));
			c2stubLocalCoordByModule[iLay]->SaveAs(outDir+TString(cc)+TString(sBeamDisplaced)+TString(cMinPt)+TString(".png"));
		}
	}

	unsigned ih2stubLocalCoordDupl=0;
	TCanvas* c2stubLocalCoordByModuleDupl[6];
	for (unsigned iLay=0; iLay<6; ++iLay) {
		char cc[100];
		sprintf(cc,"c2stubLocalCoordByModuleDupl%s_Layer%d",TString(sBeamDisplaced).Data(),iLay);
		c2stubLocalCoordByModuleDupl[iLay] = new TCanvas(cc,cc,50,0,1700,900);
		unsigned nx, ny;
		findFactors(nModules[iLay], nx,ny);
//		cout << iLay << "\t" << nModules[iLay] << "\t" << nx << ":" << ny << endl;
		c2stubLocalCoordByModuleDupl[iLay]->Divide(nx,ny);
		for (unsigned iMod=0; iMod<nModules[iLay]; ++iMod) {
			c2stubLocalCoordByModuleDupl[iLay]->cd(iMod+1);
			h2stubLocalCoordDupl[ih2stubLocalCoordDupl]->Write();
			h2stubLocalCoordDupl[ih2stubLocalCoordDupl++]->DrawCopy("colz0");
		}
		if (makePlots) {
			sprintf(cc,"stubOverlapLocalCoords%s_Layer%d",TString(sBeamDisplaced).Data(),iLay);
			c2stubLocalCoordByModuleDupl[iLay]->SaveAs(outDir+TString(cc)+TString(sBeamDisplaced)+TString(cMinPt)+TString(".C"));
			c2stubLocalCoordByModuleDupl[iLay]->SaveAs(outDir+TString(cc)+TString(sBeamDisplaced)+TString(cMinPt)+TString(".pdf"));
			c2stubLocalCoordByModuleDupl[iLay]->SaveAs(outDir+TString(cc)+TString(sBeamDisplaced)+TString(cMinPt)+TString(".png"));
		}
	}

	unsigned ih2stubLocalCoordDuplEff=0;
	TCanvas* c2stubLocalCoordByModuleDuplEff[6];
	for (unsigned iLay=0; iLay<6; ++iLay) {
		char cc[100];
		sprintf(cc,"c2stubLocalCoordByModuleDuplEff%s_Layer%d",TString(sBeamDisplaced).Data(),iLay);
		c2stubLocalCoordByModuleDuplEff[iLay] = new TCanvas(cc,cc,50,0,1700,900);
		unsigned nx, ny;
		findFactors(nModules[iLay], nx,ny);
//		cout << iLay << "\t" << nModules[iLay] << "\t" << nx << ":" << ny << endl;
		c2stubLocalCoordByModuleDuplEff[iLay]->Divide(nx,ny);
		for (unsigned iMod=0; iMod<nModules[iLay]; ++iMod) {
			c2stubLocalCoordByModuleDuplEff[iLay]->cd(iMod+1);
			TString hName(h2stubLocalCoordDupl[ih2stubLocalCoordDuplEff]->GetName());
			hName.ReplaceAll(TString("Dupl_"),TString("Eff_"));
			h2stubLocalCoordEff[ih2stubLocalCoordDuplEff] = (TH2F*) h2stubLocalCoordDupl[ih2stubLocalCoordDuplEff]->Clone(hName);
			h2stubLocalCoordEff[ih2stubLocalCoordDuplEff]->SetTitle(hName);
			h2stubLocalCoordEff[ih2stubLocalCoordDuplEff]->Divide(h2stubLocalCoordAll[ih2stubLocalCoordDuplEff]);
			h2stubLocalCoordEff[ih2stubLocalCoordDuplEff]->GetZaxis()->SetRangeUser(0,1);
			h2stubLocalCoordEff[ih2stubLocalCoordDuplEff]->Write();
			h2stubLocalCoordEff[ih2stubLocalCoordDuplEff++]->DrawCopy("colz0");
		}
		if (makePlots) {
			sprintf(cc,"stubOverlapLocalCoordsEff_Layer%d",iLay);
			c2stubLocalCoordByModuleDuplEff[iLay]->SaveAs(outDir+TString(cc)+TString(sBeamDisplaced)+TString(cMinPt)+TString(".root"));
			c2stubLocalCoordByModuleDuplEff[iLay]->SaveAs(outDir+TString(cc)+TString(sBeamDisplaced)+TString(cMinPt)+TString(".C"));
			c2stubLocalCoordByModuleDuplEff[iLay]->SaveAs(outDir+TString(cc)+TString(sBeamDisplaced)+TString(cMinPt)+TString(".pdf"));
			c2stubLocalCoordByModuleDuplEff[iLay]->SaveAs(outDir+TString(cc)+TString(sBeamDisplaced)+TString(cMinPt)+TString(".png"));
		}
	}
	fOutHistos->Close();

	//*********************************************

	bool makeHistoRadii = false;
	if (makeHistoRadii) {
	vector <double> vRadii;
	vRadii.push_back(0);
	bool hasEntries = false;
	for (int iBin=1; iBin<h1StubRadius->GetXaxis()->GetNbins()+1; ++iBin) {
		if ((hasEntries && !h1StubRadius->GetBinContent(iBin)) || (!hasEntries && h1StubRadius->GetBinContent(iBin))) {
			std::cout << h1StubRadius->GetBinLowEdge(iBin) << ",";
			vRadii.push_back(h1StubRadius->GetBinLowEdge(iBin));
			hasEntries = not(hasEntries);
		}
	}
	vRadii.push_back(120);
	TFile* fRadiiw = 0; isfOpen = false;
	fRadiiw = new TFile("barrelRadii.root","RECREATE");
	isfOpen = fRadiiw->IsOpen();
	if (!isfOpen) {
		cout << "ERROR. Not able to load the radii file. Exiting..." << endl;
		return;
	}

	TH1* hRadiiw = new TH1S("hRadii","hRadii",vRadii.size()-1,&vRadii[0]);
	unsigned iCount = 0;
	for (unsigned iBin=0; iBin<vRadii.size()-1; ++iBin) {
		if (iBin%2==0) {
			hRadiiw->SetBinContent(iBin+1,0);
		}
		else {
			unsigned iSubLayer=1+iCount%4;
			unsigned iLayer=iCount/4;
			hRadiiw->SetBinContent(iBin+1,iLayer*10+iSubLayer);
			++iCount;
		}
	}
	hRadiiw->Write();
	fRadiiw->Close();
	}
	return;
}

void overlapRemovalRun(Long64_t nMax=1000000,bool makePlots=false, float minPt=3, unsigned iBeamDisplaced=0) {
	gStyle->SetOptStat(0);
	TString fName;
	TString sBeamDisplaced("");
	switch (iBeamDisplaced) {
		case 0:
			fName="/data/rossin/EOS/singleMuNoTest2_2000/SingleMuonFlatOneOverPt0p0005To0p5_tt27NoTest_cfi_py_GEN_SIM_DIGI_L1TrackTrigger_2Msum.root";
			fName="/data/rossin/EOS/SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3/SingleMuonTest_tt27_PU0_20150408/SingleMuonTest_tt27_PU0_ntuple_Pt0p2_6p5M.root";
			fName="/data/rossin/EOS/SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3/SingleMuonTest_tt27_PU0_20150408/SingleMuonTest_tt27_PU0_ntuple_Pt3_6M.root";
			sBeamDisplaced="";
			break;
		case 1:
			fName="/data/rossin/EOS/SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3/SingleMuonTest_tt27_PU0_20150408/SingleMuonTest_tt27_XY1mmAway_PU0_ntuple_Pt3.root";
			sBeamDisplaced="XY1mmAway_";
			break;
		case 2:
			fName="/data/rossin/EOS/SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3/SingleMuonTest_tt27_PU0_20150408/SingleMuonTest_tt27_XY1mmTowards_PU0_ntuple_Pt3.root";
			sBeamDisplaced="XY1mmTowards_";
			break;
		case 3:
			fName="/data/rossin/EOS/SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3/SingleMuonTest_tt27_PU0_20150408/SingleMuonTest_tt27_XY1mmOrthogonal_PU0_ntuple_Pt3.root";
			sBeamDisplaced="XY1mmOrthogonal_";
			break;
	}
	overlapRemoval s(fName);
	s.Loop(nMax,makePlots,minPt,sBeamDisplaced);
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


