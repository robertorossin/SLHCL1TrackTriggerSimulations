/*
 * overlapRemovalSystematics.C
 *
 *  Created on: Aug 17, 2015
 *      Author: rossin
 */

#include <TStyle.h>
#include <TFile.h>
#include <TTree.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TPolyLine.h>
#include <TList.h>
#include <TString.h>
#include <TMath.h>
#include <iostream>
#include <iomanip> // for setw
#include <vector>

class module{
public:
	unsigned id   ;
	unsigned layer;
	unsigned x1   ;
	unsigned y1   ;
	unsigned x2   ;
	unsigned y2   ;
	unsigned mx1   ;
	unsigned my1   ;
	unsigned mx2   ;
	unsigned my2   ;
//	TH1F* h; // histo to store area ratios
	module(unsigned iid,unsigned ilayer,unsigned ix1,unsigned iy1,unsigned ix2,unsigned iy2) {
		id    = iid   ;
		layer = ilayer;
		x1    = ix1   ;
		y1    = iy1   ;
		x2    = ix2   ;
		y2    = iy2   ;

		mx1   = ix1   ;
		my1   = iy1   ;
		mx2   = ix2   ;
		my2   = iy2   ;
	}

};


void overlapRemovalSystematics() {

	gStyle->SetOptStat(111111);
	TString cProc[4];

	cProc[0] = "";
	cProc[1] = "XY1mmAway_";
	cProc[2] = "XY1mmTowards_";
	cProc[3] = "XY1mmOrthogonal_";

	TFile* fIn[4];
	TTree* tmodule[4];
	std::map <unsigned , module> moduleMap;

	for (unsigned iProc=0;iProc<4; ++iProc) {
		TString cFile(TString("/home/rossin/Dropbox/TT/Work/figures_stubOverlapRemoval/stubOverlapHistos"+cProc[iProc]));
		fIn[iProc]=new TFile(cFile+TString("Frames.root"));
		if (!fIn[iProc]->IsOpen()) {
			std::cout << "ERROR. Cannot open file " << cFile << std::endl;
			std::cout << "Exiting." << std::endl;
			return;
		}
		tmodule[iProc] = (TTree*)fIn[iProc]->Get("tmodule");
		if (!tmodule[iProc]) {
			std::cout << "ERROR. Cannot open file " << cFile << std::endl;
			std::cout << "Exiting." << std::endl;
			return;
		}
		std::cout << "Opening file " << cFile+TString("Frames.root") << std::endl;

		Int_t id   ;
		Int_t layer;
		Int_t x1   ;
		Int_t y1   ;
		Int_t x2   ;
		Int_t y2   ;
	  tmodule[iProc]->SetBranchAddress("id"   ,&id   );
	  tmodule[iProc]->SetBranchAddress("layer",&layer);
	  tmodule[iProc]->SetBranchAddress("x1"   ,&x1   );
	  tmodule[iProc]->SetBranchAddress("x2"   ,&x2   );
	  tmodule[iProc]->SetBranchAddress("y1"   ,&y1   );
	  tmodule[iProc]->SetBranchAddress("y2"   ,&y2   );

	  Long64_t nevent = tmodule[iProc]->GetEntries();
	  for (Long64_t i=0;i<nevent;i++) {
	    //read branch "fNtrack"only
	    tmodule[iProc]->GetEntry(i);
//	    std::cout << id << "\t" << layer << std::endl;
	    if (!iProc) {
	    	module m(id,layer,x1,y1,x2,y2);
	    	char hName[50];
	    	sprintf(hName,"hOverlapSys_Mod%u",id);
//	    	m.h = new TH1F(hName,hName,20,0.9,1.1);
	    	std::pair<std::map <unsigned , module>::iterator,bool> ret;
	    	ret = moduleMap.insert(std::make_pair(id,m));
	    	if (ret.second==false) {
	    		std::cout << "ERROR. Module " << id << " already present. Exiting." << std::endl;
	    		return;
	    	}
	    } // nominal positions
	    else { //displaced samples
	    	std::map <unsigned , module>::iterator it;
	    	it = moduleMap.find(id);
	    	if (it != moduleMap.end()) {

	    		if (x1!=it->second.mx1 ||
	    				x2!=it->second.mx2 ||
							y1!=it->second.my1 ||
							y2!=it->second.my2)
	    		std::cout << iProc
	    				<< std::setw(8) << id
	    				<< std::setw(10) << x1 << std::setw(5) << it->second.mx1
	    				<< std::setw(10) << x2 << std::setw(5) << it->second.mx2
	    				<< std::setw(10) << y1 << std::setw(5) << it->second.my1
	    				<< std::setw(10) << y2 << std::setw(5) << it->second.my2
							<< std::endl;

	    		it->second.mx1 = ( x1 < (Int_t)it->second.mx1 ? x1 : it->second.mx1 );
	    		it->second.my1 = ( y1 < (Int_t)it->second.my1 ? y1 : it->second.my1 );
	    		it->second.mx2 = ( x2 > (Int_t)it->second.mx2 ? x2 : it->second.mx2 );
	    		it->second.my2 = ( y2 > (Int_t)it->second.my2 ? y2 : it->second.my2 );

//	    		std::cout << iProc
//	    				<< std::setw(8) << id
//	    				<< std::setw(10) << x1 << std::setw(5) << it->second.mx1
//	    				<< std::setw(10) << x2 << std::setw(5) << it->second.mx2
//	    				<< std::setw(10) << y1 << std::setw(5) << it->second.my1
//	    				<< std::setw(10) << y2 << std::setw(5) << it->second.my2
//							<< std::endl
//							<< std::endl;
	    	} else {
	    		std::cout << "ERROR. Module " << id << " not found. Exiting." << std::endl;
	    		return;
	    	}
	    }
	  }

	}

	char cc[100];
	sprintf(cc,"cOverlapSys");
	TCanvas* cOverlapSys = new TCanvas(cc,cc,0,0,900,900);
	cOverlapSys->Divide(3,2);
	TH1F* hSys[6];
	for (unsigned iLay=0; iLay<6; ++iLay) {
		char hName[50];
		sprintf(hName,"hOverlapSys_Layer%u",iLay);
		hSys[iLay] = new TH1F(hName,hName,25,1.0,1.05);
		for (std::map <unsigned , module>::iterator it = moduleMap.begin(); it != moduleMap.end(); ++it) {
			if (it->second.layer == iLay) {
				float  x1 = it->second. x1;
				float  y1 = it->second. y1;
				float  x2 = it->second. x2;
				float  y2 = it->second. y2;
				float mx1 = it->second.mx1;
				float my1 = it->second.my1;
				float mx2 = it->second.mx2;
				float my2 = it->second.my2;
				hSys[iLay]->Fill((mx2-mx1)*(my2-my1)/((x2-x1)*(y2-y1)));
			}
		}
		cOverlapSys->cd(iLay+1);
		hSys[iLay]->DrawCopy();
	}
	return;
}

