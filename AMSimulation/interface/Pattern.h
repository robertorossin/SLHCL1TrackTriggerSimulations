#ifndef AMSimulation_Pattern_h_
#define AMSimulation_Pattern_h_

#include <cassert>
#include <cstdint>
#include <vector>
#include <array>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265359
#endif

namespace slhcl1tt {

class Pattern {
  public:
    typedef std::vector<uint8_t>    vuint8_t;
    typedef std::vector<uint32_t>   vuint32_t;
    typedef std::vector<uint64_t>   vuint64_t;
    typedef std::array<uint64_t, 8> pattern8_t;

    // Constructor and destructor
    Pattern(int n, uint32_t h)
    : nLayers_(n), hash_(h), frequency_(1),
      patternId_(), emptyPatternId_() {

        hitAddresses_.resize(n,0);
        hitIds_.resize(n,0);
        hitBits_.resize(n,0);
    }
    ~Pattern() {}

    // Copy constructor
    Pattern(const Pattern& rhs)
    : nLayers_(rhs.nLayers_), hash_(rhs.hash_), frequency_(rhs.frequency_),
      patternId_(rhs.patternId_), emptyPatternId_(rhs.emptyPatternId_) {

        hitAddresses_ = rhs.hitAddresses_;
        hitIds_ = rhs.hitIds_;
        hitBits_ = rhs.hitBits_;
    }


    // Static functions to be called without the class instance
    // Retrieve layer, ladder, module from a moduleId
    static uint32_t decodeLayer(uint32_t moduleId) {
        return moduleId / 10000;
    }

    static uint32_t decodeLadder(uint32_t moduleId) {
        return (moduleId / 100) % 100;
    }

    static uint32_t decodeModule(uint32_t moduleId) {
        return moduleId % 100;
    }

    // Encode moduleId, hit into a hitId
    static uint64_t encodeHitId(uint32_t moduleId, uint32_t col, uint32_t row) {
        uint32_t lay = decodeLayer(moduleId);
        uint32_t lad = decodeLadder(moduleId);
        uint32_t mod = decodeModule(moduleId);
        uint64_t id = 0;
        id |= uint64_t(lay & iLayerMask_    ) << iLayerStartBit_     |
              uint64_t(lad & iLadderMask_   ) << iLadderStartBit_    |
              uint64_t(mod & iModuleMask_   ) << iModuleStartBit_    |
              uint64_t(col & iSubLadderMask_) << iSubLadderStartBit_ |
              uint64_t(row & iSubModuleMask_) << iSubModuleStartBit_ ;
        return id;
    }

    // Retrieve layer, ladder, module, subladder, submodule from a hitId
    static uint32_t decodeHitLayer(uint64_t hitId) {
        return uint64_t(hitId >> iLayerStartBit_) & iLayerMask_;
    }

    static uint32_t decodeHitLadder(uint64_t hitId) {
        return uint64_t(hitId >> iLadderStartBit_) & iLadderMask_;
    }

    static uint32_t decodeHitModule(uint64_t hitId) {
        return uint64_t(hitId >> iModuleStartBit_) & iModuleMask_;
    }

    static uint32_t decodeHitSubLadder(uint64_t hitId) {
        return uint64_t(hitId >> iSubLadderStartBit_) & iSubLadderMask_;
    }

    static uint32_t decodeHitSubModule(uint64_t hitId) {
        return uint64_t(hitId >> iSubModuleStartBit_) & iSubModuleMask_;
    }


    // Setters
    void setHit(int l, uint64_t hit, const bool* addr) {
        assert(l < nLayers_);
        hitIds_.at(l) = hit;
        hitAddresses_.at(l) = addr;
    }

    void setHit(int l, uint32_t moduleId, uint32_t col, uint32_t row, const bool* addr) {
        setHit(l, encodeHitId(moduleId, col, row), addr);
    }

    void setBit(int l, uint8_t bit) {
        assert(l < nLayers_);
        hitBits_.at(l) = bit;
    }

    void touch() { ++frequency_; }


    // Getters
    const bool* getHitAddress(int l)           const { return hitAddresses_.at(l); }
    uint64_t getHitId(int l)                   const { return hitIds_.at(l); }
    uint8_t getHitBit(int l)                   const { return hitBits_.at(l); }

    std::vector<const bool*> getHitAddresses() const { return hitAddresses_; }
    vuint64_t getHitIds()                      const { return hitIds_; }
    vuint8_t  getHitBits()                     const { return hitBits_; }

    pattern8_t getPatternId() {
        if (patternId_ == emptyPatternId_) {
            vuint64_t hitIds_tmp = hitIds_;
            hitIds_tmp.resize(8,0);  // pad with zeroes
            std::copy(hitIds_tmp.begin(), hitIds_tmp.end(), patternId_.begin());
        }
        return patternId_;
    }

    uint32_t hash()       const { return hash_; }
    uint32_t frequency()  const { return frequency_; }


    // Constants
    // Using full definitions now -- need to be reduced eventually
    static const uint32_t iSubModuleStartBit_ = 0;
    static const uint32_t iSubLadderStartBit_ = 10;
    static const uint32_t iModuleStartBit_    = 15;
    static const uint32_t iLadderStartBit_    = 22;
    static const uint32_t iLayerStartBit_     = 29;
    // next start bit should be 34

    static const uint32_t iSubModuleMask_     = 0x3FF; // 0-1023
    static const uint32_t iSubLadderMask_     = 0x1F;  // 0-31
    static const uint32_t iModuleMask_        = 0x7F;  // 0-127
    static const uint32_t iLadderMask_        = 0x7F;  // 0-127
    static const uint32_t iLayerMask_         = 0x1F;  // 0-15

    // Maximum number of active layers due to hardware design
    static const uint32_t maxNLayers_         = 8;

    // This is for a different purpose than the above. To be fixed!
    static const uint32_t nModuleIds_         = 230000;
    static const uint32_t nSubModuleIds_      = 1024;
    static const uint32_t nSubLadderIds_      = 32;

  private:
    int nLayers_;
    uint32_t hash_;
    uint32_t frequency_;
    pattern8_t patternId_, emptyPatternId_;  // only for faster access
    std::vector<const bool*> hitAddresses_;
    vuint64_t hitIds_;
    vuint8_t hitBits_;  // for DC bits. 0: L, 1: R, 2: LR, 3: UNUSED
};

// To sort a collection of patterns using frequency
inline bool sortByFrequency(const Pattern& lhs, const Pattern& rhs) {
    return lhs.frequency() > rhs.frequency();
}

}  // namespace slhcl1tt

#endif
