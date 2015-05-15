#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/HitBuffer.h"
using namespace slhcl1tt;

#include <cassert>
#include <iostream>
#include <stdexcept>


// _____________________________________________________________________________
int HitBuffer::init(unsigned maxBins) {
    superstripHits_.clear();

    superstripBools_.clear();
    superstripBools_.resize(maxBins);

    return 0;
}

// _____________________________________________________________________________
void HitBuffer::reset() {
    superstripHits_.clear();

    std::fill(superstripBools_.begin(), superstripBools_.end(), false);
}

// _____________________________________________________________________________
void HitBuffer::insert(superstrip_type ss, unsigned stubRef) {
    superstripHits_[ss].push_back(stubRef);

    superstripBools_[ss] = true;
}

// _____________________________________________________________________________
void HitBuffer::freeze(unsigned maxStubs) {
    for (std::map<superstrip_type, std::vector<unsigned> >::iterator it=superstripHits_.begin();
         it!=superstripHits_.end(); ++it) {
        if (it->second.size() > maxStubs)
            it->second.resize(maxStubs);
    }

    assert(superstripBools_.size() != 0);
    frozen_ = true;
}

// _____________________________________________________________________________
void HitBuffer::print() {
    std::cout << "nbins: " << superstripBools_.size() << std::endl;
}
