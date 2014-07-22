#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternMatcher.h"

#include <type_traits>

static const unsigned MAX_NLAYERS = pattern_type().size();  // ought to be 8

struct genPart {
    float pt;
    float eta;
    float phi;
    int charge;
};

class TTRoadWriter {
  public:
    TTRoadWriter(TString out, TString prefix, TString suffix);
    ~TTRoadWriter();

    void fill(const std::vector<TTRoad>& roads, const std::vector<genPart>& genParts);
    long long write();

  private:
    void init(TString out, TString prefix, TString suffix);

    TFile* tfile;
    TTree* ttree;

    // Roads
    typedef unsigned short unsigned16;
    std::auto_ptr<std::vector<unsigned16> >               vr_nHitLayers;
    std::auto_ptr<std::vector<unsigned> >                 vr_bankIndex;
    //
    std::auto_ptr<std::vector<std::vector<unsigned> > >   vr_patternIds; // now these are the hashes of the superstripIds
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitXs;
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitYs;
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitZs;
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitXErrors;
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitYErrors;
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitZErrors;
    std::auto_ptr<std::vector<std::vector<int> > >        vr_hitCharges;
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitPts;
    std::auto_ptr<std::vector<std::vector<unsigned> > >   vr_hitSuperstripIds;
    std::auto_ptr<std::vector<std::vector<unsigned16> > > vr_hitSuperstripBits;

    // Roads (inner vectors)
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
    std::auto_ptr<std::vector<float> > vp_pt;
    std::auto_ptr<std::vector<float> > vp_eta;
    std::auto_ptr<std::vector<float> > vp_phi;
    std::auto_ptr<std::vector<int> >   vp_charge;
};


bool sort_by_first(const std::pair<key_type, key_type>& lhs, const std::pair<key_type, key_type>& rhs) {
    return lhs.first < rhs.first;
}

bool compare_by_first(const std::pair<key_type, key_type>& lhs, const std::pair<key_type, key_type>& rhs) {
    return lhs.first == rhs.first;
}

