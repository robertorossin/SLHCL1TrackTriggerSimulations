#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternGenerator.h"

#ifndef ROOT_Math_GenVector_eta
#include "Math/GenVector/eta.h"
#endif

static const unsigned MAX_NTOWERS = 6 * 8;
static const unsigned MIN_NLAYERS = 3;


// _____________________________________________________________________________
// Read and parse the trigger tower csv file
int PatternGenerator::readTriggerTowerFile(TString src) {
    std::vector<unsigned> values;
    if (src.EndsWith(".csv")) {
        if (verbose_)  std::cout << Info() << "Opening " << src << std::endl;

        unsigned i = 0;
        std::string line;
        ifstream ifs(src.Data());
        while (std::getline(ifs, line)) {
            std::istringstream iss(line);
            std::string issline;

            while (std::getline(iss, issline, ',')) {  // split by commas
                if (i != 0)  // skip the first line
                    values.push_back(std::stoi(issline));
            }
            ++i;
        }
    }

    if (values.empty()) {
        std::cout << Error() << "Failed to read the trigger tower file: " << src << std::endl;
        return 1;
    }

    triggerTowerMap_.clear();
    unsigned triggerTowerId = 0;
    for (unsigned i=0; i<values.size(); ++i) {
        if (i == 0)  continue;  // skip the first index
        if (values.at(i-1) <= 6 && values.at(i) <= 8) {  // eta_idx, phi_idx
            triggerTowerId = (values.at(i-1)-1) * 8 + (values.at(i)-1);  // mapped to 0-47
            triggerTowerMap_.insert(std::make_pair(triggerTowerId, std::vector<unsigned>()));
        } else if (values.at(i) > 10000) {
            triggerTowerMap_.at(triggerTowerId).push_back(values.at(i));
        }
    }

    if (triggerTowerMap_.size() != MAX_NTOWERS) {
        std::cout << Error() << "Failed to get all trigger towers from the trigger tower file: " << src << std::endl;
        return 1;
    }

    // Prepare the reversed trigger tower map
    triggerTowerMapReversed_.clear();
    for (auto it: triggerTowerMap_) {  // loop over trigger tower map
        for (auto it2: it.second) {  // loop over the moduleIds in the tower
            auto found = triggerTowerMapReversed_.find(it2);
            if (found == triggerTowerMapReversed_.end()) {  // if moduleId is not yet in the reversed map
                std::vector<unsigned> triggerTowerIds;
                triggerTowerIds.push_back(it.first);  // push back the towerId
                triggerTowerMapReversed_.insert(std::make_pair(it2, triggerTowerIds));
            } else {
                found->second.push_back(it.first);  // push back the towerId
            }
        }
    }
    return 0;
}


// _____________________________________________________________________________
// Read the input ntuples
int PatternGenerator::readFile(TString src) {
    if (src.EndsWith(".root")) {
        if (verbose_)  std::cout << Info() << "Opening " << src << std::endl;
        if (chain_->Add(src) )  // 1 if successful, 0 otherwise
            return 0;

    } else if (src.EndsWith(".txt")) {
        TFileCollection* fc = new TFileCollection("fileinfolist", "", src);
        if (chain_->AddFileInfoList((TCollection*) fc->GetList()) )  // 1 if successful, 0 otherwise
            return 0;
    }

    std::cout << Error() << "Input source should be either a .root file or a .txt file." << std::endl;
    return 1;
}


