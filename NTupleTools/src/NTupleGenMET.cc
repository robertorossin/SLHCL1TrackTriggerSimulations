#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleGenMET.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETFwd.h"


NTupleGenMET::NTupleGenMET(const edm::ParameterSet& iConfig) :
  inputTag_(iConfig.getParameter<edm::InputTag>("inputTag")),
  prefix_  (iConfig.getParameter<std::string>("prefix")),
  suffix_  (iConfig.getParameter<std::string>("suffix")) {

    produces<float> (prefix_ + "px"    + suffix_);
	produces<float> (prefix_ + "py"    + suffix_);
    produces<float> (prefix_ + "pt"    + suffix_);
	produces<float> (prefix_ + "phi"   + suffix_);
    produces<float> (prefix_ + "sumEt" + suffix_);
}

void NTupleGenMET::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<float> v_px    (new float());
    std::auto_ptr<float> v_py    (new float());
    std::auto_ptr<float> v_pt    (new float());
    std::auto_ptr<float> v_phi   (new float());
    std::auto_ptr<float> v_sumEt (new float());

    //__________________________________________________________________________
    if (!iEvent.isRealData()) {
        edm::Handle < reco::GenMETCollection > mets;
        iEvent.getByLabel(inputTag_, mets);

        if (mets.isValid() && mets->size() > 0) {
            edm::LogInfo("NTupleGenMET") << "Size: " << mets->size();

            reco::GenMETCollection::const_iterator it = mets->begin();

            // Fill the values
            *v_px    = it->px();
            *v_py    = it->py();
            *v_pt    = it->pt();
            *v_phi   = it->phi();
            *v_sumEt = it->sumEt();

        } else {
            edm::LogError("NTupleGenMET") << "Cannot get the product: " << inputTag_;
        }
    }

    //__________________________________________________________________________
    iEvent.put(v_px    , prefix_ + "px"    + suffix_);
	iEvent.put(v_py    , prefix_ + "py"    + suffix_);
    iEvent.put(v_pt    , prefix_ + "pt"    + suffix_);
    iEvent.put(v_phi   , prefix_ + "phi"   + suffix_);
    iEvent.put(v_sumEt , prefix_ + "sumEt" + suffix_);
}
