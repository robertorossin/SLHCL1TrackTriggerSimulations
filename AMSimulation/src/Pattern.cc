#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Pattern.h"

namespace slhcl1tt {

void Pattern::ready() {
    concatenate();
}

void Pattern::concatenate() {
    vuint64_t hitIds_tmp = hitIds_;
    hitIds_tmp.resize(8,0);  // pad with zeroes
    std::copy(hitIds_tmp.begin(), hitIds_tmp.end(), patternId_.begin());
}

}  // namespace slhcl1tt