template<typename ForwardIterator, typename Size, typename BinaryPredicate>
ForwardIterator majority_logic(ForwardIterator first, ForwardIterator last,
                               Size n, BinaryPredicate pred) {
    // Not enough elements to begin with
    if ((last - first) < n)
        return first;

    Size count = 1;
    ForwardIterator dest = first, next = first+1;

    while (next != last) {
        if (!pred(*first++, *next++) ) {
            if (count >= n) {
                // Do the copying
                while (count > 0) {
                    *dest++ = std::move(*(first - count));
                    --count;
                }
            }
            count = 1;
        } else {
            ++count;
        }
    }

    ++first;
    if (count >= n) {
        // Do the copying
        while (count > 0) {
            *dest++ = std::move(*(first - count));
            --count;
        }
    }
    return dest;
}


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
        count_type               frequency;
        std::vector<addr_type> * superstripIds  = 0;

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

        std::vector<key_type> superstripHashes;
        superstripHashes.resize(MAX_NLAYERS);

        for (long long ievt=0; ievt<nentries; ++ievt) {
            ttree->GetEntry(ievt);
            if (frequency < minFrequency_)
                break;

            assert(superstripIds->size() == MAX_NLAYERS);

            // Convert each ssId to a hash key
            for (unsigned i=0; i<superstripIds->size(); ++i) {
                if (superstripIds->at(i) != 0)
                    superstripHashes.at(i) = hasher_ -> hash(superstripIds->at(i));
            }

            //std::copy(superstripIds->begin(), superstripIds->end(), inputPatterns_vector_.at(ievt).begin());
            std::copy(superstripHashes.begin(), superstripHashes.end(), inputPatterns_vector_.at(ievt).begin());

            if (verbose_>3)  std::cout << Debug() << "... patt: " << ievt << "  " << inputPatterns_vector_.at(ievt) << " freq: " << (unsigned) frequency << std::endl;
        }

        if ((size_t) nentries != inputPatterns_vector_.size()) {
            std::cout << Error() << "Failed to read all patterns from the root file: " << inputPatterns_vector_.size() << " vs " << nentries << std::endl;
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
int PatternMatcher::makeRoads_vector(TString out) {

    // Make sure the map has already been set up
    assert(! inputPatterns_vector_.empty());
    assert(std::is_pod<TTSuperstrip>::value && std::is_pod<TTPattern>::value && std::is_pod<TTHit>::value);

    // For reading
    if (verbose_)  std::cout << Info() << "Reading " << nEvents_ << " events" << std::endl;

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

    // For writing
    if (verbose_)  std::cout << Info() << "Recreating " << out << std::endl;
    TTRoadWriter writer(out, prefixRoad_, suffix_);


    // _________________________________________________________________________
    // Loop over all events

    std::map<key_type, std::vector<TTHit> > superstripHits;  // key: superstrip hash, values: vector of hits
    std::vector<TTHit> hits;
    std::vector<std::vector<TTHit> > hitses;  // just like hits, but before serialized

    std::vector<bool> superstripBooleans;
    superstripBooleans.resize(hasher_ -> hash(fakeSuperstripId_) + 1);

    key_type fakeSuperstripHash = hasher_ -> hash(fakeSuperstripId_);
    key_type fakeSuperstripHash1 = hasher_ -> hash(fakeSuperstripId1_);

    std::vector<TTRoad> roads;
    roads.reserve(200);
    std::vector<genPart> genParts;
    genParts.reserve(50);

    int nPassed = 0, nKept = 0;
    for (long long ievt=0; ievt<nEvents_; ++ievt) {
        Long64_t local_entry = chain_->LoadTree(ievt);  // for TChain
        if (local_entry < 0)  break;
        chain_->GetEntry(ievt);

        unsigned nstubs = vb_modId->size();
        if (verbose_>1 && ievt%5000==0)  std::cout << Debug() << Form("... Processing event: %7lld, keeping: %7i, triggering: %7i", ievt, nKept, nPassed) << std::endl;
        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " # stubs: " << nstubs << std::endl;

        if (!nstubs) {  // skip if no stub
            writer.fill(std::vector<TTRoad>(), std::vector<genPart>());
            ++nKept;
            continue;
        }

        // _____________________________________________________________________
        // Start pattern recognition
        superstripHits.clear();
        for (unsigned l=0; l<superstripBooleans.size(); ++l)
            superstripBooleans.at(l) = false;

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
            superstripBooleans.at(hash) = true;

            if (verbose_>2)  std::cout << Debug() << "... ... stub: " << l << " moduleId: " << moduleId << " col: " << col << " row: " << row << " ssId: " << ssId << " trkId: " << vb_trkId->at(l) << std::endl;
        }

        // _____________________________________________________________________
        // Create roads
        roads.clear();

        std::map<key_type, std::vector<TTHit> >::iterator found;
        int nMisses = 0;

        for (std::vector<pattern_type>::const_iterator itv = inputPatterns_vector_.begin();
             itv != inputPatterns_vector_.end(); ++itv) {
            nMisses = 0;

            for (pattern_type::const_iterator it = itv->begin(); it != itv->begin() + nLayers_; ++it) {
                //assert(*it != 0);
                if ((*it != fakeSuperstripHash)  &&
                    (*it != fakeSuperstripHash1) &&
                    (! superstripBooleans.at(*it)))
                        ++nMisses;

                // if number of allowed misses more than the requirement
                if (nMisses > po.nMisses)
                    break;
            }

            if (verbose_>2 && (int) hitses.size() == po.nLayers)  std::cout << Debug() << "... ... # hitses: " << hitses.size() << " # misses: " << nMisses << std::endl;

            if (nMisses <= po.nMisses) {
                hits.clear();
                hitses.clear();

                for (pattern_type::const_iterator it = itv->begin(); it != itv->begin() + nLayers_; ++it) {
                    if ((*it != fakeSuperstripHash)  &&
                        (*it != fakeSuperstripHash1)) {
                        found = superstripHits.find(*it);
                        if (found != superstripHits.end()) {
                            hitses.push_back(found -> second);
                            if ((int) hitses.back().size() > maxHits_)
                                hitses.back().resize(maxHits_);
                        }

                    } else {
                        // Fake superstrip
                        hitses.push_back(std::vector<TTHit>());
                    }
                }

                for (unsigned i=0; i<hitses.size(); ++i) {
                    hits.insert(hits.end(), hitses.at(i).begin(), hitses.at(i).end());
                }
                roads.emplace_back(hitses.size(), itv - inputPatterns_vector_.begin(), *itv, hits);

                if (verbose_>2)  std::cout << Debug() << "... ... road: " << roads.size() - 1 << " " << roads.back() << std::endl;
            }

            if ((int) roads.size() >= maxRoads_)
                break;
        }

        if (! roads.empty())
            ++nPassed;

        // _____________________________________________________________________
        // In addition, keep genParticle info
        genParts.clear();

        unsigned nparts = vp_pt->size();
        for (unsigned l=0; l<nparts; ++l) {
            genPart part;
            part.pt     = vp_pt->at(l);
            part.eta    = vp_eta->at(l);
            part.phi    = vp_phi->at(l);
            part.charge = vp_charge->at(l);
            genParts.push_back(part);
        }

        writer.fill(roads, genParts);
        ++nKept;
    }

    if (verbose_)  std::cout << Info() << "Processed " << nEvents_ << " events, kept " << nKept << ", triggered on " << nPassed << std::endl;

    long long nentries = writer.write();
    assert(nentries == nKept);

    return 0;
}


