#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleGenMET.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETFwd.h"


NTupleGenMET::NTupleGenMET(const edm::ParameterSet& iConfig) :
    inputTag(iConfig.getParameter<edm::InputTag>("InputTag")),
    prefix  (iConfig.getParameter<std::string>  ("Prefix")),
    suffix  (iConfig.getParameter<std::string>  ("Suffix"))
{
    produces <double> ( prefix + "Px" + suffix );
    produces <double> ( prefix + "Py" + suffix );
}

void NTupleGenMET::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr <double> px(new double());
    std::auto_ptr <double> py(new double());

    //-----------------------------------------------------------------
    if (!iEvent.isRealData()) {
        edm::Handle < reco::GenMETCollection > mets;
        iEvent.getByLabel(inputTag, mets);

        if (mets.isValid()) {
            edm::LogInfo("NTupleGenMET") << "Total # GenMETs: " << mets->size();
            reco::GenMET met(mets->at(0));
            *px = met.px();
            *py = met.py();
//            for (reco::GenMETCollection::const_iterator it = mets->begin(); it != mets->end(); ++it) {
//
//                // fill in all the vectors
//                px->push_back(it->px());
//                py->push_back(it->py());
//            }
        } else {
            edm::LogError("NTupleGenMET") << "Error! Can't get the product " << inputTag;
        }
    }

    //-----------------------------------------------------------------
    // put vectors in the event
    iEvent.put( px, prefix + "Px" + suffix );
    iEvent.put( py, prefix + "Py" + suffix );
}
