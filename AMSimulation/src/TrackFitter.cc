#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TrackFitter.h"

class TTTrackWriter {
  public:
    TTTrackWriter(TString out, TString prefix, TString suffix);
    ~TTTrackWriter();

    void fill(const std::vector<TTTrack>& tracks);
    long long write();

  private:
    void init(TString out, TString prefix, TString suffix);

    TFile* tfile;
    TTree* ttree;

    // Tracks
    std::auto_ptr<std::vector<float> >              vt_px;
    std::auto_ptr<std::vector<float> >              vt_py;
    std::auto_ptr<std::vector<float> >              vt_pz;
    std::auto_ptr<std::vector<float> >              vt_pt;
    std::auto_ptr<std::vector<float> >              vt_eta;
    std::auto_ptr<std::vector<float> >              vt_phi;
    std::auto_ptr<std::vector<float> >              vt_vx;
    std::auto_ptr<std::vector<float> >              vt_vy;
    std::auto_ptr<std::vector<float> >              vt_vz;
    std::auto_ptr<std::vector<float> >              vt_rinv;
    std::auto_ptr<std::vector<float> >              vt_chi2;
    std::auto_ptr<std::vector<float> >              vt_ptconsistency;
    std::auto_ptr<std::vector<unsigned> >           vt_nstubs;
};


// _____________________________________________________________________________
// Read the input ntuples
int TrackFitter::readFile(TString src) {
    if (src.EndsWith(".root")) {
        if (verbose_)  std::cout << Info() << "Opening " << src << std::endl;
        if (chain_->Add(src) )  // 1 if successful, 0 otherwise
            return 0;

    } else if (src.EndsWith(".txt")) {
        TFileCollection* fc = new TFileCollection("fileinfolist", "", src);
        if (chain_->AddFileInfoList((TCollection*) fc->GetList()) )  // 1 if successful, 0 otherwise
            return 0;
    }

    std::cout << Error() << "Input source should be either a .root file or a .txt file." << std::endl;
    return 1;
}


