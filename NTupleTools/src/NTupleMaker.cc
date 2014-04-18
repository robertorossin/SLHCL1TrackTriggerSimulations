#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleMaker.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ConstProductRegistry.h"
#include "FWCore/Framework/interface/ProductSelector.h"
#include "FWCore/Framework/interface/ProductSelectorRules.h"
#include "DataFormats/Provenance/interface/Selections.h"
#include "Math/LorentzVector.h"
#include "Math/Vector3D.h"

#include <map>
#include "boost/foreach.hpp"
#include <TBranch.h>
#include <TLorentzVector.h>

template <class T>
NTupleMaker::TypedBranchConnector<T>::TypedBranchConnector(edm::BranchDescription const* desc, std::string st, TTree * tree)
: moduleLabel_ (desc->moduleLabel() ),
  instanceName_(desc->productInstanceName() ),
  processName_ (desc->processName() ){
    object_ptr_ = &object_;
    std::string s = instanceName_ + st;
    if (st != "") {
        tree->Branch(instanceName_.c_str(),  object_ptr_);  // raw type
    } else {
        tree->Branch(instanceName_.c_str(), &object_ptr_);  // STL type
    }
}

template <class T>
void NTupleMaker::TypedBranchConnector<T>::connect(const edm::Event& iEvent) {
  edm::Handle<T> handle_;
  iEvent.getByLabel(moduleLabel_, instanceName_, handle_);
  object_ = *handle_;
}

void NTupleMaker::beginJob() {

    edm::Service<edm::ConstProductRegistry> reg;
    edm::Selections allBranches = reg->allBranchDescriptions();
    edm::ProductSelectorRules productSelectorRules_(pset, "outputCommands", "NTupleMaker");
    edm::ProductSelector productSelector;
    if (!productSelector.initialized()) {
        productSelector.initialize(productSelectorRules_, allBranches);
    }

    std::set<std::string> branchnames;
    for (edm::Selections::const_iterator it = allBranches.begin(); it != allBranches.end(); ++it) {
        const edm::BranchDescription* selection = *it;

        if(productSelector.selected(*selection)) {
            //Check for duplicate branch names
            if (branchnames.find(selection->productInstanceName() ) != branchnames.end() ) {
                throw edm::Exception(edm::errors::Configuration)
                    << "Error in NTupleMaker: More than one branch named: "<< selection->productInstanceName() << std::endl;
            } else {
                branchnames.insert(selection->productInstanceName() );
            }

            if (selection->friendlyClassName() == "float") {
                connectors.push_back(new TypedBranchConnector<float>(selection, "/F", tree));
            } else if (selection->friendlyClassName() == "floats") {
                connectors.push_back(new TypedBranchConnector<std::vector<float> >(selection, "", tree));
            } else {
                throw edm::Exception(edm::errors::Configuration)
                    << "Error in NTupleMaker: Cannot handle leaves of friendlyClassName: " << selection->friendlyClassName() << std::endl;
            }
        }
    }
}


void NTupleMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

    for (std::vector<BranchConnector*>::iterator it = connectors.begin(); it != connectors.end(); ++it) {
        (*it)->connect(iEvent);
    }
    tree->Fill();
}

