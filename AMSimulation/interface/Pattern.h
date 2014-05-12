#ifndef AMSimulation_Pattern_h_
#define AMSimulation_Pattern_h_

#include <cassert>
#include <cstdint>
#include <vector>
#include <array>
#include <algorithm>


namespace slhcl1tt {

class Pattern {
  public:
    typedef std::vector<uint16_t>   vuint16_t;
    typedef std::vector<uint32_t>   vuint32_t;
    typedef std::vector<uint64_t>   vuint64_t;
    typedef std::array<uint64_t,8>  pattern8_t;

    // Constructor and destructor
    Pattern(uint32_t n, uint32_t b, uint32_t h)
    : nLayers_(n), nDCBits_(b), hash_(h), frequency_(1),
      patternId_(), emptyPatternId_() {

        assert(nLayers_ <= 8);  // using 8-element array
        assert(nDCBits_ <= 4);         // using 16-bit integer
        hitAddresses_.resize(nLayers_,0);
        hitIds_.resize(nLayers_,0);
        hitBits_.resize(nLayers_,1);
    }
    ~Pattern() {}

    // Copy constructor
    Pattern(const Pattern& rhs)
    : nLayers_(rhs.nLayers_), nDCBits_(rhs.nDCBits_), hash_(rhs.hash_), frequency_(rhs.frequency_),
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
    static uint64_t encodeHitId(uint32_t lay, uint32_t lad, uint32_t mod, uint32_t col, uint32_t row) {
        uint64_t id = 0;
        id |= uint64_t(lay & iLayerMask_    ) << iLayerStartBit_     |
              uint64_t(lad & iLadderMask_   ) << iLadderStartBit_    |
              uint64_t(mod & iModuleMask_   ) << iModuleStartBit_    |
              uint64_t(col & iSubLadderMask_) << iSubLadderStartBit_ |
              uint64_t(row & iSubModuleMask_) << iSubModuleStartBit_ ;
        return id;
    }

    static uint64_t encodeHitId(uint32_t moduleId, uint32_t col, uint32_t row) {
        uint32_t lay = decodeLayer(moduleId);
        uint32_t lad = decodeLadder(moduleId);
        uint32_t mod = decodeModule(moduleId);
        return encodeHitId(lay, lad, mod, col, row);
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

    static void encodeDCBit(uint32_t nDCBits, uint64_t& hitId, uint16_t& hitBit) {  // NOTE: pass by reference
        if (nDCBits == 0)  return;  // don't do anything

        uint32_t lay = decodeHitLayer(hitId);
        uint32_t lad = decodeHitLadder(hitId);
        uint32_t mod = decodeHitModule(hitId);
        uint32_t col = decodeHitSubLadder(hitId);
        uint32_t row = decodeHitSubModule(hitId);

        // FIXME: only do the submodule, since there's only one DC bit
        //uint32_t col_mod_n = col % (1 << nDCBits);
        uint32_t row_mod_n = row % (1 << nDCBits);
        //col /= (1 << nDCBits);
        row /= (1 << nDCBits);

        hitId = encodeHitId(lay, lad, mod, col, row);
        hitBit = (1 << row_mod_n);
        return;
    }


    // Setters
    void setHitAddress(uint32_t l, const uint16_t* addr) { hitAddresses_.at(l) = addr; }

    void setHitId(uint32_t l, uint64_t hit)              { hitIds_.at(l) = hit; }
    void setHitId(uint32_t l, uint32_t moduleId, uint32_t col, uint32_t row) {
        setHitId(l, encodeHitId(moduleId, col, row));
    }

    void setHitBit(uint32_t l, uint16_t bit)             { hitBits_.at(l) = bit; }

    void touch()                                         { ++frequency_; }

    void resetPatternId()                                { patternId_ = emptyPatternId_; }

    void ready();  // must be called to have correct patternId


    // Getters
    const uint16_t* getHitAddress(uint32_t l)      const { return hitAddresses_.at(l); }
    uint64_t getHitId(uint32_t l)                  const { return hitIds_.at(l); }
    uint16_t getHitBit(uint32_t l)                 const { return hitBits_.at(l); }

    std::vector<const uint16_t*> getHitAddresses() const { return hitAddresses_; }
    vuint64_t getHitIds()                          const { return hitIds_; }
    vuint16_t getHitBits()                         const { return hitBits_; }

    uint32_t hash()                                const { return hash_; }
    uint32_t frequency()                           const { return frequency_; }

    pattern8_t getPatternId()                      const {
        assert(patternId_ != emptyPatternId_);
        return patternId_;
    }


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
    //static const uint32_t nModuleIds_         = 230000;
    //static const uint32_t nSubModuleIds_      = 1024;
    //static const uint32_t nSubLadderIds_      = 32;

  private:
    void concatenate();

    uint32_t nLayers_;  // immutable
    uint32_t nDCBits_;  // immutable
    uint32_t hash_;     // immutable
    uint32_t frequency_;
    pattern8_t patternId_, emptyPatternId_;  // concatenated hitIds
    std::vector<const uint16_t*> hitAddresses_;
    vuint64_t hitIds_;
    vuint16_t hitBits_;  // for DC bits: 0 - 2^16 (=65536 max for nDCBits_=4)
};

// To sort a collection of patterns using frequency
inline bool sortByFrequency(const Pattern& lhs, const Pattern& rhs) {
    return lhs.frequency() > rhs.frequency();
}

}  // namespace slhcl1tt

#endif
