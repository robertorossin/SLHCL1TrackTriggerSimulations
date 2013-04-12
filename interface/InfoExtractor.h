#ifndef INFOEXTRACTOR_H
#define INFOEXTRACTOR_H

/**
 * InfoExtractor
 * \brief: Base class for extracting Info info
 */

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"

#include "DataFormats/Luminosity/interface/LumiDetails.h"
#include "DataFormats/Common/interface/TriggerResults.h"


#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"

//Include std C++
#include <iostream>
#include <vector>

#include "TMath.h"
#include "TTree.h"

class InfoExtractor
{

 public:

  InfoExtractor();
  ~InfoExtractor();

  void init(const edm::Run *run, const edm::EventSetup *setup); 
  void writeInfo(const edm::Event *event, const edm::EventSetup *setup); 
  void writeInfo(const edm::LuminosityBlock *lumi);
  void reset();
  void fillTree(int nevt);
  void fillPSTree();

 private:
  
  TTree* m_tree;
  TTree* m_tree_ps;

  int nevent;

  HLTConfigProvider   hltConfig;
  L1GtUtils           m_l1GtUtils;

  int   m_run;               // Run number (we are working on fill basis now)
  int   m_nLB;               // Number of lumi blocks (LB) analyzed
  int   m_LBnum;             //
  int   m_LBmin;             // First LB number
  int   m_LBmax;             // Last LB number
  int   m_tech_trig_pre[64]; // L1 Prescale factors for technical bits 
  int   m_tech_trig_act[64]; // L1 technical bits activation info 
  int   m_alg_trig_pre[128]; // L1 Prescale factors for algo bits 
  int   m_alg_trig_act[128]; // L1 algo bits activation info 

  int   m_HLT3_p;            // HLT prescale factor for algo bit 3
  int   m_HLT4_p;            // " 4 
  int   m_HLT5_p;            // " 5
  int   m_HLT6_p;            // " 6
  int   m_HLT8_p;            // " 8
  float i_bx_B1[3564]; // BX intensities for beam 1 
  float i_bx_B2[3564]; // BX intensities for beam 2 

  int   m_n_HLTs;
  std::vector< std::string > m_HLT_vector;
  std::vector< int > m_HLT_prescales;
};

#endif 
