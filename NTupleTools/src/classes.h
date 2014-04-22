#include <vector>
#include "DataFormats/Common/interface/Wrapper.h"

namespace {
  struct dictionary {
    std::vector<std::vector<unsigned> > pippo1;
    edm::Wrapper<std::vector<std::vector<unsigned> > > ppippo1;
  };
}
