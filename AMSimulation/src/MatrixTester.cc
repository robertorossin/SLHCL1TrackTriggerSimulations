#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/MatrixTester.h"

#include "SLHCL1TrackTriggerSimulations/AMSimulationIO/interface/TTStubReader.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Statistics.h"
#include <iomanip>
#include <fstream>


// _____________________________________________________________________________
int MatrixTester::setupTriggerTower(TString datadir) {
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
int MatrixTester::testMatrices(TString src) {
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
    // Load matrices
    if (fitterPCA_ -> loadConstants(po_.matrixfile)) {
        return 1;
    }
    if (verbose_) {
        std::cout << Info() << "The matrices are: " << std::endl;
        fitterPCA_ -> print();
    }


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

    // Containers
    std::vector<TTTrack2> tracks;
    tracks.reserve(300);

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
        // Start fitting tracks

        tracks.clear();
        int fitstatus = 0;

        unsigned iroad = 0;
        unsigned tower = po_.tower;
        std::vector<unsigned> stubRefs;
        for (unsigned istub=0; istub<nstubs; ++istub) {
            stubRefs.push_back(istub);
        }

        // Loop over reconstructed stubs
        std::vector<TTHit> hits;
        for (unsigned istub=0; istub<stubRefs.size(); ++istub) {
            const unsigned& ref = stubRefs.at(istub);
            hits.emplace_back(TTHit{                // using POD type constructor
                ref,
                reader.vb_r->at(ref),
                reader.vb_phi->at(ref),
                reader.vb_z->at(ref),
                0.,  // FIXME: add errors
                0.,
                0.
            });
        }

        // _____________________________________________________________________
        // Fit
        TTTrack2 atrack;
        atrack.setRoadRef(iroad);
        atrack.setTower(tower);
        atrack.setStubRefs(stubRefs);

        fitstatus = fitterPCA_->fit(hits, atrack);

        tracks.push_back(atrack);

        std::vector<float> principals(nvariables_);
        std::vector<double> parameters(nparameters_);
        std::vector<double> parameters_fit(nparameters_);

        // _____________________________________________________________________
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

            parameters[ipar++] = simPhi;
            parameters[ipar++] = simCotTheta;
            parameters[ipar++] = simVz;
            parameters[ipar++] = simChargeOverPt;
        }

        // _____________________________________________________________________
        // Get fit info
        {
            unsigned ipar = 0;
            parameters_fit[ipar++] = atrack.phi0();
            parameters_fit[ipar++] = atrack.cottheta();
            parameters_fit[ipar++] = atrack.z0();
            parameters_fit[ipar++] = atrack.rinv() / (0.003 * 3.8);

            assert(atrack.principals().size() == nvariables_);
            principals = atrack.principals();
        }


        for (unsigned ivar=0; ivar<nvariables_; ++ivar) {
            statV.at(ivar).fill(principals.at(ivar));
        }

        for (unsigned ipar=0; ipar<nparameters_; ++ipar) {
            statP.at(ipar).fill(parameters_fit.at(ipar) - parameters.at(ipar));
        }

        ++nKept;
        ++nRead;
        keepEvents.push_back(true);
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
// Main driver
int MatrixTester::run() {
    int exitcode = 0;
    Timing(1);

    exitcode = setupTriggerTower(po_.datadir);
    if (exitcode)  return exitcode;
    Timing();

    exitcode = testMatrices(po_.input);
    if (exitcode)  return exitcode;
    Timing();

    return exitcode;
}
