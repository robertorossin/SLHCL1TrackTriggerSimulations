#ifndef NTupleTools_NTupleSimBeamSpot_h_
#define NTupleTools_NTupleSimBeamSpot_h_

#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleCommon.h"


namespace reco { class BeamSpot; }

class NTupleSimBeamSpot : public edm::EDProducer {
  public:
    explicit NTupleSimBeamSpot(const edm::ParameterSet&);

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    const edm::InputTag inputTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<reco::BeamSpot> selector_;
    const unsigned maxN_;
};

#endif
