#ifndef TKLAYOUT_TRANSLATOR_H
#define TKLAYOUT_TRANSLATOR_H

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

// ROOT stuff
#include "TMath.h"
#include "TTree.h"
#include "TFile.h"

class TkLayout_Translator
{
 public:
  TkLayout_Translator(TFile *a_file);

  ~TkLayout_Translator();
  
  //Selection
  int  n_events() {return m_n_events;}
  void getInfo(int ievt); 

  void do_translation();

  void reset();
  void fillTree();


  bool isOK() {return m_OK;}
 private:

  TTree* m_tree_L1TrackTrigger;
  TTree* m_tree_retrieved;

  // Input file info

  std::vector<unsigned int> *m_eventn; // branch name "tracks.eventn"
  std::vector<unsigned int> *m_trackn; // branch name "tracks.trackn"
  std::vector<double>       *m_eta; // "tracks.eta"
  std::vector<double>       *m_phi0; // "tracks.phi0"
  std::vector<double>       *m_z0; // "tracks.z0"
  std::vector<double>       *m_pt; // "tracks.pt"

  std::vector<double>       *m_glox, *m_gloy, *m_gloz; // branch name "hits.glox" ...
  std::vector<double>       *m_locx, *m_locy; // etc...
  std::vector<float>        pterr, hitprob;
  std::vector<float>         *m_deltas;
  std::vector<char>         *m_cnt, *m_z, *m_rho, *m_phi; 

  bool m_OK;
  int  m_n_events;

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



  // Stubs info

  int m_stub;
  
  std::vector<float>  *m_stub_pt;
  std::vector<float>  *m_stub_ptMC;
  std::vector<float>  *m_stub_ptGEN;
  std::vector<float>  *m_stub_pzGEN;
  std::vector<float>  *m_stub_IP;
  std::vector<float>  *m_stub_Z0;
  std::vector<float>  *m_stub_PHI0;
  std::vector<int>    *m_stub_disk;
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
  std::vector<int>    *m_stub_tp;
  std::vector<int>    *m_stub_pdg;


};

#endif 
