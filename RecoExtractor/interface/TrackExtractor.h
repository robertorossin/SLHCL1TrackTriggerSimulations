#ifndef TRACKEXTRACTOR_H
#define TRACKEXTRACTOR_H

/**
 * TrackExtractor
 * \brief: Base class for extracting track info
 */


//Include RECO info
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/TrackReco/interface/DeDxData.h"
#include "TrackingTools/Records/interface/TransientRecHitRecord.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHitBuilder.h"


//Include std C++
#include <iostream>
#include <vector>

#include "TMath.h"
#include "TTree.h"

class TrackExtractor
{

 public:

  TrackExtractor(edm::InputTag tag);
  ~TrackExtractor();


  void writeInfo(const edm::Event *event, const edm::EventSetup *setup); 

  void reset();
  void fillTree(); 
  void fillSize(int size);
  int  getSize();

 private:
  
  TTree* m_tree;

  edm::InputTag m_tag;
  edm::ESHandle<TransientTrackingRecHitBuilder> theTrackerRecHitBuilder;

  // Track info

  static const int      m_tracks_MAX     = 1000;
  static const int      m_hits_MAX       = 100;

  int                   m_n_tracks;
  float                 m_tracks_px[m_tracks_MAX];
  float                 m_tracks_py[m_tracks_MAX];
  float                 m_tracks_pz[m_tracks_MAX];
  float                 m_tracks_vx[m_tracks_MAX];
  float                 m_tracks_vy[m_tracks_MAX];
  float                 m_tracks_vz[m_tracks_MAX];
  float                 m_tracks_normChi2[m_tracks_MAX];
  float                 m_tracks_dedx[m_tracks_MAX];
  float                 m_tracks_dedx_n[m_tracks_MAX];
  float                 m_tracks_nhits[m_tracks_MAX];

  std::vector<int> m_tracks_xhit;
  std::vector<int> m_tracks_yhit;
  std::vector<int> m_tracks_zhit;
};

#endif 
