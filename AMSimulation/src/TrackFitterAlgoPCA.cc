#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TrackFitterAlgoPCA.h"
using namespace slhcl1tt;

#include "Eigen/Eigenvalues"
#include "Eigen/QR"
#include <iostream>
#include <iomanip>
#include <fstream>


// _____________________________________________________________________________
int TrackFitterAlgoPCA::bookHistograms() {
    //TH1::AddDirectory(kFALSE);

    return 0;
}


// _____________________________________________________________________________
int TrackFitterAlgoPCA::loadConstants() {
    TString txt = "matrices_tt27_5M.txt";
    std::ifstream infile(txt.Data());
    if (!infile) {
        std::cout << "Unable to open " << txt << std::endl;
        return 1;
    }

    double x;
    solutionsC_ = Eigen::MatrixXd::Zero(1,nvariables_/2);
    for (unsigned ivar=0; ivar<nvariables_/2; ++ivar) {
        infile >> x;
        solutionsC_(0, ivar) = x;
    }

    solutionsT_ = Eigen::MatrixXd::Zero(1,nvariables_/2);
    for (unsigned ivar=0; ivar<nvariables_/2; ++ivar) {
        infile >> x;
        solutionsT_(0, ivar) = x;
    }

    meansR_ = Eigen::VectorXd::Zero(nvariables_/2);
    for (unsigned ivar=0; ivar<nvariables_/2; ++ivar) {
        infile >> x;
        meansR_(ivar) = x;
    }

    meansC_ = Eigen::VectorXd::Zero(1);
    infile >> x;
    meansC_(0) = x;

    meansT_ = Eigen::VectorXd::Zero(1);
    infile >> x;
    meansT_(0) = x;

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
int TrackFitterAlgoPCA::fit(const TTRoadComb& acomb, TTTrack2& track) {

    Eigen::VectorXd variables1 = Eigen::VectorXd::Zero(nvariables_/2);
    Eigen::VectorXd variables2 = Eigen::VectorXd::Zero(nvariables_/2);
    Eigen::VectorXd variables3 = Eigen::VectorXd::Zero(nvariables_/2);

    for (unsigned istub=0; istub<acomb.stubs_phi.size(); ++istub) {
        variables1(istub) = acomb.stubs_phi.at(istub);
        variables2(istub) = acomb.stubs_z.at(istub);
        variables3(istub) = meansR_(istub) - acomb.stubs_r.at(istub);
    }

    variables1 += ((solutionsC_ * variables1)(0,0)) * variables3;
    variables2 += ((solutionsT_ * variables2)(0,0)) * variables3;

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

    std::cout << "solutionsC: " << std::endl;
    std::cout << solutionsC_ << std::endl << std::endl;
    std::cout << "solutionsT: " << std::endl;
    std::cout << solutionsT_ << std::endl << std::endl;
    std::cout << "meansR: " << std::endl;
    std::cout << meansR_ << std::endl << std::endl;
    std::cout << "meansC: " << std::endl;
    std::cout << meansC_ << std::endl << std::endl;
    std::cout << "meansT: " << std::endl;
    std::cout << meansT_ << std::endl << std::endl;

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
