#ifndef AMSimulation_TrackFitterOption_h_
#define AMSimulation_TrackFitterOption_h_

#include <vector>
#include <iosfwd>

namespace slhcl1tt {

struct TrackFitterOption {
    unsigned nLayers;  // FIXME: remove this?
    int pqType;
    int pbins;
    int qbins;
    float pmin;
    float qmin;
    float pmax;
    float qmax;
    float sigma;
    float minWeight;
};

std::ostream& operator<<(std::ostream& o, const TrackFitterOption& po);

}  // namespace slhcl1tt

#endif
