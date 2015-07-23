#define patternAttributes_cxx
#include "patternAttributes.h"
#include <TH2.h>
#include <TProfile.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

struct patternS {
	double invPt_mean;
	double invPt_sigma;
	double cotTheta_mean;
	double cotTheta_sigma;
	double phi_mean;
	double phi_sigma;
	double z0_mean;
	double z0_sigma;
	double frequency;
  std::vector <unsigned int > superstripIds;

	bool operator<(const patternS& a) const
	{
		return invPt_mean< a.invPt_mean;
	}

};

long unsigned int patternAttributes::ReadPercentile(TString sTree, TString &pName) {
//	std::cout << sTree.Data() << std::endl;
	if (percentile==95) {
		if(sTree.Contains("SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/patternBank_SingleMuonFlatOneOverPt0p0005To0p5_tt27_sf1_nz4_pt2_SLHC25p3_100M.root")){
			pName=TString("_pattern_sf1_nz4_pt2");
			return 3100200;
		}
	}
	if (percentile==99) {
		if(sTree.Contains("SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/patternBank_SingleMuonFlatOneOverPt0p0005To0p5_tt27_sf1_nz4_pt2_SLHC25p3_100M.root")) {
			 pName=TString("_pattern_sf1_nz4_pt2");
			 return 1E7;
		}
	}
	if (percentile==100) return 1E9;
	return 0;
}

void patternAttributes::Loop(TString sTree, int makePlots)
{
	//   In a ROOT session, you can do:
	//      Root > .L patternAttributes.C
	//      Root > patternAttributes t
	//      Root > t.GetEntry(12); // Fill t data members with entry number 12
	//      Root > t.Show();       // Show values of entry 12
	//      Root > t.Show(16);     // Read and show values of entry 16
	//      Root > t.Loop();       // Loop on all entries
	//

	//     This is the loop skeleton where:
	//    jentry is the global entry number in the chain
	//    ientry is the entry number in the current Tree
	//  Note that the argument to GetEntry must be:
	//    jentry for TChain::GetEntry
	//    ientry for TTree::GetEntry and TBranch::GetEntry
	//
	//       To read only selected branches, Insert statements like:
	// METHOD1:
	//    fChain->SetBranchStatus("*",0);  // disable all branches
	//    fChain->SetBranchStatus("branchname",1);  // activate branchname
	// METHOD2: replace line
	//    fChain->GetEntry(jentry);       //read all branches
	//by  b_branchname->GetEntry(ientry); //read only this branch
	if (fChain     == 0) return;
	if (fChainAttr == 0) return;

	Long64_t nentries     = fChain    ->GetEntriesFast();
	Long64_t nentriesAttr = fChainAttr->GetEntriesFast();

	TString splotDir("/home/rossin/Dropbox/TT/Work/figures_patternPars/");
	if (makePlots) cout << "Saving plots in " << splotDir.Data() <<endl;

	if (nentries != nentriesAttr) {
		std::cout << "nentries != nentriesAttr. Exiting..." << std::endl;
		return;
	}

	std::cout << "Opening pattern ntuple\n" <<  sTree.Data() << std::endl;

	std::cout << "Patterns in ntuple: " << nentries << std::endl;
	TString pName;
	long unsigned int nPercentile = ReadPercentile(sTree,pName);
	nentries = ((int)nPercentile<nentries ? nPercentile : nentries);
	std::cout << "Loading patterns " << nentries << " for " << percentile  << "% coverage." << std::endl;

	std::vector <patternS> pattS;

	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		ientry = LoadTreeAttr(jentry);
		fChain->GetEntry(jentry);
		fChainAttr->GetEntry(jentry);

    patternS p;
  	p.invPt_mean     = invPt_mean    ;
  	p.invPt_sigma    = invPt_sigma   ;
  	p.cotTheta_mean  = cotTheta_mean ;
  	p.cotTheta_sigma = cotTheta_sigma;
  	p.phi_mean       = phi_mean      ;
  	p.phi_sigma      = phi_sigma     ;
  	p.z0_mean        = z0_mean       ;
  	p.z0_sigma       = z0_sigma      ;
  	p.frequency      = frequency     ;

  	for (unsigned int iSS=0; iSS<superstripIds->size(); ++iSS) {
  		p.superstripIds.push_back(superstripIds->at(iSS));
  	}
  	pattS.push_back(p);
  	if (jentry%100000==0) {
  		std::cout << "Loading pattern " << jentry << std::endl;
//  		std::cout << std::setw(10)
//								<< p.invPt_mean << std::setw(10)
//  				      << p.cotTheta_mean << std::setw(10)
//								<< p.phi_mean << std::setw(10)
//								<< p.z0_mean << std::setw(5)
//								<< p.frequency;
//  		for (unsigned i=0; i<superstripIds->size(); ++i)
//  			std::cout << std::setw(5) << p.superstripIds[i];
//  		std::cout << std::endl;
  	}
	}
	std::cout << "Sorting patterns by invPt" << std::endl;
	std::sort(pattS.begin(), pattS.end());
	std::cout << "Done" <<std::endl;

  const unsigned long int Npatt = pattS.size();

  float *vinvPt_mean     =new  float[Npatt];
  float *vinvPt_sigma    =new  float[Npatt];
  float *vcotTheta_mean  =new  float[Npatt];
  float *vcotTheta_sigma =new  float[Npatt];
  float *vphi_mean       =new  float[Npatt];
  float *vphi_sigma      =new  float[Npatt];
  float *vz0_mean        =new  float[Npatt];
  float *vz0_sigma       =new  float[Npatt];
  float *vfrequency      =new  float[Npatt];
  float *vcount          =new  float[Npatt];
  float *vzeros          =new  float[Npatt];
  int   *vcharge         =new    int[Npatt];
  float *vchargeMisID    =new  float[Npatt];

  float invPt_mean_max = 0.5;
  unsigned nBinsx_invPt_sigma = 1000;
  unsigned nBinsy_invPt_sigma = 500;
  float invPt_sigma_max = 0.02;
  float cotTheta_mean_min = 0.0;
  float cotTheta_mean_max = 0.8;
  float cotTheta_sigma_max = 0.15;
  float phi_sigma_max = 0.005;
  float z0_mean_max  = 15;
  float z0_sigma_max = 5;
  float eta_mean_min = 0.0;
  float eta_mean_max = 0.75;
  float eta_sigma_max = 0.12;
