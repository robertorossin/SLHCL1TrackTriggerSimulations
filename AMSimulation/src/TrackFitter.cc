#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TrackFitter.h"

#include "SLHCL1TrackTriggerSimulations/AMSimulationIO/interface/TTRoadReader.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulationIO/interface/TTTrackReader.h"


namespace {
unsigned getPtSegment(float invPt) {  // for PCA
    return (invPt - PCA_MIN_INVPT) / (PCA_MAX_INVPT - PCA_MIN_INVPT) * PCA_NSEGMENTS;
}

unsigned getHitBits(const std::vector<bool>& stubs_bool) {
    unsigned bitset = 0;

    for (unsigned i=0; i<stubs_bool.size(); ++i) {
        bitset |= (stubs_bool.at(i) << i);
    }

    switch (bitset) {
    case 0b111111:  return 0;
    case 0b111110:  return 1;
    case 0b111101:  return 2;
    case 0b111011:  return 3;
    case 0b110111:  return 4;
    case 0b101111:  return 5;
    case 0b011111:  return 6;
    default      :  return 7;
    }
}
}


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
    if (writer.init(reader.getChain(), out, prefixTrack_, suffix_)) {
        std::cout << Error() << "Failed to initialize TTTrackWriter." << std::endl;
        return 1;
    }

    // _________________________________________________________________________
    // Loop over all events

    // Containers
    std::vector<TTTrack2> tracks;
    tracks.reserve(300);

    // Bookkeepers
    long int nRead = 0, nKept = 0;

    for (long long ievt=0; ievt<nEvents_; ++ievt) {
        if (reader.loadTree(ievt) < 0)  break;
        reader.getEntry(ievt);

        const unsigned nroads = reader.vr_patternRef->size();
        if (verbose_>1 && ievt%1000==0)  std::cout << Debug() << Form("... Processing event: %7lld, fitting: %7ld", ievt, nKept) << std::endl;
        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " # roads: " << nroads << std::endl;

        if (!nroads) {  // skip if no road
            writer.fill(std::vector<TTTrack2>());
            ++nRead;
            continue;
        }

        tracks.clear();
        int fitstatus = 0;


        // _____________________________________________________________________
        // Track fitters taking fit combinations

        // Loop over the roads
        for (unsigned iroad=0; iroad<nroads; ++iroad) {
            if (iroad >= (unsigned) po_.maxRoads)  break;

            // Get combinations of stubRefs
            std::vector<std::vector<unsigned> > stubRefs = reader.vr_stubRefs->at(iroad);
            for (unsigned ilayer=0; ilayer<stubRefs.size(); ++ilayer) {
                if (stubRefs.at(ilayer).size() > (unsigned) po_.maxStubs)
                    stubRefs.at(ilayer).resize(po_.maxStubs);
            }

            const std::vector<std::vector<unsigned> >& combinations = combinationFactory_.combine(stubRefs);

            for (unsigned icomb=0; icomb<combinations.size(); ++icomb)
                assert(combinations.at(icomb).size() == reader.vr_stubRefs->at(iroad).size());

            if (verbose_>2) {
                std::cout << Debug() << "... ... road: " << iroad << " # combinations: " << combinations.size() << std::endl;
            }

            // Loop over the combinations
            for (unsigned icomb=0; icomb<combinations.size(); ++icomb) {
                if (icomb >= (unsigned) po_.maxCombs)  break;

                // Create and set TTRoadComb
                TTRoadComb acomb;
                acomb.roadRef    = iroad;
                acomb.combRef    = icomb;
                acomb.ptSegment  = getPtSegment(reader.vr_patternInvPt->at(iroad));
                acomb.stubRefs   = combinations.at(icomb);

                acomb.stubs_r   .clear();
                acomb.stubs_phi .clear();
                acomb.stubs_z   .clear();
                acomb.stubs_bool.clear();

                for (unsigned istub=0; istub<acomb.stubRefs.size(); ++istub) {
                    const unsigned stubRef = acomb.stubRefs.at(istub);
                    if (stubRef != CombinationFactory::BAD) {
                        acomb.stubs_r   .push_back(reader.vb_r   ->at(stubRef));
                        acomb.stubs_phi .push_back(reader.vb_phi ->at(stubRef));
                        acomb.stubs_z   .push_back(reader.vb_z   ->at(stubRef));
                        acomb.stubs_bool.push_back(true);
                    } else {
                        acomb.stubs_r   .push_back(0.);
                        acomb.stubs_phi .push_back(0.);
                        acomb.stubs_z   .push_back(0.);
                        acomb.stubs_bool.push_back(false);
                    }
                }

                acomb.hitBits = getHitBits(acomb.stubs_bool);

                if (verbose_>2) {
                    std::cout << Debug() << "... ... ... comb: " << icomb << " " << acomb;
                    std::cout << std::endl;
                }

                // _____________________________________________________________
                // Fit
                TTTrack2 atrack;
                fitstatus = fitter_->fit(acomb, atrack);

                atrack.setTower    (po_.tower);
                atrack.setRoadRef  (acomb.roadRef);
                atrack.setCombRef  (acomb.combRef);
                atrack.setPtSegment(acomb.ptSegment);
                atrack.setHitBits  (acomb.hitBits);
                atrack.setStubRefs (acomb.stubRefs);
                tracks.push_back(atrack);

                if (verbose_>2)  std::cout << Debug() << "... ... ... track: " << icomb << " status: " << fitstatus << std::endl;
            }
        }  // loop over the roads


        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " # tracks: " << tracks.size() << std::endl;
        if (verbose_>3) {
            for (unsigned itrack=0; itrack!=tracks.size(); ++itrack) {
                std::cout << "... ... track: " << itrack << " " << tracks.at(itrack) << std::endl;
            }
        }

        // _____________________________________________________________________
        // Remove fails and duplicates

        for (unsigned itrack=0; itrack<tracks.size(); ++itrack) {  // all tracks
            for (unsigned jtrack=0; jtrack<itrack; ++jtrack) {  // only non ghost tracks
                if (tracks.at(jtrack).isGhost())  continue;

                bool isGhost = ghostBuster_.isGhostTrack(tracks.at(jtrack).stubRefs(), tracks.at(itrack).stubRefs());
                if (isGhost) {
                    tracks.at(itrack).setAsGhost();
                }
            }
        }

        if (! tracks.empty())
            ++nKept;

        if (tracks.size() > (unsigned) po_.maxTracks)
            tracks.resize(po_.maxTracks);

        writer.fill(tracks);
        ++nRead;
    }

    if (nRead == 0) {
        std::cout << Error() << "Failed to read any event." << std::endl;
        return 1;
    }

    if (verbose_)  std::cout << Info() << Form("Read: %7ld, triggered: %7ld", nRead, nKept) << std::endl;


    // _________________________________________________________________________
    // Write histograms

    for (std::map<TString, TH1F *>::const_iterator it=fitter_->histograms_.begin();
         it!=fitter_->histograms_.end(); ++it) {
        if (it->second)  it->second->SetDirectory(gDirectory);
    }

    long long nentries = writer.writeTree();
    assert(nentries == nRead);

    return 0;
}


// _____________________________________________________________________________
// Main driver
int TrackFitter::run() {
    int exitcode = 0;
    Timing(1);

    exitcode = makeTracks(po_.input, po_.output);
    if (exitcode)  return exitcode;
    Timing();

    return exitcode;
}
