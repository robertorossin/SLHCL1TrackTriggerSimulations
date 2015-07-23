/*
 * layerSelecPlotter.C
 *
 *  Created on: Jul 13, 2015
 *      Author: rossin
 */

#include <TH2.h>
#include <TGraph.h>
#include <TLine.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TPad.h>
#include <TString.h>
#include <vector>
#include <vector>

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>


struct laySelect {
	bool is5oo6;
	unsigned layerSelect;
	float roadMean    ;
	float road95      ;
	float combRoadMean;
	float combRoad95  ;
	float combEvMean  ;
	float combEv95  ;

	bool operator<(const laySelect& a) const
	{
		return combEv95 < a.combEv95;
	}
};

bool laySelect_cmpByLayersByComb(const laySelect& a, const laySelect& b) {
	unsigned nLayers_a = 0;
	unsigned nLayers_b = 0;
	unsigned layerSelect_a = a.layerSelect;
	unsigned layerSelect_b = b.layerSelect;
	for (unsigned ibit=0; ibit<6; ++ibit) {
		nLayers_a += layerSelect_a%10;
		nLayers_b += layerSelect_b%10;
		layerSelect_a /=10;
		layerSelect_b /=10;
	}
	if      (nLayers_a < nLayers_b) return true;
	else if (nLayers_a > nLayers_b) return false;
	else    return a.combEv95 > b.combEv95;
}

void layerSelecPlotter(TString sInFile="layerSelectCuts.txt") {

	gStyle->SetOptStat(0);

	std::vector <laySelect> vlaySelect6oo6;
	std::vector <laySelect> vlaySelect5oo6;

	bool is5oo6;
	laySelect laySelect5or6oo6;

	std::string inFileName = sInFile.Data();
	std::ifstream infile(inFileName);
	std::string line;
	while (std::getline(infile, line)) {
		std::istringstream iss(line);
		if (!(iss >> is5oo6)) { break; } // error
		laySelect5or6oo6.is5oo6 = is5oo6;
		std::string layerSelectString;
		iss >> layerSelectString;
		TString layerSelectTString(layerSelectString);
		laySelect5or6oo6.layerSelect = layerSelectTString.Atoi();
		iss >> laySelect5or6oo6.roadMean
		    >> laySelect5or6oo6.road95
				>> laySelect5or6oo6.combRoadMean
				>> laySelect5or6oo6.combRoad95
				>> laySelect5or6oo6.combEvMean
				>> laySelect5or6oo6.combEv95;

		if (is5oo6) vlaySelect5oo6.push_back(laySelect5or6oo6);
		else        vlaySelect6oo6.push_back(laySelect5or6oo6);
	}

	std::sort(vlaySelect5oo6.begin(), vlaySelect5oo6.end(), laySelect_cmpByLayersByComb);
	std::sort(vlaySelect6oo6.begin(), vlaySelect6oo6.end(), laySelect_cmpByLayersByComb);

	std::cout << vlaySelect5oo6.size() << "\t" << vlaySelect6oo6.size() << std::endl;

	TH2D* h2layerSel = new TH2D("h2layerSel","h2layerSel",vlaySelect5oo6.size(),0,vlaySelect5oo6.size(),6,0,6);

	std::vector <float> vcombEv95;
	std::vector <float> vx;

	for (unsigned index=0; index<vlaySelect5oo6.size(); ++index) {
		std::cout << vlaySelect5oo6.at(index).is5oo6       << "\t"
      				<< vlaySelect5oo6.at(index).layerSelect  << "\t"
							<< vlaySelect5oo6.at(index).combEv95     << "\t"
							<< std::endl;
		vcombEv95.push_back(vlaySelect5oo6.at(index).combEv95);
		unsigned layerSelect = vlaySelect5oo6.at(index).layerSelect;
		for (unsigned ibit=0; ibit<6; ++ibit) {
			h2layerSel->SetBinContent(index+1,6-ibit,layerSelect%10);
			layerSelect /=10;
		}
	}
	auto it = max_element(std::begin(vcombEv95),std::end(vcombEv95));
	float max = *it;
	it = min_element(std::begin(vcombEv95),std::end(vcombEv95));
	float min = *it;
	for (unsigned index=0; index<vlaySelect5oo6.size(); ++index) {
//		vcombEv95.at(index) = 6*(vcombEv95.at(index)-min)/(max-min);
		vcombEv95.at(index) = max/vcombEv95.at(index);
		vx.push_back(index+0.5);
	}
	TGraph* gr = new TGraph(vx.size(),&vx[0],&vcombEv95[0]);
	TH1D* h1NlayerSelect = h2layerSel->ProjectionX("h1NlayerSelect",1,vlaySelect5oo6.size());
	TCanvas* c2layerSel = new TCanvas("c2layerSel","c2layerSel",0,0,900,900);
//	c2layerSel->Divide(1,2);
//	c2layerSel->cd(1);
	h2layerSel->GetZaxis()->SetRangeUser(-1,1);
	char cc1[50];
	char cc2[50];
	sprintf(cc1,"%3.0f",max);
	sprintf(cc2,"%3.0f",min);
	h2layerSel->SetTitle("Layer Selection DeltaS impact. 0-Layers -> 6-Layers: "+TString(cc1)+TString("->")+TString(cc2));
	h2layerSel->Draw("col");
	gr->SetLineWidth(3);
	gr->Draw("same");
//	c2layerSel->cd(2);
	h1NlayerSelect->SetLineWidth(3);
//	h1NlayerSelect->Draw("same");
	TLine* l;
	unsigned oldBinContent = 0;
	for (unsigned index=0; index<vlaySelect5oo6.size(); ++index) {
		if (h1NlayerSelect->GetBinContent(index+1) != oldBinContent) {
			oldBinContent = h1NlayerSelect->GetBinContent(index+1);
			l = new TLine(index,0,index,6);
			l->SetLineWidth(5);
			l->SetLineColor(4);
			l->Draw();
		}
	}
	TString dirPlots("/home/rossin/Dropbox/TT/Work/figures_stubCleaning/");
	c2layerSel->SaveAs(dirPlots+TString("c2layerSelectDeltasImpact.pdf"));

	return;

}
