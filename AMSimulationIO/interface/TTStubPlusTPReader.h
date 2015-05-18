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

    int init(TString src);

    // TrkParticles
    std::vector<bool> *           vp_signal;
    std::vector<bool> *           vp_intime;
    std::vector<bool> *           vp_primary;
    std::vector<int> *            vp_pdgId;
};


// _____________________________________________________________________________
class TTStubPlusTPWriter : public BasicWriter {
  public:
    TTStubPlusTPWriter(int verbose=1);
    ~TTStubPlusTPWriter();

    int init(TChain* tchain, TString out, TString prefix, TString suffix);

    void fill(const std::vector<TrackingParticle>& particles);

  protected:
    // TrkParticles
    // FIXME
};

}  // namespace slhcl1tt

#endif
