/*
 * roadMerger.C
 *
 *  Created on: Mar 10, 2016
 *      Author: rossin
 */
#define roadMerger_cxx
#include "roadMerger.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TMath.h>
#include <TRandom.h>
//#include <TInterpreter.h>
//#include "/home/rossin/Dropbox/TT/CMSSW_6_2_0_SLHC12_patch1_SL6/src/SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTRoad.h"

#include <iostream>
#include <map>
#include <vector>
#include <iosfwd>

using namespace std;

#define magicNumber 243

class TTRoad {
public:
	unsigned patternRef;
    unsigned tower;
//    unsigned nstubs;
    std::vector<unsigned> superstripIds;  // necessary?
    std::vector<std::vector<unsigned> > stubRefs;  // stubRefs[superstrip i][stub j]
    std::vector <unsigned> nStubs;

    unsigned getCombinations () {
    	unsigned nComb = 1;
    	for (unsigned iLay=0; iLay<nStubs.size(); ++iLay) if (nStubs.at(iLay)) nComb *= nStubs.at(iLay);
    	return nComb;
    }
};

void printRoad (const TTRoad& road) {
	cout << " patternId " << road.patternRef << endl;
	cout << " superstripIds " ;
	for (unsigned iSS=0; iSS<road.superstripIds.size(); ++iSS) cout << road.superstripIds.at(iSS) << " ";
	cout << endl;
	cout << " nStubs " ;
	for (unsigned iSS=0; iSS<road.nStubs.size(); ++iSS) cout << road.nStubs.at(iSS) << " ";
	cout << endl;
}
// _____________________________________________________________________________
// Output streams
//std::ostream& operator<<(std::ostream& o, const TTRoad& road);

void printCombination (vector <unsigned> combination) {
	cout << "[ ";
//	int maxstubs = 0;
//	for (unsigned ilayer = 0; ilayer < 6; ++ilayer) {
//		maxstubs = (int)data.at(ilayer).size()>maxstubs ? data.at(ilayer).size() : maxstubs;
//	}
	for (unsigned ilayer = 0; ilayer < combination.size(); ++ilayer) {
		cout << combination.at(ilayer) << " ";
	}
	cout << "]" << endl;
	return;
}

void printData (vector <vector <unsigned> > data) {
	cout << "[[ ";
//	int maxstubs = 0;
//	for (unsigned ilayer = 0; ilayer < 6; ++ilayer) {
//		maxstubs = (int)data.at(ilayer).size()>maxstubs ? data.at(ilayer).size() : maxstubs;
//	}
	for (unsigned ilayer = 0; ilayer < data.size(); ++ilayer) {
		int nstubs = data.at(ilayer).size();
		if (ilayer) cout << " [ ";
		for (int istub = 0; istub < nstubs; ++istub) {
			cout << data.at(ilayer).at(istub) << " ";
		}
		if (ilayer != data.size()-1)cout << "]" << endl;
	}
	cout << "]]" << endl;
	cout << "# arrays: " << data.size() << endl;
	return;
}

const unsigned nLay = 6;

