#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TrackFitterAlgo.h"

#ifndef ROOT_Math_GenVector_eta
#include "Math/GenVector/eta.h"
#endif


template <class CoordType>
void RetinaTrackFitterAlgo<CoordType>::fillGrid() {
    assert(!hits_.empty());

    for (int i=0; i<pbins_; ++i) {
        float p_value = pmin_ + pstep_ * (i + 0.5);

        for (int j=0; j<qbins_; ++j) {
            float q_value = qmin_ + qstep_ * (j + 0.5);

            if (pqType_ == 0)
                grid_[i][j] = getResponse(p_value, q_value);
            else if (pqType_ == 1)
                grid_[i][j] = getResponseRistori(p_value, q_value);
            else if (pqType_ == 2)
                grid_[i][j] = getResponsePol(p_value, q_value);
        }
    }
}

template <class CoordType>
void RetinaTrackFitterAlgo<CoordType>::findMaxima() {  // local maxima in 3x3 cells
    for (int i=1; i<pbins_-1; ++i) {
        for (int j=1; j<qbins_-1; ++j) {
            if (grid_[i][j] > grid_[i-1][j] &&
                grid_[i][j] > grid_[i+1][j] &&
                grid_[i][j] > grid_[i][j-1] &&
                grid_[i][j] > grid_[i][j+1] &&
                grid_[i][j] > grid_[i+1][j-1] &&
                grid_[i][j] > grid_[i+1][j+1] &&
                grid_[i][j] > grid_[i-1][j-1] &&
                grid_[i][j] > grid_[i-1][j+1]) {

                if (grid_[i][j] < minWeight_)  continue;  // cleaning
                Cell_ij cell;
                cell.i = i;
                cell.j = j;
                cell.w = grid_[i][j];
                Cell_pq cell_interpolated = findMaximumInterpolated(cell);
                maxima_.push_back(cell_interpolated);
            }
        }
    }

    std::sort(maxima_.begin(), maxima_.end(), sortByWeight);
}

// Specialized for ZR
template <>
std::vector<TrackParam> RetinaTrackFitterAlgo<ZR>::getParams() {
    std::vector<TrackParam> params;
    for (unsigned i=0; i<maxima_.size(); ++i) {
        Cell_pq cell = maxima_.at(i);
        TrackParam param;

        double theta = std::atan(cell.p);  // p=m, q=b, is only true for pqType=0
        //param.eta = -std::log(std::tan(theta/2.));
        param.eta = ROOT::Math::Impl::Eta_FromTheta(theta, 0.);
        param.dz = -cell.q / cell.p;
        param.chi2 = cell.w;
        params.push_back(param);
    }
    return params;
}

template <class CoordType>
void RetinaTrackFitterAlgo<CoordType>::resetGrid() {
    grid_.clear();
    maxima_.clear();
}


// _____________________________________________________________________________
// Private functions

template <class CoordType>
void RetinaTrackFitterAlgo<CoordType>::makeGrid() {
    grid_.resize(pbins_);
    for (int i=0; i<pbins_; ++i)
        grid_[i].resize(qbins_);
}

template <class CoordType>
Cell_pq RetinaTrackFitterAlgo<CoordType>::findMaximumInterpolated(Cell_ij cell) {  // only 1 maximum?
    int p_i = cell.i;
    int q_i = cell.j;
    float p_mean = 0.;
    float q_mean = 0.;

    p_mean =  (pmin_ + pstep_ * (p_i - 0.5)) * grid_[p_i-1][q_i  ] +
              (pmin_ + pstep_ * (p_i + 0.5)) * grid_[p_i  ][q_i  ] +
              (pmin_ + pstep_ * (p_i + 1.5)) * grid_[p_i+1][q_i  ];

    p_mean += (pmin_ + pstep_ * (p_i - 0.5)) * grid_[p_i-1][q_i-1] +
              (pmin_ + pstep_ * (p_i + 0.5)) * grid_[p_i  ][q_i-1] +
              (pmin_ + pstep_ * (p_i + 1.5)) * grid_[p_i+1][q_i-1];

    p_mean += (pmin_ + pstep_ * (p_i - 0.5)) * grid_[p_i-1][q_i+1] +
              (pmin_ + pstep_ * (p_i + 0.5)) * grid_[p_i  ][q_i+1] +
              (pmin_ + pstep_ * (p_i + 1.5)) * grid_[p_i+1][q_i+1];

    p_mean /= (grid_[p_i-1][q_i  ] + grid_[p_i][q_i  ] + grid_[p_i+1][q_i  ] +
               grid_[p_i-1][q_i-1] + grid_[p_i][q_i-1] + grid_[p_i+1][q_i-1] +
               grid_[p_i-1][q_i+1] + grid_[p_i][q_i+1] + grid_[p_i+1][q_i+1]);


    q_mean =  (qmin_ + qstep_ * (q_i - 0.5)) * grid_[p_i  ][q_i-1] +
              (qmin_ + qstep_ * (q_i + 0.5)) * grid_[p_i  ][q_i  ] +
              (qmin_ + qstep_ * (q_i + 1.5)) * grid_[p_i  ][q_i+1];

    q_mean += (qmin_ + qstep_ * (q_i - 0.5)) * grid_[p_i-1][q_i-1] +
              (qmin_ + qstep_ * (q_i + 0.5)) * grid_[p_i-1][q_i  ] +
              (qmin_ + qstep_ * (q_i + 1.5)) * grid_[p_i-1][q_i+1];

    q_mean += (qmin_ + qstep_ * (q_i - 0.5)) * grid_[p_i+1][q_i-1] +
              (qmin_ + qstep_ * (q_i + 0.5)) * grid_[p_i+1][q_i  ] +
              (qmin_ + qstep_ * (q_i + 1.5)) * grid_[p_i+1][q_i+1];

    q_mean /= (grid_[p_i  ][q_i-1] + grid_[p_i  ][q_i] + grid_[p_i  ][q_i+1] +
               grid_[p_i-1][q_i-1] + grid_[p_i-1][q_i] + grid_[p_i-1][q_i+1] +
               grid_[p_i+1][q_i-1] + grid_[p_i+1][q_i] + grid_[p_i+1][q_i+1]);

    Cell_pq cell_interpolated;
    cell_interpolated.p = p_mean;
    cell_interpolated.q = q_mean;
    cell_interpolated.w = cell.w;
    return cell_interpolated;
}


