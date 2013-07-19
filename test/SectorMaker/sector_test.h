#ifndef SECTOR_TEST_H
#define SECTOR_TEST_H

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
// Base class for sector efficiency determination 
//
// filename    : the name and directory of the input ROOT file containing the particle to test
// secfilename : the name and directory of the input ROOT file containing the sectors definition
// outfile     : the name of the output ROOT file containing the efficiency results 
//           
// nevt        : the number of particles to test
//
// Info about the code:
//
//  http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.HLLHCTuto (STEP III)
//
//  Author: viret@in2p3_dot_fr
//  Date: 28/05/2013
//
///////////////////////////////////

using namespace std;

class sector_test
{
 public:

  sector_test(std::string filename, std::string secfilename, 
	      std::string outfile, int nevt);

  void   do_test(int nevt);    
  void   initTuple(std::string test,std::string sec,std::string out);

  bool   in_sec(int sec,int mod);



 private:

  TFile *m_infile;
  TFile *m_testfile;
  TFile *m_outfile;
  TFile *m_pattfile;
  TChain *m_L1TT;
  TTree *m_sectree;
  TTree *m_efftree;
  TTree *m_PATT;

  // Coding conventions for barrel and endcap module IDs
  
  // We define a barrel ID and an endcap ID as follows:

  // B_id = (layer-1)*10000 + ladder*100 + module (0 to 57523)

  // E_id = (disk-1)*10000 + ladder*100 + module (0 to 131377)

  // Disks 0 to 6  (towards positive Z)
  // Disks 7 to 13 (towards negative Z)

  // The vector containing the total module lists of sectors 
  std::vector< std::vector<int> >   m_sectors; 
  std::vector< std::vector<int> >   *pm_sectors; 

  int m_stub;
  std::vector<int>   m_stub_layer;
  std::vector<int>   m_stub_ladder;
  std::vector<int>   m_stub_module;
  std::vector<int>   m_stub_tp;
  std::vector<float> m_stub_pxGEN;
  std::vector<float> m_stub_pyGEN;
  std::vector<float> m_stub_etaGEN;

  std::vector<int>   *pm_stub_layer;
  std::vector<int>   *pm_stub_ladder;
  std::vector<int>   *pm_stub_module;
  std::vector<int>   *pm_stub_tp;
  std::vector<float> *pm_stub_pxGEN;
  std::vector<float> *pm_stub_pyGEN;
  std::vector<float> *pm_stub_etaGEN;


  // The output tree has one entry per primary track. 

  int   nsec;  // The number of sectors containing ALL the stubs of the track
  float pt;    // The pT of the track
  float eta;   // The eta of the track
  float phi;   // The phi of the track
  int npatt;   // The number of patterns containing the full track
  int ntotpatt;// The total number of patterns 
  int   mult[500];   // 
  int   nhits;


  static const int MAX_NB_PATTERNS=1500;
  static const int MAX_NB_HITS = 100;

  int nb_layers;
  int nb_patterns;
  int event_id;
  int pattern_sector_id[MAX_NB_PATTERNS];
  
  int nbHitPerPattern[MAX_NB_PATTERNS];
  short hit_layer[MAX_NB_PATTERNS*MAX_NB_HITS];
  short hit_ladder[MAX_NB_PATTERNS*MAX_NB_HITS];
  short hit_zPos[MAX_NB_PATTERNS*MAX_NB_HITS];
  short hit_segment[MAX_NB_PATTERNS*MAX_NB_HITS];
  short hit_strip[MAX_NB_PATTERNS*MAX_NB_HITS];
  int hit_tp[MAX_NB_PATTERNS*MAX_NB_HITS];
  float hit_ptGEN[MAX_NB_PATTERNS*MAX_NB_HITS];
};

#endif

