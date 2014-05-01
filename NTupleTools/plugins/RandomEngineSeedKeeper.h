#ifndef NTupleTools_RandomEngineSeedKeeper_h_
#define NTupleTools_RandomEngineSeedKeeper_h_

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

namespace edm {
  class ConfigurationDescriptions;
}

class RandomEngineSeedKeeper : public edm::EDProducer {
  public:
    explicit RandomEngineSeedKeeper(edm::ParameterSet const& iConfig);
    ~RandomEngineSeedKeeper();
    //static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  private:
    virtual void produce(edm::Event&, const edm::EventSetup&);
    std::vector<std::string> seeds_;
};

#endif

