#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternAnalyzer.h"

#include "SLHCL1TrackTriggerSimulations/AMSimulationIO/interface/PatternBankReader.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulationIO/interface/TTStubReader.h"

namespace {
// Comparator
bool sortByInvPt(const std::pair<pattern_type, Attributes *>& lhs, const std::pair<pattern_type, Attributes *>& rhs) {
    return (lhs.second)->invPt.getMean() < (rhs.second)->invPt.getMean();
}
}


// _____________________________________________________________________________
int PatternAnalyzer::setupTriggerTower(TString datadir) {
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
int PatternAnalyzer::setupSuperstrip() {
    try {
        arbiter_ -> setDefinition(po_.superstrip, po_.tower, ttmap_);

    } catch (const std::invalid_argument& e) {
        std::cout << Error() << "Failed to set definition: " << po_.superstrip << ". What: " << e.what() << std::endl;
        return 1;
    }

    //arbiter_ -> print();
    return 0;
}

// _____________________________________________________________________________
int PatternAnalyzer::loadPatterns(TString bank) {
    if (verbose_)  std::cout << Info() << "Loading patterns from " << bank << std::endl;

    // _________________________________________________________________________
    // For reading pattern bank
    PatternBankReader pbreader(verbose_);
    if (pbreader.init(bank)) {
        std::cout << Error() << "Failed to initialize PatternBankReader." << std::endl;
        return 1;
    }

    long long npatterns = pbreader.getPatterns();
    if (npatterns > po_.maxPatterns)
        npatterns = po_.maxPatterns;
    assert(npatterns > 0);

    // Allocate memory
    patternAttributes_.clear();
    patternAttributes_.resize(npatterns);

    // _________________________________________________________________________
    // Load the patterns

    pattern_type patt;
    patt.fill(0);
    std::pair<std::map<pattern_type, Attributes *>::iterator,bool> ret;

    for (long long ipatt=0; ipatt<npatterns; ++ipatt) {
        pbreader.getPattern(ipatt);
        if (pbreader.pb_frequency < po_.minFrequency)
            break;

        assert(pbreader.pb_superstripIds->size() == po_.nLayers);

        // Make the pattern
        std::copy(pbreader.pb_superstripIds->begin(), pbreader.pb_superstripIds->end(), patt.begin());

        // Insert the pattern
        ret = patternBank_map_.insert(std::make_pair(patt, &(patternAttributes_.at(ipatt))));

        if (!ret.second) {
            std::cout << Warning() << "Failed to insert: " << patt << std::endl;
        }
    }
    if (verbose_)  std::cout << Info() << "Successfully loaded " << npatterns << " patterns." << std::endl;

    return 0;
}

// _____________________________________________________________________________
// Make the patterns
int PatternAnalyzer::makePatterns(TString src) {
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
    // Book histograms
    TH1::AddDirectory(kFALSE);
    histogram2Ds["diff_invPt_vs_invPt1"] = new TH2F("diff_invPt_vs_invPt1", "; signed 1/p_{T} [1/GeV]; Diff from mean value of road [1/GeV]", 1000, -0.5, 0.5, 1000, -0.05, 0.05);
    histogram2Ds["diff_invPt_vs_invPt2"] = new TH2F("diff_invPt_vs_invPt2", "; signed 1/p_{T} [1/GeV]; Diff from linear approx [1/GeV]", 1000, -0.5, 0.5, 1000, -0.05, 0.05);
    histogramPRs["diff_invPt_pr_invPt1"] = new TProfile("diff_invPt_pr_invPt1", "; signed 1/p_{T} [1/GeV]; Diff from mean value of road [1/GeV]", 1000, -0.5, 0.5, "s");
    histogramPRs["diff_invPt_pr_invPt2"] = new TProfile("diff_invPt_pr_invPt2", "; signed 1/p_{T} [1/GeV]; Diff from linear approx [1/GeV]", 1000, -0.5, 0.5, "s");


    // _________________________________________________________________________
    // Loop over all events

    pattern_type patt;
    patt.fill(0);

    // Bookkeepers
    long int nRead = 0, nKept = 0;

    // Cache
    std::vector<std::pair<float, Attributes *> >  cache;  // save pointer to the attr for every valid track

    for (long long ievt=0; ievt<nEvents_; ++ievt) {
        if (reader.loadTree(ievt) < 0)  break;
        reader.getEntry(ievt);

        const unsigned nstubs = reader.vb_modId->size();
        if (verbose_>1 && ievt%100000==0)  std::cout << Debug() << Form("... Processing event: %7lld, keeping: %7ld", ievt, nKept) << std::endl;
        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " # stubs: " << nstubs << std::endl;

        // Check sim info
        assert(reader.vp_pt->size() == 1);
        float simPt           = reader.vp_pt->front();
        float simEta          = reader.vp_eta->front();
        float simPhi          = reader.vp_phi->front();
        //float simVx           = reader.vp_vx->front();
        //float simVy           = reader.vp_vy->front();
        float simVz           = reader.vp_vz->front();
        int   simCharge       = reader.vp_charge->front();

        float simCotTheta     = std::sinh(simEta);
        float simChargeOverPt = float(simCharge)/simPt;

        // Apply track pt requirement
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
        // Start generating patterns
        patt.fill(0);

        // Loop over reconstructed stubs
        for (unsigned istub=0; istub<nstubs; ++istub) {
            unsigned moduleId = reader.vb_modId   ->at(istub);
            float    strip    = reader.vb_coordx  ->at(istub);  // in full-strip unit
            float    segment  = reader.vb_coordy  ->at(istub);  // in full-strip unit

            float    stub_r   = reader.vb_r       ->at(istub);
            float    stub_phi = reader.vb_phi     ->at(istub);
            float    stub_z   = reader.vb_z       ->at(istub);
            float    stub_ds  = reader.vb_trigBend->at(istub);  // in full-strip unit

            // Find superstrip ID
            unsigned ssId = 0;
            if (!arbiter_ -> useGlobalCoord()) {  // local coordinates
                ssId = arbiter_ -> superstripLocal(moduleId, strip, segment);

            } else {                              // global coordinates
                ssId = arbiter_ -> superstripGlobal(moduleId, stub_r, stub_phi, stub_z, stub_ds);
            }
            patt.at(istub) = ssId;

            if (verbose_>2) {
                std::cout << Debug() << "... ... stub: " << istub << " moduleId: " << moduleId << " strip: " << strip << " segment: " << segment << " r: " << stub_r << " phi: " << stub_phi << " z: " << stub_z << " ds: " << stub_ds << std::endl;
                std::cout << Debug() << "... ... stub: " << istub << " ssId: " << ssId << std::endl;
            }

        }

        // Find pattern in the bank
        std::map<pattern_type, Attributes *>::iterator found = patternBank_map_.find(patt);
        if (found != patternBank_map_.end()) {
            Attributes * attr = found->second;
            ++ attr->n;
            attr->invPt.fill(simChargeOverPt);
            attr->cotTheta.fill(simCotTheta);
            attr->phi.fill(simPhi);
            attr->z0.fill(simVz);

            cache.push_back(std::make_pair(simChargeOverPt, attr));

        } else {
            //std::cout << Warning() << "Failed to find: " << patt << std::endl;
        }

        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " patt: " << patt << std::endl;

        ++nKept;
        ++nRead;
    }

    if (nRead == 0) {
        std::cout << Error() << "Failed to read any event." << std::endl;
        return 1;
    }

    if (verbose_)  std::cout << Info() << Form("Read: %7ld, kept: %7ld", nRead, nKept) << std::endl;


    // _________________________________________________________________________
    // Sort by mean invPt

    // Convert map to vector of pairs
    const unsigned origSize = patternBank_map_.size();
    //patternBank_pairs_.reserve(patternBank_map_.size());  // can cause bad_alloc
    //patternBank_pairs_.insert(patternBank_pairs_.end(), patternBank_map_.begin(), patternBank_map_.end());

    for (std::map<pattern_type, Attributes *>::const_iterator it = patternBank_map_.begin();
         it != patternBank_map_.end(); ) {  // should not cause bad_alloc
        patternBank_pairs_.push_back(*it);
        it = patternBank_map_.erase(it);
    }
    assert(patternBank_pairs_.size() == origSize);

    // Clear map and release memory
    std::map<pattern_type, Attributes *> mapEmpty;
    patternBank_map_.clear();
    patternBank_map_.swap(mapEmpty);

    // Sort by invPt
    std::stable_sort(patternBank_pairs_.begin(), patternBank_pairs_.end(), sortByInvPt);

    // Assign sorted pattern id
    for (unsigned i=0; i<patternBank_pairs_.size(); ++i) {
        patternBank_pairs_.at(i).second->id = i;
    }


    // _________________________________________________________________________
    // Fill histograms

    // CUIDADO: this assumes all the pattern attributes are populated well (ntracks > npatterns)
    const unsigned npatterns = patternBank_pairs_.size();
    for (std::vector<std::pair<float, Attributes *> >::const_iterator it=cache.begin();
         it!=cache.end(); ++it) {

        float approx = 1.0 / po_.minPt * (-1. + 2./(npatterns-1) * it->second->id);
        if (it->second->n <= 1)
            std::cout << Warning() << "Too few entries: " << it->second->n << " id: " << it->second->id << std::endl;
        if (verbose_>3)
            std::cout << Debug() << "... good evt: " << it-cache.begin() << " invPt: " << it->first << " mean: " << it->second->invPt.getMean() << " id: " << it->second->id << " approx: " << approx << std::endl;

        // Mean is like a measured quantity. Usually difference is defined as "measured" - "true"
        histogram2Ds["diff_invPt_vs_invPt1"]->Fill(it->first, it->second->invPt.getMean() - it->first);
        histogram2Ds["diff_invPt_vs_invPt2"]->Fill(it->first, approx - it->first);
        histogramPRs["diff_invPt_pr_invPt1"]->Fill(it->first, it->second->invPt.getMean() - it->first);
        histogramPRs["diff_invPt_pr_invPt2"]->Fill(it->first, approx - it->first);
    }

    return 0;
}


// _____________________________________________________________________________
// Output patterns into a TTree
int PatternAnalyzer::writePatterns(TString out) {

    // _________________________________________________________________________
    // For writing
    PatternBankWriter writer(verbose_);
    if (writer.init(out)) {
        std::cout << Error() << "Failed to initialize TTRoad writer." << std::endl;
        return 1;
    }

    // _________________________________________________________________________
    // Save pattern bank statistics
    *(writer.pb_coverage)   = 0.;  // dummy
    *(writer.pb_count)      = 0;   // dummy
    *(writer.pb_tower)      = po_.tower;
    *(writer.pb_superstrip) = po_.superstrip;
    writer.fillPatternBankInfo();

    // _________________________________________________________________________
    // Save pattern bank
    const long long npatterns = patternBank_pairs_.size();

    for (long long ipatt=0; ipatt<npatterns; ++ipatt) {
        if (verbose_>1 && ipatt%100==0) {
            std::cout << Debug() << Form("... Writing event: %7lld", ipatt) << std::endl;
        }

        const Attributes * attr = patternBank_pairs_.at(ipatt).second;

        writer.pb_superstripIds->clear();
        const pattern_type& patt = patternBank_pairs_.at(ipatt).first;
        for (unsigned ilayer=0; ilayer<po_.nLayers; ++ilayer) {
            writer.pb_superstripIds->push_back(patt.at(ilayer));
        }
        *(writer.pb_frequency)      = attr->n;

        *(writer.pb_invPt_mean)     = attr->invPt.getMean();
        *(writer.pb_invPt_sigma)    = attr->invPt.getSigma();
        *(writer.pb_cotTheta_mean)  = attr->cotTheta.getMean();
        *(writer.pb_cotTheta_sigma) = attr->cotTheta.getSigma();
        *(writer.pb_phi_mean)       = attr->phi.getMean();
        *(writer.pb_phi_sigma)      = attr->phi.getSigma();
        *(writer.pb_z0_mean)        = attr->z0.getMean();
        *(writer.pb_z0_sigma)       = attr->z0.getSigma();

        writer.fillPatternBank();
        writer.fillPatternAttributes();
    }

    // _________________________________________________________________________
    // Write histograms
    for (std::map<TString, TH1F *>::const_iterator it=histograms.begin();
         it!=histograms.end(); ++it) {
        if (it->second)  it->second->SetDirectory(gDirectory);
    }

    for (std::map<TString, TH2F *>::const_iterator it=histogram2Ds.begin();
         it!=histogram2Ds.end(); ++it) {
        if (it->second)  it->second->SetDirectory(gDirectory);
    }

    for (std::map<TString, TProfile *>::const_iterator it=histogramPRs.begin();
         it!=histogramPRs.end(); ++it) {
        if (it->second)  it->second->SetDirectory(gDirectory);
    }

    long long nentries = writer.writeTree();
    assert(npatterns == nentries);

    return 0;
}


// _____________________________________________________________________________
// Main driver
int PatternAnalyzer::run() {
    int exitcode = 0;
    Timing(1);

    exitcode = setupTriggerTower(po_.datadir);
    if (exitcode)  return exitcode;
    Timing();

    exitcode = setupSuperstrip();
    if (exitcode)  return exitcode;
    Timing();

    exitcode = loadPatterns(po_.bankfile);
    if (exitcode)  return exitcode;
    Timing();

    exitcode = makePatterns(po_.input);
    if (exitcode)  return exitcode;
    Timing();

    exitcode = writePatterns(po_.output);
    if (exitcode)  return exitcode;
    Timing();

    return exitcode;
}
