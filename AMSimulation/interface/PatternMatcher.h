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
    typedef std::map<uint64_t, std::shared_ptr<bool> > HitIdBoolMap;

    PatternMatcher(PatternBankOption option)
    : po(option), nLayers_(po.nLayers),
      nEvents_(999999999),
      verbose_(1) {

        chain_ = new TChain("ntupler/tree");
    }

    ~PatternMatcher() {}

    // Setters
    void setNLayers(int n)        { nLayers_ = n; }

    void setNEvents(int n)        { if (n != -1)  nEvents_ = std::max(0, n); }
    void setVerbosity(int n)      { verbose_ = n; }

    // Getters
    int getNLayers()        const { return nLayers_; }


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

    // Program options
    int nEvents_;
    int verbose_;

    // Containers
    TChain * chain_;
    std::vector<Pattern> patterns_;

    HitIdBoolMap hitIdMap_;           // key: hitId, value: pointer to a boolean
};

#endif
