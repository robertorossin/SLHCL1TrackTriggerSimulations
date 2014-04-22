#ifndef NTupleTools_DummyProducer_h_
#define NTupleTools_DummyProducer_h_

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

class DummyProducer : public edm::EDProducer {
  public:
    explicit DummyProducer(const edm::ParameterSet&);

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

};

DummyProducer::DummyProducer(const edm::ParameterSet& iConfig) {
    produces<std::vector<float> > ("dummy");
}

void DummyProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
    std::auto_ptr<std::vector<float> > dummies(new std::vector<float>());

    dummies->push_back(-99.);
    iEvent.put(dummies, "dummy");
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(DummyProducer);

#endif
