#ifndef NTupleTools_NTupleStubs_h_
#define NTupleTools_NTupleStubs_h_

#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleCommon.h"

#include "Geometry/Records/interface/StackedTrackerGeometryRecord.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/StackedTrackerGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/StackedTrackerDetUnit.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"


class NTupleStubs : public edm::EDProducer {
  public:
    explicit NTupleStubs(const edm::ParameterSet&);

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    virtual void beginRun(const edm::Run&, const edm::EventSetup&);
    //virtual void endRun(const edm::Run&, const edm::EventSetup&);

    // For event setup
    const TrackerGeometry * theGeometry;
    const StackedTrackerGeometry * theStackedGeometry;
    const MagneticField* theMagneticField;

    const edm::InputTag inputTagClus_, inputTagStub_, inputTagTrack_, inputTagDigi_;
    const edm::InputTag inputTagClusMCAssoc_, inputTagStubMCAssoc_, inputTagTrackMCAssoc_;
    const std::string   prefixClus_, prefixStub_, prefixTrack_, prefixDigi_, suffix_;
};

#endif