class mergedRoad {
	unsigned mergedPatternId_;
	vector <unsigned> patternIds_;
	vector <unsigned> roadIds_;
	vector <vector <unsigned> > SSIds_;
	vector <unsigned> nStubs_;
	float patternInvPt_;
public:
	mergedRoad(unsigned mergedPatternId, unsigned firstPatternId, unsigned firstRoadId, const vector <unsigned> &SSIds0, const vector <unsigned> &nStubs, float patternInvPt) {
		mergedPatternId_ = mergedPatternId;
		patternIds_.push_back(firstPatternId);
		roadIds_   .push_back(firstRoadId);
		for (unsigned iLay=0; iLay<nLay; ++iLay) {
			vector <unsigned> v(0);
			v.push_back(SSIds0.at(iLay));
			SSIds_.push_back(v);
		};
		nStubs_          = nStubs;
		patternInvPt_    = patternInvPt;
	}
	void setPatternsIds(vector         <unsigned> patternIds) {patternIds_=patternIds;
	cout << " patternIds_ "<< patternIds_.size(); for (unsigned i=0; i<patternIds_.size(); ++i) cout << " " <<  patternIds_.at(i) ; cout << endl;
	}
	void setRoadIds    (vector         <unsigned>    roadIds) {   roadIds_=   roadIds; cout << " roadIds_    "<< roadIds_   .size() <<endl;}
	void setSSIds      (vector <vector <unsigned> >    SSIds) {     SSIds_=     SSIds; cout << " SSIds_      "<< SSIds_     .size() <<endl;}
	void setNstubs     (vector         <unsigned>     nStubs) {    nStubs_=    nStubs; cout << " nStubs_     "<< nStubs_    .size() <<endl;}
	void getNstubs     (vector         <unsigned>    &nStubs) {    nStubs =   nStubs_;}
	void add           (TTRoad road) {
		unsigned nDifferentLayers   = 0;
        unsigned siblingPhiDistance = 0;
        unsigned siblingZDistance   = 0;
		for (unsigned iLay=0; iLay<nLay; ++iLay) {
			bool isNewSS = true;
//			cout <<  mergedPatternId_ <<  " SSIds_.size() " << SSIds_.size() << endl;
//			cout << "road.superstripIds.size() " << road.superstripIds.size() << endl;
			for (unsigned iMergedSS=0; iMergedSS<SSIds_.at(iLay).size(); ++iMergedSS) {
			    int ssID1 = (int)SSIds_.at(iLay).at(iMergedSS);
			    int ssID2 = road.superstripIds.at(iLay);
				if (ssID1 == ssID2) isNewSS = false;
				else {
				    siblingPhiDistance = TMath::Abs((int)(ssID1%magicNumber)-(int)(ssID2%magicNumber)) ; //absolute distance in phi
                    siblingZDistance   = TMath::Abs((int)(ssID1/magicNumber)-(int)(ssID2/magicNumber)) ; //absolute distance in z
				}
//				cout << "SSIds_.at(" << iLay <<").at("<< iMergedSS <<") " << SSIds_.at(iLay).at(iMergedSS) << " ";
			}
//			cout << endl;
			if (isNewSS) {
//				cout << "\t ## \t" << nStubs_.at(iLay) << "  " <<  road.nStubs.size() << endl;
				SSIds_.at(iLay).push_back(road.superstripIds.at(iLay));
				patternIds_.push_back(road.patternRef);
//				cout << "patternIds_.size() " <<patternIds_.size() << endl;
//				for (unsigned iPid=0; iPid<patternIds_.size(); ++iPid) cout << patternIds_.at(iPid) << endl;
				nStubs_.at(iLay) += road.nStubs.at(iLay);
				++nDifferentLayers;
			}
		};
		if (nDifferentLayers>3) {
			cout << "******** ERROR *********" << endl;
			cout << "Siblings differ by more than two layers" << endl;
		}
		if (siblingPhiDistance>1 || siblingZDistance>1) {
			cout << "******** ERROR *********" << endl;
			cout << "Siblings distance > 1\t"  << siblingPhiDistance << " " << siblingZDistance << endl;
		}
	}
    unsigned getCombinations () {
    	unsigned nComb = 1;
    	for (unsigned iLay=0; iLay<nStubs_.size(); ++iLay) if (nStubs_.at(iLay)) nComb *= nStubs_.at(iLay);
    	return nComb;
    }
    float getRoadInvPt () {return patternInvPt_;}
	void print (bool verbose = 0) {
		if (patternIds_.size()<2 and !verbose) return;
		cout << "-----------------------------------------" <<endl;
		cout << " mergedPatternId_ "<< mergedPatternId_ <<endl;
		cout << " patternIds_ " ;
		for (unsigned iPid=0; iPid<patternIds_.size(); ++iPid) cout << patternIds_.at(iPid) << " " ;
		cout << endl;
		cout << "[" ;
		for (unsigned iLay=0; iLay<SSIds_.size() ; ++iLay)  {
			cout << " [ " ;
			for (unsigned iSS=0; iSS<SSIds_.at(iLay).size(); ++iSS) cout << SSIds_.at(iLay).at(iSS) << " ";
			cout << "]";
		}
		cout << "]" << endl;
		cout << " nStubs_ "; for (unsigned ssid = 0; ssid<nLay; ++ssid) cout << nStubs_.at(ssid) << " " ; cout << endl;
		cout << "-----------------------------------------" <<endl;
	}
};

