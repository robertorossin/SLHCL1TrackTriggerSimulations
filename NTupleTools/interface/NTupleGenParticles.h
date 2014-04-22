#ifndef NTupleTools_NTupleGenParticles_h_
#define NTupleTools_NTupleGenParticles_h_

#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleCommon.h"


namespace reco { class GenParticle; }

class NTupleGenParticles : public edm::EDProducer {
  public:
    explicit NTupleGenParticles(const edm::ParameterSet&);

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    const edm::InputTag inputTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<reco::GenParticle> selector_;
    const unsigned maxN_;
};

#endif
