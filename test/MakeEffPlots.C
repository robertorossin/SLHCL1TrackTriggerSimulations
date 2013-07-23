/*
  ROOT macro for stub efficiencies visualization

  Use:

  root[1]-> .L MakeEffPlots.C

  Then you can choose between the following methods 
  knowing that filename denotes the extracted ROOT file 
  containing the stub efficiencies (created by the 
  sectorMaker program).

  For infos about the efficiency definition, have a look at the following presentation:

  https://indico.cern.ch/getFile.py/access?contribId=0&resId=0&materialId=slides&confId=263068

  Solution 1 do all the plots (lot of infos)
  root[2]-> do_efficiency_plots(std::string filename) 

  Solution 2 just plot the comparicon pT plots for the barrel
  root[2]-> do_pt_barrel_summary(std::string filename,int type) 

  where type=0 is for the module efficiency, and type=1 for the 
  layer/disk efficiency

  Solution 3 just plot the comparicon pT plots for the endcap
  root[2]-> do_pt_endcap_summary(std::string filename,int type) 

  Solution 4 do the plot for barrel layer i
  root[2]-> do_layer_map(std::string filename,int layer,int type)

  Solution 5 do the plot for endcap disk i
  root[2]-> do_disk_map(std::string filename,int disk,int type)

  Information about this macro may be found in the following page:

  http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.HLLHCTuto (STEP III part 2.4)

  Author: viret@in2p3_dot_fr
  Date: 20/07/2013
*/

#include <iomanip>      // for std::setprecision

void do_efficiency_plots(std::string filename)
{
  for (int i=0;i<20;++i)
  { 
    do_pt_plots(filename,i,0);
    do_pt_plots(filename,i,1);
  }

  do_pt_barrel_summary(filename,0);
  do_pt_barrel_summary(filename,1);
  do_pt_endcap_summary(filename,0);
  do_pt_endcap_summary(filename,1);


  for (int i=0;i<13;++i)
  { 
    do_eta_plots(filename,i,0);
    do_eta_plots(filename,i,1);
  }
}

void do_pt_plots(std::string filename,int layer,int per_lay)
{
  // First get the data
  // by merging all the available files

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
 
  float pt_val[100];

  float digi_pt[20][100];

  float clus_off_pt[20][100];
  float stub_off_pt[20][100];

  float clus_pri_pt[20][100];
  float stub_pri_pt[20][100];

  int disk =0;

  if (layer>=6  && layer<13) disk=int(layer-5)%8;
  if (layer>=13 && layer<20) disk=-int(layer-12)%8;

  TFile *file    = TFile::Open(filename.c_str());
  TTree *newtree = (TTree*)file->Get("Efficiencies");

  newtree->SetBranchAddress("pt_val",&pt_val);
  newtree->SetBranchAddress("digi_pt",&digi_pt);
  newtree->SetBranchAddress("clus_off_pt",&clus_off_pt);
  newtree->SetBranchAddress("stub_off_pt",&stub_off_pt);
  newtree->SetBranchAddress("clus_pri_pt",&clus_pri_pt);
  if (per_lay==1)
  {
    newtree->SetBranchAddress("stub_pri_pt_lay",&stub_pri_pt);
  }
  else
  {
    newtree->SetBranchAddress("stub_pri_pt",&stub_pri_pt);
  }

  newtree->GetEntry(0);

  TH2F *cadre    = new TH2F("zz","zz",300,0.,20.,100,0.,1.02);
  TH2F *digi_eff = new TH2F("deff","deff",300,0.,20.,100,0.,1.02);
  TH2F *coff_eff = new TH2F("coeff","coeff",300,0.,20.,100,0.,1.02);
  TH2F *cpri_eff = new TH2F("cpeff","cpeff",300,0.,20.,100,0.,1.02);
  TH2F *soff_eff = new TH2F("soeff","soeff",300,0.,20.,100,0.,1.02);
  TH2F *spri_eff = new TH2F("speff","speff",300,0.,20.,100,0.,1.02);

  for (int i=0;i<100;++i)
  { 
    digi_eff->Fill(pt_val[i],digi_pt[layer][i]);
    coff_eff->Fill(pt_val[i],clus_off_pt[layer][i]);
    cpri_eff->Fill(pt_val[i],clus_pri_pt[layer][i]);
    soff_eff->Fill(pt_val[i],stub_off_pt[layer][i]);
    spri_eff->Fill(pt_val[i],stub_pri_pt[layer][i]);
  }

  c1 = new TCanvas("c1","Private effs",5,75,1470,860);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  c1->Range(-1.833856,-0.1286626,21.1442,1.157964);
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetBorderSize(2);
  c1->SetGridx();
  c1->SetGridy();
  c1->SetLeftMargin(0.08);
  c1->SetRightMargin(0.05);
  c1->SetFrameBorderMode(0);
  c1->SetFrameBorderMode(0);

  cadre->GetXaxis()->SetTitle("Tracking particle p_{T} (in GeV/c)");
  cadre->GetXaxis()->SetNdivisions(522);
  cadre->GetXaxis()->SetLabelFont(42);
  cadre->GetXaxis()->SetLabelSize(0.03);
  cadre->GetXaxis()->SetTitleFont(42);
  cadre->GetYaxis()->SetTitle("Efficiency");
  cadre->GetYaxis()->SetNdivisions(511);
  cadre->GetYaxis()->SetLabelFont(42);
  cadre->GetYaxis()->SetLabelSize(0.03);
  cadre->GetYaxis()->SetTitleOffset(0.8);
  cadre->GetYaxis()->SetTitleFont(42);
  cadre->Draw();
  
  digi_eff->SetMarkerStyle(3);
  cpri_eff->SetMarkerStyle(4);
  spri_eff->SetMarkerStyle(20);
  digi_eff->Draw("same");
  cpri_eff->Draw("same");
  spri_eff->Draw("same");

  leg = new TLegend(0.7,0.28,0.85,0.4);

  leg->SetTextSize(0.03);
  leg->SetFillColor(5);
  leg->AddEntry(digi_eff,"Digis","p");
  leg->AddEntry(cpri_eff,"Clusters","p");
  leg->AddEntry(spri_eff,"Stubs","p");
  leg->Draw();
    
  TPaveText *pt = new TPaveText(4.3,1.05,15.2,1.11,"br");

  char buffer[80];

  if (disk==0)
    sprintf(buffer, "Turn-on curve for tracker barrel layer %d (private stubs)",layer+5);

  if (disk>0)
    sprintf(buffer, "Turn-on curve for tracker endcap disk %d (private stubs)",disk);

  if (disk<0)
    sprintf(buffer, "Turn-on curve for tracker endcap disk %d (private stubs)",disk);

  ci = TColor::GetColor("#ccccff");
  pt->SetFillColor(ci);
  pt->SetTextSize(0.02798053);
  TText *text = pt->AddText(buffer);
  pt->Draw();
  c1->Modified();
  c1->Update();

  if (per_lay==1)
  {
    if (disk==0) sprintf (buffer, "Barrel_%d_layer_private_eff.png", layer+5);
    if (disk>0)  sprintf (buffer, "Endcap_p%d_layer_private_eff.png", disk); 
    if (disk<0)  sprintf (buffer, "Endcap_m%d_layer_private_eff.png", abs(disk)); 
  }
  else
  {
    if (disk==0) sprintf (buffer, "Barrel_%d_module_private_eff.png", layer+5);
    if (disk>0)  sprintf (buffer, "Endcap_p%d_module_private_eff.png", disk); 
    if (disk<0)  sprintf (buffer, "Endcap_m%d_module_private_eff.png", abs(disk)); 
  }

  c1->Print(buffer);


  c2 = new TCanvas("c2","Official effs",5,75,1470,860);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  c2->Range(-1.833856,-0.1286626,21.1442,1.157964);
  c2->SetFillColor(0);
  c2->SetBorderMode(0);
  c2->SetBorderSize(2);
  c2->SetGridx();
  c2->SetGridy();
  c2->SetLeftMargin(0.08);
  c2->SetRightMargin(0.05);
  c2->SetFrameBorderMode(0);
  c2->SetFrameBorderMode(0);

  cadre->GetXaxis()->SetTitle("Tracking particle p_{T} (in GeV/c)");
  cadre->GetXaxis()->SetNdivisions(522);
  cadre->GetXaxis()->SetLabelFont(42);
  cadre->GetXaxis()->SetLabelSize(0.03);
  cadre->GetXaxis()->SetTitleFont(42);
  cadre->GetYaxis()->SetTitle("Efficiency");
  cadre->GetYaxis()->SetNdivisions(511);
  cadre->GetYaxis()->SetLabelFont(42);
  cadre->GetYaxis()->SetLabelSize(0.03);
  cadre->GetYaxis()->SetTitleOffset(0.8);
  cadre->GetYaxis()->SetTitleFont(42);
  cadre->Draw();
  
  digi_eff->SetMarkerStyle(3);
  coff_eff->SetMarkerStyle(4);
  soff_eff->SetMarkerStyle(20);
  digi_eff->Draw("same");
  coff_eff->Draw("same");
  soff_eff->Draw("same");

  leg = new TLegend(0.7,0.28,0.85,0.4);

  leg->SetTextSize(0.03);
  leg->SetFillColor(5);
  leg->AddEntry(digi_eff,"Digis","p");
  leg->AddEntry(coff_eff,"Clusters","p");
  leg->AddEntry(soff_eff,"Stubs","p");
  leg->Draw();
    
  TPaveText *pt = new TPaveText(4.3,1.05,15.2,1.11,"br");

  char buffer[80];

  if (disk==0)
    sprintf(buffer, "Turn-on curve for tracker barrel layer %d (official stubs)",layer+5);

  if (disk>0)
    sprintf(buffer, "Turn-on curve for tracker endcap disk %d (official stubs)",disk);

  if (disk<0)
    sprintf(buffer, "Turn-on curve for tracker endcap disk %d (official stubs)",disk);

  ci = TColor::GetColor("#ccccff");
  pt->SetFillColor(ci);
  pt->SetTextSize(0.02798053);
  TText *text = pt->AddText(buffer);
  pt->Draw();
  c2->Modified();
  c2->Update();

  if (per_lay==1)
  {
    if (disk==0) sprintf (buffer, "Barrel_%d_layer_official_eff.png", layer+5);
    if (disk>0)  sprintf (buffer, "Endcap_p%d_layer_official_eff.png", disk); 
    if (disk<0)  sprintf (buffer, "Endcap_m%d_layer_official_eff.png", abs(disk)); 
  }
  else
  {
    if (disk==0) sprintf (buffer, "Barrel_%d_module_official_eff.png", layer+5);
    if (disk>0)  sprintf (buffer, "Endcap_p%d_module_official_eff.png", disk); 
    if (disk<0)  sprintf (buffer, "Endcap_m%d_module_official_eff.png", abs(disk)); 
  }

  c2->Print(buffer);


  c3 = new TCanvas("c3","Compare effs",5,75,1470,860);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  c3->Range(-1.833856,-0.1286626,21.1442,1.157964);
  c3->SetFillColor(0);
  c3->SetBorderMode(0);
  c3->SetBorderSize(2);
  c3->SetGridx();
  c3->SetGridy();
  c3->SetLeftMargin(0.08);
  c3->SetRightMargin(0.05);
  c3->SetFrameBorderMode(0);
  c3->SetFrameBorderMode(0);

  cadre->GetXaxis()->SetTitle("Tracking particle p_{T} (in GeV/c)");
  cadre->GetXaxis()->SetNdivisions(522);
  cadre->GetXaxis()->SetLabelFont(42);
  cadre->GetXaxis()->SetLabelSize(0.03);
  cadre->GetXaxis()->SetTitleFont(42);
  cadre->GetYaxis()->SetTitle("Efficiency");
  cadre->GetYaxis()->SetNdivisions(511);
  cadre->GetYaxis()->SetLabelFont(42);
  cadre->GetYaxis()->SetLabelSize(0.03);
  cadre->GetYaxis()->SetTitleOffset(0.8);
  cadre->GetYaxis()->SetTitleFont(42);
  cadre->Draw();
  
  spri_eff->SetMarkerStyle(3);
  soff_eff->SetMarkerStyle(4);
  spri_eff->SetMarkerSize(1.5);
  soff_eff->SetMarkerSize(1.5);
  spri_eff->Draw("same");
  soff_eff->Draw("same");

  leg = new TLegend(0.7,0.28,0.85,0.4);

  leg->SetTextSize(0.03);
  leg->SetFillColor(5);
  leg->AddEntry(spri_eff,"Stubs (private)","p");
  leg->AddEntry(soff_eff,"Stubs (official)","p");
  leg->Draw();
    
  TPaveText *pt = new TPaveText(4.3,1.05,15.2,1.11,"br");

  char buffer[80];

  if (disk==0)
    sprintf(buffer, "Turn-on curve for tracker barrel layer %d stubs",layer+5);

  if (disk>0)
    sprintf(buffer, "Turn-on curve for tracker endcap disk %d stubs)",disk);

  if (disk<0)
    sprintf(buffer, "Turn-on curve for tracker endcap disk %d stubs",disk);

  ci = TColor::GetColor("#ccccff");
  pt->SetFillColor(ci);
  pt->SetTextSize(0.02798053);
  TText *text = pt->AddText(buffer);
  pt->Draw();
  c3->Modified();
  c3->Update();


  if (per_lay==1)
  {
    if (disk==0) sprintf (buffer, "Barrel_%d_layer_stub_eff.png", layer+5);
    if (disk>0)  sprintf (buffer, "Endcap_p%d_layer_stub_eff.png", disk); 
    if (disk<0)  sprintf (buffer, "Endcap_m%d_layer_stub_eff.png", abs(disk)); 
  }
  else
  {
    if (disk==0) sprintf (buffer, "Barrel_%d_module_stub_eff.png", layer+5);
    if (disk>0)  sprintf (buffer, "Endcap_p%d_module_stub_eff.png", disk); 
    if (disk<0)  sprintf (buffer, "Endcap_m%d_module_stub_eff.png", abs(disk)); 
  }


  c3->Print(buffer);
}


