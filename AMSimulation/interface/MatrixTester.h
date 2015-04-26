#ifndef AMSimulation_MatrixTester_h_
#define AMSimulation_MatrixTester_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Helper.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/ProgramOption.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TriggerTowerMap.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TrackFitterAlgoPCA.h"
using namespace slhcl1tt;


class MatrixTester {
  public:
    // Constructor
    MatrixTester(const ProgramOption& po)
    : po_(po),
      nEvents_(po.maxEvents), verbose_(po.verbose) {

        // Initialize
        ttmap_   = new TriggerTowerMap();

        fitterPCA_ = new TrackFitterAlgoPCA(po);
        nvariables_ = fitterPCA_ -> nvariables();
        nparameters_ = fitterPCA_ -> nparameters();
    }

    // Destructor
    ~MatrixTester() {
        if (ttmap_)     delete ttmap_;
        if (fitterPCA_) delete fitterPCA_;
    }

    // Main driver
    int run();


  private:
    // Member functions
    // Setup trigger tower
    int setupTriggerTower(TString datadir);

    // Build matrices
    int testMatrices(TString src);

    // Program options
    const ProgramOption po_;
    long long nEvents_;
    int verbose_;

    // Operators
    TriggerTowerMap   * ttmap_;

    // Track fitters
    TrackFitterAlgoPCA *    fitterPCA_;
    unsigned nvariables_;
    unsigned nparameters_;
};

#endif
