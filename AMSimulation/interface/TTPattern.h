#ifndef AMSimulation_TTPattern_h_
#define AMSimulation_TTPattern_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TTSuperstrip.h"

#include <cassert>
#include <algorithm>
#include <iosfwd>
#include <vector>


// FIXME: eliminate the use of 64-bit integers
namespace slhcl1tt {

class TTPattern {
  public:
    // Constructors
    TTPattern()
    : hash_(0), frequency_(1), superstrips_(), patternId_() {}

    TTPattern(unsigned hash, const std::vector<TTSuperstrip>& superstrips)
    : hash_(hash), frequency_(1), superstrips_(superstrips),
      patternId_() {

        concatenate();
    }

    TTPattern(const TTPattern& rhs)
    : hash_(rhs.hash_), frequency_(rhs.frequency_), superstrips_(rhs.superstrips_),
      patternId_() {

        concatenate();
    }

    // Destructor
    ~TTPattern() {}

    // Setters
    // One can change the bit, but not the address since the address is
    // part of the pattern id
    void setSuperstripBit(int l, bit_type bit)  { superstrips_.at(l).setBit(bit); }

    // Getters
    std::vector<TTSuperstrip> getSuperstrips()    const { return superstrips_; }
    TTSuperstrip getSuperstrip(int l)             const { return superstrips_.at(l); }
    addr_type getSuperstripId(int l)              const { return superstrips_.at(l).id(); }
    bit_type getSuperstripBit(int l)              const { return superstrips_.at(l).bit(); }

    unsigned hash()                               const { return hash_; }
    unsigned frequency()                          const { return frequency_; }
    pattern_type id()                             const {
        pattern_type emptyPatternId;
        assert(patternId_ != emptyPatternId);
        return patternId_;
    }
    //unsigned nFakeSuperstrips()                   const {
    //    pattern_type emptyPatternId;
    //    assert(patternId_ != emptyPatternId);
    //    return std::count(patternId_.begin(), patternId_.end(), encodeFakeSuperstripId());
    //}

    // Functions
    void merge(const TTPattern& rhs);

  private:
    void concatenate();

  private:
    unsigned hash_;  // immutable
    unsigned frequency_;
    std::vector<TTSuperstrip> superstrips_;
    pattern_type patternId_;
};


// TTRoad is just a data holder. In actuality, a road should be a word, 
// and its relevant data is retrieved using a lookup table. In software, 
// we simply skip this lookup.
class TTRoad {
  public:
    // Constructors
    TTRoad()
    : hash_(0), patternId_(), hits_() {}

    TTRoad(unsigned hash, const pattern_type& patternId, const std::vector<TTHit>& hits)
    : hash_(hash), patternId_(patternId), hits_(hits) {}

    TTRoad(const TTRoad& rhs)
    : hash_(rhs.hash_), patternId_(rhs.patternId_), hits_(rhs.hits_) {}

    // Destructor
    ~TTRoad() {}

    // Setters
    // None allowed

    // Getters
    std::vector<TTHit> getHits()    const { return hits_; }
    TTHit getHit(int l)             const { return hits_.at(l); }

    unsigned hash()                 const { return hash_; }
    pattern_type patternId()        const { return patternId_; }

  private:
    unsigned hash_;  // immutable
    pattern_type patternId_;
    std::vector<TTHit> hits_;
};


// _____________________________________________________________________________
// To sort a collection of patterns using frequency
inline bool sortByFrequency(const TTPattern& lhs, const TTPattern& rhs) {
    return lhs.frequency() > rhs.frequency();
}

// _____________________________________________________________________________
// Output streams
std::ostream& operator<<(std::ostream& o, const pattern_type& pattId);

std::ostream& operator<<(std::ostream& o, const TTPattern& patt);

std::ostream& operator<<(std::ostream& o, const TTRoad& road);

}  // namespace slhcl1tt

#endif
