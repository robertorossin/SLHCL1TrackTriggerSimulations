#ifndef PATTERNGEN_H
#define PATTERNGEN_H


#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <cmath>

#include <stdio.h>    
#include <stdlib.h>     /* srand, rand */
#include <time.h>    

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"

#include <fstream>
#include <string>
#include <sstream> 
#include <bitset>         // std::bitset

using namespace std;

///////////////////////////////////
//
//
// Base class for pattern generation for the concentrator chip
//
// Input infos are :
//
// filename : the name and directory of the input ROOT file containing the STUB information
// outfile  : the name of the output ROOT file containing the pattern info (a text file with binary info is 
//            also produced) 
// npatt    : the number of patterns you want to generate 
//
// Info about the code:
//
//  ...TBD...
// 
//
//  Author: viret@in2p3_dot_fr
//  Date: 12/11/2013
//
///////////////////////////////////



class patterngen
{
 public:

  patterngen(std::string filename, std::string outfile, int npatt);

  void  get_patterns(int npatt);  // The main method  
  void  initVars();
  void  initTuple(std::string in,std::string out);

 private:

  TChain *L1TT;      // The trees containing the input data

  ofstream m_outbinary; // txt file containing the output sequences

  // Coding conventions for barrel and endcap module IDs
  
  // We define a barrel ID and an endcap ID as follows:

  // B_id = (layer-1)*10000 + ladder*100 + module (0 to 57523)

  // E_id = (disk-1)*10000 + ladder*100 + module (0 to 131377)

  // Disks 0 to 6  (towards positive Z)  (0 to  4 for 5D)
  // Disks 7 to 13 (towards negative Z)  (7 to 11 for 5D)

  // Here are the parameters needed from the data
  // Details on these might be found on
  //
  // https://github.com/sviret/HL_LHC/blob/master/Extractors/RecoExtractor/interface/L1TrackTrigger_analysis.h
  //

  int m_stub;
  std::vector<int>   m_stub_layer;
  std::vector<int>   m_stub_ladder;
  std::vector<int>   m_stub_module;
  std::vector<float> m_stub_pt;
  std::vector<float> m_stub_deltas;
  std::vector<int>   m_stub_strip;
  std::vector<int>   m_stub_seg;
  std::vector<int>   m_stub_chip;
  std::vector<int>   m_stub_clust1;
  std::vector<int>   m_clus_nseg;

  std::vector<int>   *pm_stub_layer;
  std::vector<int>   *pm_stub_ladder;
  std::vector<int>   *pm_stub_module;
  std::vector<float> *pm_stub_pt;
  std::vector<float> *pm_stub_deltas;
  std::vector<int>   *pm_stub_strip;
  std::vector<int>   *pm_stub_seg;
  std::vector<int>   *pm_stub_chip;
  std::vector<int>   *pm_stub_clust1;
  std::vector<int>   *pm_clus_nseg;

};

#endif

