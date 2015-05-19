#include "SLHCL1TrackTriggerSimulations/AMSimulationIO/interface/TTStubReader.h"

#include "SLHCL1TrackTriggerSimulations/AMSimulationIO/interface/Helper.h"
using namespace slhcl1tt;


// _____________________________________________________________________________
TTStubReader::TTStubReader(int verbose)
: BasicReader(verbose) {}

TTStubReader::~TTStubReader() {}

int TTStubReader::init(TString src, bool full) {
    if (BasicReader::init(src, full))
        return 1;

    return 0;
}


// _____________________________________________________________________________
TTStubWriter::TTStubWriter(int verbose)
: BasicWriter(verbose) {}

TTStubWriter::~TTStubWriter() {}

int TTStubWriter::init(TChain* tchain, TString out) {
    if (BasicWriter::init(tchain, out))
        return 1;

    return 0;
}

void TTStubWriter::fill() {
    ttree->Fill();
}
