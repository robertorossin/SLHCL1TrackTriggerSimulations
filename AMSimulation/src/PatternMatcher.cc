#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternMatcher.h"

#include <type_traits>

static const unsigned MAX_NLAYERS = pattern_type().size();  // ought to be 8


// _____________________________________________________________________________
// Read the input ntuples
int PatternMatcher::readFile(TString src) {
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
// Read from the pattern bank
int PatternMatcher::readPatterns_vector(TString src) {
    if (src.EndsWith(".root")) {
        if (verbose_)  std::cout << Info() << "Opening " << src << std::endl;
        TFile* tfile = TFile::Open(src);
        TTree* ttree = (TTree*) tfile->Get(bankName_);
        if (ttree == 0) {
            std::cout << Error() << "Unable to retrieve the TTree." << std::endl;
        }

        // For reading
        typedef unsigned char  unsigned8;
        unsigned8                 frequency;
        std::vector<unsigned> *   superstripIds  = 0;

        ttree->SetBranchAddress("frequency"    , &frequency);
        ttree->SetBranchAddress("superstripIds", &superstripIds);

        long long nentries = ttree->GetEntries();
        if (nentries > maxPatterns_)
            nentries = maxPatterns_;

        if (verbose_)  std::cout << Info() << "Reading " << nentries << " patterns." << std::endl;

        // _____________________________________________________________________
        // Loop over all patterns

        inputPatterns_vector_.clear();
        inputPatterns_vector_.resize(nentries);
        std::vector<key_type> superstripHashes;  // currently key_type == addr_type
        superstripHashes.resize(MAX_NLAYERS);

        for (long long ievt=0; ievt<nentries; ++ievt) {
            ttree->GetEntry(ievt);
            assert(superstripIds->size() == MAX_NLAYERS);

            // Convert each ssId to a hash key
            for (unsigned i=0; i<superstripIds->size(); ++i) {
                if (superstripIds->at(i) != 0)
                    superstripHashes.at(i) = hasher_ -> hash(superstripIds->at(i));
            }

            //std::copy(superstripIds->begin(), superstripIds->end(), inputPatterns_vector_.at(ievt).begin());
            std::copy(superstripHashes.begin(), superstripHashes.end(), inputPatterns_vector_.at(ievt).begin());

            if (verbose_>2)  std::cout << Debug() << "... patt: " << ievt << "  " << inputPatterns_vector_.at(ievt) << " freq: " << (unsigned) frequency << std::endl;
        }

        if ((size_t) nentries != inputPatterns_vector_.size()) {
            std::cout << Error() << "Failed to read all patterns from the root file: " << src << std::endl;
            return 1;
        }

        delete ttree;
        delete tfile;
    }

    if (inputPatterns_vector_.empty()) {
        std::cout << Error() << "Pattern bank is empty." << std::endl;
        return 1;
    }
    return 0;
}


// _____________________________________________________________________________
// Do pattern recognition
int PatternMatcher::makeRoads_vector() {
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
    chain_->SetBranchAddress("TTStubs_modId"    , &(vb_modId));
    chain_->SetBranchAddress("TTStubs_nhits"    , &(vb_nhits));
    chain_->SetBranchAddress("TTStubs_simPt"    , &(vb_simPt));
    chain_->SetBranchAddress("TTStubs_simEta"   , &(vb_simEta));
    chain_->SetBranchAddress("TTStubs_simPhi"   , &(vb_simPhi));
    chain_->SetBranchAddress("TTStubs_trkId"    , &(vb_trkId));

    // In addition, keep genParticle info
    std::vector<float> *          vp_pt         = 0;
    std::vector<float> *          vp_eta        = 0;
    std::vector<float> *          vp_phi        = 0;
    std::vector<int> *            vp_charge     = 0;

    chain_->SetBranchStatus("genParts_pt"       , 1);
    chain_->SetBranchStatus("genParts_eta"      , 1);
    chain_->SetBranchStatus("genParts_phi"      , 1);
    chain_->SetBranchStatus("genParts_charge"   , 1);

    chain_->SetBranchAddress("genParts_pt"      , &(vp_pt));
    chain_->SetBranchAddress("genParts_eta"     , &(vp_eta));
    chain_->SetBranchAddress("genParts_phi"     , &(vp_phi));
    chain_->SetBranchAddress("genParts_charge"  , &(vp_charge));

    allRoads_.clear();
    allGenParts_.clear();

    // Make sure the map has already been set up
    assert(! inputPatterns_vector_.empty());
    assert(std::is_pod<TTSuperstrip>::value && std::is_pod<TTPattern>::value && std::is_pod<TTHit>::value);


    // _________________________________________________________________________
    // Loop over all events

    std::map<addr_type, std::vector<TTHit> > superstripHits;  // key: superstrip id, values: vector of hits
    std::vector<TTHit> hits;
    std::vector<std::vector<TTHit> > hitses;  // just like hits, but before serialized

    int nPassed = 0, nKept = 0;

    for (long long ievt=0; ievt<nEvents_; ++ievt) {
        Long64_t local_entry = chain_->LoadTree(ievt);  // for TChain
        if (local_entry < 0)  break;
        chain_->GetEntry(ievt);

        unsigned nstubs = vb_modId->size();
        if (verbose_>1 && ievt%5000==0)  std::cout << Debug() << Form("... Processing event: %7lld, triggering: %7i", ievt, nPassed) << std::endl;
        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " # stubs: " << nstubs << " [# roads: " << allRoads_.size() << "]" << std::endl;

        if (!nstubs) {  // skip if no stub
            allRoads_.push_back(std::vector<TTRoad>());
            allGenParts_.push_back(std::vector<genPart>());
            ++nKept;
            continue;
        }

        // _____________________________________________________________________
        // Start pattern recognition
        superstripHits.clear();

        // Loop over reconstructed stubs
        for (unsigned l=0; l<nstubs; ++l) {
            unsigned moduleId = vb_modId->at(l);
            float coordx = vb_coordx->at(l);
            float coordy = vb_coordy->at(l);

            // Use half-strip unit
            id_type col = halfStripRound(coordy);
            id_type row = halfStripRound(coordx);

            // Find superstrip address
            col = arbiter_ -> subladder(moduleId, col);
            row = arbiter_ -> submodule(moduleId, row);
            addr_type ssId = encodeSuperstripId(moduleId, col, row);

            // Convert each ssId to a hash key
            key_type hash = hasher_ -> hash(ssId);

            // Position and rough pt
            float x = vb_x->at(l);
            float y = vb_y->at(l);
            float z = vb_z->at(l);
            float pt = vb_roughPt->at(l);

            // Create a hit
            //superstripHits[ssId].emplace_back(TTHit{x, y, z, 0., 0., 0., -1, pt, ssId, 1});  // POD type constructor
            superstripHits[hash].emplace_back(TTHit{x, y, z, 0., 0., 0., -1, pt, ssId, 1});  // POD type constructor

            if (verbose_>2)  std::cout << Debug() << "... ... stub: " << l << " moduleId: " << moduleId << " col: " << col << " row: " << row << " ssId: " << ssId << " trkId: " << vb_trkId->at(l) << std::endl;
        }

        // _____________________________________________________________________
        // Create roads
        std::vector<TTRoad> roadsInThisEvent;

        std::map<addr_type, std::vector<TTHit> >::iterator found;

        for (std::vector<pattern_type>::const_iterator itv = inputPatterns_vector_.begin();
             itv != inputPatterns_vector_.end(); ++itv) {
            hitses.clear();

            for (pattern_type::const_iterator it = itv->begin(); it != itv->end(); ++it) {
                if (isFakeSuperstripId(*it)) {
                    hitses.push_back(std::vector<TTHit>());

                } else {
                    found = superstripHits.find(*it);
                    if (found != superstripHits.end()) {
                        if ((int) found -> second.size() > maxHits_)
                            found -> second.resize(maxHits_);
                        hitses.push_back(found -> second);
                    }
                }
            }

            if ((int) hitses.size() >= (nLayers_ - po.nMisses)) {  // if number of hits more than the requirement
                hits.clear();
                for (unsigned i=0; i<hitses.size(); ++i) {
                    hits.insert(hits.end(), hitses.at(i).begin(), hitses.at(i).end());
                }
                roadsInThisEvent.emplace_back(hitses.size(), itv - inputPatterns_vector_.begin(), *itv, hits);

                if (verbose_>2)  std::cout << Debug() << "... ... road: " << roadsInThisEvent.size() - 1 << " " << roadsInThisEvent.back() << std::endl;
            }
        }

        if (! roadsInThisEvent.empty())
            ++nPassed;

        // _____________________________________________________________________
        // In addition, keep genParticle info

        unsigned nparts = vp_pt->size();
        std::vector<genPart> genPartsInThisEvent;
        for (unsigned l=0; l<nparts; ++l) {
            genPart part;
            part.pt     = vp_pt->at(l);
            part.eta    = vp_eta->at(l);
            part.phi    = vp_phi->at(l);
            part.charge = vp_charge->at(l);
            genPartsInThisEvent.push_back(part);
        }

        allRoads_.push_back(roadsInThisEvent);
        allGenParts_.push_back(genPartsInThisEvent);
        ++nKept;
    }
    assert((size_t) nKept == allRoads_.size());
    assert(allRoads_.size() == allGenParts_.size());

    if (verbose_)  std::cout << Info() << "Processed " << nEvents_ << " events, kept " << nKept << ", triggered on " << nPassed << " events." << std::endl;

    return 0;
}


// _____________________________________________________________________________
// Output roads into a TTree
int PatternMatcher::writeRoads_vector(TString out) {
    if (!out.EndsWith(".root")) {
        std::cout << Error() << "Output filename must be .root" << std::endl;
        return 1;
    }

    const long long nentries = allRoads_.size();
    if (verbose_)  std::cout << Info() << "Recreating " << out << " with " << nentries << " events." << std::endl;
    TFile* tfile = TFile::Open(out, "RECREATE");
    tfile->mkdir("ntupler")->cd();
    TTree* ttree = new TTree("tree", "");

    typedef unsigned char  unsigned8;
    typedef unsigned short unsigned16;
    std::auto_ptr<std::vector<unsigned8> >                vr_nHitLayers       (new std::vector<unsigned8>());
    std::auto_ptr<std::vector<unsigned> >                 vr_bankIndex        (new std::vector<unsigned>());
    //
    std::auto_ptr<std::vector<std::vector<unsigned> > >   vr_patternIds       (new std::vector<std::vector<unsigned> >());  // now these are the hashes of the superstripIds
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitXs            (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitYs            (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitZs            (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitXErrors       (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitYErrors       (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitZErrors       (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<int> > >        vr_hitCharges       (new std::vector<std::vector<int> >());
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitPts           (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<unsigned> > >   vr_hitSuperstripIds (new std::vector<std::vector<unsigned> >());
    std::auto_ptr<std::vector<std::vector<unsigned16> > > vr_hitSuperstripBits(new std::vector<std::vector<unsigned16> >());

    ttree->Branch(prefixRoad_ + "nHitLayers"        + suffix_, &(*vr_nHitLayers));
    ttree->Branch(prefixRoad_ + "bankIndex"         + suffix_, &(*vr_bankIndex));
    //
    ttree->Branch(prefixRoad_ + "patternIds"        + suffix_, &(*vr_patternIds));
    ttree->Branch(prefixRoad_ + "hitXs"             + suffix_, &(*vr_hitXs));
    ttree->Branch(prefixRoad_ + "hitYs"             + suffix_, &(*vr_hitYs));
    ttree->Branch(prefixRoad_ + "hitZs"             + suffix_, &(*vr_hitZs));
    ttree->Branch(prefixRoad_ + "hitXErrors"        + suffix_, &(*vr_hitXErrors));
    ttree->Branch(prefixRoad_ + "hitYErrors"        + suffix_, &(*vr_hitYErrors));
    ttree->Branch(prefixRoad_ + "hitZErrors"        + suffix_, &(*vr_hitZErrors));
    ttree->Branch(prefixRoad_ + "hitCharges"        + suffix_, &(*vr_hitCharges));
    ttree->Branch(prefixRoad_ + "hitPts"            + suffix_, &(*vr_hitPts));
    ttree->Branch(prefixRoad_ + "hitSuperstripIds"  + suffix_, &(*vr_hitSuperstripIds));
    ttree->Branch(prefixRoad_ + "hitSuperstripBits" + suffix_, &(*vr_hitSuperstripBits));

    std::vector<unsigned>   patternIds;
    std::vector<float>      hitXs;
    std::vector<float>      hitYs;
    std::vector<float>      hitZs;
    std::vector<float>      hitXErrors;
    std::vector<float>      hitYErrors;
    std::vector<float>      hitZErrors;
    std::vector<int>        hitCharges;
    std::vector<float>      hitPts;
    std::vector<unsigned>   hitSuperstripIds;
    std::vector<unsigned16> hitSuperstripBits;

    // In addition, keep genParticle info
    std::auto_ptr<std::vector<float> > vp_pt    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vp_eta   (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vp_phi   (new std::vector<float>());
    std::auto_ptr<std::vector<int> >   vp_charge(new std::vector<int>());

    ttree->Branch("genParts_pt"    , &(*vp_pt));
    ttree->Branch("genParts_eta"   , &(*vp_eta));
    ttree->Branch("genParts_phi"   , &(*vp_phi));
    ttree->Branch("genParts_charge", &(*vp_charge));


    // _________________________________________________________________________
    // Loop over all roads
    for (long long ievt=0; ievt<nentries; ++ievt) {
        if (verbose_>1 && ievt%50000==0)  std::cout << Debug() << Form("... Writing event: %7lld", ievt) << std::endl;
        vr_nHitLayers       ->clear();
        vr_bankIndex        ->clear();
        //
        vr_patternIds       ->clear();
        vr_hitXs            ->clear();
        vr_hitYs            ->clear();
        vr_hitZs            ->clear();
        vr_hitXErrors       ->clear();
        vr_hitYErrors       ->clear();
        vr_hitZErrors       ->clear();
        vr_hitCharges       ->clear();
        vr_hitPts           ->clear();
        vr_hitSuperstripIds ->clear();
        vr_hitSuperstripBits->clear();

        vp_pt               ->clear();
        vp_eta              ->clear();
        vp_phi              ->clear();
        vp_charge           ->clear();

        const std::vector<TTRoad>& roadsInThisEvent = allRoads_.at(ievt);
        unsigned nroads = roadsInThisEvent.size();

        for (unsigned i=0; i<nroads; ++i) {
            if ((int) i >= maxRoads_)  break;

            patternIds       .clear();
            hitXs            .clear();
            hitYs            .clear();
            hitZs            .clear();
            hitXErrors       .clear();
            hitYErrors       .clear();
            hitZErrors       .clear();
            hitCharges       .clear();
            hitPts           .clear();
            hitSuperstripIds .clear();
            hitSuperstripBits.clear();

            const TTRoad& road = roadsInThisEvent.at(i);
            const pattern_type&       patt = road.patternId();
            const std::vector<TTHit>& hits = road.getHits();

            for (unsigned j=0; j<patt.size(); ++j) {
                patternIds.push_back(patt.at(j));
            }

            for (unsigned j=0; j<hits.size(); ++j) {
                const TTHit& hit = hits.at(j);
                hitXs.push_back(hit.x);
                hitYs.push_back(hit.y);
                hitZs.push_back(hit.z);
                hitXErrors.push_back(hit.xError);
                hitYErrors.push_back(hit.yError);
                hitZErrors.push_back(hit.zError);
                hitCharges.push_back(hit.charge);
                hitPts.push_back(hit.pt);
                hitSuperstripIds.push_back(hit.superstripId);
                hitSuperstripBits.push_back(hit.superstripBit);
            }

            vr_nHitLayers       ->push_back(road.nHitLayers());
            vr_bankIndex        ->push_back(road.bankIndex());
            //
            vr_patternIds       ->push_back(patternIds);
            vr_hitXs            ->push_back(hitXs);
            vr_hitYs            ->push_back(hitYs);
            vr_hitZs            ->push_back(hitZs);
            vr_hitXErrors       ->push_back(hitXErrors);
            vr_hitYErrors       ->push_back(hitYErrors);
            vr_hitZErrors       ->push_back(hitZErrors);
            vr_hitCharges       ->push_back(hitCharges);
            vr_hitPts           ->push_back(hitPts);
            vr_hitSuperstripIds ->push_back(hitSuperstripIds);
            vr_hitSuperstripBits->push_back(hitSuperstripBits);
        }

        const std::vector<genPart>& genPartsInThisEvent = allGenParts_.at(ievt);
        unsigned nparts = genPartsInThisEvent.size();

        for (unsigned i=0; i<nparts; ++i) {
            const genPart& part = genPartsInThisEvent.at(i);
            vp_pt ->push_back(part.pt);
            vp_eta->push_back(part.eta);
            vp_phi->push_back(part.phi);
            vp_charge->push_back(part.charge);
        }

        ttree->Fill();
        assert(vr_hitXs->size() == nroads);
        assert(vp_pt->size() == nparts);
    }
    assert(ttree->GetEntries() == nentries);

    tfile->Write();
    delete ttree;
    delete tfile;
    return 0;
}

// _____________________________________________________________________________
// Private functions
// none


// _____________________________________________________________________________
// Main driver
int PatternMatcher::run(TString out, TString src, TString bank) {
    gROOT->ProcessLine("#include <vector>");  // how is it not loaded?

    int exitcode = 0;
    Timing(1);

    exitcode = readPatterns_vector(bank);
    if (exitcode)  return exitcode;
    Timing();

    exitcode = readFile(src);
    if (exitcode)  return exitcode;
    Timing();

    exitcode = makeRoads_vector();
    if (exitcode)  return exitcode;
    Timing();

    exitcode = writeRoads_vector(out);
    if (exitcode)  return exitcode;
    Timing();

    chain_->Reset();
    return exitcode;
}
