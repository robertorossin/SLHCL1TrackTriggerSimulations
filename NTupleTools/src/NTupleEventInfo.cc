#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleEventInfo.h"


NTupleEventInfo::NTupleEventInfo(const edm::ParameterSet& iConfig) :
  prefix_  (iConfig.getParameter<std::string>("prefix")),
  suffix_  (iConfig.getParameter<std::string>("suffix")) {

    produces<unsigned int> ("event");
    produces<unsigned int> ("run");
    produces<unsigned int> ("lumi");
}

void NTupleEventInfo::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<unsigned int> v_event(new unsigned int(0));
    std::auto_ptr<unsigned int> v_run  (new unsigned int(0));
    std::auto_ptr<unsigned int> v_lumi (new unsigned int(0));

    //__________________________________________________________________________
    *v_event = iEvent.id().event();
    *v_run   = iEvent.id().run();
    *v_lumi  = iEvent.id().luminosityBlock();

    //__________________________________________________________________________
    iEvent.put(v_event, "event");
    iEvent.put(v_run  , "run");
    iEvent.put(v_lumi , "lumi");
}
