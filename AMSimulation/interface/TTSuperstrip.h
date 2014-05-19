#ifndef AMSimulation_TTSuperstrip_h_
#define AMSimulation_TTSuperstrip_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/HelperPattern.h"

#include <cmath>
#include <iosfwd>
#include <vector>


namespace slhcl1tt {

// TTSuperstrip is designed to be the building block of a TTPattern.
// Nominally, it corresponds to a physical space in the detector, but
// it doesn't have to be.
class TTSuperstrip {
  public:
    // Constructors
    TTSuperstrip()
    : id_(0), bit_(1) {}

    TTSuperstrip(addr_type id, bit_type bit)
    : id_(id), bit_(bit) {}

    TTSuperstrip(const TTSuperstrip& rhs)
    : id_(rhs.id_), bit_(rhs.bit_) {}

    // Destructor
    ~TTSuperstrip() {}

    // Setters
    void setId(addr_type id)  { id_ = id; }
    void setBit(bit_type bit) { bit_ = bit; }

    // Getters
    addr_type id() const { return id_; }
    bit_type bit() const { return bit_; }

  private:
    addr_type id_;
    bit_type bit_;
};


// TTHit is designed to be the building block of a TTRoad.
// Nominally, it corresponds to the smallest unit of measurement in the
// detector, but it doesn't have to be.
class TTHit {
  public:
    // Constructors
    TTHit()
    : x_(0.), y_(0.), z_(0.), xerr_(999.), yerr_(999.), zerr_(999.),
      charge_(-1), pt_(0.), ssId_(0) {}

    TTHit(float x, float y, float z, float pt, addr_type ssId)
    : x_(x), y_(y), z_(z), xerr_(999.), yerr_(999.), zerr_(999.),
      charge_(-1), pt_(pt), ssId_(ssId) {}

    TTHit(float x, float y, float z, float xerr, float yerr, float zerr,
          int charge, float pt, addr_type ssId)
    : x_(x), y_(y), z_(z), xerr_(xerr), yerr_(yerr), zerr_(zerr),
      charge_(charge), pt_(pt), ssId_(ssId) {}

    TTHit(const TTHit& rhs)
    : x_(rhs.x_), y_(rhs.y_), z_(rhs.z_), xerr_(rhs.xerr_), yerr_(rhs.yerr_), zerr_(rhs.zerr_),
      charge_(rhs.charge_), pt_(rhs.pt_), ssId_(rhs.ssId_) {}

    // Destructor
    ~TTHit() {}

    // Setters
    void setX(float x)                    { x_ = x; }
    void setY(float y)                    { y_ = y; }
    void setZ(float z)                    { z_ = z; }
    void setXError(float xerr)            { xerr_ = xerr; }
    void setYError(float yerr)            { yerr_ = yerr; }
    void setZError(float zerr)            { zerr_ = zerr; }
    void setCharge(int charge)            { charge_ = charge; }
    void setPt(float pt)                  { pt_ = pt; }
    void setSuperstripId(addr_type ssId)  { ssId_ = ssId; }

    // Getters
    float x()                       const { return x_; }
    float y()                       const { return y_; }
    float z()                       const { return z_; }
    float xError()                  const { return xerr_; }
    float yError()                  const { return yerr_; }
    float zError()                  const { return zerr_; }
    int charge()                    const { return charge_; }
    float pt()                      const { return pt_; }
    addr_type superstripId()        const { return ssId_; }

    // Functions
    // Simple trigonometrics
    float rho()                     const { return std::sqrt(x_*x_ + y_*y_); }
    float phi()                     const { return std::atan2(y_, x_); }
    float theta()                   const { return std::atan2(rho(), z_); }

    // Conformal transformantion
    float u()                       const { return x_ / (x_*x_ + y_*y_); }
    float v()                       const { return y_ / (x_*x_ + y_*y_); }

  private:
    float x_;
    float y_;
    float z_;
    float xerr_;
    float yerr_;
    float zerr_;
    int charge_;
    float pt_;
    addr_type ssId_;
};


// _____________________________________________________________________________
// Output streams
std::ostream& operator<<(std::ostream& o, const TTSuperstrip& ss);

std::ostream& operator<<(std::ostream& o, const TTHit& hit);

}  // namespace slhcl1tt

#endif
