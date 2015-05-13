#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTRoadComb.h"
#include <iostream>

namespace slhcl1tt {

std::ostream& operator<<(std::ostream& o, const TTRoadComb& road) {
    o << "Road no: " << road.roadRef << " comb no: " << road.combRef << " pattern no: " << road.patternRef << " tower: " << road.tower << " # stubs: " << road.nstubs << " stubs: (";
    for (unsigned i=0; i<road.stubRefs.size(); ++i)
        std::cout << "(" << road.stubs_r.at(i) << "," << road.stubs_phi.at(i) << "," << road.stubs_z.at(i) << "," << road.stubs_bool.at(i) << "), ";
    o << ")" << std::endl;
    return o;
}

}  // namespace slhcl1tt
