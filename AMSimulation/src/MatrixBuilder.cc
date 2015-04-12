#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/MatrixBuilder.h"

#include "SLHCL1TrackTriggerSimulations/AMSimulationIO/interface/TTStubReader.h"

static const unsigned MIN_NGOODSTUBS = 3;
static const unsigned MAX_NGOODSTUBS = 8;


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
    // Loop over all events

    // Bookkeepers
    long int nRead = 0, nKept = 0;

    for (long long ievt=0; ievt<nEvents_; ++ievt) {
        if (reader.loadTree(ievt) < 0)  break;
        reader.getEntry(ievt);

        const unsigned nstubs = reader.vb_modId->size();
        if (verbose_>1 && ievt%100000==0)  std::cout << Debug() << Form("... Processing event: %7lld, keeping: %7ld", ievt, nKept) << std::endl;
        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " # stubs: " << nstubs << std::endl;

        if (nstubs < MIN_NGOODSTUBS) {  // skip if not enough stubs
            ++nRead;
            continue;
        }

        if (nstubs > MAX_NGOODSTUBS) {
            std::cout << Error() << "Too many stubs: " << nstubs << std::endl;
            return 1;
        }

        // Apply track pt requirement
        float simPt = reader.vp_pt->front();
        if (simPt < po_.minPt || po_.maxPt < simPt) {
            ++nRead;
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
            continue;
        }
        assert(nstubs == po_.nLayers);


        // _____________________________________________________________________
        // Start building matrices

        // Loop over reconstructed stubs
        for (unsigned istub=0; istub<nstubs; ++istub) {
            unsigned moduleId = reader.vb_modId   ->at(istub);
            float    strip    = reader.vb_coordx  ->at(istub);  // in full-strip unit
            float    segment  = reader.vb_coordy  ->at(istub);  // in full-strip unit

            float    stub_r   = reader.vb_r       ->at(istub);
            float    stub_phi = reader.vb_phi     ->at(istub);
            float    stub_z   = reader.vb_z       ->at(istub);
            float    stub_ds  = reader.vb_trigBend->at(istub);  // in full-strip unit


            if (verbose_>2) {
                std::cout << Debug() << "... ... stub: " << istub << " moduleId: " << moduleId << " strip: " << strip << " segment: " << segment << " r: " << stub_r << " phi: " << stub_phi << " z: " << stub_z << " ds: " << stub_ds << std::endl;
            }
        }

        ++nKept;
        ++nRead;
    }

    if (verbose_)  std::cout << Info() << Form("Read: %7ld, kept: %7ld", nRead, nKept) << std::endl;

    return 0;
}


// _____________________________________________________________________________
// Write matrices
int MatrixBuilder::writeMatrices(TString out) {

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
