// Class for the sector testing
// For more info, look at the header file

#include "sector_test.h"

sector_test::sector_test(std::string filename, std::string secfilename, 
			 std::string pattfilename, std::string outfile
			 , int nevt, bool dbg)
{
  
  m_dbg = dbg;

  if (pattfilename!="") 
  {
    if (!m_dbg) // In mass production one has to reorder the events
    {
      sector_test::translateTuple(pattfilename,"rewritten.root");
      sector_test::initTuple(filename,secfilename,"rewritten.root",outfile);
    }
    else
    {
      sector_test::initTuple(filename,secfilename,pattfilename,outfile);
    }
  }
 
  sector_test::do_test(nevt); // Launch the test loop over n events
}


//
// Main method, where the efficiency calculations are made
//

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

  // We create a map linking sector and module numbers and cache it 

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
  }


  // An other linking (evtID in patt and data files might be misaligned

  if (do_patt)
  {
    patt_file_size=m_PATT->GetEntries();

    for (int i=0;i<patt_file_size;++i)
    {
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

    if (i%100000==0) 
      cout << "Processed " << i << "/" << nevt << endl;

    if (do_patt && indexes[i]!=-1) 
    {
      //      cout << i << "/" << indexes[i] << endl;
      m_PATT->GetEntry(indexes[i]);
    }
    else
    {
      continue;
    }

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
      
      if (m_mod_sec.at(id).size()>1)
      {
	for (unsigned int k=1;k<m_mod_sec.at(id).size();++k) // In which sector the module is
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
    // We check if the track has let at least 4 stubs in 
    // 4 different layer/disk (nhits>=4). If yes, we compute 
    // the number of sectors containing at least 4 of those 
    // stubs (nsec)

    // First we get the number of different layers/disks hit by the primary
    for (int k=0;k<20;++k)
    {
      if (n_per_lay[k]!=0) ++nhits;
      nplay[k]=n_per_lay[k];
    }

    // Then we get the number of sectors containing more than 4 primary hits 
    for (int k=0;k<m_nsec;++k)
    {
      if (is_sec_there[k]>=4) ++nsec; 
    }

    // Finally we do the pattern loop

    if (do_patt)
    {
      ntotpatt = nb_patterns; // The total number of patterns in the sector
      hitIndex = 0;           //
      npatt    = 0;           // The patterns containing at least 4 prim hits
   
      for(int k=0;k<nb_patterns;k++)
      {
	for (int j=0;j<20;++j) n_per_lay_patt[j]=0;

	// First we count the number of prim stubs in the 
	// pattern layers/disks

	if (m_dbg) // Debug mode
	{
	  for(int m=0;m<nbHitPerPattern[k];m++)
	  {
	    if (hit_tp[hitIndex]==0)
	    {
	      n_per_lay_patt[(int)hit_layer[hitIndex]-5]++;
	      
	      //cout << endl;
	      //cout << pt  << " / " << hit_ptGEN[hitIndex] << endl;
	      //cout << eta << " / " << hit_etaGEN[hitIndex] << endl;
	      //cout << phi << " / " << hit_phiGEN[hitIndex] << endl;
	    }
	    
	    hitIndex++;
	  }
	}
	else
	{	
	  for(unsigned int m=0;m<m_links.at(k).size();m++)
	  {
	    if (m_stub_tp[m_links.at(k).at(m)]==0)
	    {
	      n_per_lay_patt[m_stub_layer[m_links.at(k).at(m)]-5]++;
	    }
	  }
	}
	
	// First we get the number of different layers/disks hit by the primary
	// in the pattern
	nhits_p=0; 
      
	for (int kk=0;kk<20;++kk)
	{
	  if (n_per_lay_patt[kk]!=0) ++nhits_p;
	}
	
	if (nhits_p>=4) ++npatt; // More than 4, the pattern is good
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
    m_pattfile  = TFile::Open(patt.c_str());
   
    if (m_dbg)
    {
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
    else
    {
      m_PATT    = (TTree*)m_pattfile->Get("L1PatternReco");
      
      pm_links=&m_links;
      pm_secid=&m_secid;
      
      m_PATT->SetBranchAddress("evt",            &event_id); 
      m_PATT->SetBranchAddress("PATT_n",         &nb_patterns);
      m_PATT->SetBranchAddress("PATT_links",     &pm_links);
      m_PATT->SetBranchAddress("PATT_secID",     &pm_secid);
    }
  }
}


//
// Reordering is need for mass pattern recognition (all events a treated in parallel)
//

void sector_test::translateTuple(std::string pattin,std::string pattout)
{
  std::vector< std::vector<int> > m_links;
  std::vector<int> m_secid;
  std::vector< std::vector<int> > *pm_links = new std::vector< std::vector<int> >;
  std::vector<int> *pm_secid = new std::vector<int> ;

  pm_links=&m_links;
  pm_secid=&m_secid;

  int evtID;
  int m_patt;

  TChain *data = new TChain("L1PatternReco");

  data->SetBranchAddress("evt",            &evtID); // Simple evt number or event ID
  data->SetBranchAddress("PATT_n",         &m_patt);
  data->SetBranchAddress("PATT_links",     &pm_links);
  data->SetBranchAddress("PATT_secID",     &pm_secid);
  data->Add(pattin.c_str());

  std::vector< std::vector<int> > *f_links = new std::vector< std::vector<int> >;
  std::vector<int> *f_secid = new std::vector<int> ;

  int f_evtID;
  int f_patt;

  TFile hfile(pattout.c_str(),"RECREATE","");

  TTree *m_tree_L1PatternReco = new TTree("L1PatternReco","L1PatternReco Analysis info");  
  
  /// Branches definition

  m_tree_L1PatternReco->Branch("evt",            &f_evtID); // Simple evt number or event ID
  m_tree_L1PatternReco->Branch("PATT_n",         &f_patt);
  m_tree_L1PatternReco->Branch("PATT_links",     &f_links);
  m_tree_L1PatternReco->Branch("PATT_secID",     &f_secid);

  int ndat = data->GetEntries();

  int evtIDmax = -1;

  std::vector<int> indexes;
  std::vector< std::vector<int> > indexes_ordered;

  indexes.clear();
    
  for(int i=0;i<ndat;i++)
  {    
    data->GetEntry(i);

    if (evtID>evtIDmax) evtIDmax=evtID;

    indexes.push_back(evtID);
  }

  indexes_ordered.resize(evtIDmax+1);

  for(int i=0;i<evtIDmax;i++) indexes_ordered[i].clear();
  for(int i=0;i<ndat;i++)     indexes_ordered[indexes[i]].push_back(i);


  for(int i=0;i<evtIDmax;i++)
  { 
    if (i%100000==0)
      cout << "Reordering event " << i << endl;
  
    f_evtID = i;
    f_patt = 0;
    f_links->clear();
    f_secid->clear();
 
    if (indexes_ordered[i].size()!=0) 
    {
      for(unsigned int j=0;j<indexes_ordered[i].size();j++)
      {    
	data->GetEntry(indexes_ordered[i].at(j));
      
	if (evtID!=i) continue; 

	f_patt += m_patt;
	
	for(int k=0;k<m_patt;k++)
	{ 
	  f_links->push_back(m_links.at(k));
	  f_secid->push_back(m_secid.at(k));
	}
      }
    }    

    m_tree_L1PatternReco->Fill();
  }

  hfile.Write();
  hfile.Close();
}
