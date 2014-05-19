#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TTPattern.h"
#include <iostream>

namespace slhcl1tt {

std::ostream& operator<<(std::ostream& o, const TTSuperstrip& ss) {
    o << "id: " << ss.id() << "  bit: " << ss.bit() << " ";
    return o;
}   
    
std::ostream& operator<<(std::ostream& o, const TTHit& hit) {
    o << "ssId: " << hit.superstripId() << "  x: " << hit.x() << "  y: " << hit.y() << "  z: " << hit.z()
      << "  signed pt: " << hit.charge() * hit.pt() << " ";
    return o;
}

}  // namespace slhcl1tt
