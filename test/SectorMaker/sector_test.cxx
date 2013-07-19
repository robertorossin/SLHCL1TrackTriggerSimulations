// Class for the sector testing
// For more info, look at the header file

#include "sector_test.h"

sector_test::sector_test(std::string filename, std::string secfilename, 
			 std::string outfile, int nevt)
{
  sector_test::initTuple(filename,secfilename,outfile);
  sector_test::do_test(nevt);
}

void sector_test::do_test(int nevt)
{
  // Get the sector info
  m_sectree->GetEntry(0);

  int id;

  const int m_nsec = m_sectors.size(); // How many sectors are in the file

  cout << "Starting a test loop over " << nevt << " events..." << endl;
  cout << "... using " << m_nsec << " trigger sectors..." << endl;

  int sharing[m_nsec][m_nsec];

  int is_sec_there[m_nsec];
  int ladder,module,layer;
  int n_per_lay[20];
  int n_per_lay_patt[20];
  int hitIndex;
  int n_rods[6] = {16,24,34,48,62,76};
  int nhits_p;

  for (int j=0;j<500;++j) mult[j]=0;
    
  for (int j=0;j<m_nsec;++j)
  {
    for (int i=0;i<m_nsec;++i)
    {
      sharing[i][j]=0;
    }
  }

  // Loop over the events
  for (int i=0;i<nevt;++i)
  { 
    nhits=0;
    nsec=0;

    eta=0.;
    pt=0.;
    phi=0.;
    npatt=-1;
    ntotpatt=0;

    for (int j=0;j<20;++j) n_per_lay[j]=0;
    for (int j=0;j<20;++j) n_per_lay_patt[j]=0;

    for (int j=0;j<m_nsec;++j)
    {
      is_sec_there[j]=0;
      mult[j]=0;
    }

    m_L1TT->GetEntry(i);
    m_PATT->GetEntry(0);

    //    if (i%1000==0) 
    	cout << "Processed " << i << "/" << nevt << endl;

    if (m_stub == 0) continue; // No stubs, don't go further

    for (int j=0;j<m_stub;++j)
    {  
      // First of all we compute the ID of the stub's module

      //if (m_stub_tp[j]!=0) continue; // Keep only the primary PGUN particle

      layer  = m_stub_layer[j]; 
      ladder = m_stub_ladder[j]; 
      if (layer<=10) ladder = (ladder+n_rods[layer-5]/4)%(n_rods[layer-5]);

      module = m_stub_module[j]; 

      //cout << layer << " / " << ladder << " / " << module << endl;

      id = 10000*layer+100*ladder+module; // Get the module ID

      for (int k=0;k<m_nsec;++k)
      {
	if (sector_test::in_sec(k,id)) // Test is module is in sector 
	{
	  ++mult[k];
	  if (m_stub_tp[j]==0) ++is_sec_there[k];
	}
      }

      if (m_stub_tp[j]==0)   
      {
	++n_per_lay[layer-5];
	pt  = sqrt(m_stub_pxGEN[j]*m_stub_pxGEN[j]+m_stub_pyGEN[j]*m_stub_pyGEN[j]);
	eta = m_stub_etaGEN[j];
	phi = atan2(m_stub_pyGEN[j],m_stub_pxGEN[j]);
      }
    }

    hitIndex=0;
    ntotpatt=nb_patterns; 
    npatt=0;
   
    for(int k=0;k<nb_patterns;k++)
    {
      nhits_p=0;

      for (int j=0;j<20;++j) n_per_lay_patt[j]=0;

	//affichage des stubs dans le pattern actif
      for(int m=0;m<nbHitPerPattern[k];m++)
      {
	if (hit_tp[hitIndex]==0)
	  n_per_lay_patt[(int)hit_layer[hitIndex]-5]++;

	hitIndex++;
      }
      
      for (int kk=0;kk<20;++kk)
      {
	if (n_per_lay_patt[kk]!=0)
	{
	  ++nhits_p;
	}
      }

      if (nhits_p>4) ++npatt;
    }

    for (int k=0;k<20;++k)
    {
      if (n_per_lay[k]!=0) ++nhits;
    }

    for (int k=0;k<m_nsec;++k)
    {
      if (is_sec_there[k]>=5) ++nsec; 
    }

    m_efftree->Fill();
  }
 
  m_outfile->Write();
  m_outfile->Close();
}

/////////////////////////////////////////////////////////////
//
// Basic methods, initializations,...
//
/////////////////////////////////////////////////////////////


// Method checking that module mod is in sector sec

