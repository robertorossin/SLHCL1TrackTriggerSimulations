#ifndef AMSimulation_HitBuffer_h_
#define AMSimulation_HitBuffer_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/Pattern.h"
#include <map>
#include <vector>

namespace slhcl1tt {

class HitBuffer {
  public:
    // Constructor
    HitBuffer() : frozen_(false) {}

    // Destructor
    ~HitBuffer() {}

    // Functions
    // Initialize
    int init(unsigned maxBins);

    void reset();

    void insert(superstrip_type ss, unsigned stubRef);

    void freeze(unsigned maxStubs);

    bool isHit(superstrip_type ss) const { return superstripBools_.at(ss); }

    std::vector<unsigned> getHits(superstrip_type ss) const { return superstripHits_.at(ss); }

    // Debug
    void print();

  private:
    // Member data
    std::map<superstrip_type, std::vector<unsigned> > superstripHits_;   // superstrip --> stubRefs (std::map)
    std::vector<bool>                                 superstripBools_;  // superstrip --> hit or empty (hash table)
    bool frozen_;
};

}

#endif
