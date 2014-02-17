#ifndef RecoExtractor_h
#define RecoExtractor_h

/** \class RecoExtractor
 *  \author by sviret
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"

#include "../interface/PixelExtractor.h"
#include "../interface/StubExtractor.h"
#include "../interface/L1TrackExtractor.h"
#include "../interface/MCExtractor.h"
#include "../interface/L1TrackTrigger_analysis.h"
#include "../interface/StubTranslator.h"
#include "../interface/AnalysisSettings.h"

#include "TFile.h"
#include "TRFIOFile.h"

class RecoExtractor : public edm::EDAnalyzer{
 public:
  /// Constructor
  RecoExtractor(const edm::ParameterSet& pset);

  /// Destructor
  virtual ~RecoExtractor(){ }
   int nevent;
  int nevent_tot;

  /// Method called before the event loop
  void beginJob();
  void endJob();

  void beginRun(edm::Run const&, edm::EventSetup const&);
  void endRun(edm::Run const&, edm::EventSetup const&);

  /// Method called once per event
  void analyze(const edm::Event&, const edm::EventSetup& );

  void fillInfo(const edm::Event *event);
  void getInfo(int ievent);
  void initialize();
  void retrieve();
  void doAna();
  


 private:
 
  bool do_fill_;

  bool do_PIX_;
  bool do_MC_;
  bool do_STUB_;
  bool do_L1TRK_;
  bool do_BANK_;
  bool do_MATCH_;
  bool do_L1tt_;

  int  nevts_;
  int  skip_;

  std::string CLUS_tag;
  std::string STUB_tag;

  std::string CLUS_name;
  std::string STUB_name;

  edm::InputTag PIX_tag_;  // 
  edm::InputTag MC_tag_;  // 

  //
  // Definition of root-tuple :
  //

  std::string outFilename_;
  std::string inFilename_;

  std::vector<std::string> m_settings_;

  TFile* m_dummyfile;
  TFile* m_infile;
  TFile* m_outfile;


  PixelExtractor*           m_PIX;
  MCExtractor*              m_MC;
  StubExtractor*            m_STUB;
  L1TrackExtractor*         m_L1TRK;
  StubTranslator*           m_BK;
  AnalysisSettings*         m_ana_settings;
  L1TrackTrigger_analysis*  m_L1TT_analysis;

};


#endif
