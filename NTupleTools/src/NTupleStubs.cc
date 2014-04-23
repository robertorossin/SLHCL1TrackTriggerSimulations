#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleStubs.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/SiPixelDigi/interface/PixelDigi.h"
#include "DataFormats/L1TrackTrigger/interface/TTCluster.h"
#include "DataFormats/L1TrackTrigger/interface/TTStub.h"
#include "DataFormats/L1TrackTrigger/interface/TTTrack.h"
#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiPixelDetId/interface/StackedTrackerDetId.h"
#include "SimTracker/TrackTriggerAssociation/interface/TTClusterAssociationMap.h"
#include "SimTracker/TrackTriggerAssociation/interface/TTStubAssociationMap.h"
#include "SimTracker/TrackTriggerAssociation/interface/TTTrackAssociationMap.h"
//#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
//#include "SimDataFormats/TrackingHit/interface/PSimHit.h"


unsigned findById(const std::vector<unsigned>& vec, unsigned id) {
    std::vector<unsigned>::const_iterator found;
    found = std::find(vec.begin(), vec.end(), id);
    if (found == vec.end())
        throw cms::Exception("LogicError") << "Id not found: " << id << ".\n";
    return (found - vec.begin());
}

NTupleStubs::NTupleStubs(const edm::ParameterSet& iConfig) :
  inputTagClus_        (iConfig.getParameter<edm::InputTag>("inputTagClus")),
  inputTagStub_        (iConfig.getParameter<edm::InputTag>("inputTagStub")),
  inputTagTrack_       (iConfig.getParameter<edm::InputTag>("inputTagTrack")),
  inputTagPixelDigi_   (iConfig.getParameter<edm::InputTag>("inputTagPixelDigi")),
  inputTagClusMCAssoc_ (iConfig.getParameter<edm::InputTag>("inputTagClusMCAssoc")),
  inputTagStubMCAssoc_ (iConfig.getParameter<edm::InputTag>("inputTagStubMCAssoc")),
  inputTagTrackMCAssoc_(iConfig.getParameter<edm::InputTag>("inputTagTrackMCAssoc")),
  prefixClus_          (iConfig.getParameter<std::string>("prefixClus")),
  prefixStub_          (iConfig.getParameter<std::string>("prefixStub")),
  prefixTrack_         (iConfig.getParameter<std::string>("prefixTrack")),
  prefixPixelDigi_     (iConfig.getParameter<std::string>("prefixPixelDigi")),
  suffix_              (iConfig.getParameter<std::string>("suffix")) {

    produces<std::vector<float> >    (prefixClus_ + "x"      + suffix_);
    produces<std::vector<float> >    (prefixClus_ + "y"      + suffix_);
    produces<std::vector<float> >    (prefixClus_ + "z"      + suffix_);
    produces<std::vector<float> >    (prefixClus_ + "localx" + suffix_);
    produces<std::vector<float> >    (prefixClus_ + "localy" + suffix_);
    produces<std::vector<float> >    (prefixClus_ + "localz" + suffix_);
    produces<std::vector<unsigned> > (prefixClus_ + "rawId"  + suffix_);
    produces<std::vector<unsigned> > (prefixClus_ + "ilayer" + suffix_);
    produces<std::vector<unsigned> > (prefixClus_ + "iring"  + suffix_);
    produces<std::vector<unsigned> > (prefixClus_ + "iside"  + suffix_);
    produces<std::vector<unsigned> > (prefixClus_ + "iphi"   + suffix_);
    produces<std::vector<unsigned> > (prefixClus_ + "iz"     + suffix_);
    produces<std::vector<bool> >     (prefixClus_ + "barrel" + suffix_);
    produces<std::vector<unsigned> > (prefixClus_ + "geoId"  + suffix_);
    produces<std::vector<unsigned> > (prefixClus_ + "stack"  + suffix_);
    produces<std::vector<unsigned> > (prefixClus_ + "width"  + suffix_);
    produces<std::vector<std::vector<int> > > (prefixClus_ + "rows"   + suffix_);
    produces<std::vector<std::vector<int> > > (prefixClus_ + "cols"   + suffix_);
    produces<std::vector<std::vector<int> > > (prefixClus_ + "adcs"   + suffix_);
    produces<std::vector<std::vector<int> > > (prefixClus_ + "chans"  + suffix_);
    produces<std::vector<float> >    (prefixClus_ + "coordx" + suffix_);
    produces<std::vector<float> >    (prefixClus_ + "coordy" + suffix_);
    produces<std::vector<bool> >     (prefixClus_ + "isGenuine" + suffix_);
    produces<std::vector<bool> >     (prefixClus_ + "isUnknown" + suffix_);
    produces<std::vector<bool> >     (prefixClus_ + "isCombinatoric" + suffix_);
    produces<std::vector<int> >      (prefixClus_ + "tpId"   + suffix_);
    produces<std::vector<int> >      (prefixClus_ + "pdgId"  + suffix_);
    produces<std::vector<float> >    (prefixClus_ + "simPt"  + suffix_);
    produces<std::vector<float> >    (prefixClus_ + "simEta" + suffix_);
    produces<unsigned>               (prefixClus_ + "size"   + suffix_);

    produces<std::vector<float> >    (prefixStub_ + "x"      + suffix_);
    produces<std::vector<float> >    (prefixStub_ + "y"      + suffix_);
    produces<std::vector<float> >    (prefixStub_ + "z"      + suffix_);
    produces<std::vector<float> >    (prefixStub_ + "dirx"   + suffix_);
    produces<std::vector<float> >    (prefixStub_ + "diry"   + suffix_);
    produces<std::vector<float> >    (prefixStub_ + "dirz"   + suffix_);
    produces<std::vector<float> >    (prefixStub_ + "roughPt"+ suffix_);
    produces<std::vector<unsigned> > (prefixStub_ + "rawId"  + suffix_);
    produces<std::vector<unsigned> > (prefixStub_ + "ilayer" + suffix_);
    produces<std::vector<unsigned> > (prefixStub_ + "iring"  + suffix_);
    produces<std::vector<unsigned> > (prefixStub_ + "iside"  + suffix_);
    produces<std::vector<unsigned> > (prefixStub_ + "iphi"   + suffix_);
    produces<std::vector<unsigned> > (prefixStub_ + "iz"     + suffix_);
    produces<std::vector<bool> >     (prefixStub_ + "barrel" + suffix_);
    produces<std::vector<float> >    (prefixStub_ + "trigDist" + suffix_);
    produces<std::vector<float> >    (prefixStub_ + "trigOffset" + suffix_);
    produces<std::vector<float> >    (prefixStub_ + "trigPos" + suffix_);
    produces<std::vector<float> >    (prefixStub_ + "trigBend" + suffix_);
    produces<std::vector<unsigned> > (prefixStub_ + "geoId0" + suffix_);
    produces<std::vector<unsigned> > (prefixStub_ + "geoId1" + suffix_);
    produces<std::vector<unsigned> > (prefixStub_ + "clusId0" + suffix_);
    produces<std::vector<unsigned> > (prefixStub_ + "clusId1" + suffix_);
    produces<std::vector<int> >      (prefixStub_ + "detWindow" + suffix_);
    produces<std::vector<bool> >     (prefixStub_ + "psmodule" + suffix_);
    produces<std::vector<bool> >     (prefixStub_ + "isGenuine" + suffix_);
    produces<std::vector<bool> >     (prefixStub_ + "isUnknown" + suffix_);
    produces<std::vector<bool> >     (prefixStub_ + "isCombinatoric" + suffix_);
    produces<std::vector<int> >      (prefixStub_ + "tpId"   + suffix_);
    produces<std::vector<int> >      (prefixStub_ + "pdgId"  + suffix_);
    produces<std::vector<float> >    (prefixStub_ + "simPt"  + suffix_);
    produces<std::vector<float> >    (prefixStub_ + "simEta" + suffix_);
    produces<unsigned>               (prefixStub_ + "size"   + suffix_);

    produces<std::vector<float> >    (prefixTrack_ + "px"    + suffix_);
    produces<std::vector<float> >    (prefixTrack_ + "py"    + suffix_);
    produces<std::vector<float> >    (prefixTrack_ + "pz"    + suffix_);
    produces<std::vector<float> >    (prefixTrack_ + "pt"    + suffix_);
    produces<std::vector<float> >    (prefixTrack_ + "eta"   + suffix_);
    produces<std::vector<float> >    (prefixTrack_ + "phi"   + suffix_);
    produces<std::vector<float> >    (prefixTrack_ + "vx"    + suffix_);
    produces<std::vector<float> >    (prefixTrack_ + "vy"    + suffix_);
    produces<std::vector<float> >    (prefixTrack_ + "vz"    + suffix_);
    produces<std::vector<float> >    (prefixTrack_ + "rinv"  + suffix_);
    produces<std::vector<float> >    (prefixTrack_ + "chi2"  + suffix_);
    produces<std::vector<float> >    (prefixTrack_ + "ptconsistency" + suffix_);
    produces<std::vector<unsigned> > (prefixTrack_ + "nstubs" + suffix_);
    produces<std::vector<bool> >     (prefixTrack_ + "isGenuine" + suffix_);
    produces<std::vector<bool> >     (prefixTrack_ + "isUnknown" + suffix_);
    produces<std::vector<bool> >     (prefixTrack_ + "isCombinatoric" + suffix_);
    produces<std::vector<int> >      (prefixTrack_ + "tpId"  + suffix_);
    produces<std::vector<int> >      (prefixTrack_ + "pdgId" + suffix_);
    produces<std::vector<float> >    (prefixTrack_ + "simPt" + suffix_);
    produces<std::vector<float> >    (prefixTrack_ + "simEta" + suffix_);
    produces<unsigned>               (prefixTrack_ + "size"  + suffix_);
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

    std::auto_ptr<std::vector<float> >    vc_x     (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    vc_y     (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    vc_z     (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    vc_localx(new std::vector<float>());
    std::auto_ptr<std::vector<float> >    vc_localy(new std::vector<float>());
    std::auto_ptr<std::vector<float> >    vc_localz(new std::vector<float>());
    std::auto_ptr<std::vector<unsigned> > vc_rawId (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> > vc_ilayer(new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> > vc_iring (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> > vc_iside (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> > vc_iphi  (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> > vc_iz    (new std::vector<unsigned>());
    std::auto_ptr<std::vector<bool> >     vc_barrel(new std::vector<bool>());
    std::auto_ptr<std::vector<unsigned> > vc_geoId (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> > vc_stack (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> > vc_width (new std::vector<unsigned>());
    std::auto_ptr<std::vector<std::vector<int> > > vc_rows (new std::vector<std::vector<int> >());
    std::auto_ptr<std::vector<std::vector<int> > > vc_cols (new std::vector<std::vector<int> >());
    std::auto_ptr<std::vector<std::vector<int> > > vc_adcs (new std::vector<std::vector<int> >());
    std::auto_ptr<std::vector<std::vector<int> > > vc_chans (new std::vector<std::vector<int> >());
    std::auto_ptr<std::vector<float> >    vc_coordx(new std::vector<float>());
    std::auto_ptr<std::vector<float> >    vc_coordy(new std::vector<float>());
    std::auto_ptr<std::vector<bool> >     vc_isGenuine(new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >     vc_isUnknown(new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >     vc_isCombinatoric(new std::vector<bool>());
    std::auto_ptr<std::vector<int> >      vc_tpId  (new std::vector<int>());
    std::auto_ptr<std::vector<int> >      vc_pdgId (new std::vector<int>());
    std::auto_ptr<std::vector<float> >    vc_simPt (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    vc_simEta(new std::vector<float>());
    std::auto_ptr<unsigned>               vc_size  (new unsigned(0));

    std::auto_ptr<std::vector<float> >    vb_x     (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    vb_y     (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    vb_z     (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    vb_dirx  (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    vb_diry  (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    vb_dirz  (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    vb_roughPt(new std::vector<float>());
    std::auto_ptr<std::vector<unsigned> > vb_rawId (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> > vb_ilayer(new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> > vb_iring (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> > vb_iside (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> > vb_iphi  (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> > vb_iz    (new std::vector<unsigned>());
    std::auto_ptr<std::vector<bool> >     vb_barrel(new std::vector<bool>());
    std::auto_ptr<std::vector<unsigned> > vb_geoId0(new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> > vb_geoId1(new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> > vb_clusId0(new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> > vb_clusId1(new std::vector<unsigned>());
    std::auto_ptr<std::vector<float> >    vb_trigDist(new std::vector<float>());
    std::auto_ptr<std::vector<float> >    vb_trigOffset(new std::vector<float>());
    std::auto_ptr<std::vector<float> >    vb_trigPos(new std::vector<float>());
    std::auto_ptr<std::vector<float> >    vb_trigBend(new std::vector<float>());
    std::auto_ptr<std::vector<int> >      vb_detWindow(new std::vector<int>());
    std::auto_ptr<std::vector<bool> >     vb_psmodule(new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >     vb_isGenuine(new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >     vb_isUnknown(new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >     vb_isCombinatoric(new std::vector<bool>());
    std::auto_ptr<std::vector<int> >      vb_tpId  (new std::vector<int>());
    std::auto_ptr<std::vector<int> >      vb_pdgId (new std::vector<int>());
    std::auto_ptr<std::vector<float> >    vb_simPt (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    vb_simEta(new std::vector<float>());
    std::auto_ptr<unsigned>               vb_size  (new unsigned(0));

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
    std::auto_ptr<std::vector<bool> >  vt_isGenuine(new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >  vt_isUnknown(new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >  vt_isCombinatoric(new std::vector<bool>());
    std::auto_ptr<std::vector<int> >   vt_tpId  (new std::vector<int>());
    std::auto_ptr<std::vector<int> >   vt_pdgId (new std::vector<int>());
    std::auto_ptr<std::vector<float> > vt_simPt (new std::vector<float>());
    std::auto_ptr<std::vector<float> > vt_simEta(new std::vector<float>());
    std::auto_ptr<unsigned>            vt_size  (new unsigned(0));

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

    /// SimPixelDigis
    edm::Handle<edm::DetSetVector<PixelDigi> > pixelDigis;
    iEvent.getByLabel(inputTagPixelDigi_, pixelDigis);
    edm::Handle<edm::DetSetVector<PixelDigiSimLink> >  pixelDigiSimLinks;
    iEvent.getByLabel(inputTagPixelDigi_, pixelDigiSimLinks);


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
            edm::LogInfo("NTupleClusters") << "dsv_detId: " << dsv_detId << " dsv_size: " << dsv_size;

            for (const_ds_iter it = itv->begin(); it != itv->end(); ++it) {
                const StackedTrackerDetId detId = it->getDetId();  // same as stub detId
                uint32_t rawId = detId.rawId();
                unsigned iLayer = detId.iLayer();
                unsigned iRing = detId.iRing();
                unsigned iSide = detId.iSide();
                if (iLayer == 999999 && (iRing == 999999 || iSide == 999999)) {
                    edm::LogError("NTupleClusters") << "Inconsistent detId with iLayer: " << iLayer << ", iRing: " << iRing << ", iSide: " << iSide;
                }
                unsigned iPhi = detId.iPhi();
                unsigned iZ = detId.iZ();
                bool isBarrel = detId.isBarrel();
                bool isEndcap = detId.isEndcap();
                if (isBarrel == isEndcap) {
                    edm::LogError("NTupleClusters") << "Inconsistent detId with isBarrel: " << isBarrel << ", isEndcap: " << isEndcap;
                }

                unsigned stack = it->getStackMember();
                unsigned width = it->findWidth();
                MeasurementPoint localcoord = it->findAverageLocalCoordinates();
                const std::vector<int>& rows = it->getRows();
                const std::vector<int>& cols = it->getCols();
                const std::vector<Ref_PixelDigi_>& hits = it->getHits();
                if (hits.size() != rows.size() || hits.size() != cols.size()) {
                    edm::LogError("NTupleClusters") << "Inconsistent # hits: " << hits.size() << " vs # rows: " << rows.size() << ", # cols: " << cols.size();
                }
                std::vector<int> adcs;  // should be unsigned, but use int for uniformity
                std::vector<int> channels;
                for (unsigned ii = 0; ii < hits.size(); ++ii) {
                    adcs.push_back(hits.at(ii)->adc());
                    channels.push_back(PixelDigi::pixelToChannel(rows.at(ii), cols.at(ii)) );
                }

                const DetId geoId = theStackedGeometry->idToDet(detId, stack)->geographicalId();
                LocalPoint  localposition = theStackedGeometry->findAverageLocalPosition(&(*it));
                GlobalPoint position = theStackedGeometry->findAverageGlobalPosition(&(*it));

                edm::LogInfo("NTupleClusters") << "rawId: " << rawId << " iLayer: " << iLayer << " iRing: " << iRing << " iSide: " << iSide << " iPhi: " << iPhi << " iZ: " << iZ << " isBarrel: " << isBarrel << " geoId: " << geoId.rawId() << " stack: " << stack << " width: " << width << " localcoord: " << localcoord << " localposition: " << localposition << " position: " << position;

                vc_x->push_back(position.x());
                vc_y->push_back(position.y());
                vc_z->push_back(position.z());
                vc_localx->push_back(localposition.x());
                vc_localy->push_back(localposition.y());
                vc_localz->push_back(localposition.z());
                vc_rawId->push_back(rawId);  // this is stub raw Id
                vc_ilayer->push_back(iLayer);
                vc_iring->push_back(iRing);
                vc_iside->push_back(iSide);
                vc_iphi->push_back(iPhi);
                vc_iz->push_back(iZ);
                vc_barrel->push_back(isBarrel);
                vc_geoId->push_back(geoId.rawId());
                vc_stack->push_back(stack);
                vc_width->push_back(width);  // sviret/HL_LHC: CLUS_nstrips
                vc_rows->push_back(rows);  // sviret/HL_LHC: CLUS_PIX
                vc_cols->push_back(cols);  // sviret/HL_LHC: CLUS_PIX
                vc_adcs->push_back(adcs);
                vc_chans->push_back(channels);
                vc_coordx->push_back(localcoord.x());  // sviret/HL_LHC: CLUS_seg
                vc_coordy->push_back(localcoord.y());  // sviret/HL_LHC: CLUS_strip

                /// Make the reference to retrieve from MC matching
                reference iref = edmNew::makeRefTo(pixelDigiTTClusters, it);
                vc_isGenuine->push_back(mcAssocTTClusters->isGenuine(iref));
                vc_isUnknown->push_back(mcAssocTTClusters->isUnknown(iref));
                vc_isCombinatoric->push_back(mcAssocTTClusters->isCombinatoric(iref));
                if (vc_isGenuine->back()) {
                    edm::Ptr<TrackingParticle> tpptr = mcAssocTTClusters->findTrackingParticlePtr(iref);
                    vc_tpId->push_back(tpptr.key());
                    vc_pdgId->push_back(tpptr->pdgId());
                    vc_simPt->push_back(tpptr->pt());
                    vc_simEta->push_back(tpptr->eta());
                } else {
                    vc_tpId->push_back(-1);
                    vc_pdgId->push_back(-99);
                    vc_simPt->push_back(-99.);
                    vc_simEta->push_back(-99.);
                }

                n++;
            }
        }
        *vc_size = vc_x->size();

    } else {
        edm::LogError("NTupleClusters") << "Cannot get the product: " << inputTagClus_;
    }


    /// TTStubs
    if (pixelDigiTTStubs.isValid() && mcAssocTTStubs.isValid()) {
        edm::LogInfo("NTupleStubs") << "Size: " << pixelDigiTTStubs->size();

        typedef typename edmNew::DetSetVector<TTStub<Ref_PixelDigi_> >::const_iterator const_dsv_iter;
        typedef typename edmNew::DetSet      <TTStub<Ref_PixelDigi_> >::const_iterator const_ds_iter;
        typedef edm::Ref<edmNew::DetSetVector<TTStub<Ref_PixelDigi_> >, TTStub<Ref_PixelDigi_> > reference;

        unsigned n = 0;
        for (const_dsv_iter itv = pixelDigiTTStubs->begin(); itv != pixelDigiTTStubs->end(); ++itv) {
            unsigned dsv_detId = itv->detId();
            unsigned dsv_size = itv->size();
            edm::LogInfo("NTupleStubs") << "dsv_detId: " << dsv_detId << " dsv_size: " << dsv_size;

            for (const_ds_iter it = itv->begin(); it != itv->end(); ++it) {
                const StackedTrackerDetId detId = it->getDetId();
                uint32_t rawId = detId.rawId();
                unsigned iLayer = detId.iLayer();
                unsigned iRing = detId.iRing();
                unsigned iSide = detId.iSide();
                if (iLayer == 999999 && (iRing == 999999 || iSide == 999999)) {
                    edm::LogError("NTupleStubs") << "Inconsistent detId with iLayer: " << iLayer << ", iRing: " << iRing << ", iSide: " << iSide;
                }
                unsigned iPhi = detId.iPhi();
                unsigned iZ = detId.iZ();
                bool isBarrel = detId.isBarrel();
                bool isEndcap = detId.isEndcap();
                if (isBarrel == isEndcap) {
                    edm::LogError("NTupleStubs") << "Inconsistent detId with isBarrel: " << isBarrel << ", isEndcap: " << isEndcap;
                }

                // Global position and rough pt
                double magfieldStrength = theMagneticField->inTesla(GlobalPoint(0,0,0)).z();
                double roughPt = theStackedGeometry->findRoughPt(magfieldStrength, &(*it));
                GlobalPoint position = theStackedGeometry->findGlobalPosition(&(*it));
                GlobalVector direction = theStackedGeometry->findGlobalDirection(&(*it));

                // Get the clusters (inner=0, outer=1)
                //const std::vector<edm::Ref<edmNew::DetSetVector<TTCluster<Ref_PixelDigi_> >, TTCluster<Ref_PixelDigi_> > >& clusterRefs = it->getClusterRefs();
                //if (clusterRefs.size() != 2) {
                //    edm::LogError("NTupleStubs") << "# clusters != 2: " << clusterRefs.size();
                //}
                //const DetId geoId0 = theStackedGeometry->idToDet(clusterRefs[0]->getDetId(), clusterRefs[0]->getStackMember())->geographicalId();
                //const DetId geoId1 = theStackedGeometry->idToDet(clusterRefs[1]->getDetId(), clusterRefs[1]->getStackMember())->geographicalId();
                const DetId geoId0 = theStackedGeometry->idToDet(detId, 0)->geographicalId();
                const DetId geoId1 = theStackedGeometry->idToDet(detId, 1)->geographicalId();

                // Get the cluster indices in this ntuple
                if (vc_geoId->empty()) {
                    edm::LogError("NTupleStubs") << "TTClusters have not been filled!" << std::endl;
                }
                unsigned clusId0 = findById(*vc_geoId, geoId0.rawId());
                unsigned clusId1 = findById(*vc_geoId, geoId1.rawId());

                edm::LogInfo("NTupleStubs") << "rawId: " << rawId << " iLayer: " << iLayer << " iRing: " << iRing << " iSide: " << iSide << " iPhi: " << iPhi << " iZ: " << iZ << " isBarrel: " << isBarrel << " roughPt: " << roughPt << " position: " << position << " direction: " << direction << " geoId0: " << geoId0.rawId() << " geoId1: " << geoId1.rawId() << " clusId0: " << clusId0 << " clusId1: " << clusId1;

                vb_x->push_back(position.x());
                vb_y->push_back(position.y());
                vb_z->push_back(position.z());
                vb_dirx->push_back(direction.x());
                vb_diry->push_back(direction.y());
                vb_dirz->push_back(direction.z());
                vb_roughPt->push_back(roughPt);
                vb_rawId->push_back(rawId);
                vb_ilayer->push_back(iLayer);
                vb_iring->push_back(iRing);
                vb_iside->push_back(iSide);
                vb_iphi->push_back(iPhi);
                vb_iz->push_back(iZ);
                vb_barrel->push_back(isBarrel);
                vb_geoId0->push_back(geoId0.rawId());
                vb_geoId1->push_back(geoId1.rawId());
                vb_clusId0->push_back(clusId0);
                vb_clusId1->push_back(clusId1);
                vb_trigDist->push_back(it->getTriggerDisplacement());  // in full-strip units
                vb_trigOffset->push_back(it->getTriggerOffset());
                vb_trigPos->push_back(it->getTriggerPosition());
                vb_trigBend->push_back(it->getTriggerBend());
                vb_detWindow->push_back(theStackedGeometry->getDetUnitWindow(detId));
                vb_psmodule->push_back(theStackedGeometry->isPSModule(detId));

                /// Make the reference to retrieve from MC matching
                reference iref = edmNew::makeRefTo(pixelDigiTTStubs, it);
                vb_isGenuine->push_back(mcAssocTTStubs->isGenuine(iref));
                vb_isUnknown->push_back(mcAssocTTStubs->isUnknown(iref));
                vb_isCombinatoric->push_back(mcAssocTTStubs->isCombinatoric(iref));
                if (vb_isGenuine->back()) {
                    edm::Ptr<TrackingParticle> tpptr = mcAssocTTStubs->findTrackingParticlePtr(iref);
                    vb_tpId->push_back(tpptr.key());
                    vb_pdgId->push_back(tpptr->pdgId());
                    vb_simPt->push_back(tpptr->pt());
                    vb_simEta->push_back(tpptr->eta());
                } else {
                    vb_tpId->push_back(-1);
                    vb_pdgId->push_back(-99);
                    vb_simPt->push_back(-99.);
                    vb_simEta->push_back(-99.);
                }

                n++;
            }
        }
        *vb_size = vb_x->size();

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
            GlobalVector momentum = it->getMomentum();
            GlobalPoint  poca = it->getPOCA();  // point of closest approach

            edm::LogInfo("NTupleTracks") << "pt: " << momentum.perp() << " vz: " << poca.z() << std::endl;

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

            /// Make the pointer to retrieve from MC matching
            pointer iptr(pixelDigiTTTracks, it - pixelDigiTTTracks->begin());
            vt_isGenuine->push_back(mcAssocTTTracks->isGenuine(iptr));
            vt_isUnknown->push_back(mcAssocTTTracks->isUnknown(iptr));
            vt_isCombinatoric->push_back(mcAssocTTTracks->isCombinatoric(iptr));
            if (vt_isGenuine->back()) {
                edm::Ptr<TrackingParticle> tpptr = mcAssocTTTracks->findTrackingParticlePtr(iptr);
                vt_tpId->push_back(tpptr.key());
                vt_pdgId->push_back(tpptr->pdgId());
                vt_simPt->push_back(tpptr->pt());
                vt_simEta->push_back(tpptr->eta());
            } else {
                vt_tpId->push_back(-1);
                vt_pdgId->push_back(-99);
                vt_simPt->push_back(-99.);
                vt_simEta->push_back(-99.);
            }

            n++;
        }
        *vt_size = vt_px->size();

    } else {
        edm::LogError("NTupleTracks") << "Cannot get the product: " << inputTagTrack_;
    }


    /// SimPixelDigis -- skipped for now
    if (pixelDigis.isValid() && pixelDigiSimLinks.isValid()) {
        edm::LogInfo("NTupleSimPixelDigis") << "Size: " << pixelDigis->size();

        typedef edm::DetSetVector<PixelDigi>::const_iterator const_dsv_iterator;
        typedef edm::DetSet      <PixelDigi>::const_iterator const_ds_iterator;
        unsigned n = 0;
        for (const_dsv_iterator itv = pixelDigis->begin(); itv != pixelDigis->end(); ++itv) {
            uint32_t rawId = itv->detId();
            StackedTrackerDetId detId(rawId);
            DetId::Detector det = detId.det();  // Tracker=1,Muon=2,Ecal=3,Hcal=4,Calo=5,Forward=6
            int subdetId = detId.subdetId();  // PXB=1,PXF=2,...
            unsigned size = itv->size();
            edm::LogInfo("NTupleSimPixelDigis") << "rawId: " << rawId << " det: " << det << " subdetId: " << subdetId << " size: " << size;

            for (const_ds_iterator it = itv->begin(); it != itv->end(); ++it) {
                int row = it->row();
                int col = it->column();
                int channel = PixelDigi::pixelToChannel(row, col);
                unsigned short adc = it->adc();
                edm::LogInfo("NTupleSimPixelDigis") << "row: " << row << " col: " << col << " channel: " << channel << " adc: " << adc;
            }

            n++;
        }

    } else {
        edm::LogError("NTupleSimPixelDigis") << "Cannot get the product: " << inputTagPixelDigi_;
    }


    //__________________________________________________________________________
    iEvent.put(vc_x             , prefixClus_ + "x"      + suffix_);
    iEvent.put(vc_y             , prefixClus_ + "y"      + suffix_);
    iEvent.put(vc_z             , prefixClus_ + "z"      + suffix_);
    iEvent.put(vc_localx        , prefixClus_ + "localx" + suffix_);
    iEvent.put(vc_localy        , prefixClus_ + "localy" + suffix_);
    iEvent.put(vc_localz        , prefixClus_ + "localz" + suffix_);
    iEvent.put(vc_rawId         , prefixClus_ + "rawId"  + suffix_);
    iEvent.put(vc_ilayer        , prefixClus_ + "ilayer" + suffix_);
    iEvent.put(vc_iring         , prefixClus_ + "iring"  + suffix_);
    iEvent.put(vc_iside         , prefixClus_ + "iside"  + suffix_);
    iEvent.put(vc_iphi          , prefixClus_ + "iphi"   + suffix_);
    iEvent.put(vc_iz            , prefixClus_ + "iz"     + suffix_);
    iEvent.put(vc_barrel        , prefixClus_ + "barrel" + suffix_);
    iEvent.put(vc_geoId         , prefixClus_ + "geoId"  + suffix_);
    iEvent.put(vc_stack         , prefixClus_ + "stack"  + suffix_);
    iEvent.put(vc_width         , prefixClus_ + "width"  + suffix_);
    iEvent.put(vc_rows          , prefixClus_ + "rows"   + suffix_);
    iEvent.put(vc_cols          , prefixClus_ + "cols"   + suffix_);
    iEvent.put(vc_adcs          , prefixClus_ + "adcs"   + suffix_);
    iEvent.put(vc_chans         , prefixClus_ + "chans"  + suffix_);
    iEvent.put(vc_coordx        , prefixClus_ + "coordx" + suffix_);
    iEvent.put(vc_coordy        , prefixClus_ + "coordy" + suffix_);
    iEvent.put(vc_isGenuine     , prefixClus_ + "isGenuine" + suffix_);
    iEvent.put(vc_isUnknown     , prefixClus_ + "isUnknown" + suffix_);
    iEvent.put(vc_isCombinatoric, prefixClus_ + "isCombinatoric" + suffix_);
    iEvent.put(vc_tpId          , prefixClus_ + "tpId"   + suffix_);
    iEvent.put(vc_pdgId         , prefixClus_ + "pdgId"  + suffix_);
    iEvent.put(vc_simPt         , prefixClus_ + "simPt"  + suffix_);
    iEvent.put(vc_simEta        , prefixClus_ + "simEta" + suffix_);
    iEvent.put(vc_size          , prefixClus_ + "size"   + suffix_);

    iEvent.put(vb_x             , prefixStub_ + "x"      + suffix_);
    iEvent.put(vb_y             , prefixStub_ + "y"      + suffix_);
    iEvent.put(vb_z             , prefixStub_ + "z"      + suffix_);
    iEvent.put(vb_dirx          , prefixStub_ + "dirx"   + suffix_);
    iEvent.put(vb_diry          , prefixStub_ + "diry"   + suffix_);
    iEvent.put(vb_dirz          , prefixStub_ + "dirz"   + suffix_);
    iEvent.put(vb_roughPt       , prefixStub_ + "roughPt"+ suffix_);
    iEvent.put(vb_rawId         , prefixStub_ + "rawId"  + suffix_);
    iEvent.put(vb_ilayer        , prefixStub_ + "ilayer" + suffix_);
    iEvent.put(vb_iring         , prefixStub_ + "iring"  + suffix_);
    iEvent.put(vb_iside         , prefixStub_ + "iside"  + suffix_);
    iEvent.put(vb_iphi          , prefixStub_ + "iphi"   + suffix_);
    iEvent.put(vb_iz            , prefixStub_ + "iz"     + suffix_);
    iEvent.put(vb_barrel        , prefixStub_ + "barrel" + suffix_);
    iEvent.put(vb_trigDist      , prefixStub_ + "trigDist" + suffix_);
    iEvent.put(vb_trigOffset    , prefixStub_ + "trigOffset" + suffix_);
    iEvent.put(vb_trigPos       , prefixStub_ + "trigPos" + suffix_);
    iEvent.put(vb_trigBend      , prefixStub_ + "trigBend" + suffix_);
    iEvent.put(vb_geoId0        , prefixStub_ + "geoId0" + suffix_);
    iEvent.put(vb_geoId1        , prefixStub_ + "geoId1" + suffix_);
    iEvent.put(vb_clusId0       , prefixStub_ + "clusId0" + suffix_);
    iEvent.put(vb_clusId1       , prefixStub_ + "clusId1" + suffix_);
    iEvent.put(vb_detWindow     , prefixStub_ + "detWindow" + suffix_);
    iEvent.put(vb_psmodule      , prefixStub_ + "psmodule" + suffix_);
    iEvent.put(vb_isGenuine     , prefixStub_ + "isGenuine" + suffix_);
    iEvent.put(vb_isUnknown     , prefixStub_ + "isUnknown" + suffix_);
    iEvent.put(vb_isCombinatoric, prefixStub_ + "isCombinatoric" + suffix_);
    iEvent.put(vb_tpId          , prefixStub_ + "tpId"   + suffix_);
    iEvent.put(vb_pdgId         , prefixStub_ + "pdgId"  + suffix_);
    iEvent.put(vb_simPt         , prefixStub_ + "simPt"  + suffix_);
    iEvent.put(vb_simEta        , prefixStub_ + "simEta" + suffix_);
    iEvent.put(vb_size          , prefixStub_ + "size"   + suffix_);

    iEvent.put(vt_px            , prefixTrack_ + "px"    + suffix_);
    iEvent.put(vt_py            , prefixTrack_ + "py"    + suffix_);
    iEvent.put(vt_pz            , prefixTrack_ + "pz"    + suffix_);
    iEvent.put(vt_pt            , prefixTrack_ + "pt"    + suffix_);
    iEvent.put(vt_eta           , prefixTrack_ + "eta"   + suffix_);
    iEvent.put(vt_phi           , prefixTrack_ + "phi"   + suffix_);
    iEvent.put(vt_vx            , prefixTrack_ + "vx"    + suffix_);
    iEvent.put(vt_vy            , prefixTrack_ + "vy"    + suffix_);
    iEvent.put(vt_vz            , prefixTrack_ + "vz"    + suffix_);
    iEvent.put(vt_rinv          , prefixTrack_ + "rinv"  + suffix_);
    iEvent.put(vt_chi2          , prefixTrack_ + "chi2"  + suffix_);
    iEvent.put(vt_ptconsistency , prefixTrack_ + "ptconsistency" + suffix_);
    iEvent.put(vt_nstubs        , prefixTrack_ + "nstubs" + suffix_);
    iEvent.put(vt_isGenuine     , prefixTrack_ + "isGenuine" + suffix_);
    iEvent.put(vt_isUnknown     , prefixTrack_ + "isUnknown" + suffix_);
    iEvent.put(vt_isCombinatoric, prefixTrack_ + "isCombinatoric" + suffix_);
    iEvent.put(vt_tpId          , prefixTrack_ + "tpId"  + suffix_);
    iEvent.put(vt_pdgId         , prefixTrack_ + "pdgId" + suffix_);
    iEvent.put(vt_simPt         , prefixTrack_ + "simPt" + suffix_);
    iEvent.put(vt_simEta        , prefixTrack_ + "simEta" + suffix_);
    iEvent.put(vt_size          , prefixTrack_ + "size"  + suffix_);
}
