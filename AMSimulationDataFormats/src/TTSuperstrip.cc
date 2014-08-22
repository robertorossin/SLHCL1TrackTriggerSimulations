#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTPattern.h"
#include <iostream>


namespace slhcl1tt {

std::ostream& operator<<(std::ostream& o, const TTSuperstrip& ss) {
    o << "id: " << ss.id << " bit: " << ss.bit << " ";
    return o;
}

std::ostream& operator<<(std::ostream& o, const TTHit& hit) {
    o << "x: " << hit.x << " y: " << hit.y << " z: " << hit.z
      << " xerr: " << hit.xError << " yerr: " << hit.yError << " zerr: " << hit.zError
      << " signed pt: " << hit.charge * hit.pt
      << " ssId: " << hit.superstripId << " ";
    return o;
}

}  // namespace slhcl1tt
