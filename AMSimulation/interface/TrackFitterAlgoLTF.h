#ifndef AMSimulation_TrackFitterAlgoLTF_h_
#define AMSimulation_TrackFitterAlgoLTF_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TrackFitterAlgoBase.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/ProgramOption.h"
#include "LinearizedTrackFit/LinearizedTrackFit/interface/LinearizedTrackFitter.h"

namespace slhcl1tt {

class TrackFitterAlgoLTF : public TrackFitterAlgoBase {
  public:
    TrackFitterAlgoLTF(const slhcl1tt::ProgramOption& po)
    : TrackFitterAlgoBase(), verbose_(po.verbose) {}

    ~TrackFitterAlgoLTF() {}

    int fit(const TTRoadComb& acomb, TTTrack2& atrack);

  private:
    LinearizedTrackFitter linearizedTrackFitter_;
    int verbose_;
};

}  // namespace slhcl1tt

#endif
