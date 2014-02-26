// Class for the L1 track reco efficiency tests
// For more info, look at the header file

#include "track_eff.h"

track_eff::track_eff(std::string filename, std::string secfilename, 
		     std::string outfile, int nevt, float pt_min, 
		     float d0_min, bool dbg)
{  
  m_dbg    = dbg;
  m_d0_min = d0_min;
  m_pt_min = pt_min;

  track_eff::initTuple(filename,outfile);
  
  if (!track_eff::convert(secfilename)) return; // Don't go further if there is no sector file

  track_eff::do_test(nevt); // Launch the test loop over n events
}


/////////////////////////////////////////////////////////////////////////////////
//
// ==> track_eff::do_test(int nevt)
//
// Main method, where the efficiency calculations are made
//
/////////////////////////////////////////////////////////////////////////////////

void track_eff::do_test(int nevt)
{
  int id;

  const int m_nsec = m_sec_mult; // How many sectors are in the file (from convert)

  int ndat = std::min(nevt,static_cast<int>(m_L1TT->GetEntries())); // How many events will we test

  cout << "Starting a test loop over " << ndat << " events..." << endl;
  cout << "... using " << m_nsec << " trigger sectors..." << endl;

  int is_sec_there[m_nsec];
  int ladder,module,layer;
  int n_per_lay[20];
  int n_per_lay_patt[20];
  int n_rods[6] = {16,24,34,48,62,76};
  int nhits_p;

  for (int j=0;j<500;++j) mult[j]=0;

  m_primaries.clear(); 

  std::vector<int> the_sectors;
  std::vector<int> prim_stubs;

  std::vector<int> stubs;
  std::vector<int> parts;

  // Loop over the events
 
  for (int i=0;i<ndat;++i)
    //for (int i=0;i<10;++i)
  {    
    track_eff::reset();

    m_L1TT->GetEntry(i);
    m_PATT->GetEntry(i);      

    if (i%1000==0) 
      cout << "Processed " << i << "/" << ndat << endl;

    if (m_stub == 0) continue; // No stubs, don't go further

    evt = i;
    n_stub_total=m_stub; 
    n_patt=nb_patterns; 
    n_track=nb_tracks; 

    parts.clear();

    // Here we fill the pattern and tracks containers    
    // the info is already there

    if (nb_patterns>0)
    {	  
      for(int kk=0;kk<nb_patterns;kk++)
      {
	patt_sec->push_back(m_pattsecid.at(kk));  
	patt_parts->push_back(parts);   
	patt_stubs->push_back(m_pattlinks.at(kk));
      }     
    }    
    
      
    if (nb_tracks>0)
    {	  
      for(int kk=0;kk<nb_tracks;kk++)
      {
	trk_sec->push_back(m_trksecid.at(kk));  
	trk_pt->push_back(m_trkpt.at(kk));   
	trk_eta->push_back(2*m_trketa.at(kk));   
	trk_phi->push_back(m_trkphi.at(kk));   
	trk_z->push_back(m_trkz.at(kk));  
	trk_parts->push_back(parts);   
	trk_stubs->push_back(m_trklinks.at(kk));
      }     
    } 

    //
    // First, we make a loop on the stubs in order to check 
    // how many interesting primary tracks we have
    //
    // The info is then stored in the m_primaries vector
    //

    bool already_there = 0;

    for (int j=0;j<m_stub;++j)
    {  
      stub_x->push_back(m_stub_x[j]);  
      stub_y->push_back(m_stub_y[j]);   
      stub_z->push_back(m_stub_z[j]);  
      stub_layer->push_back(m_stub_layer[j]);   
      stub_ladder->push_back(m_stub_ladder[j]);  
      stub_module->push_back(m_stub_module[j]);  
      stub_seg->push_back(m_stub_segment[j]);  
      stub_strip->push_back(m_stub_strip[j]);  
      stub_tp->push_back(-1);  
      stub_inpatt->push_back(0);  
      stub_intrk->push_back(0);  
  
      if (m_stub_tp[j]<0) continue; // Bad stub  
      
      // Basic primary selection (pt and d0 cuts)
      if (sqrt(m_stub_pxGEN[j]*m_stub_pxGEN[j]+m_stub_pyGEN[j]*m_stub_pyGEN[j])<m_d0_min) continue;
      if (sqrt(m_stub_X0[j]*m_stub_X0[j]+m_stub_Y0[j]*m_stub_Y0[j])>m_pt_min) continue; 

      already_there = false;

      for (unsigned int k=0;k<m_primaries.size();++k) // Check if it's already been found
      {  
	if (m_primaries.at(k).at(0)!=m_stub_tp[j]) continue;

	stub_tp->at(j)=k;  
	already_there = true;
	m_primaries.at(k).push_back(j); // If yes, just put the stub index
	break;
      }

      if (already_there == true) continue;   

      // Here we have a new primary, we create a new entry

      ++n_part;
      part_pdg->push_back(m_stub_pdg[j]);   
      part_nsec->push_back(0);   
      part_nhits->push_back(0);   
      part_npatt->push_back(0);   
      part_ntrk->push_back(0);   
      part_pt->push_back(sqrt(m_stub_pxGEN[j]*m_stub_pxGEN[j]+m_stub_pyGEN[j]*m_stub_pyGEN[j]));   
      part_rho->push_back(sqrt(m_stub_X0[j]*m_stub_X0[j]+m_stub_Y0[j]*m_stub_Y0[j]));  
      part_z0->push_back(m_stub_Z0[j]);    
      part_eta->push_back(m_stub_etaGEN[j]);    
      part_phi->push_back(atan2(m_stub_pyGEN[j],m_stub_pxGEN[j]));

      prim_stubs.clear();
      prim_stubs.push_back(m_stub_tp[j]); // The TP id 
      prim_stubs.push_back(j); // The first stub ID

      m_primaries.push_back(prim_stubs);

      stub_tp->at(j)=m_primaries.size()-1;  
      
    }

    if (m_primaries.size()>10)
      std::cout << "Found " << m_primaries.size() 
		<< " primary particles in the event" << i << std::endl; 
    

    //
    // Then, in the second loop, we test all the primaries 
    // in order to check if they have matched a pattern or a track
    //
     
    int idx = 0;

    for (unsigned int k=0;k<m_primaries.size();++k)
    {
      if (m_primaries.at(k).size()<4) continue; // Less then 4 stubs, give up this one

      for (int j=0;j<20;++j) n_per_lay[j]=0;
      for (int j=0;j<20;++j) n_per_lay_patt[j]=0;

      for (int j=0;j<m_nsec;++j)
      {
	is_sec_there[j]=0;
	mult[j]=0;
      }

      nhits=0;
      nsec=0;
      eta=0.;
      pt=0.;
      phi=0.;
      d0=-1.;
      npatt=-1;
      ntrack=-1;

      // Here we make a loop over prim stubs to compute the value of NHits

      for (unsigned int j=1;j<m_primaries.at(k).size();++j)
      {
	idx    = m_primaries.at(k).at(j); // stub index

	// First of all we compute the ID of the stub's module
	layer  = m_stub_layer[idx]; 
	ladder = m_stub_ladder[idx]; 
	module = m_stub_module[idx]; 

	///////
	// This hack is temporary and is due to a numbering problem in the TkLayout tool
	if (layer<=10) ladder = (ladder+n_rods[layer-5]/4)%(n_rods[layer-5]);
	///////

	id = 10000*layer+100*ladder+module; // Get the module ID
      
	if (m_modules.at(id).size()>1)
	{
	  for (unsigned int kk=1;kk<m_modules.at(id).size();++kk) // In which sector the module is
	  {
	    ++mult[m_modules.at(id).at(kk)];
	    ++is_sec_there[m_modules.at(id).at(kk)]; 
	  }
	}

	++n_per_lay[layer-5];
	pt  = sqrt(m_stub_pxGEN[idx]*m_stub_pxGEN[idx]+m_stub_pyGEN[idx]*m_stub_pyGEN[idx]);
	eta = m_stub_etaGEN[idx];
	phi = atan2(m_stub_pyGEN[idx],m_stub_pxGEN[idx]);
	d0  = sqrt(m_stub_X0[idx]*m_stub_X0[idx]+m_stub_Y0[idx]*m_stub_Y0[idx]);
	pdg = m_stub_pdg[idx];
      }
    
      // For the sector efficiency we proceed as follow
      // We check if the track has let at least 4 stubs in 
      // 4 different layer/disk (nhits>=4). If yes, we compute 
      // the number of sectors containing at least 4 of those 
      // stubs (nsec)

      // First we get the number of different layers/disks hit by the primary
      for (int kk=0;kk<20;++kk)
      {
	if (n_per_lay[kk]!=0) ++nhits;
	nplay[kk]=n_per_lay[kk];
      }

      // Then we get the number of sectors containing more than 4 primary hits 
      for (int kk=0;kk<m_nsec;++kk)
      {
	if (is_sec_there[kk]>=4) ++nsec; 
      }

      part_nsec->at(k) = nsec;   
      part_nhits->at(k)= nhits;   

      // Finally we do the pattern/track loops

      ntotpatt = nb_patterns; // The total number of patterns in the event
      ttrack   = nb_tracks;   // The total number of tracks in the event

      npatt    = 0;           // The patterns containing at least 4 prim hits
      ntrack   = 0;           // The tracks containing at least 4 prim hits

      if (nb_patterns>0)
      {	  
	for(int kk=0;kk<nb_patterns;kk++)
	{
	  for (int j=0;j<20;++j) n_per_lay_patt[j]=0;
	  
	  // First we count the number of prim stubs in the pattern
	
	  if (m_pattlinks.at(kk).size()==0) continue;
	  
	  for(unsigned int m=0;m<m_pattlinks.at(kk).size();m++) // Loop over pattern stubs
	  {
	    if (m_stub_tp[m_pattlinks.at(kk).at(m)]==m_primaries.at(k).at(0)) // Stub belongs to the primary
	      n_per_lay_patt[m_stub_layer[m_pattlinks.at(kk).at(m)]-5]++;

	    if (stub_inpatt->at(m_pattlinks.at(kk).at(m))==0)
	    {
	      stub_inpatt->at(m_pattlinks.at(kk).at(m))=1;
	      ++n_stub;
	    }		    
	  }
	  
	  nhits_p=0; 
	  
	  for (int jk=0;jk<20;++jk)
	  {
	    if (n_per_lay_patt[jk]!=0) ++nhits_p;
	  }

	  if (nhits_p>=4) 
	  {
	    ++npatt; // More than 4, the pattern is good
	    patt_parts->at(kk).push_back(k); 
	  }
	}	  	  
      }

      if (nb_tracks>0)
      {	  
	for(int kk=0;kk<nb_tracks;kk++)
	{
	  for (int j=0;j<20;++j) n_per_lay_patt[j]=0;
	  
	  // First we count the number of prim stubs in the track
	
	  if (m_trklinks.at(kk).size()==0) continue;
	  
	  for(unsigned int m=0;m<m_trklinks.at(kk).size();m++) // Loop over track stubs
	  {
	    if (m_stub_tp[m_trklinks.at(kk).at(m)]==m_primaries.at(k).at(0)) // Stub belongs to the primary
	      n_per_lay_patt[m_stub_layer[m_trklinks.at(kk).at(m)]-5]++;

	    if (stub_intrk->at(m_trklinks.at(kk).at(m))==0)
	    {
	      stub_intrk->at(m_trklinks.at(kk).at(m))=1;
	      ++n_stub_t;
	    }		    
	  }
	  
	  nhits_p=0; 
	  
	  for (int jk=0;jk<20;++jk)
	  {
	    if (n_per_lay_patt[jk]!=0) ++nhits_p;
	  }

	  if (nhits_p>=4) 
	  {
	    ++ntrack; // More than 4, the track is good
	    trk_parts->at(kk).push_back(k); 
	  }
	}	  	  
      }
      
      part_npatt->at(k) = npatt; 
      part_ntrk->at(k)  = ntrack; 
    
      m_efftree->Fill();    
    }
     


    m_finaltree->Fill(); 
  }

  m_outfile->Write();
  m_outfile->Close();
}

