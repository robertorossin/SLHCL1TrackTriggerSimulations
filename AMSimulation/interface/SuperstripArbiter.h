#ifndef AMSimulation_SuperstripArbiter_h_
#define AMSimulation_SuperstripArbiter_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/Helper.h"

#include <vector>
#include <iosfwd>

namespace slhcl1tt {

class SuperstripArbiter {
  public:
    // Constructors
    // subLadderSize and subModuleSize are expressed as if each module has 1024 subLadders and 32 subModules
    SuperstripArbiter(unsigned n, id_type subLadderSize, id_type subModuleSize)
    : n_(n), subLadderSize_(n,subLadderSize), subModuleSize_(n,subModuleSize) {
        init();
    }

    SuperstripArbiter(unsigned n, const std::vector<id_type>& subLadderVarSize, const std::vector<id_type>& subModuleVarSize)
    : n_(n), subLadderSize_(subLadderVarSize), subModuleSize_(subModuleVarSize) {
        init();
    }

    SuperstripArbiter(unsigned n, const std::vector<int>& subLadderVarSize, const std::vector<int>& subModuleVarSize)
    : n_(n), subLadderSize_(n,0), subModuleSize_(n,0) {
        for (unsigned i=0; i<n_; ++i) {
            subLadderSize_.at(i) = (id_type) subLadderVarSize.at(i);
            subModuleSize_.at(i) = (id_type) subModuleVarSize.at(i);
        }
        init();
    }

    // Destructor
    ~SuperstripArbiter() {}

    // Operators
    id_type subladder(id_type moduleId, id_type col, bool isHalfStrip=true) const {
        if (isHalfStrip)
            col /= 2;
        id_type alayer = decodeLayerArbiter(moduleId);
        if (isPSModule(moduleId))
            col /= subLadderSize_.at(alayer);
        else
            col = col * 16 / subLadderSize_.at(alayer);
        return col;
    }

    id_type submodule(id_type moduleId, id_type row, bool isHalfStrip=true) const {
        if (isHalfStrip)
            row /= 2;
        id_type alayer = decodeLayerArbiter(moduleId);
        row /= subModuleSize_.at(alayer);
        return row;
    }

    // Functions
    id_type minSubLadderSize() const {
        id_type min = 9999;
        for (unsigned i=0; i<n_; ++i) {
            if (min > subLadderSize_.at(i))
                min = subLadderSize_.at(i);
        }
        return min;
    }

    id_type minSubModuleSize() const {
        id_type min = 9999;
        for (unsigned i=0; i<n_; ++i) {
            if (min > subModuleSize_.at(i))
                min = subModuleSize_.at(i);
        }
        return min;
    }

    // Debug
    void print();

  private:
    void init() {
        if (subLadderSize_.size() != n_)
            throw "Incorrect subLadderVarSize is given";
        if (subModuleSize_.size() != n_)
            throw "Incorrect subModuleVarSize is given";

        // Only division by a power of two (2, 4, 8, 16, ...) is safe, as it corresponds to bit shifting
        for (unsigned i=0; i<n_; ++i) {
            subLadderSize_.at(i) = 1 << msb(subLadderSize_.at(i));
            subModuleSize_.at(i) = 1 << msb(subModuleSize_.at(i));
        }
    }

  private:
    // Member data
    unsigned n_;
    std::vector<id_type> subLadderSize_;
    std::vector<id_type> subModuleSize_;
};

}  // namespace slhcl1tt

#endif
