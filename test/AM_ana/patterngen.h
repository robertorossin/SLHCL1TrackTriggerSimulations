#ifndef PATTERNGEN_H
#define PATTERNGEN_H


#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>   

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

  patterngen(std::string filename, std::string outfile, int npatt, int rate);
  patterngen(std::string filename, std::string outfile, int npatt);

  void  get_all_patterns(int npatt);  // The main method  
  void  initVars();
  void  initTuple(std::string in,std::string out,int type);
  void  readConcOutput(std::string filename);
  void  ana_pix(int lay,int lad,int mod, std::vector<int> digits);
  void  get_MPA_input(int nevt);
  void  do_stub(int lay,int lad,int mod);
  void  ana_chip();


 private:

  TChain *L1TT;      // The trees containing the input data
  TChain *PIX;      // The trees containing the input data
  TChain *MC;      // The trees containing the input data

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

  int m_ntp;
  std::vector<float>  m_part_px;
  std::vector<float>  m_part_py;
  std::vector<float>  m_part_eta;
  std::vector<float>  m_part_x;
  std::vector<float>  m_part_y;
  std::vector<float>  m_part_z;

  std::vector<float>  *pm_part_px;
  std::vector<float>  *pm_part_py;
  std::vector<float>  *pm_part_eta;
  std::vector<float>  *pm_part_x;
  std::vector<float>  *pm_part_y;
  std::vector<float>  *pm_part_z;

  int m_pix;
  int m_npu;
  std::vector<int>    m_pix_layer;
  std::vector<int>    m_pix_ladder;
  std::vector<int>    m_pix_module;
  std::vector<int>    m_pix_row;
  std::vector<int>    m_pix_col;
  std::vector<float>  m_pix_ch;
  std::vector<float>  m_pix_x;
  std::vector<float>  m_pix_y;
  std::vector<float>  m_pix_z;

  std::vector<int>    *pm_pix_layer;
  std::vector<int>    *pm_pix_ladder;
  std::vector<int>    *pm_pix_module;
  std::vector<int>    *pm_pix_row;
  std::vector<int>    *pm_pix_col;
  std::vector<float>  *pm_pix_ch;
  std::vector<float>  *pm_pix_x;
  std::vector<float>  *pm_pix_y;
  std::vector<float>  *pm_pix_z;

  int m_clus;
  std::vector<int>   m_clus_layer;
  std::vector<int>   m_clus_ladder;
  std::vector<int>   m_clus_module;
  std::vector<int>   m_clus_nrows;
  std::vector<int>   m_clus_tp;
  std::vector<std::vector<int> >   m_clus_pix;
  std::vector<std::vector<int> >   m_clus_mult;
  std::vector<int>    m_clus_nseg;

  std::vector<int>   *pm_clus_layer;
  std::vector<int>   *pm_clus_ladder;
  std::vector<int>   *pm_clus_module;
  std::vector<int>   *pm_clus_nrows;
  std::vector<int>   *pm_clus_tp;
  std::vector<std::vector<int> >   *pm_clus_pix;
  std::vector<std::vector<int> >   *pm_clus_mult;
  std::vector<int>   *pm_clus_nseg;


  int m_stub;
  std::vector<int>    m_stub_layer;
  std::vector<int>    m_stub_ladder;
  std::vector<int>    m_stub_module;
  std::vector<float>  m_stub_pxGEN;  // px generated of stub i (in GeV/c)
  std::vector<float>  m_stub_pyGEN;  // py generated of stub i (in GeV/c)
  std::vector<float>  m_stub_etaGEN; // eta generated of stub i (in GeV/c)
  std::vector<float>  m_stub_X0;     // x origin of particle inducing stub i (in cm)
  std::vector<float>  m_stub_Y0;     // y origin of particle inducing stub i (in cm)
  std::vector<float>  m_stub_Z0;     // z origin of particle inducing stub i (in cm)
  std::vector<float>  m_stub_PHI0;   // phi origin of particle inducing stub i (in rad)
  std::vector<float>  m_stub_pt;
  std::vector<int>    m_stub_tp;
  std::vector<float>  m_stub_deltas;
  std::vector<float>  m_stub_strip;
  std::vector<int>    m_stub_seg;
  std::vector<int>    m_stub_chip;
  std::vector<int>    m_stub_clust1;
  std::vector<int>    m_stub_clust2;


  std::vector<int>    *pm_stub_layer;
  std::vector<int>    *pm_stub_ladder;
  std::vector<int>    *pm_stub_module;
  std::vector<float>  *pm_stub_pxGEN;  // px generated of stub i (in GeV/c)
  std::vector<float>  *pm_stub_pyGEN;  // py generated of stub i (in GeV/c)
  std::vector<float>  *pm_stub_etaGEN; // eta generated of stub i (in GeV/c)
  std::vector<float>  *pm_stub_X0;     // x origin of particle inducing stub i (in cm)
  std::vector<float>  *pm_stub_Y0;     // y origin of particle inducing stub i (in cm)
  std::vector<float>  *pm_stub_Z0;     // z origin of particle inducing stub i (in cm)
  std::vector<float>  *pm_stub_PHI0;   // phi origin of particle inducing stub i (in rad)
  std::vector<float>  *pm_stub_pt;
  std::vector<int>    *pm_stub_tp;
  std::vector<float>  *pm_stub_deltas;
  std::vector<float>  *pm_stub_strip;
  std::vector<int>    *pm_stub_seg;
  std::vector<int>    *pm_stub_chip;
  std::vector<int>    *pm_stub_clust1;
  std::vector<int>    *pm_stub_clust2;

  std::multimap< int, std::vector<int> > m_chip_trig;  //
  std::multimap< int, std::vector<int> > m_chip_raw;  //

  std::multimap< int, std::vector<int> > m_pix_idx;  //

  std::multimap< int, std::vector<int> >::const_iterator m_iter; 


  std::vector<std::multimap< int, std::vector<int> > > m_data_trig;
  std::vector<std::multimap< int, std::vector<int> > > m_data_raw;


  int m_rate; // the input L1 rate, in kHz


  TFile *m_outfile;  // The output file
  TTree *m_tri_tree; // 
  TTree *m_raw_tree; // 

  int m_tri_bx;
  int m_raw_bx;

  int m_tri_chp[8][40];
  int m_raw_chp[8][266];  

  std::vector<int>   m_evt_pix;
  std::vector<int>   m_evt_clu;
  std::vector<int>   m_evt_stu;
  std::vector<int>   m_evt_tp;


  int MPA_eff_4_4_4[80000][16][4];
  int MPA_eff_4_4_8[80000][16][4];
  int MPA_eff_5_5_4[80000][16][4];
  int MPA_eff_5_5_8[80000][16][4];



  int Conc_CBC_eff_480_8[80000][2][2];
  int Conc_CBC_eff_480_4[80000][2][2];
  int Conc_CBC_eff_320_8[80000][2][2];
  int Conc_CBC_eff_320_4[80000][2][2];

  int Conc_MPA_eff_480_8[80000][2][2];
  int Conc_MPA_eff_320_8[80000][2][2];

  int mod_fifos_4[80000][2][3];
  int mod_fifos_8[80000][2][3];
};

#endif

