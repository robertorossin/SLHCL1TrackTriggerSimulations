#ifndef NTupleTools_NTupleStubs_h_
#define NTupleTools_NTupleStubs_h_

#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleCommon.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/Records/interface/StackedTrackerGeometryRecord.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/StackedTrackerGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/StackedTrackerDetUnit.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"


// TODO: factorize the code...

class NTupleStubs : public edm::EDProducer {
  public:
    explicit NTupleStubs(const edm::ParameterSet&);

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    // ESHandle
    edm::ESHandle<TrackerGeometry>        geometryHandle;
    edm::ESHandle<StackedTrackerGeometry> stackedGeometryHandle;
    edm::ESHandle<MagneticField>          magneticFieldHandle;

    const edm::InputTag inputTagClus_, inputTagStub_, inputTagTrack_;
    const edm::InputTag inputTagClusMCAssoc_, inputTagStubMCAssoc_, inputTagTrackMCAssoc_;
    const edm::InputTag inputTagTrkPart_, inputTagTrkVertex_;
    const edm::InputTag inputTagSimTrack_, inputTagSimVertex_, inputTagSimPixelDigi_;
    const edm::InputTag beamSpotTag_;
    const std::string   prefixClus_, prefixStub_, prefixTrack_, suffix_;

    const unsigned int  maxN_;
};

#endif
