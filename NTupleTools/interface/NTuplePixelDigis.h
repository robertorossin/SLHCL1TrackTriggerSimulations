#ifndef NTupleTools_NTuplePixelDigis_h_
#define NTupleTools_NTuplePixelDigis_h_

#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleCommon.h"

#include "Geometry/Records/interface/StackedTrackerGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"


class PixelDigi;

class NTuplePixelDigis : public edm::EDProducer {
  public:
    explicit NTuplePixelDigis(const edm::ParameterSet&);

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    virtual void beginRun(const edm::Run&, const edm::EventSetup&);
    //virtual void endRun(const edm::Run&, const edm::EventSetup&);

    // For event setup
    const TrackerGeometry * theGeometry;

    const edm::InputTag inputTag_, inputTagTP_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<PixelDigi> selector_;
    const unsigned maxN_;
};

#endif
