#ifndef NTupleTools_NTupleEventInfo_h_
#define NTupleTools_NTupleEventInfo_h_

#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleCommon.h"


class NTupleEventInfo : public edm::EDProducer {
  public:
    explicit NTupleEventInfo(const edm::ParameterSet&);

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    const std::string   prefix_, suffix_;
};

#endif

