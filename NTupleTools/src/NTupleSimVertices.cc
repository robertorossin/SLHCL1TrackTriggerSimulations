#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleSimVertices.h"

#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"


NTupleSimVertices::NTupleSimVertices(const edm::ParameterSet& iConfig) :
  inputTag_(iConfig.getParameter<edm::InputTag>("inputTag")),
  prefix_  (iConfig.getParameter<std::string>("prefix")),
  suffix_  (iConfig.getParameter<std::string>("suffix")),
  selector_(iConfig.existsAs<std::string>("cut") ? iConfig.getParameter<std::string>("cut") : "", true),
  maxN_    (iConfig.getParameter<unsigned>("maxN")) {

    produces<std::vector<float> >    (prefix_ + "vx"     + suffix_);
    produces<std::vector<float> >    (prefix_ + "vy"     + suffix_);
    produces<std::vector<float> >    (prefix_ + "vz"     + suffix_);
    produces<std::vector<float> >    (prefix_ + "tof"    + suffix_);
    produces<std::vector<unsigned> > (prefix_ + "vtxId"  + suffix_);
    produces<std::vector<int> >      (prefix_ + "trkId"  + suffix_);
    produces<unsigned>               (prefix_ + "size"   + suffix_);
}

void NTupleSimVertices::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<std::vector<float> >    v_vx    (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_vy    (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_vz    (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_tof   (new std::vector<float>());
    std::auto_ptr<std::vector<unsigned> > v_vtxId (new std::vector<unsigned>());
    std::auto_ptr<std::vector<int> >      v_trkId (new std::vector<int>());
    std::auto_ptr<unsigned>               v_size  (new unsigned(0));

    //__________________________________________________________________________
    if (!iEvent.isRealData()) {
        edm::Handle<edm::SimVertexContainer> vertices;
        iEvent.getByLabel(inputTag_, vertices);

        if (vertices.isValid()) {
            edm::LogInfo("NTupleSimVertices") << "Size: " << vertices->size();

            unsigned n = 0;
            for (edm::SimVertexContainer::const_iterator it = vertices->begin(); it != vertices->end(); ++it) {
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
                v_vtxId->push_back(it->vertexId());
                v_trkId->push_back(it->parentIndex());

                n++;
            }
            *v_size = v_vx->size();

        } else {
            edm::LogError("NTupleSimVertices") << "Cannot get the product: " << inputTag_;
        }
    }

    //__________________________________________________________________________
    iEvent.put(v_vx    , prefix_ + "vx"     + suffix_);
    iEvent.put(v_vy    , prefix_ + "vy"     + suffix_);
    iEvent.put(v_vz    , prefix_ + "vz"     + suffix_);
    iEvent.put(v_tof   , prefix_ + "tof"    + suffix_);
    iEvent.put(v_vtxId , prefix_ + "vtxId"  + suffix_);
    iEvent.put(v_trkId , prefix_ + "trkId"  + suffix_);
    iEvent.put(v_size  , prefix_ + "size"   + suffix_);
}
