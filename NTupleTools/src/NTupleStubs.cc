#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleStubs.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/L1TrackTrigger/interface/TTCluster.h"
#include "DataFormats/L1TrackTrigger/interface/TTStub.h"
#include "DataFormats/L1TrackTrigger/interface/TTTrack.h"
#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiPixelDetId/interface/StackedTrackerDetId.h"
#include "SimTracker/TrackTriggerAssociation/interface/TTClusterAssociationMap.h"
#include "SimTracker/TrackTriggerAssociation/interface/TTStubAssociationMap.h"
#include "SimTracker/TrackTriggerAssociation/interface/TTTrackAssociationMap.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertex.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertexContainer.h"
//#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
//#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"


NTupleStubs::NTupleStubs(const edm::ParameterSet& iConfig) :
  inputTagClus_        (iConfig.getParameter<edm::InputTag>("inputTagClus")),
  inputTagStub_        (iConfig.getParameter<edm::InputTag>("inputTagStub")),
  inputTagTrack_       (iConfig.getParameter<edm::InputTag>("inputTagTrack")),
  inputTagClusMCAssoc_ (iConfig.getParameter<edm::InputTag>("inputTagClusMCAssoc")),
  inputTagStubMCAssoc_ (iConfig.getParameter<edm::InputTag>("inputTagStubMCAssoc")),
  inputTagTrackMCAssoc_(iConfig.getParameter<edm::InputTag>("inputTagTrackMCAssoc")),
  inputTagTrkPart_     (iConfig.getParameter<edm::InputTag>("inputTagTrkPart")),
  inputTagTrkVertex_   (iConfig.getParameter<edm::InputTag>("inputTagTrkVertex")),
  inputTagSimTrack_    (iConfig.getParameter<edm::InputTag>("inputTagSimTrack")),
  inputTagSimVertex_   (iConfig.getParameter<edm::InputTag>("inputTagSimVertex")),
  inputTagSimPixelDigi_(iConfig.getParameter<edm::InputTag>("inputTagSimPixelDigi")),
  beamSpotTag_         (iConfig.getParameter<edm::InputTag>("beamSpotTag")),
  prefixClus_          (iConfig.getParameter<std::string>("prefixClus")),
  prefixStub_          (iConfig.getParameter<std::string>("prefixStub")),
  prefixTrack_         (iConfig.getParameter<std::string>("prefixTrack")),
  suffix_              (iConfig.getParameter<std::string>("suffix")),
  maxN_                (iConfig.getParameter<unsigned>("maxN")) {

    prefixTrkPart_ = "trkParts@";
    prefixSimTrack_ = "simTracks@";
    prefixSimVertex_ = "simVertices@";

    produces<std::vector<float> > (prefixStub_ + "px"    + suffix_);
    produces<std::vector<float> > (prefixStub_ + "py"    + suffix_);
    produces<std::vector<float> > (prefixStub_ + "pz"    + suffix_);
    produces<std::vector<float> > (prefixStub_ + "E"     + suffix_);
    produces<std::vector<float> > (prefixStub_ + "pt"    + suffix_);
    produces<std::vector<float> > (prefixStub_ + "eta"   + suffix_);
    produces<std::vector<float> > (prefixStub_ + "phi"   + suffix_);
    produces<std::vector<float> > (prefixStub_ + "M"     + suffix_);
    produces<unsigned>            (prefixStub_ + "size"  + suffix_);

    produces<std::vector<float> > (prefixTrack_ + "px"   + suffix_);
    produces<std::vector<float> > (prefixTrack_ + "py"   + suffix_);
    produces<std::vector<float> > (prefixTrack_ + "pz"   + suffix_);
    produces<std::vector<float> > (prefixTrack_ + "pt"   + suffix_);
    produces<std::vector<float> > (prefixTrack_ + "eta"  + suffix_);
    produces<std::vector<float> > (prefixTrack_ + "phi"  + suffix_);
    produces<std::vector<float> > (prefixTrack_ + "vx"   + suffix_);
    produces<std::vector<float> > (prefixTrack_ + "vy"   + suffix_);
    produces<std::vector<float> > (prefixTrack_ + "vz"   + suffix_);
    produces<std::vector<float> > (prefixTrack_ + "rinv" + suffix_);
    produces<std::vector<float> > (prefixTrack_ + "chi2" + suffix_);
    produces<std::vector<float> > (prefixTrack_ + "ptconsistency" + suffix_);
    produces<std::vector<unsigned> > (prefixTrack_ + "nstubs" + suffix_);
    produces<std::vector<unsigned> > (prefixTrack_ + "tpId" + suffix_);
    produces<std::vector<bool> >  (prefixTrack_ + "isGenuine" + suffix_);
    produces<std::vector<bool> >  (prefixTrack_ + "isUnknown" + suffix_);
    produces<std::vector<bool> >  (prefixTrack_ + "isCombinatoric" + suffix_);
    produces<unsigned>            (prefixTrack_ + "size" + suffix_);

    produces<std::vector<float> > (prefixTrkPart_ + "px"   + suffix_);
    produces<std::vector<float> > (prefixTrkPart_ + "py"   + suffix_);
    produces<std::vector<float> > (prefixTrkPart_ + "pz"   + suffix_);
    produces<std::vector<float> > (prefixTrkPart_ + "pt"   + suffix_);
    produces<std::vector<float> > (prefixTrkPart_ + "eta"  + suffix_);
    produces<std::vector<float> > (prefixTrkPart_ + "phi"  + suffix_);
    produces<std::vector<float> > (prefixTrkPart_ + "vx"   + suffix_);
    produces<std::vector<float> > (prefixTrkPart_ + "vy"   + suffix_);
    produces<std::vector<float> > (prefixTrkPart_ + "vz"   + suffix_);
    produces<std::vector<int> >   (prefixTrkPart_ + "pdgId" + suffix_);
    produces<std::vector<int> >   (prefixTrkPart_ + "genpId" + suffix_);
    produces<std::vector<int> >   (prefixTrkPart_ + "nhits" + suffix_);
    produces<std::vector<int> >   (prefixTrkPart_ + "ntkhits" + suffix_);
    produces<std::vector<int> >   (prefixTrkPart_ + "ntklayers" + suffix_);
    produces<std::vector<unsigned> > (prefixTrkPart_ + "evtId" + suffix_);
    produces<std::vector<std::vector<unsigned> > > (prefixTrkPart_ + "trkIds" + suffix_);
    produces<unsigned>            (prefixTrkPart_ + "size" + suffix_);

    produces<std::vector<float> > (prefixSimTrack_ + "px"   + suffix_);
    produces<std::vector<float> > (prefixSimTrack_ + "py"   + suffix_);
    produces<std::vector<float> > (prefixSimTrack_ + "pz"   + suffix_);
    produces<std::vector<float> > (prefixSimTrack_ + "pt"   + suffix_);
    produces<std::vector<float> > (prefixSimTrack_ + "eta"  + suffix_);
    produces<std::vector<float> > (prefixSimTrack_ + "phi"  + suffix_);
    produces<std::vector<int> >   (prefixSimTrack_ + "pdgId" + suffix_);
    produces<std::vector<unsigned> > (prefixSimTrack_ + "trkId" + suffix_);
    produces<std::vector<unsigned> > (prefixSimTrack_ + "evtId" + suffix_);
    produces<std::vector<int> >   (prefixSimTrack_ + "vtxId" + suffix_);
    produces<std::vector<int> >   (prefixSimTrack_ + "genpId" + suffix_);
    produces<unsigned>            (prefixSimTrack_ + "size" + suffix_);

    produces<std::vector<float> > (prefixSimVertex_ + "vx"   + suffix_);
    produces<std::vector<float> > (prefixSimVertex_ + "vy"   + suffix_);
    produces<std::vector<float> > (prefixSimVertex_ + "vz"   + suffix_);
    produces<std::vector<float> > (prefixSimVertex_ + "tof"  + suffix_);
    produces<std::vector<int> >   (prefixSimVertex_ + "vtxId" + suffix_);
    produces<unsigned>            (prefixSimVertex_ + "size" + suffix_);

}

