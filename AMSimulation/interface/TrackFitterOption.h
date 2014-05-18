#ifndef AMSimulation_TrackFitterOption_h_
#define AMSimulation_TrackFitterOption_h_

#include <vector>
#include <algorithm>
#include <iostream>
#include <iterator>

namespace slhcl1tt {

struct TrackFitterOption {
    int nLayers;  // FIXME: remove this?
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

inline std::ostream& operator<<(std::ostream& o, const TrackFitterOption& po) {
    o << "nLayers: " << po.nLayers << "  pqType: " << po.pqType << "  pbins: " << po.pbins << "  qbins: " << po.qbins << "  pmin: " << po.pmin << "  qmin: " << po.qmin << "  pmax: " << po.pmax << "  qmax: " << po.qmax << std::endl;
    o << "sigma: " << po.sigma << "  minWeight: " << po.minWeight << std::endl;
    return o;
}

}  // namespace slhcl1tt

#endif
