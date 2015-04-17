#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TrackFitterAlgoPCA.h"

#include "Eigen/Eigenvalues"
#include "Eigen/QR"
#include <iostream>
#include <iomanip>
#include <fstream>


// _____________________________________________________________________________
int TrackFitterAlgoPCA::bookHistograms() {
    TH1::AddDirectory(kFALSE);
    //TString hname;
    //for (unsigned ivar=0; ivar<nvariables_; ++ivar) {
    //    hname = Form("var%i", ivar);
    //    histograms[hname] = new TH1F(hname, ";"+hname, 1000, -1., 1.);
    //
    //    hname = Form("pc%i", ivar);
    //    histograms[hname] = new TH1F(hname, ";"+hname, 1000, -1., 1.);
    //}
    //
    //TString parnames[nparameters_] = {"phi", "cotTheta", "z0", "invPt"};
    //for (unsigned ipar=0; ipar<nparameters_; ++ipar) {
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
    V_ = Eigen::MatrixXd::Zero(nvariables_, nvariables_);
    for (unsigned ivar=0; ivar<nvariables_; ++ivar) {
        for (unsigned jvar=0; jvar<nvariables_; ++jvar) {
            infile >> x;
            V_(ivar, jvar) = x;
        }
    }

    D_ = Eigen::MatrixXd::Zero(nparameters_, nvariables_);
    for (unsigned ipar=0; ipar<nparameters_; ++ipar) {
        for (unsigned jvar=0; jvar<nvariables_; ++jvar) {
            infile >> x;
            D_(ipar, jvar) = x;
        }
    }

    DV_ = Eigen::MatrixXd::Zero(nparameters_, nvariables_);
    DV_ = D_ * V_;

    meansV_ = Eigen::VectorXd::Zero(nvariables_);
    for (unsigned ivar=0; ivar<nvariables_; ++ivar) {
        infile >> x;
        meansV_(ivar) = x;
    }

    meansP_ = Eigen::VectorXd::Zero(nparameters_);
    for (unsigned ipar=0; ipar<nparameters_; ++ipar) {
        infile >> x;
        meansP_(ipar) = x;
    }

    sqrtEigenvalues_ = Eigen::VectorXd::Zero(nvariables_);
    for (unsigned ivar=0; ivar<nvariables_; ++ivar) {
        infile >> x;
        sqrtEigenvalues_(ivar) = x;
    }

    return 0;
}

// _____________________________________________________________________________
int TrackFitterAlgoPCA::fit(const std::vector<TTHit>& hits, TTTrack2& track) {

    if (hits.size() != nvariables_/2)  // FIXME: use hit bits
        return 0;

    Eigen::VectorXd variables1 = Eigen::VectorXd::Zero(nvariables_/2);
    Eigen::VectorXd variables2 = Eigen::VectorXd::Zero(nvariables_/2);

    for (unsigned i=0; i<hits.size(); ++i) {
        const TTHit& hit = hits.at(i);
        variables1(i) = hit.phi;
        variables2(i) = hit.z;
    }

    Eigen::VectorXd variables = Eigen::VectorXd::Zero(nvariables_);
    variables << variables1, variables2;

    Eigen::VectorXd principals = Eigen::VectorXd::Zero(nvariables_);
    principals = V_ * variables;
    //principals -= meansV_;

    Eigen::VectorXd parameters_fit = Eigen::VectorXd::Zero(nparameters_);
    parameters_fit = DV_ * variables;
    //parameters_fit -= meansP_;

    unsigned ndof = nvariables_ - nparameters_;
    double chi2 = 0.;
    for (unsigned i=0; i<ndof; ++i) {
        chi2 += (principals(i)/sqrtEigenvalues_(i))*(principals(i)/sqrtEigenvalues_(i));
    }

    double rinv = 0.003 * 3.8 * parameters_fit(3);
    double phi0 = parameters_fit(0);
    double cottheta0 = parameters_fit(1);
    double z0 = parameters_fit(2);
    double d0 = 0.;

    track.setTrackParams(rinv, phi0, cottheta0, z0, d0, chi2, ndof, 0., 0.);

    std::vector<float> principals_vec;
    for (unsigned ivar=0; ivar<nvariables_; ++ivar) {
        principals_vec.push_back(principals(ivar));
    }

    track.setPrincipals(principals_vec);

    return 0;
}

// _____________________________________________________________________________
void TrackFitterAlgoPCA::print() {
    std::ios::fmtflags flags = std::cout.flags();
    std::cout << std::setprecision(4);
    std::cout << "V: " << std::endl;
    std::cout << V_ << std::endl << std::endl;
    std::cout << "D: " << std::endl;
    std::cout << D_ << std::endl << std::endl;
    std::cout << "meansV: " << std::endl;
    std::cout << meansV_ << std::endl << std::endl;
    std::cout << "meansP: " << std::endl;
    std::cout << meansP_ << std::endl << std::endl;
    std::cout << "sqrtEigenvalues: " << std::endl;
    std::cout << sqrtEigenvalues_ << std::endl << std::endl;
    std::cout.flags(flags);
}
