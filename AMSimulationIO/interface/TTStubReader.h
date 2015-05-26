#ifndef AMSimulationIO_TTStubReader_h_
#define AMSimulationIO_TTStubReader_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulationIO/interface/BasicReader.h"


namespace slhcl1tt {

// _____________________________________________________________________________
class TTStubReader : public BasicReader {
  public:
    TTStubReader(int verbose=1);
    ~TTStubReader();

    int init(TString src, bool full=true);
};


// _____________________________________________________________________________
class TTStubWriter : public BasicWriter {
  public:
    TTStubWriter(int verbose=1);
    ~TTStubWriter();

    int init(TChain* tchain, TString out);

    void fill();
};

}  // namespace slhcl1tt

#endif