// _____________________________________________________________________________
// Make the patterns
int PatternGenerator::makePatterns() {
    if (verbose_)  std::cout << Info() << "Reading " << nEvents_ << " events" << std::endl;

    // For reading
    std::vector<float> *                vb_x              = 0;
    std::vector<float> *                vb_y              = 0;
    std::vector<float> *                vb_z              = 0;
    std::vector<float> *                vb_r              = 0;
    std::vector<float> *                vb_phi            = 0;
    std::vector<float> *                vb_roughPt        = 0;
    std::vector<unsigned> *             vb_iModCols       = 0;
    std::vector<unsigned> *             vb_iModRows       = 0;
    std::vector<unsigned> *             vb_modId          = 0;
    std::vector<unsigned> *             vb_nhits          = 0;
    std::vector<std::vector<int> > *    vb_hitCols        = 0;
    std::vector<std::vector<int> > *    vb_hitRows        = 0;
    std::vector<std::vector<int> > *    vb_hitTrkIds      = 0;
    std::vector<std::vector<float> > *  vb_hitXs          = 0;
    std::vector<std::vector<float> > *  vb_hitYs          = 0;
    std::vector<std::vector<float> > *  vb_hitZs          = 0;
    std::vector<float> *                vb_simPt          = 0;
    std::vector<float> *                vb_simEta         = 0;
    std::vector<float> *                vb_simPhi         = 0;
    std::vector<int> *                  vb_trkId          = 0;

    chain_->SetBranchStatus("*", 0);
    chain_->SetBranchStatus("TTStubs_x"        , 1);
    chain_->SetBranchStatus("TTStubs_y"        , 1);
    chain_->SetBranchStatus("TTStubs_z"        , 1);
    chain_->SetBranchStatus("TTStubs_r"        , 1);
    chain_->SetBranchStatus("TTStubs_phi"      , 1);
    chain_->SetBranchStatus("TTStubs_roughPt"  , 1);
    chain_->SetBranchStatus("TTStubs_iModCols" , 1);
    chain_->SetBranchStatus("TTStubs_iModRows" , 1);
    chain_->SetBranchStatus("TTStubs_modId"    , 1);
    chain_->SetBranchStatus("TTStubs_nhits"    , 1);
    chain_->SetBranchStatus("TTStubs_hitCols"  , 1);
    chain_->SetBranchStatus("TTStubs_hitRows"  , 1);
    chain_->SetBranchStatus("TTStubs_hitTrkIds", 1);
    chain_->SetBranchStatus("TTStubs_hitXs"    , 1);
    chain_->SetBranchStatus("TTStubs_hitYs"    , 1);
    chain_->SetBranchStatus("TTStubs_hitZs"    , 1);
    chain_->SetBranchStatus("TTStubs_simPt"    , 1);
    chain_->SetBranchStatus("TTStubs_simEta"   , 1);
    chain_->SetBranchStatus("TTStubs_simPhi"   , 1);
    chain_->SetBranchStatus("TTStubs_trkId"    , 1);

    chain_->SetBranchAddress("TTStubs_x"        , &(vb_x));
    chain_->SetBranchAddress("TTStubs_y"        , &(vb_y));
    chain_->SetBranchAddress("TTStubs_z"        , &(vb_z));
    chain_->SetBranchAddress("TTStubs_r"        , &(vb_r));
    chain_->SetBranchAddress("TTStubs_phi"      , &(vb_phi));
    chain_->SetBranchAddress("TTStubs_roughPt"  , &(vb_roughPt));
    chain_->SetBranchAddress("TTStubs_iModCols" , &(vb_iModCols));
    chain_->SetBranchAddress("TTStubs_iModRows" , &(vb_iModRows));
    chain_->SetBranchAddress("TTStubs_modId"    , &(vb_modId));
    chain_->SetBranchAddress("TTStubs_nhits"    , &(vb_nhits));
    chain_->SetBranchAddress("TTStubs_hitCols"  , &(vb_hitCols));
    chain_->SetBranchAddress("TTStubs_hitRows"  , &(vb_hitRows));
    chain_->SetBranchAddress("TTStubs_hitTrkIds", &(vb_hitTrkIds));
    chain_->SetBranchAddress("TTStubs_hitXs"    , &(vb_hitXs));
    chain_->SetBranchAddress("TTStubs_hitYs"    , &(vb_hitYs));
    chain_->SetBranchAddress("TTStubs_hitZs"    , &(vb_hitZs));
    chain_->SetBranchAddress("TTStubs_simPt"    , &(vb_simPt));
    chain_->SetBranchAddress("TTStubs_simEta"   , &(vb_simEta));
    chain_->SetBranchAddress("TTStubs_simPhi"   , &(vb_simPhi));
    chain_->SetBranchAddress("TTStubs_trkId"    , &(vb_trkId));

    allPatterns_.clear();
    if (nEvents_ >= (Long64_t) allPatterns_.max_size()) {
        std::cout << Error() << "Number of events is more than the max_size of a std::vector." << std::endl;
        return 1;
    }

    // _________________________________________________________________________
    // Loop over all events
    Long64_t nPassed = 0;
    for (Long64_t ievt=0; ievt<nEvents_; ++ievt) {
        Long64_t local_entry = chain_->LoadTree(ievt);  // for TChain
        if (local_entry < 0)  break;
        chain_->GetEntry(ievt);

        unsigned nstubs = vb_modId->size();
        if (verbose_>1 && ievt%1000==0)  std::cout << Debug() << Form("... Processing event: %7lld, keeping: %7lld", ievt, nPassed) << std::endl;
        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " # stubs: " << nstubs << " [# patterns: " << allPatterns_.size() << "]" << std::endl;

        if (!nstubs) {  // skip if no stub
            continue;
        }

        // Loop over reconstructed stubs
        std::vector<TTSuperstrip> goodSuperstrips;
        for (unsigned l=0; l<nstubs; ++l) {
            unsigned moduleId = vb_modId->at(l);
            // If there is a valid hit, but moduleId does not exist in any
            // trigger tower (due to cables, connections, or just too forward),
            // we drop them
            if (po.requireTriggerTower && triggerTowerMapReversed_.find(moduleId) == triggerTowerMapReversed_.end())
                continue;

            unsigned ncols = vb_iModCols->at(l);
            unsigned nrows = vb_iModRows->at(l);
            unsigned nhits = vb_nhits->at(l);
            if (nrows == 960 || nrows == 1016)
                nrows = 1024;
            assert(nhits == 1 && (ncols == 2 || ncols == 32) && nrows == 1024);
            int col = vb_hitCols->at(l).at(0);
            int row = vb_hitRows->at(l).at(0);

            // Set to lower resolution according to nSubLadders and nSubModules
            col /= (ncols / std::min((int) ncols, po.nSubLadders));
            row /= (nrows / std::min((int) nrows, po.nSubModules));
            addr_type ssId = encodeSuperstripId(moduleId, col, row);
            bit_type ssBit = 1 << 0;

            // Use DCBit
            encodeDCBit(nDCBits_, ssId, ssBit);

            // Create a superstrip
            TTSuperstrip ss(ssId, ssBit);
            goodSuperstrips.push_back(ss);

            if (verbose_>2)  std::cout << Debug() << "... ... stub: " << l << " moduleId: " << moduleId << " trkId: " << vb_trkId->at(l) << " # hits: " << nhits << std::endl;
            if (verbose_>2)  std::cout << Debug() << "... ... stub: " << l << " ssId: " << ssId << " ssBit: " << ssBit << " col: " << col << " row: " << row << " trkId: " << vb_hitTrkIds->at(l).at(0) << std::endl;
        }

        // Fake superstrip mechanism is carried out here

        // This map uses the fact that the keys in a C++ map are sorted
        std::map<unsigned, unsigned> mlayerToSuperstripMap;  // key: mlayer, value: index of superstrip in goodSuperstrips
        for (unsigned i=0; i<goodSuperstrips.size(); ++i) {
            addr_type ssId = goodSuperstrips.at(i).id();
            unsigned layer = decodeSuperstripLayer(ssId);
            unsigned mlayer = layerMap_.at(layer);
            if (mlayerToSuperstripMap.find(mlayer) == mlayerToSuperstripMap.end()) {  // not found
                mlayerToSuperstripMap.insert(std::make_pair(mlayer, i));
            } else {
                std::cout << Error() << "There is supposed to be one unique superstrip per layer at this stage" << std::endl;
            }
        }

        // Now find mlayer that is lacking a superstrip
        int nFakeSuperstripsInThisPattern = 0;
        for (std::map<unsigned, unsigned>::const_iterator it=layerMap_.begin(); it!=layerMap_.end(); ++it) {
            unsigned mlayer = it->second;
            if (mlayerToSuperstripMap.find(mlayer) == mlayerToSuperstripMap.end()) {  // not found
                mlayerToSuperstripMap.insert(std::make_pair(mlayer, goodSuperstrips.size()) );
                nFakeSuperstripsInThisPattern += 1;
            }
        }

        // Insert fake superstrips
        std::vector<TTSuperstrip> goodAndFakeSuperstrips;
        for (std::map<unsigned, unsigned>::const_iterator it=mlayerToSuperstripMap.begin(); it!=mlayerToSuperstripMap.end(); ++it) {
            if (it->second == goodSuperstrips.size()) {  // create a fake superstrip
                addr_type ssId = encodeFakeSuperstripId();
                bit_type ssBit = 1 << 0;
                TTSuperstrip ss(ssId, ssBit);
                goodAndFakeSuperstrips.push_back(ss);

            } else {
                const TTSuperstrip& ss = goodSuperstrips.at(it->second);
                goodAndFakeSuperstrips.push_back(ss);
            }
        }

        if (verbose_>2) {
            std::cout << Debug() << "... evt: " << ievt << " # goodAndFakeSuperstrips: " << goodAndFakeSuperstrips.size() << " {";
            std::copy(goodAndFakeSuperstrips.begin(), goodAndFakeSuperstrips.end(), std::ostream_iterator<TTSuperstrip>(std::cout, " "));
            std::cout << "}" << std::endl;
        }

        // Check the required number of layers and fake superstrips
        if (nFakeSuperstripsInThisPattern > nFakeSuperstrips_)
            nFakeSuperstripsInThisPattern = nFakeSuperstrips_;
        bool keep = ((int) goodSuperstrips.size() + nFakeSuperstripsInThisPattern >= nLayers_);
        if (keep) {
            // Create a pattern
            TTPattern patt(goodAndFakeSuperstrips);
            allPatterns_.push_back(patt);  // save the pattern

            ++nPassed;
        }
    }

    // Keep only unique patterns
    uniquifyPatterns();  // do this after allPatterns_ is populated

    // Erase patterns not fully contained in a trigger tower
    if (po.requireTriggerTower) {
        for (std::vector<TTPattern>::iterator it=goodPatterns_.begin(); it!=goodPatterns_.end(); ) {
            if (isFullyContainedInTriggerTower(*it)) {
                ++it;
            } else {
                it = goodPatterns_.erase(it);
            }
        }
    }

    if (verbose_>2) {
        for (unsigned i=0; i<goodPatterns_.size(); ++i) {
            const TTPattern& patt = goodPatterns_.at(i);
            std::cout << Debug() << "... patt: " << i << " " << patt << std::endl;
        }
    }

    if (verbose_)  std::cout << Info() << "Made " << goodPatterns_.size() << " final patterns, out of " << allPatterns_.size() << " inclusive, full-resolution patterns." << std::endl;

    std::sort(goodPatterns_.begin(), goodPatterns_.end(), sortByFrequency);
    return 0;
}


