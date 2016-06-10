#include "SLHCL1TrackTriggerSimulations/AMSimulationIO/interface/PatternBankReader.h"

#include "SLHCL1TrackTriggerSimulations/AMSimulationIO/interface/Helper.h"
using namespace slhcl1tt;


// _____________________________________________________________________________
PatternBankReader::PatternBankReader(int verbose)
: pb_invPt_mean     (0.),
  pb_invPt_sigma    (0.),
  pb_cotTheta_mean  (0.),
  pb_cotTheta_sigma (0.),
  pb_phi_mean       (0.),
  pb_phi_sigma      (0.),
  pb_z0_mean        (0.),
  pb_z0_sigma       (0.),
  //
  pb_coverage       (0.),
  pb_count          (0),
  pb_tower          (0),
  pb_superstrip     (0),
  pb_superstrip_nx  (0),
  pb_superstrip_nz  (0),
  //
  pb_frequency      (0),
  pb_superstripIds  (0),
  //
  verbose_(verbose) {}

PatternBankReader::~PatternBankReader() {
    if (ttree3) delete ttree3;
    if (ttree2) delete ttree2;
    if (ttree)  delete ttree;
    if (tfile)  delete tfile;
}

int PatternBankReader::init(TString src) {
    if (!src.EndsWith(".root")) {
        std::cout << Error() << "Input source must be .root" << std::endl;
        return 1;
    }

    if (verbose_)  std::cout << Info() << "Opening " << src << std::endl;
    tfile = TFile::Open(src);

    if (tfile) {
        if (verbose_)  std::cout << Info() << "Successfully opened " << src << std::endl;
    } else {
        std::cout << Error() << "Failed to open " << src << std::endl;
        return 1;
    }

    ttree3 = (TTree*) tfile->Get("patternAttributes");
    if (ttree3 != 0) {
        ttree3->SetBranchAddress("invPt_mean"    , &pb_invPt_mean);
        ttree3->SetBranchAddress("invPt_sigma"   , &pb_invPt_sigma);
        ttree3->SetBranchAddress("cotTheta_mean" , &pb_cotTheta_mean);
        ttree3->SetBranchAddress("cotTheta_sigma", &pb_cotTheta_sigma);
        ttree3->SetBranchAddress("phi_mean"      , &pb_phi_mean);
        ttree3->SetBranchAddress("phi_sigma"     , &pb_phi_sigma);
        ttree3->SetBranchAddress("z0_mean"       , &pb_z0_mean);
        ttree3->SetBranchAddress("z0_sigma"      , &pb_z0_sigma);

        ttree3->SetBranchStatus("*"              , 0);
        ttree3->SetBranchStatus("invPt_mean"     , 1);
    }

    ttree2 = (TTree*) tfile->Get("patternBankInfo");
    assert(ttree2 != 0);

    ttree2->SetBranchAddress("coverage"     , &pb_coverage);
    ttree2->SetBranchAddress("count"        , &pb_count);
    ttree2->SetBranchAddress("tower"        , &pb_tower);
    ttree2->SetBranchAddress("superstrip"   , &pb_superstrip);
    ttree2->SetBranchAddress("superstrip_nx", &pb_superstrip_nx);
    ttree2->SetBranchAddress("superstrip_nz", &pb_superstrip_nz);

    ttree = (TTree*) tfile->Get("patternBank");
    assert(ttree != 0);

    ttree->SetBranchAddress("frequency"    , &pb_frequency);
    ttree->SetBranchAddress("superstripIds", &pb_superstripIds);

    return 0;
}

void PatternBankReader::getPatternBankInfo(float& coverage, unsigned& count, unsigned& tower, std::string& superstrip, unsigned& superstrip_nx, unsigned& superstrip_nz) {
    ttree2->GetEntry(0);

    coverage      = pb_coverage;
    count         = pb_count;
    tower         = pb_tower;
    superstrip    = (*pb_superstrip);
    superstrip_nx = pb_superstrip_nx;
    superstrip_nz = pb_superstrip_nz;
}

