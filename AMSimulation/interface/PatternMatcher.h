#ifndef AMSimulation_PatternMatcher_h_
#define AMSimulation_PatternMatcher_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTPattern.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Helper.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternBankOption.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/SuperstripArbiter.h"
using namespace slhcl1tt;

#include "fas/lean_lut3.h"
using namespace fas;

#include "TFile.h"
#include "TFileCollection.h"
#include "TChain.h"
#include "TTree.h"
#include "TString.h"


// SETTINGS: Majority logic
// INPUT   : TTree with moduleId, hitId, sim info + pattern bank
// OUTPUT  : Roads


class PatternMatcher {
  public:
    // Constructor
    PatternMatcher(PatternBankOption option)
    : po(option), nLayers_(po.nLayers), nDCBits_(po.nDCBits),
      bankName_("patternBank"), prefixRoad_("AMTTRoads_"), suffix_(""),
      nEvents_(999999999), minFrequency_(1), maxPatterns_(999999999), maxRoads_(999999999), maxHits_(999999999),
      verbose_(1),
      inputPatterns_fas_(0,0) {

        assert(3 <= nLayers_ && nLayers_ <= 8);
        assert(nDCBits_ <= 4);

        chain_ = new TChain("ntupler/tree");

        // Decide on the size of superstrip
        if (po.useSuperstripVarSize)
            arbiter_ = new SuperstripArbiter(po.subLadderVarSize, po.subModuleVarSize, po.subLadderECVarSize, po.subModuleECVarSize);
        else
            arbiter_ = new SuperstripArbiter(po.subLadderSize, po.subModuleSize);
    }

    // Destructor
    ~PatternMatcher() {
        delete arbiter_;
    }


    // Setters
    void setNEvents(long long n)    { if (n != -1)  nEvents_     = n > 0 ? n : 0; }
    void setMinFrequency(int n)     { minFrequency_ = n > 1 ? n : 1; }
    void setMaxPatterns(int n)      { if (n != -1)  maxPatterns_ = n > 0 ? n : 0; }
    void setMaxRoads(int n)         { if (n != -1)  maxRoads_    = n > 0 ? n : 0; }
    void setMaxHits(int n)          { if (n != -1)  maxHits_     = n > 0 ? n : 0; }
    void setVerbosity(int v)        { verbose_ = v; }

    // Getters
    // none

    // Functions
    int readFile(TString src);

    int readPatterns_vector(TString src);
    int makeRoads_vector(TString out);

    int readPatterns_fas(TString src);
    int makeRoads_fas(TString out);

    // Main driver
    int run(TString out, TString src, TString bank);

  private:
    // Private functions
    // none


  public:
    // Configurations
    const PatternBankOption po;

  private:
    // Configurations
    const unsigned nLayers_;
    const unsigned nDCBits_;  // UNUSED
    const TString bankName_;
    const TString prefixRoad_;
    const TString suffix_;

    // Program options
    long long nEvents_;
    int minFrequency_;  // min frequency of patterns to be read out
    int maxPatterns_;   // max number of patterns
    int maxRoads_;      // max number of roads per event
    int maxHits_;       // max number of hits per superstrip
    int verbose_;

    // Operators
    SuperstripArbiter * arbiter_;

    // Containers
    TChain * chain_;

    // Maps
    std::map<unsigned, std::vector<unsigned> > triggerTowerMap_;        // key: towerId, value: moduleIds in the tower
    std::map<unsigned, std::vector<unsigned> > triggerTowerReverseMap_; // key: moduleId, value: towerIds containing the module

    std::vector<pattern_type> inputPatterns_vector_;  // using std::vector approach
    fas::lean_lut3 inputPatterns_fas_;                // using fas::lean_lut3 approach
};

#endif
