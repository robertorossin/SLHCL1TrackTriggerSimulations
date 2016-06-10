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
#include <TPad.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TMath.h>
#include <TRandom.h>
#include <TInterpreter.h>
//#include "/home/rossin/Dropbox/TT/CMSSW_6_2_0_SLHC12_patch1_SL6/src/SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTRoad.h"
#ifdef __CINT__
#pragma link C++ class vector <vector <vector<unsigned> > >;
#endif

#include <iostream>
#include <map>
#include <set>
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
	cout << " nStubs " << endl;
	for (unsigned iSS=0; iSS<road.nStubs.size(); ++iSS) {
	    cout << road.nStubs.at(iSS)  << " : " ;
	    for (unsigned iStub=0; iStub<road.stubRefs.at(iSS).size(); ++iStub) cout << road.stubRefs.at(iSS).at(iStub) << "\t";
	    cout << endl;
	}
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


class mergedPattern {
	unsigned mergedPatternId_;
	vector <unsigned> patternIds_;
	unsigned mergedfrequency_;
	unsigned mergingmultiplicity_;
public:
	mergedPattern(unsigned mergedPatternId, vector <unsigned> &patternIds) {
		mergedPatternId_ = mergedPatternId;
		patternIds_      = patternIds     ;
		mergedfrequency_ = 0;
		mergingmultiplicity_ = 0;
	}
	void addfrequency(unsigned frequency) {mergedfrequency_ += frequency; ++mergingmultiplicity_;}
	unsigned int getmergedPattersId() {return mergedPatternId_;}
	unsigned int getfrequency() {return mergedfrequency_;}
	unsigned int getmergingmultiplicity() {return mergingmultiplicity_;}
	void getpatternIds(vector <unsigned> &patternIds) {patternIds.clear(); for (unsigned ip=0; ip<patternIds_.size();++ip)  patternIds.push_back(patternIds_.at(ip));};
  bool operator < (const mergedPattern& mp) const
  {
      return (mergedfrequency_> mp.mergedfrequency_);
  }

};

