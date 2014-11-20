#ifndef AMSimulation_TrackFitterAlgoLinearized_h_
#define AMSimulation_TrackFitterAlgoLinearized_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TrackFitterAlgoBase.h"
using namespace slhcl1tt;


class TrackFitterAlgoLinearized : TrackFitterAlgoBase {
  public:
    TrackFitterAlgoLinearized() {}

    ~TrackFitterAlgoLinearized() {}

    TTTrackParam fit(const std::vector<TTHit>& hits) const;
};

#endif

