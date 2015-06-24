#ifndef NTupleTools_SimTrackToTrackingParticleMap_h_
#define NTupleTools_SimTrackToTrackingParticleMap_h_

#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"
#include <map>


class SimTrackToTrackingParticleMap {
  public:
    typedef std::pair<unsigned, EncodedEventId> MixedSimTrackId;

    SimTrackToTrackingParticleMap() {}
    ~SimTrackToTrackingParticleMap() {}

    void setup(const edm::Handle<TrackingParticleCollection>& tphandle);

    unsigned size() const { return mapping.size(); }

    int get(const unsigned trackId, const EncodedEventId eventId);

  private:
    std::map<MixedSimTrackId, TrackingParticleRef> mapping;
};

#endif

