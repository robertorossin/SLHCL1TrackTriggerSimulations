#ifndef AMSimulation_PatternBankOption_h_
#define AMSimulation_PatternBankOption_h_

#include <vector>

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
    std::vector<int> sectors;
};

}  // namespace slhcl1tt

#endif
