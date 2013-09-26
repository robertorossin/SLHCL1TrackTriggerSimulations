#ifndef EFFICIENCIES_H
#define EFFICIENCIES_H


#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <cmath>



#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"

#include <fstream>
#include <string>

using namespace std;

///////////////////////////////////
//
//
// Base class for stub rates calculation (in stubs/module/BX)
//
// Input infos are :
//
// filename : the name and directory of the input ROOT file containing the STUB information
// outfile  : the name of the output ROOT file containing the rates 
//
// Info about the code:
//
//  http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.HLLHCTuto (STEP III)
//
//  Author: viret@in2p3_dot_fr
//  Date: 23/05/2013
//
///////////////////////////////////



class efficiencies
{
 public:

  efficiencies(std::string filename, std::string outfile);

  void  get_efficiencies();  // The main method  
  void  initVars();
  void  reset();
  void  initTuple(std::string in,std::string out);

 private:

  TChain *L1TT_O;      // The trees 
  TChain *L1TT_P;    
  TChain *Pix;      
  TChain *MC;      

  TFile *m_outfile;  // The output file
  TTree *m_tree; // The tree containing the efficiency information

  float pt_val[100];
  float eta_val[50];

  float digi_pt[20][100];
  float entries_pt[20][100];
  float digi_eta[20][50];
  float entries_eta[20][50];
  float entries_pt_lay[20][100];
  float entries_eta_lay[20][50];

  float clus_off_pt[20][100];
  float stub_off_pt[20][100];
  float clus_off_eta[20][50];
  float stub_off_eta[20][50];

  float clus_pri_pt[20][100];
  float stub_pri_pt[20][100];
  float clus_pri_eta[20][50];
  float stub_pri_eta[20][50];
  float stub_pri_pt_lay[20][100];
  float stub_pri_eta_lay[20][50];


  // Here are the parameters needed from the data
  // Details on these might be found on
  //
  // http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/HL_LHC/Extractors/RecoExtractor/interface/L1TrackTrigger_analysis.h?view=markup
  //


  int    	      m_part_n;
  int    	      m_part_nhit;
  int                 m_pclus;
  int                 m_clus;            
  int                 m_stub;  
  int                 m_tkclus;            
  int                 m_tkstub; 

  std::vector<int>    *m_pixclus_row;
  std::vector<int>    *m_pixclus_column;
  std::vector<int>    *m_pixclus_layer;
  std::vector<int>    *m_pixclus_module;
  std::vector<int>    *m_pixclus_ladder;
  std::vector<float>  *m_pixclus_x;
  std::vector<float>  *m_pixclus_y;

  std::vector<int>    *m_part_hits;
  std::vector<int>    *m_part_pdg;
  std::vector<float>  *m_part_px;
  std::vector<float>  *m_part_py;
  std::vector<float>  *m_part_eta;
  std::vector<float>  *m_part_x;
  std::vector<float>  *m_part_y;
  std::vector<float>  *m_hits_x;
  std::vector<float>  *m_hits_y;
  std::vector<int>    *m_hits_layer;
  std::vector<int>    *m_hits_ladder;
  std::vector<int>    *m_hits_module;

  std::vector<int>    *m_clus_nstrips;
  std::vector<int>    *m_clus_layer; 
  std::vector<int>    *m_clus_module;								       
  std::vector<int>    *m_clus_ladder;				      
  std::vector<int>    *m_clus_seg;   
  std::vector<float>  *m_clus_strip;
  std::vector<int>    *m_stub_layer;
  std::vector<int>    *m_stub_ladder;
  std::vector<int>    *m_stub_clust1;
  std::vector<int>    *m_stub_clust2;
  std::vector<int>    *m_stub_tp;

  std::vector<int>    *m_tkclus_nstrips;
  std::vector<int>    *m_tkclus_layer; 
  std::vector<int>    *m_tkclus_module;								       
  std::vector<int>    *m_tkclus_ladder;				      
  std::vector<int>    *m_tkclus_seg;   
  std::vector<float>  *m_tkclus_strip;
  std::vector<int>    *m_tkstub_layer;
  std::vector<int>    *m_tkstub_ladder;
  std::vector<int>    *m_tkstub_clust1;
  std::vector<int>    *m_tkstub_clust2;
  std::vector<int>    *m_tkstub_tp;

};

#endif

