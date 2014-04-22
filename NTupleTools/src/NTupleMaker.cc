#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleMaker.h"

#include "FWCore/Framework/interface/ConstProductRegistry.h"
#include "FWCore/Framework/interface/ProductSelector.h"
#include "FWCore/Framework/interface/ProductSelectorRules.h"
#include "FWCore/Utilities/interface/TypeID.h"
#include "DataFormats/Provenance/interface/Selections.h"


template <class T>
NTupleMaker::TypedBranchConnector<T>::TypedBranchConnector(edm::BranchDescription const* desc, std::string t, TTree * tree) :
  moduleLabel_ (desc->moduleLabel() ),
  instanceName_(desc->productInstanceName() ),
  processName_ (desc->processName() ){

    ptr_object_ = &object_;
    std::string s = instanceName_;
    std::replace(s.begin(), s.end(), '@', '_');
    if (t != "") {
        tree->Branch(s.c_str(), ptr_object_, (s+"/"+t).c_str());  // raw type
    } else {
        tree->Branch(s.c_str(), &ptr_object_);  // STL type
    }
}

template <class T>
void NTupleMaker::TypedBranchConnector<T>::connect(const edm::Event& iEvent) {
    edm::Handle<T> handle;
    iEvent.getByLabel(moduleLabel_, instanceName_, handle);
    object_ = *handle;
}

NTupleMaker::NTupleMaker(const edm::ParameterSet& iConfig) : pset(iConfig) {

    edm::Service<TFileService> fs;
    tree = fs->make<TTree>("tree", "");

    leafmap[edm::TypeID(typeid(Char_t)).friendlyClassName()] = CHAR_T;
    leafmap[edm::TypeID(typeid(UChar_t)).friendlyClassName()] = UCHAR_T;
    leafmap[edm::TypeID(typeid(Short_t)).friendlyClassName()] = SHORT_T;
    leafmap[edm::TypeID(typeid(UShort_t)).friendlyClassName()] = USHORT_T;
    leafmap[edm::TypeID(typeid(Int_t)).friendlyClassName()] = INT_T;
    leafmap[edm::TypeID(typeid(UInt_t)).friendlyClassName()] = UINT_T;
    leafmap[edm::TypeID(typeid(Float_t)).friendlyClassName()] = FLOAT_T;
    leafmap[edm::TypeID(typeid(Double_t)).friendlyClassName()] = DOUBLE_T;
    leafmap[edm::TypeID(typeid(Long64_t)).friendlyClassName()] = LONG64_T;
    leafmap[edm::TypeID(typeid(ULong64_t)).friendlyClassName()] = ULONG64_T;

    leafmap[edm::TypeID(typeid(std::vector<Bool_t>)).friendlyClassName()] = BOOL_V;
    leafmap[edm::TypeID(typeid(std::vector<Char_t>)).friendlyClassName()] = CHAR_V;
    leafmap[edm::TypeID(typeid(std::vector<UChar_t>)).friendlyClassName()] = UCHAR_V;
    leafmap[edm::TypeID(typeid(std::vector<Short_t>)).friendlyClassName()] = SHORT_V;
    leafmap[edm::TypeID(typeid(std::vector<UShort_t>)).friendlyClassName()] = USHORT_V;
    leafmap[edm::TypeID(typeid(std::vector<Int_t>)).friendlyClassName()] = INT_V;
    leafmap[edm::TypeID(typeid(std::vector<UInt_t>)).friendlyClassName()] = UINT_V;
    leafmap[edm::TypeID(typeid(std::vector<Float_t>)).friendlyClassName()] = FLOAT_V;
    leafmap[edm::TypeID(typeid(std::vector<Double_t>)).friendlyClassName()] = DOUBLE_V;
    leafmap[edm::TypeID(typeid(std::vector<Long64_t>)).friendlyClassName()] = LONG64_V;
    leafmap[edm::TypeID(typeid(std::vector<ULong64_t>)).friendlyClassName()] = ULONG64_V;
    leafmap[edm::TypeID(typeid(std::vector<Bool_t>)).friendlyClassName()] = BOOL_V;

    leafmap[edm::TypeID(typeid(std::vector<std::vector<Bool_t> >)).friendlyClassName()] = BOOL_VV;
    leafmap[edm::TypeID(typeid(std::vector<std::vector<Char_t> >)).friendlyClassName()] = CHAR_VV;
    leafmap[edm::TypeID(typeid(std::vector<std::vector<UChar_t> >)).friendlyClassName()] = UCHAR_VV;
    leafmap[edm::TypeID(typeid(std::vector<std::vector<Short_t> >)).friendlyClassName()] = SHORT_VV;
    leafmap[edm::TypeID(typeid(std::vector<std::vector<UShort_t> >)).friendlyClassName()] = USHORT_VV;
    leafmap[edm::TypeID(typeid(std::vector<std::vector<Int_t> >)).friendlyClassName()] = INT_VV;
    leafmap[edm::TypeID(typeid(std::vector<std::vector<UInt_t> >)).friendlyClassName()] = UINT_VV;
    leafmap[edm::TypeID(typeid(std::vector<std::vector<Float_t> >)).friendlyClassName()] = FLOAT_VV;
    leafmap[edm::TypeID(typeid(std::vector<std::vector<Double_t> >)).friendlyClassName()] = DOUBLE_VV;
    leafmap[edm::TypeID(typeid(std::vector<std::vector<Long64_t> >)).friendlyClassName()] = LONG64_VV;
    leafmap[edm::TypeID(typeid(std::vector<std::vector<ULong64_t> >)).friendlyClassName()] = ULONG64_VV;
    leafmap[edm::TypeID(typeid(std::vector<std::vector<Bool_t> >)).friendlyClassName()] = BOOL_VV;
}

