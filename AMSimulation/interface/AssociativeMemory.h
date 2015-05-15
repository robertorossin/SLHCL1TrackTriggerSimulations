#ifndef AMSimulation_AssociativeMemory_h_
#define AMSimulation_AssociativeMemory_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/Pattern.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/HitBuffer.h"
#include <vector>

namespace slhcl1tt {

class AssociativeMemory {
  public:
    // Constructor
    AssociativeMemory() : frozen_(false) {}

    // Destructor
    ~AssociativeMemory() {}

    // Functions
    // Initialize
    int init(unsigned npatterns);

    // Insert patterns
    void insert(std::vector<superstrip_type>::const_iterator begin, std::vector<superstrip_type>::const_iterator end, const float invPt);
    void insert(const pattern_type& patt, const float invPt);

    void freeze();

    unsigned size() const { return patternBank_.size(); }

    // Perform direct pattern lookup, return a list of patterns that are fired
    std::vector<unsigned> lookup(const HitBuffer& hitBuffer, const unsigned nLayers, const unsigned maxMisses);

    // Retrieve superstripIds and attributes
    void retrieve(const unsigned patternRef, pattern_type& superstripIds, float& invPt);

    // Debug
    void print();

  private:
    // Member data
    std::vector<pattern_type> patternBank_;
    std::vector<float>        patternAttributes_invPt_;
    bool frozen_;
};

}

#endif
