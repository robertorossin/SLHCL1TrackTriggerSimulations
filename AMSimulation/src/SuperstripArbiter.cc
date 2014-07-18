#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/SuperstripArbiter.h"
using namespace slhcl1tt;

#include <iostream>
#include <algorithm>
#include <iterator>


void SuperstripArbiter::init() {
    if (subLadderSize_.size() != nLayers_)
        throw std::invalid_argument("Incorrect subLadderSize is given.");

    if (subModuleSize_.size() != nLayers_)
        throw std::invalid_argument("Incorrect subModuleSize is given.");

    if (subLadderECSize_.size() != nRings_)
        throw std::invalid_argument("Incorrect subLadderECSize is given.");

    if (subModuleECSize_.size() != nRings_)
        throw std::invalid_argument("Incorrect subModuleECSize is given.");

    // Only division by a power of two (2, 4, 8, 16, ...) is safe, as it corresponds to bit shifting
    for (unsigned i=0; i<nLayers_; ++i) {
        subLadderSize_.at(i) = 1u << mostSigBit(subLadderSize_.at(i));
        subModuleSize_.at(i) = 1u << mostSigBit(subModuleSize_.at(i));
    }
    for (unsigned i=0; i<nRings_; ++i) {
        subLadderECSize_.at(i) = 1u << mostSigBit(subLadderECSize_.at(i));
        subModuleECSize_.at(i) = 1u << mostSigBit(subModuleECSize_.at(i));
    }
}

static id_type index;
id_type SuperstripArbiter::subladder(id_type moduleId, id_type col, bool isHalfStrip) const {
    if (isHalfStrip)
        col /= 2;
    if (!isPSModule(moduleId))
        col *= 16;

    if (isBarrelModule(moduleId)) {
        index = decodeLayer(moduleId) - 5;
        col /= subLadderSize_.at(index);
    } else {
        index = decodeLadder(moduleId);
        col /= subLadderECSize_.at(index);
    }
    return col;
}

id_type SuperstripArbiter::submodule(id_type moduleId, id_type row, bool isHalfStrip) const {
    if (isHalfStrip)
        row /= 2;

    if (isBarrelModule(moduleId)) {
        index = decodeLayer(moduleId) - 5;
        row /= subModuleSize_.at(index);
    } else {
        index = decodeLadder(moduleId);
        row /= subModuleECSize_.at(index);
    }
    return row;
}

id_type SuperstripArbiter::minSubLadderSize() const {
    // Only check barrel
    return *(std::min_element(subLadderSize_.begin(), subLadderSize_.end()) );
}

id_type SuperstripArbiter::minSubModuleSize() const {
    // Only check barrel
    return *(std::min_element(subModuleSize_.begin(), subModuleSize_.end()) );
}

void SuperstripArbiter::print() {
    std::cout << "subLadderSize: ";
    std::copy(subLadderSize_.begin(), subLadderSize_.end(), std::ostream_iterator<id_type>(std::cout, " "));
    std::cout << std::endl;
    std::cout << "subModuleSize: ";
    std::copy(subModuleSize_.begin(), subModuleSize_.end(), std::ostream_iterator<id_type>(std::cout, " "));
    std::cout << std::endl;
    std::cout << "subLadderECSize: ";
    std::copy(subLadderECSize_.begin(), subLadderECSize_.end(), std::ostream_iterator<id_type>(std::cout, " "));
    std::cout << std::endl;
    std::cout << "subModuleECSize: ";
    std::copy(subModuleECSize_.begin(), subModuleECSize_.end(), std::ostream_iterator<id_type>(std::cout, " "));
    std::cout << std::endl;
}
