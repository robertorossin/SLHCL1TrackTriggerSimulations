#ifndef NTupleTools_NTupleGenMET_h_
#define NTupleTools_NTupleGenMET_h_

#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleCommon.h"


namespace reco { class GenMET; }

class NTupleGenMET : public edm::EDProducer {
  public:
    explicit NTupleGenMET(const edm::ParameterSet&);

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    const edm::InputTag inputTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<reco::GenMET> selector_;
    const unsigned int  maxN_;
};

#endif

