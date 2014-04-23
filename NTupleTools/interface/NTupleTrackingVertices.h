#ifndef NTupleTools_NTupleTrackingVertices_h_
#define NTupleTools_NTupleTrackingVertices_h_

#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleCommon.h"


class TrackingVertex;

class NTupleTrackingVertices : public edm::EDProducer {
  public:
    explicit NTupleTrackingVertices(const edm::ParameterSet&);

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    const edm::InputTag inputTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<TrackingVertex> selector_;
    const unsigned maxN_;
};

#endif
