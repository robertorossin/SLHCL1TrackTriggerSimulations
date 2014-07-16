#ifndef AMSimulation_SuperstripArbiter_h_
#define AMSimulation_SuperstripArbiter_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/Helper.h"

#include <vector>
#include <iosfwd>

namespace slhcl1tt {

class SuperstripArbiter {
  public:
    // Constructors
    // subLadderSize, subModuleSize, subLadderECSize, subModuleECSize are
    // expressed as if each module has 32 subLadders and 1024 subModules
    SuperstripArbiter(id_type subLadderSize, id_type subModuleSize)
    : nLayers_(6), nRings_(15), subLadderSize_(nLayers_,subLadderSize), subModuleSize_(nLayers_,subModuleSize),
      subLadderECSize_(nRings_,subLadderSize), subModuleECSize_(nRings_,subModuleSize) {
        init();
    }

    SuperstripArbiter(const std::vector<id_type>& subLadderVarSize, const std::vector<id_type>& subModuleVarSize,
                      const std::vector<id_type>& subLadderECVarSize, const std::vector<id_type>& subModuleECVarSize)
    : nLayers_(6), nRings_(15), subLadderSize_(subLadderVarSize), subModuleSize_(subModuleVarSize),
      subLadderECSize_(subLadderECVarSize), subModuleECSize_(subModuleECVarSize) {
        init();
    }

    // Destructor
    ~SuperstripArbiter() {}

    // Operators
    // Return the superstrip addresses (subladder, submodule) given the
    // strip addresses (col, row)
    id_type subladder(id_type moduleId, id_type col, bool isHalfStrip=true) const;

    id_type submodule(id_type moduleId, id_type row, bool isHalfStrip=true) const;

    // Functions
    id_type minSubLadderSize() const {
        // Only check barrel
        id_type min = 9999;
        for (unsigned i=0; i<nLayers_; ++i) {
            if (min > subLadderSize_.at(i))
                min = subLadderSize_.at(i);
        }
        return min;
    }

    id_type minSubModuleSize() const {
        // Only check barrel
        id_type min = 9999;
        for (unsigned i=0; i<nLayers_; ++i) {
            if (min > subModuleSize_.at(i))
                min = subModuleSize_.at(i);
        }
        return min;
    }

    // Debug
    void print();

  private:
    void init();

  private:
    // Member data
    const unsigned nLayers_;
    const unsigned nRings_;
    std::vector<id_type> subLadderSize_;
    std::vector<id_type> subModuleSize_;
    std::vector<id_type> subLadderECSize_;
    std::vector<id_type> subModuleECSize_;
};

}  // namespace slhcl1tt

#endif
