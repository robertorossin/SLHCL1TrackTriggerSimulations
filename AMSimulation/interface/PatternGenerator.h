#ifndef AMSimulation_PatternGenerator_h_
#define AMSimulation_PatternGenerator_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/Pattern.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Helper.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/ProgramOption.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TriggerTowerMap.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/SuperstripArbiter.h"
using namespace slhcl1tt;


class PatternGenerator {
  public:
    // Constructor
    PatternGenerator(const ProgramOption& po)
    : po_(po),
      nEvents_(po.maxEvents), verbose_(po.verbose) {

        // Initialize
        ttmap_   = new TriggerTowerMap();
        arbiter_ = new SuperstripArbiter();
    }

    // Destructor
    ~PatternGenerator() {
        if (ttmap_)     delete ttmap_;
        if (arbiter_)   delete arbiter_;
    }

    // Main driver
    int run();


  private:
    // Member functions
    // Setup trigger tower and superstrip definitions
    int setupTriggerTower(TString datadir);
    int setupSuperstrip();

    // Generate pattern bank
    int makePatterns(TString src);

    // Write pattern bank
    int writePatterns(TString out);

    // Program options
    const ProgramOption po_;
    long long nEvents_;
    int verbose_;

    // Operators
    TriggerTowerMap   * ttmap_;
    SuperstripArbiter * arbiter_;

    // Pattern bank data
    std::map<pattern_type, unsigned>                patternBank_map_;
    std::vector<std::pair<pattern_type, unsigned> > patternBank_pairs_;

    // Bookkeepers
    float coverage_;
    unsigned coverage_count_;
};

#endif
