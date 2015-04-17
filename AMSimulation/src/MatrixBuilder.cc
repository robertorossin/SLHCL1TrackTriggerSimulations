#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/MatrixBuilder.h"

#include "SLHCL1TrackTriggerSimulations/AMSimulationIO/interface/TTStubReader.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Statistics.h"

#include "Eigen/Eigenvalues"
#include "Eigen/QR"
#include <iomanip>
#include <fstream>


// _____________________________________________________________________________
int MatrixBuilder::setupTriggerTower(TString datadir) {
    TString csvfile1 = datadir + "trigger_sector_map.csv";
    TString csvfile2 = datadir + "trigger_sector_boundaries.csv";

    try {
        ttmap_ -> readTriggerTowerMap(csvfile1);

    } catch (const std::invalid_argument& e) {
        std::cout << Error() << "Failed to parse: " << csvfile1 << ". What: " << e.what() << std::endl;
        return 1;
    }

    try {
        ttmap_ -> readTriggerTowerBoundaries(csvfile2);

    } catch (const std::invalid_argument& e) {
        std::cout << Error() << "Failed to parse: " << csvfile2 << ". What: " << e.what() << std::endl;
        return 1;
    }

    //ttmap_ -> print();
    return 0;
}

// _____________________________________________________________________________
// Build matrices
int MatrixBuilder::buildMatrices(TString src) {
    if (verbose_)  std::cout << Info() << "Reading " << nEvents_ << " events and generating patterns." << std::endl;

    // _________________________________________________________________________
    // For reading
    TTStubReader reader(verbose_);
    if (reader.init(src, false)) {
        std::cout << Error() << "Failed to initialize TTStubReader." << std::endl;
        return 1;
    }

    // _________________________________________________________________________
    // Get trigger tower reverse map
    const std::map<unsigned, bool>& ttrmap = ttmap_ -> getTriggerTowerReverseMap(po_.tower);


    // _________________________________________________________________________
    // Loop over all events (filter)

    if (verbose_)  std::cout << Info() << "Begin event filtering" << std::endl;

    // Event decisions
    std::vector<bool> keepEvents;

    // Bookkeepers
    long int nRead = 0, nKept = 0;

#if 0
    TString txt = "keepEvents.txt";
    std::ifstream infile(txt.Data());
    if (!infile) {
        std::cout << "Unable to open " << txt << std::endl;
        return 1;
    }
    int x;
    while (infile >> x) {
        long long ievt = nRead;
        if (verbose_>1 && ievt%100000==0)  std::cout << Debug() << Form("... Processing event: %7lld, keeping: %7ld", ievt, nKept) << std::endl;

        keepEvents.push_back(x);

        if (x)
            ++nKept;
        ++nRead;
    }
#else
    for (long long ievt=0; ievt<nEvents_; ++ievt) {
        if (reader.loadTree(ievt) < 0)  break;
        reader.getEntry(ievt);

        const unsigned nstubs = reader.vb_modId->size();
        if (verbose_>1 && ievt%100000==0)  std::cout << Debug() << Form("... Processing event: %7lld, keeping: %7ld", ievt, nKept) << std::endl;
        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " # stubs: " << nstubs << std::endl;

        // Apply track pt requirement
        float simPt = reader.vp_pt->front();
        if (simPt < po_.minPt || po_.maxPt < simPt) {
            ++nRead;
            keepEvents.push_back(false);
            continue;
        }

        // Apply trigger tower acceptance
        unsigned ngoodstubs = 0;
        for (unsigned istub=0; istub<nstubs; ++istub) {
            unsigned moduleId = reader.vb_modId   ->at(istub);
            if (ttrmap.find(moduleId) != ttrmap.end()) {
                ++ngoodstubs;
            }
        }
        if (ngoodstubs != po_.nLayers) {
            ++nRead;
            keepEvents.push_back(false);
            continue;
        }
        assert(nstubs == po_.nLayers);

        ++nKept;
        ++nRead;
        keepEvents.push_back(true);
    }
#endif

    if (nRead == 0) {
        std::cout << Error() << "Failed to read any event." << std::endl;
        return 1;
    }

    if (verbose_)  std::cout << Info() << Form("Read: %7ld, kept: %7ld", nRead, nKept) << std::endl;


    // _________________________________________________________________________
    // Loop over all events

    if (verbose_)  std::cout << Info() << "Begin first loop on tracks" << std::endl;

    // Mean vector and covariance matrix
    Eigen::VectorXd means = Eigen::VectorXd::Zero(nvariables_);
    Eigen::MatrixXd covariances = Eigen::MatrixXd::Zero(nvariables_, nvariables_);

    // Bookkeepers
    nRead = 0, nKept = 0;

    for (long long ievt=0; ievt<nEvents_; ++ievt) {
        if (reader.loadTree(ievt) < 0)  break;
        reader.getEntry(ievt);

        const unsigned nstubs = reader.vb_modId->size();
        if (verbose_>1 && ievt%100000==0)  std::cout << Debug() << Form("... Processing event: %7lld, keeping: %7ld", ievt, nKept) << std::endl;

        if (!keepEvents.at(ievt)) {
            ++nRead;
            continue;
        }

        // _____________________________________________________________________
        // Start building matrices

        Eigen::VectorXd variables1 = Eigen::VectorXd::Zero(nvariables_/2);
        Eigen::VectorXd variables2 = Eigen::VectorXd::Zero(nvariables_/2);

        // Loop over reconstructed stubs
        for (unsigned istub=0; istub<nstubs; ++istub) {
            float    stub_r   = reader.vb_r       ->at(istub);
            float    stub_phi = reader.vb_phi     ->at(istub);
            float    stub_z   = reader.vb_z       ->at(istub);

            variables1(istub) = stub_phi;
            variables2(istub) = stub_z;
        }

        Eigen::VectorXd variables = Eigen::VectorXd::Zero(nvariables_);
        variables << variables1, variables2;

        // Update mean vector
        long int nTracks = nKept + 1;
        for (unsigned ivar=0; ivar<nvariables_; ++ivar) {
            means(ivar) += (variables(ivar) - means(ivar))/nTracks;
        }

        // Update covariance matrix
        if (nTracks > 1) {
            for (unsigned ivar=0; ivar<nvariables_; ++ivar) {
                for (unsigned jvar=0; jvar<nvariables_; ++jvar) {
                    covariances(ivar, jvar) += (variables(ivar) - means(ivar)) * (variables(jvar) - means(jvar)) / (nTracks-1) - covariances(ivar, jvar)/nTracks;
                }
            }
        }

        ++nKept;
        ++nRead;
        keepEvents.push_back(true);
    }

    if (verbose_>1) {
        std::cout << Info() << "means: " << std::endl;
        std::cout << means << std::endl << std::endl;
        std::cout << Info() << "covariances: " << std::endl;
        std::cout << covariances << std::endl << std::endl;
    }

    // Find eigenvectors of covariance matrix
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eigensolver(covariances);
    sqrtEigenvalues_ = Eigen::VectorXd::Zero(nvariables_);
    sqrtEigenvalues_ = eigensolver.eigenvalues();
    for (unsigned ivar=0; ivar<nvariables_; ++ivar) {
        sqrtEigenvalues_(ivar) = std::sqrt(sqrtEigenvalues_(ivar));  // take the square root
    }

    // Find matrix V
    // V is the orthogonal transformation from coordinates space to principal components space
    // The principal components are constraints + rotated track parameters
    V_ = Eigen::MatrixXd::Zero(nvariables_, nvariables_);
    V_ = (eigensolver.eigenvectors()).transpose();

    if (verbose_>1) {
        std::cout << Info() << "sqrt(eigenvalues) of covariances: " << std::endl;
        std::cout << sqrtEigenvalues_ << std::endl << std::endl;
        std::cout << Info() << "eigenvectors^T: " << std::endl;
        std::cout << V_ << std::endl << std::endl;
    }


    // _________________________________________________________________________
    // Loop over all events again

    if (verbose_)  std::cout << Info() << "Begin second loop on tracks" << std::endl;

    // Mean vector and covariance matrix for principal components and track parameters
    meansV_ = Eigen::VectorXd::Zero(nvariables_);
    meansP_ = Eigen::VectorXd::Zero(nparameters_);
    Eigen::MatrixXd covariancesV = Eigen::MatrixXd::Zero(nvariables_, nvariables_);
    Eigen::MatrixXd covariancesPV = Eigen::MatrixXd::Zero(nparameters_, nvariables_);

    // Bookkeepers
    nRead = 0, nKept = 0;

    for (long long ievt=0; ievt<nEvents_; ++ievt) {
        if (reader.loadTree(ievt) < 0)  break;
        reader.getEntry(ievt);

        const unsigned nstubs = reader.vb_modId->size();
        if (verbose_>1 && ievt%100000==0)  std::cout << Debug() << Form("... Processing event: %7lld, keeping: %7ld", ievt, nKept) << std::endl;

        if (!keepEvents.at(ievt)) {
            ++nRead;
            continue;
        }

        // _____________________________________________________________________
        // Start building matrices

        Eigen::VectorXd variables1 = Eigen::VectorXd::Zero(nvariables_/2);
        Eigen::VectorXd variables2 = Eigen::VectorXd::Zero(nvariables_/2);

        // Loop over reconstructed stubs
        for (unsigned istub=0; istub<nstubs; ++istub) {
            float    stub_r   = reader.vb_r       ->at(istub);
            float    stub_phi = reader.vb_phi     ->at(istub);
            float    stub_z   = reader.vb_z       ->at(istub);

            variables1(istub) = stub_phi;
            variables2(istub) = stub_z;
        }

        Eigen::VectorXd variables = Eigen::VectorXd::Zero(nvariables_);
        variables << variables1, variables2;

        Eigen::VectorXd parameters = Eigen::VectorXd::Zero(nparameters_);

        // Get sim info
        {
            unsigned ipar = 0;
            float simPt           = reader.vp_pt->front();
            float simEta          = reader.vp_eta->front();
            float simPhi          = reader.vp_phi->front();
            //float simVx           = reader.vp_vx->front();
            //float simVy           = reader.vp_vy->front();
            float simVz           = reader.vp_vz->front();
            int   simCharge       = reader.vp_charge->front();

            float simCotTheta     = std::sinh(simEta);
            float simChargeOverPt = float(simCharge)/simPt;

            parameters(ipar++) = simPhi;
            parameters(ipar++) = simCotTheta;
            parameters(ipar++) = simVz;
            parameters(ipar++) = simChargeOverPt;
        }

        // Transform coordinates to principal components
        Eigen::VectorXd principals = Eigen::VectorXd::Zero(nvariables_);
        principals = V_ * (variables - means);

        // Update mean vectors
        long int nTracks = nKept + 1;
        for (unsigned ivar=0; ivar<nvariables_; ++ivar) {
            meansV_(ivar) += (principals(ivar) - meansV_(ivar))/nTracks;
        }

        for (unsigned ipar=0; ipar<nparameters_; ++ipar) {
            meansP_(ipar) += (parameters(ipar) - meansP_(ipar))/nTracks;
        }

        // Update covariance matrix
        if (nTracks > 1) {
            for (unsigned ivar=0; ivar<nvariables_; ++ivar) {
                for (unsigned jvar=0; jvar<nvariables_; ++jvar) {
                    covariancesV(ivar, jvar) += (principals(ivar) - meansV_(ivar)) * (principals(jvar) - meansV_(jvar)) / (nTracks-1) - covariancesV(ivar, jvar)/nTracks;
                }
            }

            for (unsigned ipar=0; ipar<nparameters_; ++ipar) {
                for (unsigned jvar=0; jvar<nvariables_; ++jvar) {
                    covariancesPV(ipar, jvar) += (parameters(ipar) - meansP_(ipar)) * (principals(jvar) - meansV_(jvar)) / (nTracks-1) - covariancesPV(ipar, jvar)/nTracks;
                }
            }
        }

        ++nKept;
        ++nRead;
    }

    if (verbose_>1) {
        std::cout << Info() << "meansV: " << std::endl;
        std::cout << meansV_ << std::endl << std::endl;
        std::cout << Info() << "meansP: " << std::endl;
        std::cout << meansP_ << std::endl << std::endl;
        std::cout << Info() << "covariancesV: " << std::endl;
        std::cout << covariancesV << std::endl << std::endl;
        std::cout << Info() << "covariancesPV: " << std::endl;
        std::cout << covariancesPV << std::endl << std::endl;
    }

    // Find matrix D
    // D is the transformation from principal components to track parameters
    D_ = Eigen::MatrixXd::Zero(nparameters_, nvariables_);
    //D_ = covariancesPV * covariancesV.inverse();
    D_ = (covariancesV.colPivHouseholderQr().solve(covariancesPV.transpose())).transpose();

    DV_ = Eigen::MatrixXd::Zero(nparameters_, nvariables_);
    DV_ = D_ * V_;

    if (verbose_>1) {
        std::cout << Info() << "covariancesPV * covariancesV^{-1}: " << std::endl;
        std::cout << D_ << std::endl << std::endl;
        std::cout << Info() << "covariancesPV * covariancesV^{-1} * eigenvectors^T: " << std::endl;
        std::cout << DV_ << std::endl << std::endl;
    }


    // _________________________________________________________________________
    // Loop over all events again

    if (verbose_)  std::cout << Info() << "Begin third loop on tracks" << std::endl;

    // Get mean values again
    meansV_ = Eigen::VectorXd::Zero(nvariables_);
    meansP_ = Eigen::VectorXd::Zero(nparameters_);

    // Statistics
    std::vector<Statistics> statV(nvariables_);
    std::vector<Statistics> statP(nparameters_);

    // Bookkeepers
    nRead = 0, nKept = 0;

    for (long long ievt=0; ievt<nEvents_; ++ievt) {
        if (reader.loadTree(ievt) < 0)  break;
        reader.getEntry(ievt);

        const unsigned nstubs = reader.vb_modId->size();
        if (verbose_>1 && ievt%100000==0)  std::cout << Debug() << Form("... Processing event: %7lld, keeping: %7ld", ievt, nKept) << std::endl;

        if (!keepEvents.at(ievt)) {
            ++nRead;
            continue;
        }

        // _____________________________________________________________________
        // Start collecting statistics

        Eigen::VectorXd variables1 = Eigen::VectorXd::Zero(nvariables_/2);
        Eigen::VectorXd variables2 = Eigen::VectorXd::Zero(nvariables_/2);

        // Loop over reconstructed stubs
        for (unsigned istub=0; istub<nstubs; ++istub) {
            float    stub_r   = reader.vb_r       ->at(istub);
            float    stub_phi = reader.vb_phi     ->at(istub);
            float    stub_z   = reader.vb_z       ->at(istub);

            variables1(istub) = stub_phi;
            variables2(istub) = stub_z;
        }

        Eigen::VectorXd variables = Eigen::VectorXd::Zero(nvariables_);
        variables << variables1, variables2;

        Eigen::VectorXd parameters = Eigen::VectorXd::Zero(nparameters_);

        // Get sim info
        {
            unsigned ipar = 0;
            float simPt           = reader.vp_pt->front();
            float simEta          = reader.vp_eta->front();
            float simPhi          = reader.vp_phi->front();
            //float simVx           = reader.vp_vx->front();
            //float simVy           = reader.vp_vy->front();
            float simVz           = reader.vp_vz->front();
            int   simCharge       = reader.vp_charge->front();

            float simCotTheta     = std::sinh(simEta);
            float simChargeOverPt = float(simCharge)/simPt;

            parameters(ipar++) = simPhi;
            parameters(ipar++) = simCotTheta;
            parameters(ipar++) = simVz;
            parameters(ipar++) = simChargeOverPt;
        }

        Eigen::VectorXd principals = Eigen::VectorXd::Zero(nvariables_);
        principals = V_ * variables;

        Eigen::VectorXd parameters_fit = Eigen::VectorXd::Zero(nparameters_);
        parameters_fit = DV_ * variables;

        // Update mean vectors
        long int nTracks = nKept + 1;
        for (unsigned ivar=0; ivar<nvariables_; ++ivar) {
            meansV_(ivar) += (principals(ivar) - meansV_(ivar))/nTracks;
        }

        for (unsigned ipar=0; ipar<nparameters_; ++ipar) {
            meansP_(ipar) += (parameters_fit(ipar) - meansP_(ipar))/nTracks;
        }

        // Collect statistics
        for (unsigned ivar=0; ivar<nvariables_; ++ivar) {
            statV.at(ivar).fill(principals(ivar));
        }

        for (unsigned ipar=0; ipar<nparameters_; ++ipar) {
            statP.at(ipar).fill(parameters_fit(ipar) - parameters(ipar));
        }

        ++nKept;
        ++nRead;
    }

    if (verbose_>1) {
        std::ios::fmtflags flags = std::cout.flags();
        std::cout << std::setprecision(4);
        std::cout << Info() << "statV: " << std::endl;
        for (unsigned ivar=0; ivar<nvariables_; ++ivar) {
            std::cout << "principal " << ivar << ": " << statV.at(ivar).getEntries() << " " << statV.at(ivar).getMean() << " " << statV.at(ivar).getSigma() << std::endl;
        }
        std::cout << Info() << "statP: " << std::endl;
        for (unsigned ipar=0; ipar<nparameters_; ++ipar) {
            std::cout << "parameter " << ipar << ": " << statP.at(ipar).getEntries() << " " << statP.at(ipar).getMean() << " " << statP.at(ipar).getSigma() << std::endl;
        }
        std::cout.flags(flags);
    }

    return 0;
}


// _____________________________________________________________________________
// Write matrices
int MatrixBuilder::writeMatrices(TString out) {
    std::ofstream outfile(out.Data());
    if (!outfile) {
        std::cout << Error() << "Unable to open " << out << std::endl;
        return 1;
    }

    if (verbose_) {
        std::cout << Info() << "The matrices are: " << std::endl;
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

    outfile << V_;
    outfile << std::endl << std::endl;
    outfile << D_;
    outfile << std::endl << std::endl;
    outfile << meansV_;
    outfile << std::endl << std::endl;
    outfile << meansP_;
    outfile << std::endl << std::endl;
    outfile << sqrtEigenvalues_;
    outfile << std::endl << std::endl;
    outfile.close();

    return 0;
}


// _____________________________________________________________________________
// Main driver
int MatrixBuilder::run() {
    int exitcode = 0;
    Timing(1);

    exitcode = setupTriggerTower(po_.datadir);
    if (exitcode)  return exitcode;
    Timing();

    exitcode = buildMatrices(po_.input);
    if (exitcode)  return exitcode;
    Timing();

    exitcode = writeMatrices(po_.output);
    if (exitcode)  return exitcode;
    Timing();

    return exitcode;
}