void do_pt_barrel_summary(std::string filename, int per_lay)
{
  // First get the data
  // by merging all the available files

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
 
  float pt_val[100];

  float stub_off_pt[20][100];
  float stub_pri_pt[20][100];


  TFile *file    = TFile::Open(filename.c_str());
  TTree *newtree = (TTree*)file->Get("Efficiencies");

  newtree->SetBranchAddress("pt_val",&pt_val);
  newtree->SetBranchAddress("stub_off_pt",&stub_off_pt);

  if (per_lay==1)
  {
    newtree->SetBranchAddress("stub_pri_pt_lay",&stub_pri_pt);
  }
  else
  {
    newtree->SetBranchAddress("stub_pri_pt",&stub_pri_pt);
  }

  newtree->GetEntry(0);

  TH2F *cadre    = new TH2F("zz","zz",300,0.,20.,100,0.,1.02);
  TH2F *L1_off   = new TH2F("L1off","L1off",300,0.,20.,100,0.,1.02);
  TH2F *L2_off   = new TH2F("L2off","L2off",300,0.,20.,100,0.,1.02);
  TH2F *L3_off   = new TH2F("L3off","L3off",300,0.,20.,100,0.,1.02);
  TH2F *L4_off   = new TH2F("L4off","L4off",300,0.,20.,100,0.,1.02);
  TH2F *L5_off   = new TH2F("L5off","L5off",300,0.,20.,100,0.,1.02);
  TH2F *L6_off   = new TH2F("L6off","L6off",300,0.,20.,100,0.,1.02);
  TH2F *L1_pri   = new TH2F("L1pri","L1pri",300,0.,20.,100,0.,1.02);
  TH2F *L2_pri   = new TH2F("L2pri","L2pri",300,0.,20.,100,0.,1.02);
  TH2F *L3_pri   = new TH2F("L3pri","L3pri",300,0.,20.,100,0.,1.02);
  TH2F *L4_pri   = new TH2F("L4pri","L4pri",300,0.,20.,100,0.,1.02);
  TH2F *L5_pri   = new TH2F("L5pri","L5pri",300,0.,20.,100,0.,1.02);
  TH2F *L6_pri   = new TH2F("L6pri","L6pri",300,0.,20.,100,0.,1.02);



  for (int i=0;i<100;++i)
  { 
    L1_off->Fill(pt_val[i],stub_off_pt[0][i]);
    L2_off->Fill(pt_val[i],stub_off_pt[1][i]);
    L3_off->Fill(pt_val[i],stub_off_pt[2][i]);
    L4_off->Fill(pt_val[i],stub_off_pt[3][i]);
    L5_off->Fill(pt_val[i],stub_off_pt[4][i]);
    L6_off->Fill(pt_val[i],stub_off_pt[5][i]);

    L1_pri->Fill(pt_val[i],stub_pri_pt[0][i]);
    L2_pri->Fill(pt_val[i],stub_pri_pt[1][i]);
    L3_pri->Fill(pt_val[i],stub_pri_pt[2][i]);
    L4_pri->Fill(pt_val[i],stub_pri_pt[3][i]);
    L5_pri->Fill(pt_val[i],stub_pri_pt[4][i]);
    L6_pri->Fill(pt_val[i],stub_pri_pt[5][i]);
  }

  c1 = new TCanvas("c1","Private effs",5,75,870,860);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  c1->Range(-1.833856,-0.1286626,21.1442,1.157964);
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetBorderSize(2);
  c1->SetGridx();
  c1->SetGridy();
  c1->SetLeftMargin(0.08);
  c1->SetRightMargin(0.05);
  c1->SetFrameBorderMode(0);
  c1->SetFrameBorderMode(0);

  cadre->GetXaxis()->SetTitle("Tracking particle p_{T} (in GeV/c)");
  cadre->GetXaxis()->SetNdivisions(522);
  cadre->GetXaxis()->SetLabelFont(42);
  cadre->GetXaxis()->SetLabelSize(0.03);
  cadre->GetXaxis()->SetTitleFont(42);
  cadre->GetYaxis()->SetTitle("Efficiency");
  cadre->GetYaxis()->SetNdivisions(511);
  cadre->GetYaxis()->SetLabelFont(42);
  cadre->GetYaxis()->SetLabelSize(0.03);
  cadre->GetYaxis()->SetTitleOffset(1.);
  cadre->GetYaxis()->SetTitleFont(42);
  cadre->Draw();
  
  L1_pri->SetMarkerStyle(20);
  L2_pri->SetMarkerStyle(20);
  L3_pri->SetMarkerStyle(20);
  L4_pri->SetMarkerStyle(20);
  L5_pri->SetMarkerStyle(20);
  L6_pri->SetMarkerStyle(20);
  
  L1_pri->SetMarkerColor(9);
  L2_pri->SetMarkerColor(4);
  L3_pri->SetMarkerColor(7);
  L4_pri->SetMarkerColor(3);
  L5_pri->SetMarkerColor(5);
  L6_pri->SetMarkerColor(2);

  L1_pri->SetMarkerSize(1.4);
  L2_pri->SetMarkerSize(1.4);
  L3_pri->SetMarkerSize(1.4);
  L4_pri->SetMarkerSize(1.4);
  L5_pri->SetMarkerSize(1.4);
  L6_pri->SetMarkerSize(1.4);

  L1_pri->Draw("same");
  L2_pri->Draw("same");
  L3_pri->Draw("same");
  L4_pri->Draw("same");
  L5_pri->Draw("same");
  L6_pri->Draw("same");


  leg = new TLegend(0.6,0.2,0.85,0.4);

  leg->SetTextSize(0.03);
  leg->SetFillColor(0);
  leg->AddEntry(L1_pri,"Layer 1","p");
  leg->AddEntry(L2_pri,"Layer 2","p");
  leg->AddEntry(L3_pri,"Layer 3","p");
  leg->AddEntry(L4_pri,"Layer 4","p");
  leg->AddEntry(L5_pri,"Layer 5","p");
  leg->AddEntry(L6_pri,"Layer 6","p");
  leg->Draw();
    
  TPaveText *pt = new TPaveText(1.3,1.05,18.7,1.11,"br");

  char buffer[80];

  sprintf(buffer, "Turn-on curve for tracker barrel layers");

  ci = TColor::GetColor("#ccccff");
  pt->SetFillColor(ci);
  pt->SetTextSize(0.02798053);
  TText *text = pt->AddText(buffer);
  pt->Draw();
  c1->Modified();
  c1->Update();

  if (per_lay==1)
  {
    sprintf (buffer, "Barrel_layer_private_eff.png");
    c1->Print(buffer);
    
    sprintf (buffer, "Barrel_layer_private_eff.eps");
    c1->Print(buffer);
  }
  else
  {
    sprintf (buffer, "Barrel_module_private_eff.png");
    c1->Print(buffer);
    
    sprintf (buffer, "Barrel_module_private_eff.eps");
    c1->Print(buffer);
  }
}

