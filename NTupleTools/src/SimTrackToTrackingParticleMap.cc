#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/SimTrackToTrackingParticleMap.h"

void SimTrackToTrackingParticleMap::setup(const edm::Handle<TrackingParticleCollection>& tphandle) {
    if (tphandle.isValid()) {
        for (unsigned itp=0; itp<tphandle->size(); ++itp) {
            TrackingParticleRef tpref(tphandle, itp);
            for (TrackingParticle::g4t_iterator itrk = tpref->g4Track_begin(); itrk != tpref->g4Track_end(); ++itrk) {
                MixedSimTrackId trkId(itrk->trackId(), tpref->eventId());
                mapping.insert(std::make_pair(trkId, tpref));
            }
        }
    }
}

int SimTrackToTrackingParticleMap::get(unsigned trackId, EncodedEventId eventId) {
    MixedSimTrackId trkId(trackId, eventId);
    std::map<MixedSimTrackId, TrackingParticleRef>::const_iterator found = mapping.find(trkId);
    if (found != mapping.end()) {
        return found->second.key();
    }
    return -1;
}

