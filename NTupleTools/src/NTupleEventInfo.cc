#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleEventInfo.h"


NTupleEventInfo::NTupleEventInfo(const edm::ParameterSet& iConfig) :
  prefix_  (iConfig.getParameter<std::string>("prefix")),
  suffix_  (iConfig.getParameter<std::string>("suffix")) {

    produces<unsigned> ("event");
    produces<unsigned> ("run");
    produces<unsigned> ("lumi");
    produces<int>      ("bx");
}

void NTupleEventInfo::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<unsigned> v_event(new unsigned(0));
    std::auto_ptr<unsigned> v_run  (new unsigned(0));
    std::auto_ptr<unsigned> v_lumi (new unsigned(0));
    std::auto_ptr<int>      v_bx   (new int(-1));

    //__________________________________________________________________________
    *v_event = iEvent.id().event();
    *v_run   = iEvent.id().run();
    *v_lumi  = iEvent.id().luminosityBlock();
    *v_bx    = iEvent.bunchCrossing();

    //__________________________________________________________________________
    iEvent.put(v_event, "event");
    iEvent.put(v_run  , "run");
    iEvent.put(v_lumi , "lumi");
    iEvent.put(v_bx   , "bx");
}

