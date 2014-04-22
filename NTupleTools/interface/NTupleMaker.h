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

#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleCommon.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include <map>
#include <string>
#include <vector>
#include "TTree.h"


class NTupleMaker : public edm::EDAnalyzer {
  public:
    // NOTE: STRING is not implemented
    enum LeafType {
        CHAR_T=1, UCHAR_T  , SHORT_T  , USHORT_T  , INT_T  , UINT_T ,
        FLOAT_T , DOUBLE_T , LONG64_T , ULONG64_T , BOOL_T ,
        CHAR_V  , UCHAR_V  , SHORT_V  , USHORT_V  , INT_V  , UINT_V ,
        FLOAT_V , DOUBLE_V , LONG64_V , ULONG64_V , BOOL_V ,
        CHAR_VV , UCHAR_VV , SHORT_VV , USHORT_VV , INT_VV , UINT_VV,
        FLOAT_VV, DOUBLE_VV, LONG64_VV, ULONG64_VV, BOOL_VV,
        STRING,
        NumLeafTypes
    };

    explicit NTupleMaker(const edm::ParameterSet&);

  private:
    virtual void beginJob();
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob(){}

    // Connect branch to handle
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
        T* ptr_object_;
    };

    edm::ParameterSet pset;
    TTree * tree;

    std::map<std::string, LeafType> leafmap;
    std::vector<BranchConnector*> connectors;
};

#endif
