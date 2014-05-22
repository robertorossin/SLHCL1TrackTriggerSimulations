#ifndef AMSimulation_PatternGenerator_h_
#define AMSimulation_PatternGenerator_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TTPattern.h"
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


// SETTINGS: DC bits, superstrip size, etc
// INPUT   : TTree with moduleId, hitId, sim info + trigger tower file
// OUTPUT  : Pattern bank

class PatternGenerator {
  public:
    // Constructor
    PatternGenerator(PatternBankOption option)
    : po(option), nLayers_(po.nLayers), nDCBits_(po.nDCBits), nFakeSuperstrips_(po.nFakeSuperstrips),
      filter_(true),
      nEvents_(999999999), maxPatterns_(999999999), verbose_(1) {

        assert(nLayers_ <= 8);
        assert(nDCBits_ <= 4);
        assert(nFakeSuperstrips_ <= 1);

        chain_ = new TChain("ntupler/tree");

        //eventSelect_ = "(genParts_pt[0]>2 && abs(genParts_eta[0])<2.2 && Sum$(TTStubs_trkId==1 && abs(atan2(TTStubs_r,TTStubs_z)-atan2(genParts_pt[0],genParts_pz[0]))<0.05 && abs(deltaPhi(atan2(TTStubs_y,TTStubs_x),genParts_phi[0]))<0.03)>=Sum$(TTStubs_trkId==1)-2)";
        eventSelect_ = "(1)";  // always on

        makeLayerMap();
    }

    // Destructor
    ~PatternGenerator() {}


    // Setters
    //void setNLayers(int n)          { nLayers_ = n; }
    //void setNDCBits(int n)          { nDCBits_ = n; }
    //void setNFakeSuperstrips(int n) { nFakeSuperstrips_ = n; }

    void setFilter(bool b=true)     { filter_ = b; }
    void setNEvents(int n)          { if (n != -1)  nEvents_ = std::max(0, n); }
    void setMaxPatterns(int n)      { if (n != -1)  maxPatterns_ = std::max(0, n); }
    void setVerbosity(int n)        { verbose_ = n; }

    // Getters
    int getNLayers()          const { return nLayers_; }
    int getNDCBits()          const { return nDCBits_; }
    int getNFakeSuperstrips() const { return nFakeSuperstrips_; }

    // Functions
    int readTriggerTowerFile(TString src);

    int readFile(TString src);

    int filterHits(TString out_tmp);  // make a temporary tree

    int makePatterns();

    int writePatterns(TString out);

    // Main driver
    int run(TString out, TString src, TString layout);

  private:
    void makeLayerMap();

    void uniquifyPatterns();

    bool isFullyContainedInTriggerTower(const TTPattern& patt);


  public:
    // Configurations
    const PatternBankOption po;

  private:
    // Configurations
    const int nLayers_;
    const int nDCBits_;
    const int nFakeSuperstrips_;

    // Program options
    bool filter_;
    int nEvents_;
    int maxPatterns_;  // maximum number of patterns
    int verbose_;

    // Event selection
    TString eventSelect_;

    // Containers
    TChain * chain_;
    std::vector<TTPattern> allPatterns_;
    std::vector<TTPattern> goodPatterns_;

    std::map<unsigned, unsigned> layerMap_;  // defines layer merging

    std::map<unsigned, std::vector<unsigned> > triggerTowerMap_;  // key: towerId, value: moduleIds in the tower
    std::map<unsigned, std::vector<unsigned> > triggerTowerMapReversed_;  // key: moduleId, value: towerIds containing the module
};

#endif
