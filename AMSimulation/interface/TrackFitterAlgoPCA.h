#ifndef AMSimulation_TrackFitterAlgoPCA_h_
#define AMSimulation_TrackFitterAlgoPCA_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TrackFitterAlgoBase.h"
using namespace slhcl1tt;


class TrackFitterAlgoPCA : public TrackFitterAlgoBase {
  public:
    TrackFitterAlgoPCA(bool fiveParameters=false) : TrackFitterAlgoBase(), fiveParameters_(fiveParameters) {}

    ~TrackFitterAlgoPCA() {}

    int fit(const std::vector<TTHit>& hits, TTTrack2& track);

  private:
    bool fiveParameters_;
};

#endif

