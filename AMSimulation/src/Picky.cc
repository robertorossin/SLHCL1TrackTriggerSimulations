#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Picky.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/Helper.h"
using namespace slhcl1tt;

#include <algorithm>
#include <cassert>
#include <iterator>
#include <iostream>
#include <stdexcept>


Picky::Picky()
 :  simPt_    (-9999.),
    simEta_   (-9999.),
    simPhi_   (-9999.),
    simVZ_    (-9999.),
    simCharge_(-999999) {

    // 4 possible r coordinates per barrel layer
    barrel_r_coords_ = {
        21.3678,21.9722,23.2104,23.8148,
        34.1546,34.7590,36.1954,36.7998,
        49.2178,49.8222,51.2582,51.8626,
        67.0078,67.6122,69.0080,69.6124,
        87.1979,87.8023,89.1981,89.8025,
        106.4078,107.0122,108.4078,109.0122
    };

    // 6 possible z coordinates per endcap disk
    endcap_z_coords_ = {
        128.494,130.074,130.294,132.614,134.194,134.414,
        153.295,154.875,155.095,157.415,158.995,159.215,
        182.654,184.234,184.454,186.774,188.354,188.574,
        217.408,218.988,219.208,221.528,223.108,223.328,
        258.550,260.130,260.350,262.670,264.250,264.470
    };

    // phi cut per barrel layer
    barrel_phi_cuts_ = { 0.004, 0.004, 0.005, 0.007, 0.009, 0.012 };

    // z cut per barrel layer
    barrel_z_cuts_   = { 0.300, 0.400, 0.500, 3.000, 3.500, 4.000 };

    // phi cut per endcap disk
    endcap_phi_cuts_ = { 0.100, 0.100, 0.100, 0.100, 0.100 };  // disabled

    // r cut per endcap disk
    endcap_r_cuts_   = { 9.900, 9.900, 9.900, 9.900, 9.900 };  // disabled

    assert(barrel_r_coords_.size() == 6 * 4 && barrel_phi_cuts_.size() == 6 && barrel_z_cuts_.size() == 6);
    assert(endcap_z_coords_.size() == 5 * 6 && endcap_phi_cuts_.size() == 5 && endcap_r_cuts_.size() == 5);
}


void Picky::setTrack(float pt, float eta, float phi, float vz, int charge) {
    simPt_     = pt;
    simEta_    = eta;
    simPhi_    = phi;
    simVZ_     = vz;
    simCharge_ = charge;
}

bool Picky::applyCuts(unsigned lay16, const float& absdphi, const float& absdz, const float& absdr) {
    if (lay16 < 6) {
        return (absdphi < barrel_phi_cuts_.at(lay16) && absdz < barrel_z_cuts_.at(lay16));

    } else if (lay16 < 11) {
        return (absdphi < endcap_phi_cuts_.at(lay16-6) && absdr < endcap_r_cuts_.at(lay16-6));

    } else if (lay16 < 16) {
        return (absdphi < endcap_phi_cuts_.at(lay16-11) && absdr < endcap_r_cuts_.at(lay16-11));

    }
    return true;
}

unsigned Picky::findInterior(unsigned lay16, const float& z, const float& r) {
    if (lay16 < 6) {
        for (unsigned i=lay16*4, j=0; i<barrel_r_coords_.size()-1; ++i, ++j)
            if (r < barrel_r_coords_.at(i+1))
                return j;
        return 4-1;

    } else if (lay16 < 11) {
        for (unsigned i=(lay16-6)*4, j=0; i<endcap_z_coords_.size()-1; ++i, ++j)
            if (z < endcap_z_coords_.at(i+1))
                return j;
        return 6-1;

    } else if (lay16 < 16) {
        for (unsigned i=(lay16-11)*4, j=0; i<endcap_z_coords_.size()-1; ++i, ++j)
            if (z < endcap_z_coords_.at(i+1))
                return j;
        return 6-1;

    }
    return 99;
}

void Picky::print() {
    std::cout << "barrel r: ";
    std::copy(barrel_r_coords_.begin(), barrel_r_coords_.end(), std::ostream_iterator<float>(std::cout, " "));
    std::cout << std::endl;

    std::cout << "barrel phi cuts: ";
    std::copy(barrel_phi_cuts_.begin(), barrel_phi_cuts_.end(), std::ostream_iterator<float>(std::cout, " "));
    std::cout << std::endl;

    std::cout << "barrel z cuts: ";
    std::copy(barrel_z_cuts_.begin(), barrel_z_cuts_.end(), std::ostream_iterator<float>(std::cout, " "));
    std::cout << std::endl;

    std::cout << "endcap z: ";
    std::copy(endcap_z_coords_.begin(), endcap_z_coords_.end(), std::ostream_iterator<float>(std::cout, " "));
    std::cout << std::endl;

    std::cout << "endcap phi cuts: ";
    std::copy(endcap_phi_cuts_.begin(), endcap_phi_cuts_.end(), std::ostream_iterator<float>(std::cout, " "));
    std::cout << std::endl;

    std::cout << "endcap r cuts: ";
    std::copy(endcap_r_cuts_.begin(), endcap_r_cuts_.end(), std::ostream_iterator<float>(std::cout, " "));
    std::cout << std::endl;
}
