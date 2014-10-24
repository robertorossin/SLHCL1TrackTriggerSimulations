#ifndef AMSimulationIO_TTRoadReader_h_
#define AMSimulationIO_TTRoadReader_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTPattern.h"

#include "TFile.h"
#include "TROOT.h"
#include "TTree.h"
#include "TString.h"
#include <memory>
#include <vector>

namespace slhcl1tt {


// _____________________________________________________________________________
class TTRoadReader {
  public:
    TTRoadReader(int verbose=1);
    ~TTRoadReader();

    // FIXME: implement this

  private:
    TFile* tfile;
    TTree* ttree;
    const int verbose_;
};


// _____________________________________________________________________________
class TTRoadWriter {
  public:
    TTRoadWriter(int verbose=1);
    ~TTRoadWriter();

    int init(TString out, TString prefix, TString suffix);

    void fill(const std::vector<TTRoad>& roads, const std::vector<GenParticle>& genParts);

    Long64_t write();

  private:
    // Roads
    std::auto_ptr<std::vector<count_type> >               vr_nHitLayers;
    std::auto_ptr<std::vector<id_type> >                  vr_bankIndex;
    //
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitXs;
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitYs;
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitZs;
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitXErrors;
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitYErrors;
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitZErrors;
    std::auto_ptr<std::vector<std::vector<int> > >        vr_hitCharges;
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitPts;
    std::auto_ptr<std::vector<std::vector<id_type> > >    vr_hitSuperstripIds;
    std::auto_ptr<std::vector<std::vector<int> > >        vr_hitTrkIds;

    // Roads (inner vectors)
    std::vector<float>      hitXs;
    std::vector<float>      hitYs;
    std::vector<float>      hitZs;
    std::vector<float>      hitXErrors;
    std::vector<float>      hitYErrors;
    std::vector<float>      hitZErrors;
    std::vector<int>        hitCharges;
    std::vector<float>      hitPts;
    std::vector<id_type>    hitSuperstripIds;
    std::vector<int>        hitTrkIds;

    // In addition, keep genParticle info
    std::auto_ptr<std::vector<float> > vp_pt;
    std::auto_ptr<std::vector<float> > vp_eta;
    std::auto_ptr<std::vector<float> > vp_phi;
    std::auto_ptr<std::vector<float> > vp_vx;
    std::auto_ptr<std::vector<float> > vp_vy;
    std::auto_ptr<std::vector<float> > vp_vz;
    std::auto_ptr<std::vector<int> >   vp_charge;

    TFile* tfile;
    TTree* ttree;
    const int verbose_;
};

}  // namespace slhcl1tt

#endif
