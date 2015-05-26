#ifndef AMSimulationIO_TTStubPlusTPReader_h_
#define AMSimulationIO_TTStubPlusTPReader_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TrackingParticle.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulationIO/interface/BasicReader.h"

namespace slhcl1tt {

// _____________________________________________________________________________
class TTStubPlusTPReader : public BasicReader {
  public:
    TTStubPlusTPReader(int verbose=1);
    ~TTStubPlusTPReader();

    int init(TString src, bool full=true);

    void nullParticles(const std::vector<bool>& nulling, bool full=true);

    // trkParticle information
    std::vector<float> *          vp2_pt;
    std::vector<float> *          vp2_eta;
    std::vector<float> *          vp2_phi;
    std::vector<float> *          vp2_vx;
    std::vector<float> *          vp2_vy;
    std::vector<float> *          vp2_vz;
    std::vector<int> *            vp2_charge;
    std::vector<bool> *           vp2_signal;
    std::vector<bool> *           vp2_intime;
    std::vector<bool> *           vp2_primary;
};


// _____________________________________________________________________________
class TTStubPlusTPWriter : public BasicWriter {
  public:
    TTStubPlusTPWriter(int verbose=1);
    ~TTStubPlusTPWriter();

    int init(TChain* tchain, TString out);

    void fill();
};

}  // namespace slhcl1tt

#endif