void do_pt_endcap_summary(std::string filename, int per_lay)
{
  // First get the data
  // by merging all the available files

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
 
  float pt_val[100];

  float stub_off_pt[20][100];
  float stub_pri_pt[20][100];


  TFile *file    = TFile::Open(filename.c_str());
  TTree *newtree = (TTree*)file->Get("Efficiencies");

  newtree->SetBranchAddress("pt_val",&pt_val);
  newtree->SetBranchAddress("stub_off_pt",&stub_off_pt);

  if (per_lay==1)
  {
    newtree->SetBranchAddress("stub_pri_pt_lay",&stub_pri_pt);
  }
  else
  {
    newtree->SetBranchAddress("stub_pri_pt",&stub_pri_pt);
  }

  newtree->GetEntry(0);

  TH2F *cadre    = new TH2F("zz","zz",300,0.,20.,100,0.,1.02);
  TH2F *D1_off   = new TH2F("L1off","L1off",300,0.,20.,100,0.,1.02);
  TH2F *D2_off   = new TH2F("L2off","L2off",300,0.,20.,100,0.,1.02);
  TH2F *D3_off   = new TH2F("L3off","L3off",300,0.,20.,100,0.,1.02);
  TH2F *D4_off   = new TH2F("L4off","L4off",300,0.,20.,100,0.,1.02);
  TH2F *D5_off   = new TH2F("L5off","L5off",300,0.,20.,100,0.,1.02);
  TH2F *D1_pri   = new TH2F("L1pri","L1pri",300,0.,20.,100,0.,1.02);
  TH2F *D2_pri   = new TH2F("L2pri","L2pri",300,0.,20.,100,0.,1.02);
  TH2F *D3_pri   = new TH2F("L3pri","L3pri",300,0.,20.,100,0.,1.02);
  TH2F *D4_pri   = new TH2F("L4pri","L4pri",300,0.,20.,100,0.,1.02);
  TH2F *D5_pri   = new TH2F("L5pri","L5pri",300,0.,20.,100,0.,1.02);

  for (int i=0;i<100;++i)
  { 
    D1_off->Fill(pt_val[i],stub_off_pt[6][i]);
    D2_off->Fill(pt_val[i],stub_off_pt[7][i]);
    D3_off->Fill(pt_val[i],stub_off_pt[8][i]);
    D4_off->Fill(pt_val[i],stub_off_pt[9][i]);
    D5_off->Fill(pt_val[i],stub_off_pt[10][i]);  
 
    D1_pri->Fill(pt_val[i],stub_pri_pt[6][i]);
    D2_pri->Fill(pt_val[i],stub_pri_pt[7][i]);
    D3_pri->Fill(pt_val[i],stub_pri_pt[8][i]);
    D4_pri->Fill(pt_val[i],stub_pri_pt[9][i]);
    D5_pri->Fill(pt_val[i],stub_pri_pt[10][i]);  
  }

  c1 = new TCanvas("c1","Private effs",5,75,870,860);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  c1->Range(-1.833856,-0.1286626,21.1442,1.157964);
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetBorderSize(2);
  c1->SetGridx();
  c1->SetGridy();
  c1->SetLeftMargin(0.08);
  c1->SetRightMargin(0.05);
  c1->SetFrameBorderMode(0);
  c1->SetFrameBorderMode(0);

  cadre->GetXaxis()->SetTitle("Tracking particle p_{T} (in GeV/c)");
  cadre->GetXaxis()->SetNdivisions(522);
  cadre->GetXaxis()->SetLabelFont(42);
  cadre->GetXaxis()->SetLabelSize(0.03);
  cadre->GetXaxis()->SetTitleFont(42);
  cadre->GetYaxis()->SetTitle("Efficiency");
  cadre->GetYaxis()->SetNdivisions(511);
  cadre->GetYaxis()->SetLabelFont(42);
  cadre->GetYaxis()->SetLabelSize(0.03);
  cadre->GetYaxis()->SetTitleOffset(1.);
  cadre->GetYaxis()->SetTitleFont(42);
  cadre->Draw();
  
  D1_pri->SetMarkerStyle(20);
  D2_pri->SetMarkerStyle(20);
  D3_pri->SetMarkerStyle(20);
  D4_pri->SetMarkerStyle(20);
  D5_pri->SetMarkerStyle(20);
  
  D1_pri->SetMarkerColor(9);
  D2_pri->SetMarkerColor(4);
  D3_pri->SetMarkerColor(7);
  D4_pri->SetMarkerColor(3);
  D5_pri->SetMarkerColor(5);

  D1_pri->SetMarkerSize(1.4);
  D2_pri->SetMarkerSize(1.4);
  D3_pri->SetMarkerSize(1.4);
  D4_pri->SetMarkerSize(1.4);
  D5_pri->SetMarkerSize(1.4);

  D1_pri->Draw("same");
  D2_pri->Draw("same");
  D3_pri->Draw("same");
  D4_pri->Draw("same");
  D5_pri->Draw("same");


  leg = new TLegend(0.6,0.2,0.85,0.4);

  leg->SetTextSize(0.03);
  leg->SetFillColor(0);
  leg->AddEntry(D1_pri,"Disk 1","p");
  leg->AddEntry(D2_pri,"Disk 2","p");
  leg->AddEntry(D3_pri,"Disk 3","p");
  leg->AddEntry(D4_pri,"Disk 4","p");
  leg->AddEntry(D5_pri,"Disk 5","p");
  leg->Draw();
    
  TPaveText *pt = new TPaveText(1.3,1.05,18.7,1.11,"br");

  char buffer[80];

  sprintf(buffer, "Turn-on curve for tracker endcap layers");

  ci = TColor::GetColor("#ccccff");
  pt->SetFillColor(ci);
  pt->SetTextSize(0.02798053);
  TText *text = pt->AddText(buffer);
  pt->Draw();
  c1->Modified();
  c1->Update();


  if (per_lay==1)
  {
    sprintf (buffer, "Endcap_layer_private_eff.png");
    c1->Print(buffer);
    
    sprintf (buffer, "Endcap_layer_private_eff.eps");
    c1->Print(buffer);
  }
  else
  {
    sprintf (buffer, "Endcap_module_private_eff.png");
    c1->Print(buffer);
    
    sprintf (buffer, "Endcap_module_private_eff.eps");
    c1->Print(buffer);
  }

}

