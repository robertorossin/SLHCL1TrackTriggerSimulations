#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTRoadComb.h"
#include <iostream>

namespace slhcl1tt {

std::ostream& operator<<(std::ostream& o, const TTRoadComb& comb) {
    o << "roadRef: " << comb.roadRef << " combRef: " << comb.combRef << " patternRef: " << comb.patternRef << " ptSegment: " << comb.ptSegment << " hitBits: " << comb.hitBits << " stubs: (";
    for (unsigned i=0; i<comb.stubRefs.size(); ++i)
        std::cout << "(" << comb.stubs_r.at(i) << "," << comb.stubs_phi.at(i) << "," << comb.stubs_z.at(i) << "," << comb.stubs_bool.at(i) << "), ";
    o << ")" << std::endl;
    return o;
}

}  // namespace slhcl1tt
