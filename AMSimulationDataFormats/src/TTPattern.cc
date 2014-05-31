#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTPattern.h"
#include <iostream>


namespace slhcl1tt {

void TTPattern::merge(const TTPattern& rhs) {
    for (unsigned i=0; i<superstrips_.size(); ++i) {
        bit_type ssBit = getSuperstripBit(i);
        ssBit |= rhs.getSuperstripBit(i);
        setSuperstripBit(i, ssBit);
    }
    ++frequency_;
}


std::ostream& operator<<(std::ostream& o, const pattern_type& pattId) {
    for (pattern_type::const_iterator it=pattId.begin(); it!=pattId.end(); ++it) {
        o << *it << " ";
    }
    return o;
}

std::ostream& operator<<(std::ostream& o, const TTPattern& patt) {
    o << "id: " << patt.id() << " frequency: " << patt.frequency() << " ";
    return o;
}

std::ostream& operator<<(std::ostream& o, const TTRoad& road) {
    o << "pattId: " << road.patternId() << " hits: ";
    std::vector<TTHit> hits = road.getHits();
    for (std::vector<TTHit>::const_iterator it=hits.begin(); it!=hits.end(); ++it) {
        o << *it << " ";
    }
    o << " ";
    return o;
}

}  // namespace slhcl1tt