void do_eta_plots(std::string filename,int layer,int per_lay)
{
  // First get the data
  // by merging all the available files

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
 
  float eta_val[50];

  float digi_eta[20][50];

  float clus_off_eta[20][50];
  float stub_off_eta[20][50];

  float clus_pri_eta[20][50];
  float stub_pri_eta[20][50];

  int disk =0;

  if (layer>=6  && layer<13) disk=int(layer-5)%8;
  if (layer>=13 && layer<20) disk=-int(layer-12)%8;

  TFile *file    = TFile::Open(filename.c_str());
  TTree *newtree = (TTree*)file->Get("Efficiencies");

  newtree->SetBranchAddress("eta_val",&eta_val);
  newtree->SetBranchAddress("digi_eta",&digi_eta);
  newtree->SetBranchAddress("clus_off_eta",&clus_off_eta);
  newtree->SetBranchAddress("stub_off_eta",&stub_off_eta);
  newtree->SetBranchAddress("clus_pri_eta",&clus_pri_eta);

  if (per_lay==1)
  {
    newtree->SetBranchAddress("stub_pri_eta_lay",&stub_pri_eta);
  }
  else
  {
    newtree->SetBranchAddress("stub_pri_eta",&stub_pri_eta);
  }


  newtree->GetEntry(0);

  TH2F *cadre    = new TH2F("zz","zz",300,-2.5,2.5,100,0.,1.02);
  TH2F *digi_eff = new TH2F("deff","deff",300,-2.5,2.5,100,0.,1.02);
  TH2F *coff_eff = new TH2F("coeff","coeff",300,-2.5,2.5,100,0.,1.02);
  TH2F *cpri_eff = new TH2F("cpeff","cpeff",300,-2.5,2.5,100,0.,1.02);
  TH2F *soff_eff = new TH2F("soeff","soeff",300,-2.5,2.5,100,0.,1.02);
  TH2F *spri_eff = new TH2F("speff","speff",300,-2.5,2.5,100,0.,1.02);

  for (int i=0;i<50;++i)
  { 
    if (digi_eta[layer][i]!=0.)     digi_eff->Fill(eta_val[i],digi_eta[layer][i]);
    if (clus_off_eta[layer][i]!=0.) coff_eff->Fill(eta_val[i],clus_off_eta[layer][i]);
    if (clus_pri_eta[layer][i]!=0.) cpri_eff->Fill(eta_val[i],clus_pri_eta[layer][i]);
    if (stub_off_eta[layer][i]!=0.) soff_eff->Fill(eta_val[i],stub_off_eta[layer][i]);
    if (stub_pri_eta[layer][i]!=0.) spri_eff->Fill(eta_val[i],stub_pri_eta[layer][i]);

    if (disk>0)
    {
      if (digi_eta[layer+7][i]!=0.)     digi_eff->Fill(eta_val[i],digi_eta[layer+7][i]);
      if (clus_off_eta[layer+7][i]!=0.) coff_eff->Fill(eta_val[i],clus_off_eta[layer+7][i]);
      if (clus_pri_eta[layer+7][i]!=0.) cpri_eff->Fill(eta_val[i],clus_pri_eta[layer+7][i]);
      if (stub_off_eta[layer+7][i]!=0.) soff_eff->Fill(eta_val[i],stub_off_eta[layer+7][i]);
      if (stub_pri_eta[layer+7][i]!=0.) spri_eff->Fill(eta_val[i],stub_pri_eta[layer+7][i]);
    }
  }

  c1 = new TCanvas("c1","Private effs",5,75,1470,860);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  c1->Range(-1.833856,-0.1286626,21.1442,1.157964);
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetBorderSize(2);
  c1->SetGridx();
  c1->SetGridy();
  c1->SetLeftMargin(0.08);
  c1->SetRightMargin(0.05);
  c1->SetFrameBorderMode(0);
  c1->SetFrameBorderMode(0);

  cadre->GetXaxis()->SetTitle("Tracking particle #eta");
  cadre->GetXaxis()->SetNdivisions(522);
  cadre->GetXaxis()->SetLabelFont(42);
  cadre->GetXaxis()->SetLabelSize(0.03);
  cadre->GetXaxis()->SetTitleFont(42);
  cadre->GetYaxis()->SetTitle("Efficiency");
  cadre->GetYaxis()->SetNdivisions(511);
  cadre->GetYaxis()->SetLabelFont(42);
  cadre->GetYaxis()->SetLabelSize(0.03);
  cadre->GetYaxis()->SetTitleOffset(0.8);
  cadre->GetYaxis()->SetTitleFont(42);
  cadre->Draw();
  
  digi_eff->SetMarkerStyle(3);
  cpri_eff->SetMarkerStyle(4);
  spri_eff->SetMarkerStyle(20);
  digi_eff->Draw("same");
  cpri_eff->Draw("same");
  spri_eff->Draw("same");

  leg = new TLegend(0.7,0.28,0.85,0.4);

  leg->SetTextSize(0.03);
  leg->SetFillColor(5);
  leg->AddEntry(digi_eff,"Digis","p");
  leg->AddEntry(cpri_eff,"Clusters","p");
  leg->AddEntry(spri_eff,"Stubs","p");
  leg->Draw();
    
  TPaveText *pt = new TPaveText(-1.2,1.05,1.2,1.11,"br");

  char buffer[80];

  if (disk==0)
    sprintf(buffer, "Turn-on curve for tracker barrel layer %d (private stubs)",layer+5);

  if (disk>0)
    sprintf(buffer, "Turn-on curve for tracker endcap disks %d (private stubs)",disk);

  if (disk<0)
    sprintf(buffer, "Turn-on curve for tracker endcap disks %d (private stubs)",disk);

  ci = TColor::GetColor("#ccccff");
  pt->SetFillColor(ci);
  pt->SetTextSize(0.02798053);
  TText *text = pt->AddText(buffer);
  pt->Draw();
  c1->Modified();
  c1->Update();


  if (per_lay==1)
  {
    if (disk==0) sprintf (buffer, "Barrel_%d_layer_private_eta.png", layer+5);
    if (disk>0)  sprintf (buffer, "Endcap_%d_layer_private_eta.png", disk); 
    if (disk<0)  sprintf (buffer, "Endcap_%d_layer_private_eta.png", abs(disk)); 
  }
  else
  {
    if (disk==0) sprintf (buffer, "Barrel_%d_module_private_eta.png", layer+5);
    if (disk>0)  sprintf (buffer, "Endcap_%d_module_private_eta.png", disk); 
    if (disk<0)  sprintf (buffer, "Endcap_%d_module_private_eta.png", abs(disk)); 
  }

  c1->Print(buffer);


  c2 = new TCanvas("c2","Official effs",5,75,1470,860);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  c2->Range(-1.833856,-0.1286626,21.1442,1.157964);
  c2->SetFillColor(0);
  c2->SetBorderMode(0);
  c2->SetBorderSize(2);
  c2->SetGridx();
  c2->SetGridy();
  c2->SetLeftMargin(0.08);
  c2->SetRightMargin(0.05);
  c2->SetFrameBorderMode(0);
  c2->SetFrameBorderMode(0);

  cadre->GetXaxis()->SetTitle("Tracking particle #eta");
  cadre->GetXaxis()->SetNdivisions(522);
  cadre->GetXaxis()->SetLabelFont(42);
  cadre->GetXaxis()->SetLabelSize(0.03);
  cadre->GetXaxis()->SetTitleFont(42);
  cadre->GetYaxis()->SetTitle("Efficiency");
  cadre->GetYaxis()->SetNdivisions(511);
  cadre->GetYaxis()->SetLabelFont(42);
  cadre->GetYaxis()->SetLabelSize(0.03);
  cadre->GetYaxis()->SetTitleOffset(0.8);
  cadre->GetYaxis()->SetTitleFont(42);
  cadre->Draw();
  
  digi_eff->SetMarkerStyle(3);
  coff_eff->SetMarkerStyle(4);
  soff_eff->SetMarkerStyle(20);
  digi_eff->Draw("same");
  coff_eff->Draw("same");
  soff_eff->Draw("same");

  leg = new TLegend(0.7,0.28,0.85,0.4);

  leg->SetTextSize(0.03);
  leg->SetFillColor(5);
  leg->AddEntry(digi_eff,"Digis","p");
  leg->AddEntry(coff_eff,"Clusters","p");
  leg->AddEntry(soff_eff,"Stubs","p");
  leg->Draw();
    
  TPaveText *pt = new TPaveText(-1.2,1.05,1.2,1.11,"br");

  char buffer[80];

  if (disk==0)
    sprintf(buffer, "Turn-on curve for tracker barrel layer %d (official stubs)",layer+5);

  if (disk>0)
    sprintf(buffer, "Turn-on curve for tracker endcap disks %d (official stubs)",disk);

  if (disk<0)
    sprintf(buffer, "Turn-on curve for tracker endcap disks %d (official stubs)",disk);

  ci = TColor::GetColor("#ccccff");
  pt->SetFillColor(ci);
  pt->SetTextSize(0.02798053);
  TText *text = pt->AddText(buffer);
  pt->Draw();
  c2->Modified();
  c2->Update();


  if (per_lay==1)
  {
    if (disk==0) sprintf (buffer, "Barrel_%d_layer_official_eta.png", layer+5);
    if (disk>0)  sprintf (buffer, "Endcap_%d_layer_official_eta.png", disk); 
    if (disk<0)  sprintf (buffer, "Endcap_%d_layer_official_eta.png", abs(disk)); 
  }
  else
  {
    if (disk==0) sprintf (buffer, "Barrel_%d_module_official_eta.png", layer+5);
    if (disk>0)  sprintf (buffer, "Endcap_%d_module_official_eta.png", disk); 
    if (disk<0)  sprintf (buffer, "Endcap_%d_module_official_eta.png", abs(disk)); 
  }

  c2->Print(buffer);


  c3 = new TCanvas("c3","Compare effs",5,75,1470,860);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  c3->Range(-1.833856,-0.1286626,21.1442,1.157964);
  c3->SetFillColor(0);
  c3->SetBorderMode(0);
  c3->SetBorderSize(2);
  c3->SetGridx();
  c3->SetGridy();
  c3->SetLeftMargin(0.08);
  c3->SetRightMargin(0.05);
  c3->SetFrameBorderMode(0);
  c3->SetFrameBorderMode(0);

  cadre->GetXaxis()->SetTitle("Tracking particle #eta");
  cadre->GetXaxis()->SetNdivisions(522);
  cadre->GetXaxis()->SetLabelFont(42);
  cadre->GetXaxis()->SetLabelSize(0.03);
  cadre->GetXaxis()->SetTitleFont(42);
  cadre->GetYaxis()->SetTitle("Efficiency");
  cadre->GetYaxis()->SetNdivisions(511);
  cadre->GetYaxis()->SetLabelFont(42);
  cadre->GetYaxis()->SetLabelSize(0.03);
  cadre->GetYaxis()->SetTitleOffset(0.8);
  cadre->GetYaxis()->SetTitleFont(42);
  cadre->Draw();
  
  spri_eff->SetMarkerStyle(3);
  soff_eff->SetMarkerStyle(4);
  spri_eff->SetMarkerSize(1.5);
  soff_eff->SetMarkerSize(1.5);
  spri_eff->Draw("same");
  soff_eff->Draw("same");

  leg = new TLegend(0.7,0.28,0.85,0.4);

  leg->SetTextSize(0.03);
  leg->SetFillColor(5);
  leg->AddEntry(spri_eff,"Stubs (private)","p");
  leg->AddEntry(soff_eff,"Stubs (official)","p");
  leg->Draw();
    
  TPaveText *pt = new TPaveText(-1.2,1.05,1.2,1.11,"br");

  char buffer[80];

  if (disk==0)
    sprintf(buffer, "Turn-on curve for tracker barrel layer %d stubs",layer+5);

  if (disk>0)
    sprintf(buffer, "Turn-on curve for tracker endcap disks %d stubs)",disk);

  if (disk<0)
    sprintf(buffer, "Turn-on curve for tracker endcap disks %d stubs",disk);

  ci = TColor::GetColor("#ccccff");
  pt->SetFillColor(ci);
  pt->SetTextSize(0.02798053);
  TText *text = pt->AddText(buffer);
  pt->Draw();
  c3->Modified();
  c3->Update();


  if (per_lay==1)
  {
    if (disk==0) sprintf (buffer, "Barrel_%d_layer_stub_eta.png", layer+5);
    if (disk>0)  sprintf (buffer, "Endcap_%d_layer_stub_eta.png", disk); 
    if (disk<0)  sprintf (buffer, "Endcap_%d_layer_stub_eta.png", abs(disk)); 
  }
  else
  {
    if (disk==0) sprintf (buffer, "Barrel_%d_module_stub_eta.png", layer+5);
    if (disk>0)  sprintf (buffer, "Endcap_%d_module_stub_eta.png", disk); 
    if (disk<0)  sprintf (buffer, "Endcap_%d_module_stub_eta.png", abs(disk)); 
  }

  c3->Print(buffer);
}


