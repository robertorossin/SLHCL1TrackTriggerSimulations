#define roadsStubs_cxx
#include "roadsStubs.h"
#define stubsNtuple_cxx
#include "stubsNtuple.h"
#include "/home/rossin/Dropbox/TT/CMSSW_6_2_0_SLHC12_patch1_SL6/src/SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTRoad.h"
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
#include <fstream>

using namespace std;

struct tVectorComp {
	bool operator() (const TVector3& lhs, const TVector3& rhs) const
	{return lhs.Mag2()<rhs.Mag2();}
};

double gau(double* x, double* par) ;
short int stubLayer(double r) ;
//bool passStubPtCut (double stubInvRoughPt, double roadInvPt, double roadInvPtRms, double nSigmas);

void roadsStubsClean5oo6_2 (bool savePlots=0, const int nLayers=6, bool use95coverage=0, TString clayerSelect="111111") {

	if (!clayerSelect.IsBin()) {
		cout << "layer selector not a binary integer. Exiting" << endl;
		return;
	}
	if (clayerSelect.Length()!=6) {
		cout << "layer selector length != 6. Specify all the bits, innermost -> outermost <-> LSB -> MSB. Exiting" << endl;
		return;
	}
	bool layerSelect[6];
	cout << "Selecting layers: ";
	for (unsigned ibit=0; ibit<6; ++ibit) {
		layerSelect[ibit]=TString(clayerSelect(ibit)).Atoi();
		cout << layerSelect[ibit] << " " ;
	}
	cout << endl;

	bool isPlottose = false;

	TStyle* style = gStyle;
	style->SetOptStat(0);

	bool isSLHC25=0;

	TString dirPlots("/home/rossin/Dropbox/TT/Work/figures_stubCleaning/");

//	TString sTree="results_sf1_nz1_tt27_pt2_SingleMuonFlatOneOverPt0p005To0p5_tt27_Sum2M_SL6.root" ;
//	TString sTree="singleMuonNoTest/results_SingleMuonFlatOneOverPt0p005To0p5_tt27NoTest_cfi_py_GEN_SIM_DIGI_L1TrackTrigger_2M.root" ;
//	TString sTree="singleMuNoTest2_2000/results_20150528_SingleMuonFlatOneOverPt0p0005To0p5_tt27NoTest_4M.root" ;
//	TString sTree="singleMuNoTest2_2000/results_20150528_5outOf6_SingleMuonFlatOneOverPt0p0005To0p5_tt27NoTest_4M.root" ;
//	TString sTree="singleMuNoTest2_2000/roads_sf1_nz1_tt27_pt2_SingleMuonFlatOneOverPt0p0005To0p5_tt27NoTest_4M_SLHC25.root"; isSLHC25=1; // 6/6
//	TString sTree="singleMuNoTest2_2000/roads_5outOf6_sf1_nz1_tt27_pt2_SingleMuonFlatOneOverPt0p0005To0p5_tt27NoTest_4M_SLHC25.root"; isSLHC25=1; // 5/6

//	TString sTree="SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz1_tt27_pt2_6oo6_SingleMuonTest_tt27_PU0_20150408_SLHC25p3_newBank_10M.root"; isSLHC25=1; TString pName("SingleMu_PU0_6oo6");// 6/6
//	TString sTree="SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz1_tt27_pt2_5oo6_SingleMuonTest_tt27_PU0_20150408_SLHC25p3_newBank_10M.root"; isSLHC25=1; TString pName("SingleMu_PU0_5or6oo6");// 5/6

//	TString sTree="SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz1_tt27_pt2_6oo6_SingleMuonTest_tt27_PU140_20150408_SLHC25p3_newBank_10M.root"; isSLHC25=1; TString pName("SingleMu_PU140_6oo6");// 5/6
//	TString sTree="SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz1_tt27_pt2_5oo6_SingleMuonTest_tt27_PU140_20150408_SLHC25p3_newBank_10M.root"; isSLHC25=1; TString pName("SingleMu_PU140_5or6oo6");// 5/6
//	TString sTree="Neutrino_tt27_PU140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz1_tt27_pt2_6oo6_Neutrino_PU140_20150408_SLHC25p3_newBank_10M.root"; isSLHC25=1; TString pName("Neutrino_PU140_6oo6");// 6/6
//	TString sTree="Neutrino_tt27_PU140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz1_tt27_pt2_5or6oo6_Neutrino_PU140_20150408_SLHC25p3_newBank_10M.root"; isSLHC25=1; TString pName("Neutrino_PU140_5or6oo6");// 5/6
//	TString sTree="TTbarTTbar_tt27_PU140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz1_tt27_pt2_6oo6_TTbarTTbar_tt27_PU140_20150408_SLHC25p3_newBank_10M.root"; isSLHC25=1; TString pName("4Tops_PU140_6oo6");// 6/6
//	TString sTree="TTbarTTbar_tt27_PU140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz1_tt27_pt2_5oo6_TTbarTTbar_tt27_PU140_20150408_SLHC25p3_newBank_10M.root"; isSLHC25=1; TString pName("4Tops_PU140_5or6oo6");// 5/6

//	TString sTree="SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz1_tt27_pt2_5oo6_SingleMuonTest_tt27_PU0_20150408_SLHC25p3_newBank_10M.root"; isSLHC25=1; TString pName("SingleMu_PU0_sf1_nz1_pt2_5or6oo6");// 5/6
//	TString sTree="SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz1_tt27_pt2_6oo6_SingleMuonTest_tt27_PU0_20150408_SLHC25p3_newBank_10M.root"; isSLHC25=1; TString pName("SingleMu_PU0_sf1_nz1_pt2_6oo6");// 6/6
//	TString sTree="SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/roads_5oo6_SingleMuonFlatOneOverPt0p0005To0p5_tt27_sf1_nz4_pt2_SLHC25p3_2M.root"; isSLHC25=1; TString pName("SingleMu_PU0_sf1_nz4_pt2_5or6oo6");// 5/6
//	TString sTree="SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/roads_5oo6_SingleMuonFlatOneOverPt0p0005To0p5_tt27_sf1_nz4_pt3_SLHC25p3_2M.root"; isSLHC25=1; TString pName("SingleMu_PU0_sf1_nz4_pt3_5or6oo6");// 5/6
//	TString sTree="SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/roads_6oo6_SingleMuonFlatOneOverPt0p0005To0p5_tt27_sf1_nz4_pt2_SLHC25p3_2M.root"; isSLHC25=1; TString pName("SingleMu_PU0_sf1_nz4_pt2_6oo6");// 6/6
//	TString sTree="SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/roads_6oo6_SingleMuonFlatOneOverPt0p0005To0p5_tt27_sf1_nz4_pt3_SLHC25p3_2M.root"; isSLHC25=1; TString pName("SingleMu_PU0_sf1_nz4_pt3_6oo6");// 6/6

//	TString sTree="Neutrino_tt27_PU140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz4_tt27_pt2_6oo6_Neutrino_PU140_20150408_SLHC25p3_newBank_100M.root"; isSLHC25=1; TString pName("Neutrino_PU140_sf1_nz4_pt2_6oo6"); TString pTitle("PU140 SF=1 Nz=4 Pt>2 GeV/c");;// 6/6
//	TString sTree="Neutrino_tt27_PU140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz4_tt27_pt2_5or6oo6_Neutrino_PU140_20150408_SLHC25p3_newBank_100M.root"; isSLHC25=1; TString pName("Neutrino_PU140_sf1_nz4_pt2_5or6oo6"); TString pTitle("PU140 SF=1 Nz=4 Pt>2 GeV/c");// 6/6
	TString sTree="Neutrino_tt27_PU140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz4_tt27_pt3_5or6oo6_Neutrino_PU140_20150408_SLHC25p3_newBank_100M.root"; isSLHC25=1; TString pName("Neutrino_PU140_sf1_nz4_pt3_5or6oo6"); TString pTitle("PU140 SF=1 Nz=4 Pt>3 GeV/c");// 6/6

//	TString sTree="TTbarTTbar_tt27_PU140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz4_tt27_pt2_5oo6_TTbarTTbar_tt27_PU140_20150408_SLHC25p3_100M.root"; isSLHC25=1; TString pName("4Tops_PU140_sf1_nz4_pt2_5or6oo6"); TString pTitle("tttt+PU140 SF=1 Nz=4 Pt>2 GeV/c");// 6/6
//	TString sTree="TTbarTTbar_tt27_PU140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz4_tt27_pt2_6oo6_TTbarTTbar_tt27_PU140_20150408_SLHC25p3_100M.root"; isSLHC25=1; TString pName("4Tops_PU140_sf1_nz4_pt2_6oo6"); TString pTitle("tttt+PU140 SF=1 Nz=4 Pt>2 GeV/c");// 6/6
//	TString sTree="TTbarTTbar_tt27_PU140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz4_tt27_pt3_5oo6_TTbarTTbar_tt27_PU140_20150408_SLHC25p3_100M.root"; isSLHC25=1; TString pName("4Tops_PU140_sf1_nz4_pt3_5or6oo6"); TString pTitle("tttt+PU140 SF=1 Nz=4 Pt>3 GeV/c");// 6/6
//	TString sTree="TTbarTTbar_tt27_PU140_20150408_SLHC25p3_NewPatterns/roads_sf1_nz4_tt27_pt3_6oo6_TTbarTTbar_tt27_PU140_20150408_SLHC25p3_100M.root"; isSLHC25=1; TString pName("4Tops_PU140_sf1_nz4_pt3_6oo6"); TString pTitle("tttt+PU140 SF=1 Nz=4 Pt>3 GeV/c");// 6/6

//	TFile *f = TFile::Open("/data/rossin/EOS/patternBank_sf1_nz1_tt27_pt2_400M_ssID_pT.root","READ");
//	TFile *f = TFile::Open("/data/rossin/EOS/patternBank_sf1_nz1_tt27_pt2_first50M.root","READ");
//	TFile *f = TFile::Open("/data/rossin/EOS/patternBank_sf1_nz1_tt27_pt2_SingleMuonFlatOneOverPt0p005To0p5_tt27_Sum2M_SL6.root","READ");
//	TFile *f = TFile::Open("/data/rossin/EOS/patternBank_SingleMuonFlatOneOverPt0p005To0p5_tt27_2M.root","READ");
//	TFile *f = TFile::Open("/data/rossin/EOS/singleMuonNoTest/patternBank_SingleMuonFlatOneOverPt0p005To0p5_tt27_2M.root","READ");
//	TFile *f = TFile::Open("/data/rossin/EOS/singleMuNoTest2_2000/patternBank_sf1_nz1_tt27_pt2_SingleMuonFlatOneOverPt0p0005To0p5_tt27NoTest_4M.root","READ");
//	TFile *f = TFile::Open("/data/rossin/EOS/singleMuNoTest2_2000/patternBank_sf1_nz1_tt27_pt2_SingleMuonFlatOneOverPt0p0005To0p5_tt27NoTest_4M_SLHC25.root","READ"); isSLHC25=1;
//	TFile *f = TFile::Open("/data/rossin/EOS/SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/patternBank_sf1_nz1_tt27_pt2_SingleMuon_tt27_SLHC25p3_10M.root","READ"); isSLHC25=1; unsigned npatt95coverage = 104000;

//	TFile *f = TFile::Open("/data/rossin/EOS/SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/patternBank_SingleMuonFlatOneOverPt0p0005To0p5_tt27_sf1_nz1_pt2_SLHC25p3_100M.root","READ"); isSLHC25=1; unsigned npatt95coverage = 103200;
//	TFile *f = TFile::Open("/data/rossin/EOS/SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/patternBank_SingleMuonFlatOneOverPt0p0005To0p5_tt27_sf1_nz4_pt2_SLHC25p3_100M.root","READ"); isSLHC25=1; unsigned npatt95coverage = 3100200;
	TFile *f = TFile::Open("/data/rossin/EOS/SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/patternBank_SingleMuonFlatOneOverPt0p0005To0p5_tt27_sf1_nz4_pt3_SLHC25p3_100M.root","READ"); isSLHC25=1; unsigned npatt95coverage = 1862700;

//	TString sTree="SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/roads_6oo6_SingleMuonFlatOneOverPt0p0005To0p5_tt27_sf1_nz1_pt200_SLHC25p3_100M.root"; isSLHC25=1; TString pName("SingleMuon_sf1_nz1_pt200"); TString pTitle("Single Mu SF=1 Nz=1 Pt>200 GeV/c");// 6/6
//	TFile *f = TFile::Open("/data/rossin/EOS/SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/patternBank_SingleMuonFlatOneOverPt0p0005To0p5_tt27_sf1_nz1_pt200_SLHC25p3_100M.root","READ"); isSLHC25=1; unsigned npatt95coverage = 1E7;

	if (!f) { return; }
	//	TTree *t=0; f->GetObject("tSSID",t);
	TTree *t=0;
	TTree *tAtt=0;
	if (isSLHC25) f->GetObject("patternAttributes",tAtt);
	f->GetObject("patternBank",t);
	Long64_t tentries = t->GetEntriesFast();
	if (use95coverage) tentries = (npatt95coverage < tentries ? npatt95coverage : tentries);
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

	const unsigned nLayersLoop = 2;

	TProfile* pStubPtroadPt [6][nLayersLoop];
	TH2*     h2StubPtroadPt [6][nLayersLoop];
	TH2*     h2StubPt1roadPt[6];
	//	TH2*     h2roadPtgenPt  [6];
	TH1*     h1StubsPerLayer[6][nLayersLoop][2];
	TH1*     h1RoadPerEvent    [nLayersLoop][2];
	TH1*     h1CombPerRoad     [nLayersLoop][2];
	TH1*     h1CombPerEvent    [nLayersLoop][2];
	TProfile2D*     hProfile2DCombEtaPhi    [nLayersLoop][2];

	TFile* fInCuts = 0; bool isfInOpen = false;
	fInCuts = new TFile("deltaScuts.root");
	isfInOpen = fInCuts->IsOpen();
	if (!isfInOpen) {
		cout << "ERROR. Not able to load the deltaS cut file. Exiting..." << endl;
		return;
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
			}
		}
		sName=TString("h2stubInvPt1_VS_roadInvPt")+TString(cc);
		h2StubPt1roadPt[iBin] = new TH2D    (sName,sName,nBinsInvPt,-maxRoadInvPt,maxRoadInvPt,nBinsRoughInvPt,-maxStubInvPt,maxStubInvPt);
		//		sName=TString("h2roadInvPt_VS_genInvPt")+TString(cc);
		//		h2roadPtgenPt  [iBin] = new TH2D    (sName,sName,nBinsInvPt,-0.6         ,0.6         ,nBinsRoughInvPt,-maxRoadInvPt,maxRoadInvPt);
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
				if (!iMissLay) {
					nRoadHisto/=8;
					nCombRoadHisto/=2;
					nCombHisto/=8;
				}
			}
			if (pName.Contains("SingleMu_PU140_") || pName.Contains("Neutrino_PU140_")) {
				nRoadHisto=800;
				nCombRoadHisto = 200;
				nCombHisto=3000;
				if (!iMissLay) {
					nRoadHisto/=4;
					nCombRoadHisto/=2;
					nCombHisto/=4;
				}
			}

			sName=TString("h1RoadPerEvent")+TString(cc2)+TString(cc3);
			h1RoadPerEvent[iMissLay][iClean] = new TH1D(sName,"",nRoadHisto,0,nRoadHisto);
			h1RoadPerEvent[iMissLay][iClean] ->SetXTitle("# road/tower/BX");
			h1RoadPerEvent[iMissLay][iClean] ->SetTitle(pTitle);
			sName=TString("h1CombPerRoad")+TString(cc2)+TString(cc3);
			h1CombPerRoad[iMissLay][iClean] = new TH1D(sName,"",nCombRoadHisto,0,nCombRoadHisto);
			h1CombPerRoad[iMissLay][iClean] ->SetXTitle("# comb/road/BX");
			h1CombPerRoad[iMissLay][iClean] ->SetTitle(pTitle);
			sName=TString("h1CombPerEvent")+TString(cc2)+TString(cc3);
			h1CombPerEvent[iMissLay][iClean] = new TH1D(sName+TString("_SingleStub"),"",nCombHisto,0,nCombHisto);
			h1CombPerEvent[iMissLay][iClean] ->SetXTitle("# comb/tower/BX");
			h1CombPerEvent[iMissLay][iClean] ->SetTitle(pTitle);

			sName=TString ("hProfile2DCombEtaPhi")+TString(cc2)+TString(cc3);
			sTitle=TString("Combinations per Road")+TString(cc)+TString(cc2);
			hProfile2DCombEtaPhi [iMissLay][iClean] = new TProfile2D    (sName,sTitle,200,0,0.7,200,0.8,1.5);
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


	unsigned nTested=0;
	Long64_t nbytes = 0, nb = 0;
	//	if (!deriveCuts) nentries=0;

	TH2D* hslopes11   = new TH2D("hslopes11"  ,"hslopes11"  ,6,0,6,7,0,7);
	TH2D* hhalfwidths = new TH2D("hhalfwidths","hhalfwidths",6,0,6,7,0,7);
	double slopes11[6][7];
	double slopes1 [6][7];
	double slopes3 [6][7];
	double halfwidths[6][7];
	if (isfInOpen) {
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
	unsigned count5         = 0;

	unsigned count6Clean    = 0;
	unsigned count5Clean    = 0;


	for (Long64_t jentry=0; jentry<nentries;jentry++) { // LOOPING OVER EVENTS
		//	for (Long64_t jentry=0; jentry<100;jentry++) {
		Long64_t ientry = r.LoadTree(jentry);
		if (ientry < 0) break;
		nb = r.fChain->GetEntry(jentry);   nbytes += nb;
		unsigned int nPart  = r.genParts_pt     ->size();
		//		unsigned int nTotStubs = r.TTStubs_r       ->size();
		unsigned int nRoads = r.AMTTRoads_nstubs->size();
		if (!nRoads) continue;
		++countEv;

		bool eventFired6 = false;
		bool eventFired5 = false;
		bool eventFiredClean6 = false;
		bool eventFiredClean5 = false;

		unsigned short nCombinationsPerEvent5oo6 = 0;
		unsigned short nCombinationsPerEvent6oo6 = 0;
		unsigned short nCombinationsPerEventClean5oo6 = 0;
		unsigned short nCombinationsPerEventClean6oo6 = 0;

		unsigned short nRoadsPerEvent5oo6 = 0;
		unsigned short nRoadsPerEvent6oo6 = 0;
		unsigned short nRoadsPerEventClean5oo6 = 0;
		unsigned short nRoadsPerEventClean6oo6 = 0;

		double genPartPt  = r.genParts_pt ->at(0);
		double genPartEta = r.genParts_eta->at(0);
		double genPartPhi = r.genParts_phi->at(0);
		double genPartz0  = r.genParts_vz ->at(0);

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
			//			slhcl1tt::TTRoad roadDsClean   (road);


			std::map <short int,unsigned int> stubLayers_map;
			std::map <short int,unsigned int> stubLayers_mapClean;
			short unsigned int stubsPerLayer     [6] = {0,0,0,0,0,0};
			short unsigned int stubsPerLayerClean[6] = {0,0,0,0,0,0};

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


			for (unsigned int iSS=0; iSS<road.superstripIds.size();++iSS) { // Looping over SS in ROAD
				unsigned int ssID = road.superstripIds.at(iSS);
				unsigned int nStubs = road.stubRefs.at(iSS).size();
				//				vector <unsigned> vStubsIDDsClean   ;
				//				vector <unsigned> vStubsIDSingleStubDsClean;
				vector <std::pair<TVector3,unsigned> > vRStubsID;
				for (unsigned int iStub=0; iStub<nStubs; ++iStub) { // looping on stubs 1
					unsigned int stubID = road.stubRefs.at(iSS).at(iStub);
					short int stub_layer = TMath::Floor(r.TTStubs_modId   ->at(stubID)/10000.)-5;
					++stubsPerLayer[stub_layer];
					stubLayers_map [stub_layer]=stubID;
					double stub_bend = r.TTStubs_trigBend->at(stubID);
				} //end looping on stubs 1
			} // end looping on SS

			if (stubLayers_map.size()==6) {
				fired6Layer   =true; // a 5/6 will fire a 6/6 pattern, but no stub will be found
				fired5or6Layer=true;
				eventFired6   =true;
			}
			if (stubLayers_map.size()==5) {
				fired5Layer   =true;
				fired5or6Layer=true;
				eventFired5   =true;
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

			for (unsigned int iSS=0; iSS<road.superstripIds.size();++iSS) { // Looping over SS in ROAD
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
					if (!layerSelect[stub_layer] ||
							(centralValueCut-halfWidthCut < stub_bend &&
							stub_bend < centralValueCut+halfWidthCut)) {
						++stubsPerLayerClean[stub_layer];
						stubLayers_mapClean[stub_layer]=stubID;
					}
				} //end looping on stubs
			} // end looping on SS

			if (stubLayers_mapClean.size()==6) {
				fired6LayerClean   =true; // a 5/6 will fire a 6/6 pattern, but no stub will be found
				fired5or6LayerClean=true;
				eventFiredClean6   =true;
			}
			if (stubLayers_mapClean.size()==5) {
				fired5LayerClean   =true;
				fired5or6LayerClean=true;
				eventFiredClean5   =true;
			}

			int missingLayerClean = -1;
			if (fired5LayerClean) for (unsigned iLay=0; iLay<6; ++iLay) {
				if (!stubsPerLayerClean[iLay]) {
					missingLayerClean = iLay;
					break;
				}
			}


			unsigned short nCombinationsPerRoad6oo6 = 1;
			unsigned short nCombinationsPerRoad5oo6 = 1;
			unsigned short nCombinationsPerRoadClean6oo6 = 1;
			unsigned short nCombinationsPerRoadClean5oo6 = 1;
			for (unsigned iLay=0; iLay <6; ++iLay) {
				if (fired6Layer) {
					h1StubsPerLayer [iLay][0][0]->Fill(stubsPerLayer[iLay]);
					nCombinationsPerRoad6oo6 *= stubsPerLayer[iLay];
					h1StubsPerLayer [iLay][0][1]->Fill(stubsPerLayerClean[iLay]);

					if (fired6LayerClean) {
						nCombinationsPerRoadClean6oo6 *= stubsPerLayerClean[iLay];
					}
				}

				if (fired5or6Layer) {
					h1StubsPerLayer [iLay][1][0]->Fill(stubsPerLayer[iLay]);
					if (stubsPerLayer[iLay]) nCombinationsPerRoad5oo6 *= stubsPerLayer[iLay];
					h1StubsPerLayer[iLay][1][1]->Fill(stubsPerLayerClean[iLay]);
				}
				if (fired5or6LayerClean) {
					if (stubsPerLayerClean[iLay]) nCombinationsPerRoadClean5oo6 *= stubsPerLayerClean[iLay];
				}
			}

			if (fired6Layer) {
				++nRoadsPerEvent6oo6;
				h1CombPerRoad          [0][0]     ->Fill(nCombinationsPerRoad6oo6);
				if (TMath::Abs(genPartz0)<0.5 && genPartPt>200) hProfile2DCombEtaPhi   [0][0]     ->Fill(genPartEta,genPartPhi,nCombinationsPerRoad6oo6);
				nCombinationsPerEvent6oo6 += nCombinationsPerRoad6oo6;
				if (fired6LayerClean) {
					++nRoadsPerEventClean6oo6;
					h1CombPerRoad          [0][1]   ->Fill(nCombinationsPerRoadClean6oo6);
					nCombinationsPerEventClean6oo6 += nCombinationsPerRoadClean6oo6;
				}
			}

			if (fired5or6Layer) {
				++nRoadsPerEvent5oo6;
				h1CombPerRoad          [1][0]     ->Fill(nCombinationsPerRoad5oo6);
				nCombinationsPerEvent5oo6 += nCombinationsPerRoad5oo6;
			}

			if (fired5or6LayerClean) {
				++nRoadsPerEventClean5oo6;
				h1CombPerRoad          [1][1]     ->Fill(nCombinationsPerRoadClean5oo6);
				nCombinationsPerEventClean5oo6 += nCombinationsPerRoadClean5oo6;
			}
		} // end looping on roads

		h1RoadPerEvent[0][0]->Fill(nRoadsPerEvent6oo6);
		h1RoadPerEvent[0][1]->Fill(nRoadsPerEventClean6oo6);
		h1RoadPerEvent[1][0]->Fill(nRoadsPerEvent5oo6);
		h1RoadPerEvent[1][1]->Fill(nRoadsPerEventClean5oo6);

		h1CombPerEvent[0][0]->Fill(nCombinationsPerEvent6oo6);
		h1CombPerEvent[0][1]->Fill(nCombinationsPerEventClean6oo6);
		h1CombPerEvent[1][0]->Fill(nCombinationsPerEvent5oo6);
		h1CombPerEvent[1][1]->Fill(nCombinationsPerEventClean5oo6);

		if (eventFired6) ++count6;
		if (eventFired5) ++count5;
		if (eventFiredClean6) ++count6Clean;
		if (eventFiredClean5) ++count5Clean;

		if (jentry%5000==0 && jentry) {
			cout << "Processing entry " << setw(12) << jentry << setw(12) << 1.0*countEv/jentry
					<< setw(12) << 1.0*count6/countEv << setw(12) << 1.0*count5/countEv ;
			cout << setw(12) << 1.0*count6Clean/countEv << setw(12) << 1.0*count5Clean/countEv ;
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
			sprintf(cc2," 5 or 6 oo6");
			sprintf(cc5,"_5or6oo6");
		}
		else {
			sprintf(cc2," 6oo6");
			sprintf(cc5,"_6oo6_");
		}
		for (unsigned iClean=0; iClean<1; ++iClean) {
			char cc4[50]="";
			//			if (!iClean) sprintf(cc4,"_NoClean");
			TString sTitle(TString("Stubs per layer")+TString(cc2)+TString(cc4));
			TString sName (TString("cStubsPerLayer")+TString(cc5)+TString(cc4));
			cStubsPerLayer[iMissLay][iClean]= new TCanvas(sName,sTitle,0,0,900,900);
			cStubsPerLayer[iMissLay][iClean]->Divide(3,2);
			for (unsigned iLay=0; iLay <6; ++iLay) {
				cStubsPerLayer[iMissLay][iClean]->cd(iLay+1);
				gPad->SetLogy();
				gPad->SetGrid();
				tl = new TLegend(0.3,0.75,0.9,0.9);
				sprintf(cc,"Stubs in Layer %u",iLay);
				h1StubsPerLayer[iLay][iMissLay][0]->SetTitle(TString(cc)+TString(cc2));
				h1StubsPerLayer[iLay][iMissLay][0]->SetLineWidth(2);
				h1StubsPerLayer[iLay][iMissLay][0]->SetXTitle("# stubs/layer/road");
				h1StubsPerLayer[iLay][iMissLay][0]->SetMinimum(0.5);
				h1StubsPerLayer[iLay][iMissLay][0]->DrawCopy();
				sprintf(cc3,"Mean # stubs: %3.2lf",h1StubsPerLayer[iLay][iMissLay][0]->GetMean());
				tl->AddEntry(h1StubsPerLayer[iLay][iMissLay][0],cc3);

				h1StubsPerLayer[iLay][iMissLay][1]->SetLineWidth(2);
				h1StubsPerLayer[iLay][iMissLay][1]->SetLineColor(2);
				h1StubsPerLayer[iLay][iMissLay][1]->DrawCopy("same");
				sprintf(cc3,"Mean # stubs clean: %3.1lf",h1StubsPerLayer[iLay][iMissLay][1]->GetMean());
				tl->AddEntry(h1StubsPerLayer[iLay][iMissLay][1],cc3);
				tl->Draw("APL");
			}
			sName =(dirPlots+cStubsPerLayer[iMissLay][iClean]->GetName()+TString("_")+pName+TString("_cleanLay")+clayerSelect+TString(".png"));
			if (savePlots) {
				cStubsPerLayer[iMissLay][iClean]->Update();
				cStubsPerLayer[iMissLay][iClean]->SaveAs(sName);
			}
		}
	}


	double  percentiles   [3] = {0.01, 0.5, 0.95};
	double xpercentiles[6][3];
	TCanvas* cRoadPerEvent[2]; // 6oo6, 5oo6 per layer
	TCanvas* cCombPerRoad [2];
	TCanvas* cCombPerEvent[2];
	for (unsigned iLay=0; iLay <2; ++iLay) {
		if (nLayers==6 && iLay) continue;
		char cc [50];
		char cc2[50];
		if      (iLay==0) {
			sprintf(cc ,"_6oo6");
			sprintf(cc2," 6 oo 6");
		}
		else if (iLay==1) {
			sprintf(cc ,"_5or6oo6");
			sprintf(cc2," 5or6 oo 6");
		}
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

	ofstream myfile;
	myfile.open ("example.txt", std::ios_base::app);

	for (unsigned iLay=0; iLay <nLayersLoop; ++iLay) {
		if (nLayers==6 && iLay) continue;
		char cc [150];
		char cc2[150];
		char cc3[150];
		if (iLay==0) {
			sprintf(cc2," 6 oo 6");
			sprintf(cc ,"_6oo6_");
		}
		else if (iLay==1) {
			sprintf(cc2," 5or6 oo 6");
			sprintf(cc ,"_5or6oo6_");
		}
		else continue;
		TLegend * tlr=new TLegend(0.3,0.7,0.9,0.9);
		TLegend * tlc=new TLegend(0.3,0.7,0.9,0.9);
		TLegend * tle=new TLegend(0.3,0.7,0.9,0.9);
		myfile << iLay  << "\t"  << clayerSelect << "\t";
		cout << iLay  << "\t" << clayerSelect << "\t";
		for (unsigned iClean=0; iClean<2; ++iClean) {
			cRoadPerEvent[iLay]->cd();
			gPad->SetLogy();
			gPad->SetGrid();
			h1RoadPerEvent[iLay][iClean]->SetLineWidth(2);
			h1RoadPerEvent[iLay][iClean]->GetQuantiles(3,xpercentiles[0],percentiles);
			if (iClean) {
				h1RoadPerEvent[iLay][iClean]->SetLineColor(2);
				h1RoadPerEvent[iLay][iClean]->DrawCopy("same");
				sprintf(cc3,"# roads clean. #mu=%3.1lf, #Lambda_{95}=%3.1lf",h1RoadPerEvent[iLay][iClean]->GetMean(),xpercentiles[0][2]);
				tlr->AddEntry(h1RoadPerEvent[iLay][iClean],cc3);
				tlr->Draw("APL");
				myfile << h1RoadPerEvent[iLay][iClean]->GetMean() << "\t" << xpercentiles[0][2] << "\t";
				cout << h1RoadPerEvent[iLay][iClean]->GetMean() << "\t" << xpercentiles[0][2] << "\t";
			}
			else {
				h1RoadPerEvent[iLay][1]     ->DrawCopy();
				h1RoadPerEvent[iLay][1]     ->SetXTitle("# roads/tower/BX");
				h1RoadPerEvent[iLay][1]     ->SetTitle (TString("Roads per Tower")+cc2);
				h1RoadPerEvent[iLay][iClean]->DrawCopy("same");
				sprintf(cc3,"# roads. #mu=%3.1lf, #Lambda_{95}=%3.1lf",h1RoadPerEvent[iLay][iClean]->GetMean(),xpercentiles[0][2]);
				tlr->AddEntry(h1RoadPerEvent[iLay][iClean],cc3);
			}
			cCombPerRoad[iLay]->cd();
			gPad->SetLogy();
			gPad->SetGrid();
			h1CombPerRoad[iLay][iClean]->SetLineWidth(2);
			h1CombPerRoad[iLay][iClean]->GetQuantiles(3,xpercentiles[0],percentiles);
			if (iClean) {
				h1CombPerRoad[iLay][iClean]->SetLineColor(2);
				h1CombPerRoad[iLay][iClean]->DrawCopy("same");
				sprintf(cc3,"# combs clean. #mu=%3.1lf, #Lambda_{95}=%3.1lf",h1CombPerRoad[iLay][iClean]->GetMean(),xpercentiles[0][2]);
				tlc->AddEntry(h1CombPerRoad[iLay][iClean],cc3);
				tlc->Draw("APL");
				myfile << h1CombPerRoad[iLay][iClean]->GetMean() << "\t" << xpercentiles[0][2] << "\t";
				cout << h1CombPerRoad[iLay][iClean]->GetMean() << "\t" << xpercentiles[0][2] << "\t";
			}
			else {
				h1CombPerRoad[iLay][1]     ->DrawCopy();
				h1CombPerRoad[iLay][1]     ->SetXTitle("# combs/road/BX");
				h1CombPerRoad[iLay][1]     ->SetTitle (TString("Combs per Road")+cc2);
				h1CombPerRoad[iLay][iClean]->DrawCopy("same");
				sprintf(cc3,"# combs. #mu=%3.1lf, #Lambda_{95}=%3.1lf",h1CombPerRoad[iLay][iClean]->GetMean(),xpercentiles[0][2]);
				tlc->AddEntry(h1CombPerRoad[iLay][iClean],cc3);
			}

			cCombPerEvent[iLay]->cd();
			gPad->SetLogy();
			gPad->SetGrid();
			h1CombPerEvent[iLay][iClean]->SetLineWidth(2);
			h1CombPerEvent[iLay][iClean]->GetQuantiles(3,xpercentiles[0],percentiles);
			if (iClean) {
				h1CombPerEvent[iLay][iClean]->SetLineColor(2);
				h1CombPerEvent[iLay][iClean]->DrawCopy("same");
				sprintf(cc3,"# combs clean. #mu=%3.1lf, #Lambda_{95}=%3.1lf",h1CombPerEvent[iLay][iClean]->GetMean(),xpercentiles[0][2]);
				tle->AddEntry(h1CombPerEvent[iLay][iClean],cc3);
				myfile << h1CombPerEvent[iLay][iClean]->GetMean() << "\t" << xpercentiles[0][2] << "\n";
				cout << h1CombPerEvent[iLay][iClean]->GetMean() << "\t" << xpercentiles[0][2] << "\n";
			}
			else {
				h1CombPerEvent[iLay][1]     ->DrawCopy();
				h1CombPerEvent[iLay][1]     ->SetXTitle("# combs/tower/BX");
				h1CombPerEvent[iLay][1]     ->SetTitle (TString("Combs per Tower per BX")+cc2);
				h1CombPerEvent[iLay][iClean]->DrawCopy("same");
				sprintf(cc3,"# combs. #mu=%3.1lf, #Lambda_{95}=%3.1lf",h1CombPerEvent[iLay][iClean]->GetMean(),xpercentiles[0][2]);
				tle->AddEntry(h1CombPerEvent[iLay][iClean],cc3);
				tle->Draw("APL");
			}

			TCanvas* cCombEtaPhi = new TCanvas("cCombEtaPhi","cCombEtaPhi",0,0,900,900);
			hProfile2DCombEtaPhi   [0][0]->DrawCopy("colz0");
		}

		if (savePlots) {
			sName=(dirPlots+TString("roadsPerEvent")+TString(cc)+pName+TString("_cleanLay")+clayerSelect+TString(".png"));
			cRoadPerEvent [iLay]->Update();
			cRoadPerEvent [iLay]->SaveAs(sName);
			sName=(dirPlots+TString("combsPerRoad")+TString(cc)+pName+TString("_cleanLay")+clayerSelect+TString(".png"));
			cCombPerRoad  [iLay]->Update();
			cCombPerRoad  [iLay]->SaveAs(sName);
			sName=(dirPlots+TString("combsPerEvent")+TString(cc)+pName+TString("_cleanLay")+clayerSelect+TString(".png"));
			cCombPerEvent [iLay]->Update();
			cCombPerEvent [iLay]->SaveAs(sName);
		}

		//		if (nLayers==5) {
		//			cRoadPerEvent[1]->cd();
		//			gPad->SetLogy();
		//			for (unsigned iClean=0; iClean<2; ++iClean) {
		//				h1RoadPerEvent[iLay][iClean]->SetLineWidth(2);
		//				if (iClean) {
		//					h1RoadPerEvent[iLay][iClean]->SetLineColor(2);
		//					h1RoadPerEvent[iLay][iClean]->DrawCopy("same");
		//				}
		//				else {
		//					h1RoadPerEvent[iLay][1]     ->DrawCopy();
		//					h1RoadPerEvent[iLay][iClean]->DrawCopy("same");
		//				}
		//			}
		//			cCombPerRoad[1]->cd();
		//			gPad->SetLogy();
		//			for (unsigned iClean=0; iClean<2; ++iClean) {
		//				h1CombPerRoad[iLay][iClean]->SetLineWidth(2);
		//				if (iClean) {
		//					h1CombPerRoad[iLay][iClean]->SetLineColor(2);
		//					h1CombPerRoad[iLay][iClean]->DrawCopy("same");
		//				}
		//				else {
		//					h1CombPerRoad[iLay][1]     ->DrawCopy();
		//					h1CombPerRoad[iLay][iClean]->DrawCopy("same");
		//				}
		//			}
		//			cCombPerEvent[1]->cd();
		//			gPad->SetLogy();
		//			for (unsigned iClean=0; iClean<2; ++iClean) {
		//				h1CombPerEvent[iLay][iClean]->SetLineWidth(2);
		//				if (iClean) {
		//					h1CombPerEvent[iLay][iClean]->SetLineColor(2);
		//					h1CombPerEvent[iLay][iClean]->DrawCopy("same");
		//				}
		//				else {
		//					h1CombPerEvent[iLay][1]     ->DrawCopy();
		//					h1CombPerEvent[iLay][iClean]->DrawCopy("same");
		//				}
		//			}
		//		}
	}
	myfile.close();

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
