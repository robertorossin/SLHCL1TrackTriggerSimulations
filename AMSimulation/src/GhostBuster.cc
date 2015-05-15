#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/GhostBuster.h"
using namespace slhcl1tt;

#include <cassert>
#include <iostream>


// _____________________________________________________________________________
bool GhostBuster::isGhostRoad(const std::vector<unsigned>& superstripIds1, const std::vector<unsigned>& superstripIds2, const unsigned tolerance) const {
    assert(superstripIds1.size() == superstripIds2.size());

    unsigned n = 0;
    for (unsigned i=0; i<superstripIds1.size(); ++i) {
        if (superstripIds1.at(i) != superstripIds2.at(i))
            ++n;
    }

    if (n >= tolerance)
        return true;

    return false;
}

// _____________________________________________________________________________
bool GhostBuster::isGhostTrack(const std::vector<unsigned>& stubRefs1, const std::vector<unsigned>& stubRefs2, const unsigned tolerance) const {
    assert(stubRefs1.size() == stubRefs2.size());

    unsigned n = 0;
    for (unsigned i=0; i<stubRefs1.size(); ++i) {
        if (stubRefs1.at(i) != stubRefs2.at(i))
            ++n;
    }

    if (n >= tolerance)
        return true;

    return false;
}

// _____________________________________________________________________________
void GhostBuster::print() {
    std::cout << std::endl;
}