/////////////////////////////////////////////////////////////////////////////////
//
// ==> track_eff::initTuple(std::string test,std::string out)
//
// This method opens the input  roottuple
//
/////////////////////////////////////////////////////////////////////////////////


void track_eff::initTuple(std::string test,std::string out)
{
  m_L1TT   = new TChain("TkStubs");   // Tree containing the stub info 
  m_PATT   = new TChain("L1tracks");  // Tree containing the L1 track reco info

  // Input data file 

  std::size_t found = test.find(".root");

  // Case 1, it's a root file
  if (found!=std::string::npos)
  {
    m_L1TT->Add(test.c_str());
    m_PATT->Add(test.c_str());
  }
  else // This is a list provided into a text file
  {
    std::string STRING;
    std::ifstream in(test.c_str());
    if (!in) 
    {
      std::cout << "Please provide a valid data filename list" << std::endl; 
      return;
    }    
  
    while (!in.eof()) 
    {
      getline(in,STRING);

      found = STRING.find(".root");
      if (found!=std::string::npos)
      {
	m_L1TT->Add(STRING.c_str());   
	m_PATT->Add(STRING.c_str());   
      }
    }

    in.close();
  }

  pm_stub_layer=&m_stub_layer;
  pm_stub_ladder=&m_stub_ladder;
  pm_stub_module=&m_stub_module;
  pm_stub_segment=&m_stub_segment;
  pm_stub_strip=&m_stub_strip;
  pm_stub_pxGEN=&m_stub_pxGEN;
  pm_stub_pyGEN=&m_stub_pyGEN;
  pm_stub_etaGEN=&m_stub_etaGEN;
  pm_stub_tp=&m_stub_tp;
  pm_stub_pdg=&m_stub_pdg;
  pm_stub_X0=&m_stub_X0;
  pm_stub_Y0=&m_stub_Y0;
  pm_stub_Z0=&m_stub_Z0;
  pm_stub_x=&m_stub_x;
  pm_stub_y=&m_stub_y;
  pm_stub_z=&m_stub_z;
  pm_clus_x=&m_clus_x;
  pm_clus_y=&m_clus_y;
  pm_clus_z=&m_clus_z;
  pm_stub_clust2=&m_stub_clust2;

  pm_pattlinks=&m_pattlinks;
  pm_pattsecid=&m_pattsecid;

  pm_trklinks=&m_trklinks;
  pm_trksecid=&m_trksecid;
  pm_trkpt=&m_trkpt;
  pm_trketa=&m_trketa;
  pm_trkphi=&m_trkphi;
  pm_trkz=&m_trkz;



  m_L1TT->SetBranchAddress("L1Tkevt",            &m_evtid); 
  m_L1TT->SetBranchAddress("L1TkSTUB_n",         &m_stub);
  m_L1TT->SetBranchAddress("L1TkSTUB_layer",     &pm_stub_layer);
  m_L1TT->SetBranchAddress("L1TkSTUB_ladder",    &pm_stub_ladder);
  m_L1TT->SetBranchAddress("L1TkSTUB_module",    &pm_stub_module);
  m_L1TT->SetBranchAddress("L1TkSTUB_seg",       &pm_stub_segment);
  m_L1TT->SetBranchAddress("L1TkSTUB_strip",     &pm_stub_strip);
  m_L1TT->SetBranchAddress("L1TkSTUB_pxGEN",     &pm_stub_pxGEN);
  m_L1TT->SetBranchAddress("L1TkSTUB_pyGEN",     &pm_stub_pyGEN);
  m_L1TT->SetBranchAddress("L1TkSTUB_X0",        &pm_stub_X0);
  m_L1TT->SetBranchAddress("L1TkSTUB_Y0",        &pm_stub_Y0);
  m_L1TT->SetBranchAddress("L1TkSTUB_Z0",        &pm_stub_Z0);
  m_L1TT->SetBranchAddress("L1TkSTUB_x",         &pm_stub_x);
  m_L1TT->SetBranchAddress("L1TkSTUB_y",         &pm_stub_y);
  m_L1TT->SetBranchAddress("L1TkSTUB_z",         &pm_stub_z);
  m_L1TT->SetBranchAddress("L1TkSTUB_etaGEN",    &pm_stub_etaGEN);
  m_L1TT->SetBranchAddress("L1TkSTUB_tp",        &pm_stub_tp);
  m_L1TT->SetBranchAddress("L1TkSTUB_pdgID",     &pm_stub_pdg);
  m_L1TT->SetBranchAddress("L1TkSTUB_clust2",    &pm_stub_clust2);
  m_L1TT->SetBranchAddress("L1TkCLUS_x",         &pm_clus_x);
  m_L1TT->SetBranchAddress("L1TkCLUS_y",         &pm_clus_y);
  m_L1TT->SetBranchAddress("L1TkCLUS_z",         &pm_clus_z);

  m_PATT->SetBranchAddress("L1PATT_n",           &nb_patterns);
  m_PATT->SetBranchAddress("L1PATT_links",       &pm_pattlinks);
  m_PATT->SetBranchAddress("L1PATT_secid",       &pm_pattsecid);

  m_PATT->SetBranchAddress("L1TRK_n",            &nb_tracks);
  m_PATT->SetBranchAddress("L1TRK_links",        &pm_trklinks);
  m_PATT->SetBranchAddress("L1TRK_secid",        &pm_trksecid);
  m_PATT->SetBranchAddress("L1TRK_pt",           &pm_trkpt);
  m_PATT->SetBranchAddress("L1TRK_eta",          &pm_trketa);
  m_PATT->SetBranchAddress("L1TRK_phi",          &pm_trkphi);
  m_PATT->SetBranchAddress("L1TRK_z",            &pm_trkz);


  // Output file definition (see the header)

  m_outfile = new TFile(out.c_str(),"recreate");
  m_efftree = new TTree("SectorEff","");

  m_efftree->Branch("event",      &evt,     "event/I"); 
  m_efftree->Branch("pdgID",      &pdg,     "pdgID/I"); 
  m_efftree->Branch("nsec",       &nsec,    "nsec/I"); 
  m_efftree->Branch("nhits",      &nhits,   "nhits/I"); 
  m_efftree->Branch("npatt",      &npatt,   "npatt/I"); 
  m_efftree->Branch("ntrack",     &ntrack,  "ntrack/I"); 
  m_efftree->Branch("tpatt",      &ntotpatt,"tpatt/I"); 
  m_efftree->Branch("ttrack",     &ttrack,  "ttrack/I"); 
  m_efftree->Branch("nplay",      &nplay,   "nplay[20]/I"); 
  m_efftree->Branch("pt",         &pt,      "pt/F"); 
  m_efftree->Branch("eta",        &eta,     "eta/F"); 
  m_efftree->Branch("phi",        &phi,     "phi/F"); 
  m_efftree->Branch("d0",         &d0,      "d0/F"); 
  m_efftree->Branch("mult",       &mult,    "mult[500]/I"); 

  m_finaltree = new TTree("FullInfo","");

  m_finaltree->Branch("evt",          &evt); 

  m_finaltree->Branch("n_stub_total", &n_stub_total); 
  m_finaltree->Branch("n_stub_inpat", &n_stub); 
  m_finaltree->Branch("n_stub_intrk", &n_stub_t); 
  m_finaltree->Branch("stub_x",       &stub_x); 
  m_finaltree->Branch("stub_y",       &stub_y); 
  m_finaltree->Branch("stub_z",       &stub_z); 
  m_finaltree->Branch("stub_layer",   &stub_layer); 
  m_finaltree->Branch("stub_ladder",  &stub_ladder);
  m_finaltree->Branch("stub_module",  &stub_module);
  m_finaltree->Branch("stub_segment", &stub_seg);
  m_finaltree->Branch("stub_strip",   &stub_strip);
  m_finaltree->Branch("stub_tp",      &stub_tp);
  m_finaltree->Branch("stub_inpatt",  &stub_inpatt);
  m_finaltree->Branch("stub_intrack", &stub_intrk);

  m_finaltree->Branch("n_part",       &n_part); 
  m_finaltree->Branch("part_pdg",     &part_pdg); 
  m_finaltree->Branch("part_nsec",    &part_nsec); 
  m_finaltree->Branch("part_nhits",   &part_nhits); 
  m_finaltree->Branch("part_npatt",   &part_npatt); 
  m_finaltree->Branch("part_ntrk",    &part_ntrk); 
  m_finaltree->Branch("part_pt",      &part_pt); 
  m_finaltree->Branch("part_rho",     &part_rho);
  m_finaltree->Branch("part_z0",      &part_z0);
  m_finaltree->Branch("part_eta",     &part_eta);  
  m_finaltree->Branch("part_phi",     &part_phi); 

  m_finaltree->Branch("n_patt",       &n_patt); 
  m_finaltree->Branch("patt_sec",     &patt_sec); 
  m_finaltree->Branch("patt_parts",   &patt_parts); 
  m_finaltree->Branch("patt_stubs",   &patt_stubs); 

  m_finaltree->Branch("n_track",      &n_track); 
  m_finaltree->Branch("trk_sec",      &trk_sec); 
  m_finaltree->Branch("trk_parts",    &trk_parts); 
  m_finaltree->Branch("trk_stubs",    &trk_stubs); 
  m_finaltree->Branch("trk_pt",       &trk_pt); 
  m_finaltree->Branch("trk_phi",      &trk_phi); 
  m_finaltree->Branch("trk_z",        &trk_z); 
  m_finaltree->Branch("trk_eta",      &trk_eta); 
}


