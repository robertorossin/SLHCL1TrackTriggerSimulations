#ifndef NTupleTools_NTupleBeamSpot_h_
#define NTupleTools_NTupleBeamSpot_h_

#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleCommon.h"


namespace reco { class BeamSpot; }

class NTupleBeamSpot : public edm::EDProducer {
  public:
    explicit NTupleBeamSpot(const edm::ParameterSet&);

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
