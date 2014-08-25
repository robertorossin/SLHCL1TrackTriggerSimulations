#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/PatternMatcher.h"

#include <type_traits>

static const unsigned MAX_NLAYERS = pattern_type().size();  // ought to be 8

struct genPart {
    float pt;
    float eta;
    float phi;
    float vx;
    float vy;
    float vz;
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
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitXs;
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitYs;
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitZs;
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitXErrors;
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitYErrors;
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitZErrors;
    std::auto_ptr<std::vector<std::vector<int> > >        vr_hitCharges;
    std::auto_ptr<std::vector<std::vector<float> > >      vr_hitPts;
    std::auto_ptr<std::vector<std::vector<unsigned> > >   vr_hitSuperstripIds;

    // Roads (inner vectors)
    std::vector<float>      hitXs;
    std::vector<float>      hitYs;
    std::vector<float>      hitZs;
    std::vector<float>      hitXErrors;
    std::vector<float>      hitYErrors;
    std::vector<float>      hitZErrors;
    std::vector<int>        hitCharges;
    std::vector<float>      hitPts;
    std::vector<unsigned>   hitSuperstripIds;

    // In addition, keep genParticle info
    std::auto_ptr<std::vector<float> > vp_pt;
    std::auto_ptr<std::vector<float> > vp_eta;
    std::auto_ptr<std::vector<float> > vp_phi;
    std::auto_ptr<std::vector<float> > vp_vx;
    std::auto_ptr<std::vector<float> > vp_vy;
    std::auto_ptr<std::vector<float> > vp_vz;
    std::auto_ptr<std::vector<int> >   vp_charge;
};


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
        count_type             frequency;
        std::vector<id_type> * superstripIds  = 0;

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

        for (long long ievt=0; ievt<nentries; ++ievt) {
            ttree->GetEntry(ievt);
            if (frequency < minFrequency_)
                break;

            assert(superstripIds->size() == MAX_NLAYERS);

            // Here we insert all superstrips including the virtual ones (calo, muon)
            // When doing the matching, the virtual ones are not used
            std::copy(superstripIds->begin(), superstripIds->end(), inputPatterns_vector_.at(ievt).begin());

            if (verbose_>3) {
                for (unsigned i=0; i<superstripIds->size(); ++i) {
                    std::cout << Debug() << "... patt: " << i << "  ";
                    std::copy(superstripIds->begin(), superstripIds->end(), std::ostream_iterator<id_type>(std::cout, " "));
                    std::cout << " freq: " << (unsigned) frequency << std::endl;
                }
            }
        }

        if ((size_t) nentries != inputPatterns_vector_.size()) {
            std::cout << Error() << "Failed to read all patterns from the root file: " << inputPatterns_vector_.size() << " vs " << nentries << std::endl;
            return 1;
        }

        // _____________________________________________________________________
        // Also get the trigger tower maps
        TTree* ttree2 = (TTree*) tfile->Get("triggerTower");
        if (ttree2 == 0) {
            std::cout << Error() << "Unable to retrieve the TTree." << std::endl;
        }

        std::map<unsigned, std::vector<unsigned> > * ptr_map1 = 0;
        std::map<unsigned, std::vector<unsigned> > * ptr_map2 = 0;
        ttree2->SetBranchAddress("triggerTowerMap"       , &ptr_map1);
        ttree2->SetBranchAddress("triggerTowerReverseMap", &ptr_map2);

        ttree2->GetEntry(0);
        triggerTowerMap_.swap(*ptr_map1);
        triggerTowerReverseMap_.swap(*ptr_map2);

        delete ttree2;
        delete ttree;
        delete tfile;
    }

    if (! inputPatterns_vector_.size()) {
        std::cout << Error() << "Pattern bank is empty." << std::endl;
        return 1;
    }
    if (triggerTowerMap_.empty() || triggerTowerReverseMap_.empty()) {
        std::cout << Error() << "Trigger tower map is empty." << std::endl;
        return 1;
    }

    return 0;
}