// _____________________________________________________________________________
// Read from the pattern bank
int PatternMatcher::readPatterns_fas(TString src) {
    if (src.EndsWith(".root")) {
        if (verbose_)  std::cout << Info() << "Opening " << src << std::endl;
        TFile* tfile = TFile::Open(src);
        TTree* ttree = (TTree*) tfile->Get(bankName_);
        if (ttree == 0) {
            std::cout << Error() << "Unable to retrieve the TTree." << std::endl;
        }

        // For reading
        count_type               frequency;
        std::vector<addr_type> * superstripIds  = 0;

        ttree->SetBranchAddress("frequency"    , &frequency);
        ttree->SetBranchAddress("superstripIds", &superstripIds);

        long long nentries = ttree->GetEntries();
        if (nentries > maxPatterns_)
            nentries = maxPatterns_;

        if (verbose_)  std::cout << Info() << "Reading " << nentries << " patterns." << std::endl;

        // _____________________________________________________________________
        // Loop over all patterns

        // Set K = number of values associated to a key
        //     N = number of distinct keus
        //     M = number of distinct values
        inputPatterns_fas_.init(MAX_NLAYERS, nentries, hasher_ -> hash(fakeSuperstripId_) + 1);

        std::vector<key_type> superstripHashes;
        superstripHashes.resize(MAX_NLAYERS);
        key_type * key = 0;

        for (long long ievt=0; ievt<nentries; ++ievt) {
            ttree->GetEntry(ievt);
            if (frequency < minFrequency_)
                break;

            assert(superstripIds->size() == MAX_NLAYERS);

            // Convert each ssId to a hash key
            for (unsigned i=0; i<superstripIds->size(); ++i) {
                if (superstripIds->at(i) != 0)
                    superstripHashes.at(i) = hasher_ -> hash(superstripIds->at(i));
            }

            key = std::addressof(superstripHashes.at(0));
            inputPatterns_fas_.insert(key, key+superstripHashes.size());

            //if (verbose_>3)  std::cout << Debug() << "... patt: " << ievt << "  " << inputPatterns_vector_.at(ievt) << " freq: " << (unsigned) frequency << std::endl;
        }

        if ((size_t) nentries * MAX_NLAYERS != inputPatterns_fas_.size()) {
            std::cout << Error() << "Failed to read all patterns from the root file: " << inputPatterns_fas_.size() << " vs " << nentries * MAX_NLAYERS << std::endl;
            return 1;
        }

        // Generate the reverse look up table
        inputPatterns_fas_.reverse();

        delete ttree;
        delete tfile;
    }

    if (! inputPatterns_fas_.size()) {
        std::cout << Error() << "Pattern bank is empty." << std::endl;
        return 1;
    }
    return 0;
}


