#ifndef NTupleTools_MixedSimHitMap_h_
#define NTupleTools_MixedSimHitMap_h_

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include <map>


class TrackerGeometry;

class MixedSimHitMap {
  public:
    MixedSimHitMap() {}
    ~MixedSimHitMap() {}

    void setup(const std::vector<edm::InputTag>& simHitCollections, const edm::Event& iEvent);

    unsigned size() const { return mapping.size(); }

    unsigned get(const unsigned subdet, const unsigned tofBin, const unsigned CFposition);

  private:
    std::map<unsigned, unsigned> mapping;
};

#endif
