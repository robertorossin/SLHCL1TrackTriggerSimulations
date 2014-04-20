#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleStubs.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/L1TrackTrigger/interface/TTCluster.h"
#include "DataFormats/L1TrackTrigger/interface/TTStub.h"
#include "DataFormats/L1TrackTrigger/interface/TTTrack.h"
#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"
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
  maxN_                (iConfig.getParameter<unsigned int>("maxN")) {

    produces<std::vector<float> > (prefixStub_ + "px"    + suffix_);
    produces<std::vector<float> > (prefixStub_ + "py"    + suffix_);
    produces<std::vector<float> > (prefixStub_ + "pz"    + suffix_);
    produces<std::vector<float> > (prefixStub_ + "E"     + suffix_);
    produces<std::vector<float> > (prefixStub_ + "pt"    + suffix_);
    produces<std::vector<float> > (prefixStub_ + "eta"   + suffix_);
    produces<std::vector<float> > (prefixStub_ + "phi"   + suffix_);
    produces<std::vector<float> > (prefixStub_ + "M"     + suffix_);
    produces<unsigned int>        (prefixStub_ + "size"  + suffix_);

    produces<std::vector<float> > (prefixTrack_ + "px"   + suffix_);
    produces<std::vector<float> > (prefixTrack_ + "py"   + suffix_);
    produces<std::vector<float> > (prefixTrack_ + "pz"   + suffix_);
    produces<std::vector<float> > (prefixTrack_ + "pt"   + suffix_);
    produces<std::vector<float> > (prefixTrack_ + "eta"  + suffix_);
    produces<std::vector<float> > (prefixTrack_ + "phi"  + suffix_);
    produces<std::vector<float> > (prefixTrack_ + "z0"   + suffix_);
    produces<std::vector<float> > (prefixTrack_ + "rinv" + suffix_);
    produces<std::vector<float> > (prefixTrack_ + "chi2" + suffix_);
    produces<std::vector<float> > (prefixTrack_ + "ptconsistency" + suffix_);
    produces<std::vector<unsigned int> > (prefixTrack_ + "nstubs" + suffix_);
    produces<std::vector<bool> >  (prefixTrack_ + "isGenuine" + suffix_);
    produces<std::vector<bool> >  (prefixTrack_ + "isUnknown" + suffix_);
    produces<std::vector<bool> >  (prefixTrack_ + "isCombinatoric" + suffix_);
    produces<unsigned int>        (prefixTrack_ + "size" + suffix_);

}