void NTupleStubs::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {
    /// Geometry setup
    /// Set pointers to Geometry
    edm::ESHandle<TrackerGeometry> geometryHandle;
    iSetup.get<TrackerDigiGeometryRecord>().get(geometryHandle);
    theGeometry = geometryHandle.product();

    /// Set pointers to Stacked Modules
    edm::ESHandle<StackedTrackerGeometry> stackedGeometryHandle;
    iSetup.get<StackedTrackerGeometryRecord>().get(stackedGeometryHandle);
    theStackedGeometry = stackedGeometryHandle.product();

    /// Magnetic field setup
    edm::ESHandle<MagneticField> magneticFieldHandle;
    iSetup.get<IdealMagneticFieldRecord>().get(magneticFieldHandle);
    theMagneticField = magneticFieldHandle.product();

    /// Make sure uint32_t == unsigned int
    unsigned checkunsigned = 4294967295;
    uint32_t checkuint32_t = 4294967295;
    assert(checkunsigned == checkuint32_t);
}

void NTupleStubs::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<std::vector<float> > v_px     (new std::vector<float>());
    std::auto_ptr<std::vector<float> > v_py     (new std::vector<float>());
    std::auto_ptr<std::vector<float> > v_pz     (new std::vector<float>());
    std::auto_ptr<std::vector<float> > v_E      (new std::vector<float>());
    std::auto_ptr<std::vector<float> > v_pt     (new std::vector<float>());
    std::auto_ptr<std::vector<float> > v_eta    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > v_phi    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > v_M      (new std::vector<float>());
    std::auto_ptr<unsigned>            v_size   (new unsigned(0));

    std::auto_ptr<std::vector<float> > vt_px    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vt_py    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vt_pz    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vt_pt    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vt_eta   (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vt_phi   (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vt_vx    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vt_vy    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vt_vz    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vt_rinv  (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vt_chi2  (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vt_ptconsistency(new std::vector<float>());
    std::auto_ptr<std::vector<unsigned> > vt_nstubs(new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> > vt_tpId  (new std::vector<unsigned>());
    std::auto_ptr<std::vector<bool> >  vt_isGenuine(new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >  vt_isUnknown(new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >  vt_isCombinatoric(new std::vector<bool>());
    std::auto_ptr<unsigned>            vt_size  (new unsigned(0));

    std::auto_ptr<std::vector<float> > vp_px    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vp_py    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vp_pz    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vp_pt    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vp_eta   (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vp_phi   (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vp_vx    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vp_vy    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vp_vz    (new std::vector<float>());
    std::auto_ptr<std::vector<int> >   vp_pdgId (new std::vector<int>());
    std::auto_ptr<std::vector<int> >   vp_genpId(new std::vector<int>());
    std::auto_ptr<std::vector<int> >   vp_nhits (new std::vector<int>());
    std::auto_ptr<std::vector<int> >   vp_ntkhits(new std::vector<int>());
    std::auto_ptr<std::vector<int> >   vp_ntklayers(new std::vector<int>());
    std::auto_ptr<std::vector<unsigned> > vp_evtId(new std::vector<unsigned>());
    std::auto_ptr<std::vector<std::vector<unsigned> > > vp_trkIds(new std::vector<std::vector<unsigned> >());
    std::auto_ptr<unsigned>            vp_size  (new unsigned(0));

    std::auto_ptr<std::vector<float> > vs_px    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vs_py    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vs_pz    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vs_pt    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vs_eta   (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vs_phi   (new std::vector<float>());
    std::auto_ptr<std::vector<int> >   vs_pdgId (new std::vector<int>());
    std::auto_ptr<std::vector<unsigned> > vs_trkId (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> > vs_evtId (new std::vector<unsigned>());
    std::auto_ptr<std::vector<int> >   vs_vtxId (new std::vector<int>());
    std::auto_ptr<std::vector<int> >   vs_genpId(new std::vector<int>());
    std::auto_ptr<unsigned>            vs_size  (new unsigned(0));

    std::auto_ptr<std::vector<float> > vv_vx    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vv_vy    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vv_vz    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vv_tof   (new std::vector<float>());
    std::auto_ptr<std::vector<int> >   vv_vtxId (new std::vector<int>());
    std::auto_ptr<unsigned>            vv_size  (new unsigned(0));


    //__________________________________________________________________________
    /// Get all the handles!

    /// TTCluster, TTStub, TTTrack
    edm::Handle<edmNew::DetSetVector<TTCluster<Ref_PixelDigi_> > > pixelDigiTTClusters;
    iEvent.getByLabel(inputTagClus_, pixelDigiTTClusters);
    edm::Handle<edmNew::DetSetVector<TTStub<Ref_PixelDigi_> > > pixelDigiTTStubs;
    iEvent.getByLabel(inputTagStub_, pixelDigiTTStubs);
    edm::Handle<std::vector<TTTrack<Ref_PixelDigi_> > > pixelDigiTTTracks;
    iEvent.getByLabel(inputTagTrack_, pixelDigiTTTracks);

    /// MC truth association maps
    edm::Handle<TTClusterAssociationMap<Ref_PixelDigi_> > mcAssocTTClusters;
    iEvent.getByLabel(inputTagClusMCAssoc_, mcAssocTTClusters);
    edm::Handle<TTStubAssociationMap<Ref_PixelDigi_> >    mcAssocTTStubs;
    iEvent.getByLabel(inputTagStubMCAssoc_, mcAssocTTStubs);
    edm::Handle<TTTrackAssociationMap<Ref_PixelDigi_> >   mcAssocTTTracks;
    iEvent.getByLabel(inputTagTrackMCAssoc_, mcAssocTTTracks);

    /// Tracking particles, tracking vertices
    edm::Handle<TrackingParticleCollection> trackingParticles;
    iEvent.getByLabel(inputTagTrkPart_, trackingParticles);
    edm::Handle<TrackingVertexCollection> trackingVertices;
    iEvent.getByLabel(inputTagTrkVertex_, trackingVertices);

    /// SimHits
    edm::Handle<edm::SimTrackContainer> simTracks;
    iEvent.getByLabel(inputTagSimTrack_, simTracks);
    edm::Handle<edm::SimVertexContainer> simVertices;
    iEvent.getByLabel(inputTagSimVertex_, simVertices);

    /// SimPixelDigis
    edm::Handle<edm::DetSetVector<PixelDigi> > simPixelDigis;
    iEvent.getByLabel(inputTagSimPixelDigi_, simPixelDigis);
    edm::Handle<edm::DetSetVector<PixelDigiSimLink> >  simPixelDigiSimLinks;
    iEvent.getByLabel(inputTagSimPixelDigi_, simPixelDigiSimLinks);

    /// Beamspot
    edm::Handle<reco::BeamSpot> beamSpot;
    iEvent.getByLabel(beamSpotTag_, beamSpot);
    math::XYZPoint bsPosition(0,0,0);
    if (beamSpot.isValid()) {
        bsPosition = beamSpot->position();
    }

    //__________________________________________________________________________
    /// TTClusters
    if (pixelDigiTTClusters.isValid() && mcAssocTTClusters.isValid()) {
        edm::LogInfo("NTupleClusters") << "Size: " << pixelDigiTTClusters->size();

        typedef typename edmNew::DetSetVector<TTCluster<Ref_PixelDigi_> >::const_iterator const_dsv_iter;
        typedef typename edmNew::DetSet      <TTCluster<Ref_PixelDigi_> >::const_iterator const_ds_iter;
        typedef edm::Ref<edmNew::DetSetVector<TTCluster<Ref_PixelDigi_> >, TTCluster<Ref_PixelDigi_> > reference;
        unsigned n = 0;

        for (const_dsv_iter itv = pixelDigiTTClusters->begin(); itv != pixelDigiTTClusters->end(); ++itv) {
            unsigned dsv_detId = itv->detId();
            unsigned dsv_size = itv->size();
            edm::LogError("NTupleClusters") << "dsv_detId: " << dsv_detId << " dsv_size: " << dsv_size;

            for (const_ds_iter it = itv->begin(); it != itv->end(); ++it) {
                StackedTrackerDetId detId = it->getDetId();
                uint32_t rawId = detId.rawId();
                unsigned iLayer = detId.iLayer();
                unsigned iRing = detId.iRing();
                unsigned iSide = detId.iSide();
                if (iLayer == 999999 && iRing == 999999)
                    continue;
                unsigned iStack = (iLayer != 999999) ? iLayer : 1000 + iRing;
                unsigned iPhi = detId.iPhi();
                unsigned iZ = detId.iZ();
                edm::LogError("NTupleClusters") << "rawId: " << rawId << " iLayer: " << iLayer << " iRing: " << iRing << " iSide: " << iSide << " iStack: " << iStack << " iPhi: " << iPhi << " iZ: " << iZ;

                LocalPoint  localposition = theStackedGeometry->findAverageLocalPosition(&(*it));
                GlobalPoint position = theStackedGeometry->findAverageGlobalPosition(&(*it));
                edm::LogInfo("NTupleClusters") << "localposition: " << localposition << " position: " << position;

                unsigned stackMember = it->getStackMember();
                unsigned width = it->findWidth();
                const std::vector<int>& rows = it->getRows();
                const std::vector<int>& cols = it->getCols();
                MeasurementPoint localcoord = it->findAverageLocalCoordinates();
                edm::LogError("NTupleClusters") << "stackmember: " << stackMember << " width: " << width << " localcoord: " << localcoord;


                /// Make the reference to be put in the map
                reference tempCluRef = edmNew::makeRefTo(pixelDigiTTClusters, it);
                const DetId aDetId = theStackedGeometry->idToDet(tempCluRef->getDetId(), tempCluRef->getStackMember() )->geographicalId();
                edm::LogError("NTupleClusters") << "detId: " << detId.rawId() << " aDetId: " << aDetId.rawId();



            }
        }
    } else {
        edm::LogError("NTupleClusters") << "Cannot get the product: " << inputTagClus_;
    }

    /// TTStubs
    if (pixelDigiTTStubs.isValid() && mcAssocTTStubs.isValid()) {
        edm::LogInfo("NTupleStubs") << "Size: " << pixelDigiTTStubs->size();

        typedef typename edmNew::DetSetVector<TTStub<Ref_PixelDigi_> >::const_iterator const_dsv_iter;
        typedef typename edmNew::DetSet      <TTStub<Ref_PixelDigi_> >::const_iterator const_ds_iter;
        unsigned n = 0;

        for (const_dsv_iter itv = pixelDigiTTStubs->begin(); itv != pixelDigiTTStubs->end(); ++itv) {
            unsigned dsv_detId = itv->detId();
            unsigned dsv_size = itv->size();
            edm::LogError("NTupleStubs") << "dsv_detId: " << dsv_detId << " dsv_size: " << dsv_size;

            for (const_ds_iter it = itv->begin(); it != itv->end(); ++it) {
                StackedTrackerDetId detId = it->getDetId();
                uint32_t rawId = detId.rawId();
                unsigned iLayer = detId.iLayer();
                unsigned iRing = detId.iRing();
                unsigned iSide = detId.iSide();
                if (iLayer == 999999 && iRing == 999999)
                    continue;
                unsigned iStack = (iLayer != 999999) ? iLayer : 1000 + iRing;
                unsigned iPhi = detId.iPhi();
                unsigned iZ = detId.iZ();
                edm::LogInfo("NTupleStubs") << "rawId: " << rawId << " iLayer: " << iLayer << " iRing: " << iRing << " iSide: " << iSide << " iStack: " << iStack << " iPhi: " << iPhi << " iZ: " << iZ;

                double magfieldStrength = theMagneticField->inTesla(GlobalPoint(0,0,0)).z();
                double       roughpt   = theStackedGeometry->findRoughPt(magfieldStrength, &(*it));
                GlobalPoint  position  = theStackedGeometry->findGlobalPosition(&(*it));
                GlobalVector direction = theStackedGeometry->findGlobalDirection(&(*it));
                edm::LogInfo("NTupleStubs") << "roughpt: " << roughpt << " position: " << position << " direction: " << direction;

                // in full-strip units
                double trigDist   = it->getTriggerDisplacement();
                double trigOffset = it->getTriggerOffset();
                double trigPos    = it->getTriggerPosition();
                double trigBend   = it->getTriggerBend();
                edm::LogInfo("NTupleStubs") << "trigDist: " << trigDist << " trigOffset: " << trigOffset << " trigPos: " << trigPos << " trigBend: " << trigBend;


                // get cluster info
                //std::vector< edm::Ref< edmNew::DetSetVector< TTCluster< T > >, TTCluster< T > > > getClusterRefs()
                //const DetId innerDetId = theStackedGeometry->idToDet( innerCluster->getDetId(), 0 )->geographicalId();
                //const DetId outerDetId = theStackedGeometry->idToDet( outerCluster->getDetId(), 1 )->geographicalId();
                //GlobalPoint avgposition = theStackedGeometry->findAverageGlobalPosition( &(*it) );
                //LocalPoint coords = iter->findAverageLocalCoordinates();

                // get MC info
                //bool genuineStub    = MCTruthTTStubHandle->isGenuine( tempStubPtr );

                //innerStack
                //outerStack


            }



            // Fill the vectors
            //v_px    ->push_back(it->px());
            //v_py    ->push_back(it->py());
            //v_pz    ->push_back(it->pz());
            //v_E     ->push_back(it->energy());
            //v_pt    ->push_back(it->pt());
            //v_eta   ->push_back(it->eta());
            //v_phi   ->push_back(it->phi());
            //v_M     ->push_back(it->mass());
        }
        *v_size = v_px->size();

    } else {
        edm::LogError("NTupleStubs") << "Cannot get the product: " << inputTagStub_;
    }

    /// TTTracks
    if (pixelDigiTTTracks.isValid() && mcAssocTTTracks.isValid()) {
        edm::LogInfo("NTupleTracks") << "Size: " << pixelDigiTTTracks->size();

        typedef typename std::vector<TTTrack<Ref_PixelDigi_> >::const_iterator const_iterator;
        typedef edm::Ptr<TTTrack<Ref_PixelDigi_> > pointer;
        unsigned n = 0;

        for (const_iterator it = pixelDigiTTTracks->begin(); it != pixelDigiTTTracks->end(); ++it) {
            pointer iptr(pixelDigiTTTracks, it - pixelDigiTTTracks->begin());

            GlobalVector momentum = it->getMomentum();
            GlobalPoint  poca = it->getPOCA();  // point of closest approach
            vt_px->push_back(momentum.x());
            vt_py->push_back(momentum.y());
            vt_pz->push_back(momentum.z());
            vt_pt->push_back(momentum.perp());
            vt_eta->push_back(momentum.eta());
            vt_phi->push_back(momentum.phi());
            vt_vx->push_back(poca.x());
            vt_vy->push_back(poca.y());
            vt_vz->push_back(poca.z());
            vt_rinv->push_back(it->getRInv());
            vt_chi2->push_back(it->getChi2());
            vt_ptconsistency->push_back(it->getStubPtConsistency());
            vt_nstubs->push_back(it->getStubRefs().size());

            edm::Ptr<TrackingParticle> tpptr = mcAssocTTTracks->findTrackingParticlePtr(iptr);
            vt_tpId->push_back(tpptr.key());
            vt_isGenuine->push_back(mcAssocTTTracks->isGenuine(iptr));
            vt_isUnknown->push_back(mcAssocTTTracks->isUnknown(iptr));
            vt_isCombinatoric->push_back(mcAssocTTTracks->isCombinatoric(iptr));

            n++;
        }
        *vt_size = vt_px->size();

    } else {
        edm::LogError("NTupleTracks") << "Cannot get the product: " << inputTagTrack_;
    }

    /// Tracking particles, tracking vertices
    if (trackingParticles.isValid() && trackingVertices.isValid()) {
        edm::LogInfo("NTupleTrkPart") << "Size: " << trackingParticles->size();

        unsigned n = 0;
        for (TrackingParticleCollection::const_iterator it = trackingParticles->begin(); it != trackingParticles->end(); ++it) {
            vp_px->push_back(it->px()); // first simTrack
            vp_py->push_back(it->py());
            vp_pz->push_back(it->pz());
            vp_pt->push_back(it->pt());
            vp_eta->push_back(it->eta());
            vp_phi->push_back(it->phi());
            vp_vx->push_back(it->vx() - bsPosition.x()); // parent vertex
            vp_vy->push_back(it->vy() - bsPosition.y());
            vp_vz->push_back(it->vz() - bsPosition.z());
            vp_pdgId->push_back(it->pdgId());
            int genpId = it->genParticles().empty() ? -99 : it->genParticles().begin()->key();
            vp_genpId->push_back(genpId);
            vp_nhits->push_back(it->numberOfHits());
            vp_ntkhits->push_back(it->numberOfTrackerHits());
            vp_ntklayers->push_back(it->numberOfTrackerLayers());
            vp_evtId->push_back(it->eventId().rawId());

            std::vector<unsigned> trkIds;  // simTrackId (a.k.a. g4TrackId)
            for (TrackingParticle::g4t_iterator itsim = it->g4Track_begin(); itsim != it->g4Track_end(); ++itsim) {
                trkIds.push_back(itsim->trackId());
            }
            vp_trkIds->push_back(trkIds);

            n++;
        }
        *vp_size = vp_px->size();

    } else {
        edm::LogError("NTupleTrkPart") << "Cannot get the product: " << inputTagTrkPart_;
    }

    /// SimPixelDigi
    if (simPixelDigis.isValid() && simPixelDigiSimLinks.isValid()) {
        edm::LogInfo("NTupleSimPixelDigis") << "Size: " << simPixelDigis->size();

        typedef edm::DetSetVector<PixelDigi>::const_iterator const_iterator;
        unsigned n = 0;
        for (const_iterator it = simPixelDigis->begin(); it != simPixelDigis->end(); ++it) {
            uint32_t            rawId = it->detId();
            StackedTrackerDetId detId(rawId);
            DetId::Detector     det = detId.det();  // Tracker=1,Muon=2,Ecal=3,Hcal=4,Calo=5,Forward=6
            int                 subdetId = detId.subdetId();  // PXB=1,PXF=2,...

            n++;
        }

    } else {
        edm::LogError("NTupleSimPixelDigis") << "Cannot get the product: " << inputTagSimPixelDigi_;
    }

    /// SimTracks
    if (simTracks.isValid()) {
        edm::LogInfo("NTupleSimTrack") << "Size: " << simTracks->size();

        unsigned n = 0;
        for (edm::SimTrackContainer::const_iterator it = simTracks->begin(); it != simTracks->end(); ++it) {
            const math::XYZTLorentzVectorD& momentum = it->momentum();
            vs_px->push_back(momentum.px());
            vs_py->push_back(momentum.py());
            vs_pz->push_back(momentum.pz());
            vs_pt->push_back(momentum.pt());
            vs_eta->push_back(momentum.eta());
            vs_phi->push_back(momentum.phi());
            vs_pdgId->push_back(it->type());
            vs_trkId->push_back(it->trackId());
            vs_evtId->push_back(it->eventId().rawId());
            vs_vtxId->push_back(it->vertIndex());
            vs_genpId->push_back(it->genpartIndex());

            n++;
        }
        *vs_size = vs_px->size();
    } else {
        edm::LogError("NTupleSimTrack") << "Cannot get the product: " << inputTagSimTrack_;
    }

    /// SimVertices
    if (simVertices.isValid()) {
        edm::LogInfo("NTupleSimVertex") << "Size: " << simVertices->size();

        unsigned n = 0;
        for (edm::SimVertexContainer::const_iterator it = simVertices->begin(); it != simVertices->end(); ++it) {
            const math::XYZTLorentzVectorD& position = it->position();
            vv_vx->push_back(position.x() - bsPosition.x());
            vv_vy->push_back(position.y() - bsPosition.y());
            vv_vz->push_back(position.z() - bsPosition.z());
            vv_tof->push_back(position.t());
            vv_vtxId->push_back(it->vertexId());

            n++;
        }
        *vv_size = vv_vx->size();
    } else {
        edm::LogError("NTupleSimVertex") << "Cannot get the product: " << inputTagSimVertex_;
    }


    //__________________________________________________________________________
    iEvent.put(v_px     , prefixStub_ + "px"    + suffix_);
    iEvent.put(v_py     , prefixStub_ + "py"    + suffix_);
    iEvent.put(v_pz     , prefixStub_ + "pz"    + suffix_);
    iEvent.put(v_E      , prefixStub_ + "E"     + suffix_);
    iEvent.put(v_pt     , prefixStub_ + "pt"    + suffix_);
    iEvent.put(v_eta    , prefixStub_ + "eta"   + suffix_);
    iEvent.put(v_phi    , prefixStub_ + "phi"   + suffix_);
    iEvent.put(v_M      , prefixStub_ + "M"     + suffix_);
    iEvent.put(v_size   , prefixStub_ + "size"  + suffix_);

    iEvent.put(vt_px    , prefixTrack_ + "px"   + suffix_);
    iEvent.put(vt_py    , prefixTrack_ + "py"   + suffix_);
    iEvent.put(vt_pz    , prefixTrack_ + "pz"   + suffix_);
    iEvent.put(vt_pt    , prefixTrack_ + "pt"   + suffix_);
    iEvent.put(vt_eta   , prefixTrack_ + "eta"  + suffix_);
    iEvent.put(vt_phi   , prefixTrack_ + "phi"  + suffix_);
    iEvent.put(vt_vx    , prefixTrack_ + "vx"   + suffix_);
    iEvent.put(vt_vy    , prefixTrack_ + "vy"   + suffix_);
    iEvent.put(vt_vz    , prefixTrack_ + "vz"   + suffix_);
    iEvent.put(vt_rinv  , prefixTrack_ + "rinv" + suffix_);
    iEvent.put(vt_chi2  , prefixTrack_ + "chi2" + suffix_);
    iEvent.put(vt_ptconsistency, prefixTrack_ + "ptconsistency" + suffix_);
    iEvent.put(vt_nstubs, prefixTrack_ + "nstubs" + suffix_);
    iEvent.put(vt_tpId  , prefixTrack_ + "tpId" + suffix_);
    iEvent.put(vt_isGenuine, prefixTrack_ + "isGenuine" + suffix_);
    iEvent.put(vt_isUnknown, prefixTrack_ + "isUnknown" + suffix_);
    iEvent.put(vt_isCombinatoric, prefixTrack_ + "isCombinatoric" + suffix_);
    iEvent.put(vt_size  , prefixTrack_ + "size" + suffix_);

    iEvent.put(vp_px    , prefixTrkPart_ + "px"   + suffix_);
    iEvent.put(vp_py    , prefixTrkPart_ + "py"   + suffix_);
    iEvent.put(vp_pz    , prefixTrkPart_ + "pz"   + suffix_);
    iEvent.put(vp_pt    , prefixTrkPart_ + "pt"   + suffix_);
    iEvent.put(vp_eta   , prefixTrkPart_ + "eta"  + suffix_);
    iEvent.put(vp_phi   , prefixTrkPart_ + "phi"  + suffix_);
    iEvent.put(vp_vx    , prefixTrkPart_ + "vx"   + suffix_);
    iEvent.put(vp_vy    , prefixTrkPart_ + "vy"   + suffix_);
    iEvent.put(vp_vz    , prefixTrkPart_ + "vz"   + suffix_);
    iEvent.put(vp_pdgId , prefixTrkPart_ + "pdgId" + suffix_);
    iEvent.put(vp_genpId, prefixTrkPart_ + "genpId" + suffix_);
    iEvent.put(vp_nhits , prefixTrkPart_ + "nhits" + suffix_);
    iEvent.put(vp_ntkhits, prefixTrkPart_ + "ntkhits" + suffix_);
    iEvent.put(vp_ntklayers, prefixTrkPart_ + "ntklayers" + suffix_);
    iEvent.put(vp_evtId , prefixTrkPart_ + "evtId" + suffix_);
    iEvent.put(vp_trkIds, prefixTrkPart_ + "trkIds" + suffix_);
    iEvent.put(vp_size  , prefixTrkPart_ + "size" + suffix_);

    iEvent.put(vs_px    , prefixSimTrack_ + "px"   + suffix_);
    iEvent.put(vs_py    , prefixSimTrack_ + "py"   + suffix_);
    iEvent.put(vs_pz    , prefixSimTrack_ + "pz"   + suffix_);
    iEvent.put(vs_pt    , prefixSimTrack_ + "pt"   + suffix_);
    iEvent.put(vs_eta   , prefixSimTrack_ + "eta"  + suffix_);
    iEvent.put(vs_phi   , prefixSimTrack_ + "phi"  + suffix_);
    iEvent.put(vs_pdgId , prefixSimTrack_ + "pdgId" + suffix_);
    iEvent.put(vs_trkId , prefixSimTrack_ + "trkId" + suffix_);
    iEvent.put(vs_evtId , prefixSimTrack_ + "evtId" + suffix_);
    iEvent.put(vs_vtxId , prefixSimTrack_ + "vtxId" + suffix_);
    iEvent.put(vs_genpId, prefixSimTrack_ + "genpId" + suffix_);
    iEvent.put(vs_size  , prefixSimTrack_ + "size" + suffix_);

    iEvent.put(vv_vx    , prefixSimVertex_ + "vx"   + suffix_);
    iEvent.put(vv_vy    , prefixSimVertex_ + "vy"   + suffix_);
    iEvent.put(vv_vz    , prefixSimVertex_ + "vz"   + suffix_);
    iEvent.put(vv_tof   , prefixSimVertex_ + "tof"  + suffix_);
    iEvent.put(vv_vtxId , prefixSimVertex_ + "vtxId" + suffix_);
    iEvent.put(vv_size  , prefixSimVertex_ + "size" + suffix_);

}
