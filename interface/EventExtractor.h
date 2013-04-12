#ifndef EVENTEXTRACTOR_H
#define EVENTEXTRACTOR_H

/**
 * EventExtractor
 * \brief: Base class for extracting Event info
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Luminosity/interface/LumiDetails.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/Common/interface/TriggerResults.h"

//Include std C++
#include <iostream>
#include <vector>

#include "TMath.h"
#include "TTree.h"



class EventExtractor
{

 public:

  EventExtractor(edm::InputTag tag);
  ~EventExtractor();


  void writeInfo(const edm::LuminosityBlock *lumi); 
  void writeInfo(const edm::Event *event, bool MC); 
  void reset();
  bool isSelected();

 private:
  
  TTree* m_tree;
  edm::InputTag m_tag;

  bool  m_selected;

  int   m_evtID;
  int   m_BCID;
  int   m_time;
  int   m_offset;
  int   m_lumi;
  int   m_run; 

  int   m_phy_decl;          // Are we in PHYS mode for the event

  float m_lumi_1;            // Intensity of beam 1 bunch at event BCID
  float m_lumi_2;            // Intensity of beam 2 bunch at event BCID


  int   m_tech_trig_ind[64]; // L1 Technical triggers firing the event
  int   m_alg_trig_ind[128]; // L1 Algo triggers firing the event


  float i_bx_B1_now[3564];   // Beam1 intensity map for the event
  float i_bx_B2_now[3564];   // Beam2 intensity map for the event


  int   m_n_HLTs;
  std::vector< std::string > m_HLT_vector;


};

#endif 