//  unsigned nBins_cotTheta_mean = 1000;
//  float cotTheta_mean_min = -0.5;
//  float cotTheta_mean_max = 1.5;

  TH2*         h2invPt_sigma     = new TH2D    (   "h2invPt_sigma"   ,   "h2invPt_sigma"   , nBinsx_invPt_sigma, -invPt_mean_max  , invPt_mean_max  ,nBinsy_invPt_sigma,0                ,   invPt_sigma_max);
  TProfile*     pinvPt_sigma     = new TProfile(    "pinvPt_sigma"   ,    "pinvPt_sigma"   , nBinsx_invPt_sigma, -invPt_mean_max  , invPt_mean_max  ,                   0                ,   invPt_sigma_max);
  TH2*      h2cotTheta_mean      = new TH2D    ( "h2cotTheta_mean"   , "h2cotTheta_mean"   , nBinsx_invPt_sigma, -invPt_mean_max  , invPt_mean_max  ,nBinsy_invPt_sigma,cotTheta_mean_min,cotTheta_mean_max);
  TProfile*  pcotTheta_mean      = new TProfile(  "pcotTheta_mean"   ,  "pcotTheta_mean"   , nBinsx_invPt_sigma, -invPt_mean_max  , invPt_mean_max  ,                   0                ,cotTheta_sigma_max);
  TH2*      h2cotTheta_sigma     = new TH2D    ("h2cotTheta_sigma"   ,"h2cotTheta_sigma"   , nBinsx_invPt_sigma, cotTheta_mean_min,cotTheta_mean_max,nBinsy_invPt_sigma,0                ,cotTheta_sigma_max);
  TProfile*  pcotTheta_sigma     = new TProfile( "pcotTheta_sigma"   , "pcotTheta_sigma"   , nBinsx_invPt_sigma, cotTheta_mean_min,cotTheta_mean_max,                   0                ,cotTheta_sigma_max);
  TH2*         h2phi_sigma       = new TH2D    (     "h2phi_sigma"   ,     "h2phi_sigma"   , nBinsx_invPt_sigma, -invPt_mean_max  , invPt_mean_max  ,nBinsy_invPt_sigma,0                ,   phi_sigma_max);
  TProfile*     pphi_sigma       = new TProfile(      "pphi_sigma"   ,      "pphi_sigma"   , nBinsx_invPt_sigma, -invPt_mean_max  , invPt_mean_max  ,                   0                ,   phi_sigma_max);
  TH2*         h2z0_sigma_z0     = new TH2D    (      "h2z0_sigma_z0",      "h2z0_sigma_z0", nBinsx_invPt_sigma, -   z0_mean_max  ,    z0_mean_max  ,nBinsy_invPt_sigma,0                ,    z0_sigma_max);
  TProfile*     pz0_sigma_z0     = new TProfile(       "pz0_sigma_z0",       "pz0_sigma_z0", nBinsx_invPt_sigma, -   z0_mean_max  ,    z0_mean_max  ,                   0                ,    z0_sigma_max);
  TH2*         h2z0_sigma_cTheta = new TH2D    (      "h2z0_sigma_cTheta",      "h2z0_sigma_cTheta", nBinsx_invPt_sigma, cotTheta_mean_min  ,cotTheta_mean_max,nBinsy_invPt_sigma,0                ,    z0_sigma_max);
  TProfile*     pz0_sigma_cTheta = new TProfile(       "pz0_sigma_cTheta",       "pz0_sigma_cTheta", nBinsx_invPt_sigma, cotTheta_mean_min  ,cotTheta_mean_max,                   0                ,    z0_sigma_max);
  TH2*      h2eta_mean      = new TH2D    ( "h2eta_mean"   , "h2eta_mean"   , nBinsx_invPt_sigma, -invPt_mean_max  , invPt_mean_max  ,nBinsy_invPt_sigma,eta_mean_min,eta_mean_max);
  TProfile*  peta_mean      = new TProfile(  "peta_mean"   ,  "peta_mean"   , nBinsx_invPt_sigma, -invPt_mean_max  , invPt_mean_max  ,                   0                ,eta_sigma_max);
  TH2*      h2eta_sigma     = new TH2D    ("h2eta_sigma"   ,"h2eta_sigma"   , nBinsx_invPt_sigma, eta_mean_min,eta_mean_max,nBinsy_invPt_sigma,0                ,eta_sigma_max);
  TProfile*  peta_sigma     = new TProfile( "peta_sigma"   , "peta_sigma"   , nBinsx_invPt_sigma, eta_mean_min,eta_mean_max,                   0                ,eta_sigma_max);

	for (unsigned iPatt=0; iPatt<pattS.size(); ++iPatt) {
		patternS p(pattS.at(iPatt));
		vinvPt_mean    [iPatt] = p.invPt_mean;
		vinvPt_sigma   [iPatt] = p.invPt_sigma;
		vcotTheta_mean [iPatt] = p.cotTheta_mean;
		vcotTheta_sigma[iPatt] = p.cotTheta_sigma;
		vphi_mean      [iPatt] = p.phi_mean;
		vphi_sigma     [iPatt] = p.phi_sigma;
		vcount         [iPatt] = iPatt;
		vzeros         [iPatt] = 0;

		h2invPt_sigma    ->Fill(p.invPt_mean   ,p.invPt_sigma);
		pinvPt_sigma     ->Fill(p.invPt_mean   ,p.invPt_sigma);
		h2cotTheta_mean  ->Fill(p.invPt_mean   ,p.cotTheta_mean );
		pcotTheta_mean   ->Fill(p.invPt_mean   ,p.cotTheta_mean );
		h2cotTheta_sigma ->Fill(p.cotTheta_mean,p.cotTheta_sigma);
		pcotTheta_sigma  ->Fill(p.cotTheta_mean,p.cotTheta_sigma);
		h2phi_sigma      ->Fill(p.invPt_mean   ,p.phi_sigma);
		pphi_sigma       ->Fill(p.invPt_mean   ,p.phi_sigma);
		h2z0_sigma_z0    ->Fill(p.z0_mean      ,p.z0_sigma);
		pz0_sigma_z0     ->Fill(p.z0_mean      ,p.z0_sigma);
		h2z0_sigma_cTheta->Fill(p.cotTheta_mean,p.z0_sigma);
		 pz0_sigma_cTheta->Fill(p.cotTheta_mean,p.z0_sigma);

		double jacobian = 1/TMath::Sqrt(1+p.cotTheta_mean*p.cotTheta_mean);
		h2eta_mean  ->Fill(p.invPt_mean                 ,TMath::ASinH(p.cotTheta_mean) );
		 peta_mean  ->Fill(p.invPt_mean                 ,TMath::ASinH(p.cotTheta_mean) );
		h2eta_sigma ->Fill(TMath::ASinH(p.cotTheta_mean),jacobian*p.cotTheta_sigma     );
		 peta_sigma ->Fill(TMath::ASinH(p.cotTheta_mean),jacobian*p.cotTheta_sigma     );

		if (iPatt%100000==0) std::cout << "Processing pattern " << iPatt<< std::endl;
	}

	unsigned iStagger=0;
	TGraph*  ginvPt_mean = new TGraph(Npatt,vcount,vinvPt_mean);
  TCanvas* cinvPt_mean = new TCanvas("cinvPt_mean","cinvPt_mean",(iStagger)*10,(++iStagger)*10,900,900);
  cinvPt_mean->SetGrid();
  ginvPt_mean->Draw("AP");
  if (makePlots) cinvPt_mean->SaveAs(splotDir+TString(cinvPt_mean->GetName())+pName+TString(".png"));

  TCanvas* cinvPt_sigma = new TCanvas("cinvPt_sigma","cinvPt_sigma",(iStagger)*10,(++iStagger)*10,900,900);
  cinvPt_sigma->SetGrid();
  cinvPt_sigma->SetLogz();
  double zmax = h2invPt_sigma->GetZaxis()->GetXmax();
  h2invPt_sigma->GetZaxis()->SetRangeUser(1,200);
  h2invPt_sigma->DrawCopy("colz0");
  pinvPt_sigma ->SetMarkerStyle(20);
  pinvPt_sigma ->SetMarkerSize (0.3);
  pinvPt_sigma ->SetMarkerColor(2);
  pinvPt_sigma ->DrawCopy("same");
  if (makePlots) cinvPt_sigma->SaveAs(splotDir+TString(cinvPt_sigma->GetName())+pName+TString(".png"));

  TGraph* ginvPt_sigma     = new TGraph(Npatt,vcount,vinvPt_sigma);
  TGraph* gcotTheta_sigma  = new TGraph(Npatt,vinvPt_mean,vcotTheta_sigma);

