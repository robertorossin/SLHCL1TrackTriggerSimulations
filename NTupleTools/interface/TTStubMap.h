#ifndef NTupleTools_TTStubMap_h_
#define NTupleTools_TTStubMap_h_

#include "DataFormats/L1TrackTrigger/interface/TTStub.h"
#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"
#include <map>


class TTStubMap {
  public:
    typedef edmNew::DetSetVector<TTStub<Ref_PixelDigi_> > dsv_stub;
    typedef edmNew::DetSet<TTStub<Ref_PixelDigi_> >       ds_stub;
    typedef edm::Ref<dsv_stub, TTStub<Ref_PixelDigi_> >   ref_stub;

    TTStubMap() {}
    ~TTStubMap() {}

    void setup(const edm::Handle<dsv_stub>& handle);

    unsigned get(const ref_stub aref);

  private:
    std::map<ref_stub, unsigned> mapping;
};

#endif