// See equation at: http://www.texpaste.com/n/dqhv8w8o
// $$R_{ij} = \sum_{k,\boldsymbol{r}} \exp\left(- \frac{s_{ijk\boldsymbol{r}}^{2}}{2\sigma^2}\right)$$
// where $$s_{ijk\boldsymbol{r}} = x_{\boldsymbol{r}}^{(k)} - y_{k}\left(p_{i}, q_{j}\right)$$
template <class CoordType>
float RetinaTrackFitterAlgo<CoordType>::getResponse(float m_temp, float b_temp) {
    float R_ij = 0.;
    unsigned nhits = hits_.size();

    float y_k, x_kr, s_ijkr, term;
    for (unsigned kr=0; kr<nhits; ++kr) {
        // y = mx + b --> x = (y-b)/m  (y_k is more of a x-coordinate)
        y_k = (hits_.at(kr).y() - b_temp) / m_temp;
        x_kr = hits_.at(kr).x();
        s_ijkr = x_kr - y_k;
        //if (s_ijkr / sigma > 3.) continue; // cut outliers
        term = std::exp(-0.5 * s_ijkr * s_ijkr / (sigma_ * sigma_) );
        R_ij += term;
    }

    if (R_ij < 1e-6)  return 1e-6;
    return R_ij;
}

// $$x_{\pm} = \frac{x_{1} \pm x_{2}}{2}$$
template <class CoordType>
float RetinaTrackFitterAlgo<CoordType>::getResponseRistori(float xplus_temp, float xminus_temp) {
    // 'y' = 23.0, 35.7, 50.8, 68.6, 88.8, 108.0
    float y1 = 108.0;
    float y2 = 23.0;
    float x1 = xplus_temp + xminus_temp;
    float x2 = xplus_temp - xminus_temp;

    float m_temp = (y2 - y1) / (x2 - x1);
    float b_temp = y2 - m_temp * x2;

    float R_ij = 0.;
    unsigned nhits = hits_.size();

    float y_k, x_kr, s_ijkr, term;
    for (unsigned kr=0; kr<nhits; ++kr) {
        // y = mx + b --> x = (y-b)/m  (y_k is more of a x-coordinate)
        y_k = (hits_.at(kr).y() - b_temp) / m_temp;
        x_kr = hits_.at(kr).x();
        s_ijkr = x_kr - y_k;
        //if (s_ijkr / sigma > 3.) continue; // cut outliers
        term = std::exp(-0.5 * s_ijkr * s_ijkr / (sigma_ * sigma_) );
        R_ij += term;
    }

    if (R_ij < 1e-6)  return 1e-6;
    return R_ij;
}

// Don't understand the equation yet :(
template <class CoordType>
float RetinaTrackFitterAlgo<CoordType>::getResponsePol(float gamma_temp, float b_temp) {
    float R_ij = 0.;
    unsigned nhits = hits_.size();

    float y_k, x_kr, s_ijkr, term, theta;
    for (unsigned kr=0; kr<nhits; ++kr) {
        theta = std::atan2(hits_.at(kr).y(), hits_.at(kr).x());
        y_k = b_temp * std::cos(gamma_temp) * std::cos(theta) / std::sin(theta - gamma_temp);
        x_kr = hits_.at(kr).x();
        s_ijkr = x_kr - y_k;
        //if (s_ijkr / sigma > 3.) continue; // cut outliers
        term = std::exp(-0.5 * s_ijkr * s_ijkr / (sigma_ * sigma_) );
        R_ij += term;
    }

    if (R_ij < 1e-6)  return 1e-6;
    return R_ij;
}


// Explicit instantiations
template class RetinaTrackFitterAlgo<ZR>;
