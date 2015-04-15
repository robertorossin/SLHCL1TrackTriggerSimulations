#ifndef AMSimulation_MatrixBuilder_h_
#define AMSimulation_MatrixBuilder_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Helper.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/ProgramOption.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TriggerTowerMap.h"
using namespace slhcl1tt;

#include "SLHCL1TrackTriggerSimulations/AMSimulation/external/Eigen/Core"


class MatrixBuilder {
  public:
    // Constructor
    MatrixBuilder(const ProgramOption& po)
    : po_(po),
      nEvents_(po.maxEvents), verbose_(po.verbose) {

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

    // Matrices
    Eigen::VectorXd sqrtEigenvalues_;
    Eigen::MatrixXd D_;
    Eigen::MatrixXd V_;
    Eigen::MatrixXd DV_;

};

#endif
