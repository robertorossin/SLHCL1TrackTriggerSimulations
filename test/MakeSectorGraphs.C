/*
  ROOT macro for trigger sectors visualization

  Use:

  root[1]-> .L MakeSectorGraphs.C

  Then you can choose between the following methods 
  knowing that filename denotes the extracted ROOT file 
  containing the sectors definition (created by the 
  sectorMaker program) 

  Solution 1 do all the plots
  root[2]-> do_sec_plots(std::string filename) 

  Solution 2 do the plot for barrel layer i
  root[2]-> do_layer_map(std::string filename,int layer)

  Solution 3 do the plot for endcap disk i
  root[2]-> do_disk_map(std::string filename,int disk)

  Information about this macro may be found in the following page:

  http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.HLLHCTuto (STEP III)

  Author: viret@in2p3_dot_fr
  Date: 23/05/2013
*/


void do_sec_plots(std::string filename)
{
  for (int i=0;i<14;++i)
  { 
    do_disk_map(filename,i);
  }

  for (int i=5;i<11;++i)
  { 
    do_layer_map(filename,i);
  }
}


void do_layer_map(std::string filename,int layer)
{
  // First get the data
  // by merging all the available files

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  int count_b[58000];
 
  double n_lad_barrel[6]  = {16,24,34,48,62,76};
  double n_mod_barrel[6]  = {63,55,54,24,24,24};


  TFile *oldfile = TFile::Open(filename.c_str());
  TTree *newtree = (TTree*)oldfile->Get("Sectors");

  newtree->SetBranchAddress("counter_barrel",  &count_b);
  newtree->GetEntry(0);

  int idx;
  float maxval = 0;

  for (int i=0;i<7;++i)
  { 
    for (int j=0;j<58000;++j)
    { 
      if (count_b[j]>maxval) maxval = count_b[j];
    }
  }

  TH2F *cadre = new TH2F("zz","zz",n_mod_barrel[layer-5],0.,n_mod_barrel[layer-5],n_lad_barrel[layer-5],0.,n_lad_barrel[layer-5]);
  TH2F *cadre2 = new TH2F("zz","zz",n_mod_barrel[layer-5]+1,0.,n_mod_barrel[layer-5]+1,n_lad_barrel[layer-5],0.,n_lad_barrel[layer-5]);

  cadre2->Fill(n_mod_barrel[layer-5]+0.5,0.5,0.);
  cadre2->Fill(n_mod_barrel[layer-5]+0.5,0.5,maxval);

  for (int j=0;j<n_mod_barrel[layer-5];++j)
  {
    for (int i=0;i<n_lad_barrel[layer-5];++i)
    {
      idx = 10000*(layer-5) + 100*i + j;

      cadre2->Fill(j+0.5,i+0.5,count_b[idx]);
    }
  }

  const Int_t NRGBs = 5;
  const Int_t NCont = 20;
  
  Double_t stops[NRGBs] = { 1.00, 0.84, 0.61, 0.34, 0.00 };
  Double_t red[NRGBs]   = { 0.00, 0.3, 0.6, 0.9, 1. };
  Double_t green[NRGBs] = { 1., 1., 1., 1., 1. };
  Double_t blue[NRGBs]  = { 1., 1., 1., 1., 1. };

  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);

  c1 = new TCanvas("c1","Layer map",201,77,1470,858);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  c1->Range(-5.887851,-1.930603,70.65421,17.37543);
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetBorderSize(2);
  c1->SetGridx();
  c1->SetGridy();
  c1->SetLeftMargin(0.07692308);
  c1->SetTopMargin(0.07124352);
  c1->SetFrameBorderMode(0);
  c1->SetFrameBorderMode(0);
  

  cadre->GetXaxis()->SetTitle("Barrel module index");
  cadre->GetXaxis()->SetNdivisions(n_mod_barrel[layer-5]);
  cadre->GetXaxis()->SetLabelFont(42);
  cadre->GetXaxis()->SetLabelSize(0.02);
  cadre->GetXaxis()->SetTitleSize(0.035);
  cadre->GetXaxis()->SetTickLength(1);
  cadre->GetXaxis()->SetTitleFont(42);
  cadre->GetYaxis()->SetTitle("Barrel ladder index");
  cadre->GetYaxis()->SetNdivisions(n_lad_barrel[layer-5]);
  cadre->GetYaxis()->SetLabelFont(42);
  cadre->GetYaxis()->SetLabelOffset(0.004);
  cadre->GetYaxis()->SetLabelSize(0.02);
  cadre->GetYaxis()->SetTitleSize(0.03);
  cadre->GetYaxis()->SetTickLength(0.99);
  cadre->GetYaxis()->SetTitleFont(42);
  cadre->GetZaxis()->SetLabelFont(42);
  cadre->GetZaxis()->SetLabelSize(0.035);
  cadre->GetZaxis()->SetTitleSize(0.035);
  cadre->GetZaxis()->SetTitleFont(42);  

  cadre->Draw();
  cadre2->Draw("colzsame");
   
  TPaveText *pt = new TPaveText(-0.249009,16.26533,10.45435,17.25477,"br");
  TPaveText *pt = new TPaveText(0.,n_lad_barrel[layer-5],10./55.*n_mod_barrel[layer-5],17./16.*n_lad_barrel[layer-5],"br");

  char buffer[50];
  sprintf (buffer, "Barrel Layer %d",layer);

  ci = TColor::GetColor("#ccccff");
  pt->SetFillColor(ci);
  pt->SetTextSize(0.03125);
  TText *text = pt->AddText(buffer);
  pt->Draw();
  c1->Modified();

  c1->Update();

  sprintf (buffer, "Barrel_%d_rate_140.eps", layer);
  c1->Print(buffer);

  sprintf (buffer, "Barrel_%d_rate_140.png", layer);
  c1->Print(buffer);

  c1->Update();
}



