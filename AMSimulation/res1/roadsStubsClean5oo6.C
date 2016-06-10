#define roadsStubs_cxx
#include "roadsStubs.h"
#define stubsNtuple_cxx
#include "stubsNtuple.h"
#include "/home/rossin/Dropbox/TT/CMSSW_6_2_0_SLHC12_patch1_SL6/src/SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTRoad.h"
#include <TH2.h>
#include <TProfile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TMath.h>
#include <TF1.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <TArrow.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TText.h>
#include <TLatex.h>
#include <TEllipse.h>
#include <TVector3.h>

#include <stdio.h>
#include <iostream>
#include <map>
#include <iomanip>

using namespace std;

struct tVectorComp {
	bool operator() (const TVector3& lhs, const TVector3& rhs) const
	{return lhs.Mag2()<rhs.Mag2();}
};

double gau(double* x, double* par) ;
short int stubLayer(double r) ;
//bool passStubPtCut (double stubInvRoughPt, double roadInvPt, double roadInvPtRms, double nSigmas);

void roadsStubsClean5oo6 (int applyOnMuon = 1, bool savePlots=0, bool deriveCuts = 0, int nLayers=6, unsigned bitReduction=0) {

	bool isPlottose = false;

	TStyle* style = gStyle;
	style->SetOptStat(0);

	if (applyOnMuon != 1) deriveCuts=0;
	bool isSLHC25=0;

	TString dirPlots("/home/rossin/Dropbox/TT/Work/figures_stubCleaning/");

//	TString sTree="results_sf1_nz1_tt27_pt2_SingleMuonFlatOneOverPt0p005To0p5_tt27_Sum2M_SL6.root" ;
//	TString sTree="singleMuonNoTest/results_SingleMuonFlatOneOverPt0p005To0p5_tt27NoTest_cfi_py_GEN_SIM_DIGI_L1TrackTrigger_2M.root" ;
//	TString sTree="singleMuNoTest2_2000/results_20150528_SingleMuonFlatOneOverPt0p0005To0p5_tt27NoTest_4M.root" ;
//	TString sTree="singleMuNoTest2_2000/results_20150528_5outOf6_SingleMuonFlatOneOverPt0p0005To0p5_tt27NoTest_4M.root" ;
//	TString sTree="singleMuNoTest2_2000/roads_sf1_nz1_tt27_pt2_SingleMuonFlatOneOverPt0p0005To0p5_tt27NoTest_4M_SLHC25.root"; isSLHC25=1; TString pName("SingleMuFlat_PU0_6oo6");// 6/6
//	TString sTree="singleMuNoTest2_2000/roads_5outOf6_sf1_nz1_tt27_pt2_SingleMuonFlatOneOverPt0p0005To0p5_tt27NoTest_4M_SLHC25.root"; isSLHC25=1; TString pName("SingleMuFlat_PU0_5oo6");// 5/6

//	TString sTree="SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz1_tt27_pt2_6oo6_SingleMuonTest_tt27_PU0_20150408_SLHC25p3_newBank_10M.root"; isSLHC25=1; TString pName("SingleMu_PU0_6oo6");// 6/6
//	TString sTree="SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz1_tt27_pt2_5oo6_SingleMuonTest_tt27_PU0_20150408_SLHC25p3_newBank_10M.root"; isSLHC25=1; TString pName("SingleMu_PU0_5or6oo6");// 5/6

//	TString sTree="SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz1_tt27_pt2_6oo6_SingleMuonTest_tt27_PU140_20150408_SLHC25p3_newBank_10M.root"; isSLHC25=1; TString pName("SingleMu_PU140_6oo6");// 5/6
//	TString sTree="SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz1_tt27_pt2_5oo6_SingleMuonTest_tt27_PU140_20150408_SLHC25p3_newBank_10M.root"; isSLHC25=1; TString pName("SingleMu_PU140_5or6oo6");// 5/6
//	TString sTree="Neutrino_tt27_PU140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz1_tt27_pt2_6oo6_Neutrino_PU140_20150408_SLHC25p3_newBank_10M.root"; isSLHC25=1; TString pName("Neutrino_PU140_6oo6");// 6/6
//	TString sTree="Neutrino_tt27_PU140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz1_tt27_pt2_5or6oo6_Neutrino_PU140_20150408_SLHC25p3_newBank_10M.root"; isSLHC25=1; TString pName("Neutrino_PU140_5or6oo6");// 5/6
//	TString sTree="TTbarTTbar_tt27_PU140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz1_tt27_pt2_6oo6_TTbarTTbar_tt27_PU140_20150408_SLHC25p3_newBank_10M.root"; isSLHC25=1; TString pName("4Tops_PU140_6oo6");// 6/6
//	TString sTree="TTbarTTbar_tt27_PU140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz1_tt27_pt2_5oo6_TTbarTTbar_tt27_PU140_20150408_SLHC25p3_newBank_10M.root"; isSLHC25=1; TString pName("4Tops_PU140_5or6oo6");// 5/6

//	TString sTree="SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/roads_5oo6_SingleMuonFlatOneOverPt0p0005To0p5_tt27_sf1_nz4_pt2_SLHC25p3_2M.root"; isSLHC25=1; TString pName("SingleMu_PU0_sf1_nz4_pt2_5or6oo6");// 5/6
//	TString sTree="SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/roads_5oo6_SingleMuonFlatOneOverPt0p0005To0p5_tt27_sf1_nz4_pt3_SLHC25p3_2M.root"; isSLHC25=1; TString pName("SingleMu_PU0_sf1_nz4_pt3_5or6oo6");// 5/6
//	TString sTree="SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/roads_6oo6_SingleMuonFlatOneOverPt0p0005To0p5_tt27_sf1_nz4_pt2_SLHC25p3_2M.root"; isSLHC25=1; TString pName("SingleMu_PU0_sf1_nz4_pt2_6oo6");// 6/6
//	TString sTree="SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/roads_6oo6_SingleMuonFlatOneOverPt0p0005To0p5_tt27_sf1_nz4_pt3_SLHC25p3_2M.root"; isSLHC25=1; TString pName("SingleMu_PU0_sf1_nz4_pt3_6oo6");// 6/6

//	TString sTree="Neutrino_tt27_PU140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz4_tt27_pt3_5or6oo6_Neutrino_PU140_20150408_SLHC25p3_newBank_100M.root"; isSLHC25=1; TString pName("Neutrino_PU140_sf1_nz4_pt3_5or6oo6");// 6/6
//	TString sTree="Neutrino_tt27_PU140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz4_tt27_pt2_6oo6_Neutrino_PU140_20150408_SLHC25p3_newBank_100M.root"; isSLHC25=1; TString pName("Neutrino_PU140_sf1_nz4_pt2_6oo6");// 6/6
//	TString sTree="Neutrino_tt27_PU140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz4_tt27_pt2_5or6oo6_Neutrino_PU140_20150408_SLHC25p3_newBank_100M.root"; isSLHC25=1; TString pName("Neutrino_PU140_sf1_nz4_pt2_5or6oo6");// 6/6
	TString sTree="Neutrino_PU140_tt27_sf1_nz4_pt3_20151107/roads_Neutrino_PU140_tt27_sf1_nz4_pt3_5oo6_95c_14k.root"; isSLHC25=1; TString pName("Neutrino_PU140_sf1_nz4_pt3_5or6oo6");// 6/6

//		TString sTree="TTbarTTbar_tt27_PU140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz4_tt27_pt2_5oo6_TTbarTTbar_tt27_PU140_20150408_SLHC25p3_100M.root"; isSLHC25=1; TString pName("4Tops_PU140_sf1_nz4_pt2_5or6oo6");// 6/6
//		TString sTree="TTbarTTbar_tt27_PU140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz4_tt27_pt2_6oo6_TTbarTTbar_tt27_PU140_20150408_SLHC25p3_100M.root"; isSLHC25=1; TString pName("4Tops_PU140_sf1_nz4_pt2_6oo6");// 6/6
//		TString sTree="TTbarTTbar_tt27_PU140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz4_tt27_pt3_5oo6_TTbarTTbar_tt27_PU140_20150408_SLHC25p3_100M.root"; isSLHC25=1; TString pName("4Tops_PU140_sf1_nz4_pt3_5or6oo6");// 6/6
//		TString sTree="TTbarTTbar_tt27_PU140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz4_tt27_pt3_6oo6_TTbarTTbar_tt27_PU140_20150408_SLHC25p3_100M.root"; isSLHC25=1; TString pName("4Tops_PU140_sf1_nz4_pt3_6oo6");// 6/6

//	TFile *f = TFile::Open("/data/rossin/EOS/patternBank_sf1_nz1_tt27_pt2_400M_ssID_pT.root","READ");
//	TFile *f = TFile::Open("/data/rossin/EOS/patternBank_sf1_nz1_tt27_pt2_first50M.root","READ");
//	TFile *f = TFile::Open("/data/rossin/EOS/patternBank_sf1_nz1_tt27_pt2_SingleMuonFlatOneOverPt0p005To0p5_tt27_Sum2M_SL6.root","READ");
//	TFile *f = TFile::Open("/data/rossin/EOS/patternBank_SingleMuonFlatOneOverPt0p005To0p5_tt27_2M.root","READ");
//	TFile *f = TFile::Open("/data/rossin/EOS/singleMuonNoTest/patternBank_SingleMuonFlatOneOverPt0p005To0p5_tt27_2M.root","READ");
//	TFile *f = TFile::Open("/data/rossin/EOS/singleMuNoTest2_2000/patternBank_sf1_nz1_tt27_pt2_SingleMuonFlatOneOverPt0p0005To0p5_tt27NoTest_4M.root","READ");
//	TFile *f = TFile::Open("/data/rossin/EOS/singleMuNoTest2_2000/patternBank_sf1_nz1_tt27_pt2_SingleMuonFlatOneOverPt0p0005To0p5_tt27NoTest_4M_SLHC25.root","READ"); isSLHC25=1;
//	TFile *f = TFile::Open("/data/rossin/EOS/SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/patternBank_sf1_nz1_tt27_pt2_SingleMuon_tt27_SLHC25p3_10M.root","READ"); isSLHC25=1; unsigned npatt95coverage = 104000;

//	TFile *f = TFile::Open("/data/rossin/EOS/SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/patternBank_SingleMuonFlatOneOverPt0p0005To0p5_tt27_sf1_nz4_pt3_SLHC25p3_100M.root","READ"); isSLHC25=1; unsigned npatt95coverage = 1862700;
//	TFile *f = TFile::Open("/data/rossin/EOS/SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/patternBank_SingleMuonFlatOneOverPt0p0005To0p5_tt27_sf1_nz4_pt2_SLHC25p3_100M.root","READ"); isSLHC25=1; unsigned npatt95coverage = 3100200;
	TFile *f = TFile::Open("/data/rossin/EOS/Neutrino_PU140_tt27_sf1_nz4_pt3_20151107/patternBank_tt27_sf1_nz4_pt3_100M.root","READ"); isSLHC25=1;


	if (!f) { return; }
//	TTree *t=0; f->GetObject("tSSID",t);
	TTree *t=0;
	TTree *tAtt=0;
	if (isSLHC25) f->GetObject("patternAttributes",tAtt);
	f->GetObject("patternBank",t);
	Long64_t tentries = t->GetEntriesFast();
//	tentries = (npatt95coverage < tentries ? npatt95coverage : tentries);
	cout << "# of patterns in bank: " << tentries << endl;

	std::vector <unsigned int > *AMTTRoads_superstripIds = 0;
	unsigned short ssFrequency;
	double ssPt, ssPtRms, ssPhi, ssPhiRms;
	int ssChargeSum;
	TBranch* bssFrequency=0;
	TBranch *bsuperstripIds= 0;
	TBranch* bssPt  = 0;
	TBranch* bssPtRms = 0;
	TBranch* bssPhi = 0;
	TBranch* bssPhiRms = 0;
	TBranch* bssChargeSum = 0;
//	t->SetBranchAddress("superstripIds",&AMTTRoads_superstripIds,&bsuperstripIds);
//	t->Branch("superstripIds"   ,&AMTTRoads_superstripIds);
//	t->SetBranchAddress("superstripMeanInvPt",&ssPt,&bssPt);
//	t->SetBranchAddress("superstripRmsInvPt" ,&ssRms,&bssRms);
	t->SetBranchAddress("superstripIds",&AMTTRoads_superstripIds,&bsuperstripIds);
	t->Branch("superstripIds"   ,&AMTTRoads_superstripIds);
	t->SetBranchAddress("frequency",&ssFrequency,&bssFrequency);

	float fssPt, fssPtRms, fssPhi, fssPhiRms;
	if (isSLHC25) {
		tAtt->SetBranchAddress("invPt_mean",&fssPt,&bssPt);
		tAtt->SetBranchAddress("invPt_sigma" ,&fssPtRms,&bssPtRms);
		tAtt->SetBranchAddress("phi_mean",&fssPhi,&bssPhi);
		tAtt->SetBranchAddress("phi_sigma" ,&fssPhiRms,&bssPhiRms);
//		tAtt->SetBranchAddress("chargeSum" ,&ssChargeSum,&bssChargeSum);
		ssChargeSum = 1;
	}
	else {
		t->SetBranchAddress("meanPt",&ssPt,&bssPt);
		t->SetBranchAddress("rmsPt" ,&ssPtRms,&bssPtRms);
		t->SetBranchAddress("meanPhi",&ssPhi,&bssPhi);
		t->SetBranchAddress("rmsPhi" ,&ssPhiRms,&bssPhiRms);
		t->SetBranchAddress("chargeSum" ,&ssChargeSum,&bssChargeSum);
	}

//	std::map <vector<unsigned int>, std::pair<std::pair<Double_t,Double_t>,Double_t> > ssIDmap;
//	std::map <vector<unsigned int>, std::pair<std::pair<Double_t,Double_t>,Double_t> >::iterator it;
//	std::vector <patternS > vPatternS ; // 6/6 patterns
//	std::vector <std::vector <patternS > > vvPatternS; // 5/6 patterns
	std::map <vector<unsigned int>, patternS > patternMap;
	std::map <vector<unsigned int>, patternS >::iterator pattern_it;
	vector <std::map <vector<unsigned int>, patternS > > vpatternMap;

//	for (unsigned iLay=0; iLay<7; ++iLay) {
//		std::vector <patternS > vPatternSS;
		std::map <vector<unsigned int>, patternS > patternMapS;
		for (Long64_t jentry=0; jentry<tentries;jentry++) {
			Long64_t ientry = t->LoadTree(jentry);
			tAtt->LoadTree(jentry);
			bsuperstripIds->GetEntry(jentry);
			bssFrequency  ->GetEntry(jentry);
			bssPt         ->GetEntry(jentry);
			bssPtRms      ->GetEntry(jentry);
//			bssPhi        ->GetEntry(jentry);
//			bssPhiRms     ->GetEntry(jentry);
			if (!isSLHC25) bssChargeSum  ->GetEntry(jentry);
			if (isSLHC25) {		ssPt = (double) fssPt; ssPtRms = (double) fssPtRms; ssPhi= (double) fssPhi; ssPhiRms = (double) fssPhiRms;}

			//		if (!ssChargeSum) continue;

			patternS p;
			p.chargeSum = ssChargeSum;
			p.frequency = ssFrequency;
			p.meanPhi   = ssPhi      ;
			if (isSLHC25) p.meanPt    = ssPt;
			else          p.meanPt    = TMath::Sign(ssPt,(double)ssChargeSum);
			p.rmsPhi    = ssPhiRms   ;
			p.rmsPt     = ssPtRms    ;
			p.superstripIds = *AMTTRoads_superstripIds;
//			if (iLay==0) {
//				ssIDmap.insert(make_pair(*AMTTRoads_superstripIds,make_pair(make_pair(TMath::Sign(ssPt,(double)ssChargeSum),ssPhi),ssFrequency)));
//				vPatternS .push_back(p);
				patternMap.insert(make_pair(p.superstripIds,p));
//			}
//			else {
//				patternS pp(p);
//				pp.superstripIds.erase(pp.superstripIds.begin()+iLay-1);
//				vPatternSS.push_back(pp);
//				patternMapS.insert(make_pair(pp.superstripIds,pp));
//			}
		}
//		if (iLay) {
//			vvPatternS.push_back(vPatternSS);
//			vpatternMap.push_back(patternMapS);
//		}
//	}
//	cout << "Loaded " << ssIDmap  .size() << " patterns." <<endl;
	cout << "Loaded " << patternMap.size() << " patterns." <<endl;

/*
	for (unsigned iPatt=0; iPatt<10; ++iPatt) {
		for (unsigned iSS=0; iSS<vPatternS.at(iPatt).superstripIds.size(); ++iSS) cout << vPatternS.at(iPatt).superstripIds[iSS] << "\t";
		cout << endl;
		for (unsigned iLay=0; iLay<vvPatternS.size(); ++iLay) {
			for (unsigned iSS=0; iSS<vvPatternS.at(iLay).at(iPatt).superstripIds.size(); ++iSS) cout << vvPatternS.at(iLay).at(iPatt).superstripIds[iSS] << "\t";
			cout << endl;
		}
		cout << endl;
	}
*/

//	TString sStubs("SingleMuonFlatOneOverPt0p005To0p5_tt27_cfi_py_GEN_SIM_DIGI_L1TrackTrigger_100k_ntuple_Sum2M.root");
//	TTree *tStubs=0;
//	stubsNtuple stub(sStubs,tStubs);
//	if (stub.fChain == 0) return;
//	Long64_t sentries = stub.fChain->GetEntriesFast();
//	cout << "Loading " << sentries << " stub events" << endl;

	const unsigned nBinsRoughInvPt = 27;
	const unsigned nBinsInvPt      = 30;
//	double minStubInvPt = -6.75;
	double maxStubInvPt = 6.75;
//	double minRoadInvPt = -0.5;
	double maxRoadInvPt = 0.5;
	TF1* fgau = new TF1("fgau",gau,-10,10,3); fgau->SetNpx(1000);
	TF1* fp1  = new TF1("fp1" ,"x*[0]"                  ,-10,10);
	TF1* fp3  = new TF1("fp3" ,"x*[0]+x*x*x*[1]",-10,10);

	const unsigned nLayersLoop = 8;

	TProfile* pStubPtroadPt [6][nLayersLoop];
	TH2*     h2StubPtroadPt [6][nLayersLoop];
	TH2*     h2StubPt1roadPt[6];
//	TH2*     h2roadPtgenPt  [6];
	TH1*     h1StubsPerLayer[6][nLayersLoop][2];
	TH1*     h1RoadPerEvent    [nLayersLoop][2];
	TH1*     h1CombPerRoad     [nLayersLoop][2];
	TH1*     h1CombPerEvent    [nLayersLoop][2];
	TH1*     h1StubsPerLayerSingleStub[6][nLayersLoop][2];
	TH1*     h1RoadPerEventSingleStub    [nLayersLoop][2];
	TH1*     h1CombPerRoadSingleStub     [nLayersLoop][2];
	TH1*     h1CombPerEventSingleStub    [nLayersLoop][2];
//	h1StubsPerLayer [6][0] = new TH1D("h1roadComb","h1roadPtcomb",100,0,100);
//	h1StubsPerLayer [6][0] ->SetXTitle("combinations/tower/BX");
//	TH1*     h1stubBend     [6][2]; //[2] for all roads and roads with exactly 6 stubs
	TH1* h1RoadInvPtMissingLayer[6];

	TFile* fInCuts = 0; bool isfInOpen = false;
	if (!deriveCuts) {
		fInCuts = new TFile("deltaScuts_4p0Sigma.root");
		isfInOpen = fInCuts->IsOpen();
		if (!isfInOpen) {
			cout << "ERROR. Not able to load the deltaS cut file. Exiting..." << endl;
			return;
		}
	}

	TString sName;
	TString sTitle;
	for (unsigned iBin=0; iBin <6; ++iBin) {
		char cc [50];
		char cc2[50];
		sprintf(cc,"_%u",iBin);
		for (unsigned iMissLay=0; iMissLay<nLayersLoop; ++iMissLay) {
			if (iMissLay) sprintf(cc2,"_5oo6_NoLayer_%u",iMissLay-1);
			else          sprintf(cc2,"_6oo6");
			if (iMissLay==nLayersLoop-1) sprintf(cc2,"_5oo6");
			sName=TString("pstubInvPt_VS_roadInvPt")+TString(cc)+TString(cc2);
			pStubPtroadPt  [iBin][iMissLay] = new TProfile(sName,sName,nBinsInvPt,-maxRoadInvPt,maxRoadInvPt,-10,10,"s");
			pStubPtroadPt  [iBin][iMissLay] ->SetXTitle("road mean invPt [c/GeV]");
			pStubPtroadPt  [iBin][iMissLay] ->SetYTitle("#Delta s [strip]");
			sName=TString ("h2stubInvPt_VS_roadInvPt")+TString(cc2)+TString(cc);
			sTitle=TString("stubInvPt VS roadInvPt - Layer")+TString(cc)+TString(cc2);
			h2StubPtroadPt [iBin][iMissLay] = new TH2D    (sName,sTitle,nBinsInvPt,-maxRoadInvPt,maxRoadInvPt,nBinsRoughInvPt,-maxStubInvPt,maxStubInvPt);
			h2StubPtroadPt [iBin][iMissLay] ->SetXTitle("road mean invPt [c/GeV]");
			h2StubPtroadPt [iBin][iMissLay] ->SetYTitle("#Deltas [strip]");

			char cc3[50];
			for (unsigned iClean=0; iClean<2; ++iClean) {
				if (iClean) sprintf(cc3,"_Cleaned");
				else        sprintf(cc3," ");
				sName=TString("h1stubsInLayer_")+TString(cc2)+TString(cc)+TString(cc3);
				h1StubsPerLayer           [iBin][iMissLay][iClean] = new TH1D(sName,sName,20,0,20);
				h1StubsPerLayerSingleStub [iBin][iMissLay][iClean] = new TH1D(sName+TString("_SingleStub"),sName+TString("_SingleStub"),20,0,20);
			}
		}
		sName=TString("h2stubInvPt1_VS_roadInvPt")+TString(cc);
		h2StubPt1roadPt[iBin] = new TH2D    (sName,sName,nBinsInvPt,-maxRoadInvPt,maxRoadInvPt,nBinsRoughInvPt,-maxStubInvPt,maxStubInvPt);
//		sName=TString("h2roadInvPt_VS_genInvPt")+TString(cc);
//		h2roadPtgenPt  [iBin] = new TH2D    (sName,sName,nBinsInvPt,-0.6         ,0.6         ,nBinsRoughInvPt,-maxRoadInvPt,maxRoadInvPt);
		sName=TString("h1roadInvPt_MissingLayer")+TString(cc);
		h1RoadInvPtMissingLayer[iBin] = new TH1D(sName,sName,20,0,20);
		sName=TString("h1trigBendInLayer")+TString(cc);
	}
	for (unsigned iMissLay=0; iMissLay<nLayersLoop; ++iMissLay) {
		char cc [50];
		char cc2[50];
		char cc3[50];
		if (iMissLay) sprintf(cc2,"_5oo6_NoLayer_%u",iMissLay-1);
		else          sprintf(cc2,"_6oo6");
		if (iMissLay==nLayersLoop-1) sprintf(cc2,"_5oo6");
		for (unsigned iClean=0; iClean<2; ++iClean) {
			if (iClean) sprintf(cc3,"_Cleaned");
			else        sprintf(cc3," ");
			int nRoadHisto = 50;
			int nCombRoadHisto = 100;
			int nCombHisto = 200;
			if (pName.Contains("4Tops_PU140_")) {
				nRoadHisto=2000;
				nCombRoadHisto = 200;
				nCombHisto=10000;
			}
			if (pName.Contains("SingleMu_PU140_") || pName.Contains("Neutrino_PU140_")) {
				nRoadHisto=800;
				nCombRoadHisto = 200;
				nCombHisto=3000;
			}

			sName=TString("h1RoadPerEvent")+TString(cc2)+TString(cc3);
			h1RoadPerEvent[iMissLay][iClean] = new TH1D(sName,"",nRoadHisto,0,nRoadHisto);
			h1RoadPerEventSingleStub[iMissLay][iClean] = new TH1D(sName+TString("_SingleStub"),"",nRoadHisto,0,nRoadHisto);
			h1RoadPerEvent[iMissLay][iClean] ->SetXTitle("# road/tower/BX");
			sName=TString("h1CombPerRoad")+TString(cc2)+TString(cc3);
			h1CombPerRoad[iMissLay][iClean] = new TH1D(sName,"",nCombRoadHisto,0,nCombRoadHisto);
			h1CombPerRoadSingleStub[iMissLay][iClean] = new TH1D(sName+TString("_SingleStub"),"",nCombRoadHisto,0,nCombRoadHisto);
			h1CombPerRoad[iMissLay][iClean] ->SetXTitle("# comb/road/BX");
			sName=TString("h1CombPerEvent")+TString(cc2)+TString(cc3);
			h1CombPerEvent[iMissLay][iClean] = new TH1D(sName+TString("_SingleStub"),"",nCombHisto,0,nCombHisto);
			h1CombPerEventSingleStub[iMissLay][iClean] = new TH1D(sName,"",nCombHisto,0,nCombHisto);
			h1CombPerEvent[iMissLay][iClean] ->SetXTitle("# comb/tower/BX");
		}
	}
//	const int nHoughEv=5*5;
//	TF1* fHough_zr = new TF1("fHough_zr","[0]+[1]*x",-15,15);
//	fHough_zr->SetNpx(1000);

	TTree* tree=0;
	roadsStubs r(sTree,tree);
	if (r.fChain == 0) return;

	Long64_t nentries = r.fChain->GetEntries();
	cout << "Loaded " << nentries << " events" << endl;

//	TCanvas*  cHough_zr = new TCanvas("cHough_zr","cHough_zr",0,0,800,800);
//	cHough_zr->Divide(sqrt(nHoughEv),sqrt(nHoughEv));



	TAxis* stubBendQuantized[6];
	double stubBendMax[6] = {2.5,2.5,3.0,4.5,5.5,6.5};
	for (unsigned iLay=0; iLay<6; ++iLay) {
		vector <double> vstubBendQuantized;
		for (unsigned iBend=0; iBend<stubBendMax[iLay]*4; ++iBend) vstubBendQuantized.push_back(-stubBendMax[iLay]+0.5*iBend+0.25);
		if (bitReduction==1) {
			vstubBendQuantized.clear();
			if (iLay<2) {
				double astubBendQuantized[8] = {-2.75,-2.25,-1.75,-0.75,0.75,1.75,2.25,2.75};
				for (unsigned iBend=0; iBend<8; ++iBend) vstubBendQuantized.push_back(astubBendQuantized[iBend]);
			}
			else if (iLay==2) {
				double astubBendQuantized[8] = {-3.25,-2.75,-2.25,-1.25,1.25,2.25,2.75,3.25};
				for (unsigned iBend=0; iBend<8; ++iBend) vstubBendQuantized.push_back(astubBendQuantized[iBend]);
			}
			else if (iLay==3) {
				double astubBendQuantized[16] = {-4.75,-4.25,-3.75,-3.25,-2.75,-2.25,-1.75,-0.75,0.75,1.75,2.25,2.75,3.25,3.75,4.25,4.75};
				for (unsigned iBend=0; iBend<16; ++iBend) vstubBendQuantized.push_back(astubBendQuantized[iBend]);
			}
			else if (iLay==4) {
				double astubBendQuantized[16] = {-5.75,-5.25,-4.75,-4.25,-3.75,-3.25,-2.25,-0.75,0.75,2.25,3.25,3.75,4.25,4.75,5.25,5.75};
				for (unsigned iBend=0; iBend<16; ++iBend) vstubBendQuantized.push_back(astubBendQuantized[iBend]);
			}
			else if (iLay==5) {
				double astubBendQuantized[16] = {-6.75,-6.25,-5.75,-5.25,-4.75,-3.75,-2.75,-0.75,0.75,2.75,3.75,4.75,5.25,5.75,6.25,6.75};
				for (unsigned iBend=0; iBend<16; ++iBend) vstubBendQuantized.push_back(astubBendQuantized[iBend]);
			}
		}
		stubBendQuantized[iLay] = new TAxis(vstubBendQuantized.size()-1,&vstubBendQuantized[0]);
		std::cout << iLay  << "\t";
		for (unsigned iBend=0; iBend<vstubBendQuantized.size()+1; ++iBend) std::cout << stubBendQuantized[iLay]->GetBinCenter(iBend) << " ";
		std::cout << std::endl;
	}


	int genPdgId = 13;
	if (!applyOnMuon) genPdgId = 211;

	unsigned nFailed=0;
	unsigned nTested=0;
	unsigned iHough=0;
	Long64_t nbytes = 0, nb = 0;
//	if (!deriveCuts) nentries=0;

	TH2D* hslopes11   = new TH2D("hslopes11"  ,"hslopes11"  ,6,0,6,7,0,7);
	TH2D* hhalfwidths = new TH2D("hhalfwidths","hhalfwidths",6,0,6,7,0,7);
	double slopes11[6][7];
	double slopes1 [6][7];
	double slopes3 [6][7];
	double halfwidths[6][7];
	if (!deriveCuts && isfInOpen) {
		hslopes11   = (TH2D*) fInCuts->Get("hslopes11"  );
		hhalfwidths = (TH2D*) fInCuts->Get("hhalfwidths");
		if (!hslopes11) return;
		cout << "Loading slopes and halfwidths" << endl;
		for (unsigned iMissLay=0; iMissLay<7; ++iMissLay) {
			for (unsigned iLay=0; iLay<6; ++iLay) {
				slopes11  [iLay][iMissLay] = hslopes11  ->GetBinContent(iLay+1,iMissLay+1);
				halfwidths[iLay][iMissLay] = hhalfwidths->GetBinContent(iLay+1,iMissLay+1);
				cout <<   "slopes11["  <<iLay<<"]["<<iMissLay<<"] = "    << slopes11  [iLay][iMissLay]
                     << "; halfwidths["<<iLay<<"]["<<iMissLay<<"] = "    << halfwidths[iLay][iMissLay]
					  << ";" << endl;
			}
		}
	}

	unsigned countEv        = 0;
	unsigned count6         = 0;
//	unsigned count5         = 0;
	unsigned count5or6      = 0;

	unsigned count6Clean    = 0;
	unsigned count5Clean    = 0;
	unsigned count5or6Clean = 0;

//	unsigned count6SingleStub    = 0;
//	unsigned count5SingleStub    = 0;
//	unsigned count5or6SingleStub = 0;
//
//	unsigned count6SingleStubClean    = 0;
//	unsigned count5SingleStubClean    = 0;
//	unsigned count5or6SingleStubClean = 0;

	for (Long64_t jentry=0; jentry<nentries;jentry++) { // LOOPING OVER EVENTS
//	for (Long64_t jentry=0; jentry<100;jentry++) {
		Long64_t ientry = r.LoadTree(jentry);
		if (ientry < 0) break;
		nb = r.fChain->GetEntry(jentry);   nbytes += nb;
		unsigned int nPart  = r.genParts_pt     ->size();
		if (deriveCuts && nPart!=1) continue;
//		unsigned int nTotStubs = r.TTStubs_r       ->size();
		unsigned int nRoads = r.AMTTRoads_nstubs->size();
		if (!nRoads) continue;
		++countEv;

		bool eventFired6 = false;
		bool eventFired5or6 = false;
		bool eventFiredClean6 = false;
		bool eventFiredClean5or6 = false;

//		bool has1Match6         = false;
//		bool has1Match5         = false;
//		bool has1Match5or6      = false;
//
//		bool has1Match6Clean    = false;
//		bool has1Match5Clean    = false;
//		bool has1Match5or6Clean = false;
//
//		bool has1Match6SingleStubClean    = false;
//		bool has1Match5SingleStubClean    = false;
//		bool has1Match5or6SingleStubClean = false;

		unsigned short nCombinationsPerEvent                [nLayersLoop] = {0, 0, 0, 0, 0, 0, 0 ,0};
		unsigned short nCombinationsPerEventClean           [nLayersLoop] = {0, 0, 0, 0, 0, 0, 0 ,0};
		unsigned short nCombinationsPerEventSingleStub      [nLayersLoop] = {0, 0, 0, 0, 0, 0, 0 ,0};
		unsigned short nCombinationsPerEventSingleStubClean [nLayersLoop] = {0, 0, 0, 0, 0, 0, 0 ,0};

		unsigned short nRoadsPerEvent                [nLayersLoop] = {0, 0, 0, 0, 0, 0, 0, 0};
		unsigned short nRoadsPerEventClean           [nLayersLoop] = {0, 0, 0, 0, 0, 0, 0, 0};
//		unsigned short nRoadsPerEventSingleStub      [nLayersLoop] = {0, 0, 0, 0, 0, 0, 0, 0};
		unsigned short nRoadsPerEventSingleStubClean [nLayersLoop] = {0, 0, 0, 0, 0, 0, 0, 0};

//		vector <slhcl1tt::TTRoad> vroadSingleStub;
//		vector <slhcl1tt::TTRoad> vroadSingleStubDsClean;
//		vector <slhcl1tt::TTRoad> vroadDsClean;

//		cout << "nRoads " << nRoads << endl;
		for (unsigned int iRoads=0; iRoads<nRoads; ++iRoads) { // LOOPING OVER ROADS in EVENT
			slhcl1tt::TTRoad road;
			road.stubRefs      = r.AMTTRoads_stubRefs     ->at(iRoads);
			road.superstripIds = r.AMTTRoads_superstripIds->at(iRoads);
			road.nstubs        = r.AMTTRoads_nstubs       ->at(iRoads);
//			road.patternRef    = r.AMTTRoads_patternRef   ->at(iRoads);
			slhcl1tt::TTRoad roadSingleStub(road);
//			slhcl1tt::TTRoad roadDsClean   (road);


			std::map <short int,unsigned int> stubLayers_map;
			std::map <short int,unsigned int> stubLayers_mapClean;
			std::map <short int,unsigned int> stubLayers_mapSingleStub;
			std::map <short int,unsigned int> stubLayers_mapSingleStubClean;
			short unsigned int stubsPerLayer     [6] = {0,0,0,0,0,0};
			short unsigned int stubsPerLayerClean[6] = {0,0,0,0,0,0};
			short unsigned int stubsPerLayerSingleStub[6] = {0,0,0,0,0,0};
			short unsigned int stubsPerLayerSingleStubClean[6] = {0,0,0,0,0,0};

			pattern_it=patternMap.find(road.superstripIds);
//			it=ssIDmap.find(road.superstripIds);
			if (pattern_it==patternMap.end() ) {
				continue;
			}
			bool fired6Layer         = false;
			bool fired5Layer         = false;
			bool fired5or6Layer      = false;

			bool fired6LayerClean    = false;
			bool fired5LayerClean    = false;
			bool fired5or6LayerClean = false;

			bool fired6LayerSingleStubClean    = false;
			bool fired5LayerSingleStubClean    = false;
			bool fired5or6LayerSingleStubClean = false;


			for (unsigned int iSS=0; iSS<road.superstripIds.size();++iSS) { // Looping over SS in ROAD
				unsigned int ssID = road.superstripIds.at(iSS);
				unsigned int nStubs = road.stubRefs.at(iSS).size();
				vector <unsigned> vStubsIDSingleStub;
//				vector <unsigned> vStubsIDDsClean   ;
//				vector <unsigned> vStubsIDSingleStubDsClean;
				vector <std::pair<TVector3,unsigned> > vRStubsID;
				for (unsigned int iStub=0; iStub<nStubs; ++iStub) { // looping on stubs 1
					unsigned int stubID = road.stubRefs.at(iSS).at(iStub);
					short int stub_layer = TMath::Floor(r.TTStubs_modId   ->at(stubID)/10000.)-5;
					++stubsPerLayer[stub_layer];
					stubLayers_map [stub_layer]=stubID;
					double stub_bend = r.TTStubs_trigBend->at(stubID);
					double stub_x1 = r.TTStubs_x->at(stubID);
					double stub_y1 = r.TTStubs_y->at(stubID);
					double stub_z1 = r.TTStubs_z->at(stubID);
//					cout << setw(6) << jentry << setw(6) << iSS << setw(3) << stub_layer << setw(10) << stub_x1 << setw(10) << stub_y1 << setw(10) << stub_z1 << setw(6) << stub_bend << endl;
					TVector3 v31(stub_x1,stub_y1,stub_z1);
					vRStubsID.push_back(std::pair<TVector3,unsigned>(v31,stubID));
//					vStubsIDSingleStub.push_back(stubID);
//					vStubsIDDsClean   .push_back(stubID);
				} //end looping on stubs 1

				std::map <TVector3,unsigned,tVectorComp> map_vRStubsID;
				for (unsigned int iStub=0; iStub<vRStubsID.size(); ++iStub) {
					 std::map <TVector3,unsigned>::iterator map_vRStubsID_it;
					 map_vRStubsID_it = map_vRStubsID.find(vRStubsID.at(iStub).first);
					 if (map_vRStubsID_it == map_vRStubsID.end()) {
						 map_vRStubsID.insert(vRStubsID.at(iStub));
					 }
					 else {
						 unsigned stubIDvec = vRStubsID.at(iStub).second;
						 unsigned stubIDmap = map_vRStubsID_it->second;
						 if (TMath::Abs(r.TTStubs_trigBend->at(stubIDvec)) < TMath::Abs(r.TTStubs_trigBend->at(stubIDmap))) {
							 map_vRStubsID.erase(map_vRStubsID_it);
							 map_vRStubsID.insert(vRStubsID.at(iStub));
						 }
					 }
				}

				for (std::map <TVector3,unsigned>::iterator map_vRStubsID_it = map_vRStubsID.begin();
						map_vRStubsID_it != map_vRStubsID.end(); ++map_vRStubsID_it) {
					vStubsIDSingleStub.push_back(map_vRStubsID_it->second);
				}
				roadSingleStub.stubRefs.at(iSS) = vStubsIDSingleStub;


//				if (vStubsIDSingleStub.size() != nStubs) {
//					cout << "***  Duplicate stubs  ***" << endl ;
//					for (unsigned int iStub=0; iStub<nStubs; ++iStub) {
//						unsigned int stubID = road.stubRefs.at(iSS).at(iStub);
//						double stub_bend = r.TTStubs_trigBend->at(stubID);
//						double stub_x1 = r.TTStubs_x->at(stubID);
//						double stub_y1 = r.TTStubs_y->at(stubID);
//						double stub_z1 = r.TTStubs_z->at(stubID);
//						cout << setw(10) << stub_x1 << setw(10) << stub_y1 << setw(10) << stub_z1 << setw(4) << stub_bend << endl;
//					}
//					cout << "#  Removed duplicate stubs  #" << endl ;
//					for (unsigned int iStub=0; iStub<vStubsIDSingleStub.size(); ++iStub) {
//						unsigned int stubID = vStubsIDSingleStub.at(iStub);
//						double stub_bend = r.TTStubs_trigBend->at(stubID);
//						double stub_x1 = r.TTStubs_x->at(stubID);
//						double stub_y1 = r.TTStubs_y->at(stubID);
//						double stub_z1 = r.TTStubs_z->at(stubID);
//						cout << setw(10) << stub_x1 << setw(10) << stub_y1 << setw(10) << stub_z1 << setw(4) << stub_bend << endl;
//					}
//				}

			} // end looping on SS

			if (roadSingleStub.superstripIds.size() != road.superstripIds.size()) {
				cout << "ERROR, SS multiplicity changed" << endl;
				return;
			}

			for (unsigned int iSS=0; iSS<roadSingleStub.superstripIds.size();++iSS) { // Looping over SS in ROAD 2
				unsigned int nStubs = roadSingleStub.stubRefs.at(iSS).size();
				for (unsigned int iStub=0; iStub<nStubs; ++iStub) { // looping on stubs 3
					unsigned int stubID = roadSingleStub.stubRefs.at(iSS).at(iStub);
					short int stub_layer = TMath::Floor(r.TTStubs_modId   ->at(stubID)/10000.)-5;
					++stubsPerLayerSingleStub[stub_layer];
					stubLayers_mapSingleStub[stub_layer]=stubID;
				}// end looping on stubs 3
			} // end looping on SS 2

			if (stubLayers_map.size()==6) {
				fired6Layer   =true; // a 5/6 will fire a 6/6 pattern, but no stub will be found
				fired5or6Layer=true;
				eventFired6   =true;
			}
			if (stubLayers_map.size()==5) {
				fired5Layer   =true;
				fired5or6Layer=true;
				eventFired5or6=true;
			}
//			if (fired6Layer && !has1Match6) {
//				++count6;
//				has1Match6   =true;
//			}
//			if (fired5Layer && !has1Match5) {
//				++count5;
//				has1Match5=true;
//			}
//			if (fired5or6Layer && !has1Match5or6) {
//				++count5or6;
//				has1Match5or6=true;
//			}

			if ((!fired6Layer && nLayers==6)) {
				continue;
			}
			if ((!fired5or6Layer && nLayers==5)) {
				continue;
			}

//			cout << setw(6) << stubLayers_map.size()
//				         << setw(3) << stubsPerLayer[0]
//				         << setw(3) << stubsPerLayer[1]
//						 << setw(3) << stubsPerLayer[2]
//						 << setw(3) << stubsPerLayer[3]
//						 << setw(3) << stubsPerLayer[4]
//						 << setw(3) << stubsPerLayer[5]
//						 << endl;

			//			short maxnMissed = 6-nLayers;
			if ((int)stubLayers_map.size()<nLayers) {
				cout << fired6Layer << "\t" << stubLayers_map.size() << endl;
				cout << "Too many missing layers" <<endl;
//				for (unsigned int iSS=0; iSS<road.superstripIds.size();++iSS) {
//					unsigned int ssID = road.superstripIds.at(iSS);
//					cout << "ssID:\t" << ssID << endl;
//					unsigned int nStubs = road.stubRefs.at(iSS).size();
//					cout << "nStubs:\t" <<nStubs << endl;
//					for (unsigned int iStub=0; iStub<nStubs; ++iStub) {
//						unsigned int stubID = road.stubRefs.at(iSS).at(iStub);
//						short int stub_layer = TMath::Floor(r.TTStubs_modId   ->at(stubID)/10000.)-5;
//						++stubsPerLayer[stub_layer];
//						double stub_bend = r.TTStubs_trigBend->at(stubID);
//						stubLayers_map[stub_layer]=stubID;
//						cout << r.TTStubs_modId   ->at(stubID) << "\t";
//					} //end looping on stubs
//				} // end looping on SS
//				cout << endl;
				return;
			}

			double roadInvPt       = -999;
			unsigned roadInvPtFreq = 0;
			int missingLayer = -1;

			if (fired6Layer || fired5Layer) {
				roadInvPt     = pattern_it->second.meanPt;
				roadInvPtFreq = pattern_it->second.frequency;
			}
			else continue;

			if (fired5Layer) {
				for (unsigned iL=0; iL <6; ++iL) {
					if (!stubsPerLayer[iL]) {
						missingLayer=iL;
						break;
					}
				}
			}


			if (!deriveCuts) {
				for (unsigned int iSS=0; iSS<road.superstripIds.size();++iSS) { // Looping over SS in ROAD
//					unsigned int ssID = road.superstripIds.at(iSS);
					for (unsigned int iStub=0; iStub<road.stubRefs.at(iSS).size(); ++iStub) {
						unsigned int stubID = road.stubRefs.at(iSS).at(iStub);
						short int stub_layer = TMath::Floor(r.TTStubs_modId   ->at(stubID)/10000.)-5;
						double halfWidthCut = 0;
						if (fired6Layer) {
							fp1->SetParameter(0,slopes11[stub_layer][0]             );
							halfWidthCut     = halfwidths[stub_layer][0];
						}
						if (fired5Layer) {
							fp1->SetParameter(0,slopes11[stub_layer][missingLayer+1]);
							halfWidthCut     = halfwidths[stub_layer][missingLayer+1];
						}
						double centralValueCut = fp1->Eval(roadInvPt);
						if (centralValueCut==0) cout << "MIEZZEGA" << "\t" << fired6Layer << "\t" << fired5Layer << "\t" <<  roadInvPt << "\t" << fp1->GetParameter(0) << endl;
						if (halfWidthCut==0) cout << "CAZZAROLA" << "\t" << stub_layer << "\t" << missingLayer+1<< endl;
						double stub_bend = r.TTStubs_trigBend->at(stubID);
						if (bitReduction) {
							stub_bend = stubBendQuantized[stub_layer]->GetBinCenter(stubBendQuantized[stub_layer]->FindBin(stub_bend));
						}
						if (centralValueCut-halfWidthCut < stub_bend &&
								stub_bend < centralValueCut+halfWidthCut){
							++stubsPerLayerClean[stub_layer];
							stubLayers_mapClean[stub_layer]=stubID;
						}
					} //end looping on stubs

					for (unsigned int iStub=0; iStub<roadSingleStub.stubRefs.at(iSS).size(); ++iStub) { // looping on stubs after duplication removal
						unsigned int stubID = roadSingleStub.stubRefs.at(iSS).at(iStub);
						short int stub_layer = TMath::Floor(r.TTStubs_modId   ->at(stubID)/10000.)-5;
						double halfWidthCut = 0;
						if (fired6Layer) {
							fp1->SetParameter(0,slopes11[stub_layer][0]             );
							halfWidthCut     = halfwidths[stub_layer][0];
						}
						if (fired5Layer) {
							fp1->SetParameter(0,slopes11[stub_layer][missingLayer+1]);
							halfWidthCut     = halfwidths[stub_layer][missingLayer+1];
						}
						double centralValueCut = fp1->Eval(roadInvPt);
						double stub_bend = r.TTStubs_trigBend->at(stubID);
						if (bitReduction) {
							stub_bend = stubBendQuantized[stub_layer]->GetBinCenter(stubBendQuantized[stub_layer]->FindBin(stub_bend));
						}
						if (centralValueCut-halfWidthCut < stub_bend &&
								stub_bend < centralValueCut+halfWidthCut){
							++stubsPerLayerSingleStubClean[stub_layer];
							stubLayers_mapSingleStubClean[stub_layer]=stubID;
						}
					} //end looping on stubs after duplication removal


				} // end looping on SS

				if (stubLayers_mapClean.size()==6) {
					fired6LayerClean   =true; // a 5/6 will fire a 6/6 pattern, but no stub will be found
					fired5or6LayerClean=true;
					eventFiredClean6   =true;
					eventFiredClean5or6=true;
				}
				if (stubLayers_mapClean.size()==5) {
					fired5LayerClean   =true;
					fired5or6LayerClean=true;
					eventFiredClean5or6=true;
				}
//				if (fired6LayerClean && !has1Match6Clean) {
//					++count6Clean;
//					has1Match6Clean   =true;
//				}
//				if (fired5LayerClean && !has1Match5Clean) {
//					++count5Clean;
//					has1Match5Clean=true;
//				}
//				if (fired5or6LayerClean && !has1Match5or6Clean) {
//					++count5or6Clean;
//					has1Match5or6Clean=true;
//				}
//
//				if (stubLayers_mapSingleStubClean.size()==6) {
//					fired6LayerSingleStubClean   =true; // a 5/6 will fire a 6/6 pattern, but no stub will be found
//					fired5or6LayerSingleStubClean=true;
//				}
//				if (stubLayers_mapSingleStubClean.size()==5) {
//					fired5LayerSingleStubClean   =true; // a 5/6 will fire a 6/6 pattern, but no stub will be found
//					fired5or6LayerSingleStubClean=true;
//				}
//				if (fired5LayerSingleStubClean && !has1Match5Clean) {
//					++count5SingleStubClean;
//					has1Match5SingleStubClean=true;
//				}

			}


			int missingLayerClean = -1;
			for (unsigned iLay=0; iLay<6; ++iLay) {
				if (!stubsPerLayerClean[iLay]) {
					missingLayerClean = iLay;
					break;
				}
			}

			stubLayers_map.clear();
			unsigned int nStubsRoad=0;
			for (unsigned int iSS=0; iSS<road.superstripIds.size();++iSS) {
				unsigned int ssID = road.superstripIds.at(iSS);
				unsigned int nStubs = road.stubRefs.at(iSS).size();
				for (unsigned int iStub=0; iStub<nStubs; ++iStub) {
					unsigned int stubID = road.stubRefs.at(iSS).at(iStub);
//					double stub_r = r.TTStubs_r->at(stubID);
//					double stub_z = r.TTStubs_z->at(stubID);
//					short int stub_layer = stubLayer(stub_r);
					short int stub_layer = TMath::Floor(r.TTStubs_modId   ->at(stubID)/10000.)-5;
					if (stub_layer<0) {
						cout << "ERROR" << cout;
						return;
					}
//					++stubsPerLayer[stub_layer];
					double stub_bend = r.TTStubs_trigBend->at(stubID);
					stubLayers_map[stub_layer]=stubID;
					if (fired6Layer) {
						pStubPtroadPt [stub_layer][0]->Fill(roadInvPt,stub_bend);
						h2StubPtroadPt[stub_layer][0]->Fill(roadInvPt,stub_bend);
//						h2roadPtgenPt [stub_layer]->Fill(1/r.genParts_pt     ->at(0),roadInvPt);
//						cout << "6oo6\t" << roadInvPt << "\t" << stub_bend << endl;
					}
					if (fired5Layer) {
						pStubPtroadPt [stub_layer][missingLayer+1]->Fill(roadInvPt,stub_bend);
						h2StubPtroadPt[stub_layer][missingLayer+1]->Fill(roadInvPt,stub_bend);
//						cout << "5oo6\t" << roadInvPt << "\t" << stub_bend << endl;
					}
					if (fired5or6Layer) {
						pStubPtroadPt [stub_layer][nLayersLoop-1] ->Fill(roadInvPt,stub_bend);
						h2StubPtroadPt[stub_layer][nLayersLoop-1] ->Fill(roadInvPt,stub_bend);
					}
				} //end looping on stubs
				if (fired5Layer) {
					h1RoadInvPtMissingLayer[missingLayer]->Fill(roadInvPt);
				}
			} // end looping on SS
//			if ((int)stubLayers_map.size()-nLayers<0) {
//				cout << "Not enough layers with stubs. Skipping road " << stubLayers_map.size() << endl;
//				for (unsigned iLay=0; iLay <6; ++iLay)  cout << stubsPerLayer[iLay] << "\t";
//				cout << endl;
//				for (unsigned iLay=0; iLay <6; ++iLay)  cout << vPatternS[iPatt].superstripIds[iLay] << "\t";
//				cout << endl;
//				continue;
//			}

			unsigned short nCombinationsPerRoad               [nLayersLoop] = {1, 1, 1, 1, 1, 1, 1, 1};
			unsigned short nCombinationsPerRoadClean          [nLayersLoop] = {1, 1, 1, 1, 1, 1, 1, 1};
			unsigned short nCombinationsPerRoadSingleStub     [nLayersLoop] = {1, 1, 1, 1, 1, 1, 1, 1};
			unsigned short nCombinationsPerRoadSingleStubClean[nLayersLoop] = {1, 1, 1, 1, 1, 1, 1, 1};
			for (unsigned iBin=0; iBin <6; ++iBin) {
				if (fired6Layer) {
					h1StubsPerLayer [iBin][0]            [0]->Fill(stubsPerLayer[iBin]);
					h1StubsPerLayer [iBin][nLayersLoop-1][0]->Fill(stubsPerLayer[iBin]);
					nCombinationsPerRoad[0]            *=stubsPerLayer[iBin];
					nCombinationsPerRoad[nLayersLoop-1]*=stubsPerLayer[iBin];

					h1StubsPerLayerSingleStub [iBin][0]            [0]->Fill(stubsPerLayerSingleStub[iBin]);
					h1StubsPerLayerSingleStub [iBin][nLayersLoop-1][0]->Fill(stubsPerLayerSingleStub[iBin]);
					nCombinationsPerRoadSingleStub[0]            *=stubsPerLayerSingleStub[iBin];
					nCombinationsPerRoadSingleStub[nLayersLoop-1]*=stubsPerLayerSingleStub[iBin];
					if (fired6LayerClean) {
						h1StubsPerLayer [iBin][0]            [1]->Fill(stubsPerLayerClean[iBin]);
						h1StubsPerLayer [iBin][nLayersLoop-1][1]->Fill(stubsPerLayerClean[iBin]);
						nCombinationsPerRoadClean[0]            *=stubsPerLayerClean[iBin];
						nCombinationsPerRoadClean[nLayersLoop-1]*=stubsPerLayerClean[iBin];
						if (fired6LayerSingleStubClean) {
							h1StubsPerLayerSingleStub [iBin][0]            [1]->Fill(stubsPerLayerSingleStubClean[iBin]);
							h1StubsPerLayerSingleStub [iBin][nLayersLoop-1][1]->Fill(stubsPerLayerSingleStubClean[iBin]);
							nCombinationsPerRoadSingleStubClean[0]            *=stubsPerLayerSingleStubClean[iBin];
							nCombinationsPerRoadSingleStubClean[nLayersLoop-1]*=stubsPerLayerSingleStubClean[iBin];
						}
					}
				}
				if (fired5Layer) {
					h1StubsPerLayer           [iBin][nLayersLoop-1][0]->Fill(stubsPerLayer[iBin]);
					h1StubsPerLayerSingleStub [iBin][nLayersLoop-1][0]->Fill(stubsPerLayerSingleStub[iBin]);
					if (stubsPerLayer[iBin]) {
						nCombinationsPerRoad          [nLayersLoop-1]*=stubsPerLayer          [iBin];
						nCombinationsPerRoadSingleStub[nLayersLoop-1]*=stubsPerLayerSingleStub[iBin];
					}
					for (unsigned iMissLay=0; iMissLay<6; ++iMissLay) {
						if ((int)iMissLay==missingLayer) {
							h1StubsPerLayer           [iBin][iMissLay+1][0]->Fill(stubsPerLayer          [iBin]);
							h1StubsPerLayerSingleStub [iBin][iMissLay+1][0]->Fill(stubsPerLayerSingleStub[iBin]);
						}
						if ((int)iMissLay!=missingLayer) {
							if (stubsPerLayer          [iBin]) nCombinationsPerRoad          [iMissLay+1]*=stubsPerLayer          [iBin];
							if (stubsPerLayerSingleStub[iBin]) nCombinationsPerRoadSingleStub[iMissLay+1]*=stubsPerLayerSingleStub[iBin];
						}
					}
				}
				if (fired5LayerClean) {
					h1StubsPerLayer[iBin][nLayersLoop-1][1]->Fill(stubsPerLayerClean[iBin]);
					if (stubsPerLayerClean[iBin]) nCombinationsPerRoadClean[nLayersLoop-1]*=stubsPerLayerClean[iBin];
					for (unsigned iMissLay=0; iMissLay<6; ++iMissLay) {
						if ((int)iMissLay==missingLayerClean) h1StubsPerLayer [iBin][iMissLay+1][1]->Fill(stubsPerLayerClean[iBin]);
						if ((int)iMissLay!=missingLayerClean) if (stubsPerLayerClean[iBin]) nCombinationsPerRoadClean[iMissLay+1]*=stubsPerLayerClean[iBin];
					}
				}
				if (fired5LayerSingleStubClean) {
					h1StubsPerLayerSingleStub[iBin][nLayersLoop-1][1]->Fill(stubsPerLayerSingleStubClean[iBin]);
					if (stubsPerLayerSingleStubClean[iBin]) nCombinationsPerRoadSingleStubClean[nLayersLoop-1]*=stubsPerLayerSingleStubClean[iBin];
					for (unsigned iMissLay=0; iMissLay<6; ++iMissLay) {
						if ((int)iMissLay==missingLayerClean) h1StubsPerLayerSingleStub [iBin][iMissLay+1][1]->Fill(stubsPerLayerSingleStubClean[iBin]);
						if ((int)iMissLay!=missingLayerClean) if (stubsPerLayerSingleStubClean[iBin]) nCombinationsPerRoadClean[iMissLay+1]*=stubsPerLayerSingleStubClean[iBin];
					}
				}
			}
			for (unsigned iLay=0; iLay <nLayersLoop; ++iLay) {
				if ((fired6Layer && !iLay) || (fired5Layer &&  iLay)) {
				h1CombPerRoad          [iLay][0]     ->Fill(nCombinationsPerRoad               [iLay]);
				h1CombPerRoad          [iLay][1]     ->Fill(nCombinationsPerRoadClean          [iLay]);
				h1CombPerRoadSingleStub[iLay][0]     ->Fill(nCombinationsPerRoadSingleStub     [iLay]);
				h1CombPerRoadSingleStub[iLay][1]     ->Fill(nCombinationsPerRoadSingleStubClean[iLay]);
				nCombinationsPerEvent               [iLay]+=nCombinationsPerRoad               [iLay];
				nCombinationsPerEventSingleStub     [iLay]+=nCombinationsPerRoadSingleStub     [iLay];
				}
				if ((fired6LayerClean && !iLay) || (fired5LayerClean &&  iLay))
					nCombinationsPerEventClean          [iLay]+=nCombinationsPerRoadClean          [iLay];
				if ((fired6LayerSingleStubClean && !iLay) || (fired5LayerSingleStubClean&&  iLay))
					nCombinationsPerEventSingleStubClean[iLay]+=nCombinationsPerRoadSingleStubClean[iLay];
//				cout << setw(2) << fired6Layer << setw(2) << fired5Layer << setw(2) << iLay << setw(4) << nCombinationsPerRoad[iLay] << setw(4) << nCombinationsPerEvent[iLay] <<endl;
			}
			if (fired6Layer) {
				++nRoadsPerEvent           [0];
				++nRoadsPerEvent           [nLayersLoop-1];
//				++nRoadsPerEventSingleStub [0];
//				++nRoadsPerEventSingleStub [nLayersLoop-1];
				if (fired6LayerClean) {
					++nRoadsPerEventClean       [0];
					++nRoadsPerEventClean       [nLayersLoop-1];
					if (fired6LayerSingleStubClean) {
						++nRoadsPerEventSingleStubClean       [0];
						++nRoadsPerEventSingleStubClean       [nLayersLoop-1];
					}
				}
			}
			if (fired5Layer) {
				++nRoadsPerEvent           [missingLayer+1];
				++nRoadsPerEvent           [nLayersLoop-1];
			}
			if (fired5LayerClean) {
				++nRoadsPerEventClean      [missingLayer+1];
				++nRoadsPerEventClean      [nLayersLoop-1];
			}
			if (fired5LayerSingleStubClean) {
				++nRoadsPerEventSingleStubClean      [missingLayer+1];
				++nRoadsPerEventSingleStubClean      [nLayersLoop-1];
			}
		} // end looping on roads

		for (unsigned iLay=0; iLay <nLayersLoop; ++iLay) {
//			if (!iLay) cout << setw(4) << nCombinationsPerEvent[iLay] <<endl;

			if (nCombinationsPerEvent[iLay]) {
				h1CombPerEvent          [iLay][0]->Fill(nCombinationsPerEvent[iLay]);
				h1RoadPerEvent          [iLay][0]->Fill(nRoadsPerEvent     [iLay]);
			}
			if (nCombinationsPerEventSingleStub[iLay]) h1CombPerEventSingleStub[iLay][0]->Fill(nCombinationsPerEventSingleStub[iLay]);

			if (nCombinationsPerEventClean[iLay]) {
				h1CombPerEvent          [iLay][1]->Fill(nCombinationsPerEventClean[iLay]);
				h1RoadPerEvent          [iLay][1]->Fill(nRoadsPerEventClean[iLay]);
			}
			if (nCombinationsPerEventSingleStubClean[iLay]) {
				h1CombPerEventSingleStub[iLay][1]->Fill(nCombinationsPerEventSingleStubClean[iLay]);
				h1RoadPerEventSingleStub[iLay][1]->Fill(nRoadsPerEventSingleStubClean[iLay]);
			}
		}

		++nTested;
		if (eventFired6) ++count6;
		if (eventFired5or6) ++count5or6;
		if (eventFiredClean6) ++count6Clean;
		if (eventFiredClean5or6) ++count5or6Clean;

//		if (count == nRoads && nRoads) {
//			++nFailed;
//		}
//		unsigned nTotComb      [nLayersLoop] = {0, 0, 0, 0, 0, 0, 0, 0};
//		unsigned nTotCombClean [nLayersLoop] = {0, 0, 0, 0, 0, 0, 0, 0};
//		for (unsigned iLay=0; iLay <nLayersLoop; ++iLay) {
//			nTotComb     [iLay]+=nCombinationsPerEvent     [iLay];
//			nTotCombClean[iLay]+=nCombinationsPerEventClean[iLay];
//		}
		if (jentry%5000==0 && jentry) {
			cout << "Processing entry " << setw(12) << jentry << setw(12) << 1.0*countEv/jentry
					<< setw(12) << 1.0*count6/countEv << setw(12) << 1.0*count5or6/countEv;
			if (!deriveCuts) cout << setw(12) << 1.0*count6Clean/countEv << setw(12) << 1.0*count5or6Clean/countEv;
//					<< setw(12) << 1.0*count6SingleStubClean/countEv << setw(12) << 1.0*count5SingleStubClean/countEv;
			cout << endl;
		}
	} // end looping over events
//	cout << count6 << "\t" << count5 << endl;

//	return;

	TLegend* tl;
	TCanvas* cStubsPerLayer[nLayersLoop][2];
	for (unsigned iMissLay=0; iMissLay <nLayersLoop; ++iMissLay) {
		if (nLayers==6 && iMissLay) continue;
		char cc [50];
		char cc2[50];
		char cc3[50];
		char cc5[50];
		if (iMissLay) {
			sprintf(cc2," 5oo6 Miss Layer %u",iMissLay-1);
			sprintf(cc5,"_5oo6MissLayer_%u_",iMissLay-1);
		}
		else {
			sprintf(cc2," 6oo6");
			sprintf(cc5,"_6oo6_");
		}
		if (iMissLay==nLayersLoop-1) {
			sprintf(cc2," 5 or 6 oo6");
			sprintf(cc5,"_5or6oo6");
		}
		for (unsigned iClean=0; iClean<2; ++iClean) {
			char cc4[50]="";
			if (!iClean) sprintf(cc4,"_NoClean");
			TString sTitle(TString("Stubs per layer")+TString(cc2)+TString(cc4));
			TString sName (TString("cStubsPerLayer")+TString(cc5)+TString(cc4));
			cStubsPerLayer[iMissLay][iClean]= new TCanvas(sName,sTitle,0,0,900,900);
			cStubsPerLayer[iMissLay][iClean]->Divide(3,2);
			for (unsigned iLay=0; iLay <1; ++iLay) {
				cStubsPerLayer[iMissLay][iClean]->cd(iLay+1);
				gPad->SetLogy();
				gPad->SetGrid();
				tl = new TLegend(0.3,0.75,0.9,0.9);
				sprintf(cc,"Stubs in Layer %u",iLay);
				h1StubsPerLayer[iLay][iMissLay][0]->SetTitle(TString(cc)+TString(cc2));
				h1StubsPerLayer[iLay][iMissLay][0]->SetLineWidth(2);
				h1StubsPerLayer[iLay][iMissLay][0]->SetXTitle("# stubs/layer/road");
				h1StubsPerLayer[iLay][iMissLay][0]->DrawCopy();
				if (!iClean) continue;
				sprintf(cc3,"Mean # stubs: %3.2lf",h1StubsPerLayer[iLay][iMissLay][0]->GetMean());
				tl->AddEntry(h1StubsPerLayer[iLay][iMissLay][0],cc3);
				h1StubsPerLayerSingleStub[iLay][iMissLay][0]->SetLineWidth(2);
				h1StubsPerLayerSingleStub[iLay][iMissLay][0]->SetLineColor(38);
				h1StubsPerLayerSingleStub[iLay][iMissLay][0]->DrawCopy("same");
				sprintf(cc3,"Mean # stubs bug fix: %3.2lf",h1StubsPerLayerSingleStub[iLay][iMissLay][0]->GetMean());
				tl->AddEntry(h1StubsPerLayerSingleStub[iLay][iMissLay][0],cc3);
				if (deriveCuts) continue;
				h1StubsPerLayer[iLay][iMissLay][1]->SetLineWidth(2);
				h1StubsPerLayer[iLay][iMissLay][1]->SetLineColor(2);
				h1StubsPerLayer[iLay][iMissLay][1]->DrawCopy("same");
				sprintf(cc3,"Mean # stubs clean: %3.1lf",h1StubsPerLayer[iLay][iMissLay][1]->GetMean());
				tl->AddEntry(h1StubsPerLayer[iLay][iMissLay][1],cc3);
				h1StubsPerLayerSingleStub[iLay][iMissLay][1]->SetLineWidth(2);
				h1StubsPerLayerSingleStub[iLay][iMissLay][1]->SetLineColor(46);
				h1StubsPerLayerSingleStub[iLay][iMissLay][1]->DrawCopy("same");
				sprintf(cc3,"Mean # stubs bug fix clean: %3.2lf",h1StubsPerLayerSingleStub[iLay][iMissLay][1]->GetMean());
				tl->AddEntry(h1StubsPerLayerSingleStub[iLay][iMissLay][1],cc3);
				tl->Draw("APL");
			}
			sName =(dirPlots+cStubsPerLayer[iMissLay][iClean]->GetName()+TString("_")+pName+TString(".png"));
			if (savePlots) cStubsPerLayer[iMissLay][iClean]->SaveAs(sName);
		}
	}


	double  percentiles   [3] = {0.01, 0.5, 0.95};
	double xpercentiles[6][3];
	TCanvas* cRoadPerEvent[3]; // 6oo6, 5oo6 per layer, 5or6oo6 summary
	TCanvas* cCombPerRoad [3];
	TCanvas* cCombPerEvent[3];
	for (unsigned iLay=0; iLay <3; ++iLay) {
		if (nLayers==6 && iLay) continue;
		char cc [50];
		char cc2[50];
		if      (iLay==0) sprintf(cc2," 6 oo 6");
		else if (iLay==1) sprintf(cc2," 5 oo 6 by layer");
		else if (iLay==2) sprintf(cc2," 5or6 oo 6");
		sprintf(cc,"_iMiss_%u",iLay);
		TString sName (TString("cRoadsPerEvent")+TString(cc));
		TString sTitle(TString("Roads per event")+TString(cc2));
		cRoadPerEvent[iLay] = new TCanvas(sName,sTitle,0,0,900,900);
		sName  = (TString("cCombPerRoad")+TString(cc));
		sTitle = (TString("Combs per road")+TString(cc2));
		cCombPerRoad [iLay] = new TCanvas(sName,sTitle,0,0,900,900);
		sName  = (TString("cCombPerEvent")+TString(cc));
		sTitle = (TString("Combs per event")+TString(cc2));
		cCombPerEvent[iLay] = new TCanvas(sName,sTitle,0,0,900,900);
	}
	for (unsigned iMissLay=0; iMissLay <nLayersLoop; ++iMissLay) {
		if (nLayers==6 && iMissLay) continue;
		unsigned iPick=0;
		if (iMissLay==nLayersLoop-1) iPick=2;
		char cc [150];
		char cc2[150];
		char cc3[150];
		TLegend * tlr=new TLegend(0.3,0.7,0.9,0.9);
		TLegend * tlc=new TLegend(0.3,0.7,0.9,0.9);
		TLegend * tle=new TLegend(0.3,0.7,0.9,0.9);
		for (unsigned iClean=0; iClean<2; ++iClean) {
			if (iMissLay==0 || iMissLay==nLayersLoop-1) {

				cRoadPerEvent[iPick]->cd();
				if      (iMissLay==0)             {
					sprintf(cc2," 6 oo 6");
					sprintf(cc ,"_6oo6_");
				}
				else if (iMissLay==nLayersLoop-1) {
//					cRoadPerEvent[2]       ->cd();
					sprintf(cc2," 5or6 oo 6");
					sprintf(cc ,"_5or6oo6_");
				}
				else continue;
//				else {
//					sprintf(cc2," 5 oo 6 Miss Layer %u",iMissLay-1);
//					sprintf(cc ,"_5oo6_MissLay_%u"     ,iMissLay-1);
//				}
				gPad->SetLogy();
				gPad->SetGrid();
				h1RoadPerEvent          [iMissLay][iClean]->SetLineWidth(2);
				h1RoadPerEventSingleStub[iMissLay][iClean]->SetLineWidth(2);
				h1RoadPerEvent[iMissLay][iClean]->GetQuantiles(3,xpercentiles[0],percentiles);
				if (iClean) {
					h1RoadPerEvent[iMissLay][iClean]->SetLineColor(2);
					h1RoadPerEvent[iMissLay][iClean]->DrawCopy("same");
					sprintf(cc3,"# roads clean. #mu=%3.1lf, #Lambda_{95}=%3.1lf",h1RoadPerEvent[iMissLay][iClean]->GetMean(),xpercentiles[0][2]);
					tlr->AddEntry(h1RoadPerEvent[iMissLay][iClean],cc3);
//					h1RoadPerEventSingleStub[iMissLay][iClean]->SetLineColor(46);
//					h1RoadPerEventSingleStub[iMissLay][iClean]->DrawCopy("same");
//					h1RoadPerEventSingleStub[iMissLay][iClean]->GetQuantiles(3,xpercentiles[0],percentiles);
//					sprintf(cc3,"# roads bug fix clean. #mu=%3.1lf, #Lambda_{95}=%3.1lf",h1RoadPerEventSingleStub[iMissLay][iClean]->GetMean(),xpercentiles[0][2]);
//					tlr->AddEntry(h1RoadPerEventSingleStub[iMissLay][iClean],cc3);
				}
				else {
					h1RoadPerEvent[iMissLay][1]     ->DrawCopy();
					h1RoadPerEvent[iMissLay][1]     ->SetXTitle("# roads/tower/BX");
					h1RoadPerEvent[iMissLay][1]     ->SetTitle (TString("Roads per Tower")+cc2);
					h1RoadPerEvent[iMissLay][iClean]->DrawCopy("same");
					sprintf(cc3,"# roads. #mu=%3.1lf, #Lambda_{95}=%3.1lf",h1RoadPerEvent[iMissLay][iClean]->GetMean(),xpercentiles[0][2]);
					tlr->AddEntry(h1RoadPerEvent[iMissLay][iClean],cc3);
//					h1RoadPerEventSingleStub[iMissLay][iClean]->SetLineColor(38);
//					h1RoadPerEventSingleStub[iMissLay][iClean]->DrawCopy("same");
//					h1RoadPerEventSingleStub[iMissLay][iClean]->GetQuantiles(3,xpercentiles[0],percentiles);
//					sprintf(cc3,"# roads bug fix #mu=%3.1lf, #Lambda_{95}=%3.1lf",h1RoadPerEventSingleStub[iMissLay][iClean]->GetMean(),xpercentiles[0][2]);
//					tlr->AddEntry(h1RoadPerEventSingleStub[iMissLay][iClean],cc3);
				}
				tlr->Draw("APL");

				cCombPerRoad[iPick]->cd();
				gPad->SetLogy();
				gPad->SetGrid();
				h1CombPerRoad          [iMissLay][iClean]->SetLineWidth(2);
				h1CombPerRoadSingleStub[iMissLay][iClean]->SetLineWidth(2);
				h1CombPerRoad[iMissLay][iClean]->GetQuantiles(3,xpercentiles[0],percentiles);
				if (iClean) {
					h1CombPerRoad[iMissLay][iClean]->SetLineColor(2);
					h1CombPerRoad[iMissLay][iClean]->DrawCopy("same");
					sprintf(cc3,"# combs clean. #mu=%3.1lf, #Lambda_{95}=%3.1lf",h1CombPerRoad[iMissLay][iClean]->GetMean(),xpercentiles[0][2]);
					tlc->AddEntry(h1CombPerRoad[iMissLay][iClean],cc3);
//					h1CombPerRoadSingleStub[iMissLay][iClean]->SetLineColor(46);
//					h1CombPerRoadSingleStub[iMissLay][iClean]->DrawCopy("same");
//					h1CombPerRoadSingleStub[iMissLay][iClean]->GetQuantiles(3,xpercentiles[0],percentiles);
//					sprintf(cc3,"# combs bug fix clean. #mu=%3.1lf, #Lambda_{95}=%3.1lf",h1CombPerRoadSingleStub[iMissLay][iClean]->GetMean(),xpercentiles[0][2]);
//					tlc->AddEntry(h1CombPerRoadSingleStub[iMissLay][iClean],cc3);
				}
				else {
					h1CombPerRoad[iMissLay][1]     ->DrawCopy();
					h1CombPerRoad[iMissLay][1]     ->SetXTitle("# combs/road/BX");
					h1CombPerRoad[iMissLay][1]     ->SetTitle (TString("Combs per Road")+cc2);
					h1CombPerRoad[iMissLay][iClean]->DrawCopy("same");
					sprintf(cc3,"# combs. #mu=%3.1lf, #Lambda_{95}=%3.1lf",h1CombPerRoad[iMissLay][iClean]->GetMean(),xpercentiles[0][2]);
					tlc->AddEntry(h1CombPerRoad[iMissLay][iClean],cc3);
//					h1CombPerRoadSingleStub[iMissLay][iClean]->SetLineColor(38);
//					h1CombPerRoadSingleStub[iMissLay][iClean]->DrawCopy("same");
//					h1CombPerRoadSingleStub[iMissLay][iClean]->GetQuantiles(3,xpercentiles[0],percentiles);
//					sprintf(cc3,"# combs bug fix #mu=%3.1lf, #Lambda_{95}=%3.1lf",h1CombPerRoadSingleStub[iMissLay][iClean]->GetMean(),xpercentiles[0][2]);
//					tlc->AddEntry(h1CombPerRoadSingleStub[iMissLay][iClean],cc3);
				}
				tlc->Draw("APL");

				cCombPerEvent[iPick]->cd();
				gPad->SetLogy();
				gPad->SetGrid();
				h1CombPerEvent          [iMissLay][iClean]->SetLineWidth(2);
				h1CombPerEventSingleStub[iMissLay][iClean]->SetLineWidth(2);
				h1CombPerEvent[iMissLay][iClean]->GetQuantiles(3,xpercentiles[0],percentiles);
				if (iClean) {
					h1CombPerEvent[iMissLay][iClean]->SetLineColor(2);
					h1CombPerEvent[iMissLay][iClean]->DrawCopy("same");
					sprintf(cc3,"# combs clean. #mu=%3.1lf, #Lambda_{95}=%3.1lf",h1CombPerEvent[iMissLay][iClean]->GetMean(),xpercentiles[0][2]);
					tle->AddEntry(h1CombPerEvent[iMissLay][iClean],cc3);
//					h1CombPerEventSingleStub[iMissLay][iClean]->SetLineColor(46);
//					h1CombPerEventSingleStub[iMissLay][iClean]->DrawCopy("same");
//					h1CombPerEventSingleStub[iMissLay][iClean]->GetQuantiles(3,xpercentiles[0],percentiles);
//					sprintf(cc3,"# combs bug fix clean. #mu=%3.1lf, #Lambda_{95}=%3.1lf",h1CombPerEventSingleStub[iMissLay][iClean]->GetMean(),xpercentiles[0][2]);
//					tle->AddEntry(h1CombPerEventSingleStub[iMissLay][iClean],cc3);
				}
				else {
					h1CombPerEvent[iMissLay][1]     ->DrawCopy();
					h1CombPerEvent[iMissLay][1]     ->SetXTitle("# combs/tower/BX");
					h1CombPerEvent[iMissLay][1]     ->SetTitle (TString("Combs per Tower per BX")+cc2);
					h1CombPerEvent[iMissLay][iClean]->DrawCopy("same");
//					if (iClean==0) cout << h1CombPerEvent[iMissLay][iClean]->GetMean() <<endl;
					sprintf(cc3,"# combs. #mu=%3.1lf, #Lambda_{95}=%3.1lf",h1CombPerEvent[iMissLay][iClean]->GetMean(),xpercentiles[0][2]);
					tle->AddEntry(h1CombPerEvent[iMissLay][iClean],cc3);
//					h1CombPerEventSingleStub[iMissLay][iClean]->SetLineColor(38);
//					h1CombPerEventSingleStub[iMissLay][iClean]->DrawCopy("same");
//					h1CombPerEventSingleStub[iMissLay][iClean]->GetQuantiles(3,xpercentiles[0],percentiles);
//					sprintf(cc3,"# combs bug fix #mu=%3.1lf, #Lambda_{95}=%3.1lf",h1CombPerEventSingleStub[iMissLay][iClean]->GetMean(),xpercentiles[0][2]);
//					tle->AddEntry(h1CombPerEventSingleStub[iMissLay][iClean],cc3);
				}
				tle->Draw("APL");

			}
		}
		if (savePlots && (iMissLay==0 || iMissLay==nLayersLoop-1) ) {
			sName=(dirPlots+TString("roadsPerEvent")+TString(cc)+pName+TString(".png"));
			cRoadPerEvent [iPick]->SaveAs(sName);
			sName=(dirPlots+TString("combsPerRoad")+TString(cc)+pName+TString(".png"));
			cCombPerRoad  [iPick]->SaveAs(sName);
			sName=(dirPlots+TString("combsPerEvent")+TString(cc)+pName+TString(".png"));
			cCombPerEvent [iPick]->SaveAs(sName);
		}
	}

	if (nLayers==5) {
		cRoadPerEvent[1]->Divide(3,2);
		cCombPerRoad [1]->Divide(3,2);
		cCombPerEvent[1]->Divide(3,2);
		for (unsigned iMissLay=1; iMissLay <nLayersLoop-1; ++iMissLay) {
			cRoadPerEvent[1]->cd(iMissLay);
			gPad->SetLogy();
			for (unsigned iClean=0; iClean<2; ++iClean) {
				h1RoadPerEvent[iMissLay][iClean]->SetLineWidth(2);
				if (iClean) {
					h1RoadPerEvent[iMissLay][iClean]->SetLineColor(2);
					h1RoadPerEvent[iMissLay][iClean]->DrawCopy("same");
				}
				else {
					h1RoadPerEvent[iMissLay][1]     ->DrawCopy();
					h1RoadPerEvent[iMissLay][iClean]->DrawCopy("same");
				}
			}
			cCombPerRoad[1]->cd(iMissLay);
			gPad->SetLogy();
			for (unsigned iClean=0; iClean<2; ++iClean) {
				h1CombPerRoad[iMissLay][iClean]->SetLineWidth(2);
				if (iClean) {
					h1CombPerRoad[iMissLay][iClean]->SetLineColor(2);
					h1CombPerRoad[iMissLay][iClean]->DrawCopy("same");
				}
				else {
					h1CombPerRoad[iMissLay][1]     ->DrawCopy();
					h1CombPerRoad[iMissLay][iClean]->DrawCopy("same");
				}
			}
			cCombPerEvent[1]->cd(iMissLay);
			gPad->SetLogy();
			for (unsigned iClean=0; iClean<2; ++iClean) {
				h1CombPerEvent[iMissLay][iClean]->SetLineWidth(2);
				if (iClean) {
					h1CombPerEvent[iMissLay][iClean]->SetLineColor(2);
					h1CombPerEvent[iMissLay][iClean]->DrawCopy("same");
				}
				else {
					h1CombPerEvent[iMissLay][1]     ->DrawCopy();
					h1CombPerEvent[iMissLay][iClean]->DrawCopy("same");
				}
			}
		}
	}

	if (!deriveCuts) return;

	// using a TH2 to store the stub InvPt cuts. Y axis: bin 1 is for lowercut, bin 2 uppercut
	TH2F* hStubPtCuts[6][7];
	for (unsigned iLay=0; iLay <6; ++iLay) {
		char cc[50];
		sprintf(cc,"_Layer_%u",iLay);
		for (unsigned iMissLay=0; iMissLay<7; ++iMissLay) {
			if (nLayers==6 and iMissLay) continue;
			char cc2[50];
			if (iMissLay) sprintf(cc2,"_5oo6_NoLayer_%u",iMissLay-1);
			else          sprintf(cc2,"_6oo6");
			sName=TString("hStubPtCuts")+TString(cc2)+TString(cc);
			hStubPtCuts[iLay][iMissLay] = new TH2F(sName,sName,nBinsInvPt,-maxRoadInvPt,maxRoadInvPt,2,0,2);
		}
	}
	TCanvas* cInvPtDeltas[7];
	TH1D* h1roadPt [7][nBinsInvPt][7];
	TCanvas* cProjroadPt[6][7];
	unsigned iLayerSelect = 5;
	unsigned iInvPtBinSelect = 16;
	TGraph*  gProjroadPt[6][7];
	TMultiGraph *mg[7];
	TArrow* arrow_l=0;
	TArrow* arrow_h=0;
	double x[nBinsInvPt]; double y[nBinsInvPt];
	double nSigmas = 4.0;
//	double slopes1[6] = {3.26284, 3.16585, 4.5818, 7.76339, 10.0931, 12.2553};
//	double slopes3[6] = {0.674352, 0.311865, 0.779656, 1.18605, 1.92633, 3.13197};
//	double halfwidths[6] = {1.61575, 1.56367, 1.61041, 1.59697, 1.70294, 1.68591};

	TMultiGraph* mgCuts[7];
	TGraphErrors* gCuts[6][7];
	TH1*    h1CutWidths[6][7];
	TCanvas* cmgCuts   [7];
	TCanvas* cCutWidths[7];
	for (unsigned iMissLay=0; iMissLay<7; ++iMissLay) {
		if (nLayers==6 and iMissLay) continue;
		mg[iMissLay] = new TMultiGraph();
		char cc2[50];
		if (iMissLay) sprintf(cc2,"_5oo6_NoLayer_%u",iMissLay-1);
		else          sprintf(cc2,"_6oo6");
		cInvPtDeltas[iMissLay] = new TCanvas(cc2,cc2,0,0,1000,1000);
		cInvPtDeltas[iMissLay]->Divide(3,2);
		mgCuts      [iMissLay] = new TMultiGraph();
		for (unsigned iLay=0; iLay <6; ++iLay) {
			if (1+iLay==iMissLay) continue;
			cInvPtDeltas[iMissLay]->cd(iLay+1);
			gPad->SetLogz();
			h2StubPtroadPt[iLay][iMissLay]->SetMinimum(1);
			h2StubPtroadPt[iLay][iMissLay]->DrawCopy("colz0");
			pStubPtroadPt [iLay][iMissLay]->DrawCopy("same");
			char cc[100];
			sprintf(cc,"_Layer_%u",iLay);
			sName=TString("cProjroadPt")+TString(cc2)+TString(cc);
			if (1+iLay!=iMissLay) {
				if (isPlottose) cProjroadPt[iLay][iMissLay] = new TCanvas(sName,sName,50,300,1000,600);
				if (isPlottose) cProjroadPt[iLay][iMissLay]->Divide(10,3);
			}
			sName=TString("h1CutWidths")+TString(cc2)+TString(cc);
			h1CutWidths[iLay][iMissLay] = new TH1D(sName,sName,nBinsInvPt,-maxRoadInvPt,maxRoadInvPt);
//		cProjroadPt[iLay]->SetRightMargin(0.2);

			double stubBendVSinvPtPeak_Maxima   [7][nBinsInvPt];
			double stubBendVSinvPtPeak_MaximaErr[7][nBinsInvPt];
			double zeros                        [nBinsInvPt];
			double averageCutWidth = 0;

			for (unsigned iBin=0; iBin <nBinsInvPt; ++iBin) {
				sprintf(cc,"_Layer%u_%u",iLay,iBin);
				h1roadPt[iLay][iBin][iMissLay] = h2StubPtroadPt[iLay][iMissLay]->ProjectionY(TString("h2stubInvPt_VS_roadInvPt")+TString(cc2)+TString(cc),iBin+1,iBin+1);
				double max  = h1roadPt[iLay][iBin][iMissLay]->GetMaximum();
				double mean = h1roadPt[iLay][iBin][iMissLay]->GetBinCenter(h1roadPt[iLay][iBin][iMissLay]->GetMaximumBin());
				double rms  = h1roadPt[iLay][iBin][iMissLay]->GetRMS();
				fgau->SetParameter(0,max);
				fgau->SetParameter(1,mean);
				fgau->SetParameter(2,rms);
				if (isPlottose) cProjroadPt[iLay][iMissLay]->cd(iBin+1);
				if (isPlottose) gPad->SetLogy();
//				h1roadPt[iLay][iBin][iMissLay]->DrawCopy();
				if (isPlottose) h1roadPt[iLay][iBin][iMissLay]->Fit(fgau,"M","", mean-2.8*rms,mean+2.8*rms);
				else            h1roadPt[iLay][iBin][iMissLay]->Fit(fgau,"M0","", mean-2.8*rms,mean+2.8*rms);
				h1roadPt[iLay][iBin][iMissLay]->SetLineWidth(2.0);
				if (isPlottose) h1roadPt[iLay][iBin][iMissLay]->DrawCopy();
				sprintf(cc,"%5.3lf .lt. invPt .lt. %5.3lf", h2StubPtroadPt[iLay][iMissLay]->GetXaxis()->GetBinLowEdge(iBin+1), h2StubPtroadPt[iLay][iMissLay]->GetXaxis()->GetBinLowEdge(iBin+2));
				TText* tt = new TText();
				if (isPlottose) tt->DrawTextNDC(0.6,0.6,cc);
				x[iBin]     = h2StubPtroadPt[iLay][iMissLay]->GetXaxis()->GetBinCenter(iBin+1);
				double integral   = h1roadPt[iLay][iBin][iMissLay]->Integral(1,h1roadPt[iLay][iBin][iMissLay]->GetNbinsX()+1);
				double xMax =  1.02*h1roadPt[iLay][iBin][iMissLay]->GetXaxis()->GetXmax();
				double lowCut  = fgau->GetParameter(1)-nSigmas*fgau->GetParameter(2);
				double highCut = fgau->GetParameter(1)+nSigmas*fgau->GetParameter(2);
//				stubBendVSinvPtCuts         [iLay][iMissLay] =         fgau->GetParameter(1);
//				stubBendVSinvPtCuts_Err     [iLay][iMissLay] = nSigmas*fgau->GetParameter(2);
				unsigned int minBin = TMath::Max(0                                          ,h1roadPt[iLay][iBin][iMissLay]->FindBin( lowCut));
				unsigned int maxBin = TMath::Min(h1roadPt[iLay][iBin][iMissLay]->GetNbinsX(),h1roadPt[iLay][iBin][iMissLay]->FindBin(highCut));
				arrow_l = new TArrow(lowCut ,max/3,lowCut ,0,0.02);
				arrow_l->SetLineWidth(2.0);
				if (isPlottose) arrow_l->DrawClone();
				arrow_h = new TArrow(highCut,max/3,highCut,0,0.02);
				arrow_h->SetLineWidth(2.0);
				if (isPlottose) arrow_h->DrawClone();

//			if (iBin==iInvPtBinSelect && iLay==iLayerSelect) {
//				cProjroadPtSingle->cd();
//				gPad->SetLogy();
//				sprintf(cc,"Layer %u", iLay);
//				h1roadPt[iLay][iBin]->SetTitle(cc);
//				h1roadPt[iLay][iBin]->DrawCopy();
//				sprintf(cc,"%5.3lf < Pt^{-1} < %5.3lf", h2StubPtroadPt[iLay]->GetXaxis()->GetBinLowEdge(iBin+1), h2StubPtroadPt[iLay]->GetXaxis()->GetBinLowEdge(iBin+2));
//				TLatex* tt = new TLatex();
//				tt->DrawLatexNDC(0.4,0.85,cc);
//				arrow_l->DrawClone();
//				arrow_h->DrawClone();
//				sprintf(cc,"Single_DsCutsLayer_Layer%u_",iLay);
//				if (savePlots && deriveCuts) {
//					cProjroadPtSingle->SaveAs(dirPlots+TString(cc)+TString(".png"));
//					cProjroadPtSingle->SaveAs(dirPlots+TString(cc)+TString(".pdf"));
//				}
//			}

//				double efficiency = h1roadPt[iLay][iBin][iMissLay]->Integral(minBin,maxBin);
//				efficiency/=integral;
//				y[iBin] = efficiency;
				averageCutWidth+=highCut-lowCut;
//				//			cout << "## " << iLay << "\t" << iBin << "\t" << efficiency << endl;
//				// store cuts
				hStubPtCuts[iLay][iMissLay]->SetBinContent(iBin+1,1, lowCut);
				hStubPtCuts[iLay][iMissLay]->SetBinContent(iBin+1,2,highCut);
				stubBendVSinvPtPeak_Maxima   [iMissLay][iBin] = fgau->GetParameter(1);
				stubBendVSinvPtPeak_MaximaErr[iMissLay][iBin] = nSigmas*fgau->GetParameter(2);
				h1CutWidths            [iLay][iMissLay]->SetBinContent(iBin+1,stubBendVSinvPtPeak_MaximaErr[iMissLay][iBin]);
								  zeros                        [iBin] = 0;
			}
			sprintf(cc,"DsCutsLayer_Layer%u_",iLay);
			if (savePlots && deriveCuts && isPlottose) {
				cProjroadPt[iLay][iMissLay]->SaveAs(dirPlots+TString(cc2)+TString(cc)+TString(".png"));
				cProjroadPt[iLay][iMissLay]->SaveAs(dirPlots+TString(cc2)+TString(cc)+TString(".pdf"));
			}

			gProjroadPt [iLay][iMissLay] = new TGraph(nBinsInvPt,x,y);
			gProjroadPt [iLay][iMissLay]->SetMarkerStyle(20);
			gProjroadPt [iLay][iMissLay]->SetMarkerColor(iLay+1);
			mg[iMissLay]->Add(gProjroadPt [iLay][iMissLay]);
//		//		cout << "## " << iLay << "\t" << averageCutWidth/nBinsRoughInvPt << "\t" << slopes1[iLay] << " : " << slopes3[iLay] << endl;
			gCuts       [iLay][iMissLay] = new TGraphErrors    (nBinsInvPt,x,stubBendVSinvPtPeak_Maxima[iMissLay],zeros,stubBendVSinvPtPeak_MaximaErr[iMissLay]);
			gCuts       [iLay][iMissLay]->SetMarkerStyle(20);
			gCuts       [iLay][iMissLay]->SetMarkerColor(iLay+1);
			gCuts       [iLay][iMissLay]->SetLineColor(iLay+1);
			gCuts       [iLay][iMissLay]->SetLineWidth(2);
			mgCuts            [iMissLay]->Add(gCuts     [iLay][iMissLay]);
			gCuts       [iLay][iMissLay]->Fit(fp3,"M");
			slopes1 [iLay][iMissLay] = fp3->GetParameter(0);
			slopes3 [iLay][iMissLay] = fp3->GetParameter(1);
			gCuts       [iLay][iMissLay]->Fit(fp1,"M");
			slopes11[iLay][iMissLay] = fp1->GetParameter(0);
			halfwidths[iLay][iMissLay] = 0.5*averageCutWidth/nBinsInvPt;
			hslopes11  ->SetBinContent(iLay+1,iMissLay+1,slopes11  [iLay][iMissLay]);
			hhalfwidths->SetBinContent(iLay+1,iMissLay+1,halfwidths[iLay][iMissLay]);
			//
//		if (!deriveCuts) {
//			slopes1[0] = 3.26271; slopes3[0] = 0.675149; halfwidths[0] = 1.6157;
//			slopes1[1] = 3.16585; slopes3[1] = 0.312155; halfwidths[1] = 1.56365;
//			slopes1[2] = 4.58175; slopes3[2] = 0.779597; halfwidths[2] = 1.61038;
//			slopes1[3] = 7.76365; slopes3[3] = 1.18481; halfwidths[3] = 1.59679;
//			slopes1[4] = 10.093; slopes3[4] = 1.92707; halfwidths[4] = 1.7028;
//			slopes1[5] = 12.2553; slopes3[5] = 3.1327; halfwidths[5] = 1.68588;
//		}
//
//		TAxis* t = hStubPtCuts[0]->GetXaxis();
//		for (unsigned iBin=0; iBin < t->GetNbins(); ++iBin) {
//			cout << iLay << "\t" << hStubPtCuts[iLay]->GetXaxis()->GetBinCenter(iBin+1) << "\t" << hStubPtCuts[iLay]->GetBinContent(iBin+1,1) << "\t" << hStubPtCuts[iLay]->GetBinContent(iBin+1,2) << endl;
//		}
		}
		if (iMissLay) sprintf(cc2,"cCuts_5oo6_NoLayer_%u",iMissLay-1);
		else          sprintf(cc2,"cCuts_6oo6");
		cmgCuts           [iMissLay] = new TCanvas(cc2,cc2,0,400,1000,1000);
		cmgCuts           [iMissLay]->cd();
		cmgCuts           [iMissLay]->SetGrid();
		mgCuts            [iMissLay]->Draw("APL");
		mgCuts            [iMissLay]->GetXaxis()->SetTitle("road mean invPt [c/GeV]");
		TLegend* leg = new TLegend(0.5,0.1,0.9,0.3);
		for (unsigned iLay=0; iLay <6; ++iLay) {
			if (1+iLay==iMissLay) continue;
			char cc[100];
			sprintf(cc,"Layer_%u",iLay);
			TLegendEntry* tle=leg->AddEntry(gCuts [iLay][iMissLay],cc,"LP");
			tle->SetLineColor(gCuts [iLay][iMissLay]->GetLineColor());
		}
		leg->Draw();
		if (savePlots) {
			cmgCuts           [iMissLay]->SaveAs(dirPlots+TString(cc2)+TString(".png"));
		}

		if (iMissLay) sprintf(cc2,"cCutWidths_5oo6_NoLayer_%u",iMissLay-1);
		else          sprintf(cc2,"cCutWidths_6oo6");
		cCutWidths           [iMissLay] = new TCanvas(cc2,cc2,0,400,1000,1000);
		cCutWidths           [iMissLay]->SetGrid();
		cCutWidths           [iMissLay]->cd();
		TLegend* lCuts = new TLegend(0.1,0.1,0.9,0.4);
		TLegendEntry* tle;
		bool firstLay=true;
		for (unsigned iLay=0; iLay <6; ++iLay) {
			if (1+iLay==iMissLay) continue;
			h1CutWidths[iLay][iMissLay]->SetMinimum(1);
			h1CutWidths[iLay][iMissLay]->SetMaximum(2);
			h1CutWidths[iLay][iMissLay]->SetLineColor(iLay+40);
			h1CutWidths[iLay][iMissLay]->SetTitle("");
			h1CutWidths[iLay][iMissLay]->SetXTitle("road mean invPt [c/GeV]");
			char cc[100];
			if (iMissLay) {
				sprintf(cc,"#Deltas cut width-Lay %u-Miss Lay %u",iLay,iMissLay-1);
				if (1+iLay==iMissLay) continue;
				if (firstLay) {
					h1CutWidths[iLay][iMissLay]->DrawCopy();
					firstLay=false;
				}
				else h1CutWidths[iLay][iMissLay]->DrawCopy("same");
			}
			else {
				sprintf(cc,"#Deltas cut width-Lay %u-6/6",iLay);
				if (iLay) h1CutWidths[iLay][iMissLay]->DrawCopy("same");
				else      h1CutWidths[iLay][iMissLay]->DrawCopy();
			}
			tle = lCuts->AddEntry(h1CutWidths[iLay][iMissLay],cc,"LP");
			tle->SetLineColor(h1CutWidths[iLay][iMissLay]->GetLineColor());
//			if (iLay && (iLay != iMissLay)) h1CutWidths[iLay][iMissLay]->DrawCopy("same");
//			else          h1CutWidths[iLay][iMissLay]->DrawCopy();

			if (deriveCuts)
			for (unsigned iMLay=0; iMLay<7; ++iMLay) {
				if (nLayers==6 and iMLay) continue;
//				if (1+iLay==iMLay) continue;

//				cout <<   "slopes1 ["   <<iLay<<"]["<<iMLay<<"] = "    << slopes1   [iLay][iMLay]
//                     << "; slopes3 ["   <<iLay<<"]["<<iMLay<<"] = "    << slopes3   [iLay][iMLay]
//                     << "; halfwidths["<<iLay<<"]["<<iMLay<<"] = "    << halfwidths[iLay][iMLay]
//					  << ";" << endl;
				cout <<   "slopes11["  <<iLay<<"]["<<iMLay<<"] = "    << slopes11  [iLay][iMLay]
                     << "; halfwidths["<<iLay<<"]["<<iMLay<<"] = "    << halfwidths[iLay][iMLay]
					  << ";" << endl;
			}
		}
		lCuts->Draw();
		if (savePlots) {
			char cc[100];
			if (iMissLay)   sprintf(cc,"deltaScutWidths_5oo6_MissLayer%u",iMissLay-1);
			if (nLayers==6) sprintf(cc,"deltaScutWidths_6oo6");
			cCutWidths           [iMissLay]->SaveAs(dirPlots+TString(cc)+TString(".png"));
			cCutWidths           [iMissLay]->SaveAs(dirPlots+TString(cc)+TString(".pdf"));
		}
	}
	if (deriveCuts) {
		TFile* fOutCuts = new TFile("deltaScuts.root","RECREATE");
		fOutCuts->cd();
		if (fOutCuts->IsOpen()) {
			hslopes11->Write();
			hhalfwidths->Write();
			fOutCuts->Close();
		}
	}


//	TCanvas* cCuts = new TCanvas("cCuts","cCuts",0,0,800,800);
//	cCuts->SetGrid();
//	mgCuts->Draw("APL");
//	TLegend* leg = new TLegend(0.5,0.1,0.9,0.3);
//	for (unsigned iLay=0; iLay <6; ++iLay) {
//		char cc[10];
//		sprintf(cc,"Layer_%u",iLay);
//		leg->AddEntry(gCuts [iLay],cc,"LP");
//	}
//	leg->Draw();

//	TCanvas* cEffs = new TCanvas("cEffs","cEffs",0,0,800,800);
//	cEffs->SetGrid();
//	mg->Draw("APL");
//	leg = new TLegend(0.7,0.1,0.9,0.3);
////	leg->SetHeader("The Legend Title");
//	for (unsigned iLay=0; iLay <6; ++iLay) {
//		char cc[10];
//		sprintf(cc,"Layer_%u",iLay);
//		leg->AddEntry(gProjroadPt [iLay],cc,"LP");
//	}
//	leg->Draw();

	if (savePlots && deriveCuts) {
		for (unsigned iMissLay=0; iMissLay<7; ++iMissLay) {
			if (iMissLay && nLayers==6) continue;
			char cc2[50];
			if (iMissLay) sprintf(cc2,"_5oo6_NoLayer_%u",iMissLay-1);
			else          sprintf(cc2,"_6oo6");
			cInvPtDeltas[iMissLay]->Update();
			cInvPtDeltas[iMissLay]->SaveAs(dirPlots+TString("h2stubDeltas_VS_roadInvPt")+cc2+TString(".png"));
			cInvPtDeltas[iMissLay]->SaveAs(dirPlots+TString("h2stubDeltas_VS_roadInvPt")+cc2+TString(".pdf"));
		}
	}

	TString sEffParticle("single_muon");
	if (applyOnMuon==0) sEffParticle=TString("single_pion");
	if (applyOnMuon==2) sEffParticle=TString("single_muon_PU140");

//	TAxis* tt = h1roadPt[0][0]->GetXaxis();
//	unsigned nBend = tt->GetNbins();
//	for (unsigned iPtBin=0; iPtBin <nBinsInvPt; ++iPtBin) {
//		h1roadPt[6][iPtBin] = (TH1D*) h1roadPt[0][0]->Clone();
//		for (unsigned iBend=0; iBend<nBend; ++iBend) {
//			double binContent = 0;
//			for (unsigned iLay=0; iLay <6; ++iLay) {
//				double   xPeak = (hStubPtCuts[iLay]->GetBinContent(iPtBin+1,1)+hStubPtCuts[iLay]->GetBinContent(iPtBin+1,2))/2.0;
//				unsigned iPeak = tt->FindBin(xPeak);
//				int iB    = (int)(iBend)+1-(int)iPeak;
//				if (iB>0) binContent+=h1roadPt[iLay][iPtBin]->GetBinContent(iB);
////				if (!iBend) cout << iPtBin << "\t" << iBend << "\t" << iLay << "\t" << iPeak << "\t" << xPeak << "\t" << binContent << endl;
//			}
//			h1roadPt[6][iPtBin]->SetBinContent(iBend+1,binContent);
//		}
//	}
//	TCanvas* cCut = new TCanvas("cCut","cCut",0,0,800,800);
//	cCut->Divide(6,6);
//	for (unsigned iBin=0; iBin <nBinsRoughInvPt+1; ++iBin) {
//		cCut->cd(iBin+1);
//		cCut->SetLogy();
//		h1roadPt[6][iBin]->DrawCopy();
//	}

//	TCanvas* cProjroadPt1 = new TCanvas("cProjroadPt1","cProjroadPt1",0,400,800,800);
//	cProjroadPt1->Divide(5,5);
//	for (unsigned iBin=0; iBin <nBinsRoughInvPt; ++iBin) {
//		char cc[10];
//		sprintf(cc,"_%u",iBin);
//		h1roadPt1[iBin] = h2StubPt1roadPt->ProjectionY(TString("h2stub1InvPt_VS_roadInvPt")+TString(cc),iBin+1,iBin+2);
//		double max  = h1roadPt1[iBin]->GetMaximum();
//		double mean = h1roadPt1[iBin]->GetBinCenter(h1roadPt1[iBin]->GetMaximumBin());
//		double rms  = h1roadPt1[iBin]->GetRMS();
//		cout << max << "\t" << mean << "\t" << rms << endl;
//		fgau->SetParameter(0,max);
//		fgau->SetParameter(1,mean);
//		fgau->SetParameter(2,rms);
//		TPad* pad = (TPad*)cProjroadPt1->cd(iBin+1);
//		pad->SetLogy();
//		h1roadPt1[iBin]->Fit(fgau,"M","", mean-3*rms,mean+3*rms);
//		h1roadPt1[iBin]->DrawCopy();
//	}
//
//	double  percentiles   [3] = {0.01, 0.5, 0.99};
//	double xpercentiles[6][3];
//	TCanvas* cstubBend = new TCanvas("cstubBend","cstubBend",0,0,800,600);
//	cstubBend->Divide(3,2);
//	for (unsigned iBin=0; iBin <6; ++iBin) {
//		h1stubBend[iBin][0]->GetQuantiles(3,xpercentiles[iBin],percentiles);
//		cout << "Layer: " << iBin << ". Quantile Pt bend: ";
//		for (short i=0; i<3; ++i) cout << xpercentiles[iBin][i] << "\t";
//		cout << endl;
//		cstubBend->cd(iBin+1);
//		h1stubBend[iBin][0]->DrawNormalized();
//		h1stubBend[iBin][1]->DrawNormalized("same");
//	}

/*

//		TCanvas* cRoadgen = new TCanvas("cRoadgen","cRoadgen",0,0,800,800);
//	h2roadPtgenPt->DrawCopy("colz0");

	// loading file for efficiency test.
//	TString sTreeEff="results_sf1_nz1_tt27_pt2_SingleMuonFlatOneOverPt0p005To0p5_tt27_Sum2M_SL6.root";
//	TString sTreeEff="singleMuonNoTest/results_SingleMuonFlatOneOverPt0p005To0p5_tt27NoTest_cfi_py_GEN_SIM_DIGI_L1TrackTrigger_2M.root";
//	TString sTreeEff="singleMuonNoTest/results_SingleMuonFlatOneOverPt0p005To0p5_tt27NoTest_cfi_py_GEN_SIM_DIGI_L1TrackTrigger_2M.root";
//	TString sTreeEff="results_5outOf6_sf1_nz1_tt27_pt2_SingleMuonTest_PU140.root";
	TString sTreeEff="SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3/roads_sf1_nz1_tt27_pt2_5oo6_SingleMuonTest_tt27_PU0_20150408_SLHC25p3.root";
	if (applyOnMuon==0) sTreeEff=TString("results_sf1_nz1_tt27_pt2_SinglePionFlatOneOverPt0p005To0p5_tt27_Sum2M_SL6.root");
	if (applyOnMuon==2) sTreeEff=TString("results_sf1_nz1_tt27_pt2_SingleMuonTest_tt27_PU140_SL6.root");
	TTree* treeEff=0;
	roadsStubs rEff(sTreeEff,treeEff);
	if (rEff.fChain == 0) return;


	TH1*     h1roadPtcombInvPt             [7];
	TH1*     h1roadPtcombInvPt_BendClean   [7];
	TH1*     h1roadPtcombInvPt_Matched     [6];
	TH1*     h1roadPtcombInvPt_Matched_BendClean     [6];
	TH1*     h1roadPtRemovedStub           [6];
	TH1*     h1dsRemovedStub               [6];
	TH1*     h1dsRemovedMatchedStub        [6];
	h1roadPtcombInvPt           [6] = new TH1D("h1roadPtcombInvPt"          ,"h1roadPtcombInvPt"          ,500,0,500);
	h1roadPtcombInvPt_BendClean [6] = new TH1D("h1roadPtcombInvPt_BendClean","h1roadPtcombInvPt_BendClean",500,0,500);
	for (unsigned iLay=0; iLay <6; ++iLay) {
		char cc[10];
		sprintf(cc,"_%u",iLay);
		sName=TString("h1stubsInLayer")+TString(cc);
		h1roadPtcombInvPt           [iLay] = new TH1D(sName,sName,20,0,20);
		sName=TString("h1stubsInLayer_BendClean_")+TString(cc);
		h1roadPtcombInvPt_BendClean [iLay] = new TH1D(sName,sName,20,0,20);
		sName=TString("h1stubsInLayer_Matched_")+TString(cc);
		h1roadPtcombInvPt_Matched   [iLay] = new TH1D(sName,sName,20,0,20);
		sName=TString("h1roadPtcombInvPt_Matched_BendClean_")+TString(cc);
		h1roadPtcombInvPt_Matched_BendClean[iLay] = new TH1D(sName,sName,20,0,20);
		sName=TString("h1roadPtRemovedStub_")+TString(cc);
		h1roadPtRemovedStub[iLay] = new TH1D(sName,sName,200,-maxRoadInvPt,maxRoadInvPt);
		sName=TString("h1dsRemovedStub_")+TString(cc);
		h1dsRemovedStub[iLay] = new TH1D(sName,sName,200,-16,16);
		sName=TString("h1dsRemovedMatchedStub_")+TString(cc);
		h1dsRemovedMatchedStub[iLay] = new TH1D(sName,sName,200,-16,16);
	}
	TH1* h1roadPtcombInvPtPerRoad           [7];
	TH1* h1roadPtcombInvPtPerRoad_BendClean [7];
	TH1* h1Nroads                           [7];
	TH1* h1NMatchedroads                    [7];
	TH1* h1NCleanedroads                    [7];
	TH1* h1NMatchedCleanedroads             [7];
	TH1* h1NcombPerEv_Matched               [2];
	TH1* h1NcombPerEv_Matched_BendClean     [2];

	for (unsigned iMissLay=0; iMissLay <6; ++iMissLay) {
		char cc2[50];
		if (iMissLay) sprintf(cc2,"_5oo6_NoLayer_%u",iMissLay-1);
		else          sprintf(cc2,"_6oo6");
		sName=TString("h1roadPtcombInvPtPerRoad"          );
		h1roadPtcombInvPtPerRoad           [iMissLay]= new TH1D(sName+TString(cc2),sName+TString(cc2),100,0,100);
		sName=TString("h1roadPtcombInvPtPerRoad_BendClean");
		h1roadPtcombInvPtPerRoad_BendClean [iMissLay]= new TH1D(sName+TString(cc2),sName+TString(cc2),100,0,100);
		sName=TString("h1Nroads"                          );
		h1Nroads                           [iMissLay]= new TH1D(sName+TString(cc2),sName+TString(cc2),100,0,100);
		sName=TString("h1NMatchedroads"                   );
		h1NMatchedroads                    [iMissLay]= new TH1D(sName+TString(cc2),sName+TString(cc2),100,0,100);
		sName=TString("h1NCleanedroads"                   );
		h1NCleanedroads                    [iMissLay]= new TH1D(sName+TString(cc2),sName+TString(cc2),100,0,100);
		sName=TString("h1NMatchedCleanedroads"            );
		h1NMatchedCleanedroads             [iMissLay]= new TH1D(sName+TString(cc2),sName+TString(cc2),100,0,100);
		if (iMissLay) sprintf(cc2,"_5oo6");
		else          sprintf(cc2,"_6oo6");
		sName=TString("h1NcombPerEv_Matched"              );
		if (iMissLay<2) h1NcombPerEv_Matched               [iMissLay!=0]= new TH1D(sName+TString(cc2),sName+TString(cc2),500,0,500);
		sName=TString("h1NcombPerEv_Matched_BendClean"    );
		if (iMissLay<2) h1NcombPerEv_Matched_BendClean     [iMissLay!=0]= new TH1D(sName+TString(cc2),sName+TString(cc2),500,0,500);
	}
//	TH2* h2roadDeltaInvPtDeltaPhi           = new TH2D("h2roadDeltaInvPtDeltaPhi","h2roadDeltaInvPtDeltaPhi",500,-0.02,0.02,500,-0.01,0.01);

	Long64_t nentriesEff = rEff.fChain->GetEntriesFast();
	cout << "Loaded " << nentriesEff << " events for Eff test" << endl;

//	double scale = 1.5;
//	double Theta = 18;
//	double cosTheta = TMath::Cos(-Theta/57.29578) ;
//	double sinTheta = TMath::Sin(-Theta/57.29578);
//	double axMin   = 0.001*scale;
//	double axMax   = 0.006*scale;

	unsigned long int nEv        = 0;
	unsigned long int nEvCleaned = 0;
	unsigned long int nEvMatchedRoad = 0;
	unsigned long int nEvMatchedRoadCleaned = 0;
	vector <unsigned> nCombPerRoad;
	vector <unsigned> nCombPerRoad_BendClean;
//	for (Long64_t jentry=0; jentry<nentriesEff;jentry++) {
	for (Long64_t jentry=0; jentry<200000;jentry++) {
		Long64_t ientry = rEff.LoadTree(jentry);
		if (ientry < 0) break;
		nb = rEff.fChain->GetEntry(jentry);   nbytes += nb;
//		unsigned int nPart  = rEff.genParts_pt     ->size();
//		if (nPart!=1) continue;
//		unsigned int nTotStubs = r.TTStubs_r       ->size();
		unsigned int nRoads = rEff.AMTTRoads_nstubs->size();
		if (!nRoads) {
//			++nEv;
			continue;
		}

		double genPartInvPt = -999;
		double genPartPhi   = -999;

		if (rEff.genParts_pt->size()) {
			genPartInvPt = rEff.genParts_charge->at(0)/rEff.genParts_pt ->at(0);
			genPartPhi   = rEff.genParts_phi   ->at(0);
		}
		else continue;

		unsigned short nCleanRoads   = 0;
		unsigned short nMatchedRoads = 0;
		unsigned short nCleanMatchedRoads = 0;
		unsigned count = 0;
//		unsigned short maxFreq = 1;
		unsigned short nCombinationsInvPt_PerEvent          = 0;
		unsigned short nCombinationsInvPt_PerEvent_Matched  = 0;
		unsigned short nCombinationsInvPt_BendCleanPerEvent = 0;
		unsigned short nCombinationsInvPt_PerEvent_BendClean_Matched = 0;

		nCombPerRoad.clear();
		nCombPerRoad_BendClean.clear();

		unsigned short nRoadsInEvent[7]={0, 0, 0, 0, 0, 0, 0};
		unsigned short nRoadsInEvent[7]={0, 0, 0, 0, 0, 0, 0};
		for (unsigned int iRoads=0; iRoads<nRoads; ++iRoads) {
			slhcl1tt::TTRoad road;
			road.stubRefs      = rEff.AMTTRoads_stubRefs     ->at(iRoads);
			road.superstripIds = rEff.AMTTRoads_superstripIds->at(iRoads);
			road.nstubs        = rEff.AMTTRoads_nstubs       ->at(iRoads);
//			road.patternRef    = rEff.AMTTRoads_patternRef   ->at(iRoads);

			std::map <short int,unsigned int> stubLayers_map                  ;
//			std::map <short int,unsigned int> stubLayers_map_Matched;
			std::map <short int,unsigned int> stubLayers_map_BendClean_Matched;

			vector <short unsigned> vstubsPerLayer          (6,0);
			vector <short unsigned> vstubsPerLayer_BendClean(6,0);

//			short unsigned int stubsPerLayer          [6] = {0,0,0,0,0,0};
//			short unsigned int stubsPerLayer_BendClean[6] = {0,0,0,0,0,0};
//			short unsigned int stubsPerLayer_Matched  [6] = {0,0,0,0,0,0};
//			short unsigned int stubsPerLayer_BendClean_Matched[6] = {0,0,0,0,0,0};

			pattern_it=patternMap.find(road.superstripIds);

			bool fired6Layer = (pattern_it!=patternMap.end());
			bool fired5Layer = false;

			for (unsigned int iSS=0; iSS<road.superstripIds.size();++iSS) {
				unsigned int ssID = road.superstripIds.at(iSS);
				unsigned int nStubs = road.stubRefs.at(iSS).size();
				for (unsigned int iStub=0; iStub<nStubs; ++iStub) {
					unsigned int stubID = road.stubRefs.at(iSS).at(iStub);
					short int stub_layer = TMath::Floor(r.TTStubs_modId   ->at(stubID)/10000.)-5;
//					double stub_r    = rEff.TTStubs_r       ->at(stubID);
//					double stub_z    = rEff.TTStubs_z       ->at(stubID);
//					double stub_bend = rEff.TTStubs_trigBend->at(stubID);
//					int   stub_pdgId = rEff.TTStubs_pdgId   ->at(stubID);
//					int   stub_trkId = rEff.TTStubs_trkId   ->at(stubID);
					++vstubsPerLayer[stub_layer];
					stubLayers_map[stub_layer]=stubID;
				} //end looping on stubs
			} // end looping on SS

			if (stubLayers_map.size()!=6) fired6Layer=false; // need this to account for ssID=0 and no stubs
			if (stubLayers_map.size()< 5) continue; // need this to account for ssID=0 and no stubs

			if ((!fired6Layer && nLayers==6)) {
				++count;
				continue;
			}

			if ((int)stubLayers_map.size()<nLayers) {
				cout << "Too many missing layers" <<endl;
				return;
			}

			double roadInvPt       = 0;
			unsigned roadInvPtFreq = 0;
			int missingLayer = -1;

			unsigned short nCleanLayer= 0;

			if (fired6Layer) {
				roadInvPt  = (pattern_it->second.meanPt);
				roadInvPtFreq = pattern_it->second.frequency;
			}
			else {
				for (unsigned iL=0; iL <6; ++iL) {
					if (!vstubsPerLayer[iL]) {
						missingLayer=iL;
						break;
					}
				}
				vector <unsigned> vSSID(rEff.AMTTRoads_superstripIds->at(iRoads));
				for (unsigned iLayMiss=0; iLayMiss<6; ++iLayMiss) {
					vector <unsigned> vSSID5(vSSID);
					if (!vstubsPerLayer[iLayMiss]) vSSID5.erase(vSSID5.begin()+iLayMiss);
					else continue;
					pattern_it=vpatternMap.at(missingLayer).find(vSSID5);
					fired5Layer = (pattern_it!=vpatternMap.at(missingLayer).end() );
					if (fired5Layer) {
						roadInvPt  = (pattern_it->second.meanPt);
						roadInvPtFreq = pattern_it->second.frequency;
						break;
					}
				}
			}
			if (!fired6Layer && !fired5Layer) {
				cout << "ERROR. Did not fire any pattern" << endl;
				continue;
			}

			for (unsigned int iSS=0; iSS<road.superstripIds.size();++iSS) {
				unsigned int ssID = road.superstripIds.at(iSS);
				unsigned int nStubs = road.stubRefs.at(iSS).size();
				for (unsigned int iStub=0; iStub<nStubs; ++iStub) {
					unsigned int stubID = road.stubRefs.at(iSS).at(iStub);
					short int stub_layer = TMath::Floor(r.TTStubs_modId   ->at(stubID)/10000.)-5;
					double stub_r    = rEff.TTStubs_r       ->at(stubID);
					double stub_z    = rEff.TTStubs_z       ->at(stubID);
					double stub_bend = rEff.TTStubs_trigBend->at(stubID);
					int   stub_pdgId = rEff.TTStubs_pdgId   ->at(stubID);
					int   stub_trkId = rEff.TTStubs_trkId   ->at(stubID);
					fp1->SetParameter(0,slopes11[stub_layer][missingLayer+1]);
					if (TMath::Abs(stub_bend-fp1->Eval(roadInvPt))<halfwidths[stub_layer][missingLayer+1]*1.00) { // HACK. 4.0*0.75=3.0 sigma cuts
						++vstubsPerLayer_BendClean[stub_layer];
						stubLayers_map_BendClean_Matched[stub_layer]=stubID;
					}

				} //end looping on stubs
			} // end looping on SS

			for (unsigned iLayMiss=0; iLayMiss<7; ++iLayMiss) {
				if      (stubLayers_map.size()==6 && iLayMiss) break;
				else if (stubLayers_map.size()==5) {
					if (vstubsPerLayer[iLayMiss]) continue;
				}
				h1roadPtcombInvPtPerRoad[iLayMiss]->Fill(roadInvPt);
			}

			if (stubLayers_map_BendClean_Matched.size()< 5) continue;

			for (unsigned iLayMiss=0; iLayMiss<7; ++iLayMiss) {
				if      (stubLayers_map_BendClean_Matched.size()==6 && iLayMiss) break;
				else if (stubLayers_map_BendClean_Matched.size()==5) {
					if (vstubsPerLayer_BendClean[iLayMiss]) continue;
				}
				h1roadPtcombInvPtPerRoad_BendClean[iLayMiss]->Fill(roadInvPt);
			}
			bool roadIsGenMatched = 0;

//			cout << roadInvPt << "\t" << genPartInvPt << "\t" << roadPhi << "\t" << genPartPhi << endl;


			for (unsigned int iSS=0; iSS<road.superstripIds.size();++iSS) {
				//				unsigned int ssID = road.superstripIds.at(iSS);
				unsigned int nStubs = road.stubRefs.at(iSS).size();
				for (unsigned int iStub=0; iStub<nStubs; ++iStub) {
					unsigned int stubID = road.stubRefs.at(iSS).at(iStub);
					double stub_r    = rEff.TTStubs_r       ->at(stubID);
					double stub_z    = rEff.TTStubs_z       ->at(stubID);
					double stub_bend = rEff.TTStubs_trigBend->at(stubID);
					int   stub_pdgId = rEff.TTStubs_pdgId   ->at(stubID);
					int   stub_trkId = rEff.TTStubs_trkId   ->at(stubID);
					short int stub_layer = TMath::Floor(rEff.TTStubs_modId   ->at(stubID)/10000.)-5;
//					if (stub_layer>5 || stub_layer<0) continue;
					//					double stub_roughInvPt = TMath::Sign(1/rEff.TTStubs_roughPt->at(stubID),stub_bend);
//					// insert here the cuts
//					unsigned short roadInvPtBin = hStubPtCuts[stub_layer][0]->GetXaxis()->FindBin(roadInvPt);
//					if (   (hStubPtCuts[stub_layer]->GetBinContent(roadInvPtBin,2)>stub_bend &&
//							hStubPtCuts[stub_layer]->GetBinContent(roadInvPtBin,1)<stub_bend )) {
					fp3->SetParameters(slopes1[stub_layer],slopes3[stub_layer]);
					if (TMath::Abs(stub_bend-fp3->Eval(roadInvPt))<halfwidths[stub_layer]*1.00) { // HACK. 4.0*0.75=3.0 sigma cuts
						++stubsPerLayer_BendClean[stub_layer];
						stubLayers_map[stub_layer]=stubID;
						if (stub_trkId==1&&TMath::Abs(stub_pdgId)==genPdgId) {
							++stubsPerLayer_BendClean_Matched[stub_layer];
							stubLayers_map_BendClean_Matched[stub_layer]=stubID;
						}
					}
					else {
						h1roadPtRemovedStub[stub_layer]->Fill(roadInvPt);
						h1dsRemovedStub    [stub_layer]->Fill(stub_bend-fp3->Eval(roadInvPt));
						if (stub_trkId==1&&TMath::Abs(stub_pdgId)==genPdgId) {
							h1dsRemovedMatchedStub    [stub_layer]->Fill(stub_bend-fp3->Eval(roadInvPt));
						}
					}
					++stubsPerLayer[stub_layer];
					if (stub_trkId==1&&TMath::Abs(stub_pdgId)==genPdgId) {
						stubLayers_map_Matched[stub_layer]=stubID;
						++stubsPerLayer_Matched[stub_layer];
					}
				} //end looping on stubs
			} // end looping on SS
			if (stubLayers_map                  .size() == 6) ++nCleanRoads;
			if (stubLayers_map_Matched          .size() == 6) {++nMatchedRoads;roadIsGenMatched=1;}
			if (stubLayers_map_BendClean_Matched.size() == 6) ++nCleanMatchedRoads;
			stubLayers_map.clear();
			stubLayers_map_Matched.clear();
			stubLayers_map_BendClean_Matched.clear();
			unsigned short nCombinationsInvPt           = 1;
			unsigned short nCombinationsInvPt_BendClean = 1;
			for (unsigned iLay=0; iLay <6; ++iLay) {
				h1roadPtcombInvPt           [iLay]->Fill(stubsPerLayer          [iLay]);
				h1roadPtcombInvPt_BendClean [iLay]->Fill(stubsPerLayer_BendClean[iLay]);
				if (roadIsGenMatched) {
					h1roadPtcombInvPt_Matched          [iLay]->Fill(stubsPerLayer_Matched  [iLay]);
					h1roadPtcombInvPt_Matched_BendClean[iLay]->Fill(stubsPerLayer_BendClean_Matched[iLay]);
				}
				nCombinationsInvPt          *=stubsPerLayer          [iLay];
				nCombinationsInvPt_BendClean*=stubsPerLayer_BendClean[iLay];
			}
//			maxFreq = (maxFreq < roadInvPtFreq ? roadInvPtFreq : maxFreq);
			nCombinationsInvPt_PerEvent         +=nCombinationsInvPt;
			nCombinationsInvPt_BendCleanPerEvent+=nCombinationsInvPt_BendClean;
			if (roadIsGenMatched) {
				nCombinationsInvPt_PerEvent_Matched          +=nCombinationsInvPt;
				nCombinationsInvPt_PerEvent_BendClean_Matched+=nCombinationsInvPt_BendClean;
			}
			h1roadPtcombInvPtPerRoad          ->Fill(nCombinationsInvPt          );
			h1roadPtcombInvPtPerRoad_BendClean->Fill(nCombinationsInvPt_BendClean);
		} // end of looping on roads
		if (!nRoadsInBank) continue;
		h1Nroads                       ->Fill(nRoads);
		h1NCleanedroads                ->Fill(nCleanRoads);
		if (nMatchedRoads) h1NMatchedroads                ->Fill(nMatchedRoads);
		if (nMatchedRoads) h1NMatchedCleanedroads         ->Fill(nCleanMatchedRoads);
		h1roadPtcombInvPt           [6]->Fill(nCombinationsInvPt_PerEvent);
		h1roadPtcombInvPt_BendClean [6]->Fill(nCombinationsInvPt_BendCleanPerEvent);
		if (nMatchedRoads) h1NcombPerEv_Matched           ->Fill(nCombinationsInvPt_PerEvent_Matched);
		if (nMatchedRoads) h1NcombPerEv_Matched_BendClean ->Fill(nCombinationsInvPt_PerEvent_BendClean_Matched);
//		nEv+=maxFreq; // increment counter for events with at least one road
		nEv++; // increment counter for events with at least one road
		if (nMatchedRoads) nEvMatchedRoad++; // increment counter for events with at least one genMatched road
		if (nMatchedRoads && nCleanRoads) nEvMatchedRoadCleaned++; // increment counter for events with at least one road after cleaning
		if (nCleanRoads) nEvCleaned++;
		if ((jentry && jentry%50000==0) || jentry==nentriesEff-1) {
			cout << "Processed stub event "
					<< jentry << "\t"
					<< nEvMatchedRoad/(1.0*nEv) << "\t"
					<< nEvCleaned    /(1.0*nEv) << "\t"
					<< nEvMatchedRoadCleaned/(1.0*nEvMatchedRoad) << endl;
		}
	} //end looping on events


	char cc[100];

	//	TCanvas* cRoadDeltaPtPhi = new TCanvas("cRoadDeltaPtPhi","cRoadDeltaPtPhi",0,0,1000,800);
//	cRoadDeltaPtPhi->SetLogz();
//	cRoadDeltaPtPhi->SetGrid();
//	h2roadDeltaInvPtDeltaPhi->DrawCopy("colz0");
//	TEllipse* ellipse = new TEllipse(0,0,axMax,axMin,0,360,Theta);
//	ellipse->SetLineColor(2);
//	ellipse->SetFillStyle(0);
//	ellipse->Draw("same");

	TCanvas* cLayerStubs = new TCanvas("cLayerStubs","cLayerStubs",0,0,1000,700);
	cLayerStubs->Divide(3,2);
	for (unsigned iBin=0; iBin <6; ++iBin) {
		pad = (TPad*) cLayerStubs->cd(iBin+1);
		pad->SetLogy();
		sprintf(cc,"Stubs in SuperStrip. Layer %d", iBin);
		h1roadPtcombInvPt          [iBin]->SetTitle(cc);
		h1roadPtcombInvPt          [iBin]->SetXTitle("Stubs/SS");
		h1roadPtcombInvPt          [iBin]->SetLineColor(1);
		h1roadPtcombInvPt          [iBin]->SetLineWidth(2.0);
		h1roadPtcombInvPt          [iBin]->DrawCopy();
//		h1roadPtcombInvPt_BendClean[iBin]->SetLineColor(2);
//		h1roadPtcombInvPt_BendClean[iBin]->DrawCopy("same");
//		h1roadPtcombInvPt_Matched  [iBin]->SetLineColor(4);
//		h1roadPtcombInvPt_Matched  [iBin]->SetLineWidth(2.0);
//		h1roadPtcombInvPt_Matched  [iBin]->DrawCopy("same");
//		h1roadPtcombInvPt_Matched_BendClean[iBin]->SetLineColor(6);
//		h1roadPtcombInvPt_Matched_BendClean[iBin]->DrawCopy("same");
		TLegend* tl = new TLegend(0.2,0.75,0.9,0.9);
		sprintf(cc,"# stubs. Mean: %4.2lf", h1roadPtcombInvPt[iBin]->GetMean());
		TLegendEntry* tle = tl->AddEntry(h1roadPtcombInvPt[iBin],cc,"l");
		tle->SetLineColor(h1roadPtcombInvPt[iBin]->GetLineColor());
//		sprintf(cc,"# stubs after #Deltas cleaning. Mean: %4.2lf", h1roadPtcombInvPt_BendClean[iBin]->GetMean());
//		tle = tl->AddEntry(h1roadPtcombInvPt_BendClean[iBin],cc,"l");
//		tle->SetLineColor(h1roadPtcombInvPt_BendClean[iBin]->GetLineColor());
//		sprintf(cc,"# stubs gen-matched. Mean: %4.2lf", h1roadPtcombInvPt_Matched  [iBin]->GetMean());
//		tle = tl->AddEntry(h1roadPtcombInvPt_Matched  [iBin],cc,"l");
//		tle->SetLineColor(h1roadPtcombInvPt_Matched  [iBin]->GetLineColor());
//		sprintf(cc,"# stubs gen-matched roads after #Deltas cleaning. Mean: %4.2lf", h1roadPtcombInvPt_Matched_BendClean[iBin]->GetMean());
//		tle = tl->AddEntry(h1roadPtcombInvPt_Matched_BendClean[iBin],cc,"l");
//		tle->SetLineColor(h1roadPtcombInvPt_Matched_BendClean[iBin]->GetLineColor());
		tl->Draw();
	}
	if (savePlots) {
//		cLayerStubs->Size(1800,1000);
//		cLayerStubs->Update();
		cLayerStubs->SaveAs(dirPlots+TString("stubsPerLayer_")+sEffParticle+TString(".png"));
		cLayerStubs->SaveAs(dirPlots+TString("stubsPerLayer_")+sEffParticle+TString(".pdf"));
	}

	double  percentiles[3] = {0.90, 0.95, 0.99};
	double xpercentiles[3];
	TLatex* ttext = new TLatex();
	ttext->SetTextSize(0.05);
	TCanvas* cRoadComb[4];
	cRoadComb[0] = new TCanvas("Combinations_road_BX","Combinations/road/BX",0,0,1000,1000);
//	cRoadComb->Divide(2,2);
//	pad = (TPad*) cRoadComb->cd(1);
	cRoadComb[0]->SetLogy();
	h1roadPtcombInvPtPerRoad->SetTitle ("Combinations/road/BX");
	h1roadPtcombInvPtPerRoad->SetXTitle("# combinations/road/BX");
	h1roadPtcombInvPtPerRoad->SetLineColor(1);
	h1roadPtcombInvPtPerRoad->SetLineWidth(2.0);
	if (applyOnMuon==1) h1roadPtcombInvPtPerRoad_BendClean->GetXaxis()->SetRangeUser(0,50);
	h1roadPtcombInvPtPerRoad_BendClean->SetTitle ("Combinations/road/BX");
	h1roadPtcombInvPtPerRoad_BendClean->SetXTitle("# combinations/road/BX");
	h1roadPtcombInvPtPerRoad_BendClean->SetLineWidth(2.0);
	h1roadPtcombInvPtPerRoad_BendClean->DrawCopy();
	h1roadPtcombInvPtPerRoad->DrawCopy("same");
	h1roadPtcombInvPtPerRoad_BendClean->SetLineColor(2);
	h1roadPtcombInvPtPerRoad_BendClean->DrawCopy("same");
	if (applyOnMuon==1) h1roadPtcombInvPtPerRoad_BendClean->GetXaxis()->SetRange(0,0);
	TLegend* tl = new TLegend(0.2,0.70,0.9,0.9);
	sprintf(cc,"# combinations. Mean: %4.2lf", h1roadPtcombInvPtPerRoad->GetMean());
	TLegendEntry* tle = tl->AddEntry(h1roadPtcombInvPtPerRoad,cc,"l");
	tle->SetLineColor(h1roadPtcombInvPtPerRoad->GetLineColor());
	sprintf(cc,"# combinations after #Deltas cleaning. Mean: %4.2lf", h1roadPtcombInvPtPerRoad_BendClean->GetMean());
	tle = tl->AddEntry(h1roadPtcombInvPtPerRoad_BendClean,cc,"l");
	tle->SetLineColor(h1roadPtcombInvPtPerRoad_BendClean->GetLineColor());
	h1roadPtcombInvPtPerRoad->GetQuantiles(3,xpercentiles,percentiles);
	sprintf(cc,"#Lambda_{0.99}: %4.2lf", xpercentiles[2]);
	tle = tl->AddEntry(h1roadPtcombInvPtPerRoad,cc,"l");
	tle->SetLineColor(h1roadPtcombInvPtPerRoad->GetLineColor());
	h1roadPtcombInvPtPerRoad_BendClean->GetQuantiles(3,xpercentiles,percentiles);
	sprintf(cc,"#Lambda_{0.99}: %4.2lf", xpercentiles[2]);
	tle = tl->AddEntry(h1roadPtcombInvPtPerRoad_BendClean,cc,"l");
	tle->SetLineColor(h1roadPtcombInvPtPerRoad_BendClean->GetLineColor());
	tl->Draw();
//	ttext->DrawLatexNDC(0.4,0.45,sEffParticle.Data());

	cRoadComb[1] = new TCanvas("Roads_BX","Roads/BX",0,0,1000,1000);
//	pad = (TPad*) cRoadComb->cd(2);
	cRoadComb[1]->SetLogy();
	h1Nroads->SetTitle  ("Roads/BX");
	h1Nroads->SetXTitle ("# roads/BX");
	h1Nroads->SetLineColor (1);
	h1Nroads              ->SetLineWidth(2.0);
	h1NCleanedroads       ->SetLineWidth(2.0);
	h1NMatchedCleanedroads->SetLineWidth(2.0);
	h1NMatchedCleanedroads->SetTitle  ("Roads/BX");
	h1NMatchedCleanedroads->SetXTitle ("# roads/BX");
	h1NCleanedroads->SetTitle  ("Roads/BX");
	h1NCleanedroads->SetXTitle ("# roads/BX");
	if (applyOnMuon==1) h1NMatchedCleanedroads->GetXaxis()->SetRangeUser(0,20);
	if (applyOnMuon==1) h1NCleanedroads->GetXaxis()->SetRangeUser(0,20);
	h1NCleanedroads->DrawCopy();
	h1Nroads->DrawCopy("same");
	h1NCleanedroads->SetLineColor (2);
	h1NCleanedroads->DrawCopy("same");
//	h1NMatchedroads->SetLineColor (4);
//	h1NMatchedroads->DrawCopy("same");
//	h1NMatchedCleanedroads->SetLineColor (6);
//	h1NMatchedCleanedroads->DrawCopy("same");
	h1NMatchedCleanedroads->GetXaxis()->SetRange(0,0);
	h1NCleanedroads       ->GetXaxis()->SetRange(0,0);
	TLegend* tl2 = new TLegend(0.2,0.75,0.9,0.9);
	sprintf(cc,"# roads. Mean: %4.2lf"               , h1Nroads            ->GetMean());
	tle = tl2->AddEntry(h1Nroads,cc,"l");
	tle->SetLineColor(h1Nroads->GetLineColor());
	sprintf(cc,"# roads after #Deltas cleaning. Mean: %4.2lf", h1NCleanedroads->GetMean());
	tle = tl2->AddEntry(h1NCleanedroads,cc,"l");
	tle->SetLineColor(h1NCleanedroads->GetLineColor());
//	sprintf(cc,"# gen-matched roads. Mean: %4.2lf", h1NMatchedroads     ->GetMean());
//	tle = tl2->AddEntry(h1NMatchedroads,cc,"l");
//	tle->SetLineColor(h1NMatchedroads->GetLineColor());
//	sprintf(cc,"# gen-matched roads after #Deltas cleaning. Mean: %4.2lf", h1NMatchedCleanedroads->GetMean());
//	tle = tl2->AddEntry(h1NMatchedCleanedroads,cc,"l");
//	tle->SetLineColor(h1NMatchedCleanedroads->GetLineColor());
	tl2->Draw();

	cRoadComb[2] = new TCanvas("Combinations_BX","Combinations/BX",0,0,1000,1000);
//	pad = (TPad*) cRoadComb->cd(3);
	cRoadComb[2]->SetLogy();
	h1roadPtcombInvPt_BendClean[6]->SetTitle ("Combinations/BX");
	h1roadPtcombInvPt_BendClean[6]->SetXTitle("# combinations/BX");
	if (applyOnMuon==1) h1roadPtcombInvPt_BendClean[6]->GetXaxis()->SetRangeUser(1,50);
	h1roadPtcombInvPt          [6]->SetLineWidth(2.0);
	h1roadPtcombInvPt_BendClean[6]->SetLineWidth(2.0);
	h1roadPtcombInvPt_BendClean[6]->DrawCopy();
	h1roadPtcombInvPt [6]->SetTitle ("Combinations/BX");
	h1roadPtcombInvPt [6]->SetXTitle("# combinations/BX");
	h1roadPtcombInvPt [6]->SetLineColor(1);
	h1roadPtcombInvPt [6]->DrawCopy("same");
	h1roadPtcombInvPt_BendClean[6]->SetLineColor(2);
	h1roadPtcombInvPt_BendClean[6]->DrawCopy("same");
	h1roadPtcombInvPt_BendClean[6]->GetXaxis()->SetRange(0,0);
	TLegend* tl1 = new TLegend(0.2,0.75,0.9,0.9);
	sprintf(cc,"# combinations. Mean: %4.2lf", h1roadPtcombInvPt           [6]->GetMean());
	tle = tl1->AddEntry(h1roadPtcombInvPt           [6],cc,"l");
	tle->SetLineColor(h1roadPtcombInvPt           [6]->GetLineColor());
	sprintf(cc,"# combinations after #Deltas cleaning. Mean: %4.2lf", h1roadPtcombInvPt_BendClean [6]->GetMean());
	tle = tl1->AddEntry(h1roadPtcombInvPt_BendClean [6],cc,"l");
	tle->SetLineColor(h1roadPtcombInvPt_BendClean [6]->GetLineColor());
	h1roadPtcombInvPt [6]->GetQuantiles(3,xpercentiles,percentiles);
	sprintf(cc,"#Lambda_{0.99}: %4.2lf", xpercentiles[2]);
	tle = tl1->AddEntry(h1roadPtcombInvPt[6],cc,"l");
	tle->SetLineColor(h1roadPtcombInvPt           [6]->GetLineColor());
	h1roadPtcombInvPt_BendClean [6]->GetQuantiles(3,xpercentiles,percentiles);
	sprintf(cc,"#Lambda_{0.99}: %4.2lf", xpercentiles[2]);
	tle = tl1->AddEntry(h1roadPtcombInvPt_BendClean[6],cc,"l");
	tle->SetLineColor(h1roadPtcombInvPt_BendClean [6]->GetLineColor());
	tl1->Draw();
	double effRoadBendClean = h1roadPtcombInvPt_BendClean[6]->GetBinContent(1);
	effRoadBendClean       /= h1roadPtcombInvPt          [6]->Integral();
	sprintf(cc,"#Deltas cut eff: %5.3lf", 1-effRoadBendClean);
	cout << cc << endl;
//	ttext->DrawLatexNDC(0.4,0.5,cc);

	cRoadComb[3] = new TCanvas("MatchedCombinations_BX","MatchedCombinations/BX",0,0,1000,1000);
//	pad = (TPad*) cRoadComb->cd(4);
	cRoadComb[3]->SetLogy();
	if (applyOnMuon==1) h1NcombPerEv_Matched_BendClean->GetXaxis()->SetRangeUser(0,50);
	if (applyOnMuon==0) h1NcombPerEv_Matched_BendClean->GetXaxis()->SetRangeUser(0,50);
	h1NcombPerEv_Matched_BendClean->SetTitle ("Matched Roads - Combinations/BX");
	h1NcombPerEv_Matched_BendClean->SetXTitle("# combinations/BX");
	h1NcombPerEv_Matched          ->SetLineWidth(2.0);
	h1NcombPerEv_Matched_BendClean->SetLineWidth(2.0);
	h1NcombPerEv_Matched_BendClean->DrawCopy();
	h1NcombPerEv_Matched          ->SetTitle ("Matched Roads - Combinations/BX");
	h1NcombPerEv_Matched          ->SetXTitle("# combinations/BX");
	h1NcombPerEv_Matched          ->SetLineColor(1);
	h1NcombPerEv_Matched          ->DrawCopy("same");
	h1NcombPerEv_Matched_BendClean->SetLineColor(2);
	h1NcombPerEv_Matched_BendClean->DrawCopy("same");
	h1NcombPerEv_Matched_BendClean->GetXaxis()->SetRangeUser(0,0);
	TLegend* tl3 = new TLegend(0.2,0.75,0.9,0.9);
	sprintf(cc,"# combinations. Mean: %4.2lf"                       , h1NcombPerEv_Matched          ->GetMean());
	tle = tl3->AddEntry(h1NcombPerEv_Matched          ,cc,"l");
	tle->SetLineColor(h1NcombPerEv_Matched->GetLineColor());
	sprintf(cc,"# combinations after #Deltas cleaning. Mean: %4.2lf", h1NcombPerEv_Matched_BendClean->GetMean());
	tle = tl3->AddEntry(h1NcombPerEv_Matched_BendClean,cc,"l");
	tle->SetLineColor(h1NcombPerEv_Matched_BendClean->GetLineColor());
	h1NcombPerEv_Matched->GetQuantiles(3,xpercentiles,percentiles);
	sprintf(cc,"#Lambda_{0.99}: %4.2lf", xpercentiles[2]);
	tle = tl3->AddEntry(h1NcombPerEv_Matched,cc,"l");
	tle->SetLineColor(h1NcombPerEv_Matched->GetLineColor());
	h1NcombPerEv_Matched_BendClean->GetQuantiles(3,xpercentiles,percentiles);
	sprintf(cc,"#Lambda_{0.99}: %4.2lf", xpercentiles[2]);
	tle = tl3->AddEntry(h1NcombPerEv_Matched_BendClean,cc,"l");
	tle->SetLineColor(h1NcombPerEv_Matched_BendClean->GetLineColor());
	double effCombBendClean = h1NcombPerEv_Matched_BendClean->GetBinContent(1);
	effCombBendClean       /= h1NcombPerEv_Matched          ->Integral();
	sprintf(cc,"#Deltas cut eff: %5.3lf", 1-effCombBendClean);
	if (applyOnMuon==0) ttext->DrawLatexNDC(0.4,0.5,cc);
	if (applyOnMuon==2) ttext->DrawLatexNDC(0.4,0.5,cc);
	cout << cc << endl;
	tl3->Draw();

	if (savePlots) {
		for (int i=0; i<4; ++i) {
			cRoadComb[i]->SaveAs(dirPlots+TString(cRoadComb[i]->GetName())+sEffParticle+TString(".png"));
			cRoadComb[i]->SaveAs(dirPlots+TString(cRoadComb[i]->GetName())+sEffParticle+TString(".pdf"));
		}
	}

//	TCanvas* croadPtStubsFailing = new TCanvas("roadPtStubsFailingCleaningPerLayer","roadPtStubsFailingCleaningPerLayer",0,0,1000,1000);
//	croadPtStubsFailing->Divide(2,3);
//	for (unsigned iLay=0; iLay <6; ++iLay) {
//		croadPtStubsFailing->cd(iLay+1);
//		h1roadPtRemovedStub[iLay]->DrawCopy();
//	}
//
//	TCanvas* cdsStubsFailing = new TCanvas("dsStubsFailingCleaningPerLayer","dsStubsFailingCleaningPerLayer",0,0,1000,1000);
//	cdsStubsFailing->Divide(2,3);
//	for (unsigned iLay=0; iLay <6; ++iLay) {
//		cdsStubsFailing->cd(iLay+1);
//		h1dsRemovedStub       [iLay]->DrawCopy();
//		h1dsRemovedMatchedStub[iLay]->SetLineColor(2);
//		h1dsRemovedMatchedStub[iLay]->DrawCopy("same");
//	}
	t->ResetBranchAddresses();

//	**************************************************

	if (deriveCuts) {
	cout << "double slopes1[6] = {";
	for (unsigned iLay=0; iLay <6; ++iLay) {
		if (iLay) cout << ", " << slopes1[iLay] ;
		else      cout <<         slopes1[iLay] ;
	}
	cout << "};" << endl;
	cout << "double slopes3[6] = {";
	for (unsigned iLay=0; iLay <6; ++iLay) {
		if (iLay) cout << ", " << slopes3[iLay] ;
		else      cout <<         slopes3[iLay] ;
	}
	cout << "};" << endl;
	cout << "double halfwidths[6] = {";
	for (unsigned iLay=0; iLay <6; ++iLay) {
		if (iLay) cout << ", " << halfwidths[iLay];
		else      cout <<         halfwidths[iLay] ;
	}
	cout << "};" << endl;

	for (unsigned iLay=0; iLay <6; ++iLay) {
	cout <<   "slopes1["<<iLay<<"] = "    << slopes1   [iLay]
		 << "; slopes3["<<iLay<<"] = "    << slopes3   [iLay]
		 << "; halfwidths["<<iLay<<"] = " << halfwidths[iLay]
		 << ";" << endl;
	}
	}
*/
	return;
}

double gau(double* x, double* par) {
	if (par[2]<=0) return 0;
	else {
		double xx=(x[0]-par[1])/par[2];
		return par[0]*TMath::Exp(-xx*xx/2);
	}
}

//bool passStubPtCut (double stubInvRoughPt, double roadInvPt, double roadInvPtRms, double nSigmas) {
//	if (stubInvRoughPt<roadInvPt-nSigmas*roadInvPtRms) return 0;
//	if (stubInvRoughPt>roadInvPt+nSigmas*roadInvPtRms) return 0;
//	return 1;
//}

short int stubLayer(double r) {
	double radii_low [6]={20,30,45,65,85,105};
	double radii_high[6]={30,40,55,75,95,110};
	if (r<radii_low[0]) return -1;
	for (unsigned i=0;i<6;++i) {
		if (r>radii_low[i]&&r<radii_high[i]) return i;
	}
	return -2;
}
