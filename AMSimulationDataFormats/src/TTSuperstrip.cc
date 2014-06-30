#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTPattern.h"
#include <iostream>


namespace slhcl1tt {


void constructSuperstrip(addr_type id, bit_type bit, TTSuperstrip& ss) {
    ss.id = id;
    ss.bit = bit;
}

void constructHit(float x, float y, float z, float xerr, float yerr, float zerr, int charge, float pt, addr_type ssId, bit_type ssBit, TTHit& hit) {
    hit.x = x;
    hit.y = y;
    hit.z = z;
    hit.xError = xerr;
    hit.yError = yerr;
    hit.zError = zerr;
    hit.charge = charge;
    hit.pt = pt;
    hit.superstripId = ssId;
    hit.superstripBit = ssBit;
}

// _____________________________________________________________________________
std::ostream& operator<<(std::ostream& o, const TTSuperstrip& ss) {
    o << "id: " << ss.id << "  bit: " << ss.bit << " ";
    return o;
}

std::ostream& operator<<(std::ostream& o, const TTHit& hit) {
    o << "x: " << hit.x << "  y: " << hit.y << "  z: " << hit.z
      << "  xerr: " << hit.xError << "  yerr: " << hit.yError << "  zerr: " << hit.zError
      << "  signed pt: " << hit.charge * hit.pt
      << "  ssId: " << hit.superstripId << "  ssBit: " << hit.superstripBit << " ";
    return o;
}

}  // namespace slhcl1tt
