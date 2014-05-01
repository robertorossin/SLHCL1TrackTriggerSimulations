#include <vector>
#include "DataFormats/Common/interface/Wrapper.h"

namespace {
  struct dictionary {
    std::vector<std::vector<unsigned> > pippo1;
    edm::Wrapper<std::vector<std::vector<unsigned> > > ppippo1;
    std::vector<std::vector<float> > pippo2;
    edm::Wrapper<std::vector<std::vector<float> > > ppippo2;
  };
}