bool sector_test::in_sec(int sec, int mod)
{  
  int nmods =  m_sectors.at(sec).size();

  for (int j=0;j<nmods;++j)
  {
    if (mod == m_sectors.at(sec).at(j)) return true;
  }

  return false;
}


void sector_test::initTuple(std::string test,std::string sec,std::string out)
{
  m_infile   = TFile::Open(sec.c_str()); 
  m_sectree  = (TTree*)m_infile->Get("Sectors");
  m_pattfile = TFile::Open("Data/SingMuons6L_32_36Cov_6on6.root");

  m_L1TT   = new TChain("L1TrackTrigger"); 

  m_L1TT->Add(test.c_str());
  m_PATT     = (TTree*)m_pattfile->Get("Patterns");
  /*
  m_L1TT->Add("/scratch/viret/PU_SKIM/PU_01.root");
  m_L1TT->Add("/scratch/viret/PU_SKIM/PU_02.root");
  m_L1TT->Add("/scratch/viret/PU_SKIM/PU_03.root");
  m_L1TT->Add("/scratch/viret/PU_SKIM/PU_04.root");
  m_L1TT->Add("/scratch/viret/PU_SKIM/PU_05.root");
  m_L1TT->Add("/scratch/viret/PU_SKIM/PU_06.root");
  m_L1TT->Add("/scratch/viret/PU_SKIM/PU_07.root");
  m_L1TT->Add("/scratch/viret/PU_SKIM/PU_08.root");
  m_L1TT->Add("/scratch/viret/PU_SKIM/PU_09.root");
  */
  m_outfile = new TFile(out.c_str(),"recreate");
  m_efftree = new TTree("Efficiencies","");

  pm_sectors=&m_sectors;

  m_sectree->SetBranchAddress("sectors",          &pm_sectors);
 
  pm_stub_layer=&m_stub_layer;
  pm_stub_ladder=&m_stub_ladder;
  pm_stub_module=&m_stub_module;
  pm_stub_pxGEN=&m_stub_pxGEN;
  pm_stub_pyGEN=&m_stub_pyGEN;
  pm_stub_etaGEN=&m_stub_etaGEN;
  pm_stub_tp=&m_stub_tp;

  m_L1TT->SetBranchAddress("STUB_n",         &m_stub);
  m_L1TT->SetBranchAddress("STUB_layer",     &pm_stub_layer);
  m_L1TT->SetBranchAddress("STUB_ladder",    &pm_stub_ladder);
  m_L1TT->SetBranchAddress("STUB_module",    &pm_stub_module);
  m_L1TT->SetBranchAddress("STUB_pxGEN",     &pm_stub_pxGEN);
  m_L1TT->SetBranchAddress("STUB_pyGEN",     &pm_stub_pyGEN);
  m_L1TT->SetBranchAddress("STUB_etaGEN",    &pm_stub_etaGEN);
  m_L1TT->SetBranchAddress("STUB_tp",        &pm_stub_tp);

  m_PATT->SetBranchAddress("nbLayers",            &nb_layers); 
  m_PATT->SetBranchAddress("nbPatterns",          &nb_patterns);
  m_PATT->SetBranchAddress("eventID",             &event_id);   
  m_PATT->SetBranchAddress("sectorID",            pattern_sector_id); 
  m_PATT->SetBranchAddress("nbStubs",             nbHitPerPattern);   
  m_PATT->SetBranchAddress("stub_layers",         hit_layer);      
  m_PATT->SetBranchAddress("stub_ladders",        hit_ladder);     
  m_PATT->SetBranchAddress("stub_module",         hit_zPos);       
  m_PATT->SetBranchAddress("stub_segment",        hit_segment);    
  m_PATT->SetBranchAddress("stub_strip",          hit_strip);      
  m_PATT->SetBranchAddress("stub_tp",             hit_tp);         
  m_PATT->SetBranchAddress("stub_ptGEN",          hit_ptGEN);   

  m_efftree->Branch("nsec",       &nsec,  "nsec/I"); 
  m_efftree->Branch("nhits",      &nhits, "nhits/I"); 
  m_efftree->Branch("npatt",      &npatt,   "npatt/I"); 
  m_efftree->Branch("tpatt",      &ntotpatt,"tpatt/I"); 
  m_efftree->Branch("pt",         &pt,    "pt/F"); 
  m_efftree->Branch("eta",        &eta,   "eta/F"); 
  m_efftree->Branch("phi",        &phi,   "phi/F"); 
  m_efftree->Branch("mult",       &mult,  "mult[500]/I"); 
}
