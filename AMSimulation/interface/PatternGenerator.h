#ifndef AMSimulation_PatternGenerator_h_
#define AMSimulation_PatternGenerator_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTPattern.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Helper.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternBankOption.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/SuperstripArbiter.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/SuperstripStitcher.h"
using namespace slhcl1tt;

#include "fas/lean_table3.h"
using namespace fas;

#include "TFile.h"
#include "TFileCollection.h"
#include "TChain.h"
#include "TTree.h"
#include "TString.h"


// SETTINGS: Superstrip size, # of fake superstrips, # of DC bits
// INPUT   : TTree with moduleId, hitId, sim info + trigger tower file
// OUTPUT  : Pattern bank

class PatternGenerator {
  public:
    // Constructor
    PatternGenerator(PatternBankOption option)
    : po(option), nLayers_(po.nLayers), nDCBits_(po.nDCBits), nFakeSuperstrips_(po.nFakeSuperstrips),
      bankName_("patternBank"),
      nEvents_(999999999), minFrequency_(1),
      verbose_(1),
      allPatterns_fas_(0,0) {

        assert(3 <= nLayers_ && nLayers_ <= 8);
        assert(0 <= nDCBits_ && nDCBits_ <= 4);
        assert(0 <= nFakeSuperstrips_ && nFakeSuperstrips_ <= 3);

        chain_ = new TChain("ntupler/tree");

        //layerMap_ = getLayerMergingMap(nLayers_);

        // Decide on the size of superstrip
        if (po.useSuperstripVarSize)
            arbiter_ = new SuperstripArbiter(po.subLadderVarSize, po.subModuleVarSize, po.subLadderECVarSize, po.subModuleECVarSize);
        else
            arbiter_ = new SuperstripArbiter(po.subLadderSize, po.subModuleSize);

        // Build a pattern from a given list of superstrips
        stitcher_ = new SuperstripStitcher(nLayers_, nFakeSuperstrips_, po.excessStrategy);
    }

    // Destructor
    ~PatternGenerator() {
        delete arbiter_;
        delete stitcher_;
    }


    // Setters
    void setNEvents(long long n)    { if (n != -1)  nEvents_ = n > 0 ? n : 0; }
    void setMinFrequency(int n)     { minFrequency_ = n > 1 ? n : 1; }
    void setVerbosity(int v)        { verbose_ = v; }

    // Getters
    // none

    // Functions
    int readTriggerTowerFile(TString src);

    int readFile(TString src);

    int makePatterns_map();
    int writePatterns_map(TString out);

    int makePatterns_fas();
    int writePatterns_fas(TString out);

    // Main driver
    int run(TString out, TString src, TString layout);

  private:
    // Private functions
    bool isWithinTriggerTower(const pattern_type& patt);


  public:
    // Configurations
    const PatternBankOption po;

  private:
    // Configurations
    const int nLayers_;
    const int nDCBits_;  // UNUSED
    const int nFakeSuperstrips_;
    const TString bankName_;

    // Program options
    long long nEvents_;
    int minFrequency_;  // min frequency of a pattern to be valid
    int verbose_;

    // Operators
    SuperstripArbiter  * arbiter_;
    SuperstripStitcher * stitcher_;

    // Containers
    TChain * chain_;
    std::map<pattern_type, unsigned> allPatterns_map_;  // using std::map approach
    std::vector<std::pair<pattern_type, unsigned> > allPatterns_map_pairs_;
    fas::lean_table3 allPatterns_fas_;                  // using fas::lean_table3 approach

    // Maps
    std::map<unsigned, unsigned> layerMap_;  // UNUSED: defines layer merging

    std::map<unsigned, std::vector<unsigned> > triggerTowerMap_;        // key: towerId, value: moduleIds in the tower
    std::map<unsigned, std::vector<unsigned> > triggerTowerReverseMap_; // key: moduleId, value: towerIds containing the module
};

#endif