void do_disk_map(std::string filename,int disk)
{

  // First get the data
  // by merging all the available files

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  int count_e[142000];

  double n_lad_endcap[14]   = {24,26,28,30,34,34,38,40,48,54,62,66,72,78};
  double n_start_endcap[14] = {0,0,0,0,0,0,1,0,0,1,0,0,0,1};

  if (disk>=7) // -Z case
  {
    for (int i=0;i<14;++i) n_start_endcap[i] = 0; // There is no problem in this case
  }

  TFile *oldfile = TFile::Open(filename.c_str());
  TTree *newtree = (TTree*)oldfile->Get("Sectors");

  newtree->SetBranchAddress("counter_endcap", &count_e);

  // Initialize some params
  newtree->GetEntry(0);

  float maxval = 0;

  for (int i=0;i<14;++i)
  { 
    for (int j=10000*i;j<10000*i+2000;++j)
    { 
      if (count_e[j]>maxval) maxval = count_e[j];
    }
  }

  TH2F *cadre  = new TH2F("zz","zz",28,-14.,14.,28,-14.,14.);
  TH2F *ring1  = new TH2F("r1","r1",n_lad_endcap[0],0.,8*atan(1.),25,0.,25.);
  TH2F *ring2  = new TH2F("r2","r2",n_lad_endcap[1],0.,8*atan(1.),25,0.,25.);
  TH2F *ring3  = new TH2F("r3","r3",n_lad_endcap[2],0.,8*atan(1.),25,0.,25.);
  TH2F *ring4  = new TH2F("r4","r4",n_lad_endcap[3],0.,8*atan(1.),25,0.,25.);
  TH2F *ring5  = new TH2F("r5","r5",n_lad_endcap[4],0.,8*atan(1.),25,0.,25.);
  TH2F *ring6  = new TH2F("r6","r6",n_lad_endcap[5],0.,8*atan(1.),25,0.,25.);
  TH2F *ring7  = new TH2F("r7","r7",n_lad_endcap[6],0.,8*atan(1.),25,0.,25.);
  TH2F *ring8  = new TH2F("r8","r8",n_lad_endcap[7],0.,8*atan(1.),25,0.,25.);
  TH2F *ring9  = new TH2F("r9","r9",n_lad_endcap[8],0.,8*atan(1.),25,0.,25.);
  TH2F *ring10 = new TH2F("r10","r10",n_lad_endcap[9],0.,8*atan(1.),25,0.,25.);
  TH2F *ring11 = new TH2F("r11","r11",n_lad_endcap[10],0.,8*atan(1.),25,0.,25.);
  TH2F *ring12 = new TH2F("r12","r12",n_lad_endcap[11],0.,8*atan(1.),25,0.,25.);
  TH2F *ring13 = new TH2F("r13","r13",n_lad_endcap[12],0.,8*atan(1.),25,0.,25.);
  TH2F *ring14 = new TH2F("r14","r14",n_lad_endcap[13],0.,8*atan(1.),25,0.,25.);

  fill_histo(ring1,count_e,n_lad_endcap[0],n_start_endcap[0],disk,0,maxval);
  fill_histo(ring2,count_e,n_lad_endcap[1],n_start_endcap[1],disk,1,maxval);
  fill_histo(ring3,count_e,n_lad_endcap[2],n_start_endcap[2],disk,2,maxval);
  fill_histo(ring4,count_e,n_lad_endcap[3],n_start_endcap[3],disk,3,maxval);
  fill_histo(ring5,count_e,n_lad_endcap[4],n_start_endcap[4],disk,4,maxval);
  fill_histo(ring6,count_e,n_lad_endcap[5],n_start_endcap[5],disk,5,maxval);
  fill_histo(ring7,count_e,n_lad_endcap[6],n_start_endcap[6],disk,6,maxval);
  fill_histo(ring8,count_e,n_lad_endcap[7],n_start_endcap[7],disk,7,maxval);
  fill_histo(ring9,count_e,n_lad_endcap[8],n_start_endcap[8],disk,8,maxval);
  fill_histo(ring10,count_e,n_lad_endcap[9],n_start_endcap[9],disk,9,maxval);
  fill_histo(ring11,count_e,n_lad_endcap[10],n_start_endcap[10],disk,10,maxval);
  fill_histo(ring12,count_e,n_lad_endcap[11],n_start_endcap[11],disk,11,maxval);
  fill_histo(ring13,count_e,n_lad_endcap[12],n_start_endcap[12],disk,12,maxval);
  fill_histo(ring14,count_e,n_lad_endcap[13],n_start_endcap[13],disk,13,maxval);

  const Int_t NRGBs = 5;
  const Int_t NCont = 20;
  
  Double_t stops[NRGBs] = { 1.00, 0.84, 0.61, 0.34, 0.00 };
  Double_t red[NRGBs]   = { 0.00, 0.3, 0.6, 0.9, 1. };
  Double_t green[NRGBs] = { 0.00, 0., .0, 0., 0.00 };
  Double_t blue[NRGBs]  = { 0.0, 0.00, 0.0, 0.00, 0.00 };

  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);

  c1 = new TCanvas("c1","Layer map",166,77,947,880);

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  c1->Range(-15.23757,-14.89125,18.52486,15.55968);
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetBorderSize(2);
  c1->SetLeftMargin(0.03665521);
  c1->SetRightMargin(0.1340206);
  c1->SetTopMargin(0.05121951);
  c1->SetBottomMargin(0.02926829);
  c1->SetFrameBorderMode(0);
  c1->SetFrameBorderMode(0);

  cadre->GetXaxis()->SetLabelSize(0.);
  cadre->GetXaxis()->SetLabelOffset(999);
  cadre->GetYaxis()->SetLabelSize(0.);
  cadre->GetYaxis()->SetLabelOffset(999);
  
  cadre->Draw("");
  ring1->Draw("polcolsame");
  ring2->Draw("polcolsame"); 
  ring3->Draw("polcolsame");
  ring4->Draw("polcolsame");
  ring5->Draw("polcolsame");
  ring6->Draw("polcolsame");
  ring7->Draw("polcolsame");
  ring8->Draw("polcolsame");
  ring9->Draw("polcolsame");
  ring10->Draw("polcolsame");
  ring11->Draw("polcolsame");
  ring12->Draw("polcolsame");
  ring13->Draw("polcolsame");
  ring14->Draw("polcolzsame");
     
  TPaveText *pt = new TPaveText(-14.55731,13.00358,-5.683091,14.59651,"br");

  char buffer[50];

  if (disk>=7)
  {
    sprintf(buffer, "Endcap Disk %d (%sZ side)", (disk-7)%7+1,"-");
  }
  else
  {
    sprintf(buffer, "Endcap Disk %d (%sZ side)", disk%7+1,"+");
  }

  ci = TColor::GetColor("#ccccff");
  pt->SetFillColor(ci);
  pt->SetTextSize(0.02798053);
  TText *text = pt->AddText(buffer);
  pt->Draw();
  c1->Modified();

  c1->Update();

  sprintf (buffer, "Disk_%d_rate_140.eps", disk+1);
  c1->Print(buffer);

  sprintf (buffer, "Disk_%d_rate_140.png", disk+1);
  c1->Print(buffer);
  
}


void fill_histo(TH2F *hist,int val[], double nlad, double dec, int disk, int ring,float max)
{
  float scale = 8*atan(1.)/nlad;

  float phi_sec = 0.;

  for (int i=0;i<int(nlad);++i)  
  {
    phi_sec = (i+.5-dec)*scale;

    if (phi_sec<0.) phi_sec+=8*atan(1.); 

    idx = 10000*disk + 100*ring + i;
    hist->Fill(phi_sec,0.5+ring,val[idx]);
  }
 
  hist->Fill(0.5*scale,24.5,0);
  hist->Fill(1.5*scale,24.5,max);
}
