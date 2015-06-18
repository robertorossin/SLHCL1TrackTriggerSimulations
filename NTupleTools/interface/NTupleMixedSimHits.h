#ifndef NTupleTools_NTupleMixedSimHits_h_
#define NTupleTools_NTupleMixedSimHits_h_

#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleCommon.h"


class PSimHit;

class NTupleMixedSimHits : public edm::EDProducer {
  public:
    explicit NTupleMixedSimHits(const edm::ParameterSet&);

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    const edm::InputTag         inputTag_;
    edm::ParameterSet           simHitCollectionConfig_;
    std::vector<edm::InputTag>  simHitCollections_;
    const std::string           prefix_, suffix_;

    StringCutObjectSelector<PSimHit> selector_;
    const unsigned maxN_;
};

#endif
