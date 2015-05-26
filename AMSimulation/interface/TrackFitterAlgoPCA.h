#ifndef AMSimulation_TrackFitterAlgoPCA_h_
#define AMSimulation_TrackFitterAlgoPCA_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TrackFitterAlgoBase.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/ProgramOption.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PCAMatrix.h"

static const unsigned PCA_NSEGMENTS = 10;     // 10 segments in invPt
static const unsigned PCA_NHITBITS  = 7;      // 7 layer combinations
static const float    PCA_MAX_INVPT = +1./2;  // 2 GeV
static const float    PCA_MIN_INVPT = -1./2;  // 2 GeV


namespace slhcl1tt {

class TrackFitterAlgoPCA : public TrackFitterAlgoBase {
  public:
    TrackFitterAlgoPCA(const slhcl1tt::ProgramOption& po)
    : TrackFitterAlgoBase(), datadir_(po.datadir), tower_(po.tower), verbose_(po.verbose),
      view_(XYZ), nvariables_(12), nparameters_(4) {

        // Determine # of variables and # of parameters
        if (po.view == "XYZ") {
            view_ = XYZ;
            nvariables_ = 6 * 2;
            if (po.algo == "PCA4") {
                nparameters_ = 4;
            } else if (po.algo == "PCA5") {
                nparameters_ = 5;
            }

        } else if (po.view == "XY") {
            view_ = XY;
            nvariables_ = 6;
            if (po.algo == "PCA4") {
                nparameters_ = 2;
            } else if (po.algo == "PCA5") {
                nparameters_ = 3;
            }

        } else if (po.view == "RZ") {
            view_ = RZ;
            nvariables_ = 6;
            nparameters_ = 2;
        }

        // Book histograms
        bookHistograms();

        loadConstants();
    }

    ~TrackFitterAlgoPCA() {}

    int fit(const TTRoadComb& acomb, TTTrack2& atrack);

    unsigned nvariables()   const { return nvariables_; }
    unsigned nparameters()  const { return nparameters_; }
    void print();

  private:
    int bookHistograms();

    int loadConstants();

    // Settings
    std::string datadir_;
    unsigned    tower_;
    int         verbose_;

    FitView     view_;
    unsigned    nvariables_;   // number of hit coordinates or principal components
    unsigned    nparameters_;  // number of track parameters

    // Matrices
    std::vector<PCAMatrix> matrices;
};

}  // namespace slhcl1tt

#endif
