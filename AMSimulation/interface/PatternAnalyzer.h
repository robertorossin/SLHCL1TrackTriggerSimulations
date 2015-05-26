#ifndef AMSimulation_PatternAnalyzer_h_
#define AMSimulation_PatternAnalyzer_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/Pattern.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Helper.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/ProgramOption.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TriggerTowerMap.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/SuperstripArbiter.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Attributes.h"
using namespace slhcl1tt;

#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"


class PatternAnalyzer {
  public:
    // Constructor
    PatternAnalyzer(const ProgramOption& po)
    : po_(po),
      nEvents_(po.maxEvents), verbose_(po.verbose) {

        // Initialize
        ttmap_ = new TriggerTowerMap();
        ttmap_->read(po_.datadir);

        arbiter_ = new SuperstripArbiter();
        arbiter_->setDefinition(po_.superstrip, po_.tower, ttmap_);
    }

    // Destructor
    ~PatternAnalyzer() {
        if (ttmap_)     delete ttmap_;
        if (arbiter_)   delete arbiter_;
    }

    // Main driver
    int run();


  private:
    // Member functions

    // Load pattern bank
    int loadPatterns(TString bank);

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
    std::vector<Attributes>                             patternAttributes_;
    std::map<pattern_type, Attributes *>                patternAttributes_map_;
    std::vector<std::pair<pattern_type, Attributes *> > patternAttributes_pairs_;

    // Histograms
    std::map<TString, TH1F *>     histograms;
    std::map<TString, TH2F *>     histogram2Ds;
    std::map<TString, TProfile *> histogramPRs;
};

#endif
