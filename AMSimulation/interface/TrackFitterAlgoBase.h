#ifndef AMSimulation_TrackFitterAlgoBase_h_
#define AMSimulation_TrackFitterAlgoBase_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTRoadComb.h"
//#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTTrack.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTTrack2.h"

#include "TString.h"
#include "TH1F.h"
#include <iosfwd>
#include <map>
#include <string>
#include <vector>


class TrackFitterAlgoBase {
  public:
    TrackFitterAlgoBase() {}
    ~TrackFitterAlgoBase() {}

    //virtual int fit(const TTRoadComb& aroad, std::vector<TTTrack2>& tracks) = 0;
    //virtual int fit(const TTRoadComb& acomb, TTTrack2& track) = 0;

    // Histograms
    std::map<TString, TH1F *>     histograms;
};

#endif

