#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TrackFitter.h"


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
int TrackFitter::makeTracks() {
    if (verbose_)  std::cout << Info() << "Reading " << nEvents_ << " events" << std::endl;

    // For reading
    typedef unsigned short unsigned16;
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


    // _________________________________________________________________________
    // Loop over all events

    allTracks_.clear();
    int nPassed = 0;
    for (long long ievt=0; ievt<nEvents_; ++ievt) {
        Long64_t local_entry = chain_->LoadTree(ievt);  // for TChain
        if (local_entry < 0)  break;
        chain_->GetEntry(ievt);

        unsigned nroads = vr_patternIds->size();
        if (verbose_>1 && ievt%1000==0)  std::cout << Debug() << Form("... Processing event: %7lld, fitting: %7i", ievt, nPassed) << std::endl;
        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " # roads: " << nroads << " [# tracks: " << allTracks_.size() << "]" << std::endl;

        if (!nroads) {  // skip if no road
            allTracks_.push_back(std::vector<TTTrack>());
            continue;
        }

        std::vector<TTTrack> tracksInThisEvent;
        for (unsigned i=0; i<nroads; ++i) {
            pattern_type patternId;

            std::vector<addr_type> addresses;
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

            TTHit hit;
            std::vector<TTHit> hits;
            std::vector<ZR> hitsViewZR;
            std::vector<UV> hitsViewUV;
            for (unsigned j=0; j<nhits; ++j) {
                constructHit(
                    vr_hitXs->at(i).at(j),
                    vr_hitYs->at(i).at(j),
                    vr_hitZs->at(i).at(j),
                    vr_hitXErrors->at(i).at(j),
                    vr_hitYErrors->at(i).at(j),
                    vr_hitZErrors->at(i).at(j),
                    vr_hitCharges->at(i).at(j),
                    vr_hitPts->at(i).at(j),
                    vr_hitSuperstripIds->at(i).at(j),
                    vr_hitSuperstripBits->at(i).at(j),
                    hit);
                hits.push_back(hit);

                // In R-Z plane, a track is a straight line
                ZR hitViewZR(hit.rho(), hit.z);
                hitsViewZR.push_back(hitViewZR);

                // In X-Y plane, a track is a circle
                // Under conformal transfromation XY --> UV, a track becomes a straight line
                UV hitViewUV(hit.u(), hit.v());
                hitsViewUV.push_back(hitViewUV);
            }

            TTRoad road(patternId, hits);

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
            tracksInThisEvent.push_back(track);
        }

        allTracks_.push_back(tracksInThisEvent);
        ++nPassed;
    }

    if (verbose_)  std::cout << Info() << "Processed " << nEvents_ << " events, fit " << nPassed << " events." << std::endl;
    return 0;
}


// _____________________________________________________________________________
// Output tracks into a TTree
int TrackFitter::writeTracks(TString out) {
    if (!out.EndsWith(".root")) {
        std::cout << Error() << "Output filename must be .root" << std::endl;
        return 1;
    }

    const long long nentries = allTracks_.size();
    if (verbose_)  std::cout << Info() << "Recreating " << out << " with " << nentries << " events." << std::endl;
    TFile* tfile = TFile::Open(out, "RECREATE");
    tfile->mkdir("ntupler")->cd();
    TTree* ttree = new TTree("tree", "");

    std::auto_ptr<std::vector<float> >              vt_px               (new std::vector<float>());
    std::auto_ptr<std::vector<float> >              vt_py               (new std::vector<float>());
    std::auto_ptr<std::vector<float> >              vt_pz               (new std::vector<float>());
    std::auto_ptr<std::vector<float> >              vt_pt               (new std::vector<float>());
    std::auto_ptr<std::vector<float> >              vt_eta              (new std::vector<float>());
    std::auto_ptr<std::vector<float> >              vt_phi              (new std::vector<float>());
    std::auto_ptr<std::vector<float> >              vt_vx               (new std::vector<float>());
    std::auto_ptr<std::vector<float> >              vt_vy               (new std::vector<float>());
    std::auto_ptr<std::vector<float> >              vt_vz               (new std::vector<float>());
    std::auto_ptr<std::vector<float> >              vt_rinv             (new std::vector<float>());
    std::auto_ptr<std::vector<float> >              vt_chi2             (new std::vector<float>());
    std::auto_ptr<std::vector<float> >              vt_ptconsistency    (new std::vector<float>());
    std::auto_ptr<std::vector<unsigned> >           vt_nstubs           (new std::vector<unsigned>());

    ttree->Branch(prefixTrack_ + "px"             + suffix_, &(*vt_px));
    ttree->Branch(prefixTrack_ + "py"             + suffix_, &(*vt_py));
    ttree->Branch(prefixTrack_ + "pz"             + suffix_, &(*vt_pz));
    ttree->Branch(prefixTrack_ + "pt"             + suffix_, &(*vt_pt));
    ttree->Branch(prefixTrack_ + "eta"            + suffix_, &(*vt_eta));
    ttree->Branch(prefixTrack_ + "phi"            + suffix_, &(*vt_phi));
    ttree->Branch(prefixTrack_ + "vx"             + suffix_, &(*vt_vx));
    ttree->Branch(prefixTrack_ + "vy"             + suffix_, &(*vt_vy));
    ttree->Branch(prefixTrack_ + "vz"             + suffix_, &(*vt_vz));
    ttree->Branch(prefixTrack_ + "rinv"           + suffix_, &(*vt_rinv));
    ttree->Branch(prefixTrack_ + "chi2"           + suffix_, &(*vt_chi2));
    ttree->Branch(prefixTrack_ + "ptconsistency"  + suffix_, &(*vt_ptconsistency));
    ttree->Branch(prefixTrack_ + "nstubs"         + suffix_, &(*vt_nstubs));

    // _________________________________________________________________________
    // Loop over all tracks
    for (long long ievt=0; ievt<nentries; ++ievt) {
        if (verbose_>1 && ievt%10000==0)  std::cout << Debug() << Form("... Writing event: %7lld", ievt) << std::endl;

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

        const std::vector<TTTrack>& tracksInThisEvent = allTracks_.at(ievt);
        unsigned ntracks = tracksInThisEvent.size();

        for (unsigned i=0; i<ntracks; ++i) {
            if ((int) i >= maxTracks_)  break;

            const TTTrack& track = tracksInThisEvent.at(i);
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
    }
    assert(ttree->GetEntries() == nentries);

    tfile->Write();
    delete ttree;
    delete tfile;
    return 0;
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

    exitcode = makeTracks();
    if (exitcode)  return exitcode;
    Timing();

    exitcode = writeTracks(out);
    if (exitcode)  return exitcode;
    Timing();

    chain_->Reset();
    return exitcode;
}
