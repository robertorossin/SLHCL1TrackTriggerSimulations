#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleBeamSpot.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"


NTupleBeamSpot::NTupleBeamSpot(const edm::ParameterSet& iConfig) :
  inputTag_(iConfig.getParameter<edm::InputTag>("inputTag")),
  prefix_  (iConfig.getParameter<std::string>("prefix")),
  suffix_  (iConfig.getParameter<std::string>("suffix")),
  selector_(iConfig.existsAs<std::string>("cut") ? iConfig.getParameter<std::string>("cut") : "", true),
  maxN_    (iConfig.getParameter<unsigned>("maxN")) {

    produces<float> (prefix_ + "bx"         + suffix_);
    produces<float> (prefix_ + "by"         + suffix_);
    produces<float> (prefix_ + "bz"         + suffix_);
    produces<float> (prefix_ + "sigmaZ"     + suffix_);
    produces<float> (prefix_ + "beamWidthX" + suffix_);
    produces<float> (prefix_ + "beamWidthY" + suffix_);
}

void NTupleBeamSpot::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<float> v_bx        (new float(0.));
    std::auto_ptr<float> v_by        (new float(0.));
    std::auto_ptr<float> v_bz        (new float(0.));
    std::auto_ptr<float> v_sigmaZ    (new float(0.));
    std::auto_ptr<float> v_beamWidthX(new float(0.));
    std::auto_ptr<float> v_beamWidthY(new float(0.));

    //__________________________________________________________________________
    if (!iEvent.isRealData()) {
        edm::Handle<reco::BeamSpot> beamSpot;
        iEvent.getByLabel(inputTag_, beamSpot);

        if (beamSpot.isValid()) {

            // Fill the values
            const math::XYZPoint& position = beamSpot->position();
            *v_bx = position.x();
            *v_by = position.y();
            *v_bz = position.z();
            *v_sigmaZ = beamSpot->sigmaZ();
            *v_beamWidthX = beamSpot->BeamWidthX();
            *v_beamWidthY = beamSpot->BeamWidthY();

        } else if (inputTag_.encode() != "") {
            edm::LogError("NTupleBeamSpot") << "Cannot get the product: " << inputTag_;
        }
    }

    //__________________________________________________________________________
    iEvent.put(v_bx        , prefix_ + "bx"         + suffix_);
    iEvent.put(v_by        , prefix_ + "by"         + suffix_);
    iEvent.put(v_bz        , prefix_ + "bz"         + suffix_);
    iEvent.put(v_sigmaZ    , prefix_ + "sigmaZ"     + suffix_);
    iEvent.put(v_beamWidthX, prefix_ + "beamWidthX" + suffix_);
    iEvent.put(v_beamWidthY, prefix_ + "beamWidthY" + suffix_);
}