class mergedRoad {
	unsigned mergedPatternId_;
	vector <unsigned> patternIds_;
	vector <unsigned> roadIds_;
	vector <vector <unsigned> > SSIds_;
	vector <unsigned> nStubs_;
    vector <vector <unsigned> > stubRefs_;
	float patternInvPt_;
public:
	mergedRoad(unsigned mergedPatternId, unsigned firstPatternId, unsigned firstRoadId, const vector <unsigned> &SSIds0, const vector <unsigned> &nStubs, vector <vector <unsigned> > &stubRefs, float patternInvPt) {
		mergedPatternId_ = mergedPatternId;
		patternIds_.push_back(firstPatternId);
		roadIds_   .push_back(firstRoadId);
		for (unsigned iLay=0; iLay<SSIds0.size(); ++iLay) {
			vector <unsigned> v(0);
			v.push_back(SSIds0.at(iLay));
			SSIds_.push_back(v);
			stubRefs_.push_back(stubRefs.at(iLay));
		};
		nStubs_          = nStubs;
		patternInvPt_    = patternInvPt;
        for (unsigned iLay=0; iLay<stubRefs_.size(); ++iLay) {
            if (stubRefs_.at(iLay).size() != nStubs_.at(iLay)) {
                cout << "####\t" << iLay << "\t" << stubRefs_.at(iLay).size() << "\t" << nStubs_.at(iLay) << endl;
            }
        }
	}
	void setPatternsIds(vector         <unsigned> patternIds) {patternIds_=patternIds;
	cout << " patternIds_ "<< patternIds_.size(); for (unsigned i=0; i<patternIds_.size(); ++i) cout << " " <<  patternIds_.at(i) ; cout << endl;
	}
	void setRoadIds    (vector         <unsigned>    roadIds)     {   roadIds_=   roadIds; cout << " roadIds_    "<< roadIds_   .size() <<endl;}
	void setSSIds      (vector <vector <unsigned> >    SSIds)     {     SSIds_=     SSIds; cout << " SSIds_      "<< SSIds_     .size() <<endl;}
	void setNstubs     (vector         <unsigned>     nStubs)     {    nStubs_=    nStubs; cout << " nStubs_     "<< nStubs_    .size() <<endl;}
	void getNstubs     (vector         <unsigned>    &nStubs)     {    nStubs    .clear(); nStubs =   nStubs_;}
	void getStubRefs   (vector <vector <unsigned> >  &vStubs)     {    vStubs    .clear(); vStubs =   stubRefs_;}
	void getSSIds      (vector <vector <unsigned> >  &vSSIds)     {    vSSIds    .clear(); vSSIds =   SSIds_;}
	void getPatternsIds(vector         <unsigned>    &patternIds) {    patternIds.clear(); patternIds =   patternIds_;}
	void add           (TTRoad road) {
		unsigned nDifferentLayers   = 0;
		unsigned siblingPhiDistance = 0;
		unsigned siblingZDistance   = 0;
		for (unsigned iLay=0; iLay<road.superstripIds.size(); ++iLay) {
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
				if (road.nStubs.at(iLay) != road.stubRefs.at(iLay).size()) cout << "EHM EHM" << endl;
				for (unsigned iStub=0; iStub<road.stubRefs.at(iLay).size(); ++iStub) {
					stubRefs_.at(iLay).push_back(road.stubRefs.at(iLay).at(iStub));
				}
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
		for (unsigned iLay=0; iLay<road.stubRefs.size(); ++iLay) {
		    if (stubRefs_.at(iLay).size() != nStubs_.at(iLay)) {
		        cout << iLay << "\t" << stubRefs_.at(iLay).size() << "\t" << nStubs_.at(iLay) << endl;
		        cout << "CAZZAROLA. STO CAPENDO UNA CIPPAZZA" << endl;
		    }
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
		cout << " nStubs_ " << endl;
		for (unsigned ssid = 0; ssid<nLay; ++ssid) {
		    cout << nStubs_.at(ssid) << " : " ;
		    for (unsigned iStub=0; iStub<stubRefs_.at(ssid).size(); ++iStub) cout << stubRefs_.at(ssid).at(iStub) << "\t";
		    cout << endl;
		}
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

	bool isFullMerge =0;
	if (pName.Contains("_100c")) isFullMerge=1;

	TFile* fmergedPatt = 0;
	if (pName.Contains("sf1L0x2L5x2_nz4")) {
		if (merging==2) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m_patternBank_tt27_sf1L0x2L5x2_nz4_pt3_300M.root" ,"READ");
		if (merging==4) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m4_patternBank_tt27_sf1L0x2L5x2_nz4_pt3_300M.root","READ");
		if (merging==8) {
			if (!isFullMerge) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m8_patternBank_tt27_sf1L0x2L5x2_nz4_pt3_95c_250M.root","READ");
			else              fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m8_patternBank_tt27_sf1L0x2L5x2_nz4_pt3_100c_250M.root","READ");
		}
	}
	if (pName.Contains("sf1L0x2L5x2_nz6")) {
		if (merging==2) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m_patternBank_tt27_sf1L0x2L5x2_nz6_pt3_300M.root"  ,"READ");
		if (merging==4) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m4_patternBank_tt27_sf1L0x2L5x2_nz6_pt3_300M.root" ,"READ");
		if (merging==8) {
			if (!isFullMerge) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m8_patternBank_tt27_sf1L0x2L5x2_nz6_pt3_95c_250M.root" ,"READ");
			else              fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m8_patternBank_tt27_sf1L0x2L5x2_nz6_pt3_100c_250M.root" ,"READ");
		}

	}
	if (pName.Contains("sf1L0x2L5x2_nz8")) {
		if (merging==2) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m_patternBank_tt27_sf1L0x2L5x2_nz8_pt3_300M.root" ,"READ");
		if (merging==4) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m4_patternBank_tt27_sf1L0x2L5x2_nz8_pt3_300M.root" ,"READ");
		if (merging==8) {
			if (!isFullMerge) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m8_patternBank_tt27_sf1L0x2L5x2_nz8_pt3_95c_250M.root" ,"READ");
			else              fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m8_patternBank_tt27_sf1L0x2L5x2_nz8_pt3_100c_250M.root" ,"READ");
		}
	}
	if (pName.Contains("sf1L0x2L5x2__nz8_OR")) {
		if (merging==4) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m4_patternBank_tt27_sf1L0x2L5x2_nz8_pt3_300M_removeOverlap0p8_0p8_0p6_0p8_0p6_0p5.root" ,"READ");
		if (merging==8) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m8_patternBank_tt27_sf1L0x2L5x2_nz8_pt3_300M_removeOverlap0p8_0p8_0p6_0p8_0p6_0p5.root" ,"READ");
	}
	if (pName.Contains("sf1L0x2_nz4"    )) {
		if (merging==2) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m_patternBank_tt27_sf1L0x2_nz4_pt3_300M.root"  ,"READ");
		if (merging==4) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m4_patternBank_tt27_sf1L0x2_nz4_pt3_300M.root" ,"READ");
		if (merging==8) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m8_patternBank_tt27_sf1L0x2_nz4_pt3_100c_300M.root" ,"READ");
	}
	if (pName.Contains("sf1L0x2_nz6"    )) {
		if (merging==2) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m_patternBank_tt27_sf1L0x2_nz6_pt3_300M.root"  ,"READ");
		if (merging==4) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m4_patternBank_tt27_sf1L0x2_nz6_pt3_300M.root" ,"READ");
		if (merging==8) {
			if (!isFullMerge) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m8_patternBank_tt27_sf1L0x2_nz6_pt3_300M.root" ,"READ");
			else              fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m8_patternBank_tt27_sf1L0x2_nz6_pt3_100c_300M.root" ,"READ");
		}
	}
	if (pName.Contains("sf1L0x2_nz8"    )) {
		if (merging==2) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m_patternBank_tt27_sf1L0x2_nz8_pt3_300M.root"  ,"READ");
		if (merging==4) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m4_patternBank_tt27_sf1L0x2_nz8_pt3_300M.root" ,"READ");
		if (merging==8) {
			if (!isFullMerge) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m8_patternBank_tt27_sf1L0x2_nz8_pt3_300M.root" ,"READ");
			else              fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m8_patternBank_tt27_sf1L0x2_nz8_pt3_100c_300M.root" ,"READ");
		}
	}
	//   if (pName.Contains("sf1_nz4"    )) {
	//	   if (merging==4) fmergedPatt = new TFile("/data/rossin/EOS/patternMerging/m4_patternBank_tt27_sf1_nz4_pt3.root" ,"READ");
	//   }
	if (!fmergedPatt->IsOpen()) {
		std::cout << "ERROR. Can not open pattern file. Exiting..." << std::endl;
		return;
	}
   TTree *patternBankInfo   = (TTree*) fmergedPatt->Get("patternBankInfo");
   float totalBankCoverage = 0;
   patternBankInfo->SetBranchAddress("coverage"  ,&totalBankCoverage);
   patternBankInfo->GetEntry(0);
   std::cout << "Total coverage original bank = " << totalBankCoverage << std::endl;
   TTree *patternBank       = (TTree*) fmergedPatt->Get("patternBank");
   unsigned int nTotPatterns = patternBank->GetEntries();
   TTree *  toMerged     = (TTree*) fmergedPatt->Get(  "toMerged");
   TTree *fromMerged     = (TTree*) fmergedPatt->Get("fromMerged");
   vector  <unsigned int>   *indToMerged  = 0;
   vector  <unsigned int>   *indFromMerged= 0;
   toMerged  ->SetBranchAddress("indToMerged"  ,&indToMerged  );
   fromMerged->SetBranchAddress("indFromMerged",&indFromMerged);
   toMerged->GetEntry(0);
   if (indToMerged->size()<=nTotPatterns) std::cout << " # pattern indices loaded = " << indToMerged->size() << std::endl;
   else                                  {
	   std::cout << " ERROR. # loaded indices > # patterns in full bank. Exiting..." << std::endl;
	   return;
   }

   if (isFullMerge) std::cout << "Running on merged bank from FULL coverage original bank" << std::endl;
   else             std::cout << "Running on merged bank from 95% coverage original bank"  << std::endl;

   unsigned totalFreq       = 0;
   unsigned totalMergedFreq = 0;
   float targetCoverage = 0.95;
   unsigned bankSizeAtTargetCoverage = 0;
   unsigned mergedbankSizeAtTargetCoverage = 0;

	 const char  *catt [] = {"invPt_mean", "cotTheta_mean", "phi_mean", "z0_mean", "frequency"};
	 const char  *cbank[] = {"unmerged", "merged"};
	 const char  *ccov [] = {"full", "95"};
	 const unsigned nbins[] = {100,100,100,100,100};
	 const double minflim [] = {-0.4, -1.0, 0.0,-15.0,   0};
	 const double maxflim [] = { 0.4,  1.0, 2.0, 15.0, 100};
	 const double deltalim[] = { 0.1,  1.0,0.05, 10.0 , 100};
   TH1F* hpatternAttributes[5][2][2]; // 5 attributes; 2 banks; 2 coverages:100% and 95%
   TH1F* hpatternDeltaAttributes[5][2]; // 5 attributes; 2 banks; 2 coverages:100% and 95%
   for (unsigned iatt=0; iatt<5; ++iatt) {
		 char cc[100];
		 for (unsigned icov=0; icov<2; ++icov) {
			 for (unsigned ibank=0; ibank<2; ++ibank) {
    		 sprintf(cc,"hpatternAttributes_%s_%s_%s",catt[iatt],cbank[ibank],ccov[icov]);
  			 hpatternAttributes[iatt][ibank][icov]=new TH1F(cc,cc,nbins[iatt],minflim[iatt],maxflim[iatt]);
  		 }
  		 sprintf(cc,"hpatternDeltaAttributes_%s_%s_%s",catt[iatt],cbank[1],ccov[icov]);
			 hpatternDeltaAttributes[iatt][icov]=new TH1F(cc,cc,nbins[iatt],-deltalim[iatt],deltalim[iatt]);
  	 }
   }

   TTree *patternAttributes = (TTree*) fmergedPatt->Get("patternAttributes");
   vector <std::array<float, 5> >     vpatternAttributes;
   vector <unsigned short int> vpatternFreq;
   std::array<float, 5> apatternAttributes;
      float patternInvPt = 0;
   //   float patternCotTheta = 0;
//   float patternPhi      = 0;
//   float patternZ0       = 0;
   unsigned short int frequency;
   patternAttributes->SetBranchAddress("invPt_mean"    ,&apatternAttributes[0]);
   patternAttributes->SetBranchAddress("cotTheta_mean" ,&apatternAttributes[1]);
   patternAttributes->SetBranchAddress("phi_mean"      ,&apatternAttributes[2]);
   patternAttributes->SetBranchAddress("z0_mean"       ,&apatternAttributes[3]);

   patternBank      ->SetBranchAddress("frequency"     ,&frequency   );
   for (unsigned iPatt=0; iPatt<patternAttributes->GetEntries(); ++iPatt) {
       patternAttributes->GetEntry(iPatt);
       patternBank      ->GetEntry(iPatt);
       apatternAttributes[4] = frequency;
       vpatternAttributes.push_back(apatternAttributes);
       vpatternFreq .push_back(frequency);
       totalFreq += frequency;
       for (unsigned iatt=0; iatt<5; ++iatt) {
      	 float xmax = hpatternAttributes[iatt][0][0]->GetXaxis()->GetXmax()-0.001;
      	 float xfill = TMath::Min(apatternAttributes[iatt],xmax);
      	 hpatternAttributes[iatt][0][0]->Fill(xfill);
       }
       if (iPatt%500000==0) cout << "Reading patternAttributes " << iPatt << endl;
   }

   std::cout << std::endl << "Processed configuration: " <<  pName << std::endl;
   unsigned targetFreq = 0;
   for (unsigned iPatt=0; iPatt<patternAttributes->GetEntries(); ++iPatt) {
  	 targetFreq+=vpatternFreq.at(iPatt);
     for (unsigned iatt=0; iatt<5; ++iatt) {
    	 float xmax = hpatternAttributes[iatt][0][1]->GetXaxis()->GetXmax()-0.001;
    	 float xfill = TMath::Min(vpatternAttributes.at(iPatt)[iatt],xmax);
    	 hpatternAttributes[iatt][0][1]->Fill(xfill);
     }
  	 if (totalBankCoverage*targetFreq/totalFreq>targetCoverage) {
  		 bankSizeAtTargetCoverage = iPatt;
  		 std::cout << "Coverage " << totalBankCoverage*targetFreq/totalFreq<< " reached with " << iPatt<< " unmerged patterns" << std::endl;
  		 break;
  	 }
   }

   TH1F* hXMerged = new TH1F("hXMerged","hXMerged",8,1,9);
   vector <vector<unsigned int> > vindFromMerged;
   vector <mergedPattern> vmergedPatterns;
   unsigned nMerged = (unsigned)fromMerged->GetEntries();
   for (unsigned iMerged=0; iMerged<nMerged;++iMerged) {
	   fromMerged->GetEntry(iMerged);
	   vindFromMerged.push_back(*indFromMerged);
	   mergedPattern mp(iMerged,*indFromMerged);
	   for (unsigned imerged=0; imerged<indFromMerged->size(); ++imerged) {
	  	 mp.addfrequency   (vpatternFreq.at(indFromMerged->at(imerged)));
	  	 totalMergedFreq += vpatternFreq.at(indFromMerged->at(imerged));
	   }
	   if (mp.getmergingmultiplicity() != indFromMerged->size()) std::cout << mp.getmergingmultiplicity() << "\t" <<  indFromMerged->size() <<endl;
	   vmergedPatterns.push_back(mp);
//	   if (iMerged%100==0) cout << iMerged << "\t" << indFromMerged->size() << endl;
   }
   std::cout << " # merged pattern indices loaded = " << vindFromMerged.size() << std::endl;
   std::cout << "Total pattern frequency        = " << totalFreq << std::endl;
   std::cout << "Total merged pattern frequency = " << totalMergedFreq << std::endl;

   std::set<unsigned> patternIdset;
   std::set<unsigned>::iterator it_patternIdset;
   std::pair<std::set<unsigned>::iterator,bool> ret_patternIdset;
   std::set<unsigned> mergedpatternIdset;

   unsigned int partialMergedFreq = 0;
   std::sort(vmergedPatterns.begin(), vmergedPatterns.end()); // reversed sorting by merged frequency. From highest to lowest
   for (unsigned iMerged=0; iMerged<vmergedPatterns.size();++iMerged) {
  	 mergedPattern mp=vmergedPatterns.at(iMerged);
  	 mergedpatternIdset.insert(mp.getmergedPattersId()); // keep track of merged patterns for given coverage
  	 partialMergedFreq+=mp.getfrequency();
  	 hXMerged->Fill(mp.getmergingmultiplicity());
  	 vector<unsigned> patternIds;
  	 mp.getpatternIds(patternIds);
  	 for (unsigned ip=0; ip<patternIds.size(); ++ip) {
  		 ret_patternIdset = patternIdset.insert(patternIds[ip]);
  		 if (ret_patternIdset.second==false) {
  			 std::cout << "ERROR. DUPLICATED PATTERN IN BANK" << std::endl;
  			 return;
  		 }
       for (unsigned iatt=0; iatt<5; ++iatt) {
      	 float xmax = hpatternAttributes[iatt][1][1]->GetXaxis()->GetXmax()-0.001;
      	 float xfill = TMath::Min(vpatternAttributes.at(patternIds[ip])[iatt],xmax);
      	 hpatternAttributes[iatt][1][1]->Fill(xfill);
      	 if (iatt<4) hpatternDeltaAttributes[iatt][1]->Fill(vpatternAttributes.at(patternIds[ip])[iatt]-vpatternAttributes.at(patternIds[0])[iatt]);
       }
  	 }

//  	 if (iMerged%10000==0) std::cout << iMerged << "\t" << mp.getfrequency() << std::endl;
  	 float reachedCoverage = totalBankCoverage*partialMergedFreq/totalFreq;
  	 if (reachedCoverage>targetCoverage) {
  		 mergedbankSizeAtTargetCoverage = iMerged;
  		 std::cout << "Coverage " << reachedCoverage << " reached with " << mergedbankSizeAtTargetCoverage<< " merged patterns" << std::endl;
  		 std::cout << "Bank reduced to " << 100.0*mergedbankSizeAtTargetCoverage/bankSizeAtTargetCoverage<< "% wrt the original bank" << std::endl;
  		 std::cout << "Merged pattern bank made of " << patternIdset.size() << " single SS patterns" << std::endl;
  		 break;
  	 }
   }
   long unsigned anPatterns[]={bankSizeAtTargetCoverage,mergedbankSizeAtTargetCoverage};

   TCanvas* cAttributes = new TCanvas("cAttributes","cAttributes",0,0,900,900);
   cAttributes->Divide(2,3);
   for (unsigned iatt=0; iatt<5; ++iatt) {
  	 cAttributes->cd(iatt+1);
  	 if (iatt==4) gPad->SetLogy();
  	 hpatternAttributes[iatt][0][0]->Draw();
  	 hpatternAttributes[iatt][0][1]->SetLineColor(2);
//  	 hpatternAttributes[iatt][0][1]->SetLineWidth(3);
  	 hpatternAttributes[iatt][0][1]->Draw("same");
  	 hpatternAttributes[iatt][1][1]->SetLineColor(8);
  	 hpatternAttributes[iatt][1][1]->Draw("same");
   }

   TCanvas* cDeltaAttributes = new TCanvas("cDeltaAttributes","cDeltaAttributes",0,0,900,900);
   cDeltaAttributes->Divide(2,2);
   for (unsigned iatt=0; iatt<4; ++iatt) {
  	 cDeltaAttributes->cd(iatt+1);
  	 hpatternDeltaAttributes[iatt][1]->Draw();
  	 hpatternDeltaAttributes[iatt][1]->SetLineColor(2);
//  	 hpatternDeltaAttributes[iatt][1]->Draw("same");
//  	 hpatternDeltaAttributes[iatt][1]->SetLineColor(6);
//  	 hpatternDeltaAttributes[iatt][1]->Draw("same");
   }

   TCanvas* cXMerged = new TCanvas("cXMerged","cXMerged",0,0,600,600);
   hXMerged->SetFillColor(5);
   hXMerged->DrawNormalized();

//   return;
   // ******************************************************************



   unsigned maxRoads[2] = {0,0};
   unsigned maxCombs[2] = {0,0};
   TH1F* hLogic        [2];
   TH1F* hnSS          [2];
   TH1F* hnStubs       [2];
   TH1F* hnRoads       [2];
   TH1F* hnCombsPerRoad[2];
   TH1F* hnCombsPerBX  [2];
   TH1F* hnStubsPerSSPerLay [2][8];
   TString smergingN("");
   if (merging==2) smergingN=TString("_x2_");
   if (merging==4) smergingN=TString("_x4_");
   if (merging==8) smergingN=TString("_x8_");
   if (isFullMerge) smergingN=smergingN+TString("FM_");

   for (unsigned i=0; i<2; ++i) {
  	 TString hname("roadLogic");
  	 if (i) hname+=TString("Merged")+smergingN;
  	 hLogic       [i] = new TH1F(hname,hname,8,0,8);
  	 hname = TString("hnSS");
  	 if (i) hname+=TString("Merged")+smergingN;
  	 hnSS         [i] = new TH1F(hname,hname,8,0,8);
  	 hname = TString("hnStubs");
  	 if (i) hname+=TString("Merged")+smergingN;
  	 hnStubs      [i] = new TH1F(hname,hname,8,0,8);
  	 hname = TString("nRoads");
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
   TString newFileName(fName.ReplaceAll(".root",smergingN+TString(".root")));
   TFile *newfile = new TFile(newFileName,"recreate");
   fChain->SetBranchStatus("AMTTRoads_patternRef",0); // need to remake this because the fitter defines nroads = AMTTRoads_patternRef.size()
   fChain->SetBranchStatus("AMTTRoads_stubRefs",0);
   fChain->SetBranchStatus("AMTTRoads_nstubs",0);
   newfile->mkdir("ntupler")->cd();
   TTree *newtree = fChain->CloneTree(0,"fast");
   fChain->SetBranchStatus("AMTTRoads*",1);
   vector        <unsigned>   patternRefs;
   vector        <unsigned>   nStubs;
   vector <vector <vector<unsigned> > > stubRefs;
   newtree->Branch("AMTTRoads_patternRef", &patternRefs);
   newtree->Branch("AMTTRoads_nstubs"    , &nStubs  );
   newtree->Branch("AMTTRoads_stubRefs"  , &stubRefs);

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

	  std::set <unsigned> SSids        [6];
	  std::set <unsigned> stubsPerLayer[6];

	  patternRefs.clear();
	  nStubs.clear();
	  stubRefs.clear();
	  std::map <unsigned, mergedRoad>              mapMergedRoads;
	  std::map <unsigned, mergedRoad>::iterator it_mapMergedRoads;
	  unsigned nRoadsByPt = 0;
	  for (unsigned int iRoads=0; iRoads<nRoads; ++iRoads) { // LOOPING OVER ROADS in EVENT
		  TTRoad road;
		  road.patternRef     = AMTTRoads_patternRef   ->at(iRoads);
		  road.stubRefs       = AMTTRoads_stubRefs     ->at(iRoads);
		  road.superstripIds  = AMTTRoads_superstripIds->at(iRoads);
//		  	road.nstubs         = AMTTRoads_nstubs       ->at(iRoads);
//		  patternInvPt = vpatternInvPt.at(road.patternRef);
//          if (TMath::Abs(1.0/patternInvPt)>5.0 || TMath::Abs(1.0/patternInvPt)<0.0) continue;
		  if (road.patternRef <= bankSizeAtTargetCoverage) for (unsigned il=0; il<road.superstripIds.size(); ++il) SSids[il].insert(road.superstripIds.at(il));
//		  for (unsigned il=0; il<road.superstripIds.size(); ++il) SSids[il].insert(road.superstripIds.at(il));
		  vector <unsigned> nStubsPerLayer;
		  unsigned maxNstubPerLayer=0;
		  unsigned maxLayermaxNstub=7;
		  unsigned logic = 0;
		  for (unsigned iLay=0; iLay<nLay; ++iLay) {
		  	unsigned nStubsPLPR = road.stubRefs.at(iLay).size();
		  	if (road.patternRef <= bankSizeAtTargetCoverage) for (unsigned iStub=0; iStub<nStubsPLPR; ++iStub) stubsPerLayer[iLay].insert(road.stubRefs.at(iLay).at(iStub));
		  	if (nStubsPLPR) ++logic;
		  	if (maxNstubPerLayer < nStubsPLPR) {
		  		maxNstubPerLayer = nStubsPLPR;
		  		maxLayermaxNstub = iLay;
		  	}
		  	hnStubsPerSSPerLay[0][iLay]->Fill(nStubsPLPR);
		  	nStubsPerLayer.push_back(nStubsPLPR);
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
		  if (road.patternRef <= bankSizeAtTargetCoverage) {
		  	++nRoadsByPt;
		  	unsigned nCombRoad       = road.getCombinations();
		  	hnCombsPerRoad[0]->Fill(nCombRoad);
		  	nCombsPerBX       += nCombRoad;


		  }

		  unsigned mergedPatternRef = indToMerged->at(road.patternRef);
		  //		  	unsigned nMerged = vindFromMerged.at(mergedPatternRef).size();
		  it_patternIdset = mergedpatternIdset.find(mergedPatternRef);
		  if (it_patternIdset == mergedpatternIdset.end()) continue; // check if merged pattern is in given coverage
//		  mergedRoad mr(mergedPatternRef,road.patternRef,iRoads, road.superstripIds,nStubsPerLayer,patternInvPt);
		  mergedRoad mr(mergedPatternRef,road.patternRef,iRoads, road.superstripIds,nStubsPerLayer,road.stubRefs,patternInvPt);
		  //		  	mr.setPatternsIds();
		  //		  	for (unsigned iMerged=0; iMerged<nMerged ; ++iMerged) {
		  //		  		unsigned patternRef       = vindFromMerged.at(mergedPatternRef).at(iMerged);
		  //		  		mr.setPatternsIds(vindFromMerged.at(mergedPatternRef));
		  //		  	}
		  it_mapMergedRoads = mapMergedRoads.find(mergedPatternRef);
		  if (it_mapMergedRoads == mapMergedRoads.end()) mapMergedRoads.insert(make_pair(mergedPatternRef,mr));
		  else                                           it_mapMergedRoads->second.add(road);

		  hLogic[0]->Fill(logic);
	  }
	  for (unsigned iLay=0; iLay<nLay; ++iLay) {
	  	hnSS    [0]->Fill(iLay,SSids        [iLay].size());
	  	hnStubs [0]->Fill(iLay,stubsPerLayer[iLay].size());
	  }

	  for (unsigned iLay=0; iLay<nLay; ++iLay) {
	  	SSids        [iLay].clear();
	  	stubsPerLayer[iLay].clear();
	  }

	  for (it_mapMergedRoads=mapMergedRoads.begin(); it_mapMergedRoads!=mapMergedRoads.end(); ++it_mapMergedRoads) {
		  mergedRoad mr = it_mapMergedRoads->second;
//		  if (TMath::Abs(1.0/mr.getRoadInvPt())>5) cout << "MMMHHHH" << endl;
		  unsigned nCombRoadMerged = mr.getCombinations();
		  hnCombsPerRoad[1]->Fill(nCombRoadMerged);
		  nCombsPerBXMerged += nCombRoadMerged;
		  if (debug) mr.print();
		  vector <vector<unsigned> > nstubRefs;
//		  nstubRefs.clear();
//		  vector        <unsigned>   nnStubs;
//		  mr.getNstubs(nnStubs);
		  vector <vector <unsigned> >  vSSIds;
		  mr.getSSIds(vSSIds);
		  mr.getStubRefs(nstubRefs);
		  unsigned nStubsPerRoad = 0;
		  for (unsigned iLay=0; iLay<nstubRefs.size(); ++iLay) {
		  	for (unsigned iSS=0; iSS<vSSIds.at(iLay).size(); ++iSS) SSids        [iLay].insert(vSSIds.at(iLay).at(iSS));
//		  	cout << iLay << "\t" << vSSIds.at(iLay).size() << endl;
		  	unsigned nStubsPLPR = nstubRefs.at(iLay).size();
		  	nStubsPerRoad +=      nStubsPLPR;
		  	for (unsigned iStub=0; iStub<nStubsPLPR; ++iStub) stubsPerLayer[iLay].insert(nstubRefs.at(iLay).at(iStub));
		  }
//	      nStubsPerRoad += nnStubs.size();
		  vector <unsigned> patternIds;
		  mr.getPatternsIds(patternIds);
		  if (!patternIds.size()) {
		  	std::cout << "ERROR. Zero size merged road. keep debugging man!" << std::endl;
		  	return;
		  }
		  patternRefs.push_back(patternIds.at(0));
		  nStubs.push_back(nStubsPerRoad);
		  stubRefs.push_back(nstubRefs);
//          for (unsigned iLay=0; iLay<nnStubs.size(); ++iLay) {
//              if (nstubRefs.at(iLay).size() != nnStubs.at(iLay)) {
//                  cout << iLay << "\t" << nstubRefs.at(iLay).size() << "\t" << nnStubs.at(iLay) << endl;
//                  cout << "CAZZAROLA. NON STO CAPENDO UNA CIPPOZZA" << endl;
//              }
//          }
		  vector <unsigned> nStubsPerLayer;
		  unsigned maxNstubPerLayer=0;
		  unsigned maxLayermaxNstub=7;
		  unsigned logic = 0;
		  for (unsigned iLay=0; iLay<nLay; ++iLay) {
		  	unsigned nStubsPL = nstubRefs.at(iLay).size();
		  	if (nStubsPL) ++logic;
		  	if (maxNstubPerLayer < nStubsPL) {
		  		maxNstubPerLayer = nStubsPL;
		  		maxLayermaxNstub = iLay;
		  	}
		  	hnStubsPerSSPerLay[1][iLay]->Fill(nStubsPL);
		  	nStubsPerLayer.push_back(nStubsPL);
		  }
		  hnStubsPerSSPerLay[1][6]->Fill(maxNstubPerLayer);
		  hnStubsPerSSPerLay[1][7]->Fill(maxLayermaxNstub);
		  hLogic[1]->Fill(logic);
	  }
	  for (unsigned iLay=0; iLay<nLay; ++iLay) {
	  	hnSS    [1]->Fill(iLay,SSids        [iLay].size());
	  	hnStubs [1]->Fill(iLay,stubsPerLayer[iLay].size());
	  }

	  newtree->Fill();
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
   TCanvas* cSSStubs      = new TCanvas("cSSStubs"     ,"cSSStubs"     ,0  ,0  ,600,900);
   TCanvas* cLogic        = new TCanvas("cLogic"       ,"cLogic"       ,0  ,0  ,600,600);
   TCanvas* cRoads        = new TCanvas("cRoads"       ,"cRoads"       ,0  ,0  ,600,600);
   TCanvas* cCombsPerRoad = new TCanvas("cCombsPerRoad","cCombsPerRoad",100,100,700,600);
   TCanvas* cCombsPerBX   = new TCanvas("cCombsPerBX"  ,"cCombsPerBX"  ,200,200,700,600);
   cSSStubs->Divide(2,1);
   cSSStubs->cd(1);
   gPad->SetGrid();
   hnSS   [1]->Draw();
   hnSS   [0]->SetLineWidth(2);
   hnSS   [0]->SetLineColor(1);
   hnSS   [0]->Draw("same");
   hnSS   [1]->SetLineWidth(2);
   hnSS   [1]->SetLineColor(2);
   hnSS   [1]->Draw("same");
   cSSStubs->cd(2);
   gPad->SetGrid();
   hnStubs[1]->Draw();
   hnStubs[0]->SetLineWidth(2);
   hnStubs[0]->SetLineColor(1);
   hnStubs[0]->Draw("same");
   hnStubs[1]->SetLineWidth(2);
   hnStubs[1]->SetLineColor(2);
   hnStubs[1]->Draw("same");

   cLogic->cd();
   hLogic[0]->SetLineWidth(2);
   hLogic[0]->SetLineColor(1);
   hLogic[0]->Draw();
   hLogic[1]->SetLineWidth(2);
   hLogic[1]->SetLineColor(2);
   hLogic[1]->Draw("same");

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
	   cout << hnCombsPerRoad[i]->GetTitle() << "\t" << label << " max= " <<  0.0 << "\t nPatterns= " << anPatterns[i]  << endl;
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

//   TCanvas* cStubsPerSS = new TCanvas("cStubsPerSS","cStubsPerSS",0,0,900,900);
//   cStubsPerSS->Divide(3,3);
//   for (unsigned iLay=0; iLay<8; ++iLay) {
//       cStubsPerSS->cd(iLay+1);
//       if (iLay!=7) gPad->SetLogy();
//       gPad->SetGrid();
//       for (unsigned i=0; i<2; ++i) {
//           if (i==0) {
//               hnStubsPerSSPerLay[i][iLay]->SetLineColor(1);
//               hnStubsPerSSPerLay[i][iLay]->Draw();
//           }
//           else {
//               hnStubsPerSSPerLay[i][iLay]->SetLineColor(2);
//               hnStubsPerSSPerLay[i][iLay]->Draw("same");
//           }
//       }
//   }
//
//

   newtree->Write();
   newfile->Close();


   if (makePlots) {
	   TString outDir("/home/rossin/Dropbox/TT/Work/figures_mergingPatterns/");
	   cSSStubs         -> SaveAs(outDir+TString("nSSandStubs"       )+TString("_merged")+smergingN+pName+TString(".png"));
	   cLogic           -> SaveAs(outDir+TString("roadLogic"         )+TString("_merged")+smergingN+pName+TString(".png"));
	   cXMerged         -> SaveAs(outDir+TString("xMerging"          )+TString("_merged")+smergingN+pName+TString(".png"));
	   cAttributes      -> SaveAs(outDir+TString("patternAttrib"     )+TString("_merged")+smergingN+pName+TString(".png"));
	   cDeltaAttributes -> SaveAs(outDir+TString("patternDeltaAttrib")+TString("_merged")+smergingN+pName+TString(".png"));
	   cRoads           -> SaveAs(outDir+TString("nRoads"            )+TString("_merged")+smergingN+pName+TString(".png"));
	   cCombsPerRoad    -> SaveAs(outDir+TString("nCombsPerRoad"     )+TString("_merged")+smergingN+pName+TString(".png"));
	   cCombsPerBX      -> SaveAs(outDir+TString("nCombsPerBX"       )+TString("_merged")+smergingN+pName+TString(".png"));
//       cStubsPerSS   -> SaveAs(outDir+TString("nStubsPerSS"  )+TString("_merged")+smergingN+pName+TString(".png"));
   }
}


void patternMergerRun(unsigned iProc=0,unsigned merging=2,bool makePlots=false, unsigned int iEv=100000000,bool debug=false) {

	if (merging !=2 and merging !=4 and merging !=8) return;
	gStyle->SetOptStat(1);
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
	if (iProc==21){fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz6_pt3_20160308/roads_TTbar_PU140_tt27_sf1L0x2L5x2_nz6_pt3_5oo6_95c_300M.root");  pName=TString ("TTbar_PU140_sf1L0x2L5x2_nz6");}
	if (iProc==22){fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz6_pt3_20160308/roads_TTbar_PU200_tt27_sf1L0x2L5x2_nz6_pt3_5oo6_95c_300M.root");  pName=TString ("TTbar_PU200_sf1L0x2L5x2_nz6");}
	if (iProc==23){fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz8_pt3_20160308/roads_TTbar_PU140_tt27_sf1L0x2L5x2_nz8_pt3_5oo6_95c_300M.root");  pName=TString ("TTbar_PU140_sf1L0x2L5x2_nz8");}
	if (iProc==24){fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz8_pt3_20160308/roads_TTbar_PU200_tt27_sf1L0x2L5x2_nz8_pt3_5oo6_95c_300M.root");  pName=TString ("TTbar_PU200_sf1L0x2L5x2_nz8");}
	if (iProc==25){fName=TString("/data/rossin/EOS/tt27_sf1L0x2_nz4_pt3_20160308/roads_TTbar_PU140_tt27_sf1L0x2_nz4_pt3_5oo6_95c_300M.root");          pName=TString ("TTbar_PU140_sf1L0x2_nz4");}
	if (iProc==26){fName=TString("/data/rossin/EOS/tt27_sf1L0x2_nz4_pt3_20160308/roads_TTbar_PU200_tt27_sf1L0x2_nz4_pt3_5oo6_95c_300M.root");          pName=TString ("TTbar_PU200_sf1L0x2_nz4");}
	if (iProc==27){fName=TString("/data/rossin/EOS/tt27_sf1L0x2_nz6_pt3_20160308/roads_TTbar_PU140_tt27_sf1L0x2_nz6_pt3_5oo6_95c_300M.root");          pName=TString ("TTbar_PU140_sf1L0x2_nz6");}
	if (iProc==28){fName=TString("/data/rossin/EOS/tt27_sf1L0x2_nz6_pt3_20160308/roads_TTbar_PU200_tt27_sf1L0x2_nz6_pt3_5oo6_95c_300M.root");          pName=TString ("TTbar_PU200_sf1L0x2_nz6");}
	if (iProc==29){fName=TString("/data/rossin/EOS/tt27_sf1L0x2_nz8_pt3_20160308/roads_TTbar_PU140_tt27_sf1L0x2_nz8_pt3_5oo6_95c_300M.root");          pName=TString ("TTbar_PU140_sf1L0x2_nz8");}
	if (iProc==30){fName=TString("/data/rossin/EOS/tt27_sf1L0x2_nz8_pt3_20160308/roads_TTbar_PU200_tt27_sf1L0x2_nz8_pt3_5oo6_95c_300M.root");          pName=TString ("TTbar_PU200_sf1L0x2_nz8");}
	if (iProc==31){fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz4_pt3_20160308/roads_TTbar_PU140_tt27_sf1L0x2L5x2_nz4_pt3_5oo6_100c_300M.root"); pName=TString ("TTbar_PU140_sf1L0x2L5x2_nz4_100c");}
	if (iProc==32){fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz4_pt3_20160308/roads_TTbar_PU200_tt27_sf1L0x2L5x2_nz4_pt3_5oo6_100c_300M.root"); pName=TString ("TTbar_PU200_sf1L0x2L5x2_nz4_100c");}
	if (iProc==33){fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz6_pt3_20160308/roads_TTbar_PU140_tt27_sf1L0x2L5x2_nz6_pt3_5oo6_100c_300M.root"); pName=TString ("TTbar_PU140_sf1L0x2L5x2_nz6_100c");}
	if (iProc==34){fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz6_pt3_20160308/roads_TTbar_PU200_tt27_sf1L0x2L5x2_nz6_pt3_5oo6_100c_300M.root"); pName=TString ("TTbar_PU200_sf1L0x2L5x2_nz6_100c");}
	if (iProc==35){fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz8_pt3_20160308/roads_TTbar_PU140_tt27_sf1L0x2L5x2_nz8_pt3_5oo6_100c_300M.root"); pName=TString ("TTbar_PU140_sf1L0x2L5x2_nz8_100c");}
	if (iProc==36){fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz8_pt3_20160308/roads_TTbar_PU200_tt27_sf1L0x2L5x2_nz8_pt3_5oo6_100c_300M.root"); pName=TString ("TTbar_PU200_sf1L0x2L5x2_nz8_100c");}
	if (iProc==37){fName=TString("/data/rossin/EOS/tt27_sf1L0x2_nz4_pt3_20160308/roads_TTbar_PU140_tt27_sf1L0x2_nz4_pt3_5oo6_100c_300M.root");         pName=TString ("TTbar_PU140_sf1L0x2_nz4_100c");}
	if (iProc==38){fName=TString("/data/rossin/EOS/tt27_sf1L0x2_nz4_pt3_20160308/roads_TTbar_PU200_tt27_sf1L0x2_nz4_pt3_5oo6_100c_300M.root");         pName=TString ("TTbar_PU200_sf1L0x2_nz4_100c");}
	if (iProc==39){fName=TString("/data/rossin/EOS/tt27_sf1L0x2_nz6_pt3_20160308/roads_TTbar_PU140_tt27_sf1L0x2_nz6_pt3_5oo6_100c_300M.root");         pName=TString ("TTbar_PU140_sf1L0x2_nz6_100c");}
	if (iProc==40){fName=TString("/data/rossin/EOS/tt27_sf1L0x2_nz6_pt3_20160308/roads_TTbar_PU200_tt27_sf1L0x2_nz6_pt3_5oo6_100c_300M.root");         pName=TString ("TTbar_PU200_sf1L0x2_nz6_100c");}
	if (iProc==41){fName=TString("/data/rossin/EOS/tt27_sf1L0x2_nz8_pt3_20160308/roads_TTbar_PU140_tt27_sf1L0x2_nz8_pt3_5oo6_100c_300M.root");         pName=TString ("TTbar_PU140_sf1L0x2_nz8_100c");}
	if (iProc==42){fName=TString("/data/rossin/EOS/tt27_sf1L0x2_nz8_pt3_20160308/roads_TTbar_PU200_tt27_sf1L0x2_nz8_pt3_5oo6_100c_300M.root");         pName=TString ("TTbar_PU200_sf1L0x2_nz8_100c");}

	if (iProc==50){fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz4_pt3_20160308/roads_SingleMuonTest_PU140_tt27_sf1L0x2L5x2_nz4_pt3_5oo6_100c_250M.root");       pName=TString ("SingleMu_PU140_sf1L0x2L5x2_nz4_100c");}
	if (iProc==51){fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz6_pt3_20160308/roads_SingleMuonTest_PU140_tt27_sf1L0x2L5x2_nz6_pt3_5oo6_100c_250M.root");       pName=TString ("SingleMu_PU140_sf1L0x2L5x2_nz6_100c");}
	if (iProc==52){fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz8_pt3_20160308/roads_SingleMuonTest_PU140_tt27_sf1L0x2L5x2_nz8_pt3_5oo6_100c_250M.root");       pName=TString ("SingleMu_PU140_sf1L0x2L5x2_nz8_100c");}
	if (iProc==53){fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz4_pt3_20160308/roads_SingleMuonTest_PU0_tt27_sf1L0x2L5x2_nz4_pt3_5oo6_100c_300M.root");         pName=TString   ("SingleMu_PU0_sf1L0x2L5x2_nz4_100c");}
	if (iProc==54){fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz6_pt3_20160308/roads_SingleMuonTest_PU0_tt27_sf1L0x2L5x2_nz6_pt3_5oo6_100c_300M.root");         pName=TString   ("SingleMu_PU0_sf1L0x2L5x2_nz6_100c");}
	if (iProc==55){fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz8_pt3_20160308/roads_SingleMuonTest_PU0_tt27_sf1L0x2L5x2_nz8_pt3_5oo6_100c_300M.root");         pName=TString   ("SingleMu_PU0_sf1L0x2L5x2_nz8_100c");}
	if (iProc==56){fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz4_pt3_20160308/roads_SingleMuonTest_PU140_tt27_sf1L0x2L5x2_nz4_pt3_5oo6_95c_250M.root");        pName=TString ("SingleMu_PU140_sf1L0x2L5x2_nz4_95c" );}
	if (iProc==57){fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz6_pt3_20160308/roads_SingleMuonTest_PU140_tt27_sf1L0x2L5x2_nz6_pt3_5oo6_95c_250M.root");        pName=TString ("SingleMu_PU140_sf1L0x2L5x2_nz6_95c" );}
	if (iProc==58){fName=TString("/data/rossin/EOS/tt27_sf1L0x2L5x2_nz8_pt3_20160308/roads_SingleMuonTest_PU140_tt27_sf1L0x2L5x2_nz8_pt3_5oo6_95c_250M.root");        pName=TString ("SingleMu_PU140_sf1L0x2L5x2_nz8_95c" );}
	roadMerger rm(fName);
	rm.Loop(iEv,merging,pName,makePlots, debug, fName);
	return;
}
