#ifndef AMSimulation_PatternMatcher_h_
#define AMSimulation_PatternMatcher_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/Pattern.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Helper.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/ProgramOption.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TriggerTowerMap.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/SuperstripArbiter.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/AssociativeMemory.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/HitBuffer.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/ModuleOverlapMap.h"
using namespace slhcl1tt;


class PatternMatcher {
  public:
    // Constructor
    PatternMatcher(const ProgramOption& po)
    : po_(po),
      nEvents_(po.maxEvents), verbose_(po.verbose), removeOverlap_(po.removeOverlap),
      prefixRoad_("AMTTRoads_"), suffix_("") {

        // Initialize
        ttmap_ = new TriggerTowerMap();
        ttmap_->read(po_.datadir);

        arbiter_ = new SuperstripArbiter();
        arbiter_->setDefinition(po_.superstrip, po_.tower, ttmap_);

        if (removeOverlap_) {
        	momap_   = new ModuleOverlapMap();
        	momap_->readModuleOverlapMap(po_.datadir);
        }
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

    // Load pattern bank
    int loadPatterns(TString bank);

    // Do pattern recognition, write roads (patterns that fired)
    int makeRoads(TString src, TString out);

    // Program options
    const ProgramOption po_;
    long long nEvents_;
    int verbose_;
    bool removeOverlap_;

    // Configurations
    const TString prefixRoad_;
    const TString suffix_;

    // Operators
    TriggerTowerMap   * ttmap_;
    SuperstripArbiter * arbiter_;
    ModuleOverlapMap  * momap_;

    // Associative memory
    AssociativeMemory associativeMemory_;

    // Hit buffer
    HitBuffer hitBuffer_;
};

#endif