void PatternBankReader::getPatternInvPt(Long64_t entry, float& invPt_mean) {
    ttree3->GetEntry(entry);

    invPt_mean = pb_invPt_mean;
}


// _____________________________________________________________________________
PatternBankWriter::PatternBankWriter(int verbose)
: pb_invPt_mean     (new float(0.)),
  pb_invPt_sigma    (new float(0.)),
  pb_cotTheta_mean  (new float(0.)),
  pb_cotTheta_sigma (new float(0.)),
  pb_phi_mean       (new float(0.)),
  pb_phi_sigma      (new float(0.)),
  pb_z0_mean        (new float(0.)),
  pb_z0_sigma       (new float(0.)),
  //
  pb_coverage       (new float(0.)),
  pb_count          (new unsigned(0)),
  pb_tower          (new unsigned(0)),
  pb_superstrip     (new std::string("")),
  pb_superstrip_nx  (new unsigned(0)),
  pb_superstrip_nz  (new unsigned(0)),
  //
  pb_frequency      (new frequency_type(0)),
  pb_superstripIds  (new std::vector<superstrip_type>()),
  //
  verbose_(verbose) {}

PatternBankWriter::~PatternBankWriter() {
    if (ttree3) delete ttree3;
    if (ttree2) delete ttree2;
    if (ttree)  delete ttree;
    if (tfile)  delete tfile;
}

int PatternBankWriter::init(TString out) {
    gROOT->ProcessLine("#include <vector>");  // how is it not loaded?

    if (!out.EndsWith(".root")) {
        std::cout << Error() << "Output filename must be .root" << std::endl;
        return 1;
    }

    if (verbose_)  std::cout << Info() << "Opening " << out << std::endl;
    tfile = TFile::Open(out, "RECREATE");

    if (tfile) {
        if (verbose_)  std::cout << Info() << "Successfully opened " << out << std::endl;
    } else {
        std::cout << Error() << "Failed to open " << out << std::endl;
        return 1;
    }

    // Pattern attributes
    ttree3 = new TTree("patternAttributes", "");
    ttree3->Branch("invPt_mean"    , &(*pb_invPt_mean));
    ttree3->Branch("cotTheta_mean" , &(*pb_cotTheta_mean));
    ttree3->Branch("phi_mean"      , &(*pb_phi_mean));
    ttree3->Branch("z0_mean"       , &(*pb_z0_mean));
//    ttree3->Branch("invPt_sigma"   , &(*pb_invPt_sigma));
//    ttree3->Branch("phi_sigma"     , &(*pb_phi_sigma));
//    ttree3->Branch("cotTheta_sigma", &(*pb_cotTheta_sigma));
//    ttree3->Branch("z0_sigma"      , &(*pb_z0_sigma));

    // Pattern bank statistics
    ttree2 = new TTree("patternBankInfo", "");
    ttree2->Branch("coverage"      , &(*pb_coverage));
    ttree2->Branch("count"         , &(*pb_count));
    ttree2->Branch("tower"         , &(*pb_tower));
    ttree2->Branch("superstrip"    , &(*pb_superstrip));
    ttree2->Branch("superstrip_nx" , &(*pb_superstrip_nx));
    ttree2->Branch("superstrip_nz" , &(*pb_superstrip_nz));

    // Pattern bank
    ttree = new TTree("patternBank", "");
    ttree->Branch("frequency"      , &(*pb_frequency));
    ttree->Branch("superstripIds"  , &(*pb_superstripIds));

    return 0;
}

void PatternBankWriter::fillPatternAttributes() {
    ttree3->Fill();
}

void PatternBankWriter::fillPatternBankInfo() {
    ttree2->Fill();
    assert(ttree2->GetEntries() == 1);
}

void PatternBankWriter::fillPatternBank() {
    ttree->Fill();
}

Long64_t PatternBankWriter::writeTree() {
    Long64_t nentries = ttree->GetEntries();
    tfile->Write();
    //tfile->Close();
    return nentries;
}
