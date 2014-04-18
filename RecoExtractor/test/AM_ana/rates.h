#ifndef RATES_H
#define RATES_H


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
#include <sstream> 

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
//  http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.HLLHCTuto620 (Part 3.2.2)
// 
// This code was developped for the BE classic geometry, but also works for 
// the 5 disks alternative
//
//  Author: viret@in2p3_dot_fr
//
///////////////////////////////////



class rates
{
 public:

  rates(std::string filename, std::string outfile);

  void  get_rates();  // The main method  
  void  initVars();
  void  initTuple(std::string in,std::string out);

 private:

  TChain *L1TT;      // The trees containing the input data

  TFile *m_outfile;  // The output file
  TTree *m_ratetree; // The tree containing the rate information
  TTree *m_dbgtree;  // Debug tree 

  // Coding conventions for barrel and endcap module IDs
  
  // We define a barrel ID and an endcap ID as follows:

  // B_id = (layer-1)*10000 + ladder*100 + module (0 to 57523)

  // E_id = (disk-1)*10000 + ladder*100 + module (0 to 131377)

  // Disks 0 to 6  (towards positive Z)  (0 to  4 for 5D)
  // Disks 7 to 13 (towards negative Z)  (7 to 11 for 5D)


  // Following tables are for rates
  float m_b_rate[16][58000]; // Contains the barrel chips module rates (in stubs/bx)
  float m_b_rate_p[58000];   // Contains the barrel primary stubs module rates (in stubs/bx)
  float m_b_rate_pp[58000];  // Contains the barrel primary stubs module rates (in stubs/bx)
  float m_b_rate_s[58000];   // Contains the barrel secondary stubs module rates (in stubs/bx)
  float m_b_rate_f[58000];   // Contains the barrel fake stubs module rates (in stubs/bx)
  float m_b_crate[58000];    // Contains the barrel cluster module rates (in stubs/bx)
  float m_b_bylc_rate[600];  // Contains the barrel clus ladder rates (in stubs/bx)
  float m_b_byls_rate[600];  // Contains the barrel stub ladder rates (in stubs/bx)
 
  float m_e_rate[16][142000];// Contains the endcap chips module rates (in stubs/bx)
  float m_e_rate_p[142000];  // Contains the endcap primary stubs module rates (in stubs/bx)
  float m_e_rate_pp[142000]; // Contains the endcap primary stubs module rates (in stubs/bx)
  float m_e_rate_s[142000];  // Contains the endcap secondary stubs module rates (in stubs/bx)
  float m_e_rate_f[142000];  // Contains the endcap fake stubs module rates (in stubs/bx)
  float m_e_crate[142000];   // Contains the barrel cluster module rates (in stubs/bx)
  float m_e_bylc_rate[1500]; // Contains the barrel clus ladder rates (in stubs/bx)
  float m_e_byls_rate[1500]; // Contains the barrel stub ladder rates (in stubs/bx)

  // Here are the parameters needed from the data
  // Details on these might be found on
  //
  // https://github.com/sviret/HL_LHC/blob/master/Extractors/RecoExtractor/interface/L1TrackTrigger_analysis.h
  //


  int m_clus;
  std::vector<int>   m_clus_layer;
  std::vector<int>   m_clus_ladder;
  std::vector<int>   m_clus_module;
  std::vector<int>   m_clus_nrows;
  std::vector<int>   m_clus_nseg;

  std::vector<int>   *pm_clus_layer;
  std::vector<int>   *pm_clus_ladder;
  std::vector<int>   *pm_clus_module;
  std::vector<int>   *pm_clus_nrows;
  std::vector<int>   *pm_clus_nseg;

  int m_stub;
  std::vector<int>   m_stub_layer;
  std::vector<int>   m_stub_ladder;
  std::vector<int>   m_stub_module;
  std::vector<int>   m_stub_tp;
  std::vector<float> m_stub_pt;
  std::vector<float> m_stub_pxGEN;
  std::vector<float> m_stub_pyGEN;
  std::vector<float> m_stub_etaGEN;
  std::vector<float> m_stub_X0;
  std::vector<float> m_stub_Y0;
  std::vector<int>   m_stub_seg;
  std::vector<int>   m_stub_chip;
  std::vector<int>   m_stub_pdgID;
  std::vector<int>   m_stub_clust1;

  std::vector<int>   *pm_stub_layer;
  std::vector<int>   *pm_stub_ladder;
  std::vector<int>   *pm_stub_module;
  std::vector<int>   *pm_stub_tp;
  std::vector<float> *pm_stub_pt;
  std::vector<float> *pm_stub_pxGEN;
  std::vector<float> *pm_stub_pyGEN;
  std::vector<float> *pm_stub_etaGEN;
  std::vector<float> *pm_stub_X0;
  std::vector<float> *pm_stub_Y0;
  std::vector<int>   *pm_stub_seg;
  std::vector<int>   *pm_stub_chip;
  std::vector<int>   *pm_stub_pdgID;
  std::vector<int>   *pm_stub_clust1;


  // Some parameters for the debug tree

  int m_disk;
  int m_lay;
  int m_lad;
  int m_mod;
  int m_sen;
  int m_chp;
  float m_rate;

};

#endif

