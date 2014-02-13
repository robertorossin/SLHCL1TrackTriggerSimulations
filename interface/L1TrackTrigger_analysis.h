#ifndef L1TRACKTRIGGER_ANALYSIS_H
#define L1TRACKTRIGGER_ANALYSIS_H

//Include std C++
#include <iostream>
#include <vector>
#include <map>
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
  int m_evtNum;

  bool  m_verb;
  bool  m_matchStubs;
  bool  m_posMatch;
  int   m_max_wclus;
  int   m_max_dist;
  float m_max_StubDev;
  int   m_PDG_id;
  float m_window_size;
  float m_thresh;
  float m_pTthresh;
  bool  m_zMatch;
 
  /*
    List of the branches contained in the L1TrackTrigger tree

    tree->Branch("CLUS_n",         &m_clus);
    tree->Branch("CLUS_x",         &m_clus_x);
    tree->Branch("CLUS_y",         &m_clus_y);
    tree->Branch("CLUS_z",         &m_clus_z);
    tree->Branch("CLUS_xmc",       &m_clus_xmc);
    tree->Branch("CLUS_ymc",       &m_clus_ymc);
    tree->Branch("CLUS_zmc",       &m_clus_zmc);
    tree->Branch("CLUS_charge",    &m_clus_e);
    tree->Branch("CLUS_layer",     &m_clus_layer);
    tree->Branch("CLUS_module",    &m_clus_module);
    tree->Branch("CLUS_ladder",    &m_clus_ladder);
    tree->Branch("CLUS_seg",       &m_clus_seg);
    tree->Branch("CLUS_strip",     &m_clus_strip);
    tree->Branch("CLUS_nstrip",    &m_clus_nstrips);
    tree->Branch("CLUS_nsat",      &m_clus_sat);
    tree->Branch("CLUS_match",     &m_clus_matched);
    tree->Branch("CLUS_PS",        &m_clus_PS);
    tree->Branch("CLUS_nrows",     &m_clus_nrows);
    tree->Branch("CLUS_tp",        &m_clus_tp);
    tree->Branch("CLUS_hits",      &m_clus_hits);
    tree->Branch("CLUS_process",   &m_clus_pid);

    tree->Branch("STUB_ptMC",      &m_stub_ptMC);
    tree->Branch("STUB_clust1",    &m_stub_clust1);
    tree->Branch("STUB_clust2",    &m_stub_clust2);
    tree->Branch("STUB_cw1",       &m_stub_cw1);
    tree->Branch("STUB_cw2",       &m_stub_cw2);
    tree->Branch("STUB_cor",       &m_stub_cor);
    tree->Branch("STUB_PHI0",      &m_stub_PHI0);
    tree->Branch("STUB_tp",        &m_stub_tp);
    tree->Branch("STUB_pdgID",     &m_stub_pdg);
    tree->Branch("STUB_process",   &m_stub_pid);  
    tree->Branch("STUB_n",         &m_stub);
    tree->Branch("STUB_pt",        &m_stub_pt);
    tree->Branch("STUB_pxGEN",     &m_stub_pxGEN);
    tree->Branch("STUB_pyGEN",     &m_stub_pyGEN);
    tree->Branch("STUB_etaGEN",    &m_stub_etaGEN);
    tree->Branch("STUB_layer",     &m_stub_layer);
    tree->Branch("STUB_module",    &m_stub_module);
    tree->Branch("STUB_ladder",    &m_stub_ladder);
    tree->Branch("STUB_seg",       &m_stub_seg);
    tree->Branch("STUB_strip",     &m_stub_strip);
    tree->Branch("STUB_x",         &m_stub_x);
    tree->Branch("STUB_y",         &m_stub_y);
    tree->Branch("STUB_z",         &m_stub_z);
    tree->Branch("STUB_deltas",    &m_stub_deltas);
    tree->Branch("STUB_X0",        &m_stub_X0);
    tree->Branch("STUB_Y0",        &m_stub_Y0);
    tree->Branch("STUB_Z0",        &m_stub_Z0);
  */

  std::vector<int> clus_row;
  std::vector<int> clus_col;

  std::vector<int> i_match;
  std::vector<int> j_match;

  int i_match_s;
  int j_match_s;

  std::vector<int>                 *m_digi_ref;
  std::vector< std::vector<int> >  *m_digi_match;
  std::vector< std::vector<int> >  *m_digi_tp;
  std::vector< std::vector<int> >  *m_digi_modids;
  std::vector<int>                 *m_clus_used;   

  int m_clus;

  // Size of the following vectors is m_clus
  std::vector<float>  *m_clus_x;      // x pos of cluster i (in cm)
  std::vector<float>  *m_clus_y;      // y pos of cluster i (in cm)
  std::vector<float>  *m_clus_z;      // z pos of cluster i (in cm)
  std::vector<float>  *m_clus_xmc;    // x true pos of cluster i (in cm)
  std::vector<float>  *m_clus_ymc;    // y true pos of cluster i (in cm)
  std::vector<float>  *m_clus_zmc;    // z true pos of cluster i (in cm)
  std::vector<float>  *m_clus_e;      // UNUSED
  std::vector<int>    *m_clus_layer;  // layer of cluster i
  std::vector<int>    *m_clus_module; // module of cluster i
  std::vector<int>    *m_clus_ladder; // ladder/ring of cluster i
  std::vector<int>    *m_clus_seg;    // segment (column) of cluster i
  std::vector<float>  *m_clus_strip;  // strip position of cluster i (in half-strip unit) 
  std::vector<int>    *m_clus_sat;    // number of saturating strips in cluster i (digis>255 ADC)
  std::vector<int>    *m_clus_nstrips;// number of strips in cluster i
  std::vector<int>    *m_clus_matched;// number of simhits matched to the cluster
  std::vector<int>    *m_clus_PS;     // number of segments in the corresponding module
  std::vector<int>    *m_clus_nrows;  // number of strips in the corresponding module
  std::vector< std::vector<int> >    *m_clus_tp;   // list of tracking particles inducing cluster i
  std::vector< std::vector<int> >    *m_clus_hits; // list of simhits inducing cluster i
  std::vector< std::vector<int> >    *m_clus_pix;  // list of pixels inducing cluster i
  std::vector<int>    *m_clus_pid;    // process id inducing cluster i (see MCExtractor.h)
  std::vector<int>    *m_clus_rank;   // the number of clusters in the chip

  int m_stub;

  // Size of the following vectors is m_stub
  std::vector<float>  *m_stub_pt;     // pt calculated for stub i (in GeV/c)
  std::vector<float>  *m_stub_ptGEN;  // pt generated of stub i (in GeV/c)
  std::vector<float>  *m_stub_pxGEN;  // px generated of stub i (in GeV/c)
  std::vector<float>  *m_stub_pyGEN;  // py generated of stub i (in GeV/c)
  std::vector<float>  *m_stub_etaGEN; // eta generated of stub i (in GeV/c)
  std::vector<float>  *m_stub_X0;     // x origin of particle inducing stub i (in cm)
  std::vector<float>  *m_stub_Y0;     // y origin of particle inducing stub i (in cm)
  std::vector<float>  *m_stub_Z0;     // z origin of particle inducing stub i (in cm)
  std::vector<float>  *m_stub_PHI0;   // phi origin of particle inducing stub i (in rad)
  std::vector<int>    *m_stub_modid;  // 1000000*layer+10000*ladder+100*module+seg
  std::vector<int>    *m_stub_layer;  // layer of stub i
  std::vector<int>    *m_stub_module; // module of stub i
  std::vector<int>    *m_stub_ladder; // ladder/ring of stub i
  std::vector<int>    *m_stub_seg;    // segment of stub i
  std::vector<int>    *m_stub_chip;   // chip of stub i (8 chip per sensor)
  std::vector<float>  *m_stub_strip;  // strip of stub i (innermost module value)
  std::vector<float>  *m_stub_x;      // x pos of stub i (in cm)
  std::vector<float>  *m_stub_y;      // y pos of stub i (in cm)
  std::vector<float>  *m_stub_z;      // z pos of stub i (in cm)
  std::vector<int>    *m_stub_clust1; // bottom cluster index of stub i 
  std::vector<int>    *m_stub_clust2; // top cluster index of stub i 
  std::vector<int>    *m_stub_cw1;    // bottom cluster width (in strips) 
  std::vector<int>    *m_stub_cw2;    // top cluster width (in strips) 
  std::vector<float>  *m_stub_deltas; // corrected stub width (in half-strips)
  std::vector<float>  *m_stub_cor;    // stub wirdth applied correction (in half-strips)
  std::vector<int>    *m_stub_tp;     // index of the TP inducing the stub in the MC tree
  std::vector<int>    *m_stub_pdg;    // PDG code of the particle inducing the stub
  std::vector<int>    *m_stub_pid;    // process id inducing cluster i (see MCExtractor.h)
  std::vector<int>    *m_stub_rank;   // the priority rank of the stub at the FE (CBC/MPA) level
                                      // Classification per chip from lower bend to higher bend



};

#endif 
