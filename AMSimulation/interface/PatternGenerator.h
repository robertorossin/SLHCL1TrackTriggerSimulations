#ifndef AMSimulation_PatternGenerator_h_
#define AMSimulation_PatternGenerator_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Pattern.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Helper.h"
using namespace slhcl1tt;

#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TString.h"
#include "TStopwatch.h"
#include "TRandom3.h"


// FIXME: incorporate sector ID
// FIXME: implement DC bit
// FIXME: fake hits
// FIXME: bank merging
// FIXME: majority logic
// FIXME: print configuration
// FIXME: add timing

// SETTINGS: DC bits, superstrip size, etc
// INPUT   : TTree with moduleId, hitId, sim info + sector file
// OUTPUT  : TTree with active moduleId, and patterns

class PatternGenerator {
  public:
    typedef std::map<uint64_t, std::shared_ptr<bool> > HitIdBoolMap;
    typedef std::map<Pattern::pattern8_t, std::shared_ptr<bool> > PatternIdBoolMap;
    typedef std::map<uint64_t, uint32_t> HitIdIndexMap;
    typedef std::map<Pattern::pattern8_t, uint32_t> PatternIdIndexMap;

    // Constructor
    PatternGenerator()
    : minPt_(2.0), maxPt_(9999.0),
      minEta_(-2.5), maxEta_(2.5),
      minPhi_(-M_PI), maxPhi_(M_PI),
      nLayers_(6), nSubLadders_(32), nSubModules_(1024),
      nFakeHits_(0), nDCBits_(1),
      doFilter_(true), doTiming_(false),
      nEvents_(999999999), nPatterns_(999999999), verbose_(1) {

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

    ~PatternGenerator() {}


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
    void setNFakeHits(int n)      { nFakeHits_ = std::max(-1, n); }
    void setNDCBits(int n)        { nDCBits_ = std::max(-1, n); }

    void setActiveLayers(const std::vector<int>& v)   { activeLayers_ = v; }
    void setInactiveLayers(const std::vector<int>& v) { inactiveLayers_ = v; }
    void setSectors(const std::vector<int>& v)        { sectors_ = v; }

    void setFilter(bool b=true)   { doFilter_ = b; }
    void setTiming(bool b=false)  { doTiming_ = b; }
    void setNEvents(int n)        { if (n != -1)  nEvents_ = std::max(0, n); }
    void setNPatterns(int n)      { if (n != -1)  nPatterns_ = std::max(0, n); }
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
    int getNFakeHits()      const { return nFakeHits_; }
    int getNDCBits()        const { return nDCBits_; }

    std::vector<int> getActiveLayers()   const { return activeLayers_; }
    std::vector<int> getInactiveLayers() const { return inactiveLayers_; }
    std::vector<int> getSectors()        const { return sectors_; }


    // Functions
    void createDummy(TString out, int n);

    int readSectorFile(TString src);

    int readFile(TString src);

    int readTree();

    int writeTree(TString out);

    // Main driver
    int run(TString src, TString out, TString layout);


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
    int nFakeHits_;
    int nDCBits_;
    std::vector<int> activeLayers_;
    std::vector<int> inactiveLayers_;
    std::vector<int> sectors_;

    // Program options
    bool doFilter_;
    bool doTiming_;
    int nEvents_;
    int nPatterns_;
    int verbose_;

    // Containers
    TChain * chain_;
    std::vector<Pattern> allPatterns_;
    std::vector<Pattern> goodPatterns_;

    HitIdBoolMap hitIdMap_;           // key: hitId, value: pointer to a boolean
    PatternIdIndexMap patternIdMap_;  // key: patternId, value: index in allPatterns_

    std::map<uint32_t, uint32_t> layerMap_;  // defines layer merging
    std::map<uint32_t, Pattern::vuint32_t> sectorMap_;

};

#endif
