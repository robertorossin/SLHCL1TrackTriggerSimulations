#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TTPattern.h"

namespace slhcl1tt {

void TTPattern::merge(const TTPattern& rhs) {
    for (unsigned i=0; i<superstrips_.size(); ++i) {
        bit_type ssBit = getSuperstripBit(i);
        ssBit |= rhs.getSuperstripBit(i);
        setSuperstripBit(i, ssBit);
    }
    ++frequency_;
}

void TTPattern::concatenate() {
    std::vector<addr_type> addresses;
    for (unsigned i=0; i<superstrips_.size(); ++i) {
        addresses.push_back(superstrips_.at(i).id());
    }
    addresses.resize(8,0);  // pad with zeroes
    std::copy(addresses.begin(), addresses.end(), patternId_.begin());
}

}  // namespace slhcl1tt
