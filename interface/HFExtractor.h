#ifndef HFEXTRACTOR_H
#define HFEXTRACTOR_H

/**
 * HFExtractor
 * \brief: Base class for extracting HF info
 */


//Include RECO inf
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h" 
#include "DataFormats/L1Trigger/interface/L1HFRings.h"
#include "DataFormats/L1Trigger/interface/L1HFRingsFwd.h"

//Include std C++
#include <iostream>

#include "TMath.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"

class HFExtractor
{

 public:

  HFExtractor(edm::InputTag tag, bool skim);
  ~HFExtractor();

  void writeInfo(const edm::Event *event); 
  void init(const edm::EventSetup *setup); 

  void reset();
  void fillTree(); 
  void fillSize(int size);
  int  getSize();

 private:
  
  TTree* m_tree;

  edm::ESHandle<CaloGeometry>    caloGeometry;
  const CaloSubdetectorGeometry* HFgeom;

  edm::InputTag m_tag;
  bool m_skim;

  // HF info

  static const int      m_HFclus_MAX     = 5000;

  int    		m_nHF;
  int                   m_Etring[4];
  float                 m_Etring_real[4];
  float                 m_Ettower_real[4][18];
  float                 m_HF_asym[100];
  float                 m_HF_eta[m_HFclus_MAX];
  float                 m_HF_phi[m_HFclus_MAX];
  float                 m_HF_z[m_HFclus_MAX];
  float                 m_HF_e[m_HFclus_MAX];
  float                 m_HF_time[m_HFclus_MAX];
  float                 m_asymh;
  float                 m_mE_hfm;
  float                 m_mE_hfp;
  float                 m_E_hfm;
  float                 m_E_hfp;

};

#endif 
