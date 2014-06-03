#ifndef AMSimulation_StubCleaner_h_
#define AMSimulation_StubCleaner_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/Helper.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternBankOption.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/HelperMath.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Helper.h"
using namespace slhcl1tt;

#include "TFile.h"
#include "TFileCollection.h"
#include "TChain.h"
#include "TTree.h"
#include "TTreePlayer.h"
#include "TTreeFormula.h"
#include "TString.h"


// SETTINGS: none
// INPUT   : TTree with moduleId, hitId, sim info
// OUTPUT  : TTree with moduleId, hitId, sim info

class StubCleaner {
  public:
    // Constructor
    StubCleaner(PatternBankOption option)
    : po(option), nLayers_(po.nLayers),
      filter_(true),
      nEvents_(999999999), verbose_(1) {

        assert(3 <= nLayers_ && nLayers_ <= 8);

        chain_ = new TChain("ntupler/tree");

        eventSelect_ = "(1)";  // always on

        layerMap_ = getLayerMergingMap(nLayers_);
    }

    // Destructor
    ~StubCleaner() {}


    // Setters
    //void setNLayers(int n)          { nLayers_ = n; }

    void setFilter(bool b=true)     { filter_ = b; }
    void setNEvents(int n)          { if (n != -1)  nEvents_ = std::max(0, n); }
    void setVerbosity(int n)        { verbose_ = n; }

    // Getters
    int getNLayers()          const { return nLayers_; }

    // Functions
    int readFile(TString src);

    int cleanStubs(TString out);

    // Main driver
    int run(TString out, TString src);


  public:
    // Configurations
    const PatternBankOption po;

  private:
    // Configurations
    const int nLayers_;

    // Program options
    bool filter_;
    int nEvents_;
    int verbose_;

    // Event selection
    TString eventSelect_;

    // Containers
    TChain * chain_;

    std::map<unsigned, unsigned> layerMap_;  // defines layer merging
};

#endif
