#ifndef AMSimulation_SuperstripHasher_h_
#define AMSimulation_SuperstripHasher_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/Helper.h"

#include <vector>
#include <iosfwd>

namespace slhcl1tt {

class SuperstripHasher {
  public:
    // Constructors
    SuperstripHasher(unsigned subLadderNBits, unsigned subModuleNBits)
    : subLadderNBits_(subLadderNBits), subModuleNBits_(subModuleNBits) {
        init();
    }

    // Destructor
    ~SuperstripHasher() {}

    // Operators
    // Return the hash address given the superstrip address
    // This hash function is originally implemented by Ivan Furic
    key_type hash(addr_type superstripId) const;
    key_type hashModule(id_type moduleId) const;

    // Debug
    void print();

  private:
    void init();

  private:
    // Member data
    const unsigned subLadderNBits_;
    const unsigned subModuleNBits_;

    std::vector<id_type> barrel_z_divisions_;
    std::vector<id_type> barrel_phi_divisions_;
    std::vector<id_type> barrel_layer_offsets_;
    std::vector<id_type> endcap_ring_divisions_;
    std::vector<id_type> endcap_ring_offsets_;
    id_type fakeSuperstripHash_;  // = max hash_
};

}  // namespace slhcl1tt

#endif
