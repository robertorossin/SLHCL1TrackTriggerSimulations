#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/MatrixBuilder.h"

#include "SLHCL1TrackTriggerSimulations/AMSimulationIO/interface/TTStubReader.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Statistics.h"

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
int MatrixBuilder::bookHistograms() {
    TH1::AddDirectory(kFALSE);
    TString hname;
    for (unsigned ivar=0; ivar<nvariables_; ++ivar) {
        hname = Form("var%i", ivar);
        histograms_[hname] = new TH1F(hname, ";"+hname, 1000, -1., 1.);

        hname = Form("pc%i", ivar);
        histograms_[hname] = new TH1F(hname, ";"+hname, 1000, -1., 1.);
    }

    for (unsigned ipar=0; ipar<nparameters_; ++ipar) {
        hname = Form("par%i", ipar);
        histograms_[hname] = new TH1F(hname, ";"+hname, 1000, -1., 1.);

        hname = Form("fitpar%i", ipar);
        histograms_[hname] = new TH1F(hname, ";"+hname, 1000, -1., 1.);

        hname = Form("errpar%i", ipar);
        histograms_[hname] = new TH1F(hname, ";"+hname, 1000, -0.1, 0.1);
    }

    // Manually adjust binning
    histograms_["var0"]   ->SetBins(1000, 0., 2.4);
    histograms_["var1"]   ->SetBins(1000, 0., 2.4);
    histograms_["var2"]   ->SetBins(1000, 0., 2.4);
    histograms_["var3"]   ->SetBins(1000, 0., 2.4);
    histograms_["var4"]   ->SetBins(1000, 0., 2.4);
    histograms_["var5"]   ->SetBins(1000, 0., 2.4);
    histograms_["var6"]   ->SetBins(1000, -100., 200.);
    histograms_["var7"]   ->SetBins(1000, -100., 200.);
    histograms_["var8"]   ->SetBins(1000, -100., 200.);
    histograms_["var9"]   ->SetBins(1000, -100., 200.);
    histograms_["var10"]  ->SetBins(1000, -100., 200.);
    histograms_["var11"]  ->SetBins(1000, -100., 200.);

    histograms_["pc0"]    ->SetBins(1000, -0.005, 0.005);
    histograms_["pc1"]    ->SetBins(1000, -0.005, 0.005);
    histograms_["pc2"]    ->SetBins(1000, -0.005, 0.005);
    histograms_["pc3"]    ->SetBins(1000, -0.005, 0.005);
    histograms_["pc4"]    ->SetBins(1000, -0.25, 0.25);
    histograms_["pc5"]    ->SetBins(1000, -0.25, 0.25);
    histograms_["pc6"]    ->SetBins(1000, -2.5, 2.5);
    histograms_["pc7"]    ->SetBins(1000, -1., 7.);
    histograms_["pc8"]    ->SetBins(1000, -8., 8.);
    histograms_["pc9"]    ->SetBins(1000, -8., 8.);
    histograms_["pc10"]   ->SetBins(1000, -25., 25.);
    histograms_["pc11"]   ->SetBins(1000, -50., 200.);

    histograms_["par0"]   ->SetBins(1000, 0.6, 1.8);
    histograms_["par1"]   ->SetBins(1000, -0.1, 0.9);
    histograms_["par2"]   ->SetBins(1000, -25., 25.);
    histograms_["par3"]   ->SetBins(1000, -0.6, 0.6);

    histograms_["fitpar0"]->SetBins(1000, 0.6, 1.8);
    histograms_["fitpar1"]->SetBins(1000, -0.1, 0.9);
    histograms_["fitpar2"]->SetBins(1000, -25., 25.);
    histograms_["fitpar3"]->SetBins(1000, -0.6, 0.6);

    histograms_["errpar0"]->SetBins(1000, -0.005, 0.005);
    histograms_["errpar1"]->SetBins(1000, -0.02, 0.02);
    histograms_["errpar2"]->SetBins(1000, -0.5, 0.5);
    histograms_["errpar3"]->SetBins(1000, -0.02, 0.02);

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

//#define Use_keepEvents_txt_
#ifdef Use_keepEvents_txt_
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
    // Loop over all events (first loop)

    if (verbose_)  std::cout << Info() << "Begin first loop on tracks" << std::endl;

    // Mean vector and covariance matrix
    Eigen::VectorXd means = Eigen::VectorXd::Zero(nvariables_);
    Eigen::MatrixXd covariances = Eigen::MatrixXd::Zero(nvariables_, nvariables_);

    Eigen::VectorXd meansC = Eigen::VectorXd::Zero(1);
    Eigen::MatrixXd covariancesC = Eigen::MatrixXd::Zero(1, nvariables_/2);

    Eigen::VectorXd meansT = Eigen::VectorXd::Zero(1);
    Eigen::MatrixXd covariancesT = Eigen::MatrixXd::Zero(1, nvariables_/2);

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
        // Calculate means and covariances for C&T

        Eigen::VectorXd variables1 = Eigen::VectorXd::Zero(nvariables_/2);
        Eigen::VectorXd variables2 = Eigen::VectorXd::Zero(nvariables_/2);

        // Loop over reconstructed stubs
        for (unsigned istub=0; istub<nstubs; ++istub) {
            //float    stub_r   = reader.vb_r       ->at(istub);
            float    stub_phi = reader.vb_phi     ->at(istub);
            float    stub_z   = reader.vb_z       ->at(istub);

            variables1(istub) = stub_phi;
            variables2(istub) = stub_z;
        }

        Eigen::VectorXd variables = Eigen::VectorXd::Zero(nvariables_);
        variables << variables1, variables2;

        // Get sim info
        double simCotTheta     = std::sinh(reader.vp_eta->front());
        double simChargeOverPt = float(reader.vp_charge->front())/reader.vp_pt->front();
        //double simPhi          = reader.vp_phi->front();
        //double simVz           = reader.vp_vz->front();
        double simC = -0.5 * (0.003 * 3.8 * simChargeOverPt);  // 1/(2 x radius of curvature)
        double simT = simCotTheta;

        // Update mean vectors
        long int nTracks = nKept + 1;
        for (unsigned ivar=0; ivar<nvariables_; ++ivar) {
            means(ivar) += (variables(ivar) - means(ivar))/nTracks;
        }

        meansC(0) += (simC - meansC(0))/nTracks;
        meansT(0) += (simT - meansT(0))/nTracks;

        // Update covariance matrices
        if (nTracks > 1) {
            for (unsigned ivar=0; ivar<nvariables_; ++ivar) {
                for (unsigned jvar=0; jvar<nvariables_; ++jvar) {
                    covariances(ivar, jvar) += (variables(ivar) - means(ivar)) * (variables(jvar) - means(jvar)) / (nTracks-1) - covariances(ivar, jvar)/nTracks;
                }
            }

            for (unsigned ipar=0; ipar<1; ++ipar) {
                for (unsigned jvar=0; jvar<nvariables_/2; ++jvar) {
                    unsigned kvar = jvar;
                    covariancesC(ipar, jvar) += (simC - meansC(ipar)) * (variables(kvar) - means(kvar)) / (nTracks-1) - covariancesC(ipar, jvar)/nTracks;

                    kvar = jvar + nvariables_/2;
                    covariancesT(ipar, jvar) += (simT - meansT(ipar)) * (variables(kvar) - means(kvar)) / (nTracks-1) - covariancesT(ipar, jvar)/nTracks;
                }
            }
        }

        ++nKept;
        ++nRead;
    }

    // Find solutions for C & T
    Eigen::MatrixXd covariances_phi = covariances.block(0,0,nvariables_/2,nvariables_/2);
    Eigen::MatrixXd solutionsC = Eigen::MatrixXd::Zero(1,nvariables_/2);
    //solutionsC = covariancesC*(covariances_phi.inverse());
    solutionsC = (covariances_phi.colPivHouseholderQr().solve(covariancesC.transpose())).transpose();

    Eigen::MatrixXd covariances_z = covariances.block(nvariables_/2,nvariables_/2,nvariables_/2,nvariables_/2);
    Eigen::MatrixXd solutionsT = Eigen::MatrixXd::Zero(1,nvariables_/2);
    //solutionsT = covariancesT*(covariances_z.inverse());
    solutionsT = (covariances_z.colPivHouseholderQr().solve(covariancesT.transpose())).transpose();

    if (verbose_>1) {
        std::cout << Info() << "means: " << std::endl;
        std::cout << means << std::endl << std::endl;
        std::cout << Info() << "covariances: " << std::endl;
        std::cout << covariances << std::endl << std::endl;
        //std::cout << Info() << "covariances_phi: " << std::endl;
        //std::cout << covariances_phi << std::endl << std::endl;
        //std::cout << Info() << "covariances_z: " << std::endl;
        //std::cout << covariances_z << std::endl << std::endl;

        std::cout << Info() << "meansC: " << std::endl;
        std::cout << meansC << std::endl << std::endl;
        std::cout << Info() << "covariancesC: " << std::endl;
        std::cout << covariancesC << std::endl << std::endl;
        std::cout << Info() << "solutionsC: " << std::endl;
        std::cout << solutionsC << std::endl << std::endl;
        std::cout << Info() << "meansT: " << std::endl;
        std::cout << meansT << std::endl << std::endl;
        std::cout << Info() << "covariancesT: " << std::endl;
        std::cout << covariancesT << std::endl << std::endl;
        std::cout << Info() << "solutionsT: " << std::endl;
        std::cout << solutionsT << std::endl << std::endl;
    }


    // _________________________________________________________________________
    // Loop over all events (second loop)

    if (verbose_)  std::cout << Info() << "Begin second loop on tracks" << std::endl;

    // Mean vector and covariance matrix
    means = Eigen::VectorXd::Zero(nvariables_);  // reset
    covariances = Eigen::MatrixXd::Zero(nvariables_, nvariables_);  // reset

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
        // Calculate means and covariances for V

        Eigen::VectorXd variables1 = Eigen::VectorXd::Zero(nvariables_/2);
        Eigen::VectorXd variables2 = Eigen::VectorXd::Zero(nvariables_/2);
        Eigen::VectorXd variables3 = Eigen::VectorXd::Zero(nvariables_/2);

        // Loop over reconstructed stubs
        for (unsigned istub=0; istub<nstubs; ++istub) {
            float    stub_r   = reader.vb_r       ->at(istub);
            float    stub_phi = reader.vb_phi     ->at(istub);
            float    stub_z   = reader.vb_z       ->at(istub);

            variables1(istub) = stub_phi;
            variables2(istub) = stub_z;
            variables3(istub) = meansR_(istub) - stub_r;
        }

        variables1 += ((solutionsC * variables1)(0,0)) * variables3;
        variables2 += ((solutionsT * variables2)(0,0)) * variables3;

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
    }

    if (verbose_>1) {
        std::cout << Info() << "means: " << std::endl;
        std::cout << means << std::endl << std::endl;
        std::cout << Info() << "covariances: " << std::endl;
        std::cout << covariances << std::endl << std::endl;
    }

    // Find eigenvectors of covariance matrix
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eigensolver(covariances);
    Eigen::VectorXd sqrtEigenvalues = Eigen::VectorXd::Zero(nvariables_);
    sqrtEigenvalues = eigensolver.eigenvalues();
    for (unsigned ivar=0; ivar<nvariables_; ++ivar) {
        sqrtEigenvalues(ivar) = std::sqrt(sqrtEigenvalues(ivar));  // take the square root
    }

    // Find matrix V
    // V is the orthogonal transformation from coordinates space to principal components space
    // The principal components are constraints + rotated track parameters
    Eigen::MatrixXd V = Eigen::MatrixXd::Zero(nvariables_, nvariables_);
    V = (eigensolver.eigenvectors()).transpose();

    if (verbose_>1) {
        std::cout << Info() << "sqrt(eigenvalues) of covariances: " << std::endl;
        std::cout << sqrtEigenvalues << std::endl << std::endl;
        std::cout << Info() << "eigenvectors^T: " << std::endl;
        std::cout << V << std::endl << std::endl;
    }


    // _________________________________________________________________________
    // Loop over all events (third loop)

    if (verbose_)  std::cout << Info() << "Begin third loop on tracks" << std::endl;

    // Mean vector and covariance matrix for principal components and track parameters
    Eigen::VectorXd meansV = Eigen::VectorXd::Zero(nvariables_);
    Eigen::MatrixXd covariancesV = Eigen::MatrixXd::Zero(nvariables_, nvariables_);

    Eigen::VectorXd meansP = Eigen::VectorXd::Zero(nparameters_);
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
        // Calculate V covairance matrix and PV correlation matrix

        Eigen::VectorXd variables1 = Eigen::VectorXd::Zero(nvariables_/2);
        Eigen::VectorXd variables2 = Eigen::VectorXd::Zero(nvariables_/2);
        Eigen::VectorXd variables3 = Eigen::VectorXd::Zero(nvariables_/2);

        // Loop over reconstructed stubs
        for (unsigned istub=0; istub<nstubs; ++istub) {
            float    stub_r   = reader.vb_r       ->at(istub);
            float    stub_phi = reader.vb_phi     ->at(istub);
            float    stub_z   = reader.vb_z       ->at(istub);

            variables1(istub) = stub_phi;
            variables2(istub) = stub_z;
            variables3(istub) = meansR_(istub) - stub_r;
        }

        variables1 += ((solutionsC * variables1)(0,0)) * variables3;
        variables2 += ((solutionsT * variables2)(0,0)) * variables3;

        Eigen::VectorXd variables = Eigen::VectorXd::Zero(nvariables_);
        variables << variables1, variables2;

        Eigen::VectorXd parameters = Eigen::VectorXd::Zero(nparameters_);

        // Get sim info
        double simCotTheta     = std::sinh(reader.vp_eta->front());
        double simChargeOverPt = float(reader.vp_charge->front())/reader.vp_pt->front();
        double simPhi          = reader.vp_phi->front();
        double simVz           = reader.vp_vz->front();
        //double simC = -0.5 * (0.003 * 3.8 * simChargeOverPt);  // 1/(2 x radius of curvature)
        //double simT = simCotTheta;
        {
            unsigned ipar = 0;
            parameters(ipar++) = simPhi;
            parameters(ipar++) = simCotTheta;
            parameters(ipar++) = simVz;
            parameters(ipar++) = simChargeOverPt;
        }

        // Transform coordinates to principal components
        Eigen::VectorXd principals = Eigen::VectorXd::Zero(nvariables_);
        principals = V * (variables - means);

        // Update mean vectors
        long int nTracks = nKept + 1;
        for (unsigned ivar=0; ivar<nvariables_; ++ivar) {
            meansV(ivar) += (principals(ivar) - meansV(ivar))/nTracks;
        }

        for (unsigned ipar=0; ipar<nparameters_; ++ipar) {
            meansP(ipar) += (parameters(ipar) - meansP(ipar))/nTracks;
        }

        // Update covariance matrix
        if (nTracks > 1) {
            for (unsigned ivar=0; ivar<nvariables_; ++ivar) {
                for (unsigned jvar=0; jvar<nvariables_; ++jvar) {
                    covariancesV(ivar, jvar) += (principals(ivar) - meansV(ivar)) * (principals(jvar) - meansV(jvar)) / (nTracks-1) - covariancesV(ivar, jvar)/nTracks;
                }
            }

            for (unsigned ipar=0; ipar<nparameters_; ++ipar) {
                for (unsigned jvar=0; jvar<nvariables_; ++jvar) {
                    covariancesPV(ipar, jvar) += (parameters(ipar) - meansP(ipar)) * (principals(jvar) - meansV(jvar)) / (nTracks-1) - covariancesPV(ipar, jvar)/nTracks;
                }
            }
        }

        ++nKept;
        ++nRead;
    }

    if (verbose_>1) {
        std::cout << Info() << "meansV: " << std::endl;
        std::cout << meansV << std::endl << std::endl;
        std::cout << Info() << "meansP: " << std::endl;
        std::cout << meansP << std::endl << std::endl;
        std::cout << Info() << "covariancesV: " << std::endl;
        std::cout << covariancesV << std::endl << std::endl;
        std::cout << Info() << "covariancesPV: " << std::endl;
        std::cout << covariancesPV << std::endl << std::endl;
    }

    // Find matrix D
    // D is the transformation from principal components to track parameters
    Eigen::MatrixXd D = Eigen::MatrixXd::Zero(nparameters_, nvariables_);
    //D = covariancesPV * covariancesV.inverse();
    D = (covariancesV.colPivHouseholderQr().solve(covariancesPV.transpose())).transpose();

    Eigen::MatrixXd DV = Eigen::MatrixXd::Zero(nparameters_, nvariables_);
    DV = D * V;

    if (verbose_>1) {
        std::cout << Info() << "covariancesPV * covariancesV^{-1}: " << std::endl;
        std::cout << D << std::endl << std::endl;
        std::cout << Info() << "covariancesPV * covariancesV^{-1} * eigenvectors^T: " << std::endl;
        std::cout << DV << std::endl << std::endl;
    }


    // _________________________________________________________________________
    // Loop over all events (fourth loop)

    if (verbose_)  std::cout << Info() << "Begin fourth loop on tracks" << std::endl;

    // Get mean values again
    meansV = Eigen::VectorXd::Zero(nvariables_);
    meansP = Eigen::VectorXd::Zero(nparameters_);

    // Statistics
    std::vector<Statistics> statCT(2);
    std::vector<Statistics> statX(nvariables_);
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
        Eigen::VectorXd variables3 = Eigen::VectorXd::Zero(nvariables_/2);

        // Loop over reconstructed stubs
        for (unsigned istub=0; istub<nstubs; ++istub) {
            float    stub_r   = reader.vb_r       ->at(istub);
            float    stub_phi = reader.vb_phi     ->at(istub);
            float    stub_z   = reader.vb_z       ->at(istub);

            variables1(istub) = stub_phi;
            variables2(istub) = stub_z;
            variables3(istub) = meansR_(istub) - stub_r;
        }

        variables1 += ((solutionsC * variables1)(0,0)) * variables3;
        variables2 += ((solutionsT * variables2)(0,0)) * variables3;

        Eigen::VectorXd variables = Eigen::VectorXd::Zero(nvariables_);
        variables << variables1, variables2;

        Eigen::VectorXd parameters = Eigen::VectorXd::Zero(nparameters_);

        // Get sim info
        double simCotTheta     = std::sinh(reader.vp_eta->front());
        double simChargeOverPt = float(reader.vp_charge->front())/reader.vp_pt->front();
        double simPhi          = reader.vp_phi->front();
        double simVz           = reader.vp_vz->front();
        double simC = -0.5 * (0.003 * 3.8 * simChargeOverPt);  // 1/(2 x radius of curvature)
        double simT = simCotTheta;
        {
            unsigned ipar = 0;
            parameters(ipar++) = simPhi;
            parameters(ipar++) = simCotTheta;
            parameters(ipar++) = simVz;
            parameters(ipar++) = simChargeOverPt;
        }

        Eigen::VectorXd principals = Eigen::VectorXd::Zero(nvariables_);
        principals = V * variables;

        Eigen::VectorXd parameters_fit = Eigen::VectorXd::Zero(nparameters_);
        parameters_fit = DV * variables;

        // Update mean vectors
        long int nTracks = nKept + 1;
        for (unsigned ivar=0; ivar<nvariables_; ++ivar) {
            meansV(ivar) += (principals(ivar) - meansV(ivar))/nTracks;
        }

        for (unsigned ipar=0; ipar<nparameters_; ++ipar) {
            meansP(ipar) += (parameters_fit(ipar) - parameters(ipar) - meansP(ipar))/nTracks;
        }

        // Collect statistics and fill histograms
        statCT.at(0).fill(((solutionsC * variables1)(0,0)) - simC);
        statCT.at(1).fill(((solutionsT * variables2)(0,0)) - simT);

        TString hname;
        for (unsigned ivar=0; ivar<nvariables_; ++ivar) {
            statX.at(ivar).fill(variables(ivar));
            statV.at(ivar).fill(principals(ivar));

            hname = Form("var%i", ivar);
            histograms_[hname]->Fill(variables(ivar));

            hname = Form("pc%i", ivar);
            histograms_[hname]->Fill(principals(ivar));
        }

        for (unsigned ipar=0; ipar<nparameters_; ++ipar) {
            statP.at(ipar).fill(parameters_fit(ipar) - parameters(ipar));

            hname = Form("par%i", ipar);
            histograms_[hname]->Fill(parameters(ipar));

            hname = Form("fitpar%i", ipar);
            histograms_[hname]->Fill(parameters_fit(ipar));

            hname = Form("errpar%i", ipar);
            histograms_[hname]->Fill(parameters_fit(ipar) - parameters(ipar));
        }

        ++nKept;
        ++nRead;
    }

    if (verbose_>1) {
        std::ios::fmtflags flags = std::cout.flags();
        std::cout << std::setprecision(4);
        std::cout << Info() << "statX: " << std::endl;
        for (unsigned ivar=0; ivar<nvariables_; ++ivar) {
            std::cout << "variable " << ivar << ": " << statX.at(ivar).getEntries() << " " << statX.at(ivar).getMean() << " " << statX.at(ivar).getSigma() << std::endl;
        }
        std::cout << Info() << "statV: " << std::endl;
        for (unsigned ivar=0; ivar<nvariables_; ++ivar) {
            std::cout << "principal " << ivar << ": " << statV.at(ivar).getEntries() << " " << statV.at(ivar).getMean() << " " << statV.at(ivar).getSigma() << std::endl;
        }
        std::cout << Info() << "statCT: " << std::endl;
        for (unsigned ivar=0; ivar<2; ++ivar) {
            std::cout << "parameter " << ivar << ": " << statCT.at(ivar).getEntries() << " " << statCT.at(ivar).getMean() << " " << statCT.at(ivar).getSigma() << std::endl;
        }
        std::cout << Info() << "statP: " << std::endl;
        for (unsigned ipar=0; ipar<nparameters_; ++ipar) {
            std::cout << "parameter " << ipar << ": " << statP.at(ipar).getEntries() << " " << statP.at(ipar).getMean() << " " << statP.at(ipar).getSigma() << std::endl;
        }
        std::cout.flags(flags);
    }

    // Set PCAMatrix
    mat_.meansR = meansR_;
    mat_.meansC = meansC;
    mat_.meansT = meansT;
    mat_.solutionsC = solutionsC;
    mat_.solutionsT = solutionsT;
    mat_.sqrtEigenvalues = sqrtEigenvalues;
    mat_.meansV = meansV;
    mat_.meansP = meansP;
    mat_.V = V;
    mat_.D = D;
    mat_.DV = DV;

    if (verbose_) {
        std::cout << Info() << "The matrices are:" << std::endl;
        mat_.print();
    }

    return 0;
}


// _____________________________________________________________________________
// Write matrices
int MatrixBuilder::writeMatrices(TString out) {
    mat_.write(out.Data());

    return 0;
}


// _____________________________________________________________________________
// Write matrices
int MatrixBuilder::writeHistograms(TString out) {
    TFile* outfile = TFile::Open(out.ReplaceAll(".txt",".root"), "RECREATE");
    for (std::map<TString, TH1F *>::const_iterator it=histograms_.begin();
         it!=histograms_.end(); ++it) {
        if (it->second)  it->second->SetDirectory(gDirectory);
    }
    outfile->Write();
    outfile->Close();

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

    exitcode = bookHistograms();
    if (exitcode)  return exitcode;
    Timing();

    exitcode = buildMatrices(po_.input);
    if (exitcode)  return exitcode;
    Timing();

    exitcode = writeMatrices(po_.output);
    if (exitcode)  return exitcode;
    Timing();

    exitcode = writeHistograms(po_.output);
    if (exitcode)  return exitcode;
    Timing();

    return exitcode;
}
