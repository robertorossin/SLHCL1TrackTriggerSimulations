#ifndef NTupleTools_NTupleTTStubs_h_
#define NTupleTools_NTupleTTStubs_h_

#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleCommon.h"
#include "DataFormats/L1TrackTrigger/interface/TTCluster.h"
#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"


class NTupleTTStubs : public edm::EDProducer {
  public:
    explicit NTupleTTStubs(const edm::ParameterSet&);

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    const edm::InputTag inputTag_, inputTagMC_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<TTCluster<Ref_PixelDigi_> > selector_;
    const unsigned maxN_;
};

#endif