// _____________________________________________________________________________
// Do pattern recognition
int PatternMatcher::makeRoads_fas(TString out) {

    // Make sure the map has already been set up
    assert(inputPatterns_fas_.size());
    assert(std::is_pod<TTSuperstrip>::value && std::is_pod<TTPattern>::value && std::is_pod<TTHit>::value);

    // For reading
    if (verbose_)  std::cout << Info() << "Reading " << nEvents_ << " events" << std::endl;

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

    // For writing
    if (verbose_)  std::cout << Info() << "Recreating " << out << std::endl;
    TTRoadWriter writer(out, prefixRoad_, suffix_);


    // _________________________________________________________________________
    // Loop over all events

    std::map<key_type, std::vector<TTHit> > superstripHits;  // key: superstrip hash, values: vector of hits
    std::vector<TTHit> hits;
    std::vector<std::vector<TTHit> > hitses;  // just like hits, but before serialized

    std::vector<std::pair<key_type, key_type> > superstripPatterns;  // first: pattern id, second: superstrip hash
    superstripPatterns.reserve(5000);
    pattern_type recreatedPattern;

    key_type fakeSuperstripHash = hasher_ -> hash(fakeSuperstripId_);
    key_type fakeSuperstripHash1 = hasher_ -> hash(fakeSuperstripId1_);

    std::vector<TTRoad> roads;
    roads.reserve(200);
    std::vector<genPart> genParts;
    genParts.reserve(50);

    int nPassed = 0, nKept = 0;
    for (long long ievt=0; ievt<nEvents_; ++ievt) {
        Long64_t local_entry = chain_->LoadTree(ievt);  // for TChain
        if (local_entry < 0)  break;
        chain_->GetEntry(ievt);

        unsigned nstubs = vb_modId->size();
        if (verbose_>1 && ievt%5000==0)  std::cout << Debug() << Form("... Processing event: %7lld, keeping: %7i, triggering: %7i", ievt, nKept, nPassed) << std::endl;
        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " # stubs: " << nstubs << std::endl;

        if (!nstubs) {  // skip if no stub
            writer.fill(std::vector<TTRoad>(), std::vector<genPart>());
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
            superstripHits[hash].emplace_back(TTHit{x, y, z, 0., 0., 0., -1, pt, ssId, 1});  // POD type constructor

            if (verbose_>2)  std::cout << Debug() << "... ... stub: " << l << " moduleId: " << moduleId << " col: " << col << " row: " << row << " ssId: " << ssId << " trkId: " << vb_trkId->at(l) << std::endl;
        }

        // _____________________________________________________________________
        // Create roads
        roads.clear();
        superstripPatterns.clear();

        unsigned superstripPatterns_oldsize = 0;

        //FIXME: Use superstripBooleans instead?
        std::map<key_type, std::vector<TTHit> >::const_iterator itmap = superstripHits.begin();
        key_type * itval = 0;

        for (; itmap != superstripHits.end(); ++itmap) {
            const fas::lean_lut2::return_type& ret = inputPatterns_fas_.at(itmap -> first);

            for (itval = ret.begin; itval != ret.end; ++itval)
                superstripPatterns.emplace_back(std::make_pair(*itval, itmap -> first));

            std::inplace_merge(superstripPatterns.begin(), superstripPatterns.begin() + superstripPatterns_oldsize, superstripPatterns.end(), sort_by_first);
            superstripPatterns_oldsize = superstripPatterns.size();
        }

        superstripPatterns.erase(majority_logic(superstripPatterns.begin(),
                                                superstripPatterns.end(),
                                                nLayers_ - po.nMisses, compare_by_first),
                                 superstripPatterns.end());

        hits.clear();
        hitses.clear();
        recreatedPattern.fill(0);

        std::vector<std::pair<key_type, key_type> >::const_iterator itpair = superstripPatterns.begin();
        for (; itpair != superstripPatterns.end(); ++itpair) {

            //assert(itpair -> second != 0);
            recreatedPattern.at(hitses.size()) = itpair -> second;
            if ((itpair -> second != fakeSuperstripHash) &&
                (itpair -> second != fakeSuperstripHash1) ) {
                hitses.push_back(superstripHits.at(itpair -> second));
                if ((int) hitses.back().size() > maxHits_)
                    hitses.back().resize(maxHits_);

            } else {
                // Fake superstrip
                hitses.push_back(std::vector<TTHit>());
            }

            if ((itpair+1 == superstripPatterns.end()) || (itpair -> first != (itpair+1) -> first)) {

                for (unsigned i=0; i<hitses.size(); ++i) {
                    hits.insert(hits.end(), hitses.at(i).begin(), hitses.at(i).end());
                }

                roads.emplace_back(hitses.size(), (itpair -> first), recreatedPattern, hits);

                if (verbose_>2)  std::cout << Debug() << "... ... road: " << roads.size() - 1 << " " << roads.back() << std::endl;

                hits.clear();
                hitses.clear();
                recreatedPattern.fill(0);
            }

            if ((int) roads.size() >= maxRoads_)
                break;
        }

        if (! roads.empty())
            ++nPassed;

        // _____________________________________________________________________
        // In addition, keep genParticle info
        genParts.clear();

        unsigned nparts = vp_pt->size();
        for (unsigned l=0; l<nparts; ++l) {
            genPart part;
            part.pt     = vp_pt->at(l);
            part.eta    = vp_eta->at(l);
            part.phi    = vp_phi->at(l);
            part.charge = vp_charge->at(l);
            genParts.push_back(part);
        }

        writer.fill(roads, genParts);
        ++nKept;
    }

    if (verbose_)  std::cout << Info() << "Processed " << nEvents_ << " events, kept " << nKept << ", triggered on " << nPassed << std::endl;

    long long nentries = writer.write();
    assert(nentries == nKept);

    return 0;
}


