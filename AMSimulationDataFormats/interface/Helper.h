#ifndef AMSimulationDataFormats_Helper_h_
#define AMSimulationDataFormats_Helper_h_

#include <map>
//#include <cstdint>
//#include <array>
#include <stdint.h>  // consistent with DataFormats/DetId/interface/DetId.h
#include <tr1/array>

namespace slhcl1tt {

// Typically, short is 16 bits, int is 32 bits, long is 32 bits and long long is 64 bits
// But we need to guarantee on the size of the integer
typedef uint8_t count_type;  // for frequency
typedef uint16_t bit_type;   // for DC bits: 0 - 2^16 (=65536 max for nDCBits_=4)
typedef uint16_t key_type;   // for key used in the pattern matching map
typedef uint32_t id_type;    // generic
typedef uint32_t addr_type;  // now reduced to 32-bit (from 64-bit) and is therefore the same as id_type

typedef std::tr1::array<addr_type,8>  pattern_type;  // maximum number of superstrips in a pattern set to 8 due to hardware design
typedef std::tr1::array<bit_type,8>  pattern_bit_type;  // one DC bit for one superstrip


// The following need to be hidden from dictionary generation
#ifndef __GCCXML__


/// Constants
// Current working assumption is:
// - PS module: 32 cols,  960 rows
// - 2S module:  2 cols, 1016 rows
// For full definition, we'll need 2^10 for submodule, 2^5 for subladder, 2^18 for moduleId,
// so 2^33, just 1 bit more than a 32-bit integer
// For almost full definition, we thus truncate 1 bit in submodule
// ModuleId is as defined by Sebastien Viret
static const id_type iSubModuleStartBit_ = 0;
static const id_type iSubLadderStartBit_ = 9;
static const id_type iModuleIdStartBit_  = 14;

static const id_type iSubModuleMask_     = 0x1FF;   // 0-511 (full: 0-1023)
static const id_type iSubLadderMask_     = 0x1F;    // 0-31
static const id_type iModuleIdMask_      = 0x3FFFF; // 0-262143


/// Functions
// Retrieve layer, ladder, module from a moduleId
inline id_type decodeLayer(id_type moduleId) {
    return (moduleId / 10000) % 100;
}

inline id_type decodeLadder(id_type moduleId) {
    return (moduleId / 100) % 100;
}

inline id_type decodeModule(id_type moduleId) {
    return (moduleId) % 100;
}

inline id_type encodeModuleId(id_type lay, id_type lad, id_type mod) {
    return (10000*lay + 100*lad + mod);
}

// Encode moduleId, superstrip location into a superstripId
// SubLadder = column = _segment_
// SubModule = row = _superstrip_
inline addr_type encodeSuperstripId(id_type moduleId, id_type col, id_type row, bool truncate=false) {
    // If 'truncate' is set to true, we truncate 1 bit in submodule
    // This is needed if using full definition of col and row
    if (truncate)
        row = (row >> 1);

    addr_type id = 0;
    id |= addr_type(moduleId & iModuleIdMask_ ) << iModuleIdStartBit_  |
          addr_type(col      & iSubLadderMask_) << iSubLadderStartBit_ |
          addr_type(row      & iSubModuleMask_) << iSubModuleStartBit_ ;
    return id;
}

inline addr_type encodeSuperstripId(id_type lay, id_type lad, id_type mod, id_type col, id_type row) {
    id_type moduleId = encodeModuleId(lay, lad, mod);
    return encodeSuperstripId(moduleId, col, row);
}

// Retrieve layer, ladder, module, subladder, submodule from a superstripId
inline id_type decodeSuperstripModuleId(addr_type superstripId) {
    return addr_type(superstripId >> iModuleIdStartBit_) & iModuleIdMask_;
}

inline id_type decodeSuperstripLayer(addr_type superstripId) {
    return decodeLayer(decodeSuperstripModuleId(superstripId));
}

inline id_type decodeSuperstripLadder(addr_type superstripId) {
    return decodeLadder(decodeSuperstripModuleId(superstripId));
}

inline id_type decodeSuperstripModule(addr_type superstripId) {
    return decodeModule(decodeSuperstripModuleId(superstripId));
}

inline id_type decodeSuperstripSubLadder(addr_type superstripId) {
    return addr_type(superstripId >> iSubLadderStartBit_) & iSubLadderMask_;
}

inline id_type decodeSuperstripSubModule(addr_type superstripId) {
    return addr_type(superstripId >> iSubModuleStartBit_) & iSubModuleMask_;
}

inline void encodeDCBit(id_type nDCBits, addr_type& superstripId, bit_type& superstripBit) {  // NOTE: pass by reference
    if (nDCBits == 0)  return;  // don't do anything

    id_type moduleId = decodeSuperstripModuleId(superstripId);
    id_type col = decodeSuperstripSubLadder(superstripId);
    id_type row = decodeSuperstripSubModule(superstripId);

    // FIXME: only do the submodule, since there's only one DC bit
    //id_type col_mod_n = col % (1 << nDCBits);
    id_type row_mod_n = row % (1 << nDCBits);
    //col /= (1 << nDCBits);
    row /= (1 << nDCBits);

    superstripId = encodeSuperstripId(moduleId, col, row);
    superstripBit = (1 << row_mod_n);
    return;
}

inline addr_type encodeFakeSuperstripId() {
    // An impossible value
    addr_type id = 0;
    id -= 1;  // 2^32 - 1 = 4294967295
    return id;
}

inline bool isFakeSuperstripId(addr_type id) {
    return id == encodeFakeSuperstripId();
}

// Make a map to merge layers in barrel and in endcap
inline std::map<unsigned, unsigned> getLayerMergingMap(int n) {
    // Hardcoded layer information
    if (n <= 6) {
        std::map<unsigned, unsigned> thismap {
            {5,5}, {6,6}, {7,7}, {8,8}, {9,9}, {10,10},
            {11,10}, {12,9}, {13,8}, {14,7}, {15,6},
            {18,10}, {19,9}, {20,8}, {21,7}, {22,6}
        };
        return thismap;

    } else if (n == 7) {
        std::map<unsigned, unsigned> thismap {
            {5,5}, {6,6}, {7,7}, {8,8}, {9,9}, {10,10},
            {11,11}, {12,10}, {13,9}, {14,8}, {15,7},
            {18,11}, {19,10}, {20,9}, {21,8}, {22,7}
        };
        return thismap;

    } else {  // >= 8
        std::map<unsigned, unsigned> thismap {
            {5,5}, {6,6}, {7,7}, {8,8}, {9,9}, {10,10},
            {11,11}, {12,12}, {13,10}, {14,9}, {15,8},
            {18,11}, {19,12}, {20,10}, {21,9}, {22,8}
        };
        return thismap;
    }
}
#endif  // if not defined __GCCXML__

}  // namespace slhcl1tt

#endif
