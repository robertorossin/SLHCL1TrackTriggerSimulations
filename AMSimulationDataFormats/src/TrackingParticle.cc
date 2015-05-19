#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TrackingParticle.h"
#include <iostream>

namespace slhcl1tt {

std::ostream& operator<<(std::ostream& o, const TrackingParticle& particle) {
    o << "tpId: " << particle.tpId << " invPt: " << particle.invPt << " phi0: " << particle.phi0 << " cottheta: " << particle.cottheta << " z0: " << particle.z0 << " d0: " << particle.d0 << std::endl;
    return o;
}

}  // namespace slhcl1tt