// _____________________________________________________________________________
// Private functions
// none


// _____________________________________________________________________________
// TTRoadWriter

TTRoadWriter::TTRoadWriter(TString out, TString prefix, TString suffix)
: vr_nHitLayers       (new std::vector<unsigned16>()),
  vr_bankIndex        (new std::vector<unsigned>()),
  //
  vr_patternIds       (new std::vector<std::vector<unsigned> >()),
  vr_hitXs            (new std::vector<std::vector<float> >()),
  vr_hitYs            (new std::vector<std::vector<float> >()),
  vr_hitZs            (new std::vector<std::vector<float> >()),
  vr_hitXErrors       (new std::vector<std::vector<float> >()),
  vr_hitYErrors       (new std::vector<std::vector<float> >()),
  vr_hitZErrors       (new std::vector<std::vector<float> >()),
  vr_hitCharges       (new std::vector<std::vector<int> >()),
  vr_hitPts           (new std::vector<std::vector<float> >()),
  vr_hitSuperstripIds (new std::vector<std::vector<unsigned> >()),
  vr_hitSuperstripBits(new std::vector<std::vector<unsigned16> >()),
  //
  vp_pt               (new std::vector<float>()),
  vp_eta              (new std::vector<float>()),
  vp_phi              (new std::vector<float>()),
  vp_charge           (new std::vector<int>()) {

    init(out, prefix, suffix);
}

TTRoadWriter::~TTRoadWriter() {
    if (ttree)  delete ttree;
    if (tfile)  delete tfile;
}

