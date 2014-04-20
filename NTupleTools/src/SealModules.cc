#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleGenParticles.h"
#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleGenJets.h"
#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleGenMET.h"
#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleGenEventInfo.h"
#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleMaker.h"

DEFINE_FWK_MODULE(NTupleGenParticles);
DEFINE_FWK_MODULE(NTupleGenJets);
DEFINE_FWK_MODULE(NTupleGenMET);
DEFINE_FWK_MODULE(NTupleGenEventInfo);
DEFINE_FWK_MODULE(NTupleMaker);

