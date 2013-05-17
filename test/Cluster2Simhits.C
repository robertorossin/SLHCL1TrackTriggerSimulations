/*
  Small ROOT macro showing an example of extracted data analysis.

  RECIPE 3: Matching simhits and clusters

  Use:

  root[1]-> .L Cluster2SimHits.C
  root[2]-> clus2simhit(filename,evt) 

  where filename is the name of the extracted ROOT file and evt the number of entry to analyze

  Information about this macro may be found in the following page:

  http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.HLLHCTuto (STEP III)

  Author: viret@in2p3_dot_fr
  Date: 16/05/2013

*/

void clus2simhit(std::string filename, int evt)
{
  gROOT->ProcessLine("#include <vector>");
  

  // First get the data
  // by merging all the available files

  TChain *MC              = new TChain("MC");  
  TChain *L1TT            = new TChain("L1TrackTrigger");  

  MC->Add(filename.c_str());
  L1TT->Add(filename.c_str());


//
//1/ MC TREE content:
//
// http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/HL_LHC/Extractors/RecoExtractor/interface/MCExtractor.h?view=markup
//

  int    		m_gen_n;    
  int    		m_part_n;    
  int                   m_part_nstracks;  
  int    		m_part_nhit;
 
  std::vector<float>    *m_gen_x;    
  std::vector<float>    *m_gen_y;    
  std::vector<float>    *m_gen_z;    
  std::vector<float> 	*m_gen_px;   
  std::vector<float> 	*m_gen_py;   
  std::vector<float>    *m_gen_pz;   
  std::vector<int>      *m_gen_pdg;  
  std::vector<int>      *m_part_pdgId;
  std::vector<float> 	*m_part_px;   
  std::vector<float> 	*m_part_py;   
  std::vector<float>	*m_part_pz;   
  std::vector<float>    *m_part_eta;  
  std::vector<float> 	*m_part_phi;  
  std::vector<float>    *m_part_x;    
  std::vector<float>    *m_part_y;    
  std::vector<float>    *m_part_z;    
  std::vector<int>      *m_st;        
  std::vector<int>      *m_hits;      
  std::vector<int>      *m_st_id;  
  std::vector<float>    *m_hits_x;    
  std::vector<float>    *m_hits_y;    
  std::vector<float>    *m_hits_z;    
  std::vector<float>    *m_hits_e;    
  std::vector<float>    *m_hits_tof;  
  std::vector<int>      *m_hits_proc; 
  std::vector<int>      *m_hits_id;   
  std::vector<int>      *m_hits_pdgId;
  std::vector<int>      *m_hits_layer;
  std::vector<int>      *m_hits_ladder;
  std::vector<int>      *m_hits_module;

  MC->SetBranchAddress("gen_n",            &m_gen_n);
  MC->SetBranchAddress("gen_pdg",          &m_gen_pdg);
  MC->SetBranchAddress("gen_px",           &m_gen_px);
  MC->SetBranchAddress("gen_py",           &m_gen_py);
  MC->SetBranchAddress("gen_pz",           &m_gen_pz);
  MC->SetBranchAddress("gen_x",            &m_gen_x);
  MC->SetBranchAddress("gen_y",            &m_gen_y);
  MC->SetBranchAddress("gen_z",            &m_gen_z);
  MC->SetBranchAddress("subpart_n",        &m_part_n);
  MC->SetBranchAddress("subpart_hits",     &m_hits);
  MC->SetBranchAddress("subpart_st",       &m_st);
  MC->SetBranchAddress("subpart_pdgId",    &m_part_pdgId);
  MC->SetBranchAddress("subpart_px",       &m_part_px);
  MC->SetBranchAddress("subpart_py",       &m_part_py);
  MC->SetBranchAddress("subpart_pz",       &m_part_pz);
  MC->SetBranchAddress("subpart_eta",      &m_part_eta);
  MC->SetBranchAddress("subpart_phi",      &m_part_phi);
  MC->SetBranchAddress("subpart_x",        &m_part_x);
  MC->SetBranchAddress("subpart_y",        &m_part_y);
  MC->SetBranchAddress("subpart_z",        &m_part_z);
  MC->SetBranchAddress("subpart_nhit",     &m_part_nhit);
  MC->SetBranchAddress("subpart_hits_x",   &m_hits_x);
  MC->SetBranchAddress("subpart_hits_y",   &m_hits_y);
  MC->SetBranchAddress("subpart_hits_z",   &m_hits_z);
  MC->SetBranchAddress("subpart_hits_e",   &m_hits_e);
  MC->SetBranchAddress("subpart_hits_tof", &m_hits_tof);
  MC->SetBranchAddress("subpart_hits_proc",&m_hits_proc);
  MC->SetBranchAddress("subpart_hits_id",  &m_hits_id);
  MC->SetBranchAddress("subpart_hits_pdgId",  &m_hits_pdgId);
  MC->SetBranchAddress("subpart_hits_layer",  &m_hits_layer);
  MC->SetBranchAddress("subpart_hits_ladder", &m_hits_ladder);
  MC->SetBranchAddress("subpart_hits_module", &m_hits_module);
  MC->SetBranchAddress("subpart_nsimtracks",  &m_part_nstracks);
  MC->SetBranchAddress("subpart_st_ids",      &m_st_id);



//
//2/ L1TrackTrigger TREE content:
//
// http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/HL_LHC/Extractors/RecoExtractor/interface/L1TrackTrigger_analysis.h?view=markup
//

  int m_clus;

  std::vector<float>  *m_clus_x;     
  std::vector<float>  *m_clus_y;     
  std::vector<float>  *m_clus_z;     
  std::vector<float>  *m_clus_xmc;   
  std::vector<float>  *m_clus_ymc;   
  std::vector<float>  *m_clus_zmc;   
  std::vector<int>    *m_clus_layer;  
  std::vector<int>    *m_clus_module; 
  std::vector<int>    *m_clus_ladder; 
  std::vector<int>    *m_clus_seg;    
  std::vector<float>  *m_clus_strip;  
  std::vector<int>    *m_clus_sat;    
  std::vector<int>    *m_clus_nstrips;
  std::vector<int>    *m_clus_matched;
  std::vector<int>    *m_clus_PS;     
  std::vector<int>    *m_clus_nrows;  
  std::vector< std::vector<int> >    *m_clus_tp;   
  std::vector< std::vector<int> >    *m_clus_hits; 
  std::vector<int>    *m_clus_pid; 

  L1TT->SetBranchAddress("CLUS_n",         &m_clus);
  L1TT->SetBranchAddress("CLUS_x",         &m_clus_x);
  L1TT->SetBranchAddress("CLUS_y",         &m_clus_y);
  L1TT->SetBranchAddress("CLUS_z",         &m_clus_z);
  L1TT->SetBranchAddress("CLUS_xmc",       &m_clus_xmc);
  L1TT->SetBranchAddress("CLUS_ymc",       &m_clus_ymc);
  L1TT->SetBranchAddress("CLUS_zmc",       &m_clus_zmc);
  L1TT->SetBranchAddress("CLUS_layer",     &m_clus_layer);
  L1TT->SetBranchAddress("CLUS_module",    &m_clus_module);
  L1TT->SetBranchAddress("CLUS_ladder",    &m_clus_ladder);
  L1TT->SetBranchAddress("CLUS_seg",       &m_clus_seg);
  L1TT->SetBranchAddress("CLUS_strip",     &m_clus_strip);
  L1TT->SetBranchAddress("CLUS_nstrip",    &m_clus_nstrips);
  L1TT->SetBranchAddress("CLUS_nsat",      &m_clus_sat);
  L1TT->SetBranchAddress("CLUS_match",     &m_clus_matched);
  L1TT->SetBranchAddress("CLUS_PS",        &m_clus_PS);
  L1TT->SetBranchAddress("CLUS_nrows",     &m_clus_nrows);
  L1TT->SetBranchAddress("CLUS_tp",        &m_clus_tp);
  L1TT->SetBranchAddress("CLUS_hits",      &m_clus_hits);
  L1TT->SetBranchAddress("CLUS_process",   &m_clus_pid);



  int n_entries = MC->GetEntries();

  if (evt>=n_entries) evt = n_entries-1;
  if (evt<0) evt = 0;

  
  MC->GetEntry(evt);
  L1TT->GetEntry(evt);

  cout <<endl;
  cout << "Analyzing event " << evt <<endl;
  cout << "where " << m_clus << " cluster(s) where produced" <<endl;

  // Loop over cluster

  for (int k=0;k<m_clus;++k)
  {
    cout << "-------------------------------------------------"  <<endl;
    cout << "Cluster " << k << " properties:"  <<endl;
    cout << " X/Y/Z (in cm)     : " << m_clus_x->at(k) 
	 << "/" << m_clus_y->at(k) 
	 << "/" << m_clus_z->at(k) << endl;
    cout << " Width (in strips) : " << m_clus_nstrips->at(k) << endl;

    if (m_clus_matched->at(k)!=0)  // The cluster is matched
    {
      cout << " X/Y/Z (MC) (in cm) : " << m_clus_xmc->at(k) 
	   << "/" << m_clus_ymc->at(k) 
	   << "/" << m_clus_zmc->at(k) << endl;

      cout << " Matched with " << m_clus_matched->at(k) << " simhit(s) (the same simhit could appear twice or more):" << endl;

      for (int l=0;l<m_clus_matched->at(k);++l)
      {
	cout << "    SimHit " << m_clus_hits->at(k).at(l) << " from TP " << m_clus_tp->at(k).at(l) << " : " 
	     << m_hits_x->at(m_clus_hits->at(k).at(l)) 
	     << "/" << m_hits_y->at(m_clus_hits->at(k).at(l)) 
	     << "/" << m_hits_z->at(m_clus_hits->at(k).at(l)) << endl; 
      }
    }
    else
    {
      cout << "Unmatched" << endl;
    }
  }
}
