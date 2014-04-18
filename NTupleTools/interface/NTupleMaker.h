#ifndef NTupleTools_NTupleMaker_h_
#define NTupleTools_NTupleMaker_h_

/** \class NTupleMaker
 *
 *  This is stolen from:
 *    https://github.com/BristolTopGroup/NTupleProduction/blob/master/src/RootTupleMakerV2_Tree.cc
 *
 *  Makes a tree out of C++ standard types and vectors of C++ standard types
 *
 *  This class, which is an EDAnalyzer, takes the same "keep" and
 *  "drop" outputCommands parameter as the PoolOutputSource, making a
 *  tree of the selected variables, which it obtains from the EDM
 *  tree.
 *
 *  \author Burt Betchart - University of Rochester <burton.andrew.betchart@cern.ch>
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"

class NTupleMaker : public edm::EDAnalyzer {
  public:
    enum LeafType {
        FLOAT, FLOATS
    };

    explicit NTupleMaker(const edm::ParameterSet& iConfig) : pset(iConfig) {
        edm::Service<TFileService> fs;
        tree = fs->make<TTree>("tree", "");
    }

  private:
    virtual void beginJob();
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob(){}

    class BranchConnector {
      public:
        BranchConnector() {};
        virtual ~BranchConnector() {};
        virtual void connect(const edm::Event&) = 0;
    };

    template <class T>
    class TypedBranchConnector : public BranchConnector {
      public:
        TypedBranchConnector(edm::BranchDescription const*, std::string, TTree*);
        void connect(const edm::Event&);

      private:
        std::string moduleLabel_;
        std::string instanceName_;
        std::string processName_;
        T object_;
        T* object_ptr_;
    };

    edm::ParameterSet pset;
    TTree * tree;

    std::map<std::string, LeafType> leafmap;
    std::vector<BranchConnector*> connectors;
};

#endif
