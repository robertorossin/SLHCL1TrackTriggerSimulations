#ifndef AMSimulation_PatternMatcher_h_
#define AMSimulation_PatternMatcher_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Pattern.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternBankOption.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Helper.h"
using namespace slhcl1tt;

#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TString.h"
#include "TROOT.h"


// FIXME: missing settings in PatternGenerator
// FIXME: majority logic

// SETTINGS: majority logic, merged layers, etc
// INPUT   : TTree with moduleId, hitId, sim info + pattern bank
// OUTPUT  : TTree with pass or fail

class PatternMatcher {
  public:
    //typedef std::map<uint64_t, std::shared_ptr<bool> > HitIdBoolMap;
    typedef std::map<uint64_t, std::shared_ptr<uint16_t> > HitIdShortMap;

    PatternMatcher(PatternBankOption option)
    : po(option), nLayers_(po.nLayers), nDCBits_(po.nDCBits),
      nEvents_(999999999),
      verbose_(1) {

        chain_ = new TChain("ntupler/tree");

        // Hardcoded layer information
        if (nLayers_ <= 6) {
            layerMap_ = std::map<uint32_t, uint32_t> {
                {5,5}, {6,6}, {7,7}, {8,8}, {9,9}, {10,10},
                {11,11}, {12,10}, {13,9}, {14,8}, {15,7},
                {18,11}, {19,10}, {20,9}, {21,8}, {22,7}
            };
        } else {  // otherwise it's not merged
            layerMap_ = std::map<uint32_t, uint32_t> {
                {5,5}, {6,6}, {7,7}, {8,8}, {9,9}, {10,10},
                {11,11}, {12,12}, {13,13}, {14,14}, {15,15},
                {18,11}, {19,12}, {20,13}, {21,14}, {22,15}
            };
        };
    }

    ~PatternMatcher() {}

    // Setters
    void setNLayers(int n)        { nLayers_ = n; }
    void setNDCBits(int n)        { nDCBits_ = n; }

    void setNEvents(int n)        { if (n != -1)  nEvents_ = std::max(0, n); }
    void setVerbosity(int n)      { verbose_ = n; }

    // Getters
    int getNLayers()        const { return nLayers_; }
    int getNDCBits()        const { return nDCBits_; }


    // Functions
    void resetMap();

    int readPatterns(TString src);

    int putPatterns();

    int readFile(TString src);

    int readAndMakeTree(TString out_tmp);  // make a temporary tree

    int writeTree(TString out);

    // Main driver
    int run(TString src, TString out, TString bank);


  public:
    // Configurations
    const PatternBankOption po;

  private:
    // Configurations
    int nLayers_;
    int nDCBits_;

    // Program options
    int nEvents_;
    int verbose_;

    // Containers
    TChain * chain_;
    std::vector<Pattern> patterns_;

    //HitIdBoolMap hitIdMap_;           // key: hitId, value: pointer to a boolean
    HitIdShortMap hitIdMap_;          // key: hitId, value: pointer to a short int

    std::map<uint32_t, uint32_t> layerMap_;  // defines layer merging
    std::map<uint32_t, Pattern::vuint32_t> triggerTowerMap_;
};

#endif
