#ifndef AMSimulation_TrackFitterAlgoBase_h_
#define AMSimulation_TrackFitterAlgoBase_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTSuperstrip.h"
//#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTTrack.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTTrack2.h"

using namespace slhcl1tt;

#include <iosfwd>
#include <vector>


class TrackFitterAlgoBase {
  public:
    TrackFitterAlgoBase() {}
    ~TrackFitterAlgoBase() {}

    int triggerTower;
    int event;

    void setEvent(int event_){event=event_;}
    void setTriggerTower(int triggerTower_){triggerTower=triggerTower_;}
    //virtual int fit(const std::vector<TTHit>& hits, TTTrack2& track) = 0;

};

#endif

