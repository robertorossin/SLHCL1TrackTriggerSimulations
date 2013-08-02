// Class for the sector testing
// For more info, look at the header file

#include "sector_test.h"

sector_test::sector_test(std::string filename, std::string secfilename, 
			 std::string pattfilename, std::string outfile, int nevt)
{
  sector_test::initTuple(filename,secfilename,pattfilename,outfile);
 
  sector_test::do_test(nevt); // Launch the test loop over n events
}

void sector_test::do_test(int nevt)
{
  // Get the sector info
  m_sectree->GetEntry(0);

  int id;

  const int m_nsec = m_sectors.size(); // How many sectors are in the file
  const int m_nevt = nevt; 

  int indexes[m_nevt];

  for (int i=0;i<nevt;++i)
  {
    indexes[i]=-1;
  }

  cout << "Starting a test loop over " << nevt << " events..." << endl;
  cout << "... using " << m_nsec << " trigger sectors..." << endl;

  int is_sec_there[m_nsec];
  int ladder,module,layer;
  int n_per_lay[20];
  int n_per_lay_patt[20];
  int hitIndex;
  int n_rods[6] = {16,24,34,48,62,76};
  int nhits_p;

  int patt_file_size = 0;

  m_mod_sec.clear(); 

  std::vector<int> the_sectors;

  
  for (int i=0;i<230000;++i)
  {
    if (i%10000==0) 
      cout << "Caching " << i << "/" << 230000 << endl;

    the_sectors.clear();
    the_sectors.push_back(-1);
    if (i>50000) 
    {    
      for (int k=0;k<m_nsec;++k) // In which sector the module is
      {
	if (sector_test::in_sec(k,i)) the_sectors.push_back(k); 
      }
    }

    m_mod_sec.push_back(the_sectors); 
    /*
    if (the_sectors.size()>1)
    {
      cout << i;
      for (int k=1; k<the_sectors.size();++k)
      {
	cout << " / " << the_sectors.at(k);
      }

      cout << endl;
    }
    */
  }


  if (do_patt)
  {
    patt_file_size=m_PATT->GetEntries();

    for (int i=0;i<patt_file_size;++i)
    {
      if (i%100000==0) 
    	cout << "Processed " << i << "/" << patt_file_size << endl;

      m_PATT->GetEntry(i);
      if (event_id<nevt) indexes[event_id]=i;
    }
  }


  for (int j=0;j<500;++j) mult[j]=0;

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

    //    if (do_patt) m_PATT->GetEntry(i);

    if (do_patt && indexes[i]!=-1) 
    {
      m_PATT->GetEntry(indexes[i]);
    }
    else
    {
      continue;
    }

    if (i%10000==0) 
    	cout << "Processed " << i << "/" << nevt << endl;

    if (m_stub == 0) continue; // No stubs, don't go further

    //
    // First, we make a loop on the stubs in order to check 
    // in which sector they are

    for (int j=0;j<m_stub;++j)
    {  
      if (m_stub_tp[j]!=0) continue;   

      // First of all we compute the ID of the stub's module
      layer  = m_stub_layer[j]; 
      ladder = m_stub_ladder[j]; 
      module = m_stub_module[j]; 

      ///////
      // This hack is temporary and is due to a numbering problem in the TkLayout tool
      if (layer<=10) ladder = (ladder+n_rods[layer-5]/4)%(n_rods[layer-5]);
      ///////

      id = 10000*layer+100*ladder+module; // Get the module ID

      //      cout << id << " / " << m_mod_sec.at(id).size() << endl;
      
      if (m_mod_sec.at(id).size()>1)
      {
	for (int k=1;k<m_mod_sec.at(id).size();++k) // In which sector the module is
	{
	  ++mult[m_mod_sec.at(id).at(k)];
	  if (m_stub_tp[j]==0) ++is_sec_there[m_mod_sec.at(id).at(k)]; // A primary stub is in sector k
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

    // For the sector efficiency we proceed as follow
    // We check if the track has let at least 5 stubs in 
    // 5 different layer/disk (nhits>=5). If yes, we compute 
    // the number of sectors containing at least 5 of those 
    // stubs (nsec)

    // First we get the number of different layers/disks hit by the primary
    for (int k=0;k<20;++k)
    {
      if (n_per_lay[k]!=0) ++nhits;
      nplay[k]=n_per_lay[k];
    }

    // Then we get the number of sectors containing more than 5 primary hits 
    for (int k=0;k<m_nsec;++k)
    {
      if (is_sec_there[k]>4) ++nsec; 
    }

    // Finally we do the pattern loop
    
    if (do_patt)
    {
      ntotpatt = nb_patterns; // The total number of patterns in the sector
      hitIndex = 0;           //
      npatt    = 0;           // The patterns containing at least 5 prim hits
   
      for(int k=0;k<nb_patterns;k++)
      {
	for (int j=0;j<20;++j) n_per_lay_patt[j]=0;

	// First we count the number of prim stubs in the 
	// pattern layers/disks

	for(int m=0;m<nbHitPerPattern[k];m++)
	{
	  if (hit_tp[hitIndex]==0)
	  {
	    n_per_lay_patt[(int)hit_layer[hitIndex]-5]++;

	    //cout << endl;
	    //cout << pt << " / " << hit_ptGEN[hitIndex] << endl;
	    //cout << eta << " / " << hit_etaGEN[hitIndex] << endl;
	    //cout << phi << " / " << hit_phiGEN[hitIndex] << endl;
	  }
	  
	  hitIndex++;
	}

	// First we get the number of different layers/disks hit by the primary
	// in the pattern
	nhits_p=0; 
      
	for (int kk=0;kk<20;++kk)
	{
	  if (n_per_lay_patt[kk]!=0) ++nhits_p;
	}
	
	if (nhits_p>4) ++npatt; // More than 5, the pattern is good
      }
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


void sector_test::initTuple(std::string test,std::string sec,std::string patt,std::string out)
{
  m_infile   = TFile::Open(sec.c_str()); 
  m_sectree  = (TTree*)m_infile->Get("Sectors");

  m_L1TT   = new TChain("L1TrackTrigger"); 
  m_L1TT->Add(test.c_str());

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
  m_efftree = new TTree("SectorEff","");

  pm_sectors=&m_sectors;

  do_patt=false;

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
  

  m_efftree->Branch("nsec",       &nsec,    "nsec/I"); 
  m_efftree->Branch("nhits",      &nhits,   "nhits/I"); 
  m_efftree->Branch("npatt",      &npatt,   "npatt/I"); 
  m_efftree->Branch("tpatt",      &ntotpatt,"tpatt/I"); 
  m_efftree->Branch("nplay",      &nplay,   "nplay[20]/I"); 
  m_efftree->Branch("pt",         &pt,      "pt/F"); 
  m_efftree->Branch("eta",        &eta,     "eta/F"); 
  m_efftree->Branch("phi",        &phi,     "phi/F"); 
  m_efftree->Branch("mult",       &mult,    "mult[500]/I"); 


  // We check the pattern information only if available

  if (patt!="")
  {
    do_patt = true;
    m_pattfile = TFile::Open(patt.c_str());
    m_PATT     = (TTree*)m_pattfile->Get("Patterns");

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
    m_PATT->SetBranchAddress("stub_etaGEN",         hit_etaGEN); 
    m_PATT->SetBranchAddress("stub_phi0GEN",        hit_phiGEN); 
  }
}
