#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternBankOption.h"

#include <iostream>
#include <algorithm>
#include <iterator>


namespace slhcl1tt {

std::ostream& operator<<(std::ostream& o, const PatternBankOption& po) {
    o << "minPt: " << po.minPt << "  maxPt: " << po.maxPt << "  minEta: " << po.minEta << "  maxEta: " << po.maxEta << "  minPhi: " << po.minPhi << "  maxPhi: " << po.maxPhi << std::endl;
    o << "nLayers: " << po.nLayers << "  nSubLadders: " << po.nSubLadders << "  nSubModules: " << po.nSubModules << "  nMisses: " << po.nMisses << "  nFakeHits: " << po.nFakeHits << "  nDCBits: " << po.nDCBits << std::endl;
    o << "activeLayers: ";
    std::copy(po.activeLayers.begin(), po.activeLayers.end(), std::ostream_iterator<int>(o, " "));
    o << std::endl;
    o << "inactiveLayers: ";
    std::copy(po.inactiveLayers.begin(), po.inactiveLayers.end(), std::ostream_iterator<int>(o, " "));
    o << std::endl;
    o << "triggerTowers: ";
    std::copy(po.triggerTowers.begin(), po.triggerTowers.end(), std::ostream_iterator<int>(o, " "));
    o << std::endl;
    return o;
}

}