// _____________________________________________________________________________
// Output patterns into a TTree
int PatternGenerator::writePatterns(TString out) {
    if (!out.EndsWith(".root")) {
        std::cout << Error() << "Output filename must be .root" << std::endl;
        return 1;
    }

    if (maxPatterns_ > (int) goodPatterns_.size())
        maxPatterns_ = goodPatterns_.size();
    if (verbose_)  std::cout << Info() << "Recreating " << out << " with " << maxPatterns_ << " patterns." << std::endl;
    TFile* tfile = TFile::Open(out, "RECREATE");
    TTree* ttree = new TTree("patternBank", "");

    typedef unsigned short unsigned16;
    //typedef unsigned long long unsigned64;
    //typedef ULong64_t unsigned64;
    std::auto_ptr<unsigned>                   frequency       (new unsigned(0));
    std::auto_ptr<std::vector<unsigned> >     superstripIds   (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned16> >   superstripBits  (new std::vector<unsigned16>());

    ttree->Branch("frequency"       , &(*frequency));
    ttree->Branch("superstripIds"   , &(*superstripIds));
    ttree->Branch("superstripBits"  , &(*superstripBits));

    // _________________________________________________________________________
    // Loop over good patterns
    for (int ievt=0; ievt<maxPatterns_; ++ievt) {
        if (verbose_>1 && ievt%10000==0)  std::cout << Debug() << Form("... Writing event: %7i", ievt) << std::endl;

        const TTPattern& patt = goodPatterns_.at(ievt);
        *frequency = patt.frequency();

        superstripIds->clear();
        superstripBits->clear();
        const std::vector<TTSuperstrip> superstrips = patt.getSuperstrips();
        for (unsigned i=0; i<superstrips.size(); ++i) {
            const TTSuperstrip& superstrip = superstrips.at(i);
            superstripIds->push_back(superstrip.id());
            superstripBits->push_back(superstrip.bit());
        }

        ttree->Fill();
    }
    assert(ttree->GetEntries() == maxPatterns_);

    // Also save the triggerTower
    TTree* ttree2 = new TTree("triggerTower", "");
    std::map<unsigned, std::vector<unsigned> > * ptr_triggerTowerMap_ = &triggerTowerMap_;
    ttree2->Branch("triggerTowerMap", ptr_triggerTowerMap_);
    std::map<unsigned, std::vector<unsigned> > * ptr_triggerTowerMapReversed_ = &triggerTowerMapReversed_;
    ttree2->Branch("triggerTowerMapReversed", ptr_triggerTowerMapReversed_);
    ttree2->Fill();
    assert(ttree2->GetEntries() == 1);

    tfile->Write();
    delete ttree;
    delete ttree2;
    delete tfile;
    return 0;
}


// _____________________________________________________________________________
// Private functions

// Make unique patterns from 'allPatterns_' and put them into 'goodPatterns_'
void PatternGenerator::uniquifyPatterns() {
    assert(!allPatterns_.empty());
    goodPatterns_.clear();

    std::map<pattern_type, unsigned> patternIdMap;  // key: patternId, value: index in goodPatterns_
    for (unsigned i=0; i<allPatterns_.size(); ++i) {
        const TTPattern& patt = allPatterns_.at(i);
        const pattern_type& pattId = patt.id();

        auto found = patternIdMap.find(pattId);
        if (found == patternIdMap.end()) {  // if not exist, insert the pattern
            goodPatterns_.push_back(patt);
            patternIdMap.insert(std::make_pair(pattId, goodPatterns_.size()-1) );

        } else {
            // If exist, merge them
            // merge(...) will update the DC bit and increase the frequency
            unsigned index = found->second;
            goodPatterns_.at(index).merge(patt);
        }
    }
    //allPatterns_.clear();
}

// Remove patterns not fully contained in a trigger tower
bool PatternGenerator::isFullyContainedInTriggerTower(const TTPattern& patt) {
    std::map<unsigned, unsigned> triggerTowerIdCounts;
    unsigned moduleIdCount = 0;
    for(auto it: patt.id()) {  // loop over superstripIds in a pattern
        id_type moduleId = decodeSuperstripModuleId(it);  // retrieve the moduleId
        auto found = triggerTowerMapReversed_.find(moduleId);
        if (found == triggerTowerMapReversed_.end()) {  // if moduleId not in the map
            // do nothing
        } else {
            for (auto it2: found->second) {  // loop over towerIds containing the module
                auto found2 = triggerTowerIdCounts.find(it2);
                if (found2 == triggerTowerIdCounts.end()) {  // if not yet found, insert 1
                    triggerTowerIdCounts.insert(std::make_pair(it2, 1));
                } else {  // if found, just increment the counter
                    found2->second += 1;
                }
            }
            moduleIdCount += 1;
        }
        //std::cout << "moduleId: " << moduleId << " " << moduleIdCount << " " << found->second.front() << std::endl;
    }

    // Loop over the counters in the map
    for (auto it: triggerTowerIdCounts) {
        if ((int) it.second == (int) moduleIdCount) {
            return true;
        }
    }
    return false;
}


// _____________________________________________________________________________
// Main driver
int PatternGenerator::run(TString out, TString src, TString layout) {
    gROOT->ProcessLine("#include <vector>");  // how is it not loaded?

    int exitcode = 0;
    Timing(1);

    exitcode = readTriggerTowerFile(layout);
    if (exitcode)  return exitcode;
    Timing();

    exitcode = readFile(src);
    if (exitcode)  return exitcode;
    Timing();

    exitcode = makePatterns();
    if (exitcode)  return exitcode;
    Timing();

    exitcode = writePatterns(out);
    if (exitcode)  return exitcode;
    Timing();

    chain_->Reset();
    return exitcode;
}
