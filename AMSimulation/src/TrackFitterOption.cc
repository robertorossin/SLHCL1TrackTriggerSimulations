#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TrackFitterOption.h"

#include <iostream>
#include <algorithm>
#include <iterator>


namespace slhcl1tt {

std::ostream& operator<<(std::ostream& o, const TrackFitterOption& po) {
    o << "nLayers: " << po.nLayers << "  pqType: " << po.pqType << "  pbins: " << po.pbins << "  qbins: " << po.qbins << "  pmin: " << po.pmin << "  qmin: " << po.qmin << "  pmax: " << po.pmax << "  qmax: " << po.qmax << std::endl;
    o << "sigma: " << po.sigma << "  minWeight: " << po.minWeight << std::endl;
    return o;
}

}
