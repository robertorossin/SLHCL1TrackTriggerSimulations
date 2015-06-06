#include "SLHCL1TrackTriggerSimulations/AMSimulationIO/interface/TTStubPlusTPReader.h"

#include "SLHCL1TrackTriggerSimulations/AMSimulationIO/interface/Helper.h"
using namespace slhcl1tt;


// _____________________________________________________________________________
TTStubPlusTPReader::TTStubPlusTPReader(int verbose)
: BasicReader(verbose),

  vp2_pt               (0),
  vp2_eta              (0),
  vp2_phi              (0),
  vp2_vx               (0),
  vp2_vy               (0),
  vp2_vz               (0),
  vp2_charge           (0),
  vp2_pdgId            (0),
  vp2_signal           (0),
  vp2_intime           (0),
  vp2_primary          (0) {}

TTStubPlusTPReader::~TTStubPlusTPReader() {}

int TTStubPlusTPReader::init(TString src, bool full) {
    if (BasicReader::init(src, full))
        return 1;

    tchain->SetBranchAddress("trkParts_pt"       , &(vp2_pt));
    tchain->SetBranchAddress("trkParts_eta"      , &(vp2_eta));
    tchain->SetBranchAddress("trkParts_phi"      , &(vp2_phi));
    tchain->SetBranchAddress("trkParts_vx"       , &(vp2_vx));
    tchain->SetBranchAddress("trkParts_vy"       , &(vp2_vy));
    tchain->SetBranchAddress("trkParts_vz"       , &(vp2_vz));
    tchain->SetBranchAddress("trkParts_charge"   , &(vp2_charge));
    tchain->SetBranchAddress("trkParts_pdgId"    , &(vp2_pdgId));
    tchain->SetBranchAddress("trkParts_signal"   , &(vp2_signal));
    tchain->SetBranchAddress("trkParts_intime"   , &(vp2_intime));
    tchain->SetBranchAddress("trkParts_primary"  , &(vp2_primary));

    tchain->SetBranchStatus("trkParts_pt"       , 1);
    tchain->SetBranchStatus("trkParts_eta"      , 1);
    tchain->SetBranchStatus("trkParts_phi"      , 1);
    tchain->SetBranchStatus("trkParts_vx"       , 1);
    tchain->SetBranchStatus("trkParts_vy"       , 1);
    tchain->SetBranchStatus("trkParts_vz"       , 1);
    tchain->SetBranchStatus("trkParts_charge"   , 1);
    tchain->SetBranchStatus("trkParts_pdgId"    , 1);
    tchain->SetBranchStatus("trkParts_signal"   , 1);
    tchain->SetBranchStatus("trkParts_intime"   , 1);
    tchain->SetBranchStatus("trkParts_primary"  , 1);
    return 0;
}

void TTStubPlusTPReader::nullParticles(const std::vector<bool>& nulling, bool full) {
    nullVectorElements(vp2_pt        , nulling);
    nullVectorElements(vp2_eta       , nulling);
    nullVectorElements(vp2_phi       , nulling);
    nullVectorElements(vp2_vx        , nulling);
    nullVectorElements(vp2_vy        , nulling);
    nullVectorElements(vp2_vz        , nulling);
    //nullVectorElements(vp2_charge    , nulling);  // don't null this guy
    nullVectorElements(vp2_pdgId     , nulling);
    nullVectorElements(vp2_signal    , nulling);
    nullVectorElements(vp2_intime    , nulling);
    //nullVectorElements(vp2_primary   , nulling);  // don't null this guy
}


// _____________________________________________________________________________
TTStubPlusTPWriter::TTStubPlusTPWriter(int verbose)
: BasicWriter(verbose) {}

TTStubPlusTPWriter::~TTStubPlusTPWriter() {}

int TTStubPlusTPWriter::init(TChain* tchain, TString out) {
    if (BasicWriter::init(tchain, out))
        return 1;

    return 0;
}

void TTStubPlusTPWriter::fill() {
    ttree->Fill();
}