void do_sector_plots(std::string filename, int thresh)
{
  // First get the data
  // by merging all the available files

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
 
  int mult[500];
  float sec_rates[500];
  float sec_rms[500];

  TFile *file    = TFile::Open(filename.c_str());
  TTree *newtree = (TTree*)file->Get("Efficiencies");

  newtree->SetBranchAddress("mult",&mult);

  int n_entries = newtree->GetEntries();

  TH1F *sec_rate_e    = new TH1F("e","e",200,100.,300.);
  TH1F *sec_rate_b    = new TH1F("b","b",200,100.,300.);
  TH1F *sec_rate_be   = new TH1F("be","be",200,100.,300.);
 
  for (int i=0;i<500;++i) sec_rates[i]=0.;
  for (int i=0;i<500;++i) sec_rms[i]=0.;

  for (int i=0;i<n_entries;++i)
  {
    newtree->GetEntry(i);

    for (int j=0;j<500;++j)
    {
      if (mult[j]!=0) sec_rates[j]+=mult[j];
    }
  }

  for (int j=0;j<500;++j)
  { 
    if (sec_rates[j]!=0.) sec_rates[j] = sec_rates[j]/n_entries;
  }

  for (int i=0;i<n_entries;++i)
  {
    newtree->GetEntry(i);

    for (int j=0;j<500;++j)
    {
      if (mult[j]!=0) sec_rms[j]+=(mult[j]-sec_rates[j])*(mult[j]-sec_rates[j]);
    }
  }

  for (int j=0;j<500;++j)
  { 
    if (sec_rms[j]!=0.) sec_rms[j] = sqrt(sec_rms[j]/n_entries);
  }

  for (int j=0;j<500;++j)
  { 
    if (sec_rates[j]!=0.)
    {
      if (j/8==0 || j/8==6) sec_rate_e->Fill(sec_rates[j]);
      if (j/8==1 || j/8==5) sec_rate_be->Fill(sec_rates[j]);
      if (j/8>=2 && j/8<=4) sec_rate_b->Fill(sec_rates[j]); 
    }
  }

  for (int j=0;j<8;++j)
  {
    cout << j << "   ";
    cout << std::fixed;
    for (int i=0;i<7;++i)
    {
      cout << std::setprecision(0) << sec_rates[8*i+j] << " +/- " << sec_rms[8*i+j] << " / ";
    }

    cout << endl;
  }


  c1 = new TCanvas("c1","Private effs",5,75,1470,860);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  c1->Range(-1.833856,-0.1286626,21.1442,1.157964);
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetBorderSize(2);
  c1->SetGridx();
  c1->SetGridy();
  c1->SetLeftMargin(0.08);
  c1->SetRightMargin(0.05);
  c1->SetFrameBorderMode(0);
  c1->SetFrameBorderMode(0);

  sec_rate_e->GetXaxis()->SetTitle("Avg. #stubs/sector/BX");
  sec_rate_e->GetXaxis()->SetNdivisions(522);
  sec_rate_e->GetXaxis()->SetLabelFont(42);
  sec_rate_e->GetXaxis()->SetLabelSize(0.03);
  sec_rate_e->GetXaxis()->SetTitleFont(42);
  sec_rate_e->GetYaxis()->SetTitle("Number of entries");
  sec_rate_e->GetYaxis()->SetNdivisions(511);
  sec_rate_e->GetYaxis()->SetLabelFont(42);
  sec_rate_e->GetYaxis()->SetLabelSize(0.03);
  sec_rate_e->GetYaxis()->SetTitleOffset(0.8);
  sec_rate_e->GetYaxis()->SetTitleFont(42);
  sec_rate_e->SetFillColor(2);
  sec_rate_e->Draw();
  sec_rate_b->SetFillColor(1);
  sec_rate_b->Draw("same");
  sec_rate_be->SetFillColor(4);
  sec_rate_be->Draw("same"); 


  
  leg = new TLegend(0.75,0.78,0.95,0.9);

  leg->SetTextSize(0.03);
  leg->SetFillColor(5);
  leg->AddEntry(sec_rate_e,"Endcap sectors","f");
  leg->AddEntry(sec_rate_b,"Barrel sectors","f");
  leg->AddEntry(sec_rate_be,"Hybrid sectors","f");
  leg->Draw();

  char buffer[80];

  c1->Modified();
  c1->Update();

  sprintf (buffer, "Sector_Mult_Threshold_%d.png", thresh);

  c1->Print(buffer);
  

}


