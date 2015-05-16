#ifndef AMSimulationDataFormats_TrackingParticle_h_
#define AMSimulationDataFormats_TrackingParticle_h_

#include <vector>
#include <iosfwd>


namespace slhcl1tt {

struct TrackingParticle {
    int   tpId;
    int   pdgId;
    float invPt;
    float phi0;
    float cottheta;
    float z0;
    float d0;
};


// _____________________________________________________________________________
// Output streams
std::ostream& operator<<(std::ostream& o, const TrackingParticle& particle);

}  // namespace slhcl1tt

#endif
