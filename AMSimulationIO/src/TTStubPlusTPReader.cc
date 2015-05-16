#include "SLHCL1TrackTriggerSimulations/AMSimulationIO/interface/TTStubPlusTPReader.h"

#include "SLHCL1TrackTriggerSimulations/AMSimulationIO/interface/Helper.h"
using namespace slhcl1tt;


// _____________________________________________________________________________
TTStubPlusTPReader::TTStubPlusTPReader(int verbose)
: BasicReader(verbose),

  vp_signal       (0),
  vp_intime       (0),
  vp_primary      (0),
  vp_pdgId        (0) {}

TTStubPlusTPReader::~TTStubPlusTPReader() {}

int TTStubPlusTPReader::init(TString src) {
    if (BasicReader::init(src))
        return 1;

    // Use trkParticles instead of genParticles
    tchain->SetBranchAddress("trkParts_pt"       , &(vp_pt));
    tchain->SetBranchAddress("trkParts_eta"      , &(vp_eta));
    tchain->SetBranchAddress("trkParts_phi"      , &(vp_phi));
    tchain->SetBranchAddress("trkParts_vx"       , &(vp_vx));
    tchain->SetBranchAddress("trkParts_vy"       , &(vp_vy));
    tchain->SetBranchAddress("trkParts_vz"       , &(vp_vz));
    tchain->SetBranchAddress("trkParts_charge"   , &(vp_charge));

    tchain->SetBranchStatus("genParts_*"        , 0);
    tchain->SetBranchStatus("trkParts_pt"       , 1);
    tchain->SetBranchStatus("trkParts_eta"      , 1);
    tchain->SetBranchStatus("trkParts_phi"      , 1);
    tchain->SetBranchStatus("trkParts_vx"       , 1);
    tchain->SetBranchStatus("trkParts_vy"       , 1);
    tchain->SetBranchStatus("trkParts_vz"       , 1);
    tchain->SetBranchStatus("trkParts_charge"   , 1);

    // Set branch addresses
    tchain->SetBranchAddress("trkParts_signal"   , &(vp_signal));
    tchain->SetBranchAddress("trkParts_intime"   , &(vp_intime));
    tchain->SetBranchAddress("trkParts_primary"  , &(vp_primary));
    tchain->SetBranchAddress("trkParts_pdgId"    , &(vp_pdgId));

    tchain->SetBranchStatus("trkParts_signal"   , 1);
    tchain->SetBranchStatus("trkParts_intime"   , 1);
    tchain->SetBranchStatus("trkParts_primary"  , 1);
    tchain->SetBranchStatus("trkParts_pdgId"    , 1);
    return 0;
}


// _____________________________________________________________________________
TTStubPlusTPWriter::TTStubPlusTPWriter(int verbose)
: BasicWriter(verbose) {}

TTStubPlusTPWriter::~TTStubPlusTPWriter() {}

int TTStubPlusTPWriter::init(TChain* tchain, TString out, TString prefix, TString suffix) {
    if (BasicWriter::init(tchain, out))
        return 1;

    return 0;
}

void TTStubPlusTPWriter::fill(const std::vector<TrackingParticle>& particles) {

}
