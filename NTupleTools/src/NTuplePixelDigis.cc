#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTuplePixelDigis.h"

#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/ModuleIdFunctor.h"
#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/SimTrackToTrackingParticleMap.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiPixelDigi/interface/PixelDigi.h"
#include "SimDataFormats/TrackerDigiSimLink/interface/PixelDigiSimLink.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"

NTuplePixelDigis::NTuplePixelDigis(const edm::ParameterSet& iConfig) :
  inputTag_  (iConfig.getParameter<edm::InputTag>("inputTag")),
  inputTagTP_(iConfig.getParameter<edm::InputTag>("inputTagTP")),
  prefix_    (iConfig.getParameter<std::string>("prefix")),
  suffix_    (iConfig.getParameter<std::string>("suffix")),
  selector_  (iConfig.existsAs<std::string>("cut") ? iConfig.getParameter<std::string>("cut") : "", true),
  maxN_      (iConfig.getParameter<unsigned>("maxN")) {

    produces<std::vector<float> >                   (prefix_ + "x"              + suffix_);
    produces<std::vector<float> >                   (prefix_ + "y"              + suffix_);
    produces<std::vector<float> >                   (prefix_ + "z"              + suffix_);
    produces<std::vector<float> >                   (prefix_ + "r"              + suffix_);
    produces<std::vector<float> >                   (prefix_ + "eta"            + suffix_);
    produces<std::vector<float> >                   (prefix_ + "phi"            + suffix_);
    produces<std::vector<float> >                   (prefix_ + "localx"         + suffix_);
    produces<std::vector<float> >                   (prefix_ + "localy"         + suffix_);
    produces<std::vector<float> >                   (prefix_ + "localz"         + suffix_);
    produces<std::vector<unsigned> >                (prefix_ + "modId"          + suffix_);
    produces<std::vector<unsigned> >                (prefix_ + "geoId"          + suffix_);
    //produces<std::vector<int> >                     (prefix_ + "col"            + suffix_);
    //produces<std::vector<int> >                     (prefix_ + "row"            + suffix_);
    produces<std::vector<int> >                     (prefix_ + "adc"            + suffix_);
    produces<std::vector<int> >                     (prefix_ + "chan"           + suffix_);
    produces<std::vector<std::vector<unsigned> > >  (prefix_ + "trkIds"         + suffix_);
    produces<std::vector<std::vector<unsigned> > >  (prefix_ + "evtIds"         + suffix_);
    produces<std::vector<std::vector<int> > >       (prefix_ + "tpIds"          + suffix_);
    produces<std::vector<std::vector<unsigned> > >  (prefix_ + "cfPositions"    + suffix_);
    produces<std::vector<std::vector<bool> > >      (prefix_ + "tofBins"        + suffix_);
    produces<std::vector<std::vector<float> > >     (prefix_ + "fractions"      + suffix_);
    produces<unsigned>                              (prefix_ + "size"           + suffix_);
}

void NTuplePixelDigis::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {
    /// Geometry setup
    edm::ESHandle<TrackerGeometry> geometryHandle;
    iSetup.get<TrackerDigiGeometryRecord>().get(geometryHandle);
    theGeometry = geometryHandle.product();
}

