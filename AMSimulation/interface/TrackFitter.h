#ifndef AMSimulation_TrackFitter_h_
#define AMSimulation_TrackFitter_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTPattern.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTTrack.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Helper.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TrackFitterOption.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TrackFitterAlgo.h"
using namespace slhcl1tt;

#include "TFile.h"
#include "TFileCollection.h"
#include "TChain.h"
#include "TTree.h"
#include "TString.h"


// This code is entirely based on [lucamartini/CMS](https://github.com/lucamartini/CMS)

// SETTINGS: resolution for pq, pqType, etc
// INPUT   : Roads
// OUTPUT  : Tracks

class TrackFitter {
  public:
    // Constructor
    TrackFitter(TrackFitterOption option)
    : po(option), nLayers_(po.nLayers),
      prefixRoad_("AMTTRoads_"), prefixTrack_("AMTTTracks_"), suffix_(""),
      nEvents_(999999999), maxTracks_(999999999),
      verbose_(1) {

        assert(3 <= nLayers_ && nLayers_ <= 8);

        chain_ = new TChain("ntupler/tree");
    }

    // Destructor
    ~TrackFitter() {}


    // Setters
    void setNEvents(long long n)  { if (n != -1)  nEvents_   = n > 0 ? n : 0; }
    void setMaxTracks(int n)      { if (n != -1)  maxTracks_ = n > 0 ? n : 0; }
    void setVerbosity(int v)      { verbose_ = v; }

    // Getters
    // none

    // Functions
    int readFile(TString src);

    int makeTracks(TString out);

    // Main driver
    int run(TString out, TString src);


  public:
    // Configurations
    const TrackFitterOption po;

  private:
    // Configurations
    const int nLayers_;
    const TString prefixRoad_;
    const TString prefixTrack_;
    const TString suffix_;

    // Program options
    long long nEvents_;
    int maxTracks_;  // max number of tracks per event
    int verbose_;

    // Containers
    TChain * chain_;
};

#endif
