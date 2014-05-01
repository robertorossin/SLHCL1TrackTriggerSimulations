#ifndef AMSimulation_PatternMatcher_h_
#define AMSimulation_PatternMatcher_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Pattern.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Helper.h"
using namespace slhcl1tt;

#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TString.h"
#include "TStopwatch.h"


// FIXME: missing settings in PatternGenerators

// SETTINGS: majority logic, merged layers, etc
// INPUT   : TTree with moduleId, hitId, sim info + pattern bank
// OUTPUT  : TTree with pass or fail

class PatternMatcher {
  public:
    typedef std::map<uint64_t, std::shared_ptr<bool> > HitIdBoolMap;

    PatternMatcher()
    : minPt_(2.0), maxPt_(9999.0),
      minEta_(-2.5), maxEta_(2.5),
      minPhi_(-M_PI), maxPhi_(M_PI),
      nLayers_(6), nSubLadders_(32), nSubModules_(1024),
      doTiming_(false),
      nEvents_(999999999), verbose_(1) {

        chain_ = new TChain("ntupler/tree");
    }

    ~PatternMatcher() {}

    // Setters
    void setMinPt(float pt)       { minPt_ = pt; }
    void setMaxPt(float pt)       { maxPt_ = pt; }
    void setMinEta(float eta)     { minEta_ = eta; }
    void setMaxEta(float eta)     { maxEta_ = eta; }
    void setMinPhi(float phi)     { minPhi_ = phi; }
    void setMaxPhi(float phi)     { maxPhi_ = phi; }

    void setNLayers(int n)        { nLayers_   = std::max(0, n); }
    void setNSubLadders(int n)    { assert(n%2 == 0); nSubLadders_ = std::min(std::max(0, n), nSubLadders_); }
    void setNSubModules(int n)    { assert(n%2 == 0); nSubModules_ = std::min(std::max(0, n), nSubModules_); }

    void setTiming(bool b=false)  { doTiming_ = b; }
    void setNEvents(int n)        { if (n != -1)  nEvents_ = std::max(0, n); }
    void setVerbosity(int n)      { verbose_ = n; }


    // Getters
    float getMinPt()        const { return minPt_; }
    float getMaxPt()        const { return maxPt_; }
    float getMinEta()       const { return minEta_; }
    float getMaxEta()       const { return maxEta_; }
    float getMinPhi()       const { return minPhi_; }
    float getMaxPhi()       const { return maxPhi_; }

    int getNLayers()        const { return nLayers_; }
    int getNSubLadders()    const { return nSubLadders_; }
    int getNSubModules()    const { return nSubModules_; }
    int getNEvents()        const { return nEvents_; }


    // Functions
    void resetMap();

    int readPatterns(TString src);

    int putPatterns();

    int readFile(TString src);

    int readAndWriteTree(TString out);

    //int writeTree(TString out);

    // Main driver
    int run(TString src, TString out, TString bank);


  private:
    // Settings
    float minPt_;
    float maxPt_;
    float minEta_;  // not absolute eta
    float maxEta_;  // not absolute eta
    float minPhi_;  // from -pi to pi
    float maxPhi_;  // from -pi to pi
    int nLayers_;
    int nSubLadders_;  // 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024
    int nSubModules_;  // 1, 2, 4, 8, 16, 32

    // Program options
    bool doTiming_;
    int nEvents_;
    int verbose_;

    // Containers
    TChain * chain_;
    std::vector<Pattern> patterns_;

    HitIdBoolMap hitIdMap_;           // key: hitId, value: pointer to a boolean

};

#endif
