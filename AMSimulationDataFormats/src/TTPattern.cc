#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/TTPattern.h"
#include <iostream>


namespace slhcl1tt {

void constructPattern(const std::vector<TTSuperstrip>& superstrips, TTPattern& pattern) {
    // Set pattern id and DC bit
    for (unsigned i=0; i<pattern.id.size(); ++i) {
        if (i<superstrips.size()) {
            pattern.id.at(i) = superstrips.at(i).id;
            pattern.bit.at(i) = superstrips.at(i).bit;
        } else {
            pattern.id.at(i) = 0;
            pattern.bit.at(i) = 1;
        }
    }

    // Set frequency
    pattern.frequency = 1;
}

void unionPattern(const TTPattern& inputPattern, TTPattern& outputPattern) {
    for (unsigned i=0; i<outputPattern.id.size(); ++i) {
        outputPattern.bit.at(i) |= inputPattern.bit.at(i);
    }
    ++ outputPattern.frequency;
}

bool isEqualPatternId(const TTPattern& lhs, const TTPattern& rhs) {
    return lhs.id == rhs.id;
}

bool isLesserPatternId(const TTPattern& lhs, const TTPattern& rhs) {
    return lhs.id < rhs.id;
}

bool isHigherPatternFrequency(const TTPattern& lhs, const TTPattern& rhs) {
    return lhs.frequency > rhs.frequency;
}

// _____________________________________________________________________________
std::ostream& operator<<(std::ostream& o, const pattern_type& pattId) {
    for (pattern_type::const_iterator it=pattId.begin(); it!=pattId.end(); ++it) {
        o << *it << " ";
    }
    return o;
}

std::ostream& operator<<(std::ostream& o, const TTPattern& patt) {
    o << "id: " << patt.id << " frequency: " << (unsigned) patt.frequency << " ";
    return o;
}

std::ostream& operator<<(std::ostream& o, const std::vector<TTHit>& hits) {
    for (std::vector<TTHit>::const_iterator it=hits.begin(); it!=hits.end(); ++it) {
        o << *it << " ";
    }
    return o;
}

std::ostream& operator<<(std::ostream& o, const TTRoad& road) {
    o << "pattId: " << road.patternId() << " hits: " << road.getHits() << " ";
    return o;
}

}  // namespace slhcl1tt
