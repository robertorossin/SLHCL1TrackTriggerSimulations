#ifndef AMSimulation_PatternMatcher_h_
#define AMSimulation_PatternMatcher_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TTPattern.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternBankOption.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/HelperMath.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Helper.h"
using namespace slhcl1tt;

#include <unordered_map>
#include "TFile.h"
#include "TFileCollection.h"
#include "TChain.h"
#include "TTree.h"
#include "TString.h"


// SETTINGS: majority logic, merged layers, etc
// INPUT   : TTree with moduleId, hitId, sim info + pattern bank
// OUTPUT  : Roads

class PatternMatcher {
  public:
    typedef std::pair<bit_type, unsigned>  bit_n_index_pair;

    // Constructor
    PatternMatcher(PatternBankOption option)
    : po(option), nLayers_(po.nLayers), nDCBits_(po.nDCBits),
      prefixRoad_("AMTTRoads_"), suffix_(""),
      nEvents_(999999999), maxRoads_(999999999), maxHits_(999999999), verbose_(1) {

        assert(3 <= nLayers_ && nLayers_ <= 8);
        assert(0 <= nDCBits_ && nDCBits_ <= 4);

        chain_ = new TChain("ntupler/tree");

        makeLayerMap();
    }

    // Destructor
    ~PatternMatcher() {}

    // Setters
    //void setNLayers(int n)        { nLayers_ = n; }
    //void setNDCBits(int n)        { nDCBits_ = n; }

    void setNEvents(int n)        { if (n != -1)  nEvents_ = std::max(0, n); }
    void setMaxRoads(int n)       { if (n != -1)  maxRoads_ = std::max(0, n); }
    void setMaxHits(int n)        { if (n != -1)  maxHits_ = std::max(0, n); }
    void setVerbosity(int n)      { verbose_ = n; }

    // Getters
    int getNLayers()        const { return nLayers_; }
    int getNDCBits()        const { return nDCBits_; }

    // Functions
    int readPatterns(TString src);

    int loadPatterns();

    int readFile(TString src);

    int makeRoads();

    int writeRoads(TString out);

    // Main driver
    int run(TString out, TString src, TString bank);

  private:
    void makeLayerMap();


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
    int maxRoads_;  // max number of roads per event
    int maxHits_;   // max number of hits per road
    int verbose_;

    // Containers
    TChain * chain_;
    std::vector<std::vector<TTRoad> > allRoads_;
    //std::vector<std::vector<TTRoad> > goodRoads_;

    std::vector<TTPattern> patterns_;
    std::unordered_multimap<addr_type, bit_n_index_pair> ssIdMapFast_;   // key: superstripId, value: pointer to TTPattern

    std::map<id_type, id_type> layerMap_;  // defines layer merging
    std::map<id_type, std::vector<id_type> > triggerTowerMap_;
};

#endif
