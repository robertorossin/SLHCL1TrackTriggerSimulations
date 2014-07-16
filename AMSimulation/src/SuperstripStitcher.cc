#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/SuperstripStitcher.h"
using namespace slhcl1tt;

#include <iostream>

static const unsigned MAX_NLAYERS = pattern_type().size();


void SuperstripStitcher::init() {
    if (!(6 <= nLayers_ && nLayers_ <= 8)) {
        std::cerr << "Incorrect nLayers is given: " << nLayers_ << std::endl;
        return;
    }
    if (!(nFakeSuperstrips_ <= 3)) {
        std::cerr << "Incorrect nFakeSuperstrip is given: " << nFakeSuperstrips_ << std::endl;
        return;
    }

    unsigned seven_choose_six[7 * 6] = {
        0, 1, 2, 3, 4, 5,
        0, 1, 2, 3, 4, 6,
        0, 1, 2, 3, 5, 6,
        0, 1, 2, 4, 5, 6,
        0, 1, 3, 4, 5, 6,
        0, 2, 3, 4, 5, 6,
        1, 2, 3, 4, 5, 6
    };

    unsigned eight_choose_six[8 * 7 / 2 * 6] = {
        0, 1, 2, 3, 4, 5,
        0, 1, 2, 3, 4, 6,
        0, 1, 2, 3, 4, 7,
        0, 1, 2, 3, 5, 6,
        0, 1, 2, 3, 5, 7,
        0, 1, 2, 3, 6, 7,
        0, 1, 2, 4, 5, 6,
        0, 1, 2, 4, 5, 7,
        0, 1, 2, 4, 6, 7,
        0, 1, 2, 5, 6, 7,
        0, 1, 3, 4, 5, 6,
        0, 1, 3, 4, 5, 7,
        0, 1, 3, 4, 6, 7,
        0, 1, 3, 5, 6, 7,
        0, 1, 4, 5, 6, 7,
        0, 2, 3, 4, 5, 6,
        0, 2, 3, 4, 5, 7,
        0, 2, 3, 4, 6, 7,
        0, 2, 3, 5, 6, 7,
        0, 2, 4, 5, 6, 7,
        0, 3, 4, 5, 6, 7,
        1, 2, 3, 4, 5, 6,
        1, 2, 3, 4, 5, 7,
        1, 2, 3, 4, 6, 7,
        1, 2, 3, 5, 6, 7,
        1, 2, 4, 5, 6, 7,
        1, 3, 4, 5, 6, 7,
        2, 3, 4, 5, 6, 7
    };

    unsigned eight_choose_seven[8 * 7] = {
        0, 1, 2, 3, 4, 5, 6,
        0, 1, 2, 3, 4, 5, 7,
        0, 1, 2, 3, 4, 6, 7,
        0, 1, 2, 3, 5, 6, 7,
        0, 1, 2, 4, 5, 6, 7,
        0, 1, 3, 4, 5, 6, 7,
        0, 2, 3, 4, 5, 6, 7,
        1, 2, 3, 4, 5, 6, 7
    };

    // Assign
    seven_choose_six_.assign(seven_choose_six, seven_choose_six+(7 * 6));
    eight_choose_six_.assign(eight_choose_six, eight_choose_six+(8 * 7 / 2 * 6));
    eight_choose_seven_.assign(eight_choose_seven, eight_choose_seven+(8 * 7));
}


std::vector<pattern_type> SuperstripStitcher::stitch(const std::vector<addr_type>& superstrips) {
    ret_.clear();
    if (nLayers_ >  superstrips.size() + nFakeSuperstrips_)  // nFakeSuperstrips is not enough, return empty vector
        return ret_;

    // Copy up to 8 superstrips
    ret_.resize(1);
    std::copy(superstrips.begin(),
              superstrips.begin() + (MAX_NLAYERS < superstrips.size() ? MAX_NLAYERS : superstrips.size()),
              ret_.back().begin() );

    // Case 0: Exact size
    if (nLayers_ == superstrips.size())
        return ret_;

    // Case 1: Need fake superstrips
    unsigned i = superstrips.size(), j = 0;
    while (nLayers_ > i) {  // lack of 1 - 3 superstrips
        ret_.back().at(i) = fakeSuperstripId_ - (i - superstrips.size());
        i++;
    }
    if (nLayers_ == i)
        return ret_;

    // Case 2: Need to do all combinations
    // The combinations are hardcoded for nLayers = 6, 7, 8
    std::vector<unsigned>* n_choose_k_ptr = 0;
    if (superstrips.size() == 7 && nLayers_ == 6) {
        n_choose_k_ptr = &seven_choose_six_;
    } else if (superstrips.size() == 8 && nLayers_ == 6) {
        n_choose_k_ptr = &eight_choose_six_;
    } else if (superstrips.size() == 8 && nLayers_ == 7) {
        n_choose_k_ptr = &eight_choose_seven_;
    } else {
        std::cerr << "Not coded to do '" << superstrips.size() << " choose " << nLayers_ << "' combination. Sorry." << std::endl;
        return ret_;
    }

    ret2_.clear();
    ret2_.resize(n_choose_k_ptr -> size() / nLayers_);
    for (i=0; i<ret2_.size(); i++) {
        for (j=0; j<nLayers_; j++) {
            ret2_.at(i).at(j) = ret_.back().at(n_choose_k_ptr -> at(i * nLayers_ + j));
        }
    }
    return ret2_;
}

void SuperstripStitcher::print() {
    std::cout << "nLayers: " << nLayers_ << " nFakeSuperstrips: " << nFakeSuperstrips_ << std::endl;
}
