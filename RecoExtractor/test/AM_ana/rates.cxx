// Class for the rates determination
// For more info, look at the header file

#include "rates.h"

// Main constructor

rates::rates(std::string filename, std::string outfile)
{
  rates::initTuple(filename,outfile);
  rates::initVars();
  rates::get_rates();
}


//////////////////////////////////////////////
//
// 
//////////////////////////////////////////////

void rates::get_rates()
{
  // Initialize some params
 
  int B_id,E_id;   // The detector module IDs (defined in the header)
  int Bl_id,El_id; // This ID is for the averages per ladder (for 1D plots)

  int disk,layer,ladder,module,chip,seg,nseg;

  float IP,PT;

  bool is_prim;
  bool is_prim2;
  bool is_fake;

  int n_entries = L1TT->GetEntries();
  double fact   = 1./static_cast<float>(n_entries); // Normalization factor

  // Then loop over events

  for (int j=0;j<n_entries;++j)
  {
    L1TT->GetEntry(j); 

    if (j%100==0) cout << j << endl;

    if (m_clus == 0) continue; // No clusters, don't go further

    for (int i=0;i<m_clus;++i) // Loop over clusters
    {
      disk  = 0;
      layer = m_clus_layer[i]; 
      ladder= m_clus_ladder[i]; 
      module= m_clus_module[i]; 

      if (layer>10 && layer<=17) disk=(layer-10)%8;
      if (layer>17 && layer<=24) disk=(layer-17)%8+7;

      // Get the ids

      B_id  = (layer-5)*10000 + (ladder-1)*100 + (module-1)/2;
      E_id  = (disk-1)*10000 + (ladder-1)*100 + (module-1)/2;
      Bl_id = (layer-5)*100 + (module-1)/2;
      El_id = (disk-1)*100 + (ladder-1);

      // Then increment the rates

      if (disk==0) // Barrel
      {
	m_b_crate[B_id]      += fact;
	m_b_bylc_rate[Bl_id] += fact;
      }
      else // Disks
      {
	m_e_crate[E_id]      += fact;
	m_e_bylc_rate[El_id] += fact;
      }
    } // End of loop over clusters

    if (m_stub == 0) continue; // No stubs, don't go further
   
    for (int i=0;i<m_stub;++i) // Loop over stubs
    {  
      disk  = 0;
      layer = m_stub_layer[i]; 
      ladder= m_stub_ladder[i]; 
      module= m_stub_module[i]; 
      seg   = m_stub_seg[i]; 
      chip  = m_stub_chip[i]; 
      nseg  = m_clus_nseg[m_stub_clust1[i]];

      if (seg/(nseg/2)==1) chip += 8;

      if (layer>10 && layer<=17) disk=(layer-10)%8;
      if (layer>17 && layer<=24) disk=(layer-17)%8+7;
         
      // Get the ids

      B_id  = (layer-5)*10000 + ladder*100 + module;
      E_id  = (disk-1)*10000 + ladder*100 + module;
      Bl_id = (layer-5)*100 + module;
      El_id = (disk-1)*100 + ladder;

      // Then we look if the stub is fake/secondary/primary 

      is_prim=false;
      is_prim2=false;
      is_fake=false;

      IP = sqrt(m_stub_X0[i]*m_stub_X0[i]+m_stub_Y0[i]*m_stub_Y0[i]);
      PT = sqrt(m_stub_pxGEN[i]*m_stub_pxGEN[i]+m_stub_pyGEN[i]*m_stub_pyGEN[i]);

      if (m_stub_tp[i]>=0 && IP<0.2)         is_prim=true; // OK, perfectible
      if (m_stub_tp[i]<0)                    is_fake=true;
      if (m_stub_tp[i]>=0 && IP<0.2 && PT>2) is_prim2=true; // OK, perfectible

      if (disk==0) // Barrel
      {
	m_b_rate[chip][B_id] += fact;
	m_b_byls_rate[Bl_id] += fact;

	if (is_fake)              m_b_rate_f[B_id]  += fact; 
	if (!is_fake && !is_prim) m_b_rate_s[B_id]  += fact; 
	if (is_prim)              m_b_rate_p[B_id]  += fact;
	if (is_prim2)             m_b_rate_pp[B_id] += fact;	
      }
      else // Disk
      {
	m_e_rate[chip][E_id] += fact;
	m_e_bylc_rate[El_id] += fact;

	if (is_fake)              m_e_rate_f[E_id]  += fact; 
	if (!is_fake && !is_prim) m_e_rate_s[E_id]  += fact; 
	if (is_prim)              m_e_rate_p[E_id]  += fact;
	if (is_prim2)             m_e_rate_pp[E_id] += fact;	
      }
    } // End of loop over stubs
  } // End of loop over events

  // The main tree is filled up at this point
  // In the following we just fill up some infos


  // Here we fill some debug information


  for (int i=0;i<58000;++i) // Barrel
  {
    if (m_b_rate_f[i]+m_b_rate_s[i]+m_b_rate_p[i]==0.) continue;

    for (int j=0;j<16;++j) 
    {
      m_disk= 0;
      m_lay = static_cast<int>(i/10000);
      m_lad = static_cast<int>((i-10000*m_lay)/100);
      m_mod = static_cast<int>((i-10000*m_lay-100*m_lad));
      m_sen = j/8+1;
      m_chp = j%8+1;
      m_rate= m_b_rate[j][i];
      m_dbgtree->Fill(); 
    }
  }

  for (int i=0;i<140000;++i) // Endcap
  {
    if (m_e_rate_f[i]+m_e_rate_s[i]+m_e_rate_p[i]==0.) continue;

    for (int j=0;j<16;++j) 
    {
      m_disk= 1;
      m_lay = static_cast<int>(i/10000);
      m_lad = static_cast<int>((i-10000*m_lay)/100);
      m_mod = static_cast<int>((i-10000*m_lay-100*m_lad));
      m_sen = j/8+1;
      m_chp = j%8+1;
      m_rate= m_e_rate[j][i];
      m_dbgtree->Fill(); 
    }
  }

  // End of dbg loop, fill up root trees

  m_ratetree->Fill();  
  m_outfile->Write();
  delete L1TT;
  delete m_outfile;
}


