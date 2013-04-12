#ifndef L1TRACKTRIGGER_ANALYSIS_H
#define L1TRACKTRIGGER_ANALYSIS_H

/*****************************

Simple example class showing how to perform an 
analysis using the PatExtractor tools

S.Viret (viret@in2p3.fr): 31/05/11

More info: http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.PHYTuto

 ******************************/

//Include std C++
#include <iostream>
#include <vector>
#include <map>
//#include <math.h>
#include <set>
#include <algorithm>
using namespace std;

#include "AnalysisSettings.h"
#include "PixelExtractor.h"
#include "MCExtractor.h"

class L1TrackTrigger_analysis
{
 public:
  L1TrackTrigger_analysis(AnalysisSettings *settings, int start_evt);

  ~L1TrackTrigger_analysis();
  
  //Selection

  void do_stubs(PixelExtractor *pix, MCExtractor *mc);

  void get_digis(PixelExtractor *pix, MCExtractor *mc);
  void get_clusters(PixelExtractor *pix, MCExtractor *mc);
  void get_stubs(int layer,MCExtractor *mc);

  void initialize();
  void reset();
  void fillTree();

  bool is_neighbour(PixelExtractor *pix, int idx, int lay, int lad, int mod);
  int  getMatchingTP(int i, int j);

 private:

  TTree* m_tree_L1TrackTrigger;

  int n_tot_evt;
  int m_nstubs;

  bool  m_verb;
  bool  m_matchStubs;
  bool  m_posMatch;
  int   m_max_wclus;
  int   m_max_dist;
  float m_max_StubDev;
  int   m_PDG_id;
  float m_window_size;
  float m_thresh;

  // Clusters info


  int m_clus;
  int m_stub;

  std::vector<int> clus_row;
  std::vector<int> clus_col;

  std::vector<int> i_match;
  std::vector<int> j_match;

  int i_match_s;
  int j_match_s;

  std::vector<int>                 *m_digi_ref;
  std::vector< std::vector<int> >  *m_digi_match;
  std::vector< std::vector<int> >  *m_digi_tp;
  
  std::vector<float>  *m_clus_x;
  std::vector<float>  *m_clus_y;
  std::vector<float>  *m_clus_z;
  std::vector<float>  *m_clus_xmc;
  std::vector<float>  *m_clus_ymc;
  std::vector<float>  *m_clus_zmc;
  std::vector<float>  *m_clus_e;
  std::vector<int>    *m_clus_layer;
  std::vector<int>    *m_clus_module;
  std::vector<int>    *m_clus_ladder;
  std::vector<int>    *m_clus_seg;
  std::vector<float>  *m_clus_strip;
  std::vector<int>    *m_clus_used;
  std::vector<int>    *m_clus_sat;
  std::vector<int>    *m_clus_nstrips;
  std::vector<int>    *m_clus_matched;
  std::vector<int>    *m_clus_PS;
  std::vector<int>    *m_clus_nrows;
  std::vector< std::vector<int> >    *m_clus_tp;
  std::vector< std::vector<int> >    *m_clus_hits;
  
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
  std::vector<float>  *m_stub_deltas;
  std::vector<float>  *m_stub_cor;
  std::vector<int>    *m_stub_tp;
  std::vector<int>    *m_stub_pdg;

};

#endif 
