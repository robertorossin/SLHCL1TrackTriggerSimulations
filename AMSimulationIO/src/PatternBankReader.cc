#include "SLHCL1TrackTriggerSimulations/AMSimulationIO/interface/PatternBankReader.h"

#include "SLHCL1TrackTriggerSimulations/AMSimulationIO/interface/Helper.h"
using namespace slhcl1tt;


// _____________________________________________________________________________
PatternBankReader::PatternBankReader(int verbose)
: pb_ttmap          (0),
  pb_ttrmap         (0),
  //
  pb_frequency      (0),
  pb_superstripIds  (0),
  //
  verbose_(verbose) {}

PatternBankReader::~PatternBankReader() {
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

    ttree2 = (TTree*) tfile->Get("triggerTower");
    assert(ttree2 != 0);

    ttree2->SetBranchAddress("triggerTowerMap"       , &pb_ttmap);
    ttree2->SetBranchAddress("triggerTowerReverseMap", &pb_ttrmap);

    ttree = (TTree*) tfile->Get("patternBank");
    assert(ttree != 0);

    ttree->SetBranchAddress("frequency"    , &pb_frequency);
    ttree->SetBranchAddress("superstripIds", &pb_superstripIds);

    return 0;
}

void PatternBankReader::getTriggerTowerMaps(std::map<unsigned, std::vector<unsigned> >& ttmap,
                                            std::map<unsigned, std::vector<unsigned> >& ttrmap) {
    ttree2->GetEntry(0);

    ttmap.swap(*pb_ttmap);
    ttrmap.swap(*pb_ttrmap);
}


// _____________________________________________________________________________
PatternBankWriter::PatternBankWriter(int verbose)
: pb_ttmap          (0),
  pb_ttrmap         (0),
  //
  pb_frequency      (new count_type(0)),
  pb_superstripIds  (new std::vector<id_type>()),
  //
  verbose_(verbose) {}

PatternBankWriter::~PatternBankWriter() {
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
        if (verbose_)  std::cout << Info() << "Successfully recreated " << out << std::endl;
    } else {
        std::cout << Error() << "Failed to recreate " << out << std::endl;
        return 1;
    }

    // Trigger tower map
    ttree2 = new TTree("triggerTower", "");

    // Pattern bank
    ttree = new TTree("patternBank", "");
    ttree->Branch("frequency"              , &(*pb_frequency));
    ttree->Branch("superstripIds"          , &(*pb_superstripIds));

    return 0;
}

void PatternBankWriter::fillTriggerTower() {
    ttree2->Branch("triggerTowerMap"       , pb_ttmap);
    ttree2->Branch("triggerTowerReverseMap", pb_ttrmap);
    ttree2->Fill();
    assert(ttree2->GetEntries() == 1);
}

void PatternBankWriter::fillPatternBank() {
    ttree->Fill();
}

Long64_t PatternBankWriter::write() {
    Long64_t nentries = ttree->GetEntries();
    tfile->Write();
    //tfile->Close();
    return nentries;
}

