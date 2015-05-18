#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TrackFitterAlgoPCA.h"
using namespace slhcl1tt;

#include <iostream>


// _____________________________________________________________________________
int TrackFitterAlgoPCA::bookHistograms() {
    //TH1::AddDirectory(kFALSE);

    return 0;
}

// _____________________________________________________________________________
int TrackFitterAlgoPCA::loadConstants() {

    for (unsigned i=0; i<PCA_NSEGMENTS; ++i) {
        for (unsigned j=0; j<PCA_NHITBITS; ++j) {
            std::string filename = Form("matrix/matrices_tt%i_pt%i_hb%i.txt", tower_, i, j);

            PCAMatrix matrix;
            matrix.read(datadir_ + filename, nvariables_, nparameters_);

            matrices.push_back(matrix);
        }
    }

    if (verbose_>2)
        print();

    return 0;
}

// _____________________________________________________________________________
int TrackFitterAlgoPCA::fit(const TTRoadComb& acomb, TTTrack2& atrack) {

    int imat = acomb.ptSegment * PCA_NHITBITS + acomb.hitBits;
    const PCAMatrix& mat = matrices[imat];

    Eigen::VectorXd variables1 = Eigen::VectorXd::Zero(nvariables_/2);
    Eigen::VectorXd variables2 = Eigen::VectorXd::Zero(nvariables_/2);
    Eigen::VectorXd variables3 = Eigen::VectorXd::Zero(nvariables_/2);

    for (unsigned istub=0; istub<acomb.stubs_phi.size(); ++istub) {
        variables1(istub) = acomb.stubs_phi.at(istub);
        variables2(istub) = acomb.stubs_z.at(istub);
        variables3(istub) = mat.meansR(istub) - acomb.stubs_r.at(istub);
    }

    variables1 += ((mat.solutionsC * variables1)(0,0)) * variables3;
    variables2 += ((mat.solutionsT * variables2)(0,0)) * variables3;

    Eigen::VectorXd variables = Eigen::VectorXd::Zero(nvariables_);
    variables << variables1, variables2;

    Eigen::VectorXd principals = Eigen::VectorXd::Zero(nvariables_);
    principals = mat.V * variables;
    principals -= mat.meansV;

    Eigen::VectorXd parameters_fit = Eigen::VectorXd::Zero(nparameters_);
    parameters_fit = mat.DV * variables;
    parameters_fit -= mat.meansP;

    unsigned ndof = nvariables_ - nparameters_;
    if (1 <= acomb.hitBits && acomb.hitBits <= 6) {
        if (nvariables_ == 6 * 2)
            ndof -= 2;
        else if (nvariables_ == 6)
            ndof -= 1;
    }

    double chi2 = 0.;
    for (unsigned i=0; i<ndof; ++i) {
        chi2 += (principals(i)/mat.sqrtEigenvalues(i))*(principals(i)/mat.sqrtEigenvalues(i));
    }

    double rinv      = 0.003 * 3.8 * parameters_fit(3);
    double phi0      = parameters_fit(0);
    double cottheta0 = parameters_fit(1);
    double z0        = parameters_fit(2);
    double d0        = 0.;

    atrack.setTrackParams(rinv, phi0, cottheta0, z0, d0, chi2, ndof, 0., 0.);

    std::vector<float> principals_vec;
    for (unsigned ivar=0; ivar<nvariables_; ++ivar) {
        principals_vec.push_back(principals(ivar));
    }
    atrack.setPrincipals(principals_vec);

    return 0;
}

// _____________________________________________________________________________
void TrackFitterAlgoPCA::print() {
    std::cout << "view: " << view_ << " nvariables: " << nvariables_ << " nparameters: " << nparameters_ << std::endl;
    for (unsigned i=0; i<matrices.size(); ++i) {
        std::cout << "** matrix " << i << std::endl;
        matrices.at(i).print();
    }
    std::cout << std::endl;
}
