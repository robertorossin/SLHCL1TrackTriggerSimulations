#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternGenerator.h"

#ifndef ROOT_Math_GenVector_eta
#include "Math/GenVector/eta.h"
#endif

//#include <type_traits>  // defines std::is_pod()

static const unsigned MAX_NTOWERS = 6 * 8;

static const unsigned MAX_NPATTERNS = 40000000;  // 40M = only 1/4 of design goal

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
        std::cout << Error() << "Failed to get all trigger towers from the file: " << src << std::endl;
        return 1;
    }

    //for (unsigned i=0; i<MAX_NTOWERS; ++i) {
    //    std::cout << "Tower " << i << " has " << triggerTowerMap_[i].size() << " modules. " << std::endl;
    //}

    // Prepare the trigger tower reverse map
    triggerTowerReverseMap_.clear();
    for (auto it: triggerTowerMap_) {  // loop over trigger tower map
        for (auto it2: it.second) {  // loop over the moduleIds in the tower
            auto found = triggerTowerReverseMap_.find(it2);
            if (found == triggerTowerReverseMap_.end()) {  // if moduleId is not yet in the reverse map
                std::vector<unsigned> triggerTowerIds;
                triggerTowerIds.push_back(it.first);  // push back the towerId
                triggerTowerReverseMap_.insert(std::make_pair(it2, triggerTowerIds));
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
    std::vector<float> *          vb_x          = 0;
    std::vector<float> *          vb_y          = 0;
    std::vector<float> *          vb_z          = 0;
    std::vector<float> *          vb_r          = 0;
    std::vector<float> *          vb_phi        = 0;
    std::vector<float> *          vb_coordx     = 0;
    std::vector<float> *          vb_coordy     = 0;
    std::vector<float> *          vb_roughPt    = 0;
    std::vector<unsigned> *       vb_iModCols   = 0;
    std::vector<unsigned> *       vb_iModRows   = 0;
    std::vector<unsigned> *       vb_modId      = 0;
    std::vector<unsigned> *       vb_nhits      = 0;
    std::vector<float> *          vb_simPt      = 0;
    std::vector<float> *          vb_simEta     = 0;
    std::vector<float> *          vb_simPhi     = 0;
    std::vector<int> *            vb_trkId      = 0;

    chain_->SetBranchStatus("*"                 , 0);
    chain_->SetBranchStatus("TTStubs_x"         , 1);
    chain_->SetBranchStatus("TTStubs_y"         , 1);
    chain_->SetBranchStatus("TTStubs_z"         , 1);
    chain_->SetBranchStatus("TTStubs_r"         , 1);
    chain_->SetBranchStatus("TTStubs_phi"       , 1);
    chain_->SetBranchStatus("TTStubs_coordx"    , 1);
    chain_->SetBranchStatus("TTStubs_coordy"    , 1);
    chain_->SetBranchStatus("TTStubs_roughPt"   , 1);
    chain_->SetBranchStatus("TTStubs_iModCols"  , 1);
    chain_->SetBranchStatus("TTStubs_iModRows"  , 1);
    chain_->SetBranchStatus("TTStubs_modId"     , 1);
    chain_->SetBranchStatus("TTStubs_nhits"     , 1);
    chain_->SetBranchStatus("TTStubs_simPt"     , 1);
    chain_->SetBranchStatus("TTStubs_simEta"    , 1);
    chain_->SetBranchStatus("TTStubs_simPhi"    , 1);
    chain_->SetBranchStatus("TTStubs_trkId"     , 1);

    chain_->SetBranchAddress("TTStubs_x"        , &(vb_x));
    chain_->SetBranchAddress("TTStubs_y"        , &(vb_y));
    chain_->SetBranchAddress("TTStubs_z"        , &(vb_z));
    chain_->SetBranchAddress("TTStubs_r"        , &(vb_r));
    chain_->SetBranchAddress("TTStubs_phi"      , &(vb_phi));
    chain_->SetBranchAddress("TTStubs_coordx"   , &(vb_coordx));
    chain_->SetBranchAddress("TTStubs_coordy"   , &(vb_coordy));
    chain_->SetBranchAddress("TTStubs_roughPt"  , &(vb_roughPt));
    chain_->SetBranchAddress("TTStubs_iModCols" , &(vb_iModCols));
    chain_->SetBranchAddress("TTStubs_iModRows" , &(vb_iModRows));
    chain_->SetBranchAddress("TTStubs_modId"    , &(vb_modId));
    chain_->SetBranchAddress("TTStubs_nhits"    , &(vb_nhits));
    chain_->SetBranchAddress("TTStubs_simPt"    , &(vb_simPt));
    chain_->SetBranchAddress("TTStubs_simEta"   , &(vb_simEta));
    chain_->SetBranchAddress("TTStubs_simPhi"   , &(vb_simPhi));
    chain_->SetBranchAddress("TTStubs_trkId"    , &(vb_trkId));

    //assert(std::is_pod<TTSuperstrip>::value && std::is_pod<TTPattern>::value && std::is_pod<TTHit>::value);

    allPatterns_.clear();
    allPatterns_.reserve(MAX_NPATTERNS);
    if ((unsigned) nEvents_ >= allPatterns_.max_size()) {
        std::cout << Error() << "Number of events is more than the max_size of a std::vector." << std::endl;
        return 1;
    }

    // _________________________________________________________________________
    // Loop over all events

    // Containers are declared outside the event loop to reduce memory allocations
    TTPattern patt;
    std::vector<TTSuperstrip> goodSuperstrips, goodAndFakeSuperstrips;

    int nRead = 0;
    int nKept = 0;
    for (int ievt=0; ievt<nEvents_; ++ievt) {
        Long64_t local_entry = chain_->LoadTree(ievt);  // for TChain
        if (local_entry < 0)  break;
        chain_->GetEntry(ievt);

        unsigned nstubs = vb_modId->size();
        if (verbose_>1 && ievt%1000==0)  std::cout << Debug() << Form("... Processing event: %7i, keeping: %7i", ievt, nKept) << std::endl;
        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " # stubs: " << nstubs << " [# patterns: " << allPatterns_.size() << "]" << std::endl;

        if (!nstubs) {  // skip if no stub
            continue;
        }

        // Loop over reconstructed stubs
        goodSuperstrips.clear();
        for (unsigned l=0; l<nstubs; ++l) {
            unsigned moduleId = vb_modId->at(l);
            // If there is a valid hit, but moduleId does not exist in any
            // trigger tower (due to cables, connections, or just too forward),
            // we drop them
            if (po.requireTriggerTower && triggerTowerReverseMap_.find(moduleId) == triggerTowerReverseMap_.end())
                continue;

            unsigned ncols = vb_iModCols->at(l);
            unsigned nrows = vb_iModRows->at(l);
            unsigned nhits = vb_nhits->at(l);
            if (nrows == 960 || nrows == 1016)
                nrows = 1024;
            assert(nhits > 0 && (ncols == 2 || ncols == 32) && nrows == 1024);
            float coordx = vb_coordx->at(l);
            float coordy = vb_coordy->at(l);

            // Use half-strip unit
            id_type col = halfStripRound(coordy);
            id_type row = halfStripRound(coordx);
            ncols *= 2;
            nrows *= 2;

            // Set to lower resolution according to nSubLadders and nSubModules
            col /= (ncols / std::min(ncols, (unsigned) po.nSubLadders));
            row /= (nrows / std::min(nrows, (unsigned) po.nSubModules));
            addr_type ssId = encodeSuperstripId(moduleId, col, row);
            bit_type ssBit = 1 << 0;

            // Use DCBits
            encodeDCBit(nDCBits_, ssId, ssBit);

            // Create a superstrip
            TTSuperstrip ss;
            constructSuperstrip(ssId, ssBit, ss);
            goodSuperstrips.push_back(ss);

            if (verbose_>2) {
                std::cout << Debug() << "... ... stub: " << l << " moduleId: " << moduleId << " trkId: " << vb_trkId->at(l) << " # hits: " << nhits << std::endl;
                std::cout << Debug() << "... ... stub: " << l << " ssId: " << ssId << " ssBit: " << ssBit << " col: " << col << " row: " << row << std::endl;
            }
        }


        // Add fake superstrips
        goodAndFakeSuperstrips.clear();
        addFakeSuperstrips(goodSuperstrips, goodAndFakeSuperstrips);
        int nFakeSuperstripsInThisPattern = goodAndFakeSuperstrips.size() - goodSuperstrips.size();
        assert(nFakeSuperstripsInThisPattern >= 0);

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
            constructPattern(goodAndFakeSuperstrips, patt);
            allPatterns_.push_back(patt);  // save the pattern

            ++nKept;
        }
        ++nRead;
    }
    if (verbose_)  std::cout << Info() << Form("Read: %7i, kept: %7i", nRead, nKept) << std::endl;


    // Keep only unique patterns
    uniquifyPatterns();  // do this after allPatterns_ is populated

    // Erase patterns not fully contained within any trigger tower
    if (po.requireTriggerTower)
        erasePatternsNotWithinTriggerTowers();

    if (verbose_>2) {
        for (unsigned i=0; i<allPatterns_.size(); ++i) {
            const TTPattern& patt = allPatterns_.at(i);
            std::cout << Debug() << "... patt: " << i << " " << patt << std::endl;
        }
    }

    std::sort(allPatterns_.begin(), allPatterns_.end(), isHigherPatternFrequency);

    if (verbose_)  std::cout << Info() << "Made " << allPatterns_.size() << " final patterns, highest frequency: " << (unsigned) allPatterns_.front().frequency << std::endl;

    return 0;
}


// _____________________________________________________________________________
// Output patterns into a TTree
int PatternGenerator::writePatterns(TString out) {
    if (!out.EndsWith(".root")) {
        std::cout << Error() << "Output filename must be .root" << std::endl;
        return 1;
    }

    const unsigned nentries = allPatterns_.size();
    if (verbose_)  std::cout << Info() << "Recreating " << out << " with " << nentries << " patterns." << std::endl;
    TFile* tfile = TFile::Open(out, "RECREATE");
    TTree* ttree = new TTree("patternBank", "");

    typedef unsigned char unsigned8;
    typedef unsigned short unsigned16;
    std::auto_ptr<unsigned8>                  frequency       (new unsigned8(0));
    std::auto_ptr<std::vector<unsigned> >     superstripIds   (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned16> >   superstripBits  (new std::vector<unsigned16>());

    ttree->Branch("frequency"       , &(*frequency));
    ttree->Branch("superstripIds"   , &(*superstripIds));
    ttree->Branch("superstripBits"  , &(*superstripBits));


    // _________________________________________________________________________
    // Loop over all patterns
    for (unsigned ievt=0; ievt<nentries; ++ievt) {
        if (verbose_>1 && ievt%10000==0)  std::cout << Debug() << Form("... Writing event: %7u", ievt) << std::endl;
        superstripIds->clear();
        superstripBits->clear();

        const TTPattern& patt = allPatterns_.at(ievt);
        *frequency = patt.frequency;

        // Assume patterns are sorted by frequency
        if (*frequency < minFrequency_)
            break;

        for (unsigned i=0; i<pattern_type().size(); ++i) {
            superstripIds->push_back(patt.id.at(i));
            superstripBits->push_back(patt.bit.at(i));
        }

        ttree->Fill();
    }
    assert(ttree->GetEntries() == (int) nentries);

    tfile->Write();
    delete ttree;
    delete tfile;
    return 0;
}


// _____________________________________________________________________________
// Private functions

void PatternGenerator::addFakeSuperstrips(const std::vector<TTSuperstrip>& goodSuperstrips,
                                          std::vector<TTSuperstrip>& goodAndFakeSuperstrips) {
    // This map uses the fact that the keys in a C++ map are sorted
    std::map<unsigned, unsigned> mlayerToSuperstripMap;  // key: mlayer, value: index of superstrip in goodSuperstrips
    for (unsigned i=0; i<goodSuperstrips.size(); ++i) {
        addr_type ssId = goodSuperstrips.at(i).id;
        unsigned layer = decodeSuperstripLayer(ssId);
        unsigned mlayer = layerMap_.at(layer);
        // The superstrips must not collide in 'mlayer'
        assert(mlayerToSuperstripMap.find(mlayer) == mlayerToSuperstripMap.end());
        mlayerToSuperstripMap.insert(std::make_pair(mlayer, i));
    }

    // Now find mlayer that is lacking a superstrip
    for (std::map<unsigned, unsigned>::const_iterator it=layerMap_.begin(); it!=layerMap_.end(); ++it) {
        unsigned mlayer = it->second;
        if (mlayerToSuperstripMap.find(mlayer) == mlayerToSuperstripMap.end()) {  // not found
            mlayerToSuperstripMap.insert(std::make_pair(mlayer, goodSuperstrips.size()) );
        }
    }

    // Insert fake superstrips
    addr_type fake_ssId = encodeFakeSuperstripId();
    bit_type fake_ssBit = 1 << 0;
    TTSuperstrip fake_ss;
    constructSuperstrip(fake_ssId, fake_ssBit, fake_ss);

    for (std::map<unsigned, unsigned>::const_iterator it=mlayerToSuperstripMap.begin(); it!=mlayerToSuperstripMap.end(); ++it) {
        if (it->second == goodSuperstrips.size()) {  // create a fake superstrip
            goodAndFakeSuperstrips.push_back(fake_ss);

        } else {  // put a good superstrip
            goodAndFakeSuperstrips.push_back(goodSuperstrips.at(it->second));
        }
    }
}

// Keep unique patterns but also increase frequency
// reference: http://en.cppreference.com/w/cpp/algorithm/unique
template<typename ForwardIterator, typename BinaryPredicate>
ForwardIterator uniqueForPattern(ForwardIterator first, ForwardIterator last,
                                 BinaryPredicate pred) {
    if (first == last)
        return last;

    ForwardIterator result = first;
    while (++first != last) {
        if (!pred(*result, *first)) {
            *(++result) = std::move(*first);
        } else {
            unionPattern(*first, *result);
        }
    }
    return ++result;
}

void PatternGenerator::uniquifyPatterns() {
    assert(!allPatterns_.empty());

    // Introspective sort
    std::sort(allPatterns_.begin(), allPatterns_.end(), isLesserPatternId);

    // Merge sort
    //std::stable_sort(allPatterns_.begin(), allPatterns_.end(), isLesserPatternId);

    // Heap sort
    //std::make_heap(allPatterns_.begin(), allPatterns_.end(), isLesserPatternId);
    //std::sort_heap(allPatterns_.begin(), allPatterns_.end(), isLesserPatternId);

    std::vector<TTPattern>::iterator last = uniqueForPattern(allPatterns_.begin(), allPatterns_.end(), isEqualPatternId);
    allPatterns_.erase(last, allPatterns_.end());
}

void PatternGenerator::uniquifyPatternsUsingStdMap() {
    assert(!allPatterns_.empty());
    std::vector<TTPattern> goodPatterns;
    goodPatterns.reserve(allPatterns_.size());

    std::map<pattern_type, unsigned> patternIdMap;  // key: patternId, value: index in goodPatterns_
    for (unsigned i=0; i<allPatterns_.size(); ++i) {
        const TTPattern& patt = allPatterns_.at(i);
        const pattern_type& pattId = patt.id;

        auto found = patternIdMap.find(pattId);
        if (found == patternIdMap.end()) {  // if not exist, insert the pattern
            goodPatterns.push_back(patt);
            patternIdMap.insert(std::make_pair(pattId, goodPatterns.size()-1) );

        } else {
            // If exist, merge them
            // merge(...) will update the DC bit and increase the frequency
            unionPattern(patt, goodPatterns.at(found->second));
        }
    }

    // In the end, swap allPatterns_ with goodPatterns. Memory used for goodPatterns is deallocated once out of scope
    allPatterns_.swap(goodPatterns);
}

// FIXME: this function has not been checked!
// Apply C++ erase-remove idiom when the predicate is a class member function
// reference: http://en.cppreference.com/w/cpp/algorithm/remove
void PatternGenerator::erasePatternsNotWithinTriggerTowers() {
    auto first = allPatterns_.begin();
    auto last = allPatterns_.end();

    // find_if
    for (; first != last; ++first) {
        if (isNotWithinTriggerTower(*first)) {
            break;
        }
    }

    // remove_if
    auto result = first;
    if (first != last) {
        for (; first != last; ++first) {
            if (!isNotWithinTriggerTower(*first)) {
                *result++ = std::move(*first);
            }
        }
    }

    // erase
    allPatterns_.erase(result, last);
}

bool PatternGenerator::isNotWithinTriggerTower(const TTPattern& patt) {
    const pattern_type& pattId = patt.id;
    std::map<unsigned, unsigned> triggerTowerIdCounts;
    unsigned moduleIdCount = 0;
    id_type moduleId = 0;
    for (unsigned l=0; l<pattId.size(); ++l) {  // loop over superstripIds in a pattern
        moduleId = decodeSuperstripModuleId(pattId.at(l));  // retrieve the moduleId
        auto found = triggerTowerReverseMap_.find(moduleId);
        if (found == triggerTowerReverseMap_.end()) {  // if moduleId not in the reverse map
            // do nothing
        } else {
            for (auto tower: found->second) {  // loop over towerIds containing the module
                ++triggerTowerIdCounts[tower];  // increment towerId counter (default value is zero)
            }
            moduleIdCount += 1;
        }
        //std::cout << "moduleId: " << moduleId << " " << moduleIdCount << " " << found->second.front() << std::endl;
    }

    // Loop over the counters in the map
    for (auto it: triggerTowerIdCounts) {
        if (it.second == moduleIdCount) {  // all moduleIds share at least one trigger tower
            return false;
        }
    }
    return true;
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
