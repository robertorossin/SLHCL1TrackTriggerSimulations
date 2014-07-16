#ifndef AMSimulation_PatternBankOption_h_
#define AMSimulation_PatternBankOption_h_

#include <vector>
#include <iosfwd>

namespace slhcl1tt {

struct PatternBankOption {
    float minPt;
    float maxPt;
    float minEta;  // not absolute eta
    float maxEta;  // not absolute eta
    float minPhi;  // from -pi to pi
    float maxPhi;  // from -pi to pi
    int subLadderSize;  // 1, 2, 4, 8, 16, 32
    int subModuleSize;  // 1, 2, 4, 8, 16, 32, 64, 128, 256, 512
    int nLayers;
    int nMisses;
    int nFakeSuperstrips;
    int nDCBits;
    bool requireTriggerTower;
    std::vector<unsigned> subLadderVarSize;
    std::vector<unsigned> subModuleVarSize;
    std::vector<unsigned> subLadderECVarSize;
    std::vector<unsigned> subModuleECVarSize;
    std::vector<unsigned> triggerTowers;
};

std::ostream& operator<<(std::ostream& o, const PatternBankOption& po);

}  // namespace slhcl1tt

#endif

