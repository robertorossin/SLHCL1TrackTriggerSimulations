#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/SuperstripHasher.h"
using namespace slhcl1tt;

#include <iostream>


void SuperstripHasher::init() {
    if (subLadderNBits_ > (iModuleIdStartBit_ - iSubLadderStartBit_))
        throw std::invalid_argument("Incorrect subLadderNBits is given.");

    if (subModuleNBits_ > (iSubLadderStartBit_ - iSubModuleStartBit_))
        throw std::invalid_argument("Incorrect subModuleNBits is given.");

    id_type barrel_z_divisions[6] = {
        63, 55, 54, 24, 24, 24
    };

    id_type barrel_phi_divisions[6] = {
        16, 24, 34, 48, 62, 76
    };

    id_type barrel_layer_offsets[7] = {
        0,
        barrel_layer_offsets[0] + barrel_phi_divisions[0] * barrel_z_divisions[0],
        barrel_layer_offsets[1] + barrel_phi_divisions[1] * barrel_z_divisions[1],
        barrel_layer_offsets[2] + barrel_phi_divisions[2] * barrel_z_divisions[2],
        barrel_layer_offsets[3] + barrel_phi_divisions[3] * barrel_z_divisions[3],
        barrel_layer_offsets[4] + barrel_phi_divisions[4] * barrel_z_divisions[4],
        barrel_layer_offsets[5] + barrel_phi_divisions[5] * barrel_z_divisions[5]
    };

    id_type endcap_ring_divisions[15] = {
        20, 24, 28, 28, 32, 36, 36, 40, 40, 52, 56, 64, 68, 76, 80
    };

    id_type endcap_ring_offsets[16] = {
        0,
        endcap_ring_offsets[ 0] + endcap_ring_divisions[ 0],
        endcap_ring_offsets[ 1] + endcap_ring_divisions[ 1],
        endcap_ring_offsets[ 2] + endcap_ring_divisions[ 2],
        endcap_ring_offsets[ 3] + endcap_ring_divisions[ 3],
        endcap_ring_offsets[ 4] + endcap_ring_divisions[ 4],
        endcap_ring_offsets[ 5] + endcap_ring_divisions[ 5],
        endcap_ring_offsets[ 6] + endcap_ring_divisions[ 6],
        endcap_ring_offsets[ 7] + endcap_ring_divisions[ 7],
        endcap_ring_offsets[ 8] + endcap_ring_divisions[ 8],
        endcap_ring_offsets[ 9] + endcap_ring_divisions[ 9],
        endcap_ring_offsets[10] + endcap_ring_divisions[10],
        endcap_ring_offsets[11] + endcap_ring_divisions[11],
        endcap_ring_offsets[12] + endcap_ring_divisions[12],
        endcap_ring_offsets[13] + endcap_ring_divisions[13],
        endcap_ring_offsets[14] + endcap_ring_divisions[14]
    };

    // Assign
    barrel_z_divisions_.assign(barrel_z_divisions, barrel_z_divisions+6);
    barrel_phi_divisions_.assign(barrel_phi_divisions, barrel_phi_divisions+6);
    barrel_layer_offsets_.assign(barrel_layer_offsets, barrel_layer_offsets+7);
    endcap_ring_divisions_.assign(endcap_ring_divisions, endcap_ring_divisions+15);
    endcap_ring_offsets_.assign(endcap_ring_offsets, endcap_ring_offsets+16);

    fakeSuperstripHash_ = 15428;
}

static id_type lay, lad, mod;
static key_type h;
key_type SuperstripHasher::hashModule(id_type moduleId) const {
    lay = decodeLayer4bit(moduleId);
    lad = decodeLadder(moduleId);
    mod = decodeModule(moduleId);

    if (lay < 6) {  // in barrel
        h = barrel_layer_offsets_.at(lay)
          + mod * barrel_phi_divisions_.at(lay)
          + lad;

    } else {  // in endcap
        lay -= 6;
        h = barrel_layer_offsets_.at(6)  // after all barrel layers
          + lay * endcap_ring_offsets_.at(15)  // after all previous endcap rings
          + endcap_ring_offsets_.at(lad)
          + mod;
    }

    if (h > fakeSuperstripHash_) {
        std::cout << "ERROR: Unexpected moduleId hash: " << h << std::endl;
        throw std::logic_error("Unexpected moduleId hash.");
    }

    return h;
}

static id_type moduleId, col, row;
key_type SuperstripHasher::hash(addr_type superstripId) const {
    moduleId = decodeSuperstripModuleId(superstripId);
    col = decodeSuperstripSubLadder(superstripId);
    row = decodeSuperstripSubModule(superstripId);

    if (isFakeSuperstripId(superstripId)) {
        h = fakeSuperstripHash_;
        col = 0;
        row = row & 0x3;

    } else {
        h = hashModule(moduleId);
    }

    if (col > 1u << subLadderNBits_) {
        std::cout << "ERROR: Unexpected subladder col: " << col << std::endl;
        throw std::logic_error("Unexpected subladder col");
    }
    if (row > 1u << subModuleNBits_) {
        std::cout << "ERROR: Unexpected submodule row: " << row << std::endl;
        throw std::logic_error("Unexpected submodule row");
    }

    // Finally
    h = ( (h << (subLadderNBits_ + subModuleNBits_))
        | (col << subModuleNBits_)
        | (row) );

    return h;
}

void SuperstripHasher::print() {
    std::cout << "subLadderNBits: " << subLadderNBits_ << " subModuleNBits: " << subModuleNBits_ << " maxHashModule: " << fakeSuperstripHash_ << std::endl;
}
