#ifndef NTupleTools_PixelDigiMap_h_
#define NTupleTools_PixelDigiMap_h_

#include "DataFormats/SiPixelDigi/interface/PixelDigi.h"
#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"
#include <map>


class TrackerGeometry;

class PixelDigiMap {
  public:
    typedef edm::DetSetVector<PixelDigi> dsv_digi;
    typedef edm::DetSet<PixelDigi>       ds_digi;

    PixelDigiMap() {}
    ~PixelDigiMap() {}

    void setup(const edm::Handle<dsv_digi>& handle, const TrackerGeometry * theGeometry);

    unsigned size() const { return mapping.size(); }

    //unsigned get(const Ref_PixelDigi_ aref);
    unsigned get(const unsigned channel);

  private:
    //std::map<Ref_PixelDigi_, unsigned> mapping;
    std::map<unsigned, unsigned> mapping;
};

#endif