// _____________________________________________________________________________
// Do pattern recognition
int PatternMatcher::makeRoads_vector(TString out) {

    // Make sure the map has already been set up
    assert(inputPatterns_vector_.size());
    assert(std::is_pod<TTSuperstrip>::value && std::is_pod<TTPattern>::value && std::is_pod<TTHit>::value);

    // For reading
    if (verbose_)  std::cout << Info() << "Reading " << nEvents_ << " events." << std::endl;

    std::vector<float> *          vb_x          = 0;
    std::vector<float> *          vb_y          = 0;
    std::vector<float> *          vb_z          = 0;
    //std::vector<float> *          vb_r          = 0;
    //std::vector<float> *          vb_eta        = 0;
    //std::vector<float> *          vb_phi        = 0;
    std::vector<float> *          vb_coordx     = 0;
    std::vector<float> *          vb_coordy     = 0;
    std::vector<float> *          vb_roughPt    = 0;
    std::vector<float> *          vb_trigBend   = 0;
    std::vector<unsigned> *       vb_modId      = 0;
    //std::vector<int> *            vb_trkId      = 0;
    std::vector<float> *          vp_pt         = 0;
    std::vector<float> *          vp_eta        = 0;
    std::vector<float> *          vp_phi        = 0;
    std::vector<float> *          vp_vx         = 0;
    std::vector<float> *          vp_vy         = 0;
    std::vector<float> *          vp_vz         = 0;
    std::vector<int> *            vp_charge     = 0;

    chain_->SetBranchStatus("*"                 , 0);
    chain_->SetBranchStatus("TTStubs_x"         , 1);
    chain_->SetBranchStatus("TTStubs_y"         , 1);
    chain_->SetBranchStatus("TTStubs_z"         , 1);
    //chain_->SetBranchStatus("TTStubs_r"         , 1);
    //chain_->SetBranchStatus("TTStubs_eta"       , 1);
    //chain_->SetBranchStatus("TTStubs_phi"       , 1);
    chain_->SetBranchStatus("TTStubs_coordx"    , 1);
    chain_->SetBranchStatus("TTStubs_coordy"    , 1);
    chain_->SetBranchStatus("TTStubs_roughPt"   , 1);
    chain_->SetBranchStatus("TTStubs_trigBend"  , 1);
    chain_->SetBranchStatus("TTStubs_modId"     , 1);
    //chain_->SetBranchStatus("TTStubs_trkId"     , 1);
    chain_->SetBranchStatus("genParts_pt"       , 1);
    chain_->SetBranchStatus("genParts_eta"      , 1);
    chain_->SetBranchStatus("genParts_phi"      , 1);
    chain_->SetBranchStatus("genParts_vx"       , 1);
    chain_->SetBranchStatus("genParts_vy"       , 1);
    chain_->SetBranchStatus("genParts_vz"       , 1);
    chain_->SetBranchStatus("genParts_charge"   , 1);

    chain_->SetBranchAddress("TTStubs_x"        , &(vb_x));
    chain_->SetBranchAddress("TTStubs_y"        , &(vb_y));
    chain_->SetBranchAddress("TTStubs_z"        , &(vb_z));
    //chain_->SetBranchAddress("TTStubs_r"        , &(vb_r));
    //chain_->SetBranchAddress("TTStubs_eta"      , &(vb_eta));
    //chain_->SetBranchAddress("TTStubs_phi"      , &(vb_phi));
    chain_->SetBranchAddress("TTStubs_coordx"   , &(vb_coordx));
    chain_->SetBranchAddress("TTStubs_coordy"   , &(vb_coordy));
    chain_->SetBranchAddress("TTStubs_roughPt"  , &(vb_roughPt));
    chain_->SetBranchAddress("TTStubs_trigBend" , &(vb_trigBend));
    chain_->SetBranchAddress("TTStubs_modId"    , &(vb_modId));
    //chain_->SetBranchAddress("TTStubs_trkId"    , &(vb_trkId));
    chain_->SetBranchAddress("genParts_pt"      , &(vp_pt));
    chain_->SetBranchAddress("genParts_eta"     , &(vp_eta));
    chain_->SetBranchAddress("genParts_phi"     , &(vp_phi));
    chain_->SetBranchAddress("genParts_vx"      , &(vp_vx));
    chain_->SetBranchAddress("genParts_vy"      , &(vp_vy));
    chain_->SetBranchAddress("genParts_vz"      , &(vp_vz));
    chain_->SetBranchAddress("genParts_charge"  , &(vp_charge));

    // For writing
    if (verbose_)  std::cout << Info() << "Recreating " << out << std::endl;
    TTRoadWriter writer(out, prefixRoad_, suffix_);


    // _________________________________________________________________________
    // Loop over all events

    std::map<id_type, std::vector<id_type> > towerSuperstripsMap; // key: trigger tower id; values: vector of superstrip addr
    std::map<id_type, std::vector<TTHit> >   superstripHitsMap;   // key: superstrip addr; values: vector of hits
    std::vector<std::vector<TTHit> > hitses;  // just like hits, but before serialized
    std::vector<TTHit> hits;                  // after serialized

    const id_type& fakeSuperstrip = arbiter_ -> superstrip(27, 0, 0, 0, 0);
    id_type nvalues = fakeSuperstrip + 1;
    if (nvalues & 1) nvalues += 1;  // if odd, make it even

    std::vector<bool> superstripBooleans;  // should be replaced by a bitmap
    superstripBooleans.resize(nvalues);

    std::vector<TTRoad> roads;
    roads.reserve(300);
    std::vector<genPart> genParts;
    genParts.reserve(50);

    int nPassed = 0, nKept = 0;
    unsigned ievt_step = 0;
    for (long long ievt=0; ievt<nEvents_; ++ievt, ++ievt_step) {
        Long64_t local_entry = chain_->LoadTree(ievt);  // for TChain
        if (local_entry < 0)  break;
        chain_->GetEntry(ievt);

        const unsigned nstubs = vb_modId->size();
        if (verbose_>1 && ievt_step == 5000) {
            std::cout << Debug() << Form("... Processing event: %7lld, keeping: %7i, triggering: %7i", ievt, nKept, nPassed) << std::endl;
            ievt_step -= 5000;
        }
        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " # stubs: " << nstubs << std::endl;

        if (!nstubs) {  // skip if no stub
            writer.fill(std::vector<TTRoad>(), std::vector<genPart>());
            ++nKept;
            continue;
        }

        if (nstubs > 100000) {
            std::cout << Error() << "Way too many stubs: " << nstubs << std::endl;
            return 1;
        }

        // _____________________________________________________________________
        // Start pattern recognition
        towerSuperstripsMap.clear();
        superstripHitsMap.clear();
        std::fill(superstripBooleans.begin(), superstripBooleans.end(), false);

        // Loop over reconstructed stubs
        id_type moduleId, lay, lad, mod, col, row;  // declare the usual suspects
        float stub_x, stub_y, stub_z, stub_pt;
        for (unsigned l=0; l<nstubs; ++l) {

            // Break moduleId into lay, lad, mod
            moduleId = vb_modId->at(l);
            lay = decodeLayer(moduleId);
            lad = decodeLadder(moduleId);
            mod = decodeModule(moduleId);

            // col <-- coordy, row <-- coordx
            // use half-strip unit
            col = halfStripRound(vb_coordy->at(l));
            row = halfStripRound(vb_coordx->at(l));

            // Skip if moduleId not in any trigger tower
            if (po.requireTriggerTower && triggerTowerReverseMap_.find(moduleId) == triggerTowerReverseMap_.end()) {
                if (verbose_>2)  std::cout << Debug() << "... ... skip moduleId: " << moduleId << " not in any trigger tower." << std::endl;
                continue;
            }

            // Find superstrip address
            const id_type& ssId = arbiter_ -> superstrip(lay, lad, mod, col, row);

            // Position and rough pt
            stub_x = vb_x->at(l);
            stub_y = vb_y->at(l);
            stub_z = vb_z->at(l);
            stub_pt = vb_roughPt->at(l);

            // Create a hit
            std::vector<TTHit>& ssHits = superstripHitsMap[ssId];  // this takes the address
            ssHits.emplace_back(TTHit{stub_x, stub_y, stub_z, 0., 0., 0., -1, stub_pt, ssId});  // POD type constructor

            // Make a tick
            superstripBooleans.at(ssId) = true;

            if (ssHits.size() == 1) {  // First time this ssId shows up
                // Find associated trigger towers
                if (po.requireTriggerTower) {
                    const std::vector<unsigned>& towerIds = triggerTowerReverseMap_.at(moduleId);
                    for (unsigned i=0; i<towerIds.size(); ++i) {
                        towerSuperstripsMap[towerIds.at(i)].push_back(ssId);
                    }

                } else {
                    towerSuperstripsMap[0].push_back(ssId);
                }
            }

            if (verbose_>2)  std::cout << Debug() << "... ... stub: " << l << " moduleId: " << moduleId << " col: " << col << " row: " << row << " ssId: " << ssId << std::endl;
        }

        // _____________________________________________________________________
        // Create roads
        roads.clear();

        hitses.clear();
        hits.clear();

        std::map<id_type, std::vector<TTHit> >::iterator found;
        unsigned nMisses = 0;

        //FIXME: do not correctly model the overlaps among trigger towers
        for (std::vector<pattern_type>::const_iterator itv = inputPatterns_vector_.begin();
             itv != inputPatterns_vector_.end(); ++itv) {
            nMisses = 0;

            for (pattern_type::const_iterator it = itv->begin(); it != itv->begin() + nLayers_; ++it) {
                //assert(*it != 0);
                if ((*it != fakeSuperstrip)  &&
                    (! superstripBooleans.at(*it)) )
                        ++nMisses;

                // if number of allowed misses more than the requirement
                if (nMisses > po.nMisses)
                    break;
            }

            //if (verbose_>2 && hitses.size() == po.nLayers)  std::cout << Debug() << "... ... # hitses: " << hitses.size() << " # misses: " << nMisses << std::endl;

            if (nMisses <= po.nMisses) {
                hitses.clear();
                hits.clear();

                for (pattern_type::const_iterator it = itv->begin(); it != itv->begin() + nLayers_; ++it) {
                    if (*it != fakeSuperstrip) {
                        found = superstripHitsMap.find(*it);
                        if (found != superstripHitsMap.end()) {
                            hitses.push_back(found -> second);
                            if ((int) hitses.back().size() > maxHits_)
                                hitses.back().resize(maxHits_);
                        }

                    } else {  // fake superstrip
                        hitses.push_back(std::vector<TTHit>());
                    }
                }

                for (unsigned i=0; i<hitses.size(); ++i) {
                    hits.insert(hits.end(), hitses.at(i).begin(), hitses.at(i).end());
                }
                roads.emplace_back(hitses.size(), itv - inputPatterns_vector_.begin(), hits);

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

        const unsigned nparts = vp_pt->size();
        for (unsigned l=0; l<nparts; ++l) {
            genPart part;
            part.pt     = vp_pt->at(l);
            part.eta    = vp_eta->at(l);
            part.phi    = vp_phi->at(l);
            part.vx     = vp_vx->at(l);
            part.vy     = vp_vy->at(l);
            part.vz     = vp_vz->at(l);
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
        count_type             frequency;
        std::vector<id_type> * superstripIds  = 0;

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
        const id_type& fakeSuperstrip = arbiter_ -> superstrip(27, 0, 0, 0, 0);
        id_type nvalues = fakeSuperstrip + 1;
        if (nvalues & 1) nvalues += 1;  // if odd, make it even
        inputPatterns_fas_.init(MAX_NLAYERS, nentries, nvalues);

        id_type * addr_ptr = 0;

        for (long long ievt=0; ievt<nentries; ++ievt) {
            ttree->GetEntry(ievt);
            if (frequency < minFrequency_)
                break;

            assert(superstripIds->size() == MAX_NLAYERS);

            // Here we insert all superstrips including the virtual ones (calo, muon)
            // When doing the matching, the virtual ones are not used
            addr_ptr = std::addressof(superstripIds->at(0));
            inputPatterns_fas_.insert(addr_ptr, addr_ptr+superstripIds->size());

            if (verbose_>3) {
                for (unsigned i=0; i<superstripIds->size(); ++i) {
                    std::cout << Debug() << "... patt: " << i << "  ";
                    std::copy(superstripIds->begin(), superstripIds->end(), std::ostream_iterator<id_type>(std::cout, " "));
                    std::cout << " freq: " << (unsigned) frequency << std::endl;
                }
            }
        }

        if ((size_t) nentries * MAX_NLAYERS != inputPatterns_fas_.size()) {
            std::cout << Error() << "Failed to read all patterns from the root file: " << inputPatterns_fas_.size() << " vs " << nentries * MAX_NLAYERS << std::endl;
            return 1;
        }

        // Generate the reverse look up table
        inputPatterns_fas_.reverse();

        // _____________________________________________________________________
        // Also get the trigger tower maps
        TTree* ttree2 = (TTree*) tfile->Get("triggerTower");
        if (ttree2 == 0) {
            std::cout << Error() << "Unable to retrieve the TTree." << std::endl;
        }

        std::map<unsigned, std::vector<unsigned> > * ptr_map1 = 0;
        std::map<unsigned, std::vector<unsigned> > * ptr_map2 = 0;
        ttree2->SetBranchAddress("triggerTowerMap"       , &ptr_map1);
        ttree2->SetBranchAddress("triggerTowerReverseMap", &ptr_map2);

        ttree2->GetEntry(0);
        triggerTowerMap_.swap(*ptr_map1);
        triggerTowerReverseMap_.swap(*ptr_map2);

        delete ttree2;
        delete ttree;
        delete tfile;
    }

    if (! inputPatterns_fas_.size()) {
        std::cout << Error() << "Pattern bank is empty." << std::endl;
        return 1;
    }
    if (triggerTowerMap_.empty() || triggerTowerReverseMap_.empty()) {
        std::cout << Error() << "Trigger tower map is empty." << std::endl;
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
    if (verbose_)  std::cout << Info() << "Reading " << nEvents_ << " events." << std::endl;

    std::vector<float> *          vb_x          = 0;
    std::vector<float> *          vb_y          = 0;
    std::vector<float> *          vb_z          = 0;
    //std::vector<float> *          vb_r          = 0;
    //std::vector<float> *          vb_eta        = 0;
    //std::vector<float> *          vb_phi        = 0;
    std::vector<float> *          vb_coordx     = 0;
    std::vector<float> *          vb_coordy     = 0;
    std::vector<float> *          vb_roughPt    = 0;
    std::vector<float> *          vb_trigBend   = 0;
    std::vector<unsigned> *       vb_modId      = 0;
    //std::vector<int> *            vb_trkId      = 0;
    std::vector<float> *          vp_pt         = 0;
    std::vector<float> *          vp_eta        = 0;
    std::vector<float> *          vp_phi        = 0;
    std::vector<float> *          vp_vx         = 0;
    std::vector<float> *          vp_vy         = 0;
    std::vector<float> *          vp_vz         = 0;
    std::vector<int> *            vp_charge     = 0;

    chain_->SetBranchStatus("*"                 , 0);
    chain_->SetBranchStatus("TTStubs_x"         , 1);
    chain_->SetBranchStatus("TTStubs_y"         , 1);
    chain_->SetBranchStatus("TTStubs_z"         , 1);
    //chain_->SetBranchStatus("TTStubs_r"         , 1);
    //chain_->SetBranchStatus("TTStubs_eta"       , 1);
    //chain_->SetBranchStatus("TTStubs_phi"       , 1);
    chain_->SetBranchStatus("TTStubs_coordx"    , 1);
    chain_->SetBranchStatus("TTStubs_coordy"    , 1);
    chain_->SetBranchStatus("TTStubs_roughPt"   , 1);
    chain_->SetBranchStatus("TTStubs_trigBend"  , 1);
    chain_->SetBranchStatus("TTStubs_modId"     , 1);
    //chain_->SetBranchStatus("TTStubs_trkId"     , 1);
    chain_->SetBranchStatus("genParts_pt"       , 1);
    chain_->SetBranchStatus("genParts_eta"      , 1);
    chain_->SetBranchStatus("genParts_phi"      , 1);
    chain_->SetBranchStatus("genParts_vx"       , 1);
    chain_->SetBranchStatus("genParts_vy"       , 1);
    chain_->SetBranchStatus("genParts_vz"       , 1);
    chain_->SetBranchStatus("genParts_charge"   , 1);

    chain_->SetBranchAddress("TTStubs_x"        , &(vb_x));
    chain_->SetBranchAddress("TTStubs_y"        , &(vb_y));
    chain_->SetBranchAddress("TTStubs_z"        , &(vb_z));
    //chain_->SetBranchAddress("TTStubs_r"        , &(vb_r));
    //chain_->SetBranchAddress("TTStubs_eta"      , &(vb_eta));
    //chain_->SetBranchAddress("TTStubs_phi"      , &(vb_phi));
    chain_->SetBranchAddress("TTStubs_coordx"   , &(vb_coordx));
    chain_->SetBranchAddress("TTStubs_coordy"   , &(vb_coordy));
    chain_->SetBranchAddress("TTStubs_roughPt"  , &(vb_roughPt));
    chain_->SetBranchAddress("TTStubs_trigBend" , &(vb_trigBend));
    chain_->SetBranchAddress("TTStubs_modId"    , &(vb_modId));
    //chain_->SetBranchAddress("TTStubs_trkId"    , &(vb_trkId));
    chain_->SetBranchAddress("genParts_pt"      , &(vp_pt));
    chain_->SetBranchAddress("genParts_eta"     , &(vp_eta));
    chain_->SetBranchAddress("genParts_phi"     , &(vp_phi));
    chain_->SetBranchAddress("genParts_vx"      , &(vp_vx));
    chain_->SetBranchAddress("genParts_vy"      , &(vp_vy));
    chain_->SetBranchAddress("genParts_vz"      , &(vp_vz));
    chain_->SetBranchAddress("genParts_charge"  , &(vp_charge));

    // For writing
    if (verbose_)  std::cout << Info() << "Recreating " << out << std::endl;
    TTRoadWriter writer(out, prefixRoad_, suffix_);


    // _________________________________________________________________________
    // Loop over all events

    std::map<id_type, std::vector<id_type> > towerSuperstripsMap; // key: trigger tower id; values: vector of superstrip addr
    std::map<id_type, std::vector<TTHit> >   superstripHitsMap;   // key: superstrip addr; values: vector of hits
    std::vector<std::vector<TTHit> > hitses;  // just like hits, but before serialized
    std::vector<TTHit> hits;                  // after serialized

    const id_type& fakeSuperstrip = arbiter_ -> superstrip(27, 0, 0, 0, 0);

    fas::lean_merger3 merger(0,0);
    merger.init(nLayers_, nLayers_ - po.nMisses, inputPatterns_fas_.num_keys());

    std::vector<TTRoad> roads;
    roads.reserve(300);
    std::vector<genPart> genParts;
    genParts.reserve(50);

    int nPassed = 0, nKept = 0;
    unsigned ievt_step = 0;
    for (long long ievt=0; ievt<nEvents_; ++ievt, ++ievt_step) {
        Long64_t local_entry = chain_->LoadTree(ievt);  // for TChain
        if (local_entry < 0)  break;
        chain_->GetEntry(ievt);

        const unsigned nstubs = vb_modId->size();
        if (verbose_>1 && ievt_step == 5000) {
            std::cout << Debug() << Form("... Processing event: %7lld, keeping: %7i, triggering: %7i", ievt, nKept, nPassed) << std::endl;
            ievt_step -= 5000;
        }
        if (verbose_>2)  std::cout << Debug() << "... evt: " << ievt << " # stubs: " << nstubs << std::endl;

        if (!nstubs) {  // skip if no stub
            writer.fill(std::vector<TTRoad>(), std::vector<genPart>());
            ++nKept;
            continue;
        }

        if (nstubs > 100000) {
            std::cout << Error() << "Way too many stubs: " << nstubs << std::endl;
            return 1;
        }

        // _____________________________________________________________________
        // Start pattern recognition
        towerSuperstripsMap.clear();
        superstripHitsMap.clear();

        // Loop over reconstructed stubs
        id_type moduleId, lay, lad, mod, col, row;  // declare the usual suspects
        float stub_x, stub_y, stub_z, stub_pt;
        for (unsigned l=0; l<nstubs; ++l) {

            // Break moduleId into lay, lad, mod
            moduleId = vb_modId->at(l);
            lay = decodeLayer(moduleId);
            lad = decodeLadder(moduleId);
            mod = decodeModule(moduleId);

            // col <-- coordy, row <-- coordx
            // use half-strip unit
            col = halfStripRound(vb_coordy->at(l));
            row = halfStripRound(vb_coordx->at(l));

            // Skip if moduleId not in any trigger tower
            if (po.requireTriggerTower && triggerTowerReverseMap_.find(moduleId) == triggerTowerReverseMap_.end()) {
                if (verbose_>2)  std::cout << Debug() << "... ... skip moduleId: " << moduleId << " not in any trigger tower." << std::endl;
                continue;
            }

            // Find superstrip address
            const id_type& ssId = arbiter_ -> superstrip(lay, lad, mod, col, row);

            // Position and rough pt
            stub_x = vb_x->at(l);
            stub_y = vb_y->at(l);
            stub_z = vb_z->at(l);
            stub_pt = vb_roughPt->at(l);

            // Create a hit
            std::vector<TTHit>& ssHits = superstripHitsMap[ssId];  // this takes the address
            ssHits.emplace_back(TTHit{stub_x, stub_y, stub_z, 0., 0., 0., -1, stub_pt, ssId});  // POD type constructor

            if (ssHits.size() == 1) {  // First time this ssId shows up
                // Find associated trigger towers
                if (po.requireTriggerTower) {
                    const std::vector<unsigned>& towerIds = triggerTowerReverseMap_.at(moduleId);
                    for (unsigned i=0; i<towerIds.size(); ++i) {
                        towerSuperstripsMap[towerIds.at(i)].push_back(ssId);
                    }

                } else {
                    towerSuperstripsMap[0].push_back(ssId);
                }
            }

            if (verbose_>2)  std::cout << Debug() << "... ... stub: " << l << " moduleId: " << moduleId << " col: " << col << " row: " << row << " ssId: " << ssId << std::endl;
        }

        // _____________________________________________________________________
        // Create roads
        roads.clear();

        hitses.clear();
        hits.clear();

        // Reverse engineer the pattern
        std::map<id_type, std::vector<id_type> >::const_iterator ittower;
        for (ittower = towerSuperstripsMap.begin(); ittower != towerSuperstripsMap.end(); ++ittower) {
            const std::vector<id_type>& superstrips = ittower -> second;

            for (unsigned i=0; i<superstrips.size(); ++i) {
                merger.insert(superstrips.at(i), &inputPatterns_fas_);
                //if (verbose_>2)  std::cout << Debug() << "... ... ssId: " << superstrips.at(i) << std::endl;
            }
            merger.insert(fakeSuperstrip, &inputPatterns_fas_);  // always trigger fake superstrip

            const fas::lean_merger3::return_type& ret = merger.merge();

            const fas::lean_merger3::pair_type * itpair = ret.begin;
            for (; itpair != ret.end; ++itpair) {
                //assert(itpair -> value != 0);
                if (itpair -> value != fakeSuperstrip) {
                    hitses.push_back(superstripHitsMap.at(itpair -> value));
                    if ((int) hitses.back().size() > maxHits_)
                        hitses.back().resize(maxHits_);

                } else {  // fake superstrip
                    hitses.push_back(std::vector<TTHit>());
                }

                // Serialize
                if ((itpair+1 == ret.end) || (itpair -> key != (itpair+1) -> key)) {

                    for (unsigned i=0; i<hitses.size(); ++i) {
                        hits.insert(hits.end(), hitses.at(i).begin(), hitses.at(i).end());
                    }
                    roads.emplace_back(hitses.size(), (itpair -> key), hits);

                    if (verbose_>2)  std::cout << Debug() << "... ... road: " << roads.size() - 1 << " " << roads.back() << std::endl;
                    hitses.clear();
                    hits.clear();
                }

                if ((int) roads.size() >= maxRoads_)
                    break;
            }
        }

        if (! roads.empty())
            ++nPassed;

        // _____________________________________________________________________
        // In addition, keep genParticle info
        genParts.clear();

        const unsigned nparts = vp_pt->size();
        for (unsigned l=0; l<nparts; ++l) {
            genPart part;
            part.pt     = vp_pt->at(l);
            part.eta    = vp_eta->at(l);
            part.phi    = vp_phi->at(l);
            part.vx     = vp_vx->at(l);
            part.vy     = vp_vy->at(l);
            part.vz     = vp_vz->at(l);
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
  vr_hitXs            (new std::vector<std::vector<float> >()),
  vr_hitYs            (new std::vector<std::vector<float> >()),
  vr_hitZs            (new std::vector<std::vector<float> >()),
  vr_hitXErrors       (new std::vector<std::vector<float> >()),
  vr_hitYErrors       (new std::vector<std::vector<float> >()),
  vr_hitZErrors       (new std::vector<std::vector<float> >()),
  vr_hitCharges       (new std::vector<std::vector<int> >()),
  vr_hitPts           (new std::vector<std::vector<float> >()),
  vr_hitSuperstripIds (new std::vector<std::vector<unsigned> >()),
  //
  vp_pt               (new std::vector<float>()),
  vp_eta              (new std::vector<float>()),
  vp_phi              (new std::vector<float>()),
  vp_vx               (new std::vector<float>()),
  vp_vy               (new std::vector<float>()),
  vp_vz               (new std::vector<float>()),
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
    ttree->Branch(prefix + "hitXs"             + suffix, &(*vr_hitXs));
    ttree->Branch(prefix + "hitYs"             + suffix, &(*vr_hitYs));
    ttree->Branch(prefix + "hitZs"             + suffix, &(*vr_hitZs));
    ttree->Branch(prefix + "hitXErrors"        + suffix, &(*vr_hitXErrors));
    ttree->Branch(prefix + "hitYErrors"        + suffix, &(*vr_hitYErrors));
    ttree->Branch(prefix + "hitZErrors"        + suffix, &(*vr_hitZErrors));
    ttree->Branch(prefix + "hitCharges"        + suffix, &(*vr_hitCharges));
    ttree->Branch(prefix + "hitPts"            + suffix, &(*vr_hitPts));
    ttree->Branch(prefix + "hitSuperstripIds"  + suffix, &(*vr_hitSuperstripIds));
    //
    ttree->Branch("genParts_pt"    , &(*vp_pt));
    ttree->Branch("genParts_eta"   , &(*vp_eta));
    ttree->Branch("genParts_phi"   , &(*vp_phi));
    ttree->Branch("genParts_vx"    , &(*vp_vx));
    ttree->Branch("genParts_vy"    , &(*vp_vy));
    ttree->Branch("genParts_vz"    , &(*vp_vz));
    ttree->Branch("genParts_charge", &(*vp_charge));
}

void TTRoadWriter::fill(const std::vector<TTRoad>& roads, const std::vector<genPart>& genParts) {
    vr_nHitLayers       ->clear();
    vr_bankIndex        ->clear();
    //
    vr_hitXs            ->clear();
    vr_hitYs            ->clear();
    vr_hitZs            ->clear();
    vr_hitXErrors       ->clear();
    vr_hitYErrors       ->clear();
    vr_hitZErrors       ->clear();
    vr_hitCharges       ->clear();
    vr_hitPts           ->clear();
    vr_hitSuperstripIds ->clear();
    //
    vp_pt               ->clear();
    vp_eta              ->clear();
    vp_phi              ->clear();
    vp_vx               ->clear();
    vp_vy               ->clear();
    vp_vz               ->clear();
    vp_charge           ->clear();

    const unsigned nroads = roads.size();
    for (unsigned i=0, j=0; i<nroads; ++i) {
        hitXs            .clear();
        hitYs            .clear();
        hitZs            .clear();
        hitXErrors       .clear();
        hitYErrors       .clear();
        hitZErrors       .clear();
        hitCharges       .clear();
        hitPts           .clear();
        hitSuperstripIds .clear();

        const TTRoad&             road = roads.at(i);
        const std::vector<TTHit>& hits = road.getHits();

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
        }

        vr_nHitLayers       ->push_back(road.nHitLayers());
        vr_bankIndex        ->push_back(road.bankIndex());
        //
        vr_hitXs            ->push_back(hitXs);
        vr_hitYs            ->push_back(hitYs);
        vr_hitZs            ->push_back(hitZs);
        vr_hitXErrors       ->push_back(hitXErrors);
        vr_hitYErrors       ->push_back(hitYErrors);
        vr_hitZErrors       ->push_back(hitZErrors);
        vr_hitCharges       ->push_back(hitCharges);
        vr_hitPts           ->push_back(hitPts);
        vr_hitSuperstripIds ->push_back(hitSuperstripIds);
    }

    const unsigned nparts = genParts.size();
    for (unsigned i=0; i<nparts; ++i) {
        const genPart& part = genParts.at(i);
        vp_pt    ->push_back(part.pt);
        vp_eta   ->push_back(part.eta);
        vp_phi   ->push_back(part.phi);
        vp_vx    ->push_back(part.vx);
        vp_vy    ->push_back(part.vy);
        vp_vz    ->push_back(part.vz);
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