/////////////////////////////////////////////////////////////
//
// Basic methods, initializations,...
//
/////////////////////////////////////////////////////////////


void rates::initVars()
{
  for (int i=0;i<58000;++i)
  {
    for (int j=0;j<16;++j) m_b_rate[j][i]   = 0.;
    m_b_rate_p[i] = 0.;
    m_b_rate_pp[i] = 0.;
    m_b_rate_s[i] = 0.;
    m_b_rate_f[i] = 0.;
    m_b_crate[i]  = 0.;
  }

  for (int i=0;i<600;++i)
  {
    m_b_bylc_rate[i] = 0.;
    m_b_byls_rate[i] = 0.;
  }

  for (int i=0;i<1500;++i)
  {
    m_e_bylc_rate[i] = 0.;
    m_e_byls_rate[i] = 0.;
  }

  for (int i=0;i<142000;++i)
  {
    for (int j=0;j<16;++j) m_e_rate[j][i]   = 0.;
    m_e_rate_p[i] = 0.;
    m_e_rate_pp[i] = 0.;
    m_e_rate_s[i] = 0.;
    m_e_rate_f[i] = 0.;
    m_e_crate[i]  = 0.;
  }
}


// 
// Initialization of the ntuple. By default we now use official stubs
//

void rates::initTuple(std::string in,std::string out)
{
  L1TT   = new TChain("TkStubs"); 

  // Input data file

  std::size_t found = in.find(".root");

  // Case 1, it's a root file
  if (found!=std::string::npos)
  {
    L1TT->Add(in.c_str());
  }
  else // This is a list provided into a text file
  {
    std::string STRING;
    std::ifstream in2(in.c_str());
    if (!in2)
    {
      std::cout << "Please provide a valid data filename list" << std::endl;
      return;
    }
  
    while (!in2.eof())
    {
      getline(in2,STRING);

      found = STRING.find(".root");
      if (found!=std::string::npos) L1TT->Add(STRING.c_str());
    }

    in2.close();
  }


  pm_clus_layer=&m_clus_layer;
  pm_clus_ladder=&m_clus_ladder;
  pm_clus_module=&m_clus_module;
  pm_clus_nrows=&m_clus_nrows;
  pm_clus_nseg=&m_clus_nseg;

  pm_stub_layer=&m_stub_layer;
  pm_stub_ladder=&m_stub_ladder;
  pm_stub_module=&m_stub_module;
  pm_stub_tp=&m_stub_tp;
  pm_stub_pt=&m_stub_pt;
  pm_stub_pxGEN=&m_stub_pxGEN;
  pm_stub_pyGEN=&m_stub_pyGEN;
  pm_stub_etaGEN=&m_stub_etaGEN;
  pm_stub_X0=&m_stub_X0;
  pm_stub_Y0=&m_stub_Y0;
  pm_stub_seg=&m_stub_seg;
  pm_stub_chip=&m_stub_chip;
  pm_stub_pdgID=&m_stub_pdgID;
  pm_stub_clust1=&m_stub_clust1;


  L1TT->SetBranchAddress("L1TkSTUB_n",         &m_stub);
  L1TT->SetBranchAddress("L1TkSTUB_layer",     &pm_stub_layer);
  L1TT->SetBranchAddress("L1TkSTUB_ladder",    &pm_stub_ladder);
  L1TT->SetBranchAddress("L1TkSTUB_module",    &pm_stub_module);
  L1TT->SetBranchAddress("L1TkSTUB_pxGEN",     &pm_stub_pxGEN);
  L1TT->SetBranchAddress("L1TkSTUB_pyGEN",     &pm_stub_pyGEN);
  L1TT->SetBranchAddress("L1TkSTUB_etaGEN",    &pm_stub_etaGEN);
  L1TT->SetBranchAddress("L1TkSTUB_tp",        &pm_stub_tp);
  L1TT->SetBranchAddress("L1TkSTUB_pt",        &pm_stub_pt);
  L1TT->SetBranchAddress("L1TkSTUB_X0",        &pm_stub_X0);
  L1TT->SetBranchAddress("L1TkSTUB_Y0",        &pm_stub_Y0);
  L1TT->SetBranchAddress("L1TkSTUB_seg",       &pm_stub_seg);
  L1TT->SetBranchAddress("L1TkSTUB_chip",      &pm_stub_chip);
  L1TT->SetBranchAddress("L1TkSTUB_pdgID",     &pm_stub_pdgID);
  L1TT->SetBranchAddress("L1TkSTUB_clust1",    &pm_stub_clust1);

  L1TT->SetBranchAddress("L1TkCLUS_n",         &m_clus);
  L1TT->SetBranchAddress("L1TkCLUS_layer",     &pm_clus_layer);
  L1TT->SetBranchAddress("L1TkCLUS_ladder",    &pm_clus_ladder);
  L1TT->SetBranchAddress("L1TkCLUS_module",    &pm_clus_module);
  L1TT->SetBranchAddress("L1TkCLUS_nrows",     &pm_clus_nrows);
  L1TT->SetBranchAddress("L1TkCLUS_PS",        &pm_clus_nseg);

  m_outfile  = new TFile(out.c_str(),"recreate");
  m_ratetree = new TTree("L1Rates","L1Rates info");
  m_dbgtree  = new TTree("Details","Debug");


  m_ratetree->Branch("STUB_b_rates",         &m_b_rate,      "STUB_b_rates[16][58000]/F");
  m_ratetree->Branch("STUB_b_rates_prim2",   &m_b_rate_pp,   "STUB_b_rates_prim2[58000]/F"); 
  m_ratetree->Branch("STUB_b_rates_prim",    &m_b_rate_p,    "STUB_b_rates_prim[58000]/F"); 
  m_ratetree->Branch("STUB_b_rates_sec",     &m_b_rate_s,    "STUB_b_rates_sec[58000]/F"); 
  m_ratetree->Branch("STUB_b_rates_f",       &m_b_rate_f,    "STUB_b_rates_fake[58000]/F"); 
  m_ratetree->Branch("CLUS_b_rates",         &m_b_crate,     "CLUS_b_rates[58000]/F");
  m_ratetree->Branch("STUB_b_l_rates",       &m_b_byls_rate, "STUB_b_l_rates[600]/F");
  m_ratetree->Branch("CLUS_b_l_rates",       &m_b_bylc_rate, "CLUS_b_l_rates[600]/F");

  m_ratetree->Branch("STUB_e_rates",         &m_e_rate,      "STUB_e_rates[16][142000]/F");
  m_ratetree->Branch("STUB_e_rates_prim2",   &m_e_rate_pp,    "STUB_e_rates_prim2[142000]/F"); 
  m_ratetree->Branch("STUB_e_rates_prim",    &m_e_rate_p,    "STUB_e_rates_prim[142000]/F"); 
  m_ratetree->Branch("STUB_e_rates_sec",     &m_e_rate_s,    "STUB_e_rates_sec[142000]/F"); 
  m_ratetree->Branch("STUB_e_rates_f",       &m_e_rate_f,    "STUB_e_rates_fake[142000]/F"); 
  m_ratetree->Branch("CLUS_e_rates",         &m_e_crate,     "CLUS_e_rates[142000]/F");
  m_ratetree->Branch("STUB_e_l_rates",       &m_e_byls_rate, "STUB_e_l_rates[1500]/F");
  m_ratetree->Branch("CLUS_e_l_rates",       &m_e_bylc_rate, "CLUS_e_l_rates[1500]/F");

  m_dbgtree->Branch("disk",        &m_disk,   "disk/I"); 
  m_dbgtree->Branch("lay",         &m_lay,    "lay/I"); 
  m_dbgtree->Branch("lad",         &m_lad,    "lad/I"); 
  m_dbgtree->Branch("mod",         &m_mod,    "mod/I"); 
  m_dbgtree->Branch("sen",         &m_sen,    "sen/I"); 
  m_dbgtree->Branch("chip",        &m_chp,    "chip/I"); 
  m_dbgtree->Branch("rate",        &m_rate,   "rate/F"); 
}
