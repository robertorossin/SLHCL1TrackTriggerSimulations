#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/StubCutter.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Helper.h"
using namespace slhcl1tt;

#include <algorithm>
#include <cassert>
#include <iterator>
#include <iostream>
#include <stdexcept>

static const unsigned NLAYERS = 6;
static const unsigned NDISKS = 5;


// _____________________________________________________________________________
StubCutter::StubCutter() {

    // See $CMSSW_RELEASE_BASE/src/L1Trigger/TrackTrigger/python/TTStubAlgorithmRegister_cfi.py
    // Default is TTStubAlgorithm_tab2013_PixelDigi_
    barrel_ds_cuts_ = {
        0, 2.5, 2.5, 3.0, 4.5, 5.5, 6.5
    };

    std::vector<float> endcap_ds_cuts_0 = {
        0
    };
    std::vector<float> endcap_ds_cuts_1 = {
        0, 2.0, 2.0, 2.0, 2.0, 2.5, 2.5, 2.5, 3.0, 3.5, 4.5, 3.0, 3.5, 4.0, 4.5, 5.0
    };
    std::vector<float> endcap_ds_cuts_2 = {
        0, 1.5, 2.0, 2.0, 2.0, 2.0, 2.5, 2.5, 2.5, 3.0, 4.0, 2.5, 3.0, 3.5, 4.0, 4.5
    };
    std::vector<float> endcap_ds_cuts_3 = {
        0, 1.5, 2.0, 2.0, 2.0, 2.0, 2.0, 2.5, 2.5, 2.5, 3.5, 4.0, 2.5, 3.0, 3.5, 4.0
    };
    std::vector<float> endcap_ds_cuts_4 = {
        0, 1.5, 1.5, 1.5, 2.0, 2.0, 2.0, 2.0, 2.5, 2.5, 3.0, 3.5, 2.5, 2.5, 3.0, 3.5
    };
    std::vector<float> endcap_ds_cuts_5 = {
        0, 1.5, 1.5, 1.5, 1.5, 1.5, 2.0, 2.0, 2.0, 2.5, 2.5, 3.0, 3.5, 2.5, 2.5, 3.0
    };

    endcap_ds_cuts_.push_back(endcap_ds_cuts_0);
    endcap_ds_cuts_.push_back(endcap_ds_cuts_1);
    endcap_ds_cuts_.push_back(endcap_ds_cuts_2);
    endcap_ds_cuts_.push_back(endcap_ds_cuts_3);
    endcap_ds_cuts_.push_back(endcap_ds_cuts_4);
    endcap_ds_cuts_.push_back(endcap_ds_cuts_5);

}

// _____________________________________________________________________________
bool StubCutter::applyCuts(unsigned moduleId, const float ds) {
    unsigned lay16 = compressLayer(decodeLayer(moduleId));

    if (lay16 < 6) {
        float window = barrel_ds_cuts_.at(lay16+1);

        int dsI = ((ds>0) -(ds<0))*floor(fabs(ds*2));  /// In HALF-STRIP units!
        int windowI = ((window>0) -(window<0))*floor(fabs(window*2)); /// In HALF-STRIP units!
        return abs(dsI) <= windowI;

    } else if (lay16 < 11) {
        lay16 -= 6;
        return true;  // NOT IMPLEMENTED

    } else if (lay16 < 16) {
        lay16 -= 11;
        return true;  // NOT IMPLEMENTED

    }
    return true;
}

// _____________________________________________________________________________
void StubCutter::print() {
    std::cout << "barrel ds cuts: ";
    std::copy(barrel_ds_cuts_.begin(), barrel_ds_cuts_.end(), std::ostream_iterator<float>(std::cout, " "));
    std::cout << std::endl;

    std::cout << "endcap ds cuts: ";
    for (unsigned i=0; i<endcap_ds_cuts_.size(); i++) {
        std::copy(endcap_ds_cuts_.at(i).begin(), endcap_ds_cuts_.at(i).end(), std::ostream_iterator<float>(std::cout, " "));
        std::cout << std::endl;
    }
}
