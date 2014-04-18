/*
  ROOT macros for pattern reco efficiencies visualization

  Use:

  root[1]-> .L MakePR_EffGraphs.C

  Then you can choose between the following methods 
  knowing that filename denotes the extracted ROOT file 
  containing the PR results (created by the 
  sectorMaker program).

  Solution 1 do the test for one sector
  root[2]-> do_pattern_effs(std::string filename, int sec_num, int nh)

  Solution 2 do the test for all the sectors
  root[2]-> do_full_effs(std::string filename, int nh)

  Information about this macro may be found in the following page:

  http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.HLLHCTuto (STEP V part 3.2)

  Author: viret@in2p3_dot_fr
  Date: 30/09/2013
*/

#include <iomanip>      // for std::setprecision



//
// Method plotting pattern efficiencies  
// for a given sector
//
// =>filename is the name of the root file containing the results
// =>nh is the minimum number of layer/disks containing primary stubs 
// from the primary particle 
// =>sec_num is the sector number


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

  char buffer[80];


  const int nbin_eta = 200;
  const int nbin_phi = 200;


  TH2F *tracks     = new TH2F("tracks","tracks",nbin_eta,-2.2,2.2,nbin_phi,-3.15.,3.15);
  TH2F *eff_pat    = new TH2F("eff_p","eff_p",nbin_eta,-2.2,2.2,nbin_phi,-3.15.,3.15);
  TH1F *pt_eff     = new TH1F("eff_pt","eff_pt",100,0.,100.);
  TH1F *pt_eff_z   = new TH1F("eff_pt_z","eff_pt_z",50,0.,10.);

  float eff_stub[nbin_phi][nbin_eta];
  float eff_sector[nbin_phi][nbin_eta];
  float entries_stub[nbin_phi][nbin_eta];

  float eff_pat_m[nbin_phi][nbin_eta];
  float eff_sec_m[nbin_phi][nbin_eta];

  float entries_stub[nbin_phi][nbin_eta];

  for (int i=0;i<nbin_phi;++i)
  {
    for (int j=0;j<nbin_eta;++j)
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

  float pt_in_sec_z[50];
  float pt_in_pat_z[50];
  float good_patt_z[50];
  float tot_patt_z[50];

  for (int i=0;i<100;++i)
  {
    pt_in_sec[i]=0.;
    pt_in_pat[i]=0.;
    good_patt[i]=0.;
    tot_patt[i]=0.;
  }
  
  for (int i=0;i<50;++i)
  {
    pt_in_sec_z[i]=0.;
    pt_in_pat_z[i]=0.;
    good_patt_z[i]=0.;
    tot_patt_z[i]=0.;
  }

  int pt_bin, pt_bin_z;
  int n_sec=0;
  int n_sec_in_range=0;

  std::vector<int> overlap_evt;
  std::vector<int> overlap_evt_good;

  overlap_evt.clear();
  overlap_evt_good.clear();

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
  
  // First loop to get the sector eta/phi acceptance
  
  bool already_known;
  bool already_found;
  
  // Loop on events

  for (int i=0;i<n_entries;++i)
  {
    newtree->GetEntry(i);
    
    if (i%10000==0) 
      cout << "Processed  eeee " << i << "/" << n_entries << endl;
    
    if (fabs(eta)>2.15) continue;
    //    if (fabs(pt)<2) continue;
    
    pt_bin=static_cast<int>(pt);
    pt_bin_z=static_cast<int>(5*pt);

    i_bin = static_cast<int>(nbin_phi*(phi+PI)/(2*PI));
    j_bin = static_cast<int>(nbin_eta*(eta+2.2)/4.4);

    // Check that the track is in the sector acceptance 
    if (mult[sec_num]<nh) continue;
    
    n_sec=0;
    
    for (int j=0;j<100;++j)
    {
      if (mult[j]>=nh && j==sec_num)	++n_sec; 
    }
    
    // If n_sec is different from 0, it means that the 
    // track is in the acceptance of other trigger towers
    //
    // We have to take that into account    


    already_known = false;
    already_found = false;

    if (n_sec!=0) // This track is in an overlap area
    {
      for (int jj=0;jj<overlap_evt.size();++jj)
      {
	if (i==overlap_evt.at(jj)) // This guy was already found somewhere
	{
	  already_known = true;
	  break;
	}
      }
	
      if (already_known==true)
      {
	for (int jj=0;jj<overlap_evt_good.size();++jj)
        {
	  if (i==overlap_evt_good.at(jj)) // This guy was already matched somewhere
	  {
	    already_found = true;
	    break;
	  }
	}
      }
      else
      {
	overlap_evt.push_back(i);
      }
    }

    if (already_found==true) continue;             // Don't count twice
    if (already_known==true && npatt==0) continue; // Don't count innef twice

    if (already_known==false) 
    {
      ++pt_in_sec[pt_bin];
      
      eff_sec_m[i_bin][j_bin]+=1.;
      
      ++tot_in_sec;
      
      if (ntotpatt==-1) ntotpatt=0; 

      tot_patts+=ntotpatt;
      
      tot_patt[pt_bin]+=ntotpatt;
      
      if (pt<10) ++pt_in_sec_z[pt_bin_z];
      if (pt<10) tot_patt_z[pt_bin_z]+=ntotpatt;
      
      if (npatt!=0) // A pattern was matched for this track
      {
	++pt_in_pat[pt_bin];
	
	eff_pat_m[i_bin][j_bin]+=1.;
	tot_match+=1;
	tot_goodpatts+=npatt;
	good_patt[pt_bin]+=npatt;
	
	if (pt<10) 
	{
	  ++pt_in_pat_z[pt_bin_z];
	  ++good_patt_z[pt_bin_z];
	}
      }
    }         
    else // This track is in an overlap area
    {	
      if (npatt!=0)
      {
	overlap_evt_good.push_back(i);
	
	++pt_in_pat[pt_bin];
	
	eff_pat_m[i_bin][j_bin]+=1.;
	tot_match+=1;
	tot_goodpatts+=npatt;
	good_patt[pt_bin]+=npatt;
	
	if (pt<10) 
	{
	  ++pt_in_pat_z[pt_bin_z];
	  ++good_patt_z[pt_bin_z];
	}
      }
    }
  }	

  cout << tot_in_sec << " tracks of sector " << sec_num << " have more than " << nh << " hits in the sector" << endl;
  cout << tot_match << " were matched in a pattern..." << endl;
  cout << tot_patts << " / " << tot_goodpatts << endl;
  cout << " Fake rate is " << float(tot_patts-tot_goodpatts)/float(tot_patts) << endl;
  
  for (int i=0;i<nbin_phi;++i)
  {
    for (int j=0;j<nbin_eta;++j)   
    {
      tracks->Fill(-2.2+(j+0.5)*4.4/nbin_eta,-PI+(i+0.5)*2*PI/nbin_phi,eff_sec_m[i][j]);

      if (eff_sec_m[i][j]!=0.)
      {
	eff_pat_m[i][j] /= eff_sec_m[i][j];
	eff_pat->Fill(-2.2+(j+0.5)*4.4/nbin_eta,-PI+(i+0.5)*2*PI/nbin_phi,eff_pat_m[i][j]);
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
  }

  for (int i=0;i<50;++i)
  {
    if (pt_in_sec_z[i]!=0.)
    {
      pt_in_pat_z[i]/=pt_in_sec_z[i];
      pt_eff_z->Fill(0.2*(i+0.5),pt_in_pat_z[i]);
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
  pt_eff->GetYaxis()->SetTitle("Proportion of patterns");
  pt_eff->GetYaxis()->SetLabelFont(42);
  pt_eff->GetYaxis()->SetLabelSize(0.035);
  pt_eff->GetYaxis()->SetTitleSize(0.035);
  pt_eff->GetYaxis()->SetTitleFont(42);
  pt_eff->SetMarkerStyle(20);
  pt_eff->Draw("P");

  leg = new TLegend(0.6,0.5,0.85,0.65);
  leg->AddEntry(pt_eff,"Pattern reco efficiency","p");
  leg->Draw();

  c1->cd(4); 
  c1->cd(4)->SetGridx();
  c1->cd(4)->SetGridy();
  pt_eff_z->SetMinimum(-0.1);
  pt_eff_z->SetMaximum(1.1);
  pt_eff_z->SetMarkerStyle(3);
  pt_eff_z->GetXaxis()->SetTitle("Particle p_{T} (in GeV/c)");
  pt_eff_z->GetXaxis()->SetLabelFont(42);
  pt_eff_z->GetXaxis()->SetLabelSize(0.035);
  pt_eff_z->GetXaxis()->SetTitleSize(0.035);
  pt_eff_z->GetXaxis()->SetTitleOffset(1.19);
  pt_eff_z->GetXaxis()->SetTitleFont(42);
  pt_eff_z->GetYaxis()->SetTitle("Proportion of patterns");
  pt_eff_z->GetYaxis()->SetLabelFont(42);
  pt_eff_z->GetYaxis()->SetLabelSize(0.035);
  pt_eff_z->GetYaxis()->SetTitleSize(0.035);
  pt_eff_z->GetYaxis()->SetTitleFont(42);
  pt_eff_z->SetMarkerStyle(20);
  pt_eff_z->Draw("P");

  leg = new TLegend(0.6,0.5,0.85,0.65);
  leg->AddEntry(pt_eff_z,"Pattern reco efficiency","p");
  leg->Draw();

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




//
// Method plotting pattern efficiencies  
// for the full tracker
//
// =>filename is the name of the root file containing the results
// =>nh is the minimum number of layer/disks containing primary stubs 
// from the primary particle 
// =>plot_fake is an option that puts the fake rate vs pt info over
// the plot. This is relevant only for PGun events so could be
// turned off otherwise 
//
// Each entry corresponds to a given primary particle of the 
// input file. So there could be many entries with the same evt ID



void do_full_effs(std::string filename, int nh, int pdgid=-1, float ptcut=2., float d0cut=1., bool plot_fake=false)
{
  // First get the data
  // by merging all the available files

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  int   evt;        // Event number (for PU event, where there is more than 1 primary)
  int   nsec;       // The number of sectors containing at least 5 stubs of the prim. track
  float pt;         // The pT of the prim. track
  float d0;         // The d0 of the prim. track
  float eta;        // The eta of the prim. track
  float phi;        // The phi of the prim. track
  int   pdg;        // The pdg id of the prim. track
  int   npatt;      // The number of patterns containing at least 5 stubs of the prim. track
  int   ntotpatt;   // The total number of patterns 
  int   mult[500];  // The total number of stubs per sector 
  int   nhits;      // The total number of layers/disks hit by the prim track


  TFile *file    = TFile::Open(filename.c_str());
  TTree *newtree = (TTree*)file->Get("SectorEff");

  newtree->SetBranchAddress("event",      &evt);
  newtree->SetBranchAddress("nsec",       &nsec);
  newtree->SetBranchAddress("nhits",      &nhits);
  newtree->SetBranchAddress("npatt",      &npatt);
  newtree->SetBranchAddress("tpatt",      &ntotpatt);
  newtree->SetBranchAddress("pt",         &pt);
  newtree->SetBranchAddress("d0",         &d0);
  newtree->SetBranchAddress("eta",        &eta);
  newtree->SetBranchAddress("pdgID",      &pdg);
  newtree->SetBranchAddress("phi",        &phi);
  newtree->SetBranchAddress("mult",       &mult);

  int n_entries = newtree->GetEntries();

  const int nbin_eta = 200;
  const int nbin_phi = 200;


  TH2F *tracks     = new TH2F("tracks","tracks",nbin_eta,-2.2,2.2,nbin_phi,-3.15.,3.15);
  TH2F *eff_pat    = new TH2F("eff_p","eff_p",nbin_eta,-2.2,2.2,nbin_phi,-3.15.,3.15);
  TH2F *fak_pat    = new TH2F("fak_p","fak_p",nbin_eta,-2.2,2.2,nbin_phi,-3.15.,3.15);
  TH2F *pt_eff     = new TH2F("eff_pt","eff_pt",100,0.,100.,200,-0.1,1.1);
  TH2F *pt_fake    = new TH2F("fak_pt","fak_pt",100,0.,100.,200,-0.1,1.1);
  TH2F *pt_dupl    = new TH2F("dup_pt","dup_pt",100,0.,100.,200,-0.1,1.1);

  TH2F *pt_eff_z   = new TH2F("eff_pt_z","eff_pt_z",50,0.,10.,200,-0.1,1.1);
  TH2F *pt_fake_z  = new TH2F("fak_pt_z","fak_pt_z",50,0.,10.,200,-0.1,1.1);
  TH2F *pt_dupl_z  = new TH2F("dup_pt_z","dup_pt_z",50,0.,10.,200,-0.1,1.1);

  float eff_stub[nbin_phi][nbin_eta];
  float eff_sector[nbin_phi][nbin_eta];
  float entries_stub[nbin_phi][nbin_eta];

  float eff_pat_m[nbin_phi][nbin_eta];
  float eff_sec_m[nbin_phi][nbin_eta];

  float entries_stub[nbin_phi][nbin_eta];

  for (int i=0;i<nbin_phi;++i)
  {
    for (int j=0;j<nbin_eta;++j)
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

  int prev_evt = -1;

  int i_bin;
  int j_bin;

  float PI=4.*atan(1.);

  float pt_in_dup[100];
  float pt_in_sec[100];
  float pt_in_pat[100];
  float good_patt[100];
  float tot_patt[100];
  int tot_patt_count[100];

  float pt_in_dup_z[50];
  float pt_in_sec_z[50];
  float pt_in_pat_z[50];
  float good_patt_z[50];
  float tot_patt_z[50];
  int tot_patt_z_count[50];

  for (int i=0;i<100;++i)
  {
    pt_in_dup[i]=0.;
    pt_in_sec[i]=0.;
    pt_in_pat[i]=0.;
    good_patt[i]=0.;
    tot_patt[i]=0.;
    tot_patt_count[i]=0;
  }
  
  for (int i=0;i<50;++i)
  {
    pt_in_dup_z[i]=0.;
    pt_in_sec_z[i]=0.;
    pt_in_pat_z[i]=0.;
    good_patt_z[i]=0.;
    tot_patt_z[i]=0.;
    tot_patt_z_count[i]=0;
  }

  int pt_bin, pt_bin_z;
  int n_sec=0;
  std::vector<int> sectors;


  // First loop to get the sector eta/phi acceptance

  for (int i=0;i<n_entries;++i)
  {
    sectors.clear();

    if (i%10000==0) 
      cout << "Processed " << i << "/" << n_entries << endl;

    newtree->GetEntry(i);

    if (pdg!=pdgid && pdgid!=-1) continue;
    if (fabs(pt)>100) continue;
    if (fabs(eta)>2.2) continue;
    if (nhits<nh) continue;
    if (fabs(pt)<ptcut) continue;
    if (fabs(d0)>d0cut) continue;

    pt_bin=static_cast<int>(pt);
    pt_bin_z=static_cast<int>(5*pt);

    i_bin = static_cast<int>(nbin_phi*(phi+PI)/(2*PI));
    j_bin = static_cast<int>(nbin_eta*(eta+2.2)/4.4);

    n_sec=0;

    for (int j=0;j<100;++j)
    {
      if (mult[j]>=nh)
      {
       ++n_sec; 
       sectors.push_back(j);
      }
    }

    if (n_sec==0) continue; // The track is not in the acceptance

    ++pt_in_sec[pt_bin];

    eff_sec_m[i_bin][j_bin]+=1.;

    ++tot_in_sec;

    if (ntotpatt==-1) ntotpatt=0;

    if (evt!=prev_evt)
    {
      prev_evt = evt;
      tot_patts+=ntotpatt;

      for (int ii=0;ii<100;++ii) tot_patt_count[ii]=0;
      for (int ii=0;ii<50;++ii) tot_patt_z_count[ii]=0;
    }

    if (tot_patt_count[pt_bin]!=1) // Avoit double counting
    {
      tot_patt[pt_bin]+=ntotpatt;      
      tot_patt_count[pt_bin]=1;
    }

    if (pt<10)
    {
      if (tot_patt_z_count[pt_bin_z]!=1)
      {
	tot_patt_z[pt_bin_z]+=ntotpatt;
	tot_patt_z_count[pt_bin_z]=1;
      }

      ++pt_in_sec_z[pt_bin_z];
    }

    if (npatt!=0)
    {
      ++pt_in_pat[pt_bin];
      if (npatt>1) ++pt_in_dup[pt_bin];

      eff_pat_m[i_bin][j_bin]+=1.;
      tot_match+=1;
      tot_goodpatts+=npatt;
      good_patt[pt_bin]+=npatt;

      if (pt<10) 
      {
	++pt_in_pat_z[pt_bin_z];
	++good_patt_z[pt_bin_z];
	if (npatt>1) ++pt_in_dup_z[pt_bin_z];
      }
    }
  }	

  
  cout << tot_in_sec << " tracks have more than " << nh << " hits in a sector" << endl;
  cout << tot_match << " were matched in a pattern..." << endl;
  cout << tot_patts << " / " << tot_goodpatts << endl;
  cout << " Fake rate is " << float(tot_patts-tot_goodpatts)/float(tot_patts) << endl;


  for (int i=0;i<nbin_phi;++i)
  {
    for (int j=0;j<nbin_eta;++j)   
    {
      tracks->Fill(-2.2+(j+0.5)*4.4/nbin_eta,-PI+(i+0.5)*2*PI/nbin_phi,eff_sec_m[i][j]);

      if (eff_sec_m[i][j]!=0.)
      {
	eff_pat_m[i][j] /= eff_sec_m[i][j];
	eff_pat->Fill(-2.2+(j+0.5)*4.4/nbin_eta,-PI+(i+0.5)*2*PI/nbin_phi,eff_pat_m[i][j]);
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

    if (pt_in_pat[i]!=0.)
    {
      pt_in_dup[i]/=pt_in_pat[i];
      pt_dupl->Fill(i+0.5,pt_in_dup[i]);
    }


    //    cout << i << " /" << good_patt[i] << " / " << tot_patt[i] << endl;
    
    if (tot_patt[i]!=0.)
    {
      pt_fake->Fill(i+0.5,(tot_patt[i]-good_patt[i])/tot_patt[i]);
    }
  }

  for (int i=0;i<50;++i)
  {
    if (pt_in_sec_z[i]!=0.)
    {
      pt_in_pat_z[i]/=pt_in_sec_z[i];

      //      cout << i << " / " << pt_in_pat_z[i] << " / " << pt_in_sec_z[i] << endl;
      pt_eff_z->Fill(0.2*(i+0.5),pt_in_pat_z[i]);
    }

    if (tot_patt_z[i]!=0.)
    {
      pt_fake_z->Fill(0.2*(i+0.5),(tot_patt_z[i]-good_patt_z[i])/tot_patt_z[i]);
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
  pt_eff->GetYaxis()->SetTitle("Proportion of patterns");
  pt_eff->GetYaxis()->SetLabelFont(42);
  pt_eff->GetYaxis()->SetLabelSize(0.035);
  pt_eff->GetYaxis()->SetTitleSize(0.035);
  pt_eff->GetYaxis()->SetTitleFont(42);
  pt_eff->GetYaxis()->SetRangeUser(-0.1,1.1);
  pt_eff->SetMarkerStyle(20);
  pt_fake->SetMarkerStyle(4);
  pt_eff->Draw();
  if (plot_fake) pt_fake->Draw("same");

  leg = new TLegend(0.6,0.5,0.85,0.65);
  leg->AddEntry(pt_eff,"Pattern reco efficiency","p");
  if (plot_fake) leg->AddEntry(pt_fake,"Fake proportion","p");
  leg->Draw();

  c1->cd(4); 
  c1->cd(4)->SetGridx();
  c1->cd(4)->SetGridy();
  pt_eff_z->SetMinimum(-0.1);
  pt_eff_z->SetMaximum(1.1);
  pt_eff_z->SetMarkerStyle(3);
  pt_eff_z->GetXaxis()->SetTitle("Particle p_{T} (in GeV/c)");
  pt_eff_z->GetXaxis()->SetLabelFont(42);
  pt_eff_z->GetXaxis()->SetLabelSize(0.035);
  pt_eff_z->GetXaxis()->SetTitleSize(0.035);
  pt_eff_z->GetXaxis()->SetTitleOffset(1.19);
  pt_eff_z->GetXaxis()->SetTitleFont(42);
  pt_eff_z->GetYaxis()->SetTitle("Proportion of patterns");
  pt_eff_z->GetYaxis()->SetLabelFont(42);
  pt_eff_z->GetYaxis()->SetLabelSize(0.035);
  pt_eff_z->GetYaxis()->SetTitleSize(0.035);
  pt_eff_z->GetYaxis()->SetTitleFont(42);
  pt_eff_z->SetMarkerStyle(20);
  pt_fake_z->SetMarkerStyle(4);
  pt_eff_z->Draw("");
  //  if (plot_fake) pt_fake_z->Draw("Psame");

  leg = new TLegend(0.6,0.5,0.85,0.65);
  leg->AddEntry(pt_eff_z,"Pattern reco efficiency","p");
  //  if (plot_fake) leg->AddEntry(pt_fake_z,"Fake proportion","p");
  leg->Draw();

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


  sprintf (buffer, "Pattern_eff_%d_hits_track.png",nh);

  c1->Print(buffer);
 
  sprintf (buffer, "Pattern_eff_%d_hits_track.C",nh);

  c1->SaveSource(buffer);
 

}
