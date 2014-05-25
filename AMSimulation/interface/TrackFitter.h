#ifndef AMSimulation_TrackFitter_h_
#define AMSimulation_TrackFitter_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TTPattern.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TTTrack.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TrackFitterAlgo.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TrackFitterOption.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Helper.h"
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

    TrackFitter(TrackFitterOption option)
    : po(option), nLayers_(po.nLayers),
      prefixRoad_("AMTTRoads_"), prefixTrack_("AMTTTracks_"), suffix_(""),
      nEvents_(999999999), maxTracks_(999999999),
      verbose_(1) {

        assert(3 <= nLayers_ && nLayers_ <= 8);

        chain_ = new TChain("ntupler/tree");
    }

    ~TrackFitter() {}

    // Setters
    //void setNLayers(int n)        { nLayers_ = n; }

    void setNEvents(int n)        { if (n != -1)  nEvents_ = std::max(0, n); }
    void setMaxTracks(int n)      { if (n != -1)  maxTracks_ = std::max(0, n); }
    void setVerbosity(int n)      { verbose_ = n; }

    // Getters
    int getNLayers()        const { return nLayers_; }

    // Functions
    int readFile(TString src);

    int makeTracks();

    int writeTracks(TString out);

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
    int nEvents_;
    int maxTracks_;
    int verbose_;

    // Containers
    TChain * chain_;
    std::vector<std::vector<TTTrack> > allTracks_;
    //std::vector<std::vector<TTTrack> > goodTracks_;

};

#endif