void NTuplePixelDigis::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<std::vector<float> >                  v_x           (new std::vector<float>());
    std::auto_ptr<std::vector<float> >                  v_y           (new std::vector<float>());
    std::auto_ptr<std::vector<float> >                  v_z           (new std::vector<float>());
    std::auto_ptr<std::vector<float> >                  v_r           (new std::vector<float>());
    std::auto_ptr<std::vector<float> >                  v_eta         (new std::vector<float>());
    std::auto_ptr<std::vector<float> >                  v_phi         (new std::vector<float>());
    std::auto_ptr<std::vector<float> >                  v_localx      (new std::vector<float>());
    std::auto_ptr<std::vector<float> >                  v_localy      (new std::vector<float>());
    std::auto_ptr<std::vector<float> >                  v_localz      (new std::vector<float>());
    std::auto_ptr<std::vector<unsigned> >               v_modId       (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> >               v_geoId       (new std::vector<unsigned>());
    //std::auto_ptr<std::vector<int> >                    v_col         (new std::vector<int>());
    //std::auto_ptr<std::vector<int> >                    v_row         (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v_adc         (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v_chan        (new std::vector<int>());
    std::auto_ptr<std::vector<std::vector<unsigned> > > v_trkIds      (new std::vector<std::vector<unsigned> >());
    std::auto_ptr<std::vector<std::vector<unsigned> > > v_evtIds      (new std::vector<std::vector<unsigned> >());
    std::auto_ptr<std::vector<std::vector<int> > >      v_tpIds       (new std::vector<std::vector<int> >());
    std::auto_ptr<std::vector<std::vector<unsigned> > > v_cfPositions (new std::vector<std::vector<unsigned> >());
    std::auto_ptr<std::vector<std::vector<bool> > >     v_tofBins     (new std::vector<std::vector<bool> >());
    std::auto_ptr<std::vector<std::vector<float> > >    v_fractions   (new std::vector<std::vector<float> >());
    std::auto_ptr<unsigned>                             v_size        (new unsigned(0));

    edm::Handle<edm::DetSetVector<PixelDigi> > pixelDigis;
    iEvent.getByLabel(inputTag_, pixelDigis);

    edm::Handle<edm::DetSetVector<PixelDigiSimLink> >  pixelDigiSimLinks;
    if (!iEvent.isRealData())
        iEvent.getByLabel(inputTag_, pixelDigiSimLinks);

    /// Prepare a map of simTrack -> trackingParticle
    edm::Handle<TrackingParticleCollection> trackingParticleHandle;
    iEvent.getByLabel(inputTagTP_, trackingParticleHandle);

    SimTrackToTrackingParticleMap trkToTPMap;
    trkToTPMap.setup(trackingParticleHandle);

    /// Prepare detId -> moduleId
    ModuleIdFunctor getModuleId;

    if (pixelDigis.isValid()) {
        edm::LogInfo("NTuplePixelDigis") << "Size: " << pixelDigis->size();

        typedef edm::DetSetVector<PixelDigi>::const_iterator const_dsv_iterator;
        typedef edm::DetSet      <PixelDigi>::const_iterator const_ds_iterator;
        typedef edm::DetSet      <PixelDigiSimLink>::const_iterator const_dslink_iterator;

        unsigned n = 0;
        for (const_dsv_iterator itv = pixelDigis->begin(); itv != pixelDigis->end(); ++itv) {
            uint32_t rawId = itv->detId();
            const DetId geoId(rawId);
            DetId::Detector det = geoId.det();  // Tracker=1,Muon=2,Ecal=3,Hcal=4,Calo=5,Forward=6
            int subdetId = geoId.subdetId();  // PXB=1,PXF=2,...
            bool isBarrel = (subdetId == (int) PixelSubdetector::PixelBarrel);
            bool isEndcap = (subdetId == (int) PixelSubdetector::PixelEndcap);
            if (!isBarrel && !isEndcap)
                continue;  // only tracker

            const PixelGeomDetUnit* geomDetUnit = dynamic_cast<const PixelGeomDetUnit*>(theGeometry->idToDetUnit(geoId));
            const PixelTopology* topology = dynamic_cast<const PixelTopology*>(&(geomDetUnit->specificTopology()) );

            // This cut is as used in Geometry/TrackerGeometryBuilder/src/StackedTrackerGeometryBuilder.cc
            if (geomDetUnit->position().perp()<20.)
                continue;  // only outer tracker

            /// Module ID
            unsigned moduleId = getModuleId(geoId);
            edm::LogInfo("NTuplePixelDigis") << "rawId: " << geoId.rawId() << " det: " << det << " subdetId: " << subdetId << " modId: " << moduleId << " size: " << itv->size();

            for (const_ds_iterator it = itv->begin(); it != itv->end(); ++it) {
                //if (!selector_(*it))
                //    continue;

                int row = it->row();
                int col = it->column();
                int channel = it->channel();  // = ((row << 11) | col)
                unsigned short adc = it->adc();
                edm::LogInfo("NTuplePixelDigis") << "row: " << row << " col: " << col << " channel: " << channel << " adc: " << adc;

                MeasurementPoint mp((float) row + 0.5, (float) col + 0.5);
                const LocalPoint& localPosition = topology->localPosition(mp);
                const GlobalPoint& globalPosition = geomDetUnit->surface().toGlobal(localPosition);

                // Find simlinks
                std::vector<unsigned> trkIds;
                std::vector<unsigned> evtIds;
                std::vector<int>      tpIds;
                std::vector<unsigned> cfPositions;
                std::vector<bool>     tofBins;
                std::vector<float>    fractions;
                if (pixelDigiSimLinks->find(geoId) != pixelDigiSimLinks->end()) {
                    const edm::DetSet<PixelDigiSimLink>& simlink = (*pixelDigiSimLinks)[geoId];
                    for (const_dslink_iterator itsim = simlink.data.begin(); itsim != simlink.data.end(); ++itsim) {
                        if (channel == (int) itsim->channel()) {
                            trkIds     .push_back(itsim->SimTrackId());
                            evtIds     .push_back(itsim->eventId().rawId());
                            tpIds      .push_back(trkToTPMap.get(itsim->SimTrackId(), itsim->eventId()));
                            cfPositions.push_back(itsim->CFposition());
                            tofBins    .push_back(itsim->TofBin());
                            fractions  .push_back(itsim->fraction());
                        }
                    }
                }

                v_x->push_back(globalPosition.x());
                v_y->push_back(globalPosition.y());
                v_z->push_back(globalPosition.z());
                v_r->push_back(globalPosition.perp());
                v_eta->push_back(globalPosition.eta());
                v_phi->push_back(globalPosition.phi());
                v_localx->push_back(localPosition.x());
                v_localy->push_back(localPosition.y());
                v_localz->push_back(localPosition.z());
                v_modId->push_back(moduleId);
                v_geoId->push_back(geoId.rawId());
                //v_col->push_back(col);
                //v_row->push_back(row);
                v_adc->push_back(adc);
                v_chan->push_back(channel);
                v_trkIds->push_back(trkIds);
                v_evtIds->push_back(evtIds);
                v_tpIds->push_back(tpIds);
                v_cfPositions->push_back(cfPositions);  // link to PSimHits
                v_tofBins->push_back(tofBins);
                v_fractions->push_back(fractions);

                n++;
            }
        }
        *v_size = v_x->size();

    } else {
        edm::LogError("NTuplePixelDigis") << "Cannot get the product: " << inputTag_;
    }


    //__________________________________________________________________________
    iEvent.put(v_x           , prefix_ + "x"            + suffix_);
    iEvent.put(v_y           , prefix_ + "y"            + suffix_);
    iEvent.put(v_z           , prefix_ + "z"            + suffix_);
    iEvent.put(v_r           , prefix_ + "r"            + suffix_);
    iEvent.put(v_eta         , prefix_ + "eta"          + suffix_);
    iEvent.put(v_phi         , prefix_ + "phi"          + suffix_);
    iEvent.put(v_localx      , prefix_ + "localx"       + suffix_);
    iEvent.put(v_localy      , prefix_ + "localy"       + suffix_);
    iEvent.put(v_localz      , prefix_ + "localz"       + suffix_);
    iEvent.put(v_modId       , prefix_ + "modId"        + suffix_);
    iEvent.put(v_geoId       , prefix_ + "geoId"        + suffix_);
    //iEvent.put(v_col         , prefix_ + "col"          + suffix_);
    //iEvent.put(v_row         , prefix_ + "row"          + suffix_);
    iEvent.put(v_adc         , prefix_ + "adc"          + suffix_);
    iEvent.put(v_chan        , prefix_ + "chan"         + suffix_);
    iEvent.put(v_trkIds      , prefix_ + "trkIds"       + suffix_);
    iEvent.put(v_evtIds      , prefix_ + "evtIds"       + suffix_);
    iEvent.put(v_tpIds       , prefix_ + "tpIds"        + suffix_);
    iEvent.put(v_cfPositions , prefix_ + "cfPositions"  + suffix_);
    iEvent.put(v_tofBins     , prefix_ + "tofBins"      + suffix_);
    iEvent.put(v_fractions   , prefix_ + "fractions"    + suffix_);
    iEvent.put(v_size        , prefix_ + "size"         + suffix_);
}
