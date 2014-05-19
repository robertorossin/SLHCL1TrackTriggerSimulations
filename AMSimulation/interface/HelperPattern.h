#ifndef AMSimulation_HelperPattern_h_
#define AMSimulation_HelperPattern_h_

//#include <cstdint>
//#include <array>
#include <stdint.h>  // consistent with DataFormats/DetId/interface/DetId.h
#include <tr1/array>

namespace slhcl1tt {

// Typically, short is 16 bits, int is 32 bits, long is 32 bits and long long is 64 bits
// But we need to guarantee on the size of the integer
typedef uint16_t bit_type;   // for DC bits: 0 - 2^16 (=65536 max for nDCBits_=4)
typedef uint32_t id_type;
typedef uint64_t addr_type;
typedef std::tr1::array<addr_type,8>  pattern_type;  // maximum number of superstrips in a pattern set to 8 due to hardware design

// Constants
// Using full definitions now -- need to be reduced eventually
static const id_type iSubModuleStartBit_ = 0;
static const id_type iSubLadderStartBit_ = 10;
static const id_type iModuleStartBit_    = 15;
static const id_type iLadderStartBit_    = 22;
static const id_type iLayerStartBit_     = 29;
// next start bit should be 34

static const id_type iSubModuleMask_     = 0x3FF; // 0-1023
static const id_type iSubLadderMask_     = 0x1F;  // 0-31
static const id_type iModuleMask_        = 0x7F;  // 0-127
static const id_type iLadderMask_        = 0x7F;  // 0-127
static const id_type iLayerMask_         = 0x1F;  // 0-15


// Retrieve layer, ladder, module from a moduleId
inline id_type decodeLayer(id_type moduleId) {
    return moduleId / 10000;
}

inline id_type decodeLadder(id_type moduleId) {
    return (moduleId / 100) % 100;
}

inline id_type decodeModule(id_type moduleId) {
    return moduleId % 100;
}

// Encode moduleId, superstrip location into a superstripId
// SubLadder = column = _segment_
// SubModule = row = _superstrip_
inline addr_type encodeSuperstripId(id_type lay, id_type lad, id_type mod, id_type col, id_type row) {
    addr_type id = 0;
    id |= addr_type(lay & iLayerMask_    ) << iLayerStartBit_     |
          addr_type(lad & iLadderMask_   ) << iLadderStartBit_    |
          addr_type(mod & iModuleMask_   ) << iModuleStartBit_    |
          addr_type(col & iSubLadderMask_) << iSubLadderStartBit_ |
          addr_type(row & iSubModuleMask_) << iSubModuleStartBit_ ;
    return id;
}

inline addr_type encodeSuperstripId(id_type moduleId, id_type col, id_type row) {
    id_type lay = decodeLayer(moduleId);
    id_type lad = decodeLadder(moduleId);
    id_type mod = decodeModule(moduleId);
    return encodeSuperstripId(lay, lad, mod, col, row);
}

// Retrieve layer, ladder, module, subladder, submodule from a superstripId
inline id_type decodeSuperstripLayer(addr_type superstripId) {
    return addr_type(superstripId >> iLayerStartBit_) & iLayerMask_;
}

inline id_type decodeSuperstripLadder(addr_type superstripId) {
    return addr_type(superstripId >> iLadderStartBit_) & iLadderMask_;
}

inline id_type decodeSuperstripModule(addr_type superstripId) {
    return addr_type(superstripId >> iModuleStartBit_) & iModuleMask_;
}

inline id_type decodeSuperstripSubLadder(addr_type superstripId) {
    return addr_type(superstripId >> iSubLadderStartBit_) & iSubLadderMask_;
}

inline id_type decodeSuperstripSubModule(addr_type superstripId) {
    return addr_type(superstripId >> iSubModuleStartBit_) & iSubModuleMask_;
}

inline void encodeDCBit(id_type nDCBits, addr_type& superstripId, bit_type& superstripBit) {  // NOTE: pass by reference
    if (nDCBits == 0)  return;  // don't do anything

    id_type lay = decodeSuperstripLayer(superstripId);
    id_type lad = decodeSuperstripLadder(superstripId);
    id_type mod = decodeSuperstripModule(superstripId);
    id_type col = decodeSuperstripSubLadder(superstripId);
    id_type row = decodeSuperstripSubModule(superstripId);

    // FIXME: only do the submodule, since there's only one DC bit
    //id_type col_mod_n = col % (1 << nDCBits);
    id_type row_mod_n = row % (1 << nDCBits);
    //col /= (1 << nDCBits);
    row /= (1 << nDCBits);

    superstripId = encodeSuperstripId(lay, lad, mod, col, row);
    superstripBit = (1 << row_mod_n);
    return;
}

}  // namespace slhcl1tt

#endif
