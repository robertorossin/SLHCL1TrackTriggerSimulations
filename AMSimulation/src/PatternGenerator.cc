#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternGenerator.h"

#include "SLHCL1TrackTriggerSimulations/AMSimulationIO/interface/PatternBankReader.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulationIO/interface/TTStubReader.h"

static const unsigned MAX_FREQUENCY = 0xffffffff;  // unsigned

namespace {
// Comparator
bool sortByFrequency(const std::pair<pattern_type, unsigned>& lhs, const std::pair<pattern_type, unsigned>& rhs) {
    return lhs.second > rhs.second;
}
}


// _____________________________________________________________________________
// Make the patterns
int PatternGenerator::makePatterns(TString src) {
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

    patternBank_map_.clear();
    pattern_type patt;
    patt.fill(0);

    // Bookkeepers
    float coverage = 0.;
    long int bankSize = 0, bankSizeOld = -100000, nKeptOld = -100000;
    long int nRead = 0, nKept = 0;

    for (long long ievt=0; ievt<nEvents_; ++ievt) {
        if (reader.loadTree(ievt) < 0)  break;
        reader.getEntry(ievt);

        // Running estimate of coverage
        if (verbose_>1 && ievt%100000==0) {
            bankSize = patternBank_map_.size();
            coverage = 1.0 - float(bankSize - bankSizeOld) / float(nKept - nKeptOld);

            std::cout << Debug() << Form("... Processing event: %7lld, keeping: %7ld, # patterns: %7ld, coverage: %7.5f", ievt, nKept, bankSize, coverage) << std::endl;

            bankSizeOld = bankSize;
            nKeptOld = nKept;
        }

        const unsigned nstubs = reader.vb_modId->size();
        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " # stubs: " << nstubs << std::endl;

        // Get sim info
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

        // Insert pattern into the bank
        ++patternBank_map_[patt];

        // Update the attributes
        if (po_.speedup<1) {
            std::pair<std::map<pattern_type, Attributes *>::iterator, bool> ins = patternAttributes_map_.insert(std::make_pair(patt, new Attributes()));
            Attributes * attr = ins.first->second;
            if (attr) {
                ++ attr->n;
                attr->invPt.fill(simChargeOverPt);
                attr->cotTheta.fill(simCotTheta);
                attr->phi.fill(simPhi);
                attr->z0.fill(simVz);
            }
        }

        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " patt: " << patt << std::endl;

        ++nKept;
        ++nRead;
    }

    if (nRead == 0) {
        std::cout << Error() << "Failed to read any event." << std::endl;
        return 1;
    }

    if (verbose_)  std::cout << Info() << Form("Read: %7ld, kept: %7ld, # patterns: %7lu, coverage: %7.5f", nRead, nKept, patternBank_map_.size(), coverage) << std::endl;

    // Save these numbers
    coverage_       = coverage;
    coverage_count_ = nKept;


    // _________________________________________________________________________
    // Sort by frequency

    // Convert map to vector of pairs
    const unsigned origSize = patternBank_map_.size();
    //patternBank_pairs_.reserve(patternBank_map_.size());  // can cause bad_alloc
    //patternBank_pairs_.insert(patternBank_pairs_.end(), patternBank_map_.begin(), patternBank_map_.end());

    for (std::map<pattern_type, unsigned>::const_iterator it = patternBank_map_.begin();
         it != patternBank_map_.end(); ) {  // should not cause bad_alloc
        patternBank_pairs_.push_back(*it);
        it = patternBank_map_.erase(it);
    }
    assert(patternBank_pairs_.size() == origSize);

    // Clear map and release memory
    std::map<pattern_type, unsigned> mapEmpty;
    patternBank_map_.clear();
    patternBank_map_.swap(mapEmpty);

    // Sort by frequency
    std::stable_sort(patternBank_pairs_.begin(), patternBank_pairs_.end(), sortByFrequency);

    if (verbose_>2) {
        for (unsigned i=0; i<patternBank_pairs_.size(); ++i) {
            const std::pair<pattern_type, unsigned>& apair = patternBank_pairs_.at(i);
            std::cout << Debug() << "... patt: " << i << "  " << apair.first << " freq: " << apair.second << std::endl;
        }
    }

    unsigned highest_freq = patternBank_pairs_.size() ? patternBank_pairs_.front().second : 0;
    if (verbose_)  std::cout << Info() << "Generated " << patternBank_pairs_.size() << " patterns, highest freq: " << highest_freq << std::endl;
    assert(patternBank_pairs_.front().second <= MAX_FREQUENCY);

    return 0;
}


