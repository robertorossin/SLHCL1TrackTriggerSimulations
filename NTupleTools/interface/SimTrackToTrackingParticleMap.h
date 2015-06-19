#ifndef NTupleTools_SimTrackToTrackingParticleMap_h_
#define NTupleTools_SimTrackToTrackingParticleMap_h_

#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"
#include <map>


class SimTrackToTrackingParticleMap {
  public:
    typedef std::pair<unsigned, EncodedEventId> MixedSimTrackId;

    void setup(const edm::Handle<TrackingParticleCollection>& tphandle);

    int get(unsigned trackId, EncodedEventId eventId);

  private:
    std::map<MixedSimTrackId, TrackingParticleRef> mapping;
};

#endif