void NTupleStubs::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    /// Geometry setup
    /// Set pointers to Geometry
    iSetup.get<TrackerDigiGeometryRecord>().get(geometryHandle);
    const TrackerGeometry * theGeometry = geometryHandle.product();
    /// Set pointers to Stacked Modules
    iSetup.get<StackedTrackerGeometryRecord>().get(stackedGeometryHandle);
    const StackedTrackerGeometry * theStackedGeometry = stackedGeometryHandle.product();

    /// Magnetic field setup
    iSetup.get<IdealMagneticFieldRecord>().get(magneticFieldHandle);
    const MagneticField* theMagneticField = magneticFieldHandle.product();
    double magfieldStrength = theMagneticField->inTesla(GlobalPoint(0,0,0)).z();

    std::auto_ptr<std::vector<float> > v_px    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > v_py    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > v_pz    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > v_E     (new std::vector<float>());
    std::auto_ptr<std::vector<float> > v_pt    (new std::vector<float>());
    std::auto_ptr<std::vector<float> > v_eta   (new std::vector<float>());
    std::auto_ptr<std::vector<float> > v_phi   (new std::vector<float>());
    std::auto_ptr<std::vector<float> > v_M     (new std::vector<float>());
    std::auto_ptr<unsigned int>        v_size  (new unsigned int(0));

    std::auto_ptr<std::vector<float> > vt_px   (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vt_py   (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vt_pz   (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vt_pt   (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vt_eta  (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vt_phi  (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vt_z0   (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vt_rinv (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vt_chi2 (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vt_ptconsistency(new std::vector<float>());
    std::auto_ptr<std::vector<unsigned int> > vt_nstubs(new std::vector<unsigned int>());
    std::auto_ptr<std::vector<bool> >  vt_isGenuine(new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >  vt_isUnknown(new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >  vt_isCombinatoric(new std::vector<bool>());
    std::auto_ptr<unsigned int>        vt_size (new unsigned int(0));

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
    math::XYZPoint beamSpotPos(0,0,0);
    if (beamSpot.isValid()) {
        beamSpotPos = beamSpot->position();
    }

    //__________________________________________________________________________
    if (pixelDigiTTClusters.isValid() && mcAssocTTClusters.isValid()) {
        edm::LogInfo("NTupleClusters") << "Size: " << pixelDigiTTClusters->size();

        typedef typename edmNew::DetSetVector<TTCluster<Ref_PixelDigi_> >::const_iterator const_dsv_iter;
        typedef typename edmNew::DetSet      <TTCluster<Ref_PixelDigi_> >::const_iterator const_ds_iter;
        unsigned int n = 0;

        for (const_dsv_iter itv = pixelDigiTTClusters->begin(); itv != pixelDigiTTClusters->end(); ++itv) {
            unsigned int dsv_detId = itv->detId();
            unsigned int dsv_size = itv->size();
            edm::LogError("NTupleClusters") << "dsv_detId: " << dsv_detId << " dsv_size: " << dsv_size;

            for (const_ds_iter it = itv->begin(); it != itv->end(); ++it) {
                StackedTrackerDetId detId = it->getDetId();
                uint32_t rawId = detId.rawId();
                unsigned int iLayer = detId.iLayer();
                unsigned int iRing = detId.iRing();
                unsigned int iSide = detId.iSide();
                if (iLayer == 999999 && iRing == 999999)
                    continue;
                unsigned int iStack = (iLayer != 999999) ? iLayer : 1000 + iRing;
                unsigned int iPhi = detId.iPhi();
                unsigned int iZ = detId.iZ();
                edm::LogError("NTupleClusters") << "rawId: " << rawId << " iLayer: " << iLayer << " iRing: " << iRing << " iSide: " << iSide << " iStack: " << iStack << " iPhi: " << iPhi << " iZ: " << iZ;

                LocalPoint  localposition = theStackedGeometry->findAverageLocalPosition(&(*it));
                GlobalPoint position = theStackedGeometry->findAverageGlobalPosition(&(*it));
                edm::LogError("NTupleClusters") << "localposition: " << localposition << " position: " << position;

                unsigned int stackMember = it->getStackMember();
                unsigned int width = it->findWidth();
                //const std::vector<int>& rows = it->getRows();
                //const std::vector<int>& cols = it->getCols();
                MeasurementPoint localcoord = it->findAverageLocalCoordinates();
                edm::LogError("NTupleClusters") << "stackmember: " << stackMember << " width: " << width << " localcoord: " << localcoord;

            }
        }
    } else {
        edm::LogError("NTupleClusters") << "Cannot get the product: " << inputTagClus_;
    }

    if (pixelDigiTTStubs.isValid() && mcAssocTTStubs.isValid()) {
        edm::LogInfo("NTupleStubs") << "Size: " << pixelDigiTTStubs->size();

        typedef typename edmNew::DetSetVector<TTStub<Ref_PixelDigi_> >::const_iterator const_dsv_iter;
        typedef typename edmNew::DetSet      <TTStub<Ref_PixelDigi_> >::const_iterator const_ds_iter;
        unsigned int n = 0;

        for (const_dsv_iter itv = pixelDigiTTStubs->begin(); itv != pixelDigiTTStubs->end(); ++itv) {
            unsigned int dsv_detId = itv->detId();
            unsigned int dsv_size = itv->size();
            edm::LogError("NTupleStubs") << "dsv_detId: " << dsv_detId << " dsv_size: " << dsv_size;

            for (const_ds_iter it = itv->begin(); it != itv->end(); ++it) {
                StackedTrackerDetId detId = it->getDetId();
                uint32_t rawId = detId.rawId();
                unsigned int iLayer = detId.iLayer();
                unsigned int iRing = detId.iRing();
                unsigned int iSide = detId.iSide();
                if (iLayer == 999999 && iRing == 999999)
                    continue;
                unsigned int iStack = (iLayer != 999999) ? iLayer : 1000 + iRing;
                unsigned int iPhi = detId.iPhi();
                unsigned int iZ = detId.iZ();
                edm::LogError("NTupleStubs") << "rawId: " << rawId << " iLayer: " << iLayer << " iRing: " << iRing << " iSide: " << iSide << " iStack: " << iStack << " iPhi: " << iPhi << " iZ: " << iZ;

                double       roughpt   = theStackedGeometry->findRoughPt(magfieldStrength, &(*it));
                GlobalPoint  position  = theStackedGeometry->findGlobalPosition(&(*it));
                GlobalVector direction = theStackedGeometry->findGlobalDirection(&(*it));
                edm::LogError("NTupleStubs") << "roughpt: " << roughpt << " position: " << position << " direction: " << direction;

                // in full-strip units
                double trigDist   = it->getTriggerDisplacement();
                double trigOffset = it->getTriggerOffset();
                double trigPos    = it->getTriggerPosition();
                double trigBend   = it->getTriggerBend();
                edm::LogError("NTupleStubs") << "trigDist: " << trigDist << " trigOffset: " << trigOffset << " trigPos: " << trigPos << " trigBend: " << trigBend;


                // get cluster info
                //std::vector< edm::Ref< edmNew::DetSetVector< TTCluster< T > >, TTCluster< T > > > getClusterRefs()
                //const DetId innerDetId = theStackedGeometry->idToDet( innerCluster->getDetId(), 0 )->geographicalId();
                //const DetId outerDetId = theStackedGeometry->idToDet( outerCluster->getDetId(), 1 )->geographicalId();
                //GlobalPoint avgposition = theStackedGeometry->findAverageGlobalPosition( &(*it) );
                //LocalPoint coords = iter->findAverageLocalCoordinates();

                // get MC info
                //bool genuineStub    = MCTruthTTStubHandle->isGenuine( tempStubPtr );


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

    if (pixelDigiTTTracks.isValid() && mcAssocTTTracks.isValid()) {
        edm::LogInfo("NTupleTracks") << "Size: " << pixelDigiTTTracks->size();

        typedef typename std::vector<TTTrack<Ref_PixelDigi_> >::const_iterator const_iter;
        typedef edm::Ptr<TTTrack<Ref_PixelDigi_> > pointer;
        unsigned int n = 0;

        for (const_iter it = pixelDigiTTTracks->begin(); it != pixelDigiTTTracks->end(); ++it) {
            pointer iptr(pixelDigiTTTracks, it - pixelDigiTTTracks->begin());

            GlobalVector momentum = it->getMomentum();
            GlobalPoint  poca = it->getPOCA();  // point of closest approach
            vt_px->push_back(momentum.x());
            vt_py->push_back(momentum.y());
            vt_pz->push_back(momentum.z());
            vt_pt->push_back(momentum.perp());
            vt_eta->push_back(momentum.eta());
            vt_phi->push_back(momentum.phi());
            vt_z0->push_back(poca.z());
            vt_rinv->push_back(it->getRInv());
            vt_chi2->push_back(it->getChi2());
            vt_ptconsistency->push_back(it->getStubPtConsistency());
            vt_nstubs->push_back(it->getStubRefs().size());

            vt_isGenuine->push_back(mcAssocTTTracks->isGenuine(iptr));
            vt_isUnknown->push_back(mcAssocTTTracks->isUnknown(iptr));
            vt_isCombinatoric->push_back(mcAssocTTTracks->isCombinatoric(iptr));

            n++;
        }
        *vt_size = vt_px->size();
    } else {
        edm::LogError("NTupleTracks") << "Cannot get the product: " << inputTagTrack_;
    }


    //__________________________________________________________________________
    iEvent.put(v_px    , prefixStub_ + "px"    + suffix_);
    iEvent.put(v_py    , prefixStub_ + "py"    + suffix_);
    iEvent.put(v_pz    , prefixStub_ + "pz"    + suffix_);
    iEvent.put(v_E     , prefixStub_ + "E"     + suffix_);
    iEvent.put(v_pt    , prefixStub_ + "pt"    + suffix_);
    iEvent.put(v_eta   , prefixStub_ + "eta"   + suffix_);
    iEvent.put(v_phi   , prefixStub_ + "phi"   + suffix_);
    iEvent.put(v_M     , prefixStub_ + "M"     + suffix_);
    iEvent.put(v_size  , prefixStub_ + "size"  + suffix_);

    iEvent.put(vt_px    , prefixTrack_ + "px"   + suffix_);
    iEvent.put(vt_py    , prefixTrack_ + "py"   + suffix_);
    iEvent.put(vt_pz    , prefixTrack_ + "pz"   + suffix_);
    iEvent.put(vt_pt    , prefixTrack_ + "pt"   + suffix_);
    iEvent.put(vt_eta   , prefixTrack_ + "eta"  + suffix_);
    iEvent.put(vt_phi   , prefixTrack_ + "phi"  + suffix_);
    iEvent.put(vt_z0    , prefixTrack_ + "z0"   + suffix_);
    iEvent.put(vt_rinv  , prefixTrack_ + "rinv" + suffix_);
    iEvent.put(vt_chi2  , prefixTrack_ + "chi2" + suffix_);
    iEvent.put(vt_ptconsistency, prefixTrack_ + "ptconsistency" + suffix_);
    iEvent.put(vt_nstubs, prefixTrack_ + "nstubs" + suffix_);
    iEvent.put(vt_isGenuine, prefixTrack_ + "isGenuine" + suffix_);
    iEvent.put(vt_isUnknown, prefixTrack_ + "isUnknown" + suffix_);
    iEvent.put(vt_isCombinatoric, prefixTrack_ + "isCombinatoric" + suffix_);
    iEvent.put(vt_size  , prefixTrack_ + "size" + suffix_);
}