// _____________________________________________________________________________
// Output patterns into a TTree
int PatternGenerator::writePatterns(TString out) {

    // _________________________________________________________________________
    // For writing
    PatternBankWriter writer(verbose_);
    if (writer.init(out)) {
        std::cout << Error() << "Failed to initialize TTRoad writer." << std::endl;
        return 1;
    }

    // _________________________________________________________________________
    // Save pattern bank statistics
    *(writer.pb_coverage)   = coverage_;
    *(writer.pb_count)      = coverage_count_;
    *(writer.pb_tower)      = po_.tower;
    *(writer.pb_superstrip) = po_.superstrip;
    writer.fillPatternBankInfo();

    // _________________________________________________________________________
    // Save pattern bank
    const long long npatterns = patternBank_pairs_.size();

    // Bookkeepers
    unsigned nKept = 0;
    unsigned freq = MAX_FREQUENCY, oldFreq = MAX_FREQUENCY;
    int n90=0, n95=0, n99=0;

    for (long long ipatt=0; ipatt<npatterns; ++ipatt) {
        if (verbose_>1 && ipatt%100==0) {
            float coverage = float(nKept) / coverage_count_ * coverage_;
            if (coverage < 0.90 + 1e-5)
                n90 = ipatt;
            else if (coverage < 0.95 + 1e-5)
                n95 = ipatt;
            else if (coverage < 0.99 + 1e-5)
                n99 = ipatt;

            std::cout << Debug() << Form("... Writing event: %7lld, sorted coverage: %7.5f", ipatt, coverage) << std::endl;
        }

        freq = patternBank_pairs_.at(ipatt).second;

        // Check whether patterns are indeed sorted by frequency
        assert(oldFreq >= freq);
        oldFreq = freq;
        nKept += freq;

        if (freq < (unsigned) po_.minFrequency)  // cut off
            break;

        writer.pb_superstripIds->clear();
        const pattern_type& patt = patternBank_pairs_.at(ipatt).first;
        for (unsigned ilayer=0; ilayer<po_.nLayers; ++ilayer) {
            writer.pb_superstripIds->push_back(patt.at(ilayer));
        }
        *(writer.pb_frequency) = freq;

        if (po_.speedup<1) {
            const Attributes * attr = patternAttributes_map_.at(patt);
            *(writer.pb_invPt_mean)     = attr->invPt.getMean();
            *(writer.pb_invPt_sigma)    = attr->invPt.getSigma();
            *(writer.pb_cotTheta_mean)  = attr->cotTheta.getMean();
            *(writer.pb_cotTheta_sigma) = attr->cotTheta.getSigma();
            *(writer.pb_phi_mean)       = attr->phi.getMean();
            *(writer.pb_phi_sigma)      = attr->phi.getSigma();
            *(writer.pb_z0_mean)        = attr->z0.getMean();
            *(writer.pb_z0_sigma)       = attr->z0.getSigma();
        }

        writer.fillPatternBank();
        writer.fillPatternAttributes();
    }

    long long nentries = writer.writeTree();
    assert(npatterns == nentries);
    assert(coverage_count_ == nKept);

    if (verbose_)  std::cout << Info() << "After sorting by frequency, N(90% cov)=" << n90 << ", N(95% cov)=" << n95 << ", N(99% cov)=" << n99 << std::endl;

    return 0;
}


// _____________________________________________________________________________
// Main driver
int PatternGenerator::run() {
    int exitcode = 0;
    Timing(1);

    exitcode = makePatterns(po_.input);
    if (exitcode)  return exitcode;
    Timing();

    exitcode = writePatterns(po_.output);
    if (exitcode)  return exitcode;
    Timing();

    return exitcode;
}
