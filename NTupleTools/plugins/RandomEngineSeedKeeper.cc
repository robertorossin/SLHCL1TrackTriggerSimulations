#include "SLHCL1TrackTriggerSimulations/NTupleTools/plugins/RandomEngineSeedKeeper.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "SimDataFormats/RandomEngine/interface/RandomEngineStates.h"
#include "SimDataFormats/RandomEngine/interface/RandomEngineState.h"

#include <memory>

RandomEngineSeedKeeper::RandomEngineSeedKeeper(const edm::ParameterSet& iConfig)
: seeds_(iConfig.getParameter<std::vector<std::string> >("seeds")) {
    produces<std::vector<unsigned int> > ();
}

RandomEngineSeedKeeper::~RandomEngineSeedKeeper() {}

void RandomEngineSeedKeeper::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
    edm::Service<edm::RandomNumberGenerator> rng;
    if(rng.isAvailable()) {
        std::auto_ptr<std::vector<unsigned int> > v_seed(new std::vector<unsigned int>());

        // Automatically knows what module is requesting an engine
        //const CLHEP::HepRandomEngine& engine = rng->getEngine();

        const std::vector<RandomEngineState> & eventcache = rng->getEventCache();
        for (unsigned int j=0; j<seeds_.size(); ++j) {
            unsigned int seed0 = 0;

            for (unsigned int i=0; i<eventcache.size(); ++i) {
                const RandomEngineState& enginestate = eventcache.at(i);
                const std::vector<unsigned int>& engineseed = enginestate.getSeed();

                if (seeds_.at(j) == enginestate.getLabel() && !engineseed.empty()) {
                    seed0 = engineseed.front();
                }
            }
            v_seed->push_back(seed0);
        }
        iEvent.put(v_seed);
    }
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(RandomEngineSeedKeeper);

