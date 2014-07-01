#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/SuperstripHasher.h"

#include <iostream>
#include <iterator>


namespace slhcl1tt {

void SuperstripHasher::init() {
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

    fake_superstrip_hash_ = 15428;
}


key_type SuperstripHasher::hashModule(id_type moduleId) const {
    id_type lay = decodeLayer4bit(moduleId);
    id_type lad = decodeLadder(moduleId);
    id_type mod = decodeModule(moduleId);

    key_type h = 0;
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

    if (h > fake_superstrip_hash_) {
        std::cout << "ERROR: Unexpected moduleId hash: " << h << std::endl;
    }

    return h;
}

key_type SuperstripHasher::hash(addr_type superstripId) const {
    id_type moduleId = decodeSuperstripModuleId(superstripId);
    id_type col = decodeSuperstripSubLadder(superstripId);
    id_type row = decodeSuperstripSubModule(superstripId);

    key_type h = 0;
    if (isFakeSuperstripId(superstripId)) {
        h = fake_superstrip_hash_;
    } else {
        h = hashModule(moduleId);
    }

    if (col > (id_type) 1 << subLadderNBits_) {
        std::cout << "ERROR: Unexpected subLadder col: " << col << std::endl;
    }
    if (row > (id_type) 1 << subModuleNBits_) {
        std::cout << "ERROR: Unexpected subModule row: " << row << std::endl;
    }

    // Finally
    h = ( (h << (subLadderNBits_ + subModuleNBits_))
        | (col << subModuleNBits_)
        | (row) );

    return h;
}

void SuperstripHasher::print() {
    std::cout << "subLadderNBits, subModuleNBits, maxHashModule: " << subLadderNBits_ << ", " << subModuleNBits_ << ", " << fake_superstrip_hash_ << std::endl;
}

}
