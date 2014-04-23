#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleTrackingVertices.h"

#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertex.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertexContainer.h"


NTupleTrackingVertices::NTupleTrackingVertices(const edm::ParameterSet& iConfig) :
  inputTag_(iConfig.getParameter<edm::InputTag>("inputTag")),
  prefix_  (iConfig.getParameter<std::string>("prefix")),
  suffix_  (iConfig.getParameter<std::string>("suffix")),
  selector_(iConfig.existsAs<std::string>("cut") ? iConfig.getParameter<std::string>("cut") : "", true),
  maxN_    (iConfig.getParameter<unsigned>("maxN")) {

    produces<std::vector<float> >    (prefix_ + "vx"     + suffix_);
    produces<std::vector<float> >    (prefix_ + "vy"     + suffix_);
    produces<std::vector<float> >    (prefix_ + "vz"     + suffix_);
    produces<std::vector<float> >    (prefix_ + "tof"    + suffix_);
    produces<std::vector<unsigned> > (prefix_ + "evtId"  + suffix_);
    produces<std::vector<std::vector<unsigned> > > (prefix_ + "vtxIds" + suffix_);
    produces<unsigned>               (prefix_ + "size"   + suffix_);
}

void NTupleTrackingVertices::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<std::vector<float> >    v_vx    (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_vy    (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_vz    (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_tof   (new std::vector<float>());
    std::auto_ptr<std::vector<unsigned> > v_evtId (new std::vector<unsigned>());
    std::auto_ptr<std::vector<std::vector<unsigned> > > v_vtxIds(new std::vector<std::vector<unsigned> >());
    std::auto_ptr<unsigned>               v_size  (new unsigned(0));

    //__________________________________________________________________________
    if (!iEvent.isRealData()) {
        edm::Handle<TrackingVertexCollection> vertices;
        iEvent.getByLabel(inputTag_, vertices);

        if (vertices.isValid()) {
            edm::LogInfo("NTupleTrackingVertices") << "Size: " << vertices->size();

            unsigned n = 0;
            for (TrackingVertexCollection::const_iterator it = vertices->begin(); it != vertices->end(); ++it) {
                if (n >= maxN_)
                    break;
                if (!selector_(*it))
                    continue;

                // Fill the vectors
                const math::XYZTLorentzVectorD& position = it->position();
                v_vx->push_back(position.x());
                v_vy->push_back(position.y());
                v_vz->push_back(position.z());
                v_tof->push_back(position.t());
                v_evtId->push_back(it->eventId().rawId());

                std::vector<unsigned> vtxIds;  // simVertexId (a.k.a. g4VertexId)
                for (TrackingVertex::g4v_iterator itsim = it->g4Vertices_begin(); itsim != it->g4Vertices_end(); ++itsim) {
                    vtxIds.push_back(itsim->vertexId());
                }
                v_vtxIds->push_back(vtxIds);

                n++;
            }
            *v_size = v_vx->size();

        } else {
            edm::LogError("NTupleTrackingVertices") << "Cannot get the product: " << inputTag_;
        }
    }

    //__________________________________________________________________________
    iEvent.put(v_vx    , prefix_ + "vx"     + suffix_);
    iEvent.put(v_vy    , prefix_ + "vy"     + suffix_);
    iEvent.put(v_vz    , prefix_ + "vz"     + suffix_);
    iEvent.put(v_tof   , prefix_ + "tof"    + suffix_);
    iEvent.put(v_evtId , prefix_ + "evtId"  + suffix_);
    iEvent.put(v_vtxIds, prefix_ + "vtxIds" + suffix_);
    iEvent.put(v_size  , prefix_ + "size"   + suffix_);
}
