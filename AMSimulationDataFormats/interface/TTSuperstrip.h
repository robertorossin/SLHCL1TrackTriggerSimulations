#ifndef AMSimulationDataFormats_TTSuperstrip_h_
#define AMSimulationDataFormats_TTSuperstrip_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/Helper.h"

#include <iosfwd>


namespace slhcl1tt {

// TTSuperstrip is designed to be the building block of a TTPattern.
// Nominally, it corresponds to a physical region in the detector.
// A POD type is used to reduce memory consumption.
struct TTSuperstrip {
    id_type  id;
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
    id_type  superstripId;
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
// Output streams
std::ostream& operator<<(std::ostream& o, const TTSuperstrip& ss);

std::ostream& operator<<(std::ostream& o, const TTHit& hit);

}  // namespace slhcl1tt

#endif
