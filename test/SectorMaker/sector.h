#ifndef SECTOR_H
#define SECTOR_H

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

///////////////////////////////////
//
//
// Base class for sector determination 
//
// filename : the name and directory of the input ROOT file containing the STUB rates
// outfile  : the name of the output ROOT file containing the sector definition along with the 
//            multiplicities (ie in how many sectors each module is?)
// neta     : the number of divisions in eta (default : 1)
// nphi     : the number of divisions in phi (default : 8)
// oeta     : the percentage of eta overlap between 2 sectors (default : 0)
// ophi     : the percentage of phi overlap between 2 sectors (default : 0)
//
// Info about the code:
//
//  http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.HLLHCTuto (STEP III)
//
//  Author: viret@in2p3_dot_fr
//  Date: 23/05/2013
//
///////////////////////////////////

using namespace std;

class sector
{
 public:
  sector(std::string filename,std::string outfile,
	 int neta,int nphi,int oeta,int ophi);

  void   do_sector();    
  void   initVars();
  void   initTuple(std::string in,std::string out);

  bool is_in_eta(float mod_max,float mod_min,float sec_min,float sec_max,float cov);
  bool is_in_phi(float mod_max,float mod_min,float sec_min,float sec_max,float cov);

 private:

  TFile *m_infile;
  TFile *m_outfile;
  TTree *m_ratetree;
  TTree *m_sectree;

  // Coding conventions for barrel and endcap module IDs
  
  // We define a barrel ID and an endcap ID as follows:

  // B_id = (layer-1)*10000 + ladder*100 + module (0 to 57523)

  // E_id = (disk-1)*10000 + ladder*100 + module (0 to 131377)

  // Disks 0 to 6  (towards positive Z)
  // Disks 7 to 13 (towards negative Z)


  // Following tables are for rates
  float m_b_rate[58000];     // Contains the barrel module rates (in stubs/bx)
  float m_b_rate_p[58000];   // Contains the barrel primary stubs module rates (in stubs/bx)
  float m_b_rate_s[58000];   // Contains the barrel secondary stubs module rates (in stubs/bx)
  float m_b_rate_f[58000];   // Contains the barrel fake stubs module rates (in stubs/bx)

  float m_e_rate[142000];    // Contains the endcap module rates (in stubs/bx)
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


  // The vector containing the barrel module lists of sectors 
  std::vector< std::vector<int> > *m_barrel_mod_tot; 
  // The vector containing the endcap module lists of sectors 
  std::vector< std::vector<int> > *m_endcap_mod_tot;
  // The vector containing eta/phi coordinates of sectors
  std::vector< std::vector<float> > *m_coords;
  // The vector containing the total module lists of sectors 
  std::vector< std::vector<int> > *m_sectors; 

  int m_counter_barrel[58000];  // Sector multiplicity of each barrel module
  int m_counter_endcap[142000]; // Sector multiplicity of each endcap module


  float m_eta_size;
  float m_phi_size;

  float m_eta_step;
  float m_phi_step; 

  float m_PI;

  int m_nphi;
  int m_neta;
  int m_ophi;
  int m_oeta;
  float m_cov; 
};

#endif

