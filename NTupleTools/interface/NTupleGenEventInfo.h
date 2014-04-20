#ifndef NTupleTools_NTupleGenEventInfo_h_
#define NTupleTools_NTupleGenEventInfo_h_

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

class NTupleGenEventInfo : public edm::EDProducer {
  public:
    explicit NTupleGenEventInfo(const edm::ParameterSet&);

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    const edm::InputTag genEventInfoTag_;
    const edm::InputTag pileupInfoTag_;
    const edm::InputTag pileupWeightTag_;
    const edm::InputTag pdfWeightTag_;
    const std::string   prefix_, suffix_;
};

#endif
