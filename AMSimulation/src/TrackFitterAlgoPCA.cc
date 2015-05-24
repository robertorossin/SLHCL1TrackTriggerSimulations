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

    unsigned begin_ivar = 0;
    if (1 <= acomb.hitBits && acomb.hitBits <= 6) {
        if (nvariables_ == 6 * 2)
            begin_ivar = 2;
        else if (nvariables_ == 6)
            begin_ivar = 1;
    }

    double chi2 = 0.;
    unsigned ndof = 0;
    for (unsigned ivar=begin_ivar; ivar<(nvariables_ - nparameters_); ++ivar) {
        chi2 += (principals(ivar)/mat.sqrtEigenvalues(ivar))*(principals(ivar)/mat.sqrtEigenvalues(ivar));
        ndof += 1;
    }
    assert(ndof == 3 || ndof == 4 || ndof == 6 || ndof == 8);

    //void setTrackParams(float rinv, float phi0, float cottheta, float z0, float d0,
    //                    float chi2, int ndof, float chi2_phi, float chi2_z)
    atrack.setTrackParams(0.003 * 3.8 * parameters_fit(3), parameters_fit(0), parameters_fit(1), parameters_fit(2), 0.,
                          chi2, ndof, 0., 0.);

    std::vector<float> principals_vec;
    for (unsigned ivar=0; ivar<nvariables_; ++ivar) {
        if (mat.sqrtEigenvalues(ivar) <= 0.)
            principals_vec.push_back(0.);
        else
            principals_vec.push_back(principals(ivar)/mat.sqrtEigenvalues(ivar));
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
