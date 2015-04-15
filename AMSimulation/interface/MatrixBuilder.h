#ifndef AMSimulation_MatrixBuilder_h_
#define AMSimulation_MatrixBuilder_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Helper.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/ProgramOption.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TriggerTowerMap.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PCA.h"
using namespace slhcl1tt;

#include "SLHCL1TrackTriggerSimulations/AMSimulation/external/Eigen/Core"


class MatrixBuilder {
  public:
    // Constructor
    MatrixBuilder(const ProgramOption& po)
    : po_(po),
      nEvents_(po.maxEvents), verbose_(po.verbose),
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

        // Initialize
        ttmap_   = new TriggerTowerMap();
    }

    // Destructor
    ~MatrixBuilder() {
        if (ttmap_)     delete ttmap_;
    }

    // Main driver
    int run();


  private:
    // Member functions
    // Setup trigger tower
    int setupTriggerTower(TString datadir);

    // Build matrices
    int buildMatrices(TString src);

    // Write matrices
    int writeMatrices(TString out);

    // Program options
    const ProgramOption po_;
    long long nEvents_;
    int verbose_;

    // Operators
    TriggerTowerMap   * ttmap_;

    // Settings
    PCA_FitView view_;
    PCA_HitBits hitbits_;
    unsigned nvariables_;   // number of hit coordinates or principal components
    unsigned nparameters_;  // number of track parameters

    // Matrices
    Eigen::VectorXd shifts_;
    Eigen::VectorXd sqrtEigenvalues_;
    Eigen::MatrixXd D_;
    Eigen::MatrixXd V_;
    Eigen::MatrixXd DV_;
};

#endif
