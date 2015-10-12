/*
 * overlapRemovalAreaFinder.C
 *
 *  Created on: Aug 10, 2015
 *      Author: rossin
 */

#include <TFile.h>
#include <TTree.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TPolyLine.h>
#include <TList.h>
#include <TString.h>
#include <TMath.h>
#include <TRandom3.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TStyle.h>
#include <iostream>
#include <iomanip> // for setw
#include <vector>

typedef struct{
	unsigned id   ;
	unsigned layer;
	unsigned x1   ;
	unsigned y1   ;
	unsigned x2   ;
	unsigned y2   ;

} module;

void overlapRemovalAreaFinder(unsigned iProc = 0, bool saveCanvas = 0) {

	gStyle->SetOptStat(0);

//	float cutOffEffs[6] = {0.8, 0.8, 0.6, 0.8, 0.6, 0.5};
	float cutOffEffs[6] = {0.2, 0.2, 0.2, 0.2, 0.2, 0.2};
	TString scutOffEffs[6];
	std::cout << "CutOff efficiencies." << std::endl;
	for (unsigned layer=0; layer<6; ++layer) {
		char cc[10];
		sprintf(cc,"%2.1f",cutOffEffs[layer]);
		scutOffEffs[layer] = TString(cc).ReplaceAll(".","p");
		std::cout << cutOffEffs[layer] << "\t";
	}
	std::cout << std::endl;

	TString cProc("_");
	switch (iProc) {
	case 0:
		cProc="";
		break;
	case 1:
		cProc="XY1mmAway_";
		break;
	case 2:
		cProc="XY1mmTowards_";
		break;
	case 3:
		cProc="XY1mmOrthogonal_";
		break;
	}
	TString cFile(TString("/home/rossin/Dropbox/TT/Work/figures_stubOverlapRemoval/stubOverlapHistos"+cProc));
//	TString cFile(TString("./stubOverlapHistos"+cProc));
	//	TFile fIn("/home/rossin/Dropbox/TT/CMSSW_6_2_0_SLHC25_patch3/src/SLHCL1TrackTriggerSimulations/AMSimulation/res1/h2stubLocalCoordEff_ModuleId50336.root");
	std::cout << "Opening file " << cFile+TString(".root") << std::endl;
	TFile fIn(cFile+TString(".root"));
	if (saveCanvas) cFile = cFile+TString("Frames");
	else            cFile = cFile+TString("FramesOnlyTree");
	TH1F* hAreaRatios[6];
	for (unsigned layer=0; layer<6; ++layer) {
		cFile = cFile+TString("_") + scutOffEffs[layer];
		char cc[50];
		sprintf(cc,"hAreaRatios_Layer_%d",layer);
//		hAreaRatios[layer] = new TH1F(cc,cc,51,0.495,1.005);
		hAreaRatios[layer] = new TH1F(cc,cc,500,0.50,1.00001);
	}
	TFile fOut(cFile+TString(".root"),"recreate");
//	TFile fIn("/home/rossin/Dropbox/TT/Work/figures_stubOverlapRemoval/stubOverlapHistosXY1mmOrthogonal_.root");
//	TFile fOut("/home/rossin/Dropbox/TT/Work/figures_stubOverlapRemoval/stubOverlapHistosXY1mmOrthogonal_Frames.root","recreate");
	//	fIn.ls();

	module modRegion;
  TTree tmod("tmodule","Tree with overlapping regions to keep, per module");
	tmod.Branch("id"    ,&modRegion.id    ,"id/I"   );
	tmod.Branch("layer" ,&modRegion.layer ,"layer/I");
	tmod.Branch("x1"    ,&modRegion.x1    ,"x1/I"   );
	tmod.Branch("y1"    ,&modRegion.y1    ,"y1/I"   );
	tmod.Branch("x2"    ,&modRegion.x2    ,"x2/I"   );
	tmod.Branch("y2"    ,&modRegion.y2    ,"y2/I"   );

	std::vector <module> vmodules;


	TList* tl = fIn.GetListOfKeys();
	TIter next(tl);
	TObject *obj;
	TString stringID("h2stubLocalCoordEff_ModuleId");
	while ((obj=next())) {
		TString hName(obj->GetName());
		if (hName.Contains(stringID)) {

			TH2F* h = (TH2F*) fIn.Get(hName);
			if (!h) {
				std::cout << "Could not load histo " << hName << std::endl;
				return;
			}

			module m;
			unsigned moduleID = hName.Remove(0,stringID.Sizeof()-1).Atoi();
			unsigned layer    = TMath::Floor(moduleID/10000.)-5;
			m.id    = moduleID;
			m.layer = layer;

			bool is2S = false;
			if ( layer>2) {
				is2S = true;
//				continue;
			}
//			if (moduleID!=80711) continue;
			std::cout << "Processing: " << hName << "\t";


			//	TH2F* h = (TH2F*) fIn.Get("h2stubLocalCoordEff_ModuleId50336");
			//	TCanvas* c = new TCanvas("c","c",0,0,900,900);
			//	h->DrawCopy("colz0");

			unsigned nx = h->GetNbinsX();
			unsigned ny = h->GetNbinsY();
			float    xBinWidth = h->GetXaxis()->GetBinWidth(1);
			float    yBinWidth = h->GetYaxis()->GetBinWidth(1);

			unsigned P[2][2]={{nx/3  ,  ny/3},
					//										{2*nx/3,  ny/3},
					{2*nx/3,2*ny/3}
			//										{  nx/3,2*ny/3}
			};
			if (is2S) {
				P[0][1] = 1;
				P[1][1] = 2;
			}
			if (layer <2) {
				bool isOverlappingEta    =  (moduleID/100)%2;
				bool isOverlappingPhi    = !(moduleID%2);
				if (!isOverlappingPhi) { P[0][1] = 1; P[1][1] = ny;}
				if (!isOverlappingEta) { P[0][0] = 1; P[1][0] = nx;}
			}
			if (layer ==2) {
				bool isOverlappingEta    = (moduleID/100)%2;
				bool isOverlappingPhi    = (moduleID%2);
				if (!isOverlappingPhi) { P[0][1] = 1; P[1][1] = ny;}
				if (!isOverlappingEta) { P[0][0] = 1; P[1][0] = nx;}
			}

			unsigned nSkipRegion = 0;
			unsigned nVertex=0;
			while (nSkipRegion<5 && nVertex!=3000) {
				float integral = 0;
				unsigned nBins = 0;
				for (unsigned ix=P[0][0]; ix<P[1][0]; ++ix) {
					for (unsigned iy=P[0][1]; iy<P[1][1]; ++iy) {
						integral+=h->GetBinContent(ix,iy);
						++nBins;
					}
				}

				float stripIntegral = 0;
				unsigned nStripBin = 0;
				int deltaP[2][2]={{0,0},{0,0}};
				if (nVertex%4==0 && P[0][1]>1 && !is2S) {
					//			--P[0][1];
					deltaP[0][0]= 0;
					deltaP[0][1]=-1;
					deltaP[1][0]= 0;
					deltaP[1][1]= 0;
					for (unsigned ix=P[0][0]; ix<P[1][0]; ++ix) {
						stripIntegral+=h->GetBinContent(ix,P[0][1]+deltaP[0][1]);
						++nStripBin;
//						if (nVertex>2992) std::cout << ix << "\t" << P[0][1]+ deltaP[0][1] << "\t" << h->GetBinContent(ix,P[0][1]+deltaP[0][1])<< std::endl;

						}
				}
				if (nVertex%4==1 && (nx-P[1][0])) {
					//			++P[1][0];
					deltaP[0][0]= 0;
					deltaP[0][1]= 0;
					deltaP[1][0]= 1;
					deltaP[1][1]= 0;
					for (unsigned iy=P[0][1]; iy<P[1][1]; ++iy) {
						stripIntegral+=h->GetBinContent(P[1][0]+deltaP[1][0],iy);
						++nStripBin;
					}
				}
				if (nVertex%4==2 && (ny-P[1][1]) && !is2S) {
					//			++P[1][1];
					deltaP[0][0]= 0;
					deltaP[0][1]= 0;
					deltaP[1][0]= 0;
					deltaP[1][1]= 1;
					for (unsigned ix=P[0][0]; ix<P[1][0]; ++ix) {
						stripIntegral+=h->GetBinContent(ix,P[1][1]+deltaP[1][1]);
						++nStripBin;
					}
				}
				if (nVertex%4==3 && P[0][0]>1) {
					//			--P[0][0];
					deltaP[0][0]=-1;
					deltaP[0][1]= 0;
					deltaP[1][0]= 0;
					deltaP[1][1]= 0;
					for (unsigned iy=P[0][1]; iy<P[1][1]; ++iy) {
						stripIntegral+=h->GetBinContent(P[0][0]+deltaP[0][0],iy);
						++nStripBin;
					}
				}
				if (nStripBin==0 || stripIntegral/nStripBin<cutOffEffs[layer]*integral/nBins) ++nSkipRegion;
				else {
					P[0][0]+=deltaP[0][0];
					P[0][1]+=deltaP[0][1];
					P[1][0]+=deltaP[1][0];
					P[1][1]+=deltaP[1][1];
					nSkipRegion=0;
				}

//				if (nVertex>2992) {
//					std::cout << "(" << xBinWidth*P[0][0] << ";" << yBinWidth*P[0][1] << ")\t"
//							<< "(" << xBinWidth*P[1][0] << ";" << yBinWidth*P[1][1] << ")"
//							<< "\t" << nSkipRegion
//							<< std::endl;
//					std::cout << integral/nBins << "\t";
//					std::cout << nVertex%4 << "\t" << stripIntegral/nStripBin << std::endl ;
//				}

				++nVertex;
			}

			TCanvas c(stringID+hName,stringID+hName,0,0,900,900);
			h->DrawCopy("colz0");
			--P[0][0]; // needed to pick lower edge
			--P[0][1]; // needed to pick lower edge
			float x[5]={xBinWidth*P[0][0],xBinWidth*P[1][0],xBinWidth*P[1][0],xBinWidth*P[0][0],xBinWidth*P[0][0]};
			float y[5]={yBinWidth*P[0][1],yBinWidth*P[0][1],yBinWidth*P[1][1],yBinWidth*P[1][1],yBinWidth*P[0][1]};
			TPolyLine *pline = new TPolyLine(5,x,y);
			pline->SetLineColor(12);
			pline->SetLineWidth(5);
			pline->Draw();
			if (saveCanvas) c.Write();
			std::cout << std::setw(6) << x[0]
								<< std::setw(6) << y[0]
								<< std::setw(6) << x[2]
								<< std::setw(6) << y[2]
																				<< std::endl;

			m.id    = moduleID;
			m.layer = layer;
			modRegion.id    = moduleID;
			modRegion.layer = layer   ;
			modRegion.x1    = xBinWidth*P[0][0];
			modRegion.y1    = yBinWidth*P[0][1];
			modRegion.x2    = xBinWidth*P[1][0];
			modRegion.y2    = yBinWidth*P[1][1];
			tmod.Fill();

			if (saveCanvas) {
				float ratio = (modRegion.x2-modRegion.x1)*(modRegion.y2-modRegion.y1);
				ratio /= (h->GetXaxis()->GetXmax())*(h->GetYaxis()->GetXmax());
				hAreaRatios[layer]->Fill(ratio);
			}
		}
	}

	TLegend* tleg;
	TH1F* hMultiplicity;
	char cc3[50];
	if (saveCanvas) {
		TCanvas *cAreaRatios = new TCanvas("cAreaRatios","cAreaRatios",0,0,1000,700);
		cAreaRatios->Divide(3,2);
		for (unsigned layer=0; layer<6; ++layer) {
			cAreaRatios->cd(layer+1);
			hAreaRatios[layer]->DrawCopy();
			tleg = new TLegend(0.1,0.7,0.8,0.9);
			sprintf(cc3,"Unmasked area ratio: %4.3lf",hAreaRatios[layer]->GetMean());
			tleg->AddEntry(hAreaRatios[layer],cc3);
			tleg->Draw("APL");
			hAreaRatios[layer]->Write();
		}
		hMultiplicity = new TH1F("hMultiplicity","hMultiplicity",33,0,33);
		TRandom3 r(222);
		for (unsigned iPE=0; iPE<1000000; ++iPE) {
			unsigned nComb = 1;
			for (unsigned layer=0; layer<6; ++layer) {
				float fraction = hAreaRatios[layer]->GetRandom();
				float rand = r.Uniform(1);
//				if (rand>fraction) std::cout << layer << "\t" << fraction  << "\t" << rand << "\t" << (rand>fraction) << std::endl;
				nComb *= (rand>fraction ? 2 : 1);
			}
			hMultiplicity->Fill(nComb);
		}
		TCanvas *cMultiplicity = new TCanvas("cMultiplicity","cMultiplicity",0,0,1000,700);
		cMultiplicity->cd();
		hMultiplicity->DrawCopy();
		tleg = new TLegend(0.3,0.75,0.9,0.9);
		sprintf(cc3,"average # combinations: %4.3lf",hMultiplicity->GetMean());
		tleg->AddEntry(hMultiplicity,cc3);
		tleg->Draw("APL");
		cMultiplicity->SetLogy();
		cMultiplicity->SaveAs(cFile+TString("_PEmultiplicity.png"));
		hMultiplicity->Write();
		cAreaRatios->SaveAs(cFile+TString("_AreaRatios.png"));
		cAreaRatios->Write();
	}

	tmod.Write();
	fOut.Close();
	fIn.Close();
	return;
}
