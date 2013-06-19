/*
  ROOT macro for stub rates visualization

  Use:

  root[1]-> .L MakeRatesGraphs.C

  Then you can choose between the following methods 
  knowing that filename denotes the extracted ROOT file 
  containing the stub rates (created by the 
  sectorMaker program).

  You can also use the default ROOT file present in the 
  directory: PU_140_example.root 

  Solution 1 do all the plots
  root[2]-> do_rates_plots(std::string filename) 

  Solution 2 do the plot for barrel layer i
  root[2]-> do_layer_map(std::string filename,int layer)

  Solution 3 do the plot for endcap disk i
  root[2]-> do_disk_map(std::string filename,int disk)

  Information about this macro may be found in the following page:

  http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.HLLHCTuto (STEP III)

  Author: viret@in2p3_dot_fr
  Date: 23/05/2013
*/


void do_efficiency_plots(std::string filename)
{
  for (int i=0;i<20;++i)
  { 
    do_pt_plots(filename,i);
  }

  for (int i=0;i<13;++i)
  { 
    do_eta_plots(filename,i);
  }
}


void do_pt_plots(std::string filename,int layer)
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
  newtree->SetBranchAddress("stub_pri_pt",&stub_pri_pt);
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


  if (disk==0) sprintf (buffer, "Barrel_%d_private_eff.png", layer+5);
  if (disk>0)  sprintf (buffer, "Endcap_p%d_private_eff.png", disk); 
  if (disk<0)  sprintf (buffer, "Endcap_m%d_private_eff.png", abs(disk)); 

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


  if (disk==0) sprintf (buffer, "Barrel_%d_official_eff.png", layer+5);
  if (disk>0)  sprintf (buffer, "Endcap_p%d_official_eff.png", disk); 
  if (disk<0)  sprintf (buffer, "Endcap_m%d_official_eff.png", abs(disk)); 

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


  if (disk==0) sprintf (buffer, "Barrel_%d_stub_eff.png", layer+5);
  if (disk>0)  sprintf (buffer, "Endcap_p%d_stub_eff.png", disk); 
  if (disk<0)  sprintf (buffer, "Endcap_m%d_stub_eff.png", abs(disk)); 

  c3->Print(buffer);
}


void do_eta_plots(std::string filename,int layer)
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
  newtree->SetBranchAddress("stub_pri_eta",&stub_pri_eta);
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


  if (disk==0) sprintf (buffer, "Barrel_%d_private_eta.png", layer+5);
  if (disk>0)  sprintf (buffer, "Endcap_%d_private_eta.png", disk); 
  if (disk<0)  sprintf (buffer, "Endcap_%d_private_eta.png", abs(disk)); 

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


  if (disk==0) sprintf (buffer, "Barrel_%d_official_eta.png", layer+5);
  if (disk>0)  sprintf (buffer, "Endcap_%d_official_eta.png", disk); 
  if (disk<0)  sprintf (buffer, "Endcap_%d_official_eta.png", abs(disk)); 

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


  if (disk==0) sprintf (buffer, "Barrel_%d_stub_eta.png", layer+5);
  if (disk>0)  sprintf (buffer, "Endcap_%d_stub_eta.png", disk); 
  if (disk<0)  sprintf (buffer, "Endcap_%d_stub_eta.png", abs(disk)); 

  c3->Print(buffer);
}

