#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleTTClusters.h"

NTupleTTClusters::NTupleTTClusters(const edm::ParameterSet& iConfig) :
  inputTag_   (iConfig.getParameter<edm::InputTag>("inputTag")),
  inputTagMC_ (iConfig.getParameter<edm::InputTag>("inputTagMC")),
  prefix_     (iConfig.getParameter<std::string>("prefix")),
  suffix_     (iConfig.getParameter<std::string>("suffix")),
  selector_   (iConfig.existsAs<std::string>("cut") ? iConfig.getParameter<std::string>("cut") : "", true),
  maxN_       (iConfig.getParameter<unsigned>("maxN")) {

}

void NTupleTTClusters::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

}