//
// Method plotting sector and stub efficiencies measured 
// by the method sector_test (see HL_LHC tuto part 3.2.5 for infos)
//
// filename is the name of the root file containing the results
// nh is the minimum number of layer/disks containing primary stubs 
// from the primary particle 
//

void do_sector_effs(std::string filename, int nh)
{
  // First get the data
  // by merging all the available files

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
 
  int   nsec;       // The number of sectors containing at least 5 stubs of the prim. track
  float pt;         // The pT of the prim. track
  float eta;        // The eta of the prim. track
  float phi;        // The phi of the prim. track
  int   mult[500];  // The total number of stubs per sector 
  int   nhits;      // The total number of layers/disks hit by the prim track

  const int neta = 70;
  const int nphi = 80;

  TFile *file    = TFile::Open(filename.c_str());
  TTree *newtree = (TTree*)file->Get("SectorEff");

  newtree->SetBranchAddress("nsec",       &nsec);
  newtree->SetBranchAddress("nhits",      &nhits);
  newtree->SetBranchAddress("pt",         &pt);
  newtree->SetBranchAddress("eta",        &eta);
  newtree->SetBranchAddress("phi",        &phi);
  newtree->SetBranchAddress("mult",       &mult);

  int n_entries = newtree->GetEntries();

  TH2F *eff_hit    = new TH2F("eff_h","eff_h",neta,-2.2,2.2,nphi,-3.15.,3.15);
  TH2F *eff_sec    = new TH2F("eff_s","eff_s",neta,-2.2,2.2,nphi,-3.15.,3.15);
  TH2F *cov_sec    = new TH2F("cov_s","cov_s",neta,-2.2,2.2,nphi,-3.15.,3.15);
  TH1F *status     = new TH1F("stat","stat",3,0.,3.);

  float eff_stub[nphi][neta];
  float eff_sector[nphi][neta];
  float entries_stub[nphi][neta];

  float eff_pat_m[nphi][neta];
  float eff_sec_m[nphi][neta];

  float entries_stub[nphi][neta];

  for (int i=0;i<nphi;++i)
  {
    for (int j=0;j<neta;++j)
    {
      eff_stub[i][j]     = 0.;
      eff_sector[i][j]   = 0.;
      entries_stub[i][j] = 0.;
      eff_sec_m[i][j]    = 0.;
    }
  }

  int i_bin;
  int j_bin;

  float PI=4.*atan(1.);


  int msec;

  float ntot_1 = 0.;
  float ntot_2 = 0.;
  float ntot_3 = 0.;

  // First loop to get the sector eta/phi acceptance

  for (int i=0;i<n_entries;++i)
    //for (int i=0;i<1000;++i)
  {
    newtree->GetEntry(i);

    if (pt<2 || fabs(eta)>2.15) continue;

    ++ntot_1;
    i_bin = static_cast<int>(nphi*(phi+PI)/(2*PI));
    j_bin = static_cast<int>(neta*(eta+2.2)/4.4);


    entries_stub[i_bin][j_bin]+=1.;

    if (nhits<nh) continue;

    eff_stub[i_bin][j_bin]+=1.;
    ++ntot_2;

    if (nsec==0) continue;

    msec=0;

    for (int j=0;j<500;++j)
    {
      if (mult[j]>=nh) ++msec;
    }

    if (msec>0)
    {
      ++ntot_3;
      eff_sector[i_bin][j_bin]+=1.; 
      eff_sec_m[i_bin][j_bin]+=nsec;
    }
  }

  for (int i=0;i<nphi;++i)
  {
    for (int j=0;j<neta;++j)
    {
      if (eff_sector[i][j]!=0.)
	eff_sec_m[i][j] /= eff_sector[i][j];

      if (eff_stub[i][j]!=0.)
	eff_sector[i][j] /= eff_stub[i][j];

      if (entries_stub[i][j]!=0.)
	eff_stub[i][j] /= entries_stub[i][j];

      eff_hit->Fill(-2.2+(j+0.5)*4.4/neta,-PI+(i+0.5)*2*PI/nphi,eff_stub[i][j]);
      eff_sec->Fill(-2.2+(j+0.5)*4.4/neta,-PI+(i+0.5)*2*PI/nphi,eff_sector[i][j]);
      cov_sec->Fill(-2.2+(j+0.5)*4.4/neta,-PI+(i+0.5)*2*PI/nphi,eff_sec_m[i][j]);
    }
  }

  status->Fill(0.5,1.);
  status->Fill(1.5,ntot_2/ntot_1);
  status->Fill(2.5,ntot_3/ntot_1);

  
  char buffer[80];
  
  c1 = new TCanvas("c1","Overall efficiencies",6,102,1526,921);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  c1->Range(-1.833856,-0.1286626,21.1442,1.157964);
  c1->Divide(2,2);
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetBorderSize(2);
  c1->SetLeftMargin(0.08);
  c1->SetRightMargin(0.05);
  c1->SetFrameBorderMode(0);
  c1->SetFrameBorderMode(0);
  
  c1->cd(1);  
  eff_hit->GetXaxis()->SetTitle("#eta");
  eff_hit->GetXaxis()->SetLabelFont(42);
  eff_hit->GetXaxis()->SetLabelSize(0.03);
  eff_hit->GetXaxis()->SetTitleSize(0.035);
  eff_hit->GetXaxis()->SetTitleFont(42);
  eff_hit->GetYaxis()->SetTitle("#phi (in rad)");
  eff_hit->GetYaxis()->SetLabelFont(42);
  eff_hit->GetYaxis()->SetLabelSize(0.03);
  eff_hit->GetYaxis()->SetTitleSize(0.035);
  eff_hit->GetYaxis()->SetTitleFont(42);
  eff_hit->SetContour(99);
  eff_hit->Draw("cont4z");

  TPaveText *pte = new TPaveText(-0.57,0.61,0.58,0.68,"br");
  pte->SetFillColor(0);
  pte->SetTextFont(42);
  pte->SetTextSize(0.0349944);
  sprintf (buffer, "Proportion of prim. tracks w/stubs on more than %d distincts layers/disks ", nh);  
  TText *text = pte->AddText(buffer);
  pte->Draw();
  

  sprintf (buffer, "Proportion of prim. tracks containing at least %d stubs in at least one sector", nh);
  c1->cd(2);
  eff_sec->GetXaxis()->SetTitle("#eta");
  eff_sec->GetXaxis()->SetLabelFont(42);
  eff_sec->GetXaxis()->SetLabelSize(0.03);
  eff_sec->GetXaxis()->SetTitleSize(0.035);
  eff_sec->GetXaxis()->SetTitleFont(42);
  eff_sec->GetYaxis()->SetTitle("#phi (in rad)");
  eff_sec->GetYaxis()->SetLabelFont(42);
  eff_sec->GetYaxis()->SetLabelSize(0.03);
  eff_sec->GetYaxis()->SetTitleSize(0.035);
  eff_sec->GetYaxis()->SetTitleFont(42);
  eff_sec->SetContour(99);
  eff_sec->Draw("cont4z");

  TPaveText *pte2 = new TPaveText(-0.57,0.61,0.58,0.68,"br");
  pte2->SetFillColor(0);
  pte2->SetTextFont(42);
  pte2->SetTextSize(0.0349944);
  sprintf (buffer, "Proportion of prim. tracks containing at least %d stubs in at least one sector", nh);  
  TText *text = pte2->AddText(buffer);
  pte2->Draw();

  c1->cd(3);
  cov_sec->GetXaxis()->SetTitle("#eta");
  cov_sec->GetXaxis()->SetLabelFont(42);
  cov_sec->GetXaxis()->SetLabelSize(0.03);
  cov_sec->GetXaxis()->SetTitleSize(0.035);
  cov_sec->GetXaxis()->SetTitleFont(42);
  cov_sec->GetYaxis()->SetTitle("#phi (in rad)");
  cov_sec->GetYaxis()->SetLabelFont(42);
  cov_sec->GetYaxis()->SetLabelSize(0.03);
  cov_sec->GetYaxis()->SetTitleSize(0.035);
  cov_sec->GetYaxis()->SetTitleFont(42);
  cov_sec->SetContour(99);
  cov_sec->Draw("cont4z");

  TPaveText *pte3 = new TPaveText(-0.57,0.61,0.58,0.68,"br");
  pte3->SetFillColor(0);
  pte3->SetTextFont(42);
  pte3->SetTextSize(0.0349944);
  sprintf (buffer, "Average number of sectors containing the track");  
  TText *text = pte3->AddText(buffer);
  pte3->Draw();

  
  c1->cd(4);
  c1->cd(4)->SetGridx();
  c1->cd(4)->SetGridy();
  status->SetMinimum(0.8);
  status->SetMaximum(1.02);
  status->GetXaxis()->SetNdivisions(4);
  status->GetXaxis()->SetLabelFont(42);
  status->GetXaxis()->SetLabelSize(0);
  status->GetXaxis()->SetTickLength(0.02);
  status->GetYaxis()->SetTitle("Efficiency");
  status->GetYaxis()->SetLabelFont(42);
  status->GetYaxis()->SetLabelSize(0.03);
  status->GetYaxis()->SetTitleSize(0.035);
  status->GetYaxis()->SetTitleFont(42);
  status->Draw();
   
  TPaveText *ptt = new TPaveText(0.07,0.79,0.93,0.795,"br");
  ptt->SetFillColor(0);
  ptt->SetFillStyle(0);
  ptt->SetLineColor(0);
  ptt->SetTextFont(42);
  ptt->SetTextSize(0.035);
  text = ptt->AddText("All tracks");
  ptt->Draw();
  
  TPaveText *ptt = new TPaveText(1.07,0.79,1.93,0.795,"br");
  ptt->SetFillColor(0);
  ptt->SetFillStyle(0);
  ptt->SetLineColor(0);
  ptt->SetTextFont(42);
  ptt->SetTextSize(0.035);
  text = ptt->AddText("Stubs innef");
  ptt->Draw();

  TPaveText *ptt = new TPaveText(2.07,0.79,2.93,0.795,"br");
  ptt->SetFillColor(0);
  ptt->SetFillStyle(0);
  ptt->SetLineColor(0);
  ptt->SetTextFont(42);
  ptt->SetTextSize(0.035);
  text = ptt->AddText("Stubs+sector innef");
  ptt->Draw();

  TPaveText *ptt = new TPaveText(0.2,0.955,0.8,0.965,"br");
  ptt->SetFillColor(0);
  text = ptt->AddText("100%");
  ptt->Draw();
  
  sprintf (buffer, "%.1f%s",100*ntot_2/ntot_1,"%");  
  TPaveText *ptt = new TPaveText(1.2,0.955,1.8,.965,"br");
  ptt->SetFillColor(0);
  text = ptt->AddText(buffer);
  ptt->Draw();

  sprintf (buffer, "%.1f%s",100*ntot_3/ntot_1,"%");  
  TPaveText *ptt = new TPaveText(2.2,0.955,2.8,.965,"br");
  ptt->SetFillColor(0);
  text = ptt->AddText(buffer);
  ptt->Draw();

  c1->Modified();
  c1->Update();

  sprintf (buffer, "Sector_eff_%d_hits_track.png", nh);

  c1->Print(buffer);
  
}



