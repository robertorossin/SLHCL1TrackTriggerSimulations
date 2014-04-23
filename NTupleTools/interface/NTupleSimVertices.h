#ifndef NTupleTools_NTupleSimVertices_h_
#define NTupleTools_NTupleSimVertices_h_

#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleCommon.h"


class SimVertex;

class NTupleSimVertices : public edm::EDProducer {
  public:
    explicit NTupleSimVertices(const edm::ParameterSet&);

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    const edm::InputTag inputTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<SimVertex> selector_;
    const unsigned maxN_;
};

#endif