void TTRoadWriter::init(TString out, TString prefix, TString suffix) {
    if (!out.EndsWith(".root"))
        throw std::invalid_argument("Output filename must be .root.");

    tfile = TFile::Open(out, "RECREATE");
    if (tfile == 0)
        throw std::runtime_error("Cannot recreate output file.");

    tfile->mkdir("ntupler")->cd();
    ttree = new TTree("tree", "");

    ttree->Branch(prefix + "nHitLayers"        + suffix, &(*vr_nHitLayers));
    ttree->Branch(prefix + "bankIndex"         + suffix, &(*vr_bankIndex));
    //
    ttree->Branch(prefix + "patternIds"        + suffix, &(*vr_patternIds));
    ttree->Branch(prefix + "hitXs"             + suffix, &(*vr_hitXs));
    ttree->Branch(prefix + "hitYs"             + suffix, &(*vr_hitYs));
    ttree->Branch(prefix + "hitZs"             + suffix, &(*vr_hitZs));
    ttree->Branch(prefix + "hitXErrors"        + suffix, &(*vr_hitXErrors));
    ttree->Branch(prefix + "hitYErrors"        + suffix, &(*vr_hitYErrors));
    ttree->Branch(prefix + "hitZErrors"        + suffix, &(*vr_hitZErrors));
    ttree->Branch(prefix + "hitCharges"        + suffix, &(*vr_hitCharges));
    ttree->Branch(prefix + "hitPts"            + suffix, &(*vr_hitPts));
    ttree->Branch(prefix + "hitSuperstripIds"  + suffix, &(*vr_hitSuperstripIds));
    ttree->Branch(prefix + "hitSuperstripBits" + suffix, &(*vr_hitSuperstripBits));
    //
    ttree->Branch("genParts_pt"    , &(*vp_pt));
    ttree->Branch("genParts_eta"   , &(*vp_eta));
    ttree->Branch("genParts_phi"   , &(*vp_phi));
    ttree->Branch("genParts_charge", &(*vp_charge));
}

void TTRoadWriter::fill(const std::vector<TTRoad>& roads, const std::vector<genPart>& genParts) {
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
    //
    vp_pt               ->clear();
    vp_eta              ->clear();
    vp_phi              ->clear();
    vp_charge           ->clear();

    const unsigned nroads = roads.size();
    for (unsigned i=0, j=0; i<nroads; ++i) {
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

        const TTRoad&             road = roads.at(i);
        const pattern_type&       patt = road.patternId();
        const std::vector<TTHit>& hits = road.getHits();

        for (j=0; j<patt.size(); ++j) {
            patternIds.push_back(patt.at(j));
        }

        for (j=0; j<hits.size(); ++j) {
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

    const unsigned nparts = genParts.size();
    for (unsigned i=0; i<nparts; ++i) {
        const genPart& part = genParts.at(i);
        vp_pt    ->push_back(part.pt);
        vp_eta   ->push_back(part.eta);
        vp_phi   ->push_back(part.phi);
        vp_charge->push_back(part.charge);
    }

    ttree->Fill();
    assert(vr_hitXs->size() == nroads);
    assert(vp_pt->size() == nparts);
}

long long TTRoadWriter::write() {
    long long nentries = ttree->GetEntries();
    tfile->Write();
    //tfile->Close();
    return nentries;
}


// _____________________________________________________________________________
// Main driver
int PatternMatcher::run(TString out, TString src, TString bank) {
    gROOT->ProcessLine("#include <vector>");  // how is it not loaded?

    int exitcode = 0;
    Timing(1);

    bool use_fas = true;
    if (use_fas) {
        exitcode = readPatterns_fas(bank);
        if (exitcode)  return exitcode;
        Timing();

    } else {
        exitcode = readPatterns_vector(bank);
        if (exitcode)  return exitcode;
        Timing();
    }

    exitcode = readFile(src);
    if (exitcode)  return exitcode;
    Timing();

    if (use_fas) {
        exitcode = makeRoads_fas(out);
        if (exitcode)  return exitcode;
        Timing();

    } else {
        exitcode = makeRoads_vector(out);
        if (exitcode)  return exitcode;
        Timing();
    }

    chain_->Reset();
    return exitcode;
}