void roadMerger::Loop(unsigned int iEv,unsigned merging,TString pName, bool makePlots, bool debug, TString fName)
{

	unsigned pu=0;
	if (pName.Contains("PU140")) pu=140;
    if (pName.Contains("PU200")) pu=200;
    if (pName.Contains("PU250")) pu=250;

    bool isTTbar =0;
    if (pName.Contains("TTbar")) isTTbar=1;

   TFile* fmergedPatt = 0;
   if (pName.Contains("sf1L0x2L5x2_nz4")) {
	   if (merging==2) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m_patternBank_tt27_sf1L0x2L5x2_nz4_pt3_300M.root" ,"READ");
	   if (merging==4) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m4_patternBank_tt27_sf1L0x2L5x2_nz4_pt3_300M.root","READ");
   }
   if (pName.Contains("sf1L0x2L5x2_nz6")) {
	   if (merging==2) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m_patternBank_tt27_sf1L0x2L5x2_nz6_pt3_300M.root"  ,"READ");
       if (merging==4) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m4_patternBank_tt27_sf1L0x2L5x2_nz6_pt3_300M.root" ,"READ");
       if (merging==8) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m8_patternBank_tt27_sf1L0x2L5x2_nz6_pt3_300M.root" ,"READ");
   }
   if (pName.Contains("sf1L0x2L5x2_nz8")) {
	   if (merging==2) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m_patternBank_tt27_sf1L0x2L5x2_nz8_pt3_300M.root" ,"READ");
       if (merging==4) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m4_patternBank_tt27_sf1L0x2L5x2_nz8_pt3_300M.root" ,"READ");
       if (merging==8) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m8_patternBank_tt27_sf1L0x2L5x2_nz8_pt3_300M.root" ,"READ");
   }
   if (pName.Contains("sf1L0x2L5x2__nz8_OR")) {
       if (merging==4) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m4_patternBank_tt27_sf1L0x2L5x2_nz8_pt3_300M_removeOverlap0p8_0p8_0p6_0p8_0p6_0p5.root" ,"READ");
       if (merging==8) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m8_patternBank_tt27_sf1L0x2L5x2_nz8_pt3_300M_removeOverlap0p8_0p8_0p6_0p8_0p6_0p5.root" ,"READ");
   }
   if (pName.Contains("sf1L0x2_nz4"    )) {
	   if (merging==2) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m_patternBank_tt27_sf1L0x2_nz4_pt3_300M.root"  ,"READ");
	   if (merging==4) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m4_patternBank_tt27_sf1L0x2_nz4_pt3_300M.root" ,"READ");
   }
   if (pName.Contains("sf1L0x2_nz6"    )) {
	   if (merging==2) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m_patternBank_tt27_sf1L0x2_nz6_pt3_300M.root"  ,"READ");
       if (merging==4) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m4_patternBank_tt27_sf1L0x2_nz6_pt3_300M.root" ,"READ");
       if (merging==8) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m8_patternBank_tt27_sf1L0x2_nz6_pt3_300M.root" ,"READ");
   }
   if (pName.Contains("sf1L0x2_nz8"    )) {
	   if (merging==2) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m_patternBank_tt27_sf1L0x2_nz8_pt3_300M.root"  ,"READ");
	   if (merging==4) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m4_patternBank_tt27_sf1L0x2_nz8_pt3_300M.root" ,"READ");
       if (merging==8) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m8_patternBank_tt27_sf1L0x2_nz8_pt3_300M.root" ,"READ");
   }
//   if (pName.Contains("sf1_nz4"    )) {
//	   if (merging==4) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m4_patternBank_tt27_sf1_nz4_pt3.root" ,"READ");
//   }
   if (!fmergedPatt->IsOpen()) {
	   std::cout << "ERROR. Can not open pattern file. Exiting..." << std::endl;
	   return;
   }
   TTree *patternBank       = (TTree*) fmergedPatt->Get("patternBank");
   unsigned int nTotPatterns = patternBank->GetEntries();
   TTree *  toMerged     = (TTree*) fmergedPatt->Get(  "toMerged");
   TTree *fromMerged     = (TTree*) fmergedPatt->Get("fromMerged");
   vector  <unsigned int>   *indToMerged  = 0;
   vector  <unsigned int>   *indFromMerged= 0;
   vector <vector<unsigned int> > vindFromMerged;
   toMerged  ->SetBranchAddress("indToMerged"  ,&indToMerged  );
   fromMerged->SetBranchAddress("indFromMerged",&indFromMerged);
   toMerged->GetEntry(0);
   if (indToMerged->size()<=nTotPatterns) std::cout << " # pattern indices loaded = " << indToMerged->size() << std::endl;
   else                                  {
	   std::cout << " ERROR. # loaded indices > # patterns in full bank. Exiting..." << std::endl;
	   return;
   }
   unsigned nMerged = (unsigned)fromMerged->GetEntries();
   for (unsigned iMerged=0; iMerged<nMerged;++iMerged) {
	   fromMerged->GetEntry(iMerged);
	   vindFromMerged.push_back(*indFromMerged);
//	   if (iMerged%100==0) cout << iMerged << "\t" << indFromMerged->size() << endl;
   }

   TTree *patternAttributes = (TTree*) fmergedPatt->Get("patternAttributes");
   vector <float> vpatternInvPt;
   float patternInvPt = 0;
   patternAttributes->SetBranchAddress("invPt_mean"  ,&patternInvPt);
   for (unsigned iPatt=0; iPatt<patternAttributes->GetEntries(); ++iPatt) {
       patternAttributes->GetEntry(iPatt);
       vpatternInvPt.push_back(patternInvPt);
       if (iPatt%500000==0) cout << "Reading patternAttributes " << iPatt << endl;
   }

   std::cout << " # merged pattern indices loaded = " << vindFromMerged.size() << std::endl;
   long unsigned anPatterns[]={indToMerged->size(),vindFromMerged.size()};


   unsigned maxRoads[2] = {0,0};
   unsigned maxCombs[2] = {0,0};
   TH1F* hnRoads       [2];
   TH1F* hnCombsPerRoad[2];
   TH1F* hnCombsPerBX  [2];
   TH1F* hnStubsPerSSPerLay [2][8];
   TString smergingN("");
   if (merging==2) smergingN=TString("_x2_");
   if (merging==4) smergingN=TString("_x4_");
   if (merging==8) smergingN=TString("_x8_");

   for (unsigned i=0; i<2; ++i) {
	   TString hname("nRoads");
	   if (i) hname+=TString("Merged")+smergingN;
	   unsigned nbins = 150;
	   double xmax = (double) nbins;
	   if (pu==200) {nbins*=2;xmax*=2;}
       if (pu==250) {nbins*=2;xmax*=4;}
       if (isTTbar) {nbins*=2;xmax*=4;}
       hnRoads       [i] = new TH1F(hname,hname,nbins,0,xmax);
	   hname=TString("nCombsPerRoad");
	   if (i) hname+=TString("Merged"+smergingN);
	   nbins = 30;
	   xmax = (double) nbins;
	   if (pu==200) {nbins*=1.5;xmax*=1.5;}
       if (pu==250) {nbins*=2  ;xmax*=2  ;}
       if (isTTbar) {nbins*=2;xmax*=2;}
	   hnCombsPerRoad[i] = new TH1F(hname,hname,nbins,0,xmax);
	   hname=TString("nCombsPerBX");
	   if (i) hname+=TString("Merged"+smergingN);
	   nbins = 600;
	   xmax = (double) nbins;
       if (pu==200) {nbins*=1.5;xmax*=3;}
       if (pu==250) {nbins*=3  ;xmax*=6;}
       if (isTTbar) {nbins*=2;xmax*=4;}
	   hnCombsPerBX  [i] = new TH1F(hname,hname,nbins,0,xmax);
	   for (unsigned iLay=0; iLay<8; ++iLay) {
		   char name[100]; sprintf(name,"_%d_",iLay);
		   if (i==0) hname=TString("hStubsPerSS_Layer"+TString(name));
		   else      hname=TString("hStubsPerSS_Layer"+TString(name)+smergingN);
		   hnStubsPerSSPerLay[i][iLay] = new TH1F(hname,hname,10,0,10);
	   }
   }

   if (fChain == 0) return;
   Long64_t nentries = TMath::Min((unsigned)fChain->GetEntriesFast(),iEv);
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
//      std::map <unsigned, unsigned > nStubMap[nLay];
	  unsigned int nRoads = AMTTRoads_nstubs->size();
	  if (!nRoads) {
		  for (unsigned i=0; i<2; ++i) {
			  hnRoads     [i]->Fill(0);
			  hnCombsPerBX[i]->Fill(0);
		  }
		  continue;
	  }
	  unsigned nCombsPerBX       = 0;
	  unsigned nCombsPerBXMerged = 0;

//	  for (unsigned int iRoads=0; iRoads<nRoads; ++iRoads) { // LOOPING OVER ROADS in EVENT
//	  	TTRoad road;
//	  	road.patternRef     = AMTTRoads_patternRef   ->at(iRoads);
//	  	road.stubRefs       = AMTTRoads_stubRefs     ->at(iRoads);
//	  	road.superstripIds  = AMTTRoads_superstripIds->at(iRoads);
////	  	road.nstubs         = AMTTRoads_nstubs       ->at(iRoads);
//	  	for (unsigned int iSS=0; iSS<road.superstripIds.size();++iSS) { // Looping over SS in ROAD
//	  		unsigned int ssID = road.superstripIds.at(iSS);
//	  		unsigned int nStubs = road.stubRefs.at(iSS).size();
//	  		road.nStubs.push_back(nStubs);
////	  		nStubMap[iSS].insert(make_pair(ssID,nStubs));
//	  	}
//	  }

	  std::map <unsigned, mergedRoad>              mapMergedRoads;
	  std::map <unsigned, mergedRoad>::iterator it_mapMergedRoads;
	  unsigned nRoadsByPt = 0;
	  for (unsigned int iRoads=0; iRoads<nRoads; ++iRoads) { // LOOPING OVER ROADS in EVENT
		  TTRoad road;
		  road.patternRef     = AMTTRoads_patternRef   ->at(iRoads);
		  road.stubRefs       = AMTTRoads_stubRefs     ->at(iRoads);
		  road.superstripIds  = AMTTRoads_superstripIds->at(iRoads);
		  //		  	road.nstubs         = AMTTRoads_nstubs       ->at(iRoads);
          patternInvPt = vpatternInvPt.at(road.patternRef);
//          if (TMath::Abs(1.0/patternInvPt)>5.0 || TMath::Abs(1.0/patternInvPt)<0.0) continue;
          ++nRoadsByPt;
		  vector <unsigned> nStubsPerLayer;
          unsigned maxNstubPerLayer=0;
          unsigned maxLayermaxNstub=7;
		  for (unsigned iLay=0; iLay<nLay; ++iLay) {
		      unsigned nStubsPL = road.stubRefs.at(iLay).size();
		      if (maxNstubPerLayer < nStubsPL) {
		          maxNstubPerLayer = nStubsPL;
		          maxLayermaxNstub = iLay;
		      }
		      hnStubsPerSSPerLay[0][iLay]->Fill(nStubsPL);
			  nStubsPerLayer.push_back(nStubsPL);
		  }
          hnStubsPerSSPerLay[0][6]->Fill(maxNstubPerLayer);
          hnStubsPerSSPerLay[0][7]->Fill(maxLayermaxNstub);
          road.nStubs = nStubsPerLayer;
		  if (debug) {
			  cout <<  "  # " << iRoads << " " <<  jentry << endl;
			  printRoad(road);
		  }
		  if (road.patternRef >= indToMerged->size()) {
			  cout << "*** WARNING ***" << endl;
			  cout << "road.patternRef >= indToMerged->size()  " << road.patternRef << ">=" <<  indToMerged->size() << endl;
			  cout << "skipping road" << endl;
			  continue;
		  }
		  unsigned mergedPatternRef = indToMerged->at(road.patternRef);
		  //		  	unsigned nMerged = vindFromMerged.at(mergedPatternRef).size();
		  mergedRoad mr(mergedPatternRef,road.patternRef,iRoads, road.superstripIds,nStubsPerLayer,patternInvPt);
		  //		  	mr.setPatternsIds();
		  //		  	for (unsigned iMerged=0; iMerged<nMerged ; ++iMerged) {
		  //		  		unsigned patternRef       = vindFromMerged.at(mergedPatternRef).at(iMerged);
		  //		  		mr.setPatternsIds(vindFromMerged.at(mergedPatternRef));
		  //		  	}
		  it_mapMergedRoads = mapMergedRoads.find(mergedPatternRef);
		  if (it_mapMergedRoads == mapMergedRoads.end()) mapMergedRoads.insert(make_pair(mergedPatternRef,mr));
		  else                                           it_mapMergedRoads->second.add(road);

		  unsigned nCombRoad       = road.getCombinations();
		  hnCombsPerRoad[0]->Fill(nCombRoad);
		  nCombsPerBX       += nCombRoad;
	  }

	  for (it_mapMergedRoads=mapMergedRoads.begin(); it_mapMergedRoads!=mapMergedRoads.end(); ++it_mapMergedRoads) {
		  mergedRoad mr = it_mapMergedRoads->second;
//		  if (TMath::Abs(1.0/mr.getRoadInvPt())>5) cout << "MMMHHHH" << endl;
		  unsigned nCombRoadMerged = mr.getCombinations();
		  hnCombsPerRoad[1]->Fill(nCombRoadMerged);
		  nCombsPerBXMerged += nCombRoadMerged;
		  if (debug) mr.print();
		  vector <unsigned> nStubs;
		  mr.getNstubs(nStubs);
          vector <unsigned> nStubsPerLayer;
          unsigned maxNstubPerLayer=0;
          unsigned maxLayermaxNstub=7;
          for (unsigned iLay=0; iLay<nLay; ++iLay) {
              unsigned nStubsPL = nStubs.at(iLay);
              if (maxNstubPerLayer < nStubsPL) {
                  maxNstubPerLayer = nStubsPL;
                  maxLayermaxNstub = iLay;
              }
              hnStubsPerSSPerLay[1][iLay]->Fill(nStubsPL);
              nStubsPerLayer.push_back(nStubsPL);
          }
          hnStubsPerSSPerLay[1][6]->Fill(maxNstubPerLayer);
          hnStubsPerSSPerLay[1][7]->Fill(maxLayermaxNstub);
	  }
	  hnRoads     [0]->Fill(nRoadsByPt);
	  hnRoads     [1]->Fill(mapMergedRoads.size());
	  hnCombsPerBX[0]->Fill(nCombsPerBX);
	  hnCombsPerBX[1]->Fill(nCombsPerBXMerged);

      if (maxRoads[0] < nRoadsByPt) maxRoads[0] = nRoadsByPt;
      if (maxRoads[1] < mapMergedRoads.size()) maxRoads[1] = mapMergedRoads.size();
      if (maxCombs[0] < nCombsPerBX) maxCombs[0] = nCombsPerBX;
      if (maxCombs[1] < nCombsPerBXMerged) maxCombs[1] = nCombsPerBXMerged;

	  if (debug) cout << "##########################################################" << endl << endl;

      if (jentry%100==0) {
//    	  for (unsigned iLay=0; iLay<nLay; ++iLay) {
//    		  cout << "N ss with stubs in layer[" << iLay << "] = " << nStubMap[iLay].size() << endl;
//    	  }
    	  std::cout << "Processing event " << jentry << std::endl;
      }
   }
   TCanvas* cRoads        = new TCanvas("cRoads"       ,"cRoads"       ,0  ,0  ,700,600);
   TCanvas* cCombsPerRoad = new TCanvas("cCombsPerRoad","cCombsPerRoad",100,100,700,600);
   TCanvas* cCombsPerBX   = new TCanvas("cCombsPerBX"  ,"cCombsPerBX"  ,200,200,700,600);
    cRoads->cd();
    cRoads->SetLogy();
   hnRoads[0]->SetLineWidth(2);
   hnRoads[0]->SetLineColor(1);
   for (unsigned i=0; i<2; ++i) hnRoads[i]->SetTitle(hnRoads[i]->GetTitle()+TString(" ")+pName+TString(";#roads/BX"));
   hnRoads[0]->Draw();
   hnRoads[1]->SetLineWidth(2);
   hnRoads[1]->SetLineColor(2);
   hnRoads[1]->Draw("same");
   TLegend* tl = cRoads->BuildLegend(0.15,0.7,0.9,0.9);
   double x[3];
   double xperc[3]={0.90,0.95,0.99};
   for (unsigned i=0; i<2; ++i) {
	   hnRoads[i]->GetQuantiles(3,x,xperc);
	   char label[100];
	   sprintf(label,"Mean= %4.2lf Q[%4.2lf,%4.2lf,%4.2lf]= %4.2lf %4.2lf %4.2lf",hnRoads[i]->GetMean(),xperc[0],xperc[1],xperc[2],x[0],x[1],x[2]);
	   cout << hnRoads[i]->GetTitle() << "\t" << label << " max= " << maxRoads[i] << "\t nPatterns= " << anPatterns[i]  << endl;
	   tl->AddEntry(hnRoads[i],label);
   }
   tl->DrawClone();

    cCombsPerRoad->cd();
   hnCombsPerRoad[0]->SetLineWidth(2);
   hnCombsPerRoad[0]->SetLineColor(1);
   for (unsigned i=0; i<2; ++i) hnCombsPerRoad[i]->SetTitle(hnCombsPerRoad[i]->GetTitle()+TString(" ")+pName+TString(";#combs/road"));
   hnCombsPerRoad[0]->Draw();
   hnCombsPerRoad[1]->SetLineWidth(2);
   hnCombsPerRoad[1]->SetLineColor(2);
   hnCombsPerRoad[1]->Draw("same");
   tl = cCombsPerRoad->BuildLegend(0.25,0.7,0.9,0.9);
   for (unsigned i=0; i<2; ++i) {
	   hnCombsPerRoad[i]->GetQuantiles(3,x,xperc);
	   char label[100];
	   sprintf(label,"Mean= %4.2lf Q[%4.2lf,%4.2lf,%4.2lf]= %4.2lf %4.2lf %4.2lf",hnCombsPerRoad[i]->GetMean(),xperc[0],xperc[1],xperc[2],x[0],x[1],x[2]);
	   cout << hnCombsPerRoad[i]->GetTitle() << "\t" << label << "\t nPatterns= " << anPatterns[i]  << endl;
	   tl->AddEntry(hnCombsPerRoad[i],label);
   }
   tl->DrawClone();

    cCombsPerBX->cd();
    cCombsPerBX->SetLogy();
   for (unsigned i=0; i<2; ++i) hnCombsPerBX[i]->SetTitle(hnCombsPerBX[i]->GetTitle()+TString(" ")+pName+TString(";#combs/BX"));
   hnCombsPerBX[0]->SetLineWidth(2);
   hnCombsPerBX[0]->SetLineColor(1);
   hnCombsPerBX[0]->Draw();
   hnCombsPerBX[1]->SetLineWidth(2);
   hnCombsPerBX[1]->SetLineColor(2);
   hnCombsPerBX[1]->Draw("same");
   tl = cCombsPerBX->BuildLegend(0.15,0.65,0.9,0.9);
   for (unsigned i=0; i<2; ++i) {
	   hnCombsPerBX[i]->GetQuantiles(3,x,xperc);
	   char label[100];
	   sprintf(label,"Mean= %4.2lf Q[%4.2lf,%4.2lf,%4.2lf]= %4.2lf %4.2lf %4.2lf",hnCombsPerBX[i]->GetMean(),xperc[0],xperc[1],xperc[2],x[0],x[1],x[2]);
	   cout << hnCombsPerBX[i]->GetTitle() << "\t" << label  << " max= " << maxCombs[i] << "\t nPatterns= " << anPatterns[i] << endl;
	   tl->AddEntry(hnCombsPerBX[i],label);
   }
   tl->DrawClone();

   TCanvas* cStubsPerSS = new TCanvas("cStubsPerSS","cStubsPerSS",0,0,900,900);
   cStubsPerSS->Divide(3,3);
   for (unsigned iLay=0; iLay<8; ++iLay) {
       cStubsPerSS->cd(iLay+1);
       if (iLay!=7) gPad->SetLogy();
       gPad->SetGrid();
       for (unsigned i=0; i<2; ++i) {
           if (i==0) {
               hnStubsPerSSPerLay[i][iLay]->SetLineColor(1);
               hnStubsPerSSPerLay[i][iLay]->Draw();
           }
           else {
               hnStubsPerSSPerLay[i][iLay]->SetLineColor(2);
               hnStubsPerSSPerLay[i][iLay]->Draw("same");
           }
       }
   }


   if (makePlots) {
	   TString outDir("/home/rossin/Dropbox/TT/Work/figures_mergingPatterns/");
	   cRoads        -> SaveAs(outDir+TString("nRoads"       )+TString("_merged")+smergingN+pName+TString(".png"));
	   cCombsPerRoad -> SaveAs(outDir+TString("nCombsPerRoad")+TString("_merged")+smergingN+pName+TString(".png"));
       cCombsPerBX   -> SaveAs(outDir+TString("nCombsPerBX"  )+TString("_merged")+smergingN+pName+TString(".png"));
       cStubsPerSS   -> SaveAs(outDir+TString("nStubsPerSS"  )+TString("_merged")+smergingN+pName+TString(".png"));
   }
}


