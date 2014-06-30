#ifndef AMSimulationDataFormats_Helper_h_
#define AMSimulationDataFormats_Helper_h_

#include <cmath>
#include <map>
#include <stdint.h>  // consistent with DataFormats/DetId/interface/DetId.h


namespace slhcl1tt {

// Typically, short is 16 bits, int is 32 bits, long is 32 bits and long long is 64 bits
// But we need to guarantee on the size of the integer
typedef uint8_t  count_type; // for frequency: 0 - 255
typedef uint16_t bit_type;   // for DC bits: 0 - 2^16 (=65536 max for nDCBits_=4)
typedef uint32_t id_type;    // generic
typedef uint32_t addr_type;  // for full superstrip id, now reduced to 32-bit (from 64-bit) and is therefore the same as id_type
typedef uint32_t key_type;   // for key used in the pattern matching map, now increased to 32-bit (from 16-bit) and is therefore the same as id_type


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
inline id_type halfStripRound(float x) {
    static const float p = 10.;
    return floor((x*2)*p + 0.5)/p;
}

// Retrieve layer, ladder, module from a moduleId
inline id_type decodeLayer(id_type moduleId) {
    return (moduleId / 10000) % 100;
}

inline id_type decodeLayer4bit(id_type moduleId) {
    id_type lay = (moduleId / 10000) % 100;
    if (5 <= lay && lay <= 10)
        return (lay - 5);
    if (11 <= lay && lay <= 15)
        return (lay - 5);
    if (18 <= lay && lay <= 22)
        return (lay - 7);
    if (26 == lay)  // fake superstrip
        return 15;
    throw "Unexpected layer id";
    return -1;
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
// SubLadder = column = _segment_ = coordy
// SubModule = row = _superstrip_ = coordx
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

inline id_type decodeSuperstripLayer4bit(addr_type superstripId) {
    return decodeLayer4bit(decodeSuperstripModuleId(superstripId));
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

    // Only do the submodule, since there's only one DC bit
    //id_type col_mod_n = col % (1 << nDCBits);
    id_type row_mod_n = row % (1 << nDCBits);
    //col /= (1 << nDCBits);
    row /= (1 << nDCBits);

    superstripId = encodeSuperstripId(moduleId, col, row);
    superstripBit = (1 << row_mod_n);
    return;
}

inline addr_type encodeFakeSuperstripId() {
    // An impossible value: 2^32 - 1 = 4294967295
    return addr_type(-1);
}

inline bool isFakeSuperstripId(addr_type superstripId) {
    return (superstripId == encodeFakeSuperstripId());
}

inline key_type hashSuperstripId(addr_type superstripId) {
    // A simple hash function to make superstripId distribute more or less uniformly
    // Hence we might prefer to keep the least significant bits
    id_type lay = decodeSuperstripLayer4bit(superstripId);
    id_type lad = decodeSuperstripLadder(superstripId);
    id_type mod = decodeSuperstripModule(superstripId);
    id_type col = decodeSuperstripSubLadder(superstripId);
    id_type row = decodeSuperstripSubModule(superstripId);
    // Going from 32 bits (18 - 5 - 9) to 21 bits (4 - 4 - 7 - 1 - 5)
    key_type h = 0;
    h |= key_type(lay & 0xF ) << 17 |
         key_type(lad & 0xF ) << 13 |
         key_type(mod & 0x7F) << 6  |
         key_type(col & 0x1 ) << 5  |
         key_type(row & 0x1F) << 0  ;
    return h;
}

// Make a map to merge layers in barrel and in endcap
inline std::map<unsigned, unsigned> getLayerMergingMap(int n) {
    // Hardcoded layer information
    if (n <= 6) {
        std::map<unsigned, unsigned> thismap {
            {5,0}, {6,1}, {7,2}, {8,3}, {9,4}, {10,5},
            {11,5}, {12,4}, {13,3}, {14,2}, {15,1},
            {18,5}, {19,4}, {20,3}, {21,2}, {22,1}
        };
        return thismap;

    } else if (n == 7) {
        std::map<unsigned, unsigned> thismap {
            {5,0}, {6,1}, {7,2}, {8,3}, {9,4}, {10,5},
            {11,6}, {12,5}, {13,4}, {14,3}, {15,2},
            {18,6}, {19,5}, {20,4}, {21,3}, {22,2}
        };
        return thismap;

    } else {  // >= 8
        std::map<unsigned, unsigned> thismap {
            {5,0}, {6,1}, {7,2}, {8,3}, {9,4}, {10,5},
            {11,7}, {12,6}, {13,5}, {14,4}, {15,3},
            {18,7}, {19,6}, {20,5}, {21,4}, {22,3}
        };
        return thismap;
    }
}
#endif  // if not defined __GCCXML__

}  // namespace slhcl1tt

#endif
