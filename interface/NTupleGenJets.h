#ifndef NTupleTools_NTupleGenJets_h_
#define NTupleTools_NTupleGenJets_h_

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

class NTupleGenJets : public edm::EDProducer {
 public:
  explicit NTupleGenJets(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  const edm::InputTag   inputTag;
  const std::string     prefix,suffix;
  const double minPt, maxAbsoluteEta;
  const unsigned int    maxSize;
};

#endif