// _____________________________________________________________________________
// Do track fitting
int TrackFitter::makeTracks(TString out) {

    // For reading
    if (verbose_)  std::cout << Info() << "Reading " << nEvents_ << " events" << std::endl;

    typedef unsigned short unsigned16;
    std::vector<unsigned16> *               vr_nHitLayers        = 0;
    std::vector<unsigned> *                 vr_bankIndex         = 0;
    std::vector<std::vector<unsigned> > *   vr_patternIds        = 0;
    std::vector<std::vector<float> > *      vr_hitXs             = 0;
    std::vector<std::vector<float> > *      vr_hitYs             = 0;
    std::vector<std::vector<float> > *      vr_hitZs             = 0;
    std::vector<std::vector<float> > *      vr_hitXErrors        = 0;
    std::vector<std::vector<float> > *      vr_hitYErrors        = 0;
    std::vector<std::vector<float> > *      vr_hitZErrors        = 0;
    std::vector<std::vector<int> > *        vr_hitCharges        = 0;
    std::vector<std::vector<float> > *      vr_hitPts            = 0;
    std::vector<std::vector<unsigned> > *   vr_hitSuperstripIds  = 0;
    std::vector<std::vector<unsigned16> > * vr_hitSuperstripBits = 0;

    //chain_->SetBranchStatus("*", 1);

    chain_->SetBranchAddress(prefixRoad_ + "nHitLayers"        + suffix_, &(vr_nHitLayers));
    chain_->SetBranchAddress(prefixRoad_ + "bankIndex"         + suffix_, &(vr_bankIndex));
    chain_->SetBranchAddress(prefixRoad_ + "patternIds"        + suffix_, &(vr_patternIds));
    chain_->SetBranchAddress(prefixRoad_ + "hitXs"             + suffix_, &(vr_hitXs));
    chain_->SetBranchAddress(prefixRoad_ + "hitYs"             + suffix_, &(vr_hitYs));
    chain_->SetBranchAddress(prefixRoad_ + "hitZs"             + suffix_, &(vr_hitZs));
    chain_->SetBranchAddress(prefixRoad_ + "hitXErrors"        + suffix_, &(vr_hitXErrors));
    chain_->SetBranchAddress(prefixRoad_ + "hitYErrors"        + suffix_, &(vr_hitYErrors));
    chain_->SetBranchAddress(prefixRoad_ + "hitZErrors"        + suffix_, &(vr_hitZErrors));
    chain_->SetBranchAddress(prefixRoad_ + "hitCharges"        + suffix_, &(vr_hitCharges));
    chain_->SetBranchAddress(prefixRoad_ + "hitPts"            + suffix_, &(vr_hitPts));
    chain_->SetBranchAddress(prefixRoad_ + "hitSuperstripIds"  + suffix_, &(vr_hitSuperstripIds));
    chain_->SetBranchAddress(prefixRoad_ + "hitSuperstripBits" + suffix_, &(vr_hitSuperstripBits));

    // For writing
    if (verbose_)  std::cout << Info() << "Recreating " << out << std::endl;
    TTTrackWriter writer(out, prefixTrack_, suffix_);


    // _________________________________________________________________________
    // Loop over all events

    int nPassed = 0, nKept = 0;
    unsigned ievt_step = 0;
    for (long long ievt=0; ievt<nEvents_; ++ievt, ++ievt_step) {
        Long64_t local_entry = chain_->LoadTree(ievt);  // for TChain
        if (local_entry < 0)  break;
        chain_->GetEntry(ievt);

        unsigned nroads = vr_patternIds->size();
        if (verbose_>1 && ievt_step == 5000) {
            std::cout << Debug() << Form("... Processing event: %7lld, keeping: %7i, fitting: %7i", ievt, nKept, nPassed) << std::endl;
            ievt_step -= 5000;
        }
        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " # roads: " << nroads << std::endl;

//FIXME        if (!nroads) {  // skip if no road
            writer.fill(std::vector<TTTrack>());
            ++nKept;
            continue;
//FIXME        }

        std::vector<TTTrack> tracks;
        tracks.reserve(200);

        for (unsigned i=0; i<nroads; ++i) {
            pattern_type patternId;

            std::vector<id_type> addresses;
            for (unsigned j=0; j<vr_patternIds->at(i).size(); ++j) {
                addresses.push_back(vr_patternIds->at(i).at(j));
            }
            addresses.resize(8,0);  // pad with zeroes
            std::copy(addresses.begin(), addresses.end(), patternId.begin());

            unsigned nhits = vr_hitXs->at(i).size();
            //if (nhits<3) {
            //    std::cout << Error() << "Too few hits in a road: " << nhits << std::endl;
            //    return 1;
            //}

            std::vector<TTHit> hits;
            std::vector<ZR> hitsViewZR;
            std::vector<UV> hitsViewUV;
            for (unsigned j=0; j<nhits; ++j) {
                hits.emplace_back(TTHit{
                    vr_hitXs->at(i).at(j),
                    vr_hitYs->at(i).at(j),
                    vr_hitZs->at(i).at(j),
                    vr_hitXErrors->at(i).at(j),
                    vr_hitYErrors->at(i).at(j),
                    vr_hitZErrors->at(i).at(j),
                    vr_hitCharges->at(i).at(j),
                    vr_hitPts->at(i).at(j),
                    vr_hitSuperstripIds->at(i).at(j),
                    vr_hitSuperstripBits->at(i).at(j)
                });

                // In R-Z plane, a track is a straight line
                ZR hitViewZR(hits.back().rho(), hits.back().z);
                hitsViewZR.push_back(hitViewZR);

                // In X-Y plane, a track is a circle
                // Under conformal transfromation XY --> UV, a track becomes a straight line
                UV hitViewUV(hits.back().u(), hits.back().v());
                hitsViewUV.push_back(hitViewUV);
            }

            TTRoad road(vr_nHitLayers->at(i), vr_bankIndex->at(i), patternId, hits);

            // Fitting starts here
            RetinaTrackFitterAlgo<ZR> retinaZR(po.pqType, po.pbins, po.qbins, po.pmin, po.qmin, po.pmax, po.qmax, po.sigma, po.minWeight);
            retinaZR.setHits(hitsViewZR);
            retinaZR.fillGrid();
            retinaZR.findMaxima();
            std::vector<TrackParam> params = retinaZR.getParams();

            if (verbose_>2) {
                std::cout << Debug() << "... ... road: " << i << " # params: " << params.size() << std::endl;
                for (unsigned j=0; j<params.size(); ++j)
                    std::cout << Debug() << "... ... ... p: " << params.at(j) << std::endl;
            }


            TTTrack track;  // FIXME
            if (!params.empty()) {
                TrackParam p = params.at(0);
                GlobalVector gv(
                    p.pt * std::cos(p.phi),
                    p.pt * std::sin(p.phi),
                    p.pt > 0 ? p.pt * std::sinh(p.eta) : 0
                );
                GlobalPoint gp(0., 0., p.dz);
                track.setMomentum(gv);
                track.setPOCA(gp);
                track.setChi2(p.chi2);
            }
            tracks.push_back(track);

            if ((int) tracks.size() >= maxTracks_)
                break;
        }

        if (! tracks.empty())
            ++nPassed;

        writer.fill(tracks);
        ++nKept;
    }

    if (verbose_)  std::cout << Info() << "Processed " << nEvents_ << " events, kept " << nKept << ", fitted " << nPassed << std::endl;

    long long nentries = writer.write();
    assert(nentries == nKept);

    return 0;
}


