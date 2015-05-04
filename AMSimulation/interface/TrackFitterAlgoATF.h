/* --- An Analytical Track Fitter for rapid least-chi2 fitting ---

Algorithm described in CMS Detector Note DN-2015/004
cms.cern.ch/iCMS/jsp/openfile.jsp?tp=draft&files=DN2015_004_v2.pdf

Souvik Das, University of Florida
15 October 2014
--- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- */

#ifndef AMSimulation_TrackFitterAlgoATF_h_
#define AMSimulation_TrackFitterAlgoATF_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TrackFitterAlgoBase.h"

namespace slhcl1tt {

class TrackFitterAlgoATF : public TrackFitterAlgoBase
{
  public:
    TrackFitterAlgoATF(bool fiveParameters=false) : TrackFitterAlgoBase(), fiveParameters_(fiveParameters) {}
    ~TrackFitterAlgoATF() {}

    int fit(const TTRoadComb& acomb, TTTrack2& track);

  private:
    bool fiveParameters_;
};

}  // namespace slhcl1tt

#endif
