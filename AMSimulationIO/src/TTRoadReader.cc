#include "SLHCL1TrackTriggerSimulations/AMSimulationIO/interface/TTRoadReader.h"

#include "SLHCL1TrackTriggerSimulations/AMSimulationIO/interface/Helper.h"
using namespace slhcl1tt;


// _____________________________________________________________________________
TTRoadReader::TTRoadReader(int verbose)
:
  verbose_(verbose) {}

TTRoadReader::~TTRoadReader() {
    if (ttree)  delete ttree;
    if (tfile)  delete tfile;
}


// _____________________________________________________________________________
TTRoadWriter::TTRoadWriter(int verbose)
: vr_nHitLayers       (new std::vector<count_type>()),
  vr_bankIndex        (new std::vector<id_type>()),
  //
  vr_hitXs            (new std::vector<std::vector<float> >()),
  vr_hitYs            (new std::vector<std::vector<float> >()),
  vr_hitZs            (new std::vector<std::vector<float> >()),
  vr_hitXErrors       (new std::vector<std::vector<float> >()),
  vr_hitYErrors       (new std::vector<std::vector<float> >()),
  vr_hitZErrors       (new std::vector<std::vector<float> >()),
  vr_hitCharges       (new std::vector<std::vector<int> >()),
  vr_hitPts           (new std::vector<std::vector<float> >()),
  vr_hitSuperstripIds (new std::vector<std::vector<id_type> >()),
  //
  vp_pt               (new std::vector<float>()),
  vp_eta              (new std::vector<float>()),
  vp_phi              (new std::vector<float>()),
  vp_vx               (new std::vector<float>()),
  vp_vy               (new std::vector<float>()),
  vp_vz               (new std::vector<float>()),
  vp_charge           (new std::vector<int>()),
  //
  verbose_(verbose) {}

TTRoadWriter::~TTRoadWriter() {
    if (ttree)  delete ttree;
    if (tfile)  delete tfile;
}

int TTRoadWriter::init(TString out, TString prefix, TString suffix) {
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

    tfile->mkdir("ntupler")->cd();
    ttree = new TTree("tree", "");

    ttree->Branch(prefix + "nHitLayers"        + suffix, &(*vr_nHitLayers));
    ttree->Branch(prefix + "bankIndex"         + suffix, &(*vr_bankIndex));
    //
    ttree->Branch(prefix + "hitXs"             + suffix, &(*vr_hitXs));
    ttree->Branch(prefix + "hitYs"             + suffix, &(*vr_hitYs));
    ttree->Branch(prefix + "hitZs"             + suffix, &(*vr_hitZs));
    ttree->Branch(prefix + "hitXErrors"        + suffix, &(*vr_hitXErrors));
    ttree->Branch(prefix + "hitYErrors"        + suffix, &(*vr_hitYErrors));
    ttree->Branch(prefix + "hitZErrors"        + suffix, &(*vr_hitZErrors));
    ttree->Branch(prefix + "hitCharges"        + suffix, &(*vr_hitCharges));
    ttree->Branch(prefix + "hitPts"            + suffix, &(*vr_hitPts));
    ttree->Branch(prefix + "hitSuperstripIds"  + suffix, &(*vr_hitSuperstripIds));
    //
    ttree->Branch("genParts_pt"    , &(*vp_pt));
    ttree->Branch("genParts_eta"   , &(*vp_eta));
    ttree->Branch("genParts_phi"   , &(*vp_phi));
    ttree->Branch("genParts_vx"    , &(*vp_vx));
    ttree->Branch("genParts_vy"    , &(*vp_vy));
    ttree->Branch("genParts_vz"    , &(*vp_vz));
    ttree->Branch("genParts_charge", &(*vp_charge));
    return 0;
}

void TTRoadWriter::fill(const std::vector<TTRoad>& roads, const std::vector<GenParticle>& genParts) {
    vr_nHitLayers       ->clear();
    vr_bankIndex        ->clear();
    //
    vr_hitXs            ->clear();
    vr_hitYs            ->clear();
    vr_hitZs            ->clear();
    vr_hitXErrors       ->clear();
    vr_hitYErrors       ->clear();
    vr_hitZErrors       ->clear();
    vr_hitCharges       ->clear();
    vr_hitPts           ->clear();
    vr_hitSuperstripIds ->clear();
    //
    vp_pt               ->clear();
    vp_eta              ->clear();
    vp_phi              ->clear();
    vp_vx               ->clear();
    vp_vy               ->clear();
    vp_vz               ->clear();
    vp_charge           ->clear();

    const unsigned nroads = roads.size();
    for (unsigned i=0, j=0; i<nroads; ++i) {
        hitXs            .clear();
        hitYs            .clear();
        hitZs            .clear();
        hitXErrors       .clear();
        hitYErrors       .clear();
        hitZErrors       .clear();
        hitCharges       .clear();
        hitPts           .clear();
        hitSuperstripIds .clear();

        const TTRoad&             road = roads.at(i);
        const std::vector<TTHit>& hits = road.getHits();

        for (j=0; j<hits.size(); ++j) {
            const TTHit& hit = hits.at(j);
            hitXs.push_back(hit.x);
            hitYs.push_back(hit.y);
            hitZs.push_back(hit.z);
            hitXErrors.push_back(hit.xError);
            hitYErrors.push_back(hit.yError);
            hitZErrors.push_back(hit.zError);
            hitCharges.push_back(hit.charge);
            hitPts.push_back(hit.pt);
            hitSuperstripIds.push_back(hit.superstripId);
        }

        vr_nHitLayers       ->push_back(road.nHitLayers());
        vr_bankIndex        ->push_back(road.bankIndex());
        //
        vr_hitXs            ->push_back(hitXs);
        vr_hitYs            ->push_back(hitYs);
        vr_hitZs            ->push_back(hitZs);
        vr_hitXErrors       ->push_back(hitXErrors);
        vr_hitYErrors       ->push_back(hitYErrors);
        vr_hitZErrors       ->push_back(hitZErrors);
        vr_hitCharges       ->push_back(hitCharges);
        vr_hitPts           ->push_back(hitPts);
        vr_hitSuperstripIds ->push_back(hitSuperstripIds);
    }

    const unsigned nparts = genParts.size();
    for (unsigned i=0; i<nparts; ++i) {
        const GenParticle& part = genParts.at(i);
        vp_pt    ->push_back(part.pt);
        vp_eta   ->push_back(part.eta);
        vp_phi   ->push_back(part.phi);
        vp_vx    ->push_back(part.vx);
        vp_vy    ->push_back(part.vy);
        vp_vz    ->push_back(part.vz);
        vp_charge->push_back(part.charge);
    }

    ttree->Fill();
    assert(vr_hitXs->size() == nroads);
    assert(vp_pt->size() == nparts);
}

Long64_t TTRoadWriter::write() {
    Long64_t nentries = ttree->GetEntries();
    tfile->Write();
    //tfile->Close();
    return nentries;
}

