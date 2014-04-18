#ifndef NTupleTools_NTupleGenMET_h_
#define NTupleTools_NTupleGenMET_h_

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

class NTupleGenMET : public edm::EDProducer {
  public:
    explicit NTupleGenMET(const edm::ParameterSet&);

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    const edm::InputTag inputTag_;
    const std::string   prefix_, suffix_;
};

#endif
