#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleGenJets.h"

#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"


NTupleGenJets::NTupleGenJets(const edm::ParameterSet& iConfig) :
  inputTag_(iConfig.getParameter<edm::InputTag>("inputTag")),
  prefix_  (iConfig.getParameter<std::string>("prefix")),
  suffix_  (iConfig.getParameter<std::string>("suffix")),
  selector_(iConfig.existsAs<std::string>("cut") ? iConfig.getParameter<std::string>("cut") : "", true),
  maxN_    (iConfig.getParameter<unsigned>("maxN")) {

    produces<std::vector<float> > (prefix_ + "px"    + suffix_);
    produces<std::vector<float> > (prefix_ + "py"    + suffix_);
    produces<std::vector<float> > (prefix_ + "pz"    + suffix_);
    produces<std::vector<float> > (prefix_ + "E"     + suffix_);
    produces<std::vector<float> > (prefix_ + "pt"    + suffix_);
    produces<std::vector<float> > (prefix_ + "eta"   + suffix_);
    produces<std::vector<float> > (prefix_ + "phi"   + suffix_);
    produces<std::vector<float> > (prefix_ + "M"     + suffix_);
    produces<unsigned>            (prefix_ + "size"  + suffix_);
}

void NTupleGenJets::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<std::vector<float> > v_px    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > v_py    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > v_pz    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > v_E     (new std::vector<float>());
    std::auto_ptr<std::vector<float> > v_pt    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > v_eta   (new std::vector<float>());
    std::auto_ptr<std::vector<float> > v_phi   (new std::vector<float>());
    std::auto_ptr<std::vector<float> > v_M     (new std::vector<float>());
    std::auto_ptr<unsigned>            v_size  (new unsigned(0));

    //__________________________________________________________________________
    if (!iEvent.isRealData()) {
        edm::Handle<reco::GenJetCollection> jets;
        iEvent.getByLabel(inputTag_, jets);

        if (jets.isValid()) {
            edm::LogInfo("NTupleGenJets") << "Size: " << jets->size();

            unsigned n = 0;
            for (reco::GenJetCollection::const_iterator it = jets->begin(); it != jets->end(); ++it) {
                if (n >= maxN_)
                    break;
                if (!selector_(*it))
                    continue;

                // Fill the vectors
                v_px    ->push_back(it->px());
                v_py    ->push_back(it->py());
                v_pz    ->push_back(it->pz());
                v_E     ->push_back(it->energy());
                v_pt    ->push_back(it->pt());
                v_eta   ->push_back(it->eta());
                v_phi   ->push_back(it->phi());
                v_M     ->push_back(it->mass());

                n++;
            }
            *v_size = v_px->size();

        } else {
            edm::LogError("NTupleGenJets") << "Cannot get the product: " << inputTag_;
        }
    }

    //__________________________________________________________________________
    iEvent.put(v_px    , prefix_ + "px"    + suffix_);
    iEvent.put(v_py    , prefix_ + "py"    + suffix_);
    iEvent.put(v_pz    , prefix_ + "pz"    + suffix_);
    iEvent.put(v_E     , prefix_ + "E"     + suffix_);
    iEvent.put(v_pt    , prefix_ + "pt"    + suffix_);
    iEvent.put(v_eta   , prefix_ + "eta"   + suffix_);
    iEvent.put(v_phi   , prefix_ + "phi"   + suffix_);
    iEvent.put(v_M     , prefix_ + "M"     + suffix_);
    iEvent.put(v_size  , prefix_ + "size"  + suffix_);
}
