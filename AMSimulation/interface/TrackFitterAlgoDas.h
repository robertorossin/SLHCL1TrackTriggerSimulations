#ifndef AMSimulation_TrackFitterAlgoDas_h_
#define AMSimulation_TrackFitterAlgoDas_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TrackFitterAlgoBase.h"
using namespace slhcl1tt;

class TrackFitterAlgoDas : public TrackFitterAlgoBase {
  public:
    slhcl1tt::TTTrack* fit(std::vector<float> &v_hitPhis, std::vector<float> &v_hitRs, std::vector<float> &v_hitZs);
};

#endif
