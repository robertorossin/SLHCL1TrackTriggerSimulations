#ifndef NTupleTools_TTClusterMap_h_
#define NTupleTools_TTClusterMap_h_

#include "DataFormats/L1TrackTrigger/interface/TTCluster.h"
#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"
#include <map>


class TTClusterMap {
  public:
    typedef edmNew::DetSetVector<TTCluster<Ref_PixelDigi_> > dsv_clus;
    typedef edmNew::DetSet<TTCluster<Ref_PixelDigi_> >       ds_clus;
    typedef edm::Ref<dsv_clus, TTCluster<Ref_PixelDigi_> >   ref_clus;

    TTClusterMap() {}
    ~TTClusterMap() {}

    void setup(const edm::Handle<dsv_clus>& handle);

    unsigned get(const ref_clus aref);

  private:
    //std::map<ref_clus, unsigned> mapping;
    std::map<std::pair<DetId, std::vector<Ref_PixelDigi_> >, unsigned> mapping;
};

#endif
