#ifndef AMSimulationDataFormats_TTSuperstrip_h_
#define AMSimulationDataFormats_TTSuperstrip_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/Helper.h"

#include <cmath>
#include <iosfwd>


namespace slhcl1tt {

// TTSuperstrip is designed to be the building block of a TTPattern.
// Nominally, it corresponds to a physical region in the detector.
// A POD type is used to reduce memory consumption.
struct TTSuperstrip {
    addr_type id;
    bit_type bit;
};


// TTHit is designed to be the building block of a TTRoad.
// Nominally, it corresponds to the smallest unit of measurement in the detector.
// A POD type is used to reduce memory consumption.
struct TTHit {
    float x;
    float y;
    float z;
    float xError;
    float yError;
    float zError;
    int charge;
    float pt;
    addr_type superstripId;
    bit_type superstripBit;

    // Functions
    // Simple trigonometrics
    float rho()                     const { return std::sqrt(x*x + y*y); }
    float phi()                     const { return std::atan2(y, x); }
    float theta()                   const { return std::atan2(rho(), z); }

    // Conformal transformantion
    float u()                       const { return x / (x*x + y*y); }
    float v()                       const { return y / (x*x + y*y); }
};

// _____________________________________________________________________________
// Functions

// Construct a TTSuperstrip from a superstrip id and a DC bit
void constructSuperstrip(addr_type id, bit_type bit, TTSuperstrip& superstrip);

// Construct a TTHit from the global coordinates, charge and pt
void constructHit(float x, float y, float z, float xerr, float yerr, float zerr, int charge, float pt, addr_type ssId, bit_type ssBit, TTHit& hit);


// _____________________________________________________________________________
// Output streams
std::ostream& operator<<(std::ostream& o, const TTSuperstrip& ss);

std::ostream& operator<<(std::ostream& o, const TTHit& hit);

}  // namespace slhcl1tt

#endif
