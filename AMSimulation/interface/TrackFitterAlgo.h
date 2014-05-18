#ifndef AMSimulation_TrackFitterAlgo_h_
#define AMSimulation_TrackFitterAlgo_h_

#include <cassert>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

// FIXME: 'two-step' approach is not yet implemented


////////////////////////////////////////////////////////////////////////////////
// CoordTypes
// Each coord type is just a pair of number for 2D retina track fitter
struct XY : public std::pair<float, float> {
    XY(float x, float y) : std::pair<float, float>(x, y) {}

    float x() const { return this->first; }
    float y() const { return this->second; }
};

struct UV : public std::pair<float, float> {
    UV(float u, float v) : std::pair<float, float>(u, v) {}

    // This is u
    float x() const { return this->first; }
    // This is v
    float y() const { return this->second; }
};

struct ZR : public std::pair<float, float> {
    ZR(float z, float r) : std::pair<float, float>(z, r) {}

    // This is z
    float x() const { return this->first; }
    // This is r
    float y() const { return this->second; }
};
////////////////////////////////////////////////////////////////////////////////

// _____________________________________________________________________________
// Track fitter
struct TrackParam {
    TrackParam()
    : pt(0), phi(0), eta(0), dz(0), d0(0), chi2(0) {}

    float pt;
    float phi;
    float eta;
    float dz;
    float d0;
    float chi2;  // for retina, it means cell weight
};

// Base class -- currently useless
class TrackFitterAlgoBase {
  public:
    TrackFitterAlgoBase() {}
    ~TrackFitterAlgoBase() {}

};

// _____________________________________________________________________________
// Retina track fitter
// Cell in the PQ grid
struct Cell_pq {
    float p;
    float q;
    float w;
};

struct Cell_ij {
    int i;
    int j;
    float w;
};

// Retina track fitter algo
// Currently this algo only works for maximum 2D coordinates
// CoordType to be either 1D (X), or 2D (ZR, UV)
template <class CoordType>
class RetinaTrackFitterAlgo : TrackFitterAlgoBase {
  public:
    RetinaTrackFitterAlgo(int pqType, int pbins, int qbins, float pmin, float qmin, float pmax, float qmax, float sigma, float minWeight)
    : pqType_(pqType), pbins_(pbins), qbins_(qbins), pmin_(pmin), qmin_(qmin), pmax_(pmax), qmax_(qmax),
      sigma_(sigma), minWeight_(minWeight) {

        pstep_ = (pmax-pmin) / (float) pbins;
        qstep_ = (qmax-qmin) / (float) qbins;
        makeGrid();
    }

    ~RetinaTrackFitterAlgo() {}

    // Setters
    void setHits(const std::vector<CoordType>& hits) { hits_ = hits; }

    // Getters
    std::vector<CoordType> getHits()  const { return hits_; }

    // Functions
    void fillGrid();

    void findMaxima();

    std::vector<TrackParam> getParams();

    void resetGrid();

  private:
    void makeGrid();
    float getResponse(float m_temp, float b_temp);
    float getResponseRistori(float xplus_temp, float xminus_temp);
    float getResponsePol(float gamma_temp, float b_temp);
    Cell_pq findMaximumInterpolated(Cell_ij point);

  private:
    int pqType_;
    int pbins_;
    int qbins_;
    float pmin_;
    float qmin_;
    float pmax_;
    float qmax_;
    float pstep_;
    float qstep_;
    float sigma_;
    float minWeight_;

    std::vector<std::vector<float> > grid_;
    std::vector<Cell_pq> maxima_;

    std::vector<CoordType> hits_;
};


// _____________________________________________________________________________
inline bool sortByWeight(const Cell_pq& lhs, const Cell_pq& rhs) {
    return lhs.w > rhs.w;
}

// _____________________________________________________________________________
inline std::ostream& operator<<(std::ostream& o, const TrackParam& p) {
    o << "pt: " << p.pt << "  phi: " << p.phi << "  eta: " << p.eta << "  dz: " << p.dz << "  d0: " << p.d0 << "  chi2: " << p.chi2;
    return o;
}


#endif
