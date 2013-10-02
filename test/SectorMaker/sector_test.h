#ifndef SECTOR_TEST_H
#define SECTOR_TEST_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <cmath>
#include <stdio.h>  
#include <stdlib.h> 

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"

#include <fstream>
#include <string>
#include <sstream> 

///////////////////////////////////
//
//
// Base class for sector efficiency determination 
//
// The role of this class is to check that all particles are contained in at least one 
// trigger sector, to count the stub multiplicities per sector, and finally to 
// compute the pattern bank efficiency
//
// filename    : the name and directory of the input ROOT file containing the particle to test
// secfilename : the name and directory of the input ROOT file containing the sectors definition
// pattfilename: the name and directory of the input ROOT file containing the pattern reco output
// outfile     : the name of the output ROOT file containing the efficiency results 
//           
// nevt        : the number of particles to test
// dbg         : debug mode (true if the pattern file comes from the standalone preco, false otherwise) 
//
// Info about the code:
//
//  http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.HLLHCTuto (STEP III for the efficiency 
//                                                                test without pattern file. 
//                                                                STEP V otherwise)
//
//  Author: viret@in2p3_dot_fr
//  Date: 28/05/2013
//  Maj.update: 16/09/2013
//
///////////////////////////////////

using namespace std;

class sector_test
{
 public:

  sector_test(std::string filename, std::string secfilename, 
	      std::string pattfilename, std::string outfile, int nevt, bool dbg);

  void   do_test(int nevt);    

  void   translateTuple(std::string pattin,std::string pattout, bool dbg);
  void   initTuple(std::string test,std::string patt,std::string out);
  bool   convert(std::string sectorfilename); 
  void   reset();
    
 private:

  bool do_patt;
  bool m_dbg;

  TFile  *m_infile;
  TFile  *m_testfile;
  TFile  *m_outfile;
  TFile  *m_pattfile;
  TChain *m_L1TT;
  TChain *data;

  TTree  *m_efftree;
  TTree  *m_finaltree;
  TTree  *m_PATT;

  // List of stubs from primaries (one vector of stub ids per prim track) 
  // for a given event 

  std::vector< std::vector<int> >   m_primaries;


  // Coding conventions for barrel and endcap module IDs
  
  // We define a barrel ID and an endcap ID as follows:

  // B_id = (layer-1)*10000 + ladder*100 + module (0 to 57523)

  // E_id = (disk-1)*10000 + ladder*100 + module (0 to 131377)

  // Disks 0 to 6  (towards positive Z)
  // Disks 7 to 13 (towards negative Z)

  int m_sec_mult;
  int evtIDmax;

  std::vector< std::vector<int> >   m_modules; 


  int m_evtid;
  int m_stub;
  std::vector<int>   m_stub_layer;
  std::vector<int>   m_stub_ladder;
  std::vector<int>   m_stub_module;
  std::vector<int>   m_stub_tp;
  std::vector<int>   m_stub_pdg;
  std::vector<float> m_stub_pxGEN;
  std::vector<float> m_stub_pyGEN;
  std::vector<float> m_stub_etaGEN;
  std::vector<float> m_stub_X0;
  std::vector<float> m_stub_Y0;
  std::vector<float> m_stub_Z0;
  std::vector<float> m_stub_x;
  std::vector<float> m_stub_y;
  std::vector<float> m_stub_z;

  std::vector<int>   *pm_stub_layer;
  std::vector<int>   *pm_stub_ladder;
  std::vector<int>   *pm_stub_module;
  std::vector<int>   *pm_stub_tp;
  std::vector<int>   *pm_stub_pdg;
  std::vector<float> *pm_stub_pxGEN;
  std::vector<float> *pm_stub_pyGEN;
  std::vector<float> *pm_stub_etaGEN;
  std::vector<float> *pm_stub_X0;
  std::vector<float> *pm_stub_Y0;
  std::vector<float> *pm_stub_Z0;
  std::vector<float> *pm_stub_x;
  std::vector<float> *pm_stub_y;
  std::vector<float> *pm_stub_z;

  // The output tree has one entry per primary track. 

  int   evt;        // Event number (for PU event, where there is more than 1 primary)
  int   pdg;        // The pdg ID of the particle
  int   nsec;       // The number of sectors containing at least 5 stubs of the prim. track
  float pt;         // The pT of the prim. track
  float eta;        // The eta of the prim. track
  float phi;        // The phi of the prim. track
  float d0;         // The origin radius
  int   npatt;      // The number of patterns containing at least 5 stubs of the prim. track
  int   ntotpatt;   // The total number of patterns 
  int   mult[500];  // The total number of stubs per sector 
  int   nhits;      // The total number of layers/disks hit by the prim track
  int   nplay[20];  // The total number of prim stubs per layer 


  int n_stub_total;                  // The total number of stubs in the event
  int n_stub;                        // The total number of stubs contained in matched patterns in the event

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
  std::vector<int>                  *patt_sec;   // Sector id of all the patterns
  std::vector< std::vector<int> >   *patt_parts; // tp index of ALL the particles contained in the pattern (in part_*** vectors of this tree!!!!)
  std::vector< std::vector<int> >   *patt_stubs; // index of ALL the stubs contained in the pattern (in stub_*** vectors of this tree!!!!) 


  
  // Informations contained in the pattern file

  int nb_patterns; // Number of patterns in event evtID
  int event_id;    // Event ID


  // Info id dbg==false (CMSSW fast mode)

  std::vector< std::vector<int> > m_links; // Links to the stub ids of the pattern 
  std::vector<int>                m_secid; // Link to the sector ids of the pattern 

  std::vector< std::vector<int> > *pm_links; 
  std::vector<int>                *pm_secid;


  // Info id dbg==true (Standalone mode)

  static const int MAX_NB_PATTERNS = 1500;
  static const int MAX_NB_HITS     = 100;

  int   pattern_sector_id[MAX_NB_PATTERNS];  
  int   nbHitPerPattern[MAX_NB_PATTERNS];
  short hit_layer[MAX_NB_PATTERNS*MAX_NB_HITS];
  int   hit_tp[MAX_NB_PATTERNS*MAX_NB_HITS];

};

#endif

