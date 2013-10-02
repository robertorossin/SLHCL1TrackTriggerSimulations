#include <vector>
#ifdef __MAKECINT__
#pragma link C++ class vector<vector<int> >+;
#pragma link C++ class vector<vector<float> >+;
#endif

/*
  Small ROOT macro showing how to analyze the PR output.

  Use:

  root[1]-> .L PR_output.C
  root[2]-> am_result(filename,evtnum) 

  where filename is the name of the extracted ROOT file and evtnum the number of entry to analyze

  Information about this macro may be found in the following page:

  http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.HLLHCTuto (STEP V)

  Author: viret@in2p3_dot_fr
  Date: 02/10/2013

*/

void am_result(std::string filename, int evtnum)
{
  // First get the data
  // by merging all the available files

  TChain *L1TT            = new TChain("FullInfo");  

  L1TT->Add(filename.c_str());

//
//1/ FullInfo TREE content:
//
// https://github.com/sviret/HL_LHC/blob/master/Extractors/RecoExtractor/test/SectorMaker/sector_test.h
//


  int evt;             // Event number (for PU event, where there is more than 1 primary)
  int n_stub_total;    // The total number of stubs in the event
  int n_stub;          // The total number of stubs contained in matched patterns in the event

  std::vector<float>   *stub_x;      // x coordinates of ALL the stubs
  std::vector<float>   *stub_y;      // y coordinates of ALL the stubs
  std::vector<float>   *stub_z;      // z coordinates of ALL the stubs
  std::vector<int>     *stub_layer;  // layer number of ALL the stubs
  std::vector<int>     *stub_ladder; // ladder number of ALL the stubs
  std::vector<int>     *stub_module; // module number of ALL the stubs
  std::vector<int>     *stub_tp;     // tp index of ALL the stubs (in part_*** vectors of this tree!!!!)
  std::vector<int>     *stub_inpatt; // is the stub in a pattern (1) of not (0)?

  int n_part;                        // The total number of particles inducing at least one stub in the event

  std::vector<int>     *part_pdg;    // PDG id of the particles
  std::vector<int>     *part_nsec;   // In how many trigger towers this particle hit more than 4 different layers/disks?
  std::vector<int>     *part_nhits;  // How many different layers/disks are hit by the particle?
  std::vector<int>     *part_npatt;  // How many patterns contains more than 4 stubs of the particle (in 4 different layers/disks)?
  std::vector<float>   *part_pt;     // pt of the particles
  std::vector<float>   *part_rho;    // rho0 of the particles
  std::vector<float>   *part_z0;     // z0 of the particles
  std::vector<float>   *part_eta;    // eta of the particles 
  std::vector<float>   *part_phi;    // phi of the particles

  int n_patt;                        // The total number of patterns matched in the event

  // Sector id of all the patterns
  std::vector<int>                  *patt_sec;   

  // tp index of ALL the particles contained in the pattern (in part_*** vectors of this tree!!!!)
  std::vector< std::vector<int> >   *patt_parts; 

  // index of ALL the stubs contained in the pattern (in stub_*** vectors of this tree!!!!) 
  std::vector< std::vector<int> >   *patt_stubs; 

  L1TT->SetBranchAddress("evt",          &evt); 

  L1TT->SetBranchAddress("n_stub_total", &n_stub_total); 
  L1TT->SetBranchAddress("n_stub_inpat", &n_stub); 
  L1TT->SetBranchAddress("stub_x",       &stub_x); 
  L1TT->SetBranchAddress("stub_y",       &stub_y); 
  L1TT->SetBranchAddress("stub_z",       &stub_z); 
  L1TT->SetBranchAddress("stub_layer",   &stub_layer); 
  L1TT->SetBranchAddress("stub_ladder",  &stub_ladder);
  L1TT->SetBranchAddress("stub_module",  &stub_module);
  L1TT->SetBranchAddress("stub_tp",      &stub_tp);
  L1TT->SetBranchAddress("stub_inpatt",  &stub_inpatt);

  L1TT->SetBranchAddress("n_part",       &n_part); 
  L1TT->SetBranchAddress("part_pdg",     &part_pdg); 
  L1TT->SetBranchAddress("part_nsec",    &part_nsec); 
  L1TT->SetBranchAddress("part_nhits",   &part_nhits); 
  L1TT->SetBranchAddress("part_npatt",   &part_npatt); 
  L1TT->SetBranchAddress("part_pt",      &part_pt); 
  L1TT->SetBranchAddress("part_rho",     &part_rho);
  L1TT->SetBranchAddress("part_z0",      &part_z0);
  L1TT->SetBranchAddress("part_eta",     &part_eta);  
  L1TT->SetBranchAddress("part_phi",     &part_phi); 

  L1TT->SetBranchAddress("n_patt",       &n_patt); 
  L1TT->SetBranchAddress("patt_sec",     &patt_sec); 
  L1TT->SetBranchAddress("patt_parts",   &patt_parts); 
  L1TT->SetBranchAddress("patt_stubs",   &patt_stubs); 

  int n_entries = L1TT->GetEntries();

  if (evtnum>=n_entries) evtnum = n_entries-1;
  if (evtnum<0) evtnum = 0;


  L1TT->GetEntry(evtnum);

  cout <<endl;
  cout << "Analyzing event " << evt <<endl;
  cout << "where " << n_stub_total << " stub(s) were produced" <<endl;
  cout << n_part << " particle(s) have induced at least one stub in the tracker" <<endl;
  cout << "      " << n_stub << " stub(s) are contained in the " << n_patt << " pattern(s) matched in this event" <<endl;
  cout << endl;
  cout << "Now looping over the patterns... " <<endl;
  cout << endl;

  // Loop over patterns

  int idx_s;
  int idx_p;

  for (int k=0;k<n_patt;++k)
  {
    cout << "-------------------------------------------------"  <<endl;
    cout << "Pattern " << k+1 << " properties:"  <<endl;
    cout << "=> Sector id : " << patt_sec->at(k) <<endl;
    cout << "=> Number of stubs : " << patt_stubs->at(k).size() <<endl;
    cout << "=> Number of particles w/more than four stubs in the pattern : " << patt_parts->at(k).size() <<endl;

    if (patt_parts->at(k).size()==0)
    {
      cout << "!! FAKE PATTERN containing the following stubs: " <<endl;

      for (int kk=0;kk<patt_stubs->at(k).size();++kk)
      {
	idx_s = patt_stubs->at(k).at(kk);
	idx_p = stub_tp->at(idx_s);

	cout << " Stub " << kk+1 << endl;  
	cout << " X/Y/Z (in cm)       : " << stub_x->at(idx_s) 
	     << "/" << stub_y->at(idx_s) 
	     << "/" << stub_z->at(idx_s) << endl;

	if (stub_tp->at(idx_s)>=0)  // The cluster is matched
	{
	  cout << "    Matched with PART   : " << idx_p << endl;
	  cout << "    PTgen     : " << part_pt->at(idx_p) 
	       << endl;
	  
	  cout << "    PART origin (R/Z) : " << part_rho->at(idx_p) << " / " 
	       << part_z0->at(idx_p) << endl; 
	  cout << "    PART pdg code       : " << part_pdg->at(idx_p) << endl; 
	}
	else
	{
	  cout << "Unmatched" << endl;
	}
      }
    }
    else
    {
      for (int jj=0;jj<patt_parts->at(k).size();++jj)
      {
	cout << "Particle: " << jj+1 <<endl;
      }

      cout << "This patterns contains the following stubs: " <<endl;

      for (int kk=0;kk<patt_stubs->at(k).size();++kk)
      {
	idx_s = patt_stubs->at(k).at(kk);
	idx_p = stub_tp->at(idx_s);

	cout << " Stub " << kk+1 << endl;  
	cout << " X/Y/Z (in cm)       : " << stub_x->at(idx_s) 
	     << "/" << stub_y->at(idx_s) 
	     << "/" << stub_z->at(idx_s) << endl;

	if (stub_tp->at(idx_s)>=0)  // The cluster is matched
	{
	  cout << "    Matched with PART   : " << idx_p << endl;
	  cout << "    PTgen     : " << part_pt->at(idx_p) 
	       << endl;
	  
	  cout << "    PART origin (R/Z) : " << part_rho->at(idx_p) << " / " 
	       << part_z0->at(idx_p) << endl; 
	  cout << "    PART pdg code       : " << part_pdg->at(idx_p) << endl; 
	}
	else
	{
	  cout << "Unmatched" << endl;
	}
      }
    }
  }
}