//  TCanvas* ccotTheta_mean = new TCanvas("ccotTheta_mean","ccotTheta_mean",(iStagger)*10,(iStagger++)*10,900,900);
//  ccotTheta_mean->SetGrid();
//  ccotTheta_mean->SetLogz();
//  h2cotTheta_mean->GetZaxis()->SetRangeUser(1,100);
//  h2cotTheta_mean->DrawCopy("colz0");
//  pcotTheta_mean ->SetMarkerStyle(20);
//  pcotTheta_mean ->SetMarkerSize (0.3);
//  pcotTheta_mean ->SetMarkerColor(2);
//  pcotTheta_mean ->DrawCopy("same");

  TCanvas* ccotTheta_sigma = new TCanvas("ccotTheta_sigma","ccotTheta_sigma",(iStagger)*10,(++iStagger)*10,900,900);
  ccotTheta_sigma->SetGrid();
  ccotTheta_sigma->SetLogz();
  h2cotTheta_sigma->GetZaxis()->SetRangeUser(1,100);
  h2cotTheta_sigma->SetTitle("");
  h2cotTheta_sigma->SetXTitle("cot(#theta)");
  h2cotTheta_sigma->SetYTitle("#sigma(cot(#theta))");
  h2cotTheta_sigma->DrawCopy("colz0");
  pcotTheta_sigma ->SetMarkerStyle(20);
  pcotTheta_sigma ->SetMarkerSize (0.3);
  pcotTheta_sigma ->SetMarkerColor(2);
   if (makePlots) ccotTheta_sigma->SaveAs(splotDir+TString(ccotTheta_sigma->GetName())+pName+TString(".png"));

  TCanvas* cphi_sigma = new TCanvas("cphi_sigma","cphi_sigma",(iStagger)*10,(++iStagger)*10,900,900);
  cphi_sigma->SetGrid();
  cphi_sigma->SetLogz();
  h2phi_sigma->GetZaxis()->SetRangeUser(1,100);
  h2phi_sigma->SetTitle("");
  h2phi_sigma->SetXTitle("#phi_{0}");
  h2phi_sigma->SetYTitle("#sigma(#phi_{0})");
  h2phi_sigma->DrawCopy("colz0");
  pphi_sigma ->SetMarkerStyle(20);
  pphi_sigma ->SetMarkerSize (0.3);
  pphi_sigma ->SetMarkerColor(2);
  pphi_sigma->DrawCopy("same");
  if (makePlots) cphi_sigma->SaveAs(splotDir+TString(cphi_sigma->GetName())+pName+TString(".png"));

  TCanvas* cz0_sigma_z0 = new TCanvas("cz0_sigma_z0","cz0_sigma_z0",(iStagger)*10,(++iStagger)*10,900,900);
  cz0_sigma_z0->SetGrid();
  cz0_sigma_z0->SetLogz();
  h2z0_sigma_z0->GetZaxis()->SetRangeUser(1,100);
  h2z0_sigma_z0->SetTitle("");
  h2z0_sigma_z0->SetXTitle("z_{0} cm");
  h2z0_sigma_z0->SetYTitle("#sigma(z_{0}) cm");
  h2z0_sigma_z0->DrawCopy("colz0");
  if (makePlots) cz0_sigma_z0->SaveAs(splotDir+TString(cz0_sigma_z0->GetName())+pName+TString(".png"));

  TCanvas* cz0_sigma_cTheta = new TCanvas("cz0_sigma_cTheta","cz0_sigma_cTheta",(iStagger)*10,(++iStagger)*10,900,900);
  cz0_sigma_cTheta->SetGrid();
  cz0_sigma_cTheta->SetLogz();
  h2z0_sigma_cTheta->GetZaxis()->SetRangeUser(1,100);
  h2z0_sigma_cTheta->SetTitle("");
  h2z0_sigma_cTheta->SetXTitle("cot(#theta)");
  h2z0_sigma_cTheta->SetYTitle("#sigma(z_{0}) cm");
  h2z0_sigma_cTheta->DrawCopy("colz0");
  if (makePlots) cz0_sigma_cTheta->SaveAs(splotDir+TString(cz0_sigma_cTheta->GetName())+pName+TString(".png"));

  TCanvas* ceta_sigma = new TCanvas("ceta_sigma","ceta_sigma",(iStagger)*10,(++iStagger)*10,900,900);
  ceta_sigma->SetGrid();
  ceta_sigma->SetLogz();
  h2eta_sigma->GetZaxis()->SetRangeUser(1,100);
  h2eta_sigma->SetTitle("");
  h2eta_sigma->SetXTitle("#eta");
  h2eta_sigma->SetYTitle("#sigma(#eta)");
  h2eta_sigma->DrawCopy("colz0");
  peta_sigma ->SetMarkerStyle(20);
  peta_sigma ->SetMarkerSize (0.3);
  peta_sigma ->SetMarkerColor(2);
  if (makePlots) ceta_sigma->SaveAs(splotDir+TString(ceta_sigma->GetName())+pName+TString(".png"));

}

void patternBankAttributes(TString sTree, int perc=95, int makePlots=0) {

	gStyle->SetOptStat(0);
	const unsigned nC=35;
	int colors[nC];
	for (unsigned i=0; i<nC; ++i) colors[i]=10+i;
	gStyle->SetPalette(nC,colors);
	patternAttributes p(sTree);
	p.percentile=perc;
	p.Loop(sTree,makePlots);
	return;
}
