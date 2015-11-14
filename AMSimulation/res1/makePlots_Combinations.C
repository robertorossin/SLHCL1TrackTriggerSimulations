/*
 * makePlots_Combinations.C
 *
 *  Created on: Nov 8, 2015
 *      Author: rossin
 */

#include <TH2.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TMath.h>
#include <TF1.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TMultiGraph.h>
#include <TArrow.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TText.h>
#include <TLatex.h>
#include <TEllipse.h>
#include <TVector3.h>
#include <TFile.h>
#include <TString.h>
#include <TList.h>
#include <TObject.h>

#include <stdio.h>
#include <iostream>
#include <map>
#include <iomanip>
#include <fstream>

void makePlots_Combinations(short makePlots=0) {

	TString dirPlots("/home/rossin/Dropbox/TT/Work/figures_stubCleaning/");

	TFile fIn ("/home/rossin/Dropbox/TT/Work/figures_stubCleaning/stubCleaning_Neutrino_PU140_sf1_nz4_pt3_5oo6_14k.root"); TString sLogic ("5oo6"); TString pName("Neutrino_PU140_sf1_nz4_pt3_5oo6_14k"); TString pTitle(" PU140 SF=1 Nz=4 Pt>3 GeV/c");
	TFile fIn2("/home/rossin/Dropbox/TT/Work/figures_stubCleaning/stubCleaning_Neutrino_PU140_sf1_nz4_pt3_6oo6_14k.root"); TString sLogic2("6oo6"); //TString pName("Neutrino_PU140_sf1_nz4_pt3_6oo6_14k"); TString pTitle(" PU140 SF=1 Nz=4 Pt>3 GeV/c");
	TFile fInOR("/home/rossin/Dropbox/TT/Work/figures_stubCleaning/stubCleaning_Neutrino_PU140_sf1_nz4_pt3_5oo6_14k_OverlapRemoved.root");

//	TFile fIn("/home/rossin/Dropbox/TT/Work/figures_stubCleaning/stubCleaning_Neutrino_PU200_sf1_nz4_pt3_5oo6_4k.root"); TString sLogic("5oo6"); TString pName("Neutrino_PU200_sf1_nz4_pt3_5oo6_4k"); TString pTitle(" PU200 SF=1 Nz=4 Pt>3 GeV/c");
//	TFile fInOR("/home/rossin/Dropbox/TT/Work/figures_stubCleaning/stubCleaning_Neutrino_PU200_sf1_nz4_pt3_5oo6_4k_OverlapRemoved.root");

//	TFile fIn("/home/rossin/Dropbox/TT/Work/figures_stubCleaning/stubCleaning_Neutrino_PU250_sf1_nz4_pt3_5oo6_4k.root"); TString sLogic("5oo6"); TString pName("Neutrino_PU250_sf1_nz4_pt3_5oo6_4k"); TString pTitle(" PU250 SF=1 Nz=4 Pt>3 GeV/c");
//	TFile fInOR("/home/rossin/Dropbox/TT/Work/figures_stubCleaning/stubCleaning_Neutrino_PU250_sf1_nz4_pt3_5oo6_4k_OverlapRemoved.root");


	TH1* h1stubsInLayer  [6][2];
	TH1* h1stubsInLayerOR[6][2];
	TH1* h1RoadPerEvent  [2];
	TH1* h1RoadPerEventOR[2];
	TH1* h1CombPerRoad   [2];
	TH1* h1CombPerRoadOR [2];
	TH1* h1CombPerEvent  [2];
	TH1* h1CombPerEventOR[2];

	TH1* h1RoadPerEvent6oo6  [2];
	TH1* h1CombPerRoad6oo6   [2];
	TH1* h1CombPerEvent6oo6  [2];

    TString sClean[2] = {TString(""), TString("_Cleaned")};

    for (unsigned short iLay=0; iLay<6; ++iLay) {
	    TList* tl = fIn.GetListOfKeys();
	    TIter next(tl);
	    TObject *obj;
	    char cc[20];
	    sprintf(cc,"_%d",iLay);
	    TString hString("h1stubsInLayer__"+sLogic+TString(cc));
	    while ((obj=next())) {
	    	TString hName(obj->GetName());
	    	if (hName==hString) {
	    		for (unsigned short iClean = 0; iClean < 2; ++iClean) {
	    		    char cc2[100];
	    			h1stubsInLayer  [iLay][iClean] = (TH1*) fIn  .Get(hName+sClean[iClean]);
	    			sprintf(cc2,"L%d - ",iLay+5);
	    			h1stubsInLayer  [iLay][iClean]->SetTitle(TString(cc2)+sLogic+pTitle);
	    			h1stubsInLayerOR[iLay][iClean] = (TH1*) fInOR.Get(hName+sClean[iClean]);
	    			if (h1stubsInLayer  [iLay][iClean]==0) {
		    			std::cout << "ERROR. " << hName+sClean[iClean] << " not loaded." << std::endl;
		    			return;
	    			}
	    			if (h1stubsInLayerOR[iLay][iClean]==0) {
		    			std::cout << "ERROR. " << hName+sClean[iClean] << " overlap removed not loaded." << std::endl;
		    			return;
	    			}
	    		}
	    	}
	    }
//	    std::cout << h1stubsInLayer  [iLay][0] << "\t" << h1stubsInLayer  [iLay][1] << "\t" << h1stubsInLayerOR[iLay][0] << "\t" << h1stubsInLayerOR[iLay][1] << std::endl;
	}

    for (unsigned short iClean = 0; iClean < 2; ++iClean) {
    	TString hString("h1RoadPerEvent_"+sLogic);
    	h1RoadPerEvent  [iClean] = (TH1*) fIn  .Get(hString+sClean[iClean]);
    	h1RoadPerEventOR[iClean] = (TH1*) fInOR.Get(hString+sClean[iClean]);
    	hString=TString("h1CombPerRoad_")+sLogic;
    	h1CombPerRoad   [iClean] = (TH1*) fIn  .Get(hString+sClean[iClean]);
    	h1CombPerRoadOR [iClean] = (TH1*) fInOR.Get(hString+sClean[iClean]);
    	hString=TString("h1CombPerEvent_")+sLogic;
    	h1CombPerEvent   [iClean] = (TH1*) fIn  .Get(hString+sClean[iClean]);
    	h1CombPerEventOR [iClean] = (TH1*) fInOR.Get(hString+sClean[iClean]);

    	hString=TString("h1RoadPerEvent_"+sLogic2);
    	h1RoadPerEvent6oo6 [iClean] = (TH1*) fIn2  .Get(hString+sClean[iClean]);
    	hString=TString("h1CombPerRoad_")+sLogic2;
    	h1CombPerRoad6oo6  [iClean] = (TH1*) fIn2  .Get(hString+sClean[iClean]);
    	hString=TString("h1CombPerEvent_")+sLogic2;
    	h1CombPerEvent6oo6 [iClean] = (TH1*) fIn2  .Get(hString+sClean[iClean]);
}

    TString cName("cStubsPerLayer_"+sLogic+pName);
    TString cTitle("StubsPerLayer "+sLogic+pTitle);
    TCanvas* cStubsPerLayer = new TCanvas(cName,cTitle,0,0,1400,900);
    cStubsPerLayer->Divide(3,2);
    for (unsigned short iLay=0; iLay<6; ++iLay) {
    	char cc3[100];
    	cStubsPerLayer->cd(iLay+1);
    	gPad->SetLogy();
    	h1stubsInLayer  [iLay][0]->DrawCopy();
    	TLegend* tl = new TLegend(0.3,0.75,0.9,0.9);
		sprintf(cc3,"Mean # stubs: %3.2lf",h1stubsInLayer  [iLay][0]->GetMean());
		TLegendEntry* tle = tl->AddEntry(h1stubsInLayer  [iLay][0],cc3,"l");
		tle->SetLineColor(h1stubsInLayer  [iLay][0]->GetLineColor());
		tle->SetLineWidth(h1stubsInLayer  [iLay][0]->GetLineWidth());
		tl->Draw("L");
    }
    if (makePlots) {
    	cStubsPerLayer->SaveAs(dirPlots+cName+"__.pdf");
    	cStubsPerLayer->SaveAs(dirPlots+cName+"__.png");
    }
    for (unsigned short iLay=0; iLay<6; ++iLay) {
    	char cc3[100];
    	cStubsPerLayer->cd(iLay+1);
    	gPad->SetLogy();
    	h1stubsInLayerOR[iLay][0]->SetLineColor(2);
    	h1stubsInLayerOR[iLay][0]->DrawCopy("same");
    	TLegend* tl = new TLegend(0.25,0.75,0.9,0.9);
		sprintf(cc3,"Mean # stubs: %3.2lf",h1stubsInLayer  [iLay][0]->GetMean());
		TLegendEntry* tle = tl->AddEntry(h1stubsInLayer  [iLay][0],cc3,"l");
		tle->SetLineColor(h1stubsInLayer  [iLay][0]->GetLineColor());
		tle->SetLineWidth(h1stubsInLayer  [iLay][0]->GetLineWidth());
		sprintf(cc3,"Mean # stubs Overlap Removed: %3.2lf",h1stubsInLayerOR[iLay][0]->GetMean());
		tle = tl->AddEntry(h1stubsInLayerOR[iLay][0],cc3,"l");
		tle->SetLineColor(h1stubsInLayerOR[iLay][0]->GetLineColor());
		tle->SetLineWidth(h1stubsInLayerOR[iLay][0]->GetLineWidth());
		tl->Draw("L");
    }
    if (makePlots) {
    	cStubsPerLayer->SaveAs(dirPlots+cName+"_OR_.pdf");
    	cStubsPerLayer->SaveAs(dirPlots+cName+"_OR_.png");
    }

    cName=TString("cStubsPerLayerDsClean_")+sLogic+pName;
    cTitle=TString("StubsPerLayer Ds clean ")+sLogic+pTitle;
    TCanvas* cStubsPerLayerDsClean = new TCanvas(cName,cTitle,0,0,1400,900);
    cStubsPerLayerDsClean->Divide(3,2);
    for (unsigned short iLay=0; iLay<6; ++iLay) {
    	char cc3[100];
    	cStubsPerLayerDsClean->cd(iLay+1);
    	gPad->SetLogy();
    	h1stubsInLayer  [iLay][0]->DrawCopy();
    	h1stubsInLayer  [iLay][1]->SetLineColor(8);
    	h1stubsInLayer  [iLay][1]->DrawCopy("same");
    	TLegend* tl = new TLegend(0.25,0.75,0.9,0.9);
		sprintf(cc3,"Mean # stubs: %3.2lf",h1stubsInLayer  [iLay][0]->GetMean());
		TLegendEntry* tle = tl->AddEntry(h1stubsInLayer  [iLay][0],cc3,"l");
		tle->SetLineColor(h1stubsInLayer  [iLay][0]->GetLineColor());
		tle->SetLineWidth(h1stubsInLayer  [iLay][0]->GetLineWidth());
		sprintf(cc3,"Mean # stubs #Deltas clean: %3.2lf",h1stubsInLayer  [iLay][1]->GetMean());
		tle = tl->AddEntry(h1stubsInLayer  [iLay][1],cc3,"l");
		tle->SetLineColor(h1stubsInLayer  [iLay][1]->GetLineColor());
		tle->SetLineWidth(h1stubsInLayer  [iLay][1]->GetLineWidth());
		tl->Draw("L");
    }
    if (makePlots) {
    	cStubsPerLayerDsClean->SaveAs(dirPlots+cName+"__.pdf");
    	cStubsPerLayerDsClean->SaveAs(dirPlots+cName+"__.png");
    }

    cName=TString("cStubsPerLayerDsCleanOR_")+sLogic+pName;
    cTitle=TString("StubsPerLayer #Deltas clean Overlap Removed ")+sLogic+pTitle;
    TCanvas* cStubsPerLayerDsCleanOR = new TCanvas(cName,cTitle,0,0,1400,900);
    cStubsPerLayerDsCleanOR->Divide(3,2);
    for (unsigned short iLay=0; iLay<6; ++iLay) {
    	char cc3[100];
    	cStubsPerLayerDsCleanOR->cd(iLay+1);
    	gPad->SetLogy();
    	h1stubsInLayer  [iLay][0]->DrawCopy();
    	h1stubsInLayerOR[iLay][0]->SetLineColor(2);
    	h1stubsInLayerOR[iLay][0]->DrawCopy("same");
    	h1stubsInLayerOR[iLay][1]->SetLineColor(6);
    	h1stubsInLayerOR[iLay][1]->DrawCopy("same");
    	TLegend* tl = new TLegend(0.25,0.70,0.9,0.9);
		sprintf(cc3,"Mean # stubs: %3.2lf",h1stubsInLayer  [iLay][0]->GetMean());
		TLegendEntry* tle = tl->AddEntry(h1stubsInLayer  [iLay][0],cc3,"l");
		tle->SetLineColor(h1stubsInLayer  [iLay][0]->GetLineColor());
		tle->SetLineWidth(h1stubsInLayer  [iLay][0]->GetLineWidth());
		sprintf(cc3,"Mean # stubs Ov. Rem.: %3.2lf",h1stubsInLayerOR[iLay][0]->GetMean());
		tle = tl->AddEntry(h1stubsInLayerOR[iLay][0],cc3,"l");
		tle->SetLineColor (h1stubsInLayerOR[iLay][0]->GetLineColor());
		tle->SetLineWidth (h1stubsInLayerOR[iLay][0]->GetLineWidth());
		sprintf(cc3,"Mean # stubs Ds clean Ov. Rem.: %3.2lf",h1stubsInLayerOR[iLay][1]->GetMean());
		tle = tl->AddEntry(h1stubsInLayerOR[iLay][1],cc3,"l");
		tle->SetLineColor (h1stubsInLayerOR[iLay][1]->GetLineColor());
		tle->SetLineWidth (h1stubsInLayerOR[iLay][1]->GetLineWidth());
		tl->Draw("L");
    }
    if (makePlots) {
    	cStubsPerLayerDsCleanOR->SaveAs(dirPlots+cName+"__.pdf");
    	cStubsPerLayerDsCleanOR->SaveAs(dirPlots+cName+"__.png");
    }

	const short nQuant = 4;
	double  percentiles   [nQuant] = {0.90, 0.95, 0.99, 0.999};
	double xpercentiles[6][nQuant];

    cName=TString("cRoadPerEvent_")+sLogic+pName;
    cTitle=TString("Roads per Tower "+sLogic+pTitle);
    TCanvas* cRoadPerEvent = new TCanvas(cName,cTitle,0,0,1400,900);
    cRoadPerEvent->SetLogy();
    h1RoadPerEvent  [0]->SetTitle(cTitle);
    h1RoadPerEvent  [0]->DrawCopy();
//    h1RoadPerEvent6oo6[0]->SetLineColor(14);
//    h1RoadPerEvent6oo6[0]->DrawCopy("same");
	char cc3[100];
	h1RoadPerEvent  [0]->GetQuantiles(nQuant,xpercentiles[0],percentiles);
	sprintf(cc3,"                 #mu=%3.1lf, #Lambda_{%3.1lf}=%3.1lf",h1RoadPerEvent  [0]->GetMean(),percentiles[3]*100,xpercentiles[0][3]);
	TLegend* tl = new TLegend(0.25,0.84,0.9,0.9);
	TLegendEntry* tle = tl->AddEntry(h1RoadPerEvent  [0],cc3,"l");
	tle->SetLineColor(h1RoadPerEvent  [0]->GetLineColor());
	tle->SetLineWidth(h1RoadPerEvent  [0]->GetLineWidth());
//	h1RoadPerEvent6oo6[0]->GetQuantiles(nQuant,xpercentiles[0],percentiles);
//	sprintf(cc3,"# roads/tower-6/6. #mu=%3.1lf, #Lambda_{%3.1lf}=%3.1lf",h1RoadPerEvent6oo6[0]->GetMean(),percentiles[3]*100,xpercentiles[0][3]);
//	tle = tl->AddEntry(h1RoadPerEvent6oo6[0],cc3,"l");
//	tle->SetLineColor(h1RoadPerEvent6oo6[0]->GetLineColor());
//	tle->SetLineWidth(h1RoadPerEvent6oo6[0]->GetLineWidth());
	tl->Draw("L");
	if (makePlots) {
		cRoadPerEvent->SaveAs(dirPlots+cName+"__.pdf");
		cRoadPerEvent->SaveAs(dirPlots+cName+"__.png");
	}

    cName=TString("cRoadPerEvent_OR_")+sLogic+pName;
    cTitle=TString("Roads per Tower "+sLogic+pTitle);
    TCanvas* cRoadPerEventOR = new TCanvas(cName,cTitle,0,0,1400,900);
    cRoadPerEventOR->SetLogy();
    h1RoadPerEvent  [0]->SetTitle(cTitle);
    h1RoadPerEvent  [0]->DrawCopy();
	h1RoadPerEvent  [0]->GetQuantiles(nQuant,xpercentiles[0],percentiles);
	sprintf(cc3,"                 #mu=%3.1lf, #Lambda_{%3.1lf}=%3.1lf",h1RoadPerEvent  [0]->GetMean(),percentiles[3]*100,xpercentiles[0][3]);
	tl = new TLegend(0.25,0.8,0.9,0.9);
	tle = tl->AddEntry(h1RoadPerEvent  [0],cc3,"l");
	tle->SetLineColor(h1RoadPerEvent  [0]->GetLineColor());
	tle->SetLineWidth(h1RoadPerEvent  [0]->GetLineWidth());
    h1RoadPerEventOR[0]->SetLineColor(2);
    h1RoadPerEventOR[0]->DrawCopy("same");
	h1RoadPerEventOR[0]->GetQuantiles(nQuant,xpercentiles[0],percentiles);
	sprintf(cc3,"Ov. Masked #mu=%3.1lf, #Lambda_{%3.1lf}=%3.1lf",h1RoadPerEventOR[0]->GetMean(),percentiles[3]*100,xpercentiles[0][3]);
	tle = tl->AddEntry(h1RoadPerEventOR[0],cc3,"l");
	tle->SetLineColor(h1RoadPerEventOR[0]->GetLineColor());
	tle->SetLineWidth(h1RoadPerEventOR[0]->GetLineWidth());
	tl->Draw("L");
	if (makePlots) {
		cRoadPerEventOR->SaveAs(dirPlots+cName+"__.pdf");
		cRoadPerEventOR->SaveAs(dirPlots+cName+"__.png");
	}

    cName=TString("cCombPerRoad_")+sLogic+pName;
    cTitle=TString("Combinations per Road "+sLogic+pTitle);
    TCanvas* cCombPerRoad = new TCanvas(cName,cTitle,0,0,1400,900);
    cCombPerRoad->SetLogy();
    h1CombPerRoad  [0]->SetTitle(cTitle);
    h1CombPerRoad  [0]->DrawCopy();
	h1CombPerRoad  [0]->GetQuantiles(nQuant,xpercentiles[0],percentiles);
	sprintf(cc3,"# combinations/road.                               #mu=%3.1lf, #Lambda_{%3.1lf}=%3.1lf",h1CombPerRoad  [0]->GetMean(),percentiles[3]*100,xpercentiles[0][3]);
	tl = new TLegend(0.25,0.75,0.9,0.9);
	tle = tl->AddEntry(h1CombPerRoad  [0],cc3,"l");
	tle->SetLineColor (h1CombPerRoad  [0]->GetLineColor());
	tle->SetLineWidth (h1CombPerRoad  [0]->GetLineWidth());
	tl->Draw("L");
	if (makePlots) {
		cCombPerRoad->SaveAs(dirPlots+cName+"__.pdf");
		cCombPerRoad->SaveAs(dirPlots+cName+"__.png");
	}

    cName=TString("cCombPerRoad_OR_")+sLogic+pName;
    h1CombPerRoadOR[0]->SetLineColor(2);
    h1CombPerRoadOR[0]->DrawCopy("same");
	h1CombPerRoadOR[0]->GetQuantiles(nQuant,xpercentiles[0],percentiles);
	sprintf(cc3,"# combinations/road.               Ov. Masked. #mu=%3.1lf, #Lambda_{%3.1lf}=%3.1lf",h1CombPerRoadOR[0]->GetMean(),percentiles[3]*100,xpercentiles[0][3]);
	tle = tl->AddEntry(h1CombPerRoadOR[0],cc3,"l");
	tle->SetLineColor (h1CombPerRoadOR[0]->GetLineColor());
	tle->SetLineWidth (h1CombPerRoadOR[0]->GetLineWidth());
	tl->Draw("L");
	if (makePlots) {
		cCombPerRoad->SaveAs(dirPlots+cName+"__.pdf");
		cCombPerRoad->SaveAs(dirPlots+cName+"__.png");
	}

    cName=TString("cCombPerRoad_OR_DsClean_")+sLogic+pName;
    h1CombPerRoadOR[1]->SetLineColor(8);
    h1CombPerRoadOR[1]->DrawCopy("same");
	h1CombPerRoadOR[1]->GetQuantiles(nQuant,xpercentiles[0],percentiles);
	sprintf(cc3,"# combinations/road. #Deltas clean Ov. Masked #mu=%3.1lf, #Lambda_{%3.1lf}=%3.1lf",h1CombPerRoadOR[1]->GetMean(),percentiles[3]*100,xpercentiles[0][3]);
	tle = tl->AddEntry(h1CombPerRoadOR[1],cc3,"l");
	tle->SetLineColor (h1CombPerRoadOR[1]->GetLineColor());
	tle->SetLineWidth (h1CombPerRoadOR[1]->GetLineWidth());
	tl->Draw("L");
	if (makePlots) {
		cCombPerRoad->SaveAs(dirPlots+cName+"__.pdf");
		cCombPerRoad->SaveAs(dirPlots+cName+"__.png");
	}

    cName=TString("cCombPerEvent_")+sLogic+pName;
    cTitle=TString("Combinations per Tower "+sLogic+pTitle);
    TCanvas* cCombPerEvent = new TCanvas(cName,cTitle,0,0,1400,900);
    cCombPerEvent->SetLogy();
    h1CombPerEvent  [0]->SetTitle(cTitle);
    h1CombPerEvent  [0]->DrawCopy();
	h1CombPerEvent  [0]->GetQuantiles(nQuant,xpercentiles[0],percentiles);
	sprintf(cc3,"                           #mu=%3.1lf, #Lambda_{%3.1lf}=%3.1lf",45.7,percentiles[3]*100,772.4);
	tl = new TLegend(0.25,0.75,0.9,0.9);
	tle = tl->AddEntry(h1CombPerEvent  [0],cc3,"l");
	tle->SetLineColor (h1CombPerEvent  [0]->GetLineColor());
	tle->SetLineWidth (h1CombPerEvent  [0]->GetLineWidth());
	tl->Draw("L");
	if (makePlots) {
		cCombPerEvent->SaveAs(dirPlots+cName+"__.pdf");
		cCombPerEvent->SaveAs(dirPlots+cName+"__.png");
	}

    cName=TString("cCombPerEvent_OR_")+sLogic+pName;
    h1CombPerEventOR[0]->SetLineColor(2);
    h1CombPerEventOR[0]->DrawCopy("same");
	h1CombPerEventOR[0]->GetQuantiles(nQuant,xpercentiles[0],percentiles);
	sprintf(cc3,"              Ov. Masked #mu=%3.1lf, #Lambda_{%3.1lf}=%3.1lf",h1CombPerEventOR[0]->GetMean(),percentiles[3]*100,467.4);
	tle = tl->AddEntry(h1CombPerEventOR[0],cc3,"l");
	tle->SetLineColor (h1CombPerEventOR[0]->GetLineColor());
	tle->SetLineWidth (h1CombPerEventOR[0]->GetLineWidth());
	tl->Draw("L");
	if (makePlots) {
		cCombPerEvent->SaveAs(dirPlots+cName+"__.pdf");
		cCombPerEvent->SaveAs(dirPlots+cName+"__.png");
	}

    cName=TString("cCombPerEvent_OR_DsClean_")+sLogic+pName;
    h1CombPerEventOR[1]->SetLineColor(8);
//    h1CombPerEventOR[1]->SetTitle(cTitle);
//    h1CombPerEventOR[1]->DrawCopy();
    h1CombPerEvent  [0]->DrawCopy();
    h1CombPerEventOR[0]->DrawCopy("same");
    h1CombPerEventOR[1]->DrawCopy("same");
	h1CombPerEventOR[1]->GetQuantiles(nQuant,xpercentiles[0],percentiles);
	sprintf(cc3,"#Deltas clean Ov. Masked #mu=%3.1lf, #Lambda_{%3.1lf}=%3.1lf",h1CombPerEventOR[1]->GetMean(),percentiles[3]*100,xpercentiles[0][3]);
	tle = tl->AddEntry(h1CombPerEventOR[1],cc3,"l");
	tle->SetLineColor (h1CombPerEventOR[1]->GetLineColor());
	tle->SetLineWidth (h1CombPerEventOR[1]->GetLineWidth());
	tl->Draw("L");
	if (makePlots) {
		cCombPerEvent->SaveAs(dirPlots+cName+"__.pdf");
		cCombPerEvent->SaveAs(dirPlots+cName+"__.png");
	}

	TFile f1("/home/rossin/Dropbox/TT/Work/figures_stubOverlapRemoval/eff_LTF_SingleMuonTest_tt27_PU0_sf1_nz4_pt3_5oo6_95c_100k/efficiency1_10dedup_ppt_sf1_nz4.root");
	TFile f2("/home/rossin/Dropbox/TT/Work/figures_stubOverlapRemoval/eff_LTF_SingleMuonTest_tt27_PU0_sf1_nz4_pt3_5oo6_95c_100k_removeOverlap/efficiency1_10dedup_ppt_sf1_nz4.root");

	TCanvas* c1 = (TCanvas*) f1.Get("c1");
	TList* list1 = c1->GetListOfPrimitives();
	TIter next1(list1);
	TObject* obj1;
	TGraphAsymmErrors* eff_graph;
	while ((obj1 = next1())) {
		std::cout << obj1->GetName() << std::endl;
		if (obj1->GetName()==TString("eff_graph")) eff_graph = (TGraphAsymmErrors*) obj1;
	}

//	eff_graph->GetYaxis()->SetTitle("#varepsilon");
//	eff_graph->SetMinimum(0.8);
	TCanvas* c1OR = (TCanvas*) f2.Get("c1");
	c1OR->SetName("eff_OR");
	c1OR->SetTitle("eff_OR");
	TList* list = c1OR->GetListOfPrimitives();
	TIter next(list);
	TObject* obj;
	TGraphAsymmErrors* eff_graphOR;
	c1->Draw();
	while ((obj = next())) {
		std::cout << obj->GetName() << std::endl;
		if (obj->GetName()==TString("eff_graph")) {
			eff_graphOR = (TGraphAsymmErrors*) obj;
			eff_graphOR->SetLineStyle  (eff_graph->GetLineStyle());
			eff_graphOR->SetLineWidth  (eff_graph->GetLineWidth()/2);
			eff_graphOR->SetMarkerStyle(eff_graph->GetMarkerStyle());
			eff_graphOR->SetMarkerSize (eff_graph->GetMarkerSize());
			eff_graphOR->SetFillColor  (eff_graph->GetFillColor());
			eff_graphOR->SetFillStyle  (eff_graph->GetFillStyle());
			eff_graphOR->SetMarkerColor(4);
			eff_graphOR->SetLineColor(4);
			eff_graphOR->GetYaxis()->SetTitle("#varepsilon");
			eff_graphOR->Draw("same p");
		}
	}
	tl = new TLegend(0.4,.13,0.95,0.35);
	tl->AddEntry(eff_graph  ,"No Overlap Removal","p");
	tl->AddEntry(eff_graphOR,"     Overlap Removal","p");
	tl->Draw();
	if (makePlots) {
		c1->SaveAs(dirPlots+TString("totalRecoEff_OR")+"__.png");
		c1->SaveAs(dirPlots+TString("totalRecoEff_OR")+"__.pdf");
	}

	return;
}


