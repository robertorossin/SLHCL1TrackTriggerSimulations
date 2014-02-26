#ifndef TRACK_EFF_H
#define TRACK_EFF_H

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
// Base class for L1 track reconstruction efficiency using the official chain
//
// The role of this class is to check that all particles are contained in at least one 
// trigger sector, to count the stub multiplicities per sector, and finally to 
// compute the pattern bank and track fit efficiencies
//
// This class is using the official object and is starting from rootuple created
// in step 6 of the tutorial
//
// filename    : the name and directory of the input ROOT file containing the data to analyze
// secfilename : the name and directory of the input csv file  containing the sectors definition
// outfile     : the name of the output ROOT file containing the efficiency results 
//           
// nevt        : the number of particles to test
// dbg         : debug mode (true if the pattern file comes from the standalone preco, false otherwise) 
//
// Info about the code:
//
//  http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.HLLHCTuto620 (STEP 6)
//
//  Author: viret@in2p3_dot_fr
//  Date: 25/02/2014
//
///////////////////////////////////

using namespace std;

class track_eff
{
 public:

  track_eff(std::string filename, std::string secfilename, 
	    std::string outfile, int nevt, float pt_min, 
	    float d0_min, bool dbg);

  void   do_test(int nevt);    
  void   initTuple(std::string test,std::string out);
  bool   convert(std::string sectorfilename); 
  void   reset();
    
 private:

  bool do_patt;
  bool m_dbg;
  bool has_patt;
  float m_pt_min;
  float m_d0_min;

  TFile  *m_infile;
  TFile  *m_testfile;
  TFile  *m_outfile;
  TChain *m_L1TT;
  TChain *data;

  TTree  *m_efftree;
  TTree  *m_finaltree;
  TChain *m_PATT;

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


  std::vector< std::vector<int> >   m_modules; 


  int m_evtid;
  int m_stub;
  std::vector<int>   m_stub_layer;
  std::vector<int>   m_stub_ladder;
  std::vector<int>   m_stub_module;
  std::vector<int>   m_stub_segment;
  std::vector<float> m_stub_strip;
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
  std::vector<float> m_clus_x;
  std::vector<float> m_clus_y;
  std::vector<float> m_clus_z;
  std::vector<int>   m_stub_clust2;


  std::vector<int>   *pm_stub_layer;
  std::vector<int>   *pm_stub_ladder;
  std::vector<int>   *pm_stub_module;
  std::vector<int>   *pm_stub_segment;
  std::vector<float> *pm_stub_strip;
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
  std::vector<float> *pm_clus_x;
  std::vector<float> *pm_clus_y;
  std::vector<float> *pm_clus_z;
  std::vector<int>   *pm_stub_clust2;


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
  int   ntrack;     // 
  int   ttrack;     // 
  int   mult[500];  // The total number of stubs per sector 
  int   nhits;      // The total number of layers/disks hit by the prim track
  int   nplay[20];  // The total number of prim stubs per layer 


  int n_stub_total; // The total number of stubs in the event
  int n_stub;       // The total number of stubs contained in matched patterns in the event
  int n_stub_t;     // The total number of stubs contained in reco tracks in the event

  std::vector<float>   *stub_x;      // x coordinates of ALL the stubs |
  std::vector<float>   *stub_y;      // y coordinates of ALL the stubs |-> Bottom cluster
  std::vector<float>   *stub_z;      // z coordinates of ALL the stubs |
  std::vector<int>     *stub_layer;  // layer number of ALL the stubs
  std::vector<int>     *stub_ladder; // ladder number of ALL the stubs
  std::vector<int>     *stub_module; // module number of ALL the stubs
  std::vector<int>     *stub_seg;    // segment number of ALL the stubs
  std::vector<float>   *stub_strip;  // strip number of ALL the stubs
  std::vector<int>     *stub_tp;     // tp index of ALL the stubs (in part_*** vectors of this tree!!!!)
  std::vector<int>     *stub_inpatt; // is the stub in a pattern (1) of not (0)?
  std::vector<int>     *stub_intrk;  // is the stub in a track (1) of not (0)?

  int n_part;                        // The total number of particles inducing at least one stub in the event

  std::vector<int>     *part_pdg;    // PDG id of the particles
  std::vector<int>     *part_nsec;   // In how many trigger towers this particle hit more than 4 different layers/disks?
  std::vector<int>     *part_nhits;  // How many different layers/disks are hit by the particle?
  std::vector<int>     *part_npatt;  // How many patterns contains more than 4 stubs of the particle (in 4 different layers/disks)?
  std::vector<int>     *part_ntrk;   // How many tracks contains more than 4 stubs of the particle (in 4 different layers/disks)?
  std::vector<float>   *part_pt;     // pt of the particles
  std::vector<float>   *part_rho;    // rho0 of the particles
  std::vector<float>   *part_z0;     // z0 of the particles
  std::vector<float>   *part_eta;    // eta of the particles 
  std::vector<float>   *part_phi;    // phi of the particles

  int n_patt;                        // The total number of patterns matched in the event
  std::vector<int>                  *patt_sec;   // Sector id of all the patterns
  std::vector< std::vector<int> >   *patt_parts; // tp index of ALL the particles contained in the pattern (in part_*** vectors of this tree!!!!)
  std::vector< std::vector<int> >   *patt_stubs; // index of ALL the stubs contained in the pattern (in stub_*** vectors of this tree!!!!) 

  int n_track;                        // The total number of L1 tracks matched in the event
  std::vector<int>                  *trk_sec;    // Sector id of all the tracks
  std::vector< std::vector<int> >   *trk_parts;  // tp index of ALL the particles contained in the pattern (in part_*** vectors of this tree!!!!)
  std::vector< std::vector<int> >   *trk_stubs;  // index of ALL the stubs contained in the track (in stub_*** vectors of this tree!!!!) 
  std::vector<float>                *trk_pt;     // pt of the track
  std::vector<float>                *trk_eta;    // eta of the track
  std::vector<float>                *trk_z;      // z0 of the track
  std::vector<float>                *trk_phi;    // phi of the track
  
  // Informations contained in the pattern file

  int nb_patterns; // Number of patterns in event evtID
  int event_id;    // Event ID


  // Info id dbg==false (CMSSW fast mode)

  std::vector< std::vector<int> > m_links; // Links to the stub ids of the pattern 
  std::vector<int>                m_secid; // Link to the sector ids of the pattern 

  std::vector< std::vector<int> > *pm_links; 
  std::vector<int>                *pm_secid;

  // Informations contained in the pattern tree of official file

  int nb_tracks; // Number of L1 tracks in event

  std::vector< std::vector<int> > m_pattlinks; // Links to the stub ids of the pattern 
  std::vector<int>                m_pattsecid; // Link to the sector ids of the pattern
  std::vector< std::vector<int> > m_trklinks;  // Links to the stub ids of the track 
  std::vector<int>                m_trksecid;  // Link to the sector ids of the track
  std::vector<float>              m_trkpt; 
  std::vector<float>              m_trketa; 
  std::vector<float>              m_trkz; 
  std::vector<float>              m_trkphi; 


  std::vector< std::vector<int> > *pm_pattlinks; 
  std::vector<int>                *pm_pattsecid;
  std::vector< std::vector<int> > *pm_trklinks;
  std::vector<int>                *pm_trksecid; 
  std::vector<float>              *pm_trkpt; 
  std::vector<float>              *pm_trketa; 
  std::vector<float>              *pm_trkz; 
  std::vector<float>              *pm_trkphi; 

};

#endif

