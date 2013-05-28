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

  int id,nhits;

  cout << "Starting a test loop over " << nevt << " events..." << endl;

  const int m_nsec = m_sectors.size(); // How many sectors are in the file

  int is_sec_there[m_nsec];
  int ladder,module,layer;

  // Loop over the events
  for (int i=0;i<nevt;++i)
  { 
    nhits=0;
    nsec=0;

    eta=0.;
    pt=0.;
    phi=0.;

    for (int j=0;j<m_nsec;++j) is_sec_there[j]=1;

    m_L1TT->GetEntry(i);

    if (i%1000==0) cout << "Processed " << i << "/" << nevt << endl;

    if (m_stub == 0) continue; // No stubs, don't go further

    for (int j=0;j<m_stub;++j)
    {  
      // First of all we compute the ID of the stub's module

      if (m_stub_tp[j]!=0) continue; // Keep only the primary PGUN particle
      ++nhits;
      layer = m_stub_layer[j]; 
      ladder= m_stub_ladder[j]; 
      module= m_stub_module[j]; 

      pt=sqrt(m_stub_pxGEN[j]*m_stub_pxGEN[j]+m_stub_pyGEN[j]*m_stub_pyGEN[j]);
      eta=m_stub_etaGEN[j];
      phi=atan2(m_stub_pyGEN[j],m_stub_pxGEN[j]);

      id = 10000*layer+100*ladder+module; // Get the module ID
      
      // Loop over all sectors to see in which sectors the module is
      for (int k=0;k<m_nsec;++k)
      {
	if (!is_sec_there[k]) continue; // One of the previous stub was already out of sector k
	if (sector_test::in_sec(k,id))  continue; // The stub is in sector k
	
	is_sec_there[k]=0; // Stub is not in sector k, discard it
      }
    }

    // At the end of this loop, is_sec_there[k]==1 means that ALL the stubs of the 
    // particle are in modules belonging to sector k

    if (nhits<5) continue; 

    for (int k=0;k<m_nsec;++k) // Count in how many sectors the track is
    {
      if (is_sec_there[k]) ++nsec; 
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
  m_testfile = TFile::Open(test.c_str());

  m_sectree  = (TTree*)m_infile->Get("Sectors");
  m_L1TT     = (TTree*)m_testfile->Get("L1TrackTrigger");

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

  m_efftree->Branch("nsec",       &nsec,  "nsec/I"); 
  m_efftree->Branch("pt",         &pt,    "pt/F"); 
  m_efftree->Branch("eta",        &eta,   "eta/F"); 
  m_efftree->Branch("phi",        &phi,   "phi/F"); 
}
