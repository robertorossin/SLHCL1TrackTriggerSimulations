#ifndef AMSimulation_MatrixBuilder_h_
#define AMSimulation_MatrixBuilder_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Helper.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternBankOption.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TriggerTowerMap.h"
using namespace slhcl1tt;


class MatrixBuilder {
  public:
    // Constructor
    MatrixBuilder(PatternBankOption po)
    : po_(po),
      nEvents_(999999999), verbose_(1) {

        // Initialize
        ttmap_   = new TriggerTowerMap();
    }

    // Destructor
    ~MatrixBuilder() {
        if (ttmap_)     delete ttmap_;
    }


    // Setters
    void setNEvents(long long n)    { if (n != -1)  nEvents_ = n > 0 ? n : 0; }
    void setVerbosity(int v)        { verbose_ = v; }

    // Getters
    // none

    // Main driver
    int run(TString src, TString datadir, TString out);


  private:
    // Member functions
    // Setup trigger tower and superstrip definitions
    int setupTriggerTower(TString datadir);

    // Build matrices
    int buildMatrices(TString src);

    // Write matrices
    int writeMatrices(TString out);

    // Configurations
    const PatternBankOption po_;

    // Program options
    long long nEvents_;
    int verbose_;

    // Operators
    TriggerTowerMap   * ttmap_;

};

#endif
