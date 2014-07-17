#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/SuperstripArbiter.h"
using namespace slhcl1tt;

#include <iostream>
#include <iterator>


void SuperstripArbiter::init() {
    if (subLadderSize_.size() != nLayers_) {
        std::cerr << "Incorrect subLadderSize is given: " << subLadderSize_.size() << std::endl;
        return;
    }
    if (subModuleSize_.size() != nLayers_) {
        std::cerr << "Incorrect subModuleSize is given: " << subModuleSize_.size() << std::endl;
        return;
    }
    if (subLadderECSize_.size() != nRings_) {
        std::cerr << "Incorrect subLadderECSize is given: " << subLadderECSize_.size() << std::endl;
        return;
    }
    if (subModuleECSize_.size() != nRings_) {
        std::cerr << "Incorrect subModuleECSize is given: " << subModuleECSize_.size() << std::endl;
        return;
    }

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


id_type SuperstripArbiter::subladder(id_type moduleId, id_type col, bool isHalfStrip) const {
    if (isHalfStrip)
        col /= 2;
    if (!isPSModule(moduleId))
        col *= 16;

    if (isBarrelModule(moduleId)) {
        id_type index = decodeLayer(moduleId) - 5;
        col /= subLadderSize_.at(index);
    } else {
        id_type index = decodeLadder(moduleId);
        col /= subLadderECSize_.at(index);
    }
    return col;
}

id_type SuperstripArbiter::submodule(id_type moduleId, id_type row, bool isHalfStrip) const {
    if (isHalfStrip)
        row /= 2;

    if (isBarrelModule(moduleId)) {
        id_type index = decodeLayer(moduleId) - 5;
        row /= subModuleSize_.at(index);
    } else {
        id_type index = decodeLadder(moduleId);
        row /= subModuleECSize_.at(index);
    }
    return row;
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