/////////////////////////////////////////////////////////////////////////////////
//
// ==> track_eff::convert(std::string sectorfilename) 
//
// Here we retrieve info from the TKLayout CSV file containing the sector definition
//
// This file contains, for each sector, the ids of the modules contained in the sector
//
// The role of this method is to create the opposite, ie a vector containing, for every module the list of sectors belonging to it
//
/////////////////////////////////////////////////////////////////////////////////

bool track_eff::convert(std::string sectorfilename) 
{
  m_sec_mult = 0;

  std::vector<int> module;

  m_modules.clear();

  for (int i=0;i<230000;++i)
  {
    module.clear();
    module.push_back(-1);
    m_modules.push_back(module);
  }

  std::string STRING;
  std::ifstream in(sectorfilename.c_str());
  if (!in) 
  {
    std::cout << "Please provide a valid csv sector filename" << std::endl; 
    return false;
  }    
  
  int npar = 0;

  while (!in.eof()) 
  {
    ++m_sec_mult;
    if (m_sec_mult<=2) continue;

    getline(in,STRING);
    std::istringstream ss(STRING);
    npar = 0;
    while (ss)
    {
      std::string s;
      if (!getline( ss, s, ',' )) break;

      ++npar;
      if (npar<=2) continue;

      m_modules.at(atoi(s.c_str())).push_back(m_sec_mult-4);
    }
  }

  in.close();

  m_sec_mult -= 5;

  return true;
}


void track_eff::reset() 
{
  ntotpatt=0; 
  ttrack=0; 
  m_primaries.clear(); 

  n_stub_total=0; 
  n_stub=0; 
  n_stub_t=0;
 
  stub_x->clear();  
  stub_y->clear();   
  stub_z->clear();   
  stub_layer->clear();   
  stub_ladder->clear();  
  stub_module->clear();  
  stub_seg->clear();
  stub_strip->clear();    
  stub_tp->clear();  
  stub_inpatt->clear();  
  stub_intrk->clear();  
 
  n_part=0; 
  part_pdg->clear();   
  part_nsec->clear();   
  part_nhits->clear();   
  part_npatt->clear();   
  part_ntrk->clear();   
  part_pt->clear();   
  part_rho->clear();  
  part_z0->clear();  
  part_eta->clear();    
  part_phi->clear();   

  n_patt=0; 
  patt_sec->clear();   
  patt_parts->clear();   
  patt_stubs->clear(); 

  n_track=0;
  trk_parts->clear();   
  trk_stubs->clear();
  trk_sec->clear();   
  trk_pt->clear();   
  trk_phi->clear();   
  trk_z->clear();   
  trk_eta->clear();   
}
