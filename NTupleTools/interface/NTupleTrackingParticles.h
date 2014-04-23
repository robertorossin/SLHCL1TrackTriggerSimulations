#ifndef NTupleTools_NTupleTrackingParticles_h_
#define NTupleTools_NTupleTrackingParticles_h_

#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleCommon.h"


class TrackingParticle;

class NTupleTrackingParticles : public edm::EDProducer {
  public:
    explicit NTupleTrackingParticles(const edm::ParameterSet&);

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    const edm::InputTag inputTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<TrackingParticle> selector_;
    const unsigned maxN_;
};

#endif
