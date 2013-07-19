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
// This code was developped for the BE classic geometry, but also works for 
// the 5 disks alternative
//
//  Author: viret@in2p3_dot_fr
//  Date: 23/05/2013
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
  float m_b_rate_s[58000];   // Contains the barrel secondary stubs module rates (in stubs/bx)
  float m_b_rate_f[58000];   // Contains the barrel fake stubs module rates (in stubs/bx)

  float m_e_rate[16][142000];// Contains the endcap chips module rates (in stubs/bx)
  float m_e_rate_p[142000];  // Contains the endcap primary stubs module rates (in stubs/bx)
  float m_e_rate_s[142000];  // Contains the endcap secondary stubs module rates (in stubs/bx)
  float m_e_rate_f[142000];  // Contains the endcap fake stubs module rates (in stubs/bx)

  // Following tables are for sector definition
  float m_b_etamin[58000];   // Contains the barrel module min eta value
  float m_b_etamax[58000];   // Contains the barrel module max eta value
  float m_b_phimin[58000];   // Contains the barrel module min phi value
  float m_b_phimax[58000];   // Contains the barrel module max phi value
  float m_e_etamin[142000];  // Contains the endcap module min eta value
  float m_e_etamax[142000];  // Contains the endcap module max eta value
  float m_e_phimin[142000];  // Contains the endcap module min phi value
  float m_e_phimax[142000];  // Contains the endcap module max phi value
  float m_e_stmin[142000];   // Minimum strip number 
  float m_e_stmax[142000];   // Maximum strip number 
  float m_e_segmin[142000];  // Minimum segment number  
  float m_e_segmax[142000];  // Maximum segment number   
  float m_e_nseg[142000];    // Number of segments in the module  
  float m_e_nstrip[142000];  // Number of strips in the module  
  float m_b_stmin[58000];    // 
  float m_b_stmax[58000];    //
  float m_b_segmin[58000];   // Idem for barrel
  float m_b_segmax[58000];   //
  float m_b_nseg[58000];     // 
  float m_b_nstrip[58000];   // 

  // Here are the parameters needed from the data
  // Details on these might be found on
  //
  // https://github.com/sviret/HL_LHC/blob/master/Extractors/RecoExtractor/interface/L1TrackTrigger_analysis.h
  //


  int m_clus;
  std::vector<float> m_clus_x;
  std::vector<float> m_clus_y;
  std::vector<float> m_clus_z;
  std::vector<int>   m_clus_nrows;
  std::vector<int>   m_clus_nseg;

  std::vector<float> *pm_clus_x;
  std::vector<float> *pm_clus_y;
  std::vector<float> *pm_clus_z;
  std::vector<int>   *pm_clus_nrows;
  std::vector<int>   *pm_clus_nseg;

  int m_stub;
  std::vector<int>   m_stub_layer;
  std::vector<int>   m_stub_ladder;
  std::vector<int>   m_stub_module;
  std::vector<int>   m_stub_tp;
  std::vector<float> m_stub_pxGEN;
  std::vector<float> m_stub_pyGEN;
  std::vector<float> m_stub_etaGEN;
  std::vector<float> m_stub_X0;
  std::vector<float> m_stub_Y0;
  std::vector<float> m_stub_x;
  std::vector<float> m_stub_y;
  std::vector<float> m_stub_z;
  std::vector<int>   m_stub_strip;
  std::vector<int>   m_stub_seg;
  std::vector<int>   m_stub_chip;
  std::vector<int>   m_stub_pdgID;
  std::vector<int>   m_stub_clust1;

  std::vector<int>   *pm_stub_layer;
  std::vector<int>   *pm_stub_ladder;
  std::vector<int>   *pm_stub_module;
  std::vector<int>   *pm_stub_tp;
  std::vector<float> *pm_stub_pxGEN;
  std::vector<float> *pm_stub_pyGEN;
  std::vector<float> *pm_stub_etaGEN;
  std::vector<float> *pm_stub_X0;
  std::vector<float> *pm_stub_Y0;
  std::vector<float> *pm_stub_x;
  std::vector<float> *pm_stub_y;
  std::vector<float> *pm_stub_z;
  std::vector<int>   *pm_stub_strip;
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

