#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/MCTruthAssociator.h"
using namespace slhcl1tt;

#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>


namespace {
// Comparator
bool sortByPt(const TrackingParticle& lhs, const TrackingParticle& rhs) {
    return (1.0/lhs.invPt) > (1.0/rhs.invPt);
}

bool sortByMatchQuality(const std::pair<unsigned, float>& lhs, const std::pair<unsigned, float>& rhs) {
    return lhs.second < rhs.second;
}

float absDiff(float lhs, float rhs) {
    return std::abs(lhs - rhs);
}
}

// _____________________________________________________________________________
MCTruthAssociator::MCTruthAssociator() {
    const float sigma = 5.;
    //cut_invPt_    = 0.000165559 * sigma;
    //cut_phi0_     = 7.29527e-05 * sigma;
    //cut_cottheta_ = 0.00215212  * sigma;
    //cut_z0_       = 0.0819977   * sigma;
    //cut_d0_       = 1.00000     * sigma;

    cut_invPt_    = 0.002759 * sigma;
    cut_phi0_     = 0.000646 * sigma;
    cut_cottheta_ = 0.002695 * sigma;
    cut_z0_       = 0.08586  * sigma;
    cut_d0_       = 1.00000  * sigma;
}

// _____________________________________________________________________________
void MCTruthAssociator::associate(std::vector<TrackingParticle>& trkParts, std::vector<TTTrack2>& tracks) {

    // Sort tracking particles by pT
    std::sort(trkParts.begin(), trkParts.end(), sortByPt);

    const unsigned nparts  = trkParts.size();
    const unsigned ntracks = tracks.size();

    // Create the map for matching
    std::map<unsigned, std::vector<std::pair<unsigned, float> > > matches;  // key=ipart, value=vector of (itrack, quality) pair
    for (unsigned ipart=0; ipart<nparts; ++ipart) {
        matches[ipart] = std::vector<std::pair<unsigned, float> >();
    }

    // Loop on all tracking particles
    for (unsigned ipart=0; ipart<nparts; ++ipart) {
        const TrackingParticle& trkPart = trkParts.at(ipart);

        // Loop on all reconstructed tracks
        for (unsigned itrack=0; itrack<ntracks; ++itrack) {
            const TTTrack2& track = tracks.at(itrack);

            if (accept(trkPart, track)) {
                float quality = absDiff(trkPart.invPt   , track.invPt());  // using diff in invPt as quality
                matches.at(ipart).push_back(std::make_pair(itrack, quality));
            }
        }
    }

    // Debug
    //for (unsigned ipart=0; ipart<nparts; ++ipart) {
    //    const std::vector<std::pair<unsigned, float> >& ipart_matches = matches.at(ipart);
    //    for (unsigned imatch=0; imatch<ipart_matches.size(); ++imatch) {
    //        unsigned itrack = ipart_matches.at(imatch).first;
    //        float quality = ipart_matches.at(imatch).second;
    //        std::cout << "ipart: " << ipart << " itrack: " << itrack << " quality: " << quality << std::endl;
    //    }
    //}

    std::vector<int> mcCategories(nparts, ParticleCategory::NOTFOUND);
    std::vector<int> recoCategories(ntracks, TrackCategory::FAKE);

    // Loop on all tracking particles
    for (unsigned ipart=0; ipart<nparts; ++ipart) {
        std::vector<std::pair<unsigned, float> >& ipart_matches = matches.at(ipart);
        std::sort(ipart_matches.begin(), ipart_matches.end(), sortByMatchQuality);

        if (ipart_matches.size() == 0) {
            // Do nothing

        } else {
            bool foundTheBest = false;
            for (unsigned imatch=0; imatch<ipart_matches.size(); ++imatch) {
                unsigned itrack = ipart_matches.at(imatch).first;

                //if (recoCategories.at(itrack) == TrackCategory::GOOD)
                //    continue;
                if (recoCategories.at(itrack) > 0)
                    continue;

                if (!foundTheBest) {
                    mcCategories.at(ipart) = ParticleCategory::FOUND;
                    //recoCategories.at(itrack) = TrackCategory::GOOD;
                    recoCategories.at(itrack) = itrack;
                    foundTheBest = true;

                } else {
                    mcCategories.at(ipart) = ParticleCategory::FOUND;
                    recoCategories.at(itrack) = TrackCategory::DUPLICATE;
                }
            }
        }
    }

    // Sanity check
    unsigned ngoods = 0, nduplicates = 0, nfakes = 0, nfounds = 0, nnotfounds = 0;

    for (unsigned ipart=0; ipart<nparts; ++ipart) {
        const int cat = mcCategories.at(ipart);
        if (cat == ParticleCategory::NOTFOUND)
            ++nnotfounds;
        else
            ++nfounds;
    }

    for (unsigned itrack=0; itrack<ntracks; ++itrack) {
        const int cat = recoCategories.at(itrack);
        if (cat == TrackCategory::FAKE)
            ++nfakes;
        else if (cat == TrackCategory::DUPLICATE)
            ++nduplicates;
        else
            ++ngoods;

        tracks.at(itrack).setSynTpId(cat);
    }

    assert(nfounds + nnotfounds == nparts);
    assert(ngoods + nduplicates + nfakes == ntracks);
    assert(nfounds == ngoods);
}

// _____________________________________________________________________________
bool MCTruthAssociator::accept(const TrackingParticle& trkPart, const TTTrack2& track) {
    if ((absDiff(trkPart.invPt   , track.invPt())    < cut_invPt_   ) &&
        (absDiff(trkPart.phi0    , track.phi0())     < cut_phi0_    ) &&
        (absDiff(trkPart.cottheta, track.cottheta()) < cut_cottheta_) &&
        (absDiff(trkPart.z0      , track.z0())       < cut_z0_      ) &&
        (absDiff(trkPart.d0      , track.d0())       < cut_d0_      )) {
        return true;
    }
    return false;
}

// _____________________________________________________________________________
void MCTruthAssociator::print() {
    std::cout << "cuts on invPt: " << cut_invPt_ << " phi0: " << cut_phi0_ << " cottheta: " << cut_cottheta_ << " z0: " << cut_z0_ << " d0: " << cut_d0_ << std::endl;
}
