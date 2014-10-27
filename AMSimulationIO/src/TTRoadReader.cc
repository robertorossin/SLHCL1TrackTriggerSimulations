#include "SLHCL1TrackTriggerSimulations/AMSimulationIO/interface/TTRoadReader.h"

#include "SLHCL1TrackTriggerSimulations/AMSimulationIO/interface/Helper.h"
using namespace slhcl1tt;


// _____________________________________________________________________________
TTRoadReader::TTRoadReader(int verbose)
: vr_nHitLayers        (0),
  vr_bankIndex         (0),
  //
  vr_hitXs             (0),
  vr_hitYs             (0),
  vr_hitZs             (0),
  vr_hitXErrors        (0),
  vr_hitYErrors        (0),
  vr_hitZErrors        (0),
  vr_hitCharges        (0),
  vr_hitPts            (0),
  vr_hitSuperstripIds  (0),
  vr_hitTrkIds         (0),
  //
  vp_pt                (0),
  vp_eta               (0),
  vp_phi               (0),
  vp_vx                (0),
  vp_vy                (0),
  vp_vz                (0),
  vp_charge            (0),
  //
  verbose_(verbose) {}

TTRoadReader::~TTRoadReader() {
    if (tchain)  delete tchain;
}

int TTRoadReader::init(TString src, TString prefix, TString suffix) {
    if (!src.EndsWith(".root") && !src.EndsWith(".txt")) {
        std::cout << Error() << "Input source must be either .root or .txt" << std::endl;
        return 1;
    }

    if (verbose_)  std::cout << Info() << "Opening " << src << std::endl;
    tchain = new TChain("ntupler/tree");

    if (src.EndsWith(".root")) {
        if (tchain->Add(src) ) {
            if (verbose_)  std::cout << Info() << "Successfully read " << src << std::endl;
        } else {
            std::cout << Error() << "Failed to read " << src << std::endl;
            return 1;
        }

    } else if (src.EndsWith(".txt")) {
        TFileCollection fc("fileinfolist", "", src);
        if (tchain->AddFileInfoList((TCollection*) fc.GetList()) ) {
            if (verbose_)  std::cout << Info() << "Successfully read " << src << std::endl;
        } else {
            std::cout << Error() << "Failed to read " << src << std::endl;
            return 1;
        }
    }

    assert(tchain != 0);
    treenumber = tchain->GetTreeNumber();

    tchain->SetBranchAddress(prefix + "nHitLayers"        + suffix, &(vr_nHitLayers));
    tchain->SetBranchAddress(prefix + "bankIndex"         + suffix, &(vr_bankIndex));
    //
    tchain->SetBranchAddress(prefix + "hitXs"             + suffix, &(vr_hitXs));
    tchain->SetBranchAddress(prefix + "hitYs"             + suffix, &(vr_hitYs));
    tchain->SetBranchAddress(prefix + "hitZs"             + suffix, &(vr_hitZs));
    tchain->SetBranchAddress(prefix + "hitXErrors"        + suffix, &(vr_hitXErrors));
    tchain->SetBranchAddress(prefix + "hitYErrors"        + suffix, &(vr_hitYErrors));
    tchain->SetBranchAddress(prefix + "hitZErrors"        + suffix, &(vr_hitZErrors));
    tchain->SetBranchAddress(prefix + "hitCharges"        + suffix, &(vr_hitCharges));
    tchain->SetBranchAddress(prefix + "hitPts"            + suffix, &(vr_hitPts));
    tchain->SetBranchAddress(prefix + "hitSuperstripIds"  + suffix, &(vr_hitSuperstripIds));
    tchain->SetBranchAddress(prefix + "hitTrkIds"         + suffix, &(vr_hitTrkIds));
    //
    tchain->SetBranchAddress("genParts_pt"      , &(vp_pt));
    tchain->SetBranchAddress("genParts_eta"     , &(vp_eta));
    tchain->SetBranchAddress("genParts_phi"     , &(vp_phi));
    tchain->SetBranchAddress("genParts_vx"      , &(vp_vx));
    tchain->SetBranchAddress("genParts_vy"      , &(vp_vy));
    tchain->SetBranchAddress("genParts_vz"      , &(vp_vz));
    tchain->SetBranchAddress("genParts_charge"  , &(vp_charge));
    return 0;
}

Long64_t TTRoadReader::loadTree(Long64_t entry) {
    //if (!tchain) return -5;
    return tchain->LoadTree(entry);
}

Int_t TTRoadReader::getEntry(Long64_t entry) {
   //if (!tchain) return 0;
   return tchain->GetEntry(entry);
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
  vr_hitTrkIds        (new std::vector<std::vector<int> >()),
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
    ttree->Branch(prefix + "hitTrkIds"         + suffix, &(*vr_hitTrkIds));
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
    vr_hitTrkIds        ->clear();
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
        hitTrkIds        .clear();

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
            hitTrkIds.push_back(hit.trkId);
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
        vr_hitTrkIds        ->push_back(hitTrkIds);
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