//
// Method plotting pattern efficiencies  
// for a given sector
//
// filename is the name of the root file containing the results
// nh is the minimum number of layer/disks containing primary stubs 
// from the primary particle 
// sec_num is the sector number


void do_pattern_effs(std::string filename, int sec_num, int nh)
{
  // First get the data
  // by merging all the available files

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
 
  int   nsec;       // The number of sectors containing at least 5 stubs of the prim. track
  float pt;         // The pT of the prim. track
  float eta;        // The eta of the prim. track
  float phi;        // The phi of the prim. track
  int   npatt;      // The number of patterns containing at least 5 stubs of the prim. track
  int   ntotpatt;   // The total number of patterns 
  int   mult[500];  // The total number of stubs per sector 
  int   nhits;      // The total number of layers/disks hit by the prim track


  TFile *file    = TFile::Open(filename.c_str());
  TTree *newtree = (TTree*)file->Get("SectorEff");

  newtree->SetBranchAddress("nsec",       &nsec);
  newtree->SetBranchAddress("nhits",      &nhits);
  newtree->SetBranchAddress("npatt",      &npatt);
  newtree->SetBranchAddress("tpatt",      &ntotpatt);
  newtree->SetBranchAddress("pt",         &pt);
  newtree->SetBranchAddress("eta",        &eta);
  newtree->SetBranchAddress("phi",        &phi);
  newtree->SetBranchAddress("mult",       &mult);

  int n_entries = newtree->GetEntries();

  TH2F *tracks     = new TH2F("tracks","tracks",70,-2.2,2.2,80,-3.15.,3.15);
  TH2F *eff_pat    = new TH2F("eff_p","eff_p",70,-2.2,2.2,80,-3.15.,3.15);
  TH1F *pt_eff     = new TH1F("eff_pt","eff_pt",100,0.,100.);
  TH1F *pt_fake    = new TH1F("fak_pt","fak_pt",100,0.,100.);

  float eff_stub[80][70];
  float eff_sector[80][70];
  float entries_stub[80][70];

  float eff_pat_m[80][70];
  float eff_sec_m[80][70];

  float entries_stub[80][70];

  for (int i=0;i<80;++i)
  {
    for (int j=0;j<70;++j)
    {
      eff_stub[i][j]     = 0.;
      eff_sector[i][j]   = 0.;
      entries_stub[i][j] = 0.;
      eff_pat_m[i][j]    = 0.;
      eff_sec_m[i][j]    = 0.;
    }
  }


  int tot_in_sec    = 0;
  int tot_match     = 0;
  int tot_patts     = 0;
  int tot_goodpatts = 0;

  int i_bin;
  int j_bin;

  float PI=4.*atan(1.);

  float pt_in_sec[100];
  float pt_in_pat[100];

  float good_patt[100];
  float tot_patt[100];

  for (int i=0;i<100;++i)
  {
    pt_in_sec[i]=0.;
    pt_in_pat[i]=0.;
    good_patt[i]=0.;
    tot_patt[i]=0.;
  }
  
  int pt_bin;

  // First loop to get the sector eta/phi acceptance

  for (int i=0;i<n_entries;++i)
    //for (int i=0;i<1000;++i)
  {
    newtree->GetEntry(i);
    
    if (fabs(eta)>2.15) continue;
  
  pt_bin=static_cast<int>(pt);

  i_bin = static_cast<int>(80*(phi+PI)/(2*PI));
    j_bin = static_cast<int>(70*(eta+2.2)/4.4);

    if (nhits<nh) continue;
    if (mult[sec_num]<nh) continue;

    ++pt_in_sec[pt_bin];

    eff_sec_m[i_bin][j_bin]+=1.;

    ++tot_in_sec;

    tot_patts+=ntotpatt;

    tot_patt[pt_bin]+=ntotpatt;

    if (npatt!=0)
    {
      ++pt_in_pat[pt_bin];
      eff_pat_m[i_bin][j_bin]+=1.;
      tot_match+=1;
      tot_goodpatts+=npatt;
      good_patt[pt_bin]+=npatt;
    }
  }	

  
  cout << tot_in_sec << " tracks of sector " << sec_num << " have more than " << nh << " hits in the sector" << endl;
  cout << tot_match << " were matched in a pattern..." << endl;
  cout << tot_patts << " / " << tot_goodpatts << endl;



  for (int i=0;i<80;++i)
  {
    for (int j=0;j<70;++j)   
    {
      tracks->Fill(-2.2+(j+0.5)*4.4/70,-PI+(i+0.5)*2*PI/80,eff_sec_m[i][j]);

      if (eff_sec_m[i][j]!=0.)
      {
	eff_pat_m[i][j] /= eff_sec_m[i][j];
	eff_pat->Fill(-2.2+(j+0.5)*4.4/70,-PI+(i+0.5)*2*PI/80,eff_pat_m[i][j]);
      }
    }
  }

  for (int i=0;i<100;++i)
  {
    if (pt_in_sec[i]!=0.)
    {
      pt_in_pat[i]/=pt_in_sec[i];
      pt_eff->Fill(i+0.5,pt_in_pat[i]);
    }
    if (tot_patt[i]!=0.)
    {
      pt_fake->Fill(i+0.5,(tot_patt[i]-good_patt[i])/tot_patt[i]);
    }

  }

  char buffer[80];
  
  c1 = new TCanvas("c1","Overall efficiencies",6,102,1526,921);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  c1->Range(-1.833856,-0.1286626,21.1442,1.157964);
  c1->Divide(2,2);
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetBorderSize(2);
  c1->SetLeftMargin(0.08);
  c1->SetRightMargin(0.05);
  c1->SetFrameBorderMode(0);
  c1->SetFrameBorderMode(0);
  
  c1->cd(1);  
  eff_pat->GetXaxis()->SetTitle("#eta");
  eff_pat->GetXaxis()->SetLabelFont(42);
  eff_pat->GetXaxis()->SetLabelSize(0.03);
  eff_pat->GetXaxis()->SetTitleSize(0.035);
  eff_pat->GetXaxis()->SetTitleFont(42);
  eff_pat->GetYaxis()->SetTitle("#phi (in rad)");
  eff_pat->GetYaxis()->SetLabelFont(42);
  eff_pat->GetYaxis()->SetLabelSize(0.03);
  eff_pat->GetYaxis()->SetTitleSize(0.035);
  eff_pat->GetYaxis()->SetTitleFont(42);
  eff_pat->SetContour(99);
  eff_pat->Draw("cont4z");

  TPaveText *pte = new TPaveText(-0.57,0.61,0.58,0.68,"br");
  pte->SetFillColor(0);
  pte->SetTextFont(42);
  pte->SetTextSize(0.0349944);
  sprintf (buffer, "Pattern recognition efficiency in the sector");  
  TText *text = pte->AddText(buffer);
  pte->Draw();
  


  //  char buffer[80];

  c1->cd(2); 
  c1->cd(2)->SetGridx();
  c1->cd(2)->SetGridy();
  pt_eff->SetMinimum(-0.1);
  pt_eff->SetMaximum(1.1);
  pt_eff->SetMarkerStyle(3);
  pt_eff->GetXaxis()->SetTitle("Particle p_{T} (in GeV/c)");
  pt_eff->GetXaxis()->SetLabelFont(42);
  pt_eff->GetXaxis()->SetLabelSize(0.035);
  pt_eff->GetXaxis()->SetTitleSize(0.035);
  pt_eff->GetXaxis()->SetTitleOffset(1.19);
  pt_eff->GetXaxis()->SetTitleFont(42);
  pt_eff->GetYaxis()->SetTitle("Pattern reco efficiency in the sector");
  pt_eff->GetYaxis()->SetLabelFont(42);
  pt_eff->GetYaxis()->SetLabelSize(0.035);
  pt_eff->GetYaxis()->SetTitleSize(0.035);
  pt_eff->GetYaxis()->SetTitleFont(42);
  pt_eff->Draw("P");

 
  c1->cd(4); 
  c1->cd(4)->SetGridx();
  c1->cd(4)->SetGridy();
  pt_fake->SetMinimum(-0.1);
  pt_fake->SetMaximum(1.1);
  pt_fake->SetMarkerStyle(3);
  pt_fake->GetXaxis()->SetTitle("Particle p_{T} (in GeV/c)");
  pt_fake->GetXaxis()->SetLabelFont(42);
  pt_fake->GetXaxis()->SetLabelSize(0.035);
  pt_fake->GetXaxis()->SetTitleSize(0.035);
  pt_fake->GetXaxis()->SetTitleOffset(1.19);
  pt_fake->GetXaxis()->SetTitleFont(42);
  pt_fake->GetYaxis()->SetTitle("Proportion of fake patterns");
  pt_fake->GetYaxis()->SetLabelFont(42);
  pt_fake->GetYaxis()->SetLabelSize(0.035);
  pt_fake->GetYaxis()->SetTitleSize(0.035);
  pt_fake->GetYaxis()->SetTitleFont(42);
  pt_fake->Draw("P");

  c1->cd(3);  
  tracks->GetXaxis()->SetTitle("#eta");
  tracks->GetXaxis()->SetLabelFont(42);
  tracks->GetXaxis()->SetLabelSize(0.03);
  tracks->GetXaxis()->SetTitleSize(0.035);
  tracks->GetXaxis()->SetTitleFont(42);
  tracks->GetYaxis()->SetTitle("#phi (in rad)");
  tracks->GetYaxis()->SetLabelFont(42);
  tracks->GetYaxis()->SetLabelSize(0.03);
  tracks->GetYaxis()->SetTitleSize(0.035);
  tracks->GetYaxis()->SetTitleFont(42);
  tracks->SetContour(99);
  tracks->Draw("cont4z");

  TPaveText *pte = new TPaveText(-0.57,0.61,0.58,0.68,"br");
  pte->SetFillColor(0);
  pte->SetTextFont(42);
  pte->SetTextSize(0.0349944);
  sprintf (buffer, "Tracks collected in the sector");  
  TText *text = pte->AddText(buffer);
  pte->Draw();


  c1->Modified();
  c1->Update();


  sprintf (buffer, "Pattern_eff_sector_%d_%d_hits_track.png",sec_num,nh);

  c1->Print(buffer);
  

}
