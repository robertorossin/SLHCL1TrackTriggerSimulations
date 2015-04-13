#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TrackFitterAlgoPCA.h"

#include "SLHCL1TrackTriggerSimulations/AMSimulation/external/Eigen/Eigenvalues"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/external/Eigen/QR"
#include <iostream>
#include <iomanip>
#include <fstream>

static const unsigned NVARIABLES = 12;  // number of hit coordinates
static const unsigned NPARAMETERS = 4;  // number of track parameters


// _____________________________________________________________________________
int TrackFitterAlgoPCA::bookHistograms() {
    TH1::AddDirectory(kFALSE);
    //TString hname;
    //for (unsigned ivar=0; ivar<NVARIABLES; ++ivar) {
    //    hname = Form("var%i", ivar);
    //    histograms[hname] = new TH1F(hname, ";"+hname, 1000, -1., 1.);
    //
    //    hname = Form("pc%i", ivar);
    //    histograms[hname] = new TH1F(hname, ";"+hname, 1000, -1., 1.);
    //}
    //
    //TString parnames[NPARAMETERS] = {"phi", "cotTheta", "z0", "invPt"};
    //for (unsigned ipar=0; ipar<NPARAMETERS; ++ipar) {
    //    hname = Form("par%i", ipar);
    //    histograms[hname] = new TH1F(hname, ";"+hname, 1000, -1., 1.);
    //
    //    hname = Form("errpar%i", ipar);
    //    histograms[hname] = new TH1F(hname, ";"+hname, 1000, -0.01, 0.01);
    //}

    return 0;
}


// _____________________________________________________________________________
int TrackFitterAlgoPCA::loadConstants(TString txt) {
    std::ifstream infile(txt.Data());
    if (!infile) {
        std::cout << "Unable to open " << txt << std::endl;
        return 1;
    }

    double x;
    sqrtEigenvalues_ = Eigen::VectorXd::Zero(NVARIABLES);
    for (unsigned ivar=0; ivar<NVARIABLES; ++ivar) {
        infile >> x;
        sqrtEigenvalues_(ivar) = x;
    }

    V_ = Eigen::MatrixXd::Zero(NVARIABLES, NVARIABLES);
    for (unsigned ivar=0; ivar<NVARIABLES; ++ivar) {
        for (unsigned jvar=0; jvar<NVARIABLES; ++jvar) {
            infile >> x;
            V_(ivar, jvar) = x;
        }
    }

    D_ = Eigen::MatrixXd::Zero(NPARAMETERS, NVARIABLES);
    for (unsigned ipar=0; ipar<NPARAMETERS; ++ipar) {
        for (unsigned jvar=0; jvar<NVARIABLES; ++jvar) {
            infile >> x;
            D_(ipar, jvar) = x;
        }
    }

    DV_ = Eigen::MatrixXd::Zero(NPARAMETERS, NVARIABLES);
    DV_ = D_ * V_;

    return 0;
}

// _____________________________________________________________________________
int TrackFitterAlgoPCA::fit(const std::vector<TTHit>& hits, TTTrack2& track) {

    if (hits.size() != NVARIABLES/2)  // FIXME: use hit bits
        return 0;

    Eigen::VectorXd variables = Eigen::VectorXd::Zero(NVARIABLES);
    Eigen::VectorXd means = Eigen::VectorXd::Zero(NVARIABLES);
    for (unsigned i=0, ivar=0; i<hits.size(); ++i) {
        const TTHit& hit = hits.at(i);
        variables(ivar++) = hit.r * hit.phi;
        variables(ivar++) = hit.z;
    }

    Eigen::VectorXd principals = Eigen::VectorXd::Zero(NVARIABLES);
    principals = V_ * (variables - means);

    Eigen::VectorXd parameters_fit = Eigen::VectorXd::Zero(NPARAMETERS);
    parameters_fit = DV_ * (variables - means);

    unsigned ndof = NVARIABLES - NPARAMETERS;
    double chi2 = 0.;
    for (unsigned i=0; i<ndof; ++i) {
        chi2 += (principals(i)/sqrtEigenvalues_(i))*(principals(i)/sqrtEigenvalues_(i));
    }

    double rinv = 0.003 * 3.8 * parameters_fit(3);
    double phi0 = parameters_fit(0);
    double cottheta0 = parameters_fit(0);
    double z0 = parameters_fit(0);
    double d0 = 0.;

    track.setTrackParams(rinv, phi0, cottheta0, z0, d0, chi2, ndof, 0., 0.);

    std::vector<float> principals_vec;
    for (unsigned ivar=0; ivar<NVARIABLES; ++ivar) {
        principals_vec.push_back(principals(ivar));
    }

    track.setPrincipals(principals_vec);

    return 0;
}

// _____________________________________________________________________________
void TrackFitterAlgoPCA::print() {
    std::ios::fmtflags flags = std::cout.flags();
    std::cout << std::setprecision(4);
    std::cout << "sqrtEigenvalues: " << std::endl;
    std::cout << sqrtEigenvalues_ << std::endl << std::endl;
    std::cout << "V: " << std::endl;
    std::cout << V_ << std::endl << std::endl;
    std::cout << "D: " << std::endl;
    std::cout << D_ << std::endl << std::endl;
    std::cout << "DV: " << std::endl;
    std::cout << DV_ << std::endl << std::endl;
    std::cout.flags(flags);
}