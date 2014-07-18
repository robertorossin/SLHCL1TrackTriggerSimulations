#ifndef AMSimulation_SuperstripStitcher_h_
#define AMSimulation_SuperstripStitcher_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/Helper.h"

#include <vector>
#include <iosfwd>

namespace slhcl1tt {

class SuperstripStitcher {
  public:
    // Constructors
    SuperstripStitcher(int nLayers, int nFakeSuperstrips)
    : nLayers_(nLayers), nFakeSuperstrips_(nFakeSuperstrips) {
        init();
    }

    // Destructor
    ~SuperstripStitcher() {}


    // Operators
    // Return a vector of possible patterns given a vector of superstrips
    // Input superstrips are assumed to be sorted
    std::vector<pattern_type> stitch(const std::vector<addr_type>& superstrips) const;

    // Debug
    void print();

  private:
    // Initialize
    void init();

  private:
    // Member data
    const unsigned nLayers_;
    const unsigned nFakeSuperstrips_;

    // Precomputed values
    std::vector<unsigned> seven_choose_six_;
    std::vector<unsigned> eight_choose_six_;
    std::vector<unsigned> eight_choose_seven_;
};

}  // namespace slhcl1tt

#endif
