#ifndef AMSimulation_StubCleaner_h_
#define AMSimulation_StubCleaner_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/Helper.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Helper.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/HelperMath.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternBankOption.h"
using namespace slhcl1tt;

#include "TFile.h"
#include "TFileCollection.h"
#include "TChain.h"
#include "TTreePlayer.h"
#include "TTreeFormula.h"
#include "TString.h"


// SETTINGS: none
// INPUT   : TTree with moduleId, hitId, sim info
// OUTPUT  : TTree with moduleId, hitId, sim info (cleaned)

class StubCleaner {
  public:
    // Constructor
    StubCleaner(PatternBankOption option)
    : po(option),
      nEvents_(999999999), filter_(true),
      verbose_(1) {

        chain_ = new TChain("ntupler/tree");

        eventSelect_ = "(1)";  // always on
    }

    // Destructor
    ~StubCleaner() {}


    // Setters
    void setNEvents(long long n)    { if (n != -1)  nEvents_ = n > 0 ? n : 0; }
    void setFilter(bool b)          { filter_ = b; }
    void setVerbosity(int v)        { verbose_ = v; }

    // Getters
    // none

    // Functions
    int readFile(TString src);

    int cleanStubs(TString out);

    // Main driver
    int run(TString out, TString src);


  public:
    // Configurations
    const PatternBankOption po;

  private:
    // Program options
    long long nEvents_;
    bool filter_;  // throw away events with stubs?
    int verbose_;

    // Event selection
    TString eventSelect_;

    // Containers
    TChain * chain_;
};

#endif
