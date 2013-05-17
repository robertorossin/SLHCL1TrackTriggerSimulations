/*
  Small ROOT macro showing an example of extracted data analysis.

  Use:

  root[1]-> .L Analyze_digis.C
  root[2]->  do_ana(filename)

  where filename is the name of the extracted ROOT file

  Information about this macro may be found in the following page:

  http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.HLLHCTuto (STEP III)

  Author: viret@in2p3_dot_fr
  Date: 16/05/2013

*/

void do_ana(std::string filename)
{
  gROOT->ProcessLine("#include <vector>");

  // First get the data
  // by merging all the available files

  TChain *Pix               = new TChain("Pixels");   

  Pix->Add(filename.c_str());

//
//1/ Pixels TREE content:
//
// http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/HL_LHC/Extractors/RecoExtractor/interface/MCExtractor.h?view=markup
//

  int                 m_pix; 

  std::vector<float>  *m_pix_x;  
  std::vector<float>  *m_pix_y;  
  std::vector<float>  *m_pix_z;  
  std::vector<float>  *m_pix_e;  
  std::vector<int>    *m_pix_row;   
  std::vector<int>    *m_pix_column;
  std::vector<int>    *m_pix_simhit;
  std::vector<int>    *m_pix_layer; 
  std::vector<int>    *m_pix_module;
  std::vector<int>    *m_pix_ladder;

  Pix->SetBranchAddress("PIX_n",         &m_pix);
  Pix->SetBranchAddress("PIX_x",         &m_pix_x);
  Pix->SetBranchAddress("PIX_y",         &m_pix_y);
  Pix->SetBranchAddress("PIX_z",         &m_pix_z);
  Pix->SetBranchAddress("PIX_charge",    &m_pix_e);
  Pix->SetBranchAddress("PIX_row",       &m_pix_row);
  Pix->SetBranchAddress("PIX_column",    &m_pix_column);
  Pix->SetBranchAddress("PIX_simhit",    &m_pix_simhit);
  Pix->SetBranchAddress("PIX_layer",     &m_pix_layer);
  Pix->SetBranchAddress("PIX_module",    &m_pix_module);
  Pix->SetBranchAddress("PIX_ladder",    &m_pix_ladder);


  double PI = 4.*atan(1.);


  TH2F* Barrel_hitmap = new TH2F("BHmap","",60,-120.,120.,20,-PI,PI);

  int n_entries = Pix->GetEntries();

  for (int j=0;j<n_entries;++j)
  {
    if (j%1000==0)
      cout << j <<endl;

    Pix->GetEntry(j);

    for (int i=0;i<m_pix;++i)
    {
      if (m_pix_layer->at(i) == 5) 
      {
	Barrel_hitmap->Fill(m_pix_z->at(i),atan2(m_pix_y->at(i),m_pix_x->at(i)));
      }
    }
  }

  const Int_t NRGBs = 5;
  const Int_t NCont = 255;
  
  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  c1 = new TCanvas("c1","Tracker barrel layer digi map",200,80,1100,640);
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->Range(-149,-3.9,149.,3.6);
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetBorderSize(2);
  c1->SetRightMargin(0.066);
  c1->SetTopMargin(0.066);
  c1->SetFrameBorderMode(0);
  c1->SetFrameBorderMode(0);

  Barrel_hitmap->GetXaxis()->SetTitle("Digi z (in cm)");
  Barrel_hitmap->GetXaxis()->SetLabelFont(42);
  Barrel_hitmap->GetXaxis()->SetLabelSize(0.035);
  Barrel_hitmap->GetXaxis()->SetTitleSize(0.035);
  Barrel_hitmap->GetXaxis()->SetTitleFont(42);
  Barrel_hitmap->GetYaxis()->SetTitle("Digi #phi (in rad)");
  Barrel_hitmap->GetYaxis()->SetLabelFont(42);
  Barrel_hitmap->GetYaxis()->SetLabelSize(0.035);
  Barrel_hitmap->GetYaxis()->SetTitleSize(0.035);
  Barrel_hitmap->GetYaxis()->SetTitleFont(42);
  Barrel_hitmap->GetZaxis()->SetLabelFont(42);
  Barrel_hitmap->GetZaxis()->SetLabelSize(0.035);
  Barrel_hitmap->GetZaxis()->SetTitleSize(0.035);
  Barrel_hitmap->GetZaxis()->SetTitleFont(42);
  Barrel_hitmap->Draw("col");

  c1->Update();
}
