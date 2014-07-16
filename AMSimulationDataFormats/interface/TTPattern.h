#ifndef AMSimulationDataFormats_TTPattern_h_
#define AMSimulationDataFormats_TTPattern_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTSuperstrip.h"

#include <iosfwd>
#include <vector>


namespace slhcl1tt {

// TTPattern is a collection of 8 TTSuperstrips
// A POD type is used to reduce memory consumption.
struct TTPattern {
    count_type frequency;
    pattern_type id;
    pattern_bit_type bit;

    // Functions
    std::vector<TTSuperstrip> getSuperstrips() const {
        std::vector<TTSuperstrip> superstrips;
        for (unsigned i=0; i<id.size(); ++i) {
            TTSuperstrip ss;
            constructSuperstrip(id.at(i), bit.at(i), ss);
            superstrips.push_back(ss);
        }
        return superstrips;
    }

    TTSuperstrip getSuperstrip(int l)          const {
        TTSuperstrip ss;
        constructSuperstrip(id.at(l), bit.at(l), ss);
        return ss;
    }
};


// TTRoad is a collection of TTHit. In actuality, a road should be a word,
// and its relevant data is retrieved using a lookup table. In software,
// we simply skip this lookup.
// TTRoad is not a POD data type due to the use of std::vector<TTHit>.
class TTRoad {
  public:
    // Constructors
    TTRoad()
    : patternId_(), hits_() {}

    TTRoad(const pattern_type& patternId, const std::vector<TTHit>& hits)
    : patternId_(patternId), hits_(hits) {}

    TTRoad(const TTRoad& rhs)
    : patternId_(rhs.patternId_), hits_(rhs.hits_) {}

    // Destructor
    ~TTRoad() {}

    // Setters
    // None allowed

    // Getters
    std::vector<TTHit> getHits()    const { return hits_; }
    TTHit getHit(int l)             const { return hits_.at(l); }

    pattern_type patternId()        const { return patternId_; }

  private:
    pattern_type patternId_;
    std::vector<TTHit> hits_;
};


// _____________________________________________________________________________
// Functions

// Construct a TTPattern from a vector of TTSuperstrip
void constructPattern(const std::vector<TTSuperstrip>& superstrips, TTPattern& pattern);

// Merge an input TTPattern into an existing TTPattern
void unionPattern(const TTPattern& inputPattern, TTPattern& outputPattern);

// Compare two patterns using superstrip id (for uniquifying)
bool isEqualPatternId(const TTPattern& lhs, const TTPattern& rhs);

// Compare two patterns using superstrip id (for sorting)
bool isLesserPatternId(const TTPattern& lhs, const TTPattern& rhs);

// Compare two patterns using frequency (for sorting)
bool isHigherPatternFrequency(const TTPattern& lhs, const TTPattern& rhs);

// _____________________________________________________________________________
// Output streams
std::ostream& operator<<(std::ostream& o, const pattern_type& pattId);

std::ostream& operator<<(std::ostream& o, const TTPattern& patt);

std::ostream& operator<<(std::ostream& o, const std::vector<TTHit>& hits);

std::ostream& operator<<(std::ostream& o, const TTRoad& road);

}  // namespace slhcl1tt

#endif
