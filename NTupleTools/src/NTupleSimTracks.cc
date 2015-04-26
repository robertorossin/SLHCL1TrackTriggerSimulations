#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleSimTracks.h"

#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"


NTupleSimTracks::NTupleSimTracks(const edm::ParameterSet& iConfig) :
  inputTag_(iConfig.getParameter<edm::InputTag>("inputTag")),
  prefix_  (iConfig.getParameter<std::string>("prefix")),
  suffix_  (iConfig.getParameter<std::string>("suffix")),
  selector_(iConfig.existsAs<std::string>("cut") ? iConfig.getParameter<std::string>("cut") : "", true),
  maxN_    (iConfig.getParameter<unsigned>("maxN")) {

    produces<std::vector<float> >    (prefix_ + "px"     + suffix_);
    produces<std::vector<float> >    (prefix_ + "py"     + suffix_);
    produces<std::vector<float> >    (prefix_ + "pz"     + suffix_);
    produces<std::vector<float> >    (prefix_ + "E"      + suffix_);
    produces<std::vector<float> >    (prefix_ + "pt"     + suffix_);
    produces<std::vector<float> >    (prefix_ + "eta"    + suffix_);
    produces<std::vector<float> >    (prefix_ + "phi"    + suffix_);
    produces<std::vector<float> >    (prefix_ + "charge" + suffix_);
    produces<std::vector<int> >      (prefix_ + "pdgId"  + suffix_);
    produces<std::vector<unsigned> > (prefix_ + "trkId"  + suffix_);
    produces<std::vector<unsigned> > (prefix_ + "evtId"  + suffix_);
    produces<std::vector<int> >      (prefix_ + "vtxId"  + suffix_);
    produces<std::vector<int> >      (prefix_ + "genpId" + suffix_);
    produces<unsigned>               (prefix_ + "size"   + suffix_);
}

void NTupleSimTracks::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<std::vector<float> >    v_px    (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_py    (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_pz    (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_E     (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_pt    (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_eta   (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_phi   (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_charge(new std::vector<float>());
    std::auto_ptr<std::vector<int> >      v_pdgId (new std::vector<int>());
    std::auto_ptr<std::vector<unsigned> > v_trkId (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> > v_evtId (new std::vector<unsigned>());
    std::auto_ptr<std::vector<int> >      v_vtxId (new std::vector<int>());
    std::auto_ptr<std::vector<int> >      v_genpId(new std::vector<int>());
    std::auto_ptr<unsigned>               v_size  (new unsigned(0));

    //__________________________________________________________________________
    if (!iEvent.isRealData()) {
        edm::Handle<edm::SimTrackContainer> tracks;
        iEvent.getByLabel(inputTag_, tracks);

        if (tracks.isValid()) {
            edm::LogInfo("NTupleSimTracks") << "Size: " << tracks->size();

            unsigned n = 0;
            for (edm::SimTrackContainer::const_iterator it = tracks->begin(); it != tracks->end(); ++it) {
                if (n >= maxN_)
                    break;
                if (!selector_(*it))
                    continue;

                // Fill the vectors
                const math::XYZTLorentzVectorD& momentum = it->momentum();
                v_px->push_back(momentum.px());
                v_py->push_back(momentum.py());
                v_pz->push_back(momentum.pz());
                v_E->push_back(momentum.energy());
                v_pt->push_back(momentum.pt());
                v_eta->push_back(momentum.eta());
                v_phi->push_back(momentum.phi());
                v_charge->push_back(it->charge());
                v_pdgId->push_back(it->type());
                v_trkId->push_back(it->trackId());
                v_evtId->push_back(it->eventId().rawId());
                v_vtxId->push_back(it->vertIndex());
                v_genpId->push_back(it->genpartIndex());

                n++;
            }
            *v_size = v_px->size();

        } else {
            edm::LogError("NTupleSimTracks") << "Cannot get the product: " << inputTag_;
        }
    }

    //__________________________________________________________________________
    iEvent.put(v_px    , prefix_ + "px"     + suffix_);
    iEvent.put(v_py    , prefix_ + "py"     + suffix_);
    iEvent.put(v_pz    , prefix_ + "pz"     + suffix_);
    iEvent.put(v_E     , prefix_ + "E"      + suffix_);
    iEvent.put(v_pt    , prefix_ + "pt"     + suffix_);
    iEvent.put(v_eta   , prefix_ + "eta"    + suffix_);
    iEvent.put(v_phi   , prefix_ + "phi"    + suffix_);
    iEvent.put(v_charge, prefix_ + "charge" + suffix_);
    iEvent.put(v_pdgId , prefix_ + "pdgId"  + suffix_);
    iEvent.put(v_trkId , prefix_ + "trkId"  + suffix_);
    iEvent.put(v_evtId , prefix_ + "evtId"  + suffix_);
    iEvent.put(v_vtxId , prefix_ + "vtxId"  + suffix_);
    iEvent.put(v_genpId, prefix_ + "genpId" + suffix_);
    iEvent.put(v_size  , prefix_ + "size"   + suffix_);
}
