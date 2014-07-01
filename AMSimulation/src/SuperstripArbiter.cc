#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/SuperstripArbiter.h"

#include <iostream>
#include <iterator>


namespace slhcl1tt {

void SuperstripArbiter::print() {
    std::cout << "subLadderSize: ";
    std::copy(subLadderSize_.begin(), subLadderSize_.end(), std::ostream_iterator<id_type>(std::cout, " "));
    std::cout << std::endl;
    std::cout << "subModuleSize: ";
    std::copy(subModuleSize_.begin(), subModuleSize_.end(), std::ostream_iterator<id_type>(std::cout, " "));
    std::cout << std::endl;
}

}
