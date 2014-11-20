#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TrackFitter.h"

#include "SLHCL1TrackTriggerSimulations/AMSimulationIO/interface/TTRoadReader.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulationIO/interface/TTTrackReader.h"


// _____________________________________________________________________________
// Do track fitting
int TrackFitter::makeTracks(TString src, TString out) {
    if (verbose_)  std::cout << Info() << "Reading " << nEvents_ << " events and fitting tracks." << std::endl;

    // _________________________________________________________________________
    // For reading
    TTRoadReader reader(verbose_);

    if (reader.init(src, prefixRoad_, suffix_)) {
        std::cout << Error() << "Failed to initialize TTRoadReader." << std::endl;
        return 1;
    }

    // _________________________________________________________________________
    // For writing
    TTTrackWriter writer(verbose_);
    if (writer.init(out, prefixTrack_, suffix_)) {
        std::cout << Error() << "Failed to initialize TTTrackWriter." << std::endl;
        return 1;
    }

    // _________________________________________________________________________
    // Loop over all events

    // Bookkeepers
    int nPassed = 0, nKept = 0;

    for (long long ievt=0; ievt<nEvents_; ++ievt) {
        if (reader.loadTree(ievt) < 0)  break;
        reader.getEntry(ievt);

        const unsigned nroads = reader.vr_hitRs->size();
        if (verbose_>1 && ievt%5000==0)  std::cout << Debug() << Form("... Processing event: %7lld, keeping: %7i, fitting: %7i", ievt, nKept, nPassed) << std::endl;
        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " # roads: " << nroads << std::endl;

//FIXME        if (!nroads) {  // skip if no road
            writer.fill(std::vector<TTTrack>());
            ++nKept;
            continue;
//FIXME        }

        std::vector<TTTrack> tracks;
        tracks.reserve(200);

        // Find track fit combinations
        const unsigned ncombinations = nroads;  // FIXME

        for (unsigned i=0; i<ncombinations; ++i) {
            std::vector<TTHit> hits;  // FIXME

            const TTTrackParam tparam = fitterLin_->fit(hits);

            if (verbose_>2)  std::cout << Debug() << "... " << std::endl;

            TTTrack track;  // FIXME
            float pt = std::abs(1.0/tparam.curvature);
            GlobalVector gv(
                pt * std::cos(tparam.phi0),
                pt * std::sin(tparam.phi0),
                pt * tparam.cottheta
            );
            GlobalPoint gp(
                0.,
                0.,
                tparam.z0
            );
            track.setMomentum(gv);
            track.setPOCA(gp);
            track.setChi2(tparam.chi2);
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
// Main driver
int TrackFitter::run(TString src, TString out) {
    int exitcode = 0;
    Timing(1);

    exitcode = makeTracks(src, out);
    if (exitcode)  return exitcode;
    Timing();

    return exitcode;
}