void roadMergerRun(unsigned iProc=0,unsigned merging=2,bool makePlots=false, unsigned int iEv=100000000,bool debug=false) {

	if (merging !=2 and merging !=4 and merging !=8) return;
	gStyle->SetOptStat(0);
	TString fName; TString pName;
//	TString fName("/data/rossin/EOS/Neutrino_PU140_tt27_sf1_nz4_pt3_20151107/roads_Neutrino_PU140_tt27_sf1_nz4_pt3_5oo6_95c_14k.root"); TString pName("PU140_sf1_nz4");
//	TString fName("/data/rossin/EOS/patternMerging/roads_SingleMuonTest_PU0_tt27_sf1L0x2L5x2_nz4_pt3_5oo6_95c_300M.root"); TString pName("PU0_sf1L0x2L5x2_nz4");
//	TString fName("/data/rossin/EOS/patternMerging/roads_Neutrino_PU140_tt27_sf1L0x2L5x2_nz4_pt3_5oo6_95c_300M_merged.root"); TString pName("PU140_sf1L0x2L5x2_nz4");
	if (iProc==0) {fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz4_pt3_20160308/tracks_LTF_Neutrino_PU140_tt27_sf1L0x2L5x2_nz4_pt3_5oo6_95c_chi100_300M.root"); pName=TString ("PU140_sf1L0x2L5x2_nz4");}
	if (iProc==1) {fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz6_pt3_20160308/tracks_LTF_Neutrino_PU140_tt27_sf1L0x2L5x2_nz6_pt3_5oo6_95c_chi100_300M.root"); pName=TString ("PU140_sf1L0x2L5x2_nz6");}
    if (iProc==2) {fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz8_pt3_20160308/tracks_LTF_Neutrino_PU140_tt27_sf1L0x2L5x2_nz8_pt3_5oo6_95c_chi100_300M.root"); pName=TString ("PU140_sf1L0x2L5x2_nz8");}
//    if (iProc==2) {fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz8_pt3_20160308/roads_Neutrino_PU140_tt27_sf1L0x2L5x2_nz8_pt3_5oo6_95c_300M.root"); pName=TString ("PU140_sf1L0x2L5x2_nz8");}
	if (iProc==3) {fName=TString("/data/rossin/EOS/tt27_sf1L0x2_nz4_pt3_20160308/tracks_LTF_Neutrino_PU140_tt27_sf1L0x2_nz4_pt3_5oo6_95c_chi100_300M.root"        ); pName=TString ("PU140_sf1L0x2_nz4");    }
	if (iProc==4) {fName=TString("/data/rossin/EOS/tt27_sf1L0x2_nz6_pt3_20160308/tracks_LTF_Neutrino_PU140_tt27_sf1L0x2_nz6_pt3_5oo6_95c_chi100_300M.root"        ); pName=TString ("PU140_sf1L0x2_nz6");    }
	if (iProc==5) {fName=TString("/data/rossin/EOS/tt27_sf1L0x2_nz8_pt3_20160308/tracks_LTF_Neutrino_PU140_tt27_sf1L0x2_nz8_pt3_5oo6_95c_chi100_300M.root"        ); pName=TString ("PU140_sf1L0x2_nz8");    }
    if (iProc==6) {fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz4_pt3_20160308/tracks_LTF_Neutrino_PU200_tt27_sf1L0x2L5x2_nz4_pt3_5oo6_95c_chi100_300M.root"); pName=TString ("PU200_sf1L0x2L5x2_nz4");}
    if (iProc==7) {fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz6_pt3_20160308/tracks_LTF_Neutrino_PU200_tt27_sf1L0x2L5x2_nz6_pt3_5oo6_95c_chi100_300M.root"); pName=TString ("PU200_sf1L0x2L5x2_nz6");}
    if (iProc==8) {fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz8_pt3_20160308/tracks_LTF_Neutrino_PU200_tt27_sf1L0x2L5x2_nz8_pt3_5oo6_95c_chi100_300M.root"); pName=TString ("PU200_sf1L0x2L5x2_nz8");}
    if (iProc==9) {fName=TString("/data/rossin/EOS/tt27_sf1L0x2_nz4_pt3_20160308/tracks_LTF_Neutrino_PU200_tt27_sf1L0x2_nz4_pt3_5oo6_95c_chi100_300M.root"        ); pName=TString ("PU200_sf1L0x2_nz4");    }
    if (iProc==10){fName=TString("/data/rossin/EOS/tt27_sf1L0x2_nz6_pt3_20160308/tracks_LTF_Neutrino_PU200_tt27_sf1L0x2_nz6_pt3_5oo6_95c_chi100_300M.root"        ); pName=TString ("PU200_sf1L0x2_nz6");    }
    if (iProc==11){fName=TString("/data/rossin/EOS/tt27_sf1L0x2_nz8_pt3_20160308/tracks_LTF_Neutrino_PU200_tt27_sf1L0x2_nz8_pt3_5oo6_95c_chi100_300M.root"        ); pName=TString ("PU200_sf1L0x2_nz8");    }
    if (iProc==12){fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz4_pt3_20160308/tracks_LTF_Neutrino_PU250_tt27_sf1L0x2L5x2_nz4_pt3_5oo6_95c_chi100_300M.root"); pName=TString ("PU250_sf1L0x2L5x2_nz4");}
    if (iProc==13){fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz6_pt3_20160308/tracks_LTF_Neutrino_PU250_tt27_sf1L0x2L5x2_nz6_pt3_5oo6_95c_chi100_300M.root"); pName=TString ("PU250_sf1L0x2L5x2_nz6");}
    if (iProc==14){fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz8_pt3_20160308/tracks_LTF_Neutrino_PU250_tt27_sf1L0x2L5x2_nz8_pt3_5oo6_95c_chi100_300M.root"); pName=TString ("PU250_sf1L0x2L5x2_nz8");}
    if (iProc==15){fName=TString("/data/rossin/EOS/tt27_sf1L0x2_nz4_pt3_20160308/tracks_LTF_Neutrino_PU250_tt27_sf1L0x2_nz4_pt3_5oo6_95c_chi100_300M.root"        ); pName=TString ("PU250_sf1L0x2_nz4");    }
    if (iProc==16){fName=TString("/data/rossin/EOS/tt27_sf1L0x2_nz6_pt3_20160308/tracks_LTF_Neutrino_PU250_tt27_sf1L0x2_nz6_pt3_5oo6_95c_chi100_300M.root"        ); pName=TString ("PU250_sf1L0x2_nz6");    }
    if (iProc==17){fName=TString("/data/rossin/EOS/tt27_sf1L0x2_nz8_pt3_20160308/tracks_LTF_Neutrino_PU250_tt27_sf1L0x2_nz8_pt3_5oo6_95c_chi100_300M.root"        ); pName=TString ("PU250_sf1L0x2_nz8");    }
    if (iProc==18){fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz8_pt3_20160308/tracks_LTF_Neutrino_PU140_tt27_sf1L0x2L5x2_nz8_pt3_5oo6_95c_300M_removeOverlap0p8_0p8_0p6_0p8_0p6_0p5.root"); pName=TString ("PU140_sf1L0x2L5x2__nz8_OR");}
    if (iProc==19){fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz8_pt3_20160308/tracks_LTF_Neutrino_PU200_tt27_sf1L0x2L5x2_nz8_pt3_5oo6_95c_300M_removeOverlap0p8_0p8_0p6_0p8_0p6_0p5.root"); pName=TString ("PU200_sf1L0x2L5x2__nz8_OR");}
    if (iProc==20){fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz8_pt3_20160308/tracks_LTF_Neutrino_PU250_tt27_sf1L0x2L5x2_nz8_pt3_5oo6_95c_300M_removeOverlap0p8_0p8_0p6_0p8_0p6_0p5.root"); pName=TString ("PU250_sf1L0x2L5x2__nz8_OR");}
    if (iProc==21){fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz6_pt3_20160308/roads_TTbar_PU140_tt27_sf1L0x2L5x2_nz6_pt3_5oo6_95c_300M.root"); pName=TString ("TTbar_PU140_sf1L0x2L5x2_nz6");}
    if (iProc==22){fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz6_pt3_20160308/roads_TTbar_PU200_tt27_sf1L0x2L5x2_nz6_pt3_5oo6_95c_300M.root"); pName=TString ("TTbar_PU200_sf1L0x2L5x2_nz6");}
    if (iProc==23){fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz8_pt3_20160308/roads_TTbar_PU140_tt27_sf1L0x2L5x2_nz8_pt3_5oo6_95c_300M.root"); pName=TString ("TTbar_PU140_sf1L0x2L5x2_nz8");}
    if (iProc==24){fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz8_pt3_20160308/roads_TTbar_PU200_tt27_sf1L0x2L5x2_nz8_pt3_5oo6_95c_300M.root"); pName=TString ("TTbar_PU200_sf1L0x2L5x2_nz8");}
    if (iProc==25){fName=TString("/data/rossin/EOS/tt27_sf1L0x2_nz4_pt3_20160308/roads_TTbar_PU140_tt27_sf1L0x2_nz4_pt3_5oo6_95c_300M.root");         pName=TString ("TTbar_PU140_sf1L0x2_nz4");}
    if (iProc==26){fName=TString("/data/rossin/EOS/tt27_sf1L0x2_nz4_pt3_20160308/roads_TTbar_PU200_tt27_sf1L0x2_nz4_pt3_5oo6_95c_300M.root");         pName=TString ("TTbar_PU200_sf1L0x2_nz4");}
    if (iProc==27){fName=TString("/data/rossin/EOS/tt27_sf1L0x2_nz6_pt3_20160308/roads_TTbar_PU140_tt27_sf1L0x2_nz6_pt3_5oo6_95c_300M.root");         pName=TString ("TTbar_PU140_sf1L0x2_nz6");}
    if (iProc==28){fName=TString("/data/rossin/EOS/tt27_sf1L0x2_nz6_pt3_20160308/roads_TTbar_PU200_tt27_sf1L0x2_nz6_pt3_5oo6_95c_300M.root");         pName=TString ("TTbar_PU200_sf1L0x2_nz6");}
    if (iProc==29){fName=TString("/data/rossin/EOS/tt27_sf1L0x2_nz8_pt3_20160308/roads_TTbar_PU140_tt27_sf1L0x2_nz8_pt3_5oo6_95c_300M.root");         pName=TString ("TTbar_PU140_sf1L0x2_nz8");}
    if (iProc==30){fName=TString("/data/rossin/EOS/tt27_sf1L0x2_nz8_pt3_20160308/roads_TTbar_PU200_tt27_sf1L0x2_nz8_pt3_5oo6_95c_300M.root");         pName=TString ("TTbar_PU200_sf1L0x2_nz8");}

	roadMerger rm(fName);
	rm.Loop(iEv,merging,pName,makePlots, debug, fName);
	return;
}
