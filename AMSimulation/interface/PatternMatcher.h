#ifndef AMSimulation_PatternMatcher_h_
#define AMSimulation_PatternMatcher_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/Pattern.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Helper.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/ProgramOption.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TriggerTowerMap.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/SuperstripArbiter.h"
using namespace slhcl1tt;


class PatternMatcher {
  public:
    // Constructor
    PatternMatcher(ProgramOption po)
    : po_(po),
      nEvents_(po.maxEvents), verbose_(po.verbose),
      prefixRoad_("AMTTRoads_"), suffix_("") {

        // Initialize
        ttmap_   = new TriggerTowerMap();
        arbiter_ = new SuperstripArbiter();
    }

    // Destructor
    ~PatternMatcher() {
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

    // Load pattern bank
    int loadPatterns(TString bank);

    // Do pattern recognition, write roads (patterns that fired)
    int makeRoads(TString src, TString out);

    // Program options
    const ProgramOption po_;
    long long nEvents_;
    int verbose_;

    // Configurations
    const TString prefixRoad_;
    const TString suffix_;

    // Operators
    TriggerTowerMap   * ttmap_;
    SuperstripArbiter * arbiter_;

    // Pattern bank data
    std::vector<pattern_type> patternBank_;

    // Hit buffer
    std::map<unsigned, std::vector<unsigned> > hitBuffer_;
    std::vector<bool>                          hitBuffer_bool_;
};

#endif
