#ifndef AMSimulationDataFormats_TTRoadComb_h_
#define AMSimulationDataFormats_TTRoadComb_h_

#include <vector>
#include <iosfwd>


namespace slhcl1tt {

struct TTRoadComb {
    unsigned roadRef;
    unsigned patternRef;
    unsigned tower;
    unsigned nstubs;
    std::vector<unsigned> stubRefs;

    std::vector<float> stubs_r;
    std::vector<float> stubs_phi;
    std::vector<float> stubs_z;
    std::vector<bool>  stubs_bool;

    unsigned hitbits() {
        unsigned bits = 0;

        for (unsigned i=0; i<stubs_bool.size(); ++i) {
            bits |= (stubs_bool.at(i) << i);
        }

        switch (bits) {
        case 0b111111:  return 0;
        case 0b111110:  return 1;
        case 0b111101:  return 2;
        case 0b111011:  return 3;
        case 0b110111:  return 4;
        case 0b101111:  return 5;
        case 0b011111:  return 6;
        default      :  return 7;
        }
    }
};


// _____________________________________________________________________________
// Output streams
std::ostream& operator<<(std::ostream& o, const TTRoadComb& road);

}  // namespace slhcl1tt

#endif
