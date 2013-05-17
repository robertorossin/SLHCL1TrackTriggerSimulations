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

  /*
    List of the branches contained in the Pixels tree

    m_tree->Branch("PIX_n",         &m_pclus,    "PIX_n/I");
    m_tree->Branch("PIX_x",         &m_pixclus_x);
    m_tree->Branch("PIX_y",         &m_pixclus_y);
    m_tree->Branch("PIX_z",         &m_pixclus_z);
    m_tree->Branch("PIX_charge",    &m_pixclus_e);
    m_tree->Branch("PIX_row",       &m_pixclus_row);
    m_tree->Branch("PIX_column",    &m_pixclus_column);
    m_tree->Branch("PIX_simhit",    &m_pixclus_simhit);
    m_tree->Branch("PIX_simhitID",  &m_pixclus_simhitID);
    m_tree->Branch("PIX_layer",     &m_pixclus_layer);
    m_tree->Branch("PIX_module",    &m_pixclus_module);
    m_tree->Branch("PIX_ladder",    &m_pixclus_ladder);
    m_tree->Branch("PIX_nrow",      &m_pixclus_nrow);
    m_tree->Branch("PIX_ncolumn",   &m_pixclus_ncolumn);
    m_tree->Branch("PIX_pitchx",    &m_pixclus_pitchx);
    m_tree->Branch("PIX_pitchy",    &m_pixclus_pitchy);
  */

  // Meaning of the branches

  int    		m_pclus; // Number of digis

  // All the following vectors have a size m_pclus

  std::vector<float>               *m_pixclus_x;        // Digi x-global coordinates (in cm)
  std::vector<float>               *m_pixclus_y;        // Digi y-global coordinates (in cm)
  std::vector<float>               *m_pixclus_z;        // Digi z-global coordinates (in cm)
  std::vector<float>               *m_pixclus_e;        // Digi signal (in ADC counts)
  std::vector<int>                 *m_pixclus_row;      // Digi strip numbers 
  std::vector<int>                 *m_pixclus_column;   // Digi column numbers 
  std::vector<int>                 *m_pixclus_simhit;   // Number of simhits making the digi
  std::vector< std::vector<int> >  *m_pixclus_simhitID; // Simtrack IDs of the corresponding simhits (stored as a vector)
  std::vector<int>                 *m_pixclus_layer;    // Digi layer numbers 
  std::vector<int>                 *m_pixclus_module;   // Digi module numbers 
  std::vector<int>                 *m_pixclus_ladder;   // Digi ladder/ring numbers 
  std::vector<int>                 *m_pixclus_nrow;     // Number of strips of the sensor containing the digi
  std::vector<int>                 *m_pixclus_ncolumn;  // Number of columns of the sensor containing the digi
  std::vector<float>               *m_pixclus_pitchx;   // Strip pitch
  std::vector<float>               *m_pixclus_pitchy;   // Column pitch


  std::vector<int>      the_ids;
};

#endif 
