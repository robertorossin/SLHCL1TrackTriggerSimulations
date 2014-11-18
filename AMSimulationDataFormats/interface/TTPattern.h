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
};


// TTRoad contains info of a pattern that has fired. It consists of the index
// of the pattern in bank, its number of superstrips, and the indices
// of the stubs in those superstrips. The stub indices are serialized,
// each accompanied by a superstrip id. One or more stubs can be in any
// superstrip.
// Note that nSuperstrips is NOT the size of superstripIds vector. nSuperstrips
// is the number of superstrips in the pattern. The size of superstripIds
// vector is the number of stubs in all superstrips in the road.
// TTRoad is not a POD type due to the use of std::vector.
class TTRoad {
  public:
    // Constructors
    TTRoad()
    : bankIndex_(0), triggerTowerId_(0), nSuperstrips_(0),
      superstripIds_(), stubIndices_() {}

    TTRoad(id_type bankIndex, id_type triggerTowerId, count_type nSuperstrips,
           const std::vector<id_type>& superstripIds, const std::vector<unsigned>& stubIndices)
    : bankIndex_(bankIndex), triggerTowerId_(triggerTowerId), nSuperstrips_(nSuperstrips),
      superstripIds_(superstripIds), stubIndices_(stubIndices) {}

    TTRoad(const TTRoad& rhs)
    : bankIndex_(rhs.bankIndex_), triggerTowerId_(rhs.triggerTowerId_), nSuperstrips_(rhs.nSuperstrips_),
      superstripIds_(rhs.superstripIds_), stubIndices_(rhs.stubIndices_) {}

    // Destructor
    ~TTRoad() {}

    // Setters
    // none

    // Getters
    id_type bankIndex()                     const { return bankIndex_; }

    id_type triggerTowerId()                const { return triggerTowerId_; }

    count_type nSuperstrips()               const { return nSuperstrips_; }

    std::vector<id_type> getSuperstripIds() const { return superstripIds_; }
    id_type getSuperstripId(int l)          const { return superstripIds_.at(l); }

    std::vector<unsigned> getStubIndices()  const { return stubIndices_; }
    unsigned getStubIndex(int l)            const { return stubIndices_.at(l); }


  private:
    id_type bankIndex_;
    id_type triggerTowerId_;
    count_type nSuperstrips_;
    std::vector<id_type>  superstripIds_;
    std::vector<unsigned> stubIndices_;
};


// _____________________________________________________________________________
// Output streams
std::ostream& operator<<(std::ostream& o, const pattern_type& patt);

std::ostream& operator<<(std::ostream& o, const TTPattern& patt);

std::ostream& operator<<(std::ostream& o, const TTRoad& road);

}  // namespace slhcl1tt

#endif
