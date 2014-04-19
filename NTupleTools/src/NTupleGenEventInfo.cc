#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleGenEventInfo.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"


NTupleGenEventInfo::NTupleGenEventInfo(const edm::ParameterSet& iConfig) :
  genEventInfoTag_(iConfig.getParameter<edm::InputTag>("genEventInfo")),
  pileupInfoTag_(iConfig.getParameter<edm::InputTag>("pileupInfo")),
  pileupWeightTag_(iConfig.getParameter<edm::InputTag>("pileupWeight")),
  pdfWeightTag_(iConfig.getParameter<edm::InputTag>("pdfWeight")),
  prefix_  (iConfig.getParameter<std::string>("prefix")),
  suffix_  (iConfig.getParameter<std::string>("suffix")) {

    produces<float> (prefix_ + "trueNPV"  + suffix_);
	produces<float> (prefix_ + "weightMC" + suffix_);
    produces<float> (prefix_ + "weightPU" + suffix_);
    produces<float> (prefix_ + "weightPDF"+ suffix_);
}

void NTupleGenEventInfo::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<float> v_trueNPV  (new float(-999.));
    std::auto_ptr<float> v_weightMC (new float(-999.));
    std::auto_ptr<float> v_weightPU (new float(-999.));
    std::auto_ptr<float> v_weightPDF(new float(-999.));

    //__________________________________________________________________________
    if (!iEvent.isRealData()) {
        edm::Handle<GenEventInfoProduct> genEventInfo;
        iEvent.getByLabel(genEventInfoTag_, genEventInfo);
        if (genEventInfo.isValid()) {
            *v_weightMC = genEventInfo->weight();
        }

        edm::Handle<std::vector<PileupSummaryInfo> > pileupInfo;
        iEvent.getByLabel(pileupInfoTag_, pileupInfo);
        if (pileupInfo.isValid()) {
            *v_trueNPV = pileupInfo->front().getTrueNumInteractions();
        }

        if (pileupWeightTag_.encode() != "") {
            edm::Handle<double> pileupWeight;
            iEvent.getByLabel(pileupWeightTag_, pileupWeight);
            if (pileupWeight.isValid()) {
                *v_weightPU = *pileupWeight;
            }
        }

        if (pdfWeightTag_.encode() != "") {
            edm::Handle<double> pdfWeight;
            iEvent.getByLabel(pdfWeightTag_, pdfWeight);
            if (pdfWeight.isValid()) {
                *v_weightPDF = *pdfWeight;
            }
        }
    }

    //__________________________________________________________________________
    iEvent.put(v_trueNPV  , prefix_ + "trueNPV"  + suffix_);
	iEvent.put(v_weightMC , prefix_ + "weightMC" + suffix_);
    iEvent.put(v_weightPU , prefix_ + "weightPU" + suffix_);
    iEvent.put(v_weightPDF, prefix_ + "weightPDF"+ suffix_);
}
