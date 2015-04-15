#ifndef AMSimulation_TrackFitterAlgoPCA_h_
#define AMSimulation_TrackFitterAlgoPCA_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TrackFitterAlgoBase.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/ProgramOption.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PCA.h"
using namespace slhcl1tt;

#include "SLHCL1TrackTriggerSimulations/AMSimulation/external/Eigen/Core"


class TrackFitterAlgoPCA : public TrackFitterAlgoBase {
  public:
    TrackFitterAlgoPCA(const slhcl1tt::ProgramOption& po)
    : TrackFitterAlgoBase(),
      view_(PCA_3D), hitbits_(PCA_ALLHIT), nvariables_(12), nparameters_(4) {

        // Setup
        if (po.view == "XYZ" || po.view == "3D")
            view_ = PCA_3D;
        else if (po.view == "XY" || po.view == "RPHI")
            view_ = PCA_RPHI;
        else if (po.view == "RZ")
            view_ = PCA_RZ;

        hitbits_ = static_cast<PCA_HitBits>(po.hitbits);

        if (po.algo == "PCA4")
            nparameters_ = 4;
        else if (po.algo == "PCA5")
            nparameters_ = 5;

        if (view_ == PCA_3D) {
            if (hitbits_ == PCA_ALLHIT) {
                nvariables_ = 6 * 2;
            } else {
                nvariables_ = (6-1) * 2;
            }
        } else {
            if (hitbits_ == PCA_ALLHIT) {
                nvariables_ = 6 * 1;
            } else {
                nvariables_ = (6-1) * 1;
            }
        }

        // Book histograms
        bookHistograms();
    }

    ~TrackFitterAlgoPCA() {}

    int bookHistograms();

    int loadConstants(TString txt);

    int fit(const std::vector<TTHit>& hits, TTTrack2& track);

    unsigned nvariables()   const { return nvariables_; }
    unsigned nparameters()  const { return nparameters_; }
    void print();

  private:
    // Settings
    PCA_FitView view_;
    PCA_HitBits hitbits_;
    unsigned nvariables_;   // number of hit coordinates or principal components
    unsigned nparameters_;  // number of track parameters

    // Matrices
    Eigen::VectorXd sqrtEigenvalues_;
    Eigen::VectorXd meansV_;
    Eigen::VectorXd meansP_;
    Eigen::MatrixXd D_;
    Eigen::MatrixXd V_;
    Eigen::MatrixXd DV_;
};

#endif