void NTupleMaker::beginJob() {
    edm::Service<edm::ConstProductRegistry> reg;
    edm::Selections allBranches = reg->allBranchDescriptions();
    edm::ProductSelectorRules productSelectorRules_(pset, "outputCommands", "NTupleMaker");
    edm::ProductSelector productSelector;
    if (!productSelector.initialized()) {
        productSelector.initialize(productSelectorRules_, allBranches);
    }

    std::set<std::string> branchNames;
    for (edm::Selections::const_iterator it = allBranches.begin(); it != allBranches.end(); ++it) {
        const edm::BranchDescription* selection = *it;

        if(productSelector.selected(*selection)) {
            //Check for duplicate branch names
            if (branchNames.find(selection->productInstanceName() ) != branchNames.end() ) {
                throw edm::Exception(edm::errors::Configuration)
                    << "Error in NTupleMaker: More than one branch named: "<< selection->productInstanceName() << std::endl;
            } else {
                branchNames.insert(selection->productInstanceName() );
            }

            // Determine the leave type
            std::map<std::string, LeafType>::iterator found = leafmap.find(selection->friendlyClassName() );
            switch (found->second) {
                case CHAR_T    : connectors.push_back(new TypedBranchConnector<Char_t>    (selection, "B", tree) ); break;
                case UCHAR_T   : connectors.push_back(new TypedBranchConnector<UChar_t>   (selection, "b", tree) ); break;
                case SHORT_T   : connectors.push_back(new TypedBranchConnector<Short_t>   (selection, "S", tree) ); break;
                case USHORT_T  : connectors.push_back(new TypedBranchConnector<UShort_t>  (selection, "s", tree) ); break;
                case INT_T     : connectors.push_back(new TypedBranchConnector<Int_t>     (selection, "I", tree) ); break;
                case UINT_T    : connectors.push_back(new TypedBranchConnector<UInt_t>    (selection, "i", tree) ); break;
                case FLOAT_T   : connectors.push_back(new TypedBranchConnector<Float_t>   (selection, "F", tree) ); break;
                case DOUBLE_T  : connectors.push_back(new TypedBranchConnector<Double_t>  (selection, "D", tree) ); break;
                case LONG64_T  : connectors.push_back(new TypedBranchConnector<Long64_t>  (selection, "L", tree) ); break;
                case ULONG64_T : connectors.push_back(new TypedBranchConnector<ULong64_t> (selection, "l", tree) ); break;
                case BOOL_T    : connectors.push_back(new TypedBranchConnector<Bool_t>    (selection, "O", tree) ); break;

                case CHAR_V    : connectors.push_back(new TypedBranchConnector<std::vector<Char_t> >    (selection, "", tree) ); break;
                case UCHAR_V   : connectors.push_back(new TypedBranchConnector<std::vector<UChar_t> >   (selection, "", tree) ); break;
                case SHORT_V   : connectors.push_back(new TypedBranchConnector<std::vector<Short_t> >   (selection, "", tree) ); break;
                case USHORT_V  : connectors.push_back(new TypedBranchConnector<std::vector<UShort_t> >  (selection, "", tree) ); break;
                case INT_V     : connectors.push_back(new TypedBranchConnector<std::vector<Int_t> >     (selection, "", tree) ); break;
                case UINT_V    : connectors.push_back(new TypedBranchConnector<std::vector<UInt_t> >    (selection, "", tree) ); break;
                case FLOAT_V   : connectors.push_back(new TypedBranchConnector<std::vector<Float_t> >   (selection, "", tree) ); break;
                case DOUBLE_V  : connectors.push_back(new TypedBranchConnector<std::vector<Double_t> >  (selection, "", tree) ); break;
                case LONG64_V  : connectors.push_back(new TypedBranchConnector<std::vector<Long64_t> >  (selection, "", tree) ); break;
                case ULONG64_V : connectors.push_back(new TypedBranchConnector<std::vector<ULong64_t> > (selection, "", tree) ); break;
                case BOOL_V    : connectors.push_back(new TypedBranchConnector<std::vector<Bool_t> >    (selection, "", tree) ); break;

                case CHAR_VV   : connectors.push_back(new TypedBranchConnector<std::vector<std::vector<Char_t> > >    (selection, "", tree) ); break;
                case UCHAR_VV  : connectors.push_back(new TypedBranchConnector<std::vector<std::vector<UChar_t> > >   (selection, "", tree) ); break;
                case SHORT_VV  : connectors.push_back(new TypedBranchConnector<std::vector<std::vector<Short_t> > >   (selection, "", tree) ); break;
                case USHORT_VV : connectors.push_back(new TypedBranchConnector<std::vector<std::vector<UShort_t> > >  (selection, "", tree) ); break;
                case INT_VV    : connectors.push_back(new TypedBranchConnector<std::vector<std::vector<Int_t> > >     (selection, "", tree) ); break;
                case UINT_VV   : connectors.push_back(new TypedBranchConnector<std::vector<std::vector<UInt_t> > >    (selection, "", tree) ); break;
                case FLOAT_VV  : connectors.push_back(new TypedBranchConnector<std::vector<std::vector<Float_t> > >   (selection, "", tree) ); break;
                case DOUBLE_VV : connectors.push_back(new TypedBranchConnector<std::vector<std::vector<Double_t> > >  (selection, "", tree) ); break;
                case LONG64_VV : connectors.push_back(new TypedBranchConnector<std::vector<std::vector<Long64_t> > >  (selection, "", tree) ); break;
                case ULONG64_VV: connectors.push_back(new TypedBranchConnector<std::vector<std::vector<ULong64_t> > > (selection, "", tree) ); break;
                case BOOL_VV   : connectors.push_back(new TypedBranchConnector<std::vector<std::vector<Bool_t> > >    (selection, "", tree) ); break;

                default       : throw edm::Exception(edm::errors::Configuration)
                                    << "Error in NTupleMaker: Cannot handle leaves of friendlyClassName: "
                                    << selection->friendlyClassName() << std::endl;
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
