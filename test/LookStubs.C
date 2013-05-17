/*
  Small ROOT macro showing an example of extracted data analysis.

  RECIPE 4: Looking at stubs

  Use:

  root[1]-> .L LookStubs.C
  root[2]-> stubs(filename,evt) 

  where filename is the name of the extracted ROOT file and evt the number of entry to analyze

  Information about this macro may be found in the following page:

  http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.HLLHCTuto (STEP III)

  Author: viret@in2p3_dot_fr
  Date: 16/05/2013

*/

void stubs(std::string filename, int evt)
{
  gROOT->ProcessLine("#include <vector>");
  

  // First get the data
  // by merging all the available files

  TChain *L1TT            = new TChain("L1TrackTrigger");  

  L1TT->Add(filename.c_str());

//
//1/ L1TrackTrigger TREE content:
//
// http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/HL_LHC/Extractors/RecoExtractor/interface/L1TrackTrigger_analysis.h?view=markup
//


  int m_stub;

  std::vector<float>  *m_stub_pt;     
  std::vector<float>  *m_stub_ptMC;   
  std::vector<float>  *m_stub_pxGEN;  
  std::vector<float>  *m_stub_pyGEN;  
  std::vector<float>  *m_stub_etaGEN; 
  std::vector<float>  *m_stub_X0;     
  std::vector<float>  *m_stub_Y0;     
  std::vector<float>  *m_stub_Z0;     
  std::vector<float>  *m_stub_PHI0;   
  std::vector<int>    *m_stub_layer;  
  std::vector<int>    *m_stub_module; 
  std::vector<int>    *m_stub_ladder; 
  std::vector<int>    *m_stub_seg;    
  std::vector<int>    *m_stub_strip;  
  std::vector<float>  *m_stub_x;      
  std::vector<float>  *m_stub_y;      
  std::vector<float>  *m_stub_z;      
  std::vector<int>    *m_stub_clust1; 
  std::vector<int>    *m_stub_clust2; 
  std::vector<int>    *m_stub_cw1;    
  std::vector<int>    *m_stub_cw2;    
  std::vector<float>  *m_stub_deltas; 
  std::vector<float>  *m_stub_cor;    
  std::vector<int>    *m_stub_tp;     
  std::vector<int>    *m_stub_pdg;    
  std::vector<int>    *m_stub_pid;    

  L1TT->SetBranchAddress("STUB_ptMC",      &m_stub_ptMC);
  L1TT->SetBranchAddress("STUB_clust1",    &m_stub_clust1);
  L1TT->SetBranchAddress("STUB_clust2",    &m_stub_clust2);
  L1TT->SetBranchAddress("STUB_cw1",       &m_stub_cw1);
  L1TT->SetBranchAddress("STUB_cw2",       &m_stub_cw2);
  L1TT->SetBranchAddress("STUB_cor",       &m_stub_cor);
  L1TT->SetBranchAddress("STUB_PHI0",      &m_stub_PHI0);
  L1TT->SetBranchAddress("STUB_tp",        &m_stub_tp);
  L1TT->SetBranchAddress("STUB_pdgID",     &m_stub_pdg);
  L1TT->SetBranchAddress("STUB_process",   &m_stub_pid);  
  L1TT->SetBranchAddress("STUB_n",         &m_stub);
  L1TT->SetBranchAddress("STUB_pt",        &m_stub_pt);
  L1TT->SetBranchAddress("STUB_pxGEN",     &m_stub_pxGEN);
  L1TT->SetBranchAddress("STUB_pyGEN",     &m_stub_pyGEN);
  L1TT->SetBranchAddress("STUB_etaGEN",    &m_stub_etaGEN);
  L1TT->SetBranchAddress("STUB_layer",     &m_stub_layer);
  L1TT->SetBranchAddress("STUB_module",    &m_stub_module);
  L1TT->SetBranchAddress("STUB_ladder",    &m_stub_ladder);
  L1TT->SetBranchAddress("STUB_seg",       &m_stub_seg);
  L1TT->SetBranchAddress("STUB_strip",     &m_stub_strip);
  L1TT->SetBranchAddress("STUB_x",         &m_stub_x);
  L1TT->SetBranchAddress("STUB_y",         &m_stub_y);
  L1TT->SetBranchAddress("STUB_z",         &m_stub_z);
  L1TT->SetBranchAddress("STUB_deltas",    &m_stub_deltas);
  L1TT->SetBranchAddress("STUB_X0",        &m_stub_X0);
  L1TT->SetBranchAddress("STUB_Y0",        &m_stub_Y0);
  L1TT->SetBranchAddress("STUB_Z0",        &m_stub_Z0);

  int n_entries = L1TT->GetEntries();

  if (evt>=n_entries) evt = n_entries-1;
  if (evt<0) evt = 0;


  L1TT->GetEntry(evt);

  cout <<endl;
  cout << "Analyzing event " << evt <<endl;
  cout << "where " << m_stub << " stub(s) where produced" <<endl;

  // Loop over stubs

  for (int k=0;k<m_stub;++k)
  {
    cout << "-------------------------------------------------"  <<endl;
    cout << "Stub " << k << " properties:"  <<endl;
    cout << " X/Y/Z (in cm)       : " << m_stub_x->at(k) 
	 << "/" << m_stub_y->at(k) 
	 << "/" << m_stub_z->at(k) << endl;
    cout << "Bottom cluster / CW  : " << m_stub_clust1->at(k) << " / " << m_stub_cw1->at(k) << endl;
    cout << "Top    cluster / CW  : " << m_stub_clust2->at(k) << " / " << m_stub_cw2->at(k) << endl;
    cout << "Stub corrected width : " << m_stub_deltas->at(k) << endl;

    if (m_stub_tp->at(k)>=0)  // The cluster is matched
    {
      cout << "    Matched with TP   : " << m_stub_tp->at(k) << endl;
      cout << "    PT/PTmc/PTgen     : " << m_stub_pt->at(k) 
	   << "/" << m_stub_ptMC->at(k) 
	   << "/" << sqrt(m_stub_pxGEN->at(k)*m_stub_pxGEN->at(k)+m_stub_pyGEN->at(k)*m_stub_pyGEN->at(k)) 
	   << endl;
      
      cout << "    TP origin (X/Y/Z) : " << m_stub_X0->at(k) << " / " 
	   << m_stub_Y0->at(k) << " / "  
	   << m_stub_Z0->at(k) << endl; 
      cout << "    TP pdg code       : " << m_stub_pdg->at(k) << endl; 
    }
    else
    {
      cout << "Unmatched" << endl;
    }
  }
}
