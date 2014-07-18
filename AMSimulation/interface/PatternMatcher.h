#ifndef AMSimulation_PatternMatcher_h_
#define AMSimulation_PatternMatcher_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTPattern.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Helper.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternBankOption.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/SuperstripArbiter.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/SuperstripHasher.h"
using namespace slhcl1tt;

#include "fas/lean_lut.h"
using namespace fas;

#include "TFile.h"
#include "TFileCollection.h"
#include "TChain.h"
#include "TTree.h"
#include "TString.h"


// SETTINGS: Majority logic
// INPUT   : TTree with moduleId, hitId, sim info + pattern bank
// OUTPUT  : Roads

struct genPart {
    float pt;
    float eta;
    float phi;
    int charge;
};

class PatternMatcher {
  public:
    // Constructor
    PatternMatcher(PatternBankOption option)
    : po(option), nLayers_(po.nLayers), nDCBits_(po.nDCBits),
      bankName_("patternBank"), prefixRoad_("AMTTRoads_"), suffix_(""),
      nEvents_(999999999), maxPatterns_(999999999), maxRoads_(999999999), maxHits_(999999999),
      verbose_(1) {

        assert(3 <= nLayers_ && nLayers_ <= 8);
        assert(0 <= nDCBits_ && nDCBits_ <= 4);

        chain_ = new TChain("ntupler/tree");

        // Decide on the size of superstrip
        if (po.useSuperstripVarSize)
            arbiter_ = new SuperstripArbiter(po.subLadderVarSize, po.subModuleVarSize, po.subLadderECVarSize, po.subModuleECVarSize);
        else
            arbiter_ = new SuperstripArbiter(po.subLadderSize, po.subModuleSize);

        // Encoding for each module uses 32 subLadders and 1024 subModules
        id_type subLadderNBits = mostSigBit(32/arbiter_->minSubLadderSize());
        id_type subModuleNBits = mostSigBit(1024/arbiter_->minSubModuleSize());
        hasher_ = new SuperstripHasher(subLadderNBits, subModuleNBits);
    }

    // Destructor
    ~PatternMatcher() {
        delete arbiter_;
        delete hasher_;
    }


    // Setters
    void setNEvents(long long n)    { if (n != -1)  nEvents_     = n > 0 ? n : 0; }
    void setMaxPatterns(int n)      { if (n != -1)  maxPatterns_ = n > 0 ? n : 0; }
    void setMaxRoads(int n)         { if (n != -1)  maxRoads_    = n > 0 ? n : 0; }
    void setMaxHits(int n)          { if (n != -1)  maxHits_     = n > 0 ? n : 0; }
    void setVerbosity(int v)        { verbose_ = v; }

    // Getters
    // none

    // Functions
    int readFile(TString src);

    int readPatterns_vector(TString src);
    int makeRoads_vector();
    int writeRoads_vector(TString out);

    int readPatterns(TString src);
    int makeRoads();
    int writeRoads(TString out);

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
    const int nLayers_;
    const int nDCBits_;  // UNUSED
    const TString bankName_;
    const TString prefixRoad_;
    const TString suffix_;

    // Program options
    long long nEvents_;
    int maxPatterns_;  // maximum number of patterns
    int maxRoads_;  // max number of roads per event
    int maxHits_;   // max number of hits per superstrip
    int verbose_;

    // Operators
    SuperstripArbiter* arbiter_;
    SuperstripHasher*  hasher_;

    // Containers
    TChain * chain_;
    std::vector<std::vector<TTRoad> > allRoads_;
    std::vector<std::vector<genPart> > allGenParts_;

    std::vector<pattern_type> inputPatterns_vector_;
    fas::lean_lut inputPatterns_;
};

#endif
