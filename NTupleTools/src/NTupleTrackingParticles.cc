#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleTrackingParticles.h"

#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"


NTupleTrackingParticles::NTupleTrackingParticles(const edm::ParameterSet& iConfig) :
  inputTag_(iConfig.getParameter<edm::InputTag>("inputTag")),
  prefix_  (iConfig.getParameter<std::string>("prefix")),
  suffix_  (iConfig.getParameter<std::string>("suffix")),
  selector_(iConfig.existsAs<std::string>("cut") ? iConfig.getParameter<std::string>("cut") : "", true),
  maxN_    (iConfig.getParameter<unsigned>("maxN")) {

    produces<std::vector<float> >    (prefix_ + "px"     + suffix_);
    produces<std::vector<float> >    (prefix_ + "py"     + suffix_);
    produces<std::vector<float> >    (prefix_ + "pz"     + suffix_);
    produces<std::vector<float> >    (prefix_ + "pt"     + suffix_);
    produces<std::vector<float> >    (prefix_ + "eta"    + suffix_);
    produces<std::vector<float> >    (prefix_ + "phi"    + suffix_);
    produces<std::vector<float> >    (prefix_ + "vx"     + suffix_);
    produces<std::vector<float> >    (prefix_ + "vy"     + suffix_);
    produces<std::vector<float> >    (prefix_ + "vz"     + suffix_);
    produces<std::vector<int> >      (prefix_ + "pdgId"  + suffix_);
    produces<std::vector<int> >      (prefix_ + "genpId" + suffix_);
    produces<std::vector<int> >      (prefix_ + "nhits"  + suffix_);
    produces<std::vector<int> >      (prefix_ + "ntkhits" + suffix_);
    produces<std::vector<int> >      (prefix_ + "ntklayers" + suffix_);
    produces<std::vector<unsigned> > (prefix_ + "evtId"  + suffix_);
    produces<std::vector<std::vector<unsigned> > > (prefix_ + "trkIds" + suffix_);
    produces<unsigned>               (prefix_ + "size"   + suffix_);
}

void NTupleTrackingParticles::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<std::vector<float> >    v_px    (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_py    (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_pz    (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_pt    (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_eta   (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_phi   (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_vx    (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_vy    (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_vz    (new std::vector<float>());
    std::auto_ptr<std::vector<int> >      v_pdgId (new std::vector<int>());
    std::auto_ptr<std::vector<int> >      v_genpId(new std::vector<int>());
    std::auto_ptr<std::vector<int> >      v_nhits (new std::vector<int>());
    std::auto_ptr<std::vector<int> >      v_ntkhits(new std::vector<int>());
    std::auto_ptr<std::vector<int> >      v_ntklayers(new std::vector<int>());
    std::auto_ptr<std::vector<unsigned> > v_evtId(new std::vector<unsigned>());
    std::auto_ptr<std::vector<std::vector<unsigned> > > v_trkIds(new std::vector<std::vector<unsigned> >());
    std::auto_ptr<unsigned>               v_size  (new unsigned(0));

    //__________________________________________________________________________
    if (!iEvent.isRealData()) {
        edm::Handle<TrackingParticleCollection> parts;
        iEvent.getByLabel(inputTag_, parts);

        if (parts.isValid()) {
            edm::LogInfo("NTupleTrackingParticles") << "Size: " << parts->size();

            unsigned n = 0;
            for (TrackingParticleCollection::const_iterator it = parts->begin(); it != parts->end(); ++it) {
                if (n >= maxN_)
                    break;
                if (!selector_(*it))
                    continue;

                // Fill the vectors
                v_px->push_back(it->px()); // first simTrack
                v_py->push_back(it->py());
                v_pz->push_back(it->pz());
                v_pt->push_back(it->pt());
                v_eta->push_back(it->eta());
                v_phi->push_back(it->phi());
                v_vx->push_back(it->vx()); // parent vertex
                v_vy->push_back(it->vy());
                v_vz->push_back(it->vz());
                v_pdgId->push_back(it->pdgId());
                int genpId = it->genParticles().empty() ? -99 : it->genParticles().begin()->key();
                v_genpId->push_back(genpId);
                v_nhits->push_back(it->numberOfHits());
                v_ntkhits->push_back(it->numberOfTrackerHits());
                v_ntklayers->push_back(it->numberOfTrackerLayers());
                v_evtId->push_back(it->eventId().rawId());

                std::vector<unsigned> trkIds;  // simTrackId (a.k.a. g4TrackId)
                for (TrackingParticle::g4t_iterator itsim = it->g4Track_begin(); itsim != it->g4Track_end(); ++itsim) {
                    trkIds.push_back(itsim->trackId());
                }
                v_trkIds->push_back(trkIds);

                n++;
            }
            *v_size = v_px->size();

        } else {
            edm::LogError("NTupleTrackingParticles") << "Cannot get the product: " << inputTag_;
        }
    }

    //__________________________________________________________________________
    iEvent.put(v_px       , prefix_ + "px"     + suffix_);
    iEvent.put(v_py       , prefix_ + "py"     + suffix_);
    iEvent.put(v_pz       , prefix_ + "pz"     + suffix_);
    iEvent.put(v_pt       , prefix_ + "pt"     + suffix_);
    iEvent.put(v_eta      , prefix_ + "eta"    + suffix_);
    iEvent.put(v_phi      , prefix_ + "phi"    + suffix_);
    iEvent.put(v_vx       , prefix_ + "vx"     + suffix_);
    iEvent.put(v_vy       , prefix_ + "vy"     + suffix_);
    iEvent.put(v_vz       , prefix_ + "vz"     + suffix_);
    iEvent.put(v_pdgId    , prefix_ + "pdgId"  + suffix_);
    iEvent.put(v_genpId   , prefix_ + "genpId" + suffix_);
    iEvent.put(v_nhits    , prefix_ + "nhits"  + suffix_);
    iEvent.put(v_ntkhits  , prefix_ + "ntkhits" + suffix_);
    iEvent.put(v_ntklayers, prefix_ + "ntklayers" + suffix_);
    iEvent.put(v_evtId    , prefix_ + "evtId"  + suffix_);
    iEvent.put(v_trkIds   , prefix_ + "trkIds" + suffix_);
    iEvent.put(v_size     , prefix_ + "size"   + suffix_);
}
