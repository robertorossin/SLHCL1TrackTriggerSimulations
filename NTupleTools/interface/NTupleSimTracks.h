#ifndef NTupleTools_NTupleSimTracks_h_
#define NTupleTools_NTupleSimTracks_h_

#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleCommon.h"


class SimTrack;

class NTupleSimTracks : public edm::EDProducer {
  public:
    explicit NTupleSimTracks(const edm::ParameterSet&);

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    const edm::InputTag inputTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<SimTrack> selector_;
    const unsigned maxN_;
};

#endif
