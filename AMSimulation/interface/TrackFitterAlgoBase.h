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

namespace slhcl1tt {

enum FitView { XYZ=0, XY, RZ };

//enum HitBits { HITALL=0, MISSL0, MISSL1, MISSL2, MISSL3, MISSL4, MISSL5 };

class TrackFitterAlgoBase {
  public:
    TrackFitterAlgoBase() {}
    virtual ~TrackFitterAlgoBase() {}

    virtual int fit(const TTRoadComb& acomb, TTTrack2& atrack) = 0;

    // Histograms
    std::map<TString, TH1F *>  histograms_;
};

}  // namespace slhcl1tt

#endif

