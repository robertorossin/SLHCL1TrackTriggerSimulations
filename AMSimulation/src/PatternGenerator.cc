#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternGenerator.h"


static const unsigned MAX_MODID = 23000;
static const unsigned MAX_MODCOL = 32;
static const unsigned MAX_MODROW = 1024;
static const unsigned MAX_SECTOR = 6 * 8;

// _____________________________________________________________________________
// Create a dummy tree with randomly generated patterns
void PatternGenerator::createDummy(TString out, int n) {
    TFile* tfile = TFile::Open(out, "RECREATE");
    tfile->mkdir("ntupler");
    tfile->cd("ntupler");

    TTree* ttree = new TTree("tree", "");
    std::auto_ptr<std::vector<unsigned> >           vb_iModCols         (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> >           vb_iModRows         (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> >           vb_modId            (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> >           vb_nhits            (new std::vector<unsigned>());
    std::auto_ptr<std::vector<std::vector<int> > >  vb_hitCols          (new std::vector<std::vector<int> >());
    std::auto_ptr<std::vector<std::vector<int> > >  vb_hitRows          (new std::vector<std::vector<int> >());
    std::auto_ptr<std::vector<std::vector<int> > >  vb_hitTrkIds        (new std::vector<std::vector<int> >());
    std::auto_ptr<std::vector<float> >              vb_simPt            (new std::vector<float>());
    std::auto_ptr<std::vector<float> >              vb_simEta           (new std::vector<float>());
    std::auto_ptr<std::vector<float> >              vb_simPhi           (new std::vector<float>());
    std::auto_ptr<std::vector<int> >                vb_trkId            (new std::vector<int>());
    std::auto_ptr<unsigned>                         v_event             (new unsigned(0));

    ttree->Branch("TTStubs_iModCols" , &(*vb_iModCols));
    ttree->Branch("TTStubs_iModRows" , &(*vb_iModRows));
    ttree->Branch("TTStubs_modId"    , &(*vb_modId));
    ttree->Branch("TTStubs_nhits"    , &(*vb_nhits));
    ttree->Branch("TTStubs_hitCols"  , &(*vb_hitCols));
    ttree->Branch("TTStubs_hitRows"  , &(*vb_hitRows));
    ttree->Branch("TTStubs_hitTrkIds", &(*vb_hitTrkIds));
    ttree->Branch("TTStubs_simPt"    , &(*vb_simPt));
    ttree->Branch("TTStubs_simEta"   , &(*vb_simEta));
    ttree->Branch("TTStubs_simPhi"   , &(*vb_simPhi));
    ttree->Branch("TTStubs_trkId"    , &(*vb_trkId));
    ttree->Branch("event"            , &(*v_event));

    // Throw random numbers
    for (int i=0; i<n; ++i) {
        vb_iModCols->clear();
        vb_iModRows->clear();
        vb_modId->clear();
        vb_nhits->clear();
        vb_hitCols->clear();
        vb_hitRows->clear();
        vb_hitTrkIds->clear();
        vb_simPt->clear();
        vb_simEta->clear();
        vb_simPhi->clear();
        vb_trkId->clear();
        *v_event = 0;

        for (int l=0; l<nLayers_; ++l) {  // make one stub in each layer
            vb_iModCols->push_back(MAX_MODCOL);
            vb_iModRows->push_back(MAX_MODROW);
            vb_modId->push_back(gRandom->Uniform(0,MAX_MODID));
            vb_nhits->push_back(1);
            std::vector<int> rows, cols, trkIds;
            rows.push_back(gRandom->Uniform(0,MAX_MODCOL));
            cols.push_back(gRandom->Uniform(0,MAX_MODROW));
            trkIds.push_back(0);
            vb_hitRows->push_back(rows);
            vb_hitCols->push_back(cols);
            vb_hitTrkIds->push_back(trkIds);
            vb_simPt->push_back(gRandom->Uniform(minPt_, maxPt_));
            vb_simEta->push_back(gRandom->Uniform(minEta_, maxEta_));
            vb_simPhi->push_back(gRandom->Uniform(minPhi_, maxPhi_));
            vb_trkId->push_back(0);
            *v_event = 0;
        }
        ttree->Fill();
    }
    assert(ttree->GetEntries() == n);
    tfile->Write();
    delete ttree;
    delete tfile;
}

// _____________________________________________________________________________
// Read and parse the sector map csv file
int PatternGenerator::readSectorFile(TString src) {
    Pattern::vuint32_t values;
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
        std::cout << Error() << "Failed to read the sector file: " << src << std::endl;
        return 1;
    }

    uint32_t sectorId = 0;
    for (unsigned i=0; i<values.size(); ++i) {
        if (i != 0) {  // skip the first index
            if (values.at(i-1) <= 6 && values.at(i) <= 8) {  // eta_idx, phi_idx
                sectorId = values.at(i-1) * 10 + values.at(i);
                sectorMap_.insert(std::make_pair(sectorId, Pattern::vuint32_t()));
            } else if (values.at(i) > 10000) {
                sectorMap_[sectorId].push_back(values.at(i));
            }
        }
    }

    if (sectorMap_.size() != MAX_SECTOR) {
        std::cout << Error() << "Failed to get all sectors from the sector file: " << src << std::endl;
        return 1;
    }
    return 0;
}

// _____________________________________________________________________________
// Read the input ntuples
int PatternGenerator::readFile(TString src) {
    if (src == "EMPTY") {
        TString out = "dummy.root";
        if (verbose_)  std::cout << Info() << "Recreating " << out << " with 10000 events." << std::endl;
        createDummy(out, 10000);
        chain_->Add(out);

    } else if (src.EndsWith(".root")) {
        if (verbose_)  std::cout << Info() << "Opening " << src << std::endl;
        chain_->Add(src);

    } else if (src.EndsWith(".txt")) {
        std::string line;
        ifstream ifs(src);
        while (std::getline(ifs,line)) {
            TString tstring(line);
            if (!tstring.BeginsWith("#") && tstring.EndsWith(".root")) {
                if (verbose_)  std::cout << Info() << "Opening " << tstring << std::endl;
                chain_->Add(tstring);
            }
        }
        ifs.close();

    } else {
        std::cout << Error() << "Input source should be either a .root file or a .txt file." << std::endl;
        return 1;
    }
    return 0;
}

// _____________________________________________________________________________
// Set up all the patterns
int PatternGenerator::readTree() {
    Long64_t nentries = chain_->GetEntries();
    if (nentries <= 0) {
        std::cout << Error() << "Input source has zero entry." << std::endl;
        return 1;
    }

    if (nEvents_ > (int) nentries)
        nEvents_ = nentries;
    if (verbose_)  std::cout << Info() << "Reading " << nEvents_ << " events." << std::endl;

    std::vector<unsigned> *           vb_iModCols         = 0;
    std::vector<unsigned> *           vb_iModRows         = 0;
    std::vector<unsigned> *           vb_modId            = 0;
    std::vector<unsigned> *           vb_nhits            = 0;
    std::vector<std::vector<int> > *  vb_hitCols          = 0;
    std::vector<std::vector<int> > *  vb_hitRows          = 0;
    std::vector<std::vector<int> > *  vb_hitTrkIds        = 0;
    std::vector<float> *              vb_simPt            = 0;
    std::vector<float> *              vb_simEta           = 0;
    std::vector<float> *              vb_simPhi           = 0;
    std::vector<int> *                vb_trkId            = 0;
    unsigned                          v_event             = 0;

    chain_->SetBranchStatus("*", 0);
    chain_->SetBranchStatus("TTStubs_iModCols" , 1);
    chain_->SetBranchStatus("TTStubs_iModRows" , 1);
    chain_->SetBranchStatus("TTStubs_modId"    , 1);
    chain_->SetBranchStatus("TTStubs_nhits"    , 1);
    chain_->SetBranchStatus("TTStubs_hitCols"  , 1);
    chain_->SetBranchStatus("TTStubs_hitRows"  , 1);
    chain_->SetBranchStatus("TTStubs_hitTrkIds", 1);
    chain_->SetBranchStatus("TTStubs_simPt"    , 1);
    chain_->SetBranchStatus("TTStubs_simEta"   , 1);
    chain_->SetBranchStatus("TTStubs_simPhi"   , 1);
    chain_->SetBranchStatus("TTStubs_trkId"    , 1);
    chain_->SetBranchStatus("event"            , 1);

    // Unfortunately, very different semantics for Branch(...) vs SetBranchAddress(...)
    chain_->SetBranchAddress("TTStubs_iModCols" , &(vb_iModCols));
    chain_->SetBranchAddress("TTStubs_iModRows" , &(vb_iModRows));
    chain_->SetBranchAddress("TTStubs_modId"    , &(vb_modId));
    chain_->SetBranchAddress("TTStubs_nhits"    , &(vb_nhits));
    chain_->SetBranchAddress("TTStubs_hitCols"  , &(vb_hitCols));
    chain_->SetBranchAddress("TTStubs_hitRows"  , &(vb_hitRows));
    chain_->SetBranchAddress("TTStubs_hitTrkIds", &(vb_hitTrkIds));
    chain_->SetBranchAddress("TTStubs_simPt"    , &(vb_simPt));
    chain_->SetBranchAddress("TTStubs_simEta"   , &(vb_simEta));
    chain_->SetBranchAddress("TTStubs_simPhi"   , &(vb_simPhi));
    chain_->SetBranchAddress("TTStubs_trkId"    , &(vb_trkId));
    chain_->SetBranchAddress("event"            , &(v_event));


    allPatterns_.clear();
    if (nEvents_ >= (Long64_t) allPatterns_.max_size()) {
        std::cout << Error() << "Number of events is more than the max_size of a std::vector." << std::endl;
        return 1;
    }

    for (Long64_t ievt=0; ievt<nEvents_; ++ievt) {
        chain_->GetEntry(ievt);
        unsigned nstubs = vb_modId->size();
        if (verbose_>1 && ievt%1000==0)  std::cout << Debug() << Form("... Processing event: %7lld, keeping: %7ld", ievt, allPatterns_.size()) << std::endl;
        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " # stubs: " << nstubs << " [# patterns: " << allPatterns_.size() << "]." << std::endl;
        if (!nstubs)  // skip if no stub
            continue;

        // Take the sim-matched particle info from the outermost stub
        float simPt = vb_simPt->back();
        float simEta = vb_simEta->back();
        float simPhi = vb_simPhi->back();
        int trkId = vb_trkId->back();
        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " simPt: " << simPt << " simEta: " << simEta << " simPhi: " << simPhi << std::endl;

        bool sim = (minPt_  <= simPt  && simPt  <= maxPt_  &&
                    minEta_ <= simEta && simEta <= maxEta_ &&
                    minPhi_ <= simPhi && simPhi <= maxPhi_);
        if (!sim && doFilter_)  // skip if sim-matched particle is out of defined range
            continue;

        // Find layers with at least one sim-matched hit
        // If more than one sim-matched hit in a layer, take only the first hit
        Pattern::vuint32_t goodLayers;
        Pattern::vuint64_t goodHitIds;
        for (unsigned l=0; l<nstubs; ++l) {
            unsigned moduleId = vb_modId->at(l);
            unsigned ncols = vb_iModCols->at(l);
            unsigned nrows = vb_iModRows->at(l);
            unsigned nhits = vb_nhits->at(l);
            int col = 0;
            int row = 0;
            assert(nhits > 0);
            if (verbose_>2)  std::cout << Debug() << "... ... stub: " << l << " moduleId: " << moduleId << " trkId: " << trkId << " # hits: " << nhits << std::endl;

            sim = (vb_trkId->at(l) == trkId);
            if (!sim && doFilter_)  // skip if stub is not associated to the same sim-matched particle
                continue;

            for (unsigned ii=0; ii<nhits; ++ii) {
                col = vb_hitCols->at(l).at(ii);
                row = vb_hitRows->at(l).at(ii);
                sim = (vb_hitTrkIds->at(l).at(ii) == trkId);
                if (verbose_>2)  std::cout << Debug() << "... ... ... hit: " << ii << " col: " << col << " row: " << row << " sim: " << sim << std::endl;

                if (sim)  // stop as soon as a hit is associated to the same sim-matched particle
                    break;
            }
            if (!sim && doFilter_)  // skip if a sim-matched hit is never found
                continue;

            uint32_t layer = Pattern::decodeLayer(moduleId);
            uint32_t mlayer = layerMap_.at(layer);  // must exist
            unsigned count = std::count(goodLayers.begin(), goodLayers.end(), mlayer);
            if (verbose_>2)  std::cout << Debug() << "... ... stub: " << l << " layer: " << layer << " mlayer: " << mlayer << " count: " << count << std::endl;

            if (count>0 && doFilter_)  // skip if this layer is already considered in goodLayers
                continue;

            // Set to lower resolution according to nSubLadders and nSubModules
            col /= (ncols / std::min((int) ncols, nSubLadders_));
            row /= (nrows / std::min((int) nrows, nSubModules_));
            uint64_t hitId = Pattern::encodeHitId(moduleId, col, row);
            if (verbose_>2)  std::cout << Debug() << "... ... stub: " << l << " hitId: " << hitId << " col: " << col << " row: " << row << " ncols: " << ncols << " nrows: " << nrows << std::endl;

            goodLayers.push_back(mlayer);
            goodHitIds.push_back(hitId);
        }
        if (verbose_>2) {
            std::cout << Debug() << "... evt: " << ievt << " # goodHitIds: " << goodHitIds.size() << " goodLayers: ";
            std::copy(goodLayers.begin(), goodLayers.end(), std::ostream_iterator<uint64_t>(std::cout, " "));
            std::cout << std::endl;
        }

        if ((int) goodLayers.size()<nLayers_ && doFilter_)  // skip if number of goodLayers less than requirement
            continue;

        // Only consider up to the first nLayers_
        uint32_t hash = hashFileEvent(chain_->GetCurrentFile()->GetName(), v_event);
        Pattern patt(nLayers_, hash);
        for (int l=0; l<nLayers_; ++l) {
            if (!doFilter_ && l==(int) goodHitIds.size())  break;
            uint64_t hitId = goodHitIds.at(l);

            HitIdBoolMap::const_iterator found = hitIdMap_.find(hitId);
            if (found == hitIdMap_.end()) {
                std::shared_ptr<bool> abool(new bool(false));
                hitIdMap_.insert(std::make_pair(hitId, abool) );
                patt.setHit(l, hitId, abool.get());  // the address of the bool

            } else {
                patt.setHit(l, hitId, found->second.get());  // the address of the bool
            }

            if (verbose_>2)  std::cout << Debug() << "... ... bin: " << l << " hitId: " << hitId << " # hitIds: " << hitIdMap_.size() << std::endl;
            if (verbose_>2)  std::cout << Debug() << "... ... bin: " << l << " lay: " << Pattern::decodeHitLayer(hitId) << " lad: " << Pattern::decodeHitLadder(hitId) << " mod: " << Pattern::decodeHitModule(hitId) << " col: " << Pattern::decodeHitSubLadder(hitId) << " row: " << Pattern::decodeHitSubModule(hitId) << std::endl;
        }

        allPatterns_.push_back(patt);  // save the patterns
    }
    if (verbose_>1)  std::cout << Debug() << "Identified " << hitIdMap_.size() << " possible 'hitId's." << std::endl;

    // Resolve ambiguity and process the patterns with DC bits, sector customizations, etc...
    goodPatterns_.clear();
    for (unsigned i=0; i<allPatterns_.size(); ++i) {
        Pattern& patt = allPatterns_.at(i);  // pass by reference
        const Pattern::pattern8_t& pattId = patt.getPatternId();

        PatternIdIndexMap::const_iterator found = patternIdMap_.find(pattId);
        if (found == patternIdMap_.end()) {  // new pattern
            goodPatterns_.push_back(patt);
            patternIdMap_.insert(std::make_pair(pattId, goodPatterns_.size()-1) );

        } else {  // old pattern, increase frequency
            uint32_t index = found->second;
            goodPatterns_.at(index).touch();
        }
        if (verbose_>2) {
            std::cout << Debug() << "... patt: " << i << " ";
            std::copy(pattId.begin(), pattId.end(), std::ostream_iterator<uint64_t>(std::cout, " "));
            std::cout << " freq: " << patt.frequency() << std::endl;
        }
    }
    if (verbose_)  std::cout << Info() << "Made " << goodPatterns_.size() << " final patterns, out of " << allPatterns_.size() << " inclusive, full-resolution patterns." << std::endl;

    std::sort(goodPatterns_.begin(), goodPatterns_.end(), sortByFrequency);
    return 0;
}


// Output patterns into a TTree
int PatternGenerator::writeTree(TString out) {
    if (nPatterns_ > (int) goodPatterns_.size())
        nPatterns_ = goodPatterns_.size();
    if (verbose_)  std::cout << Info() << "Recreating " << out << " with " << nPatterns_ << " patterns." << std::endl;
    TFile* tfile = TFile::Open(out, "RECREATE");
    TTree* ttree = new TTree("patternBank", "");

    int nLayers = nLayers_;
    uint32_t hash;
    uint32_t frequency;
    uint64_t pattern[nLayers_];
    uint8_t bit[nLayers_];

    ttree->Branch("nLayers", &nLayers, "nLayers/I");
    ttree->Branch("hash", &hash, "hash/i");
    ttree->Branch("frequency", &frequency, "frequency/i");
    ttree->Branch("pattern", pattern, "pattern[nLayers]/l");  // l for ULong64_t
    ttree->Branch("bit", bit, "bit[nLayers]/b");  // b for UChar_t

    for (int i=0; i<nPatterns_; ++i) {
        const Pattern& patt = goodPatterns_.at(i);

        for (int l=0; l<nLayers_; ++l) {
            pattern[l] = patt.getHitId(l);
            bit[l] = patt.getHitBit(l);
        }
        hash = patt.hash();
        frequency = patt.frequency();

        ttree->Fill();
    }
    assert(ttree->GetEntries() == nPatterns_);
    tfile->Write();
    delete ttree;
    delete tfile;
    return 0;
}

// _____________________________________________________________________________
// Main driver
int PatternGenerator::run(TString src, TString out, TString layout) {
    int exitcode = 0;
    TStopwatch timer;
    if (doTiming_)  timer.Start();

    exitcode = readSectorFile(layout);
    if (exitcode)  return exitcode;
    if (doTiming_) { timer.Stop(); timer.Continue(); timer.Print(); }

    exitcode = readFile(src);
    if (exitcode)  return exitcode;
    if (doTiming_) { timer.Stop(); timer.Continue(); timer.Print(); }

    exitcode = readTree();
    if (exitcode)  return exitcode;
    if (doTiming_) { timer.Stop(); timer.Continue(); timer.Print(); }

    exitcode = writeTree(out);
    if (exitcode)  return exitcode;
    if (doTiming_) { timer.Stop(); timer.Continue(); timer.Print(); }

    return exitcode;
}
