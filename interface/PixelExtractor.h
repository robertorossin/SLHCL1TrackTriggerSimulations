#ifndef PIXELEXTRACTOR_H
#define PIXELEXTRACTOR_H

/**
 * PixelExtractor
 * \brief: Base class for extracting pixel info
 */


//Include RECO inf
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/InputTag.h"


#include "SimDataFormats/TrackerDigiSimLink/interface/PixelDigiSimLink.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/CommonTopologies/interface/PixelTopology.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetUnit.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"

#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "DataFormats/SiPixelDigi/interface/PixelDigi.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"

//Include std C++
#include <iostream>
#include <vector>

#include "TMath.h"
#include "TTree.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"

class PixelExtractor
{

 public:

  PixelExtractor(edm::InputTag tag,bool doTree,bool doMatch);
  PixelExtractor(TFile *a_file);
  ~PixelExtractor();


  void init(const edm::EventSetup *setup);
  void writeInfo(const edm::Event *event); 
  void getInfo(int ievt); 

  void reset();
  void fillTree(); 
  void fillSize(int size);
  int  getSize();
  int  n_events() {return m_n_events;}

  bool isOK() {return m_OK;}

  //Getters


  int getNDigis() {return m_pclus;}

  int isSimHit(int i) {return m_pixclus_simhit->at(i);}
  int tpIndex(int i,int j) {return (m_pixclus_simhitID->at(i)).at(j);}

  float e(int i) {return m_pixclus_e->at(i);}
  float x(int i) {return m_pixclus_x->at(i);}
  float y(int i) {return m_pixclus_y->at(i);}
  float z(int i) {return m_pixclus_z->at(i);}

  int layer(int i)  {return m_pixclus_layer->at(i);}
  int ladder(int i) {return m_pixclus_ladder->at(i);}
  int module(int i) {return m_pixclus_module->at(i);}
  int row(int i)    {return m_pixclus_row->at(i);}
  int column(int i) {return m_pixclus_column->at(i);}
  int nrow(int i)    {return m_pixclus_nrow->at(i);}
  int ncolumn(int i) {return m_pixclus_ncolumn->at(i);}
 private:
  
  TTree* m_tree;

  edm::Handle<TrackingParticleCollection>  TPCollection ;
  edm::Handle< edm::DetSetVector<PixelDigiSimLink> > pDigiLinkColl;
  edm::DetSet<PixelDigiSimLink> pDigiLinks;

  edm::ESHandle<TrackerGeometry> theTrackerGeometry;
  edm::InputTag m_tag;
  bool m_OK;
  bool m_matching;
  int  m_n_events;

  // Pixel info

  int    		m_pclus;

  std::vector<float>    *m_pixclus_x;
  std::vector<float>    *m_pixclus_y;
  std::vector<float>    *m_pixclus_z;
  std::vector<float>    *m_pixclus_e;
  std::vector<int>      *m_pixclus_row;
  std::vector<int>      *m_pixclus_column;
  std::vector<int>      *m_pixclus_ptype;
  std::vector<int>      *m_pixclus_simhit;
  //std::vector<int>      *m_pixclus_simhitID;

  std::vector< std::vector<int> >  *m_pixclus_simhitID;

  std::vector<int>      *m_pixclus_layer;
  std::vector<int>      *m_pixclus_module;
  std::vector<int>      *m_pixclus_ladder;
  std::vector<int>      *m_pixclus_nrow;
  std::vector<int>      *m_pixclus_ncolumn;
  std::vector<float>    *m_pixclus_pitchx;
  std::vector<float>    *m_pixclus_pitchy;


  std::vector<int>      the_ids;
};

#endif 
