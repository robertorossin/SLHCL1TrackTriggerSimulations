#ifndef AMSimulation_TrackFitter_h_
#define AMSimulation_TrackFitter_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Helper.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/ProgramOption.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TrackFitterAlgoPCA.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TrackFitterAlgoATF.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TrackFitterAlgoRetina.h"
using namespace slhcl1tt;


class TrackFitter {

  public:
    // Constructor
    TrackFitter(const ProgramOption& po)
    : po_(po),
      nEvents_(po.maxEvents), verbose_(po.verbose),
      prefixRoad_("AMTTRoads_"), prefixTrack_("AMTTTracks_"), suffix_("") {

        // Decide the track fitter to use
        fitterPCA_    = 0;
        fitterATF_    = 0;
        fitterRetina_ = 0;
        if (po_.algo == "ATF4")
            fitterATF_ = new TrackFitterAlgoATF(false);
        else if (po_.algo=="ATF5")
            fitterATF_ = new TrackFitterAlgoATF(true);
        else if (po_.algo=="PCA4")
            fitterPCA_ = new TrackFitterAlgoPCA(false);
        else if (po_.algo=="PCA5")
            fitterPCA_ = new TrackFitterAlgoPCA(true);
        else if (po_.algo=="RET")
            fitterRetina_ = new TrackFitterAlgoRetina();
    }

    // Destructor
    ~TrackFitter() {
        if (fitterPCA_)    delete fitterPCA_;
        if (fitterATF_)    delete fitterATF_;
        if (fitterRetina_) delete fitterRetina_;
    }

    // Main driver
    int run();


  private:
    // Member functions
    int loadConstants(TString txt);

    int makeTracks(TString src, TString out);

    // Program options
    const ProgramOption po_;
    long long nEvents_;
    int verbose_;

    // Configurations
    const TString prefixRoad_;
    const TString prefixTrack_;
    const TString suffix_;

    // Track fitters
    TrackFitterAlgoPCA *    fitterPCA_;
    TrackFitterAlgoATF *    fitterATF_;
    TrackFitterAlgoRetina * fitterRetina_;
};

#endif
