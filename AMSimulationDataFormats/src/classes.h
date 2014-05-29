#include <vector>
//#include "DataFormats/Common/interface/Wrapper.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTSuperstrip.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTPattern.h"

namespace {
  struct dictionary {
    //edm::Wrapper<slhcl1tt::TTSuperstrip> pippo1;
    //edm::Wrapper<slhcl1tt::TTHit> pippo2;
    //edm::Wrapper<slhcl1tt::TTPattern> pippo3;
    //edm::Wrapper<slhcl1tt::TTRoad> pippo4;
    std::vector<slhcl1tt::TTSuperstrip> vpippo1;
    std::vector<slhcl1tt::TTHit> vpippo2;
    std::vector<slhcl1tt::TTPattern> vpippo3;
    std::vector<slhcl1tt::TTRoad> vpippo4;
    //edm::Wrapper<std::vector<slhcl1tt::TTSuperstrip> > wvpippo1;
    //edm::Wrapper<std::vector<slhcl1tt::TTHit> > wvpippo2;
    //edm::Wrapper<std::vector<slhcl1tt::TTPattern> > wvpippo3;
    //edm::Wrapper<std::vector<slhcl1tt::TTRoad> > wvpippo4;
  };
}
