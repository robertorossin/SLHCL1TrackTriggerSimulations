#ifndef AMSimulation_PatternMatcher_h_
#define AMSimulation_PatternMatcher_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTPattern.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternBankOption.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/SuperstripArbiter.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/SuperstripHasher.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/HelperMath.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Helper.h"
using namespace slhcl1tt;

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
};

class PatternMatcher {
  public:
    // Constructor
    PatternMatcher(PatternBankOption option)
    : po(option), nLayers_(po.nLayers), nDCBits_(po.nDCBits),
      prefixRoad_("AMTTRoads_"), suffix_(""),
      nEvents_(999999999), maxPatterns_(999999999), maxRoads_(999999999), maxHits_(999999999), verbose_(1),
      ptr_allPatternIds_(0), ptr_allPatternBits_(0), size_allPatternIds_(0), size_allPatternBits_(0) {

        assert(3 <= nLayers_ && nLayers_ <= 8);
        assert(0 <= nDCBits_ && nDCBits_ <= 4);

        chain_ = new TChain("ntupler/tree");

        // Decide on the size of superstrip
        if (!po.subLadderVarSize.empty() && !po.subModuleVarSize.empty())
            superstripArbiter_ = new SuperstripArbiter(6, po.subLadderVarSize, po.subModuleVarSize);
        else
            superstripArbiter_ = new SuperstripArbiter(6, po.subLadderSize, po.subModuleSize);

        id_type subLadderNBits = msb(32/superstripArbiter_->minSubLadderSize());
        id_type subModuleNBits = msb(1024/superstripArbiter_->minSubModuleSize());
        superstripHasher_ = new SuperstripHasher(subLadderNBits, subModuleNBits);
    }

    // Destructor
    ~PatternMatcher() {
        delete ptr_allPatternIds_;
        delete ptr_allPatternBits_;

        delete superstripArbiter_;
        delete superstripHasher_;
    }

    // Setters
    //void setNLayers(int n)          { nLayers_ = n; }
    //void setNDCBits(int n)          { nDCBits_ = n; }

    void setNEvents(int n)          { if (n != -1)  nEvents_ = std::max(0, n); }
    void setMaxPatterns(int n)      { if (n != -1)  maxPatterns_ = std::max(0, n); }
    void setMaxRoads(int n)         { if (n != -1)  maxRoads_ = std::max(0, n); }
    void setMaxHits(int n)          { if (n != -1)  maxHits_ = std::max(0, n); }
    void setVerbosity(int n)        { verbose_ = n; }

    // Getters
    int getNLayers()          const { return nLayers_; }
    int getNDCBits()          const { return nDCBits_; }

    // Functions
    int readPatterns(TString src);

    int loadPatterns();

    int readFile(TString src);

    int makeRoads();

    int writeRoads(TString out);

    // Main driver
    int run(TString out, TString src, TString bank);

  private:
    void constructPatternFromArray(unsigned index, TTPattern& pattern);

    void clearHashMap();


  public:
    // Configurations
    const PatternBankOption po;

  private:
    // Configurations
    const int nLayers_;
    const int nDCBits_;
    const TString prefixRoad_;
    const TString suffix_;

    // Program options
    int nEvents_;
    int maxPatterns_;  // maximum number of patterns -- bad if the patternBank is not properly sorted
    int maxRoads_;  // max number of roads per event
    int maxHits_;   // max number of hits per road
    int verbose_;

    // Containers
    TChain * chain_;
    std::vector<std::vector<TTRoad> > allRoads_;
    std::vector<std::vector<genPart> > allGenParts_;

    addr_type* ptr_allPatternIds_;  // serialize patternIds to achieve minimum memory usage
    bit_type* ptr_allPatternBits_;  // serialize patternBits to achieve minimum memory usage
    unsigned size_allPatternIds_;   // keep track of the size of ptr_allPatternIds_
    unsigned size_allPatternBits_;  // keep track of the size of ptr_allPatternBits_

    std::vector<std::vector<unsigned> > ssHashMap_;

    SuperstripArbiter* superstripArbiter_;
    SuperstripHasher*  superstripHasher_;
};

#endif
