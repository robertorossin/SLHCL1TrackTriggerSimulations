#ifndef AMSimulation_PatternAnalyzer_h_
#define AMSimulation_PatternAnalyzer_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/Pattern.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Helper.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternBankOption.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TriggerTowerMap.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/SuperstripArbiter.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Attributes.h"
using namespace slhcl1tt;

#include "TH1F.h"
#include "TH2F.h"


class PatternAnalyzer {
  public:
    // Constructor
    PatternAnalyzer(PatternBankOption po)
    : po_(po),
      nEvents_(999999999), minFrequency_(1), maxPatterns_(999999999), verbose_(1) {

        // Initialize
        ttmap_   = new TriggerTowerMap();
        arbiter_ = new SuperstripArbiter();
    }

    // Destructor
    ~PatternAnalyzer() {
        if (ttmap_)     delete ttmap_;
        if (arbiter_)   delete arbiter_;
    }


    // Setters
    void setNEvents(long long n)    { if (n != -1)  nEvents_ = n > 0 ? n : 0; }
    void setMinFrequency(int n)     { minFrequency_ = n > 1 ? n : 1; }
    void setMaxPatterns(int n)      { if (n != -1)  maxPatterns_ = n > 0 ? n : 0; }
    void setVerbosity(int v)        { verbose_ = v; }

    // Getters
    // none

    // Main driver
    int run(TString src, TString bank, TString datadir, TString out);


  private:
    // Member functions
    // Setup trigger tower and superstrip definitions
    int setupTriggerTower(TString datadir);
    int setupSuperstrip();

    // Load pattern bank
    int loadPatterns(TString bank);

    // Generate pattern bank
    int makePatterns(TString src);

    // Write pattern bank
    int writePatterns(TString out);

    // Configurations
    const PatternBankOption po_;

    // Program options
    long long nEvents_;
    int minFrequency_;  // min frequency of patterns to be read out
    int maxPatterns_;   // max number of patterns
    int verbose_;

    // Operators
    TriggerTowerMap   * ttmap_;
    SuperstripArbiter * arbiter_;

    // Pattern bank data
    std::vector<Attributes>                             patternAttributes_;
    std::map<pattern_type, Attributes *>                patternBank_map_;
    std::vector<std::pair<pattern_type, Attributes *> > patternBank_pairs_;

    // Histograms
    std::map<TString, TH1F *>  histograms;
    std::map<TString, TH2F *>  histogram2Ds;
};

#endif
