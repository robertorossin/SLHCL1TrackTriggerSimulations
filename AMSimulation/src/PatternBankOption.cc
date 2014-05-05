#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternBankOption.h"

namespace slhcl1tt {

std::ostream& operator<<(std::ostream& o, const PatternBankOption& p) {
    o << "minPt: " << p.minPt << "  maxPt: " << p.maxPt << "  minEta: " << p.minEta << "  maxEta: " << p.maxEta << "  minPhi: " << p.minPhi << "  maxPhi: " << p.maxPhi << std::endl;
    o << "nLayers: " << p.nLayers << "  nSubLadders: " << p.nSubLadders << "  nSubModules: " << p.nSubModules << "  nMisses: " << p.nMisses << "  nFakeHits: " << p.nFakeHits << "  nDCBits: " << p.nDCBits << std::endl;
    o << "activeLayers: ";
    std::copy(p.activeLayers.begin(), p.activeLayers.end(), std::ostream_iterator<int>(o, " "));
    o << std::endl;
    o << "inactiveLayers: ";
    std::copy(p.inactiveLayers.begin(), p.inactiveLayers.end(), std::ostream_iterator<int>(o, " "));
    o << std::endl;
    o << "sectors: ";
    std::copy(p.sectors.begin(), p.sectors.end(), std::ostream_iterator<int>(o, " "));
    o << std::endl;
    return o;
}

}  // namespace slhcl1tt