// _____________________________________________________________________________
// Private functions
// none


// _____________________________________________________________________________
// TTTrackWriter

TTTrackWriter::TTTrackWriter(TString out, TString prefix, TString suffix)
: vt_px           (new std::vector<float>()),
  vt_py           (new std::vector<float>()),
  vt_pz           (new std::vector<float>()),
  vt_pt           (new std::vector<float>()),
  vt_eta          (new std::vector<float>()),
  vt_phi          (new std::vector<float>()),
  vt_vx           (new std::vector<float>()),
  vt_vy           (new std::vector<float>()),
  vt_vz           (new std::vector<float>()),
  vt_rinv         (new std::vector<float>()),
  vt_chi2         (new std::vector<float>()),
  vt_ptconsistency(new std::vector<float>()),
  vt_nstubs       (new std::vector<unsigned>()) {

    init(out, prefix, suffix);
}

TTTrackWriter::~TTTrackWriter() {
    if (ttree)  delete ttree;
    if (tfile)  delete tfile;
}

void TTTrackWriter::init(TString out, TString prefix, TString suffix) {
    if (!out.EndsWith(".root"))
        throw std::invalid_argument("Output filename must be .root.");

    tfile = TFile::Open(out, "RECREATE");
    if (tfile == 0)
        throw std::runtime_error("Cannot recreate output file.");

    tfile->mkdir("ntupler")->cd();
    ttree = new TTree("tree", "");

    ttree->Branch(prefix + "px"             + suffix, &(*vt_px));
    ttree->Branch(prefix + "py"             + suffix, &(*vt_py));
    ttree->Branch(prefix + "pz"             + suffix, &(*vt_pz));
    ttree->Branch(prefix + "pt"             + suffix, &(*vt_pt));
    ttree->Branch(prefix + "eta"            + suffix, &(*vt_eta));
    ttree->Branch(prefix + "phi"            + suffix, &(*vt_phi));
    ttree->Branch(prefix + "vx"             + suffix, &(*vt_vx));
    ttree->Branch(prefix + "vy"             + suffix, &(*vt_vy));
    ttree->Branch(prefix + "vz"             + suffix, &(*vt_vz));
    ttree->Branch(prefix + "rinv"           + suffix, &(*vt_rinv));
    ttree->Branch(prefix + "chi2"           + suffix, &(*vt_chi2));
    ttree->Branch(prefix + "ptconsistency"  + suffix, &(*vt_ptconsistency));
    ttree->Branch(prefix + "nstubs"         + suffix, &(*vt_nstubs));
}

void TTTrackWriter::fill(const std::vector<TTTrack>& tracks) {
    vt_px              ->clear();
    vt_py              ->clear();
    vt_pz              ->clear();
    vt_pt              ->clear();
    vt_eta             ->clear();
    vt_phi             ->clear();
    vt_vx              ->clear();
    vt_vy              ->clear();
    vt_vz              ->clear();
    vt_rinv            ->clear();
    vt_chi2            ->clear();
    vt_ptconsistency   ->clear();
    vt_nstubs          ->clear();

    const unsigned ntracks = tracks.size();
    for (unsigned i=0; i<ntracks; ++i) {
        const TTTrack& track = tracks.at(i);
        const GlobalVector& momentum = track.getMomentum();
        const GlobalPoint&  poca = track.getPOCA();

        vt_px              ->push_back(momentum.x());
        vt_py              ->push_back(momentum.y());
        vt_pz              ->push_back(momentum.z());
        vt_pt              ->push_back(std::sqrt(momentum.perp2()));
        vt_eta             ->push_back(momentum.eta());
        vt_phi             ->push_back(momentum.phi());
        vt_vx              ->push_back(poca.x());
        vt_vy              ->push_back(poca.y());
        vt_vz              ->push_back(poca.z());
        vt_rinv            ->push_back(track.getRInv());
        vt_chi2            ->push_back(track.getChi2());
        vt_ptconsistency   ->push_back(track.getStubPtConsistency());
        vt_nstubs          ->push_back(track.getStubRefs().size());
    }

    ttree->Fill();
    assert(vt_px->size() == ntracks);
}

long long TTTrackWriter::write() {
    long long nentries = ttree->GetEntries();
    tfile->Write();
    //tfile->Close();
    return nentries;
}

// _____________________________________________________________________________
// Main driver
int TrackFitter::run(TString out, TString src) {
    gROOT->ProcessLine("#include <vector>");  // how is it not loaded?

    int exitcode = 0;
    Timing(1);

    exitcode = readFile(src);
    if (exitcode)  return exitcode;
    Timing();

    exitcode = makeTracks(out);
    if (exitcode)  return exitcode;
    Timing();

    chain_->Reset();
    return exitcode;
}
