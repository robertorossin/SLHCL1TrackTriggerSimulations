#ifndef AMSimulation_PatternBankOption_h_
#define AMSimulation_PatternBankOption_h_

#include <vector>
#include <algorithm>
#include <iostream>
#include <iterator>

namespace slhcl1tt {

struct PatternBankOption {
    float minPt;
    float maxPt;
    float minEta;  // not absolute eta
    float maxEta;  // not absolute eta
    float minPhi;  // from -pi to pi
    float maxPhi;  // from -pi to pi
    int nLayers;
    int nSubLadders;  // 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024
    int nSubModules;  // 1, 2, 4, 8, 16, 32
    int nMisses;
    int nFakeHits;
    int nDCBits;
    std::vector<int> activeLayers;
    std::vector<int> inactiveLayers;
    std::vector<int> triggerTowers;
};

inline std::ostream& operator<<(std::ostream& o, const PatternBankOption& po) {
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


}  // namespace slhcl1tt

#endif

