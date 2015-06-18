#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/NTupleTTTracks.h"

#include "SLHCL1TrackTriggerSimulations/NTupleTools/interface/ModuleIdFunctor.h"
#include "SimTracker/TrackTriggerAssociation/interface/TTClusterAssociationMap.h"  // This should have been included by TTTrackAssociationMap.h
#include "SimTracker/TrackTriggerAssociation/interface/TTStubAssociationMap.h"  // This should have been included by TTTrackAssociationMap.h
#include "SimTracker/TrackTriggerAssociation/interface/TTTrackAssociationMap.h"


NTupleTTTracks::NTupleTTTracks(const edm::ParameterSet& iConfig) :
  inputTag_   (iConfig.getParameter<edm::InputTag>("inputTag")),
  inputTagMC_ (iConfig.getParameter<edm::InputTag>("inputTagMC")),
  nparameters_(iConfig.getParameter<int>("nparameters")),
  prefix_     (iConfig.getParameter<std::string>("prefix")),
  suffix_     (iConfig.getParameter<std::string>("suffix")),
  selector_   (iConfig.existsAs<std::string>("cut") ? iConfig.getParameter<std::string>("cut") : "", true),
  maxN_       (iConfig.getParameter<unsigned>("maxN")) {

    produces<std::vector<float> >                   (prefix_ + "px"             + suffix_);
    produces<std::vector<float> >                   (prefix_ + "py"             + suffix_);
    produces<std::vector<float> >                   (prefix_ + "pz"             + suffix_);
    produces<std::vector<float> >                   (prefix_ + "pt"             + suffix_);
    produces<std::vector<float> >                   (prefix_ + "eta"            + suffix_);
    produces<std::vector<float> >                   (prefix_ + "phi"            + suffix_);
    produces<std::vector<float> >                   (prefix_ + "vx"             + suffix_);
    produces<std::vector<float> >                   (prefix_ + "vy"             + suffix_);
    produces<std::vector<float> >                   (prefix_ + "vz"             + suffix_);
    produces<std::vector<float> >                   (prefix_ + "rinv"           + suffix_);
    produces<std::vector<float> >                   (prefix_ + "invPt"          + suffix_);
    produces<std::vector<float> >                   (prefix_ + "phi0"           + suffix_);
    produces<std::vector<float> >                   (prefix_ + "cottheta"       + suffix_);
    produces<std::vector<float> >                   (prefix_ + "z0"             + suffix_);
    produces<std::vector<float> >                   (prefix_ + "d0"             + suffix_);
    produces<std::vector<float> >                   (prefix_ + "chi2"           + suffix_);
    produces<std::vector<int> >                     (prefix_ + "ndof"           + suffix_);
    produces<std::vector<unsigned> >                (prefix_ + "sector"         + suffix_);
    produces<std::vector<unsigned> >                (prefix_ + "wedge"          + suffix_);
    produces<std::vector<float> >                   (prefix_ + "ptconsistency"  + suffix_);
    produces<std::vector<std::vector<unsigned> > >  (prefix_ + "stubRefs"       + suffix_);
    produces<std::vector<bool> >                    (prefix_ + "isGenuine"      + suffix_);
    produces<std::vector<bool> >                    (prefix_ + "isUnknown"      + suffix_);
    produces<std::vector<bool> >                    (prefix_ + "isCombinatoric" + suffix_);
    produces<std::vector<int> >                     (prefix_ + "tpId"           + suffix_);
    produces<std::vector<int> >                     (prefix_ + "pdgId"          + suffix_);
    produces<std::vector<float> >                   (prefix_ + "simPt"          + suffix_);
    produces<std::vector<float> >                   (prefix_ + "simEta"         + suffix_);
    produces<std::vector<float> >                   (prefix_ + "simPhi"         + suffix_);
    produces<unsigned>                              (prefix_ + "size"           + suffix_);
}

void NTupleTTTracks::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<std::vector<float> >                  v_px            (new std::vector<float>());
    std::auto_ptr<std::vector<float> >                  v_py            (new std::vector<float>());
    std::auto_ptr<std::vector<float> >                  v_pz            (new std::vector<float>());
    std::auto_ptr<std::vector<float> >                  v_pt            (new std::vector<float>());
    std::auto_ptr<std::vector<float> >                  v_eta           (new std::vector<float>());
    std::auto_ptr<std::vector<float> >                  v_phi           (new std::vector<float>());
    std::auto_ptr<std::vector<float> >                  v_vx            (new std::vector<float>());
    std::auto_ptr<std::vector<float> >                  v_vy            (new std::vector<float>());
    std::auto_ptr<std::vector<float> >                  v_vz            (new std::vector<float>());
    std::auto_ptr<std::vector<float> >                  v_rinv          (new std::vector<float>());
    std::auto_ptr<std::vector<float> >                  v_invPt         (new std::vector<float>());
    std::auto_ptr<std::vector<float> >                  v_phi0          (new std::vector<float>());
    std::auto_ptr<std::vector<float> >                  v_cottheta      (new std::vector<float>());
    std::auto_ptr<std::vector<float> >                  v_z0            (new std::vector<float>());
    std::auto_ptr<std::vector<float> >                  v_d0            (new std::vector<float>());
    std::auto_ptr<std::vector<float> >                  v_chi2          (new std::vector<float>());
    std::auto_ptr<std::vector<int> >                    v_ndof          (new std::vector<int>());
    std::auto_ptr<std::vector<unsigned> >               v_sector        (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> >               v_wedge         (new std::vector<unsigned>());
    std::auto_ptr<std::vector<float> >                  v_ptconsistency (new std::vector<float>());
    std::auto_ptr<std::vector<std::vector<unsigned> > > v_stubRefs      (new std::vector<std::vector<unsigned> >());
    std::auto_ptr<std::vector<bool> >                   v_isGenuine     (new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >                   v_isUnknown     (new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >                   v_isCombinatoric(new std::vector<bool>());
    std::auto_ptr<std::vector<int> >                    v_tpId          (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v_pdgId         (new std::vector<int>());
    std::auto_ptr<std::vector<float> >                  v_simPt         (new std::vector<float>());
    std::auto_ptr<std::vector<float> >                  v_simEta        (new std::vector<float>());
    std::auto_ptr<std::vector<float> >                  v_simPhi        (new std::vector<float>());
    std::auto_ptr<unsigned>                             v_size          (new unsigned(0));

    // _________________________________________________________________________
    /// TTTrack
    edm::Handle<std::vector<TTTrack<Ref_PixelDigi_> > > pixelDigiTTTracks;
    iEvent.getByLabel(inputTag_, pixelDigiTTTracks);

    /// MC truth association map
    edm::Handle<TTTrackAssociationMap<Ref_PixelDigi_> > mcAssocTTTracks;
    iEvent.getByLabel(inputTagMC_, mcAssocTTTracks);

    if (pixelDigiTTTracks.isValid()) {
        edm::LogInfo("NTupleTracks") << "Size: " << pixelDigiTTTracks->size();

        typedef typename std::vector<TTTrack<Ref_PixelDigi_> >::const_iterator const_iterator;
        typedef edm::Ptr<TTTrack<Ref_PixelDigi_> > pointer;
        typedef edm::Ref<edmNew::DetSetVector<TTStub<Ref_PixelDigi_> >, TTStub<Ref_PixelDigi_> > dsv_ref_stub;
        //typedef typename edmNew::DetSetVector<TTStub<Ref_PixelDigi_> >::const_iterator const_dsv_iter_stub;
        //typedef typename edmNew::DetSet      <TTStub<Ref_PixelDigi_> >::const_iterator const_ds_iter_stub;

        unsigned nPar = nparameters_;
        unsigned n = 0;
        for (const_iterator it = pixelDigiTTTracks->begin(); it != pixelDigiTTTracks->end(); ++it) {
            if (n >= maxN_)
                break;
            if (!selector_(*it))
                continue;

            const GlobalVector&              momentum = it->getMomentum(nPar);
            const GlobalPoint&               poca     = it->getPOCA(nPar);  // point of closest approach
            const std::vector<dsv_ref_stub>& stubRefs = it->getStubRefs();

            std::vector<unsigned> myStubRefs;
            //for (const_dsv_iter_stub itv = pixelDigiTTStubs->begin(); itv != pixelDigiTTStubs->end(); ++itv) {
            //    for (const_ds_iter_stub it = itv->begin(); it != itv->end(); ++it) {
            //        myStubRefs.push_back();
            //    }
            //}

            v_px->push_back(momentum.x());
            v_py->push_back(momentum.y());
            v_pz->push_back(momentum.z());
            v_pt->push_back(momentum.perp());
            v_eta->push_back(momentum.eta());
            v_phi->push_back(momentum.phi());
            v_vx->push_back(poca.x());  // technically incorrect
            v_vy->push_back(poca.y());  // technically incorrect
            v_vz->push_back(poca.z());  // technically incorrect
            v_rinv->push_back(it->getRInv(nPar));
            v_invPt->push_back(1.0/momentum.perp());
            v_phi0->push_back(momentum.phi());
            v_cottheta->push_back(1.0/std::tan(momentum.theta()));
            v_z0->push_back(poca.z());
            v_d0->push_back(poca.perp());
            v_chi2->push_back(it->getChi2(nPar));
            v_ndof->push_back(2*stubRefs.size() - nPar);
            v_sector->push_back(it->getSector());
            v_wedge->push_back(it->getWedge());
            v_ptconsistency->push_back(it->getStubPtConsistency(nPar));
            v_stubRefs->push_back(myStubRefs);  // FIXME

            // Set to dummy values first
            v_isGenuine->push_back(false);
            v_isUnknown->push_back(false);
            v_isCombinatoric->push_back(false);
            v_tpId->push_back(-1);
            v_pdgId->push_back(-99);
            v_simPt->push_back(-99.);
            v_simEta->push_back(-99.);
            v_simPhi->push_back(-99.);

            /// Retrieve MC association
            if (mcAssocTTTracks.isValid()) {
                pointer iptr(pixelDigiTTTracks, it - pixelDigiTTTracks->begin());
                v_isGenuine->back() = mcAssocTTTracks->isGenuine(iptr);
                v_isUnknown->back() = mcAssocTTTracks->isUnknown(iptr);
                v_isCombinatoric->back() = mcAssocTTTracks->isCombinatoric(iptr);
                edm::Ptr<TrackingParticle> tpptr = mcAssocTTTracks->findTrackingParticlePtr(iptr);
                if (tpptr.isNonnull()) {
                    assert(v_isGenuine->back() == true);
                    v_tpId->back() = tpptr.key();
                    v_pdgId->back() = tpptr->pdgId();
                    v_simPt->back() = tpptr->pt();
                    v_simEta->back() = tpptr->eta();
                    v_simPhi->back() = tpptr->phi();
                }
            }

            n++;
        }
        *v_size = v_px->size();

    } else {
        edm::LogError("NTupleTracks") << "Cannot get the product: " << inputTag_;
    }


    // _________________________________________________________________________
    iEvent.put(v_px             , prefix_ + "px"              + suffix_);
    iEvent.put(v_py             , prefix_ + "py"              + suffix_);
    iEvent.put(v_pz             , prefix_ + "pz"              + suffix_);
    iEvent.put(v_pt             , prefix_ + "pt"              + suffix_);
    iEvent.put(v_eta            , prefix_ + "eta"             + suffix_);
    iEvent.put(v_phi            , prefix_ + "phi"             + suffix_);
    iEvent.put(v_vx             , prefix_ + "vx"              + suffix_);
    iEvent.put(v_vy             , prefix_ + "vy"              + suffix_);
    iEvent.put(v_vz             , prefix_ + "vz"              + suffix_);
    iEvent.put(v_rinv           , prefix_ + "rinv"            + suffix_);
    iEvent.put(v_invPt          , prefix_ + "invPt"           + suffix_);
    iEvent.put(v_phi0           , prefix_ + "phi0"            + suffix_);
    iEvent.put(v_cottheta       , prefix_ + "cottheta"        + suffix_);
    iEvent.put(v_z0             , prefix_ + "z0"              + suffix_);
    iEvent.put(v_d0             , prefix_ + "d0"              + suffix_);
    iEvent.put(v_chi2           , prefix_ + "chi2"            + suffix_);
    iEvent.put(v_ndof           , prefix_ + "ndof"            + suffix_);
    iEvent.put(v_sector         , prefix_ + "sector"          + suffix_);
    iEvent.put(v_wedge          , prefix_ + "wedge"           + suffix_);
    iEvent.put(v_ptconsistency  , prefix_ + "ptconsistency"   + suffix_);
    iEvent.put(v_stubRefs       , prefix_ + "stubRefs"        + suffix_);
    iEvent.put(v_isGenuine      , prefix_ + "isGenuine"       + suffix_);
    iEvent.put(v_isUnknown      , prefix_ + "isUnknown"       + suffix_);
    iEvent.put(v_isCombinatoric , prefix_ + "isCombinatoric"  + suffix_);
    iEvent.put(v_tpId           , prefix_ + "tpId"            + suffix_);
    iEvent.put(v_pdgId          , prefix_ + "pdgId"           + suffix_);
    iEvent.put(v_simPt          , prefix_ + "simPt"           + suffix_);
    iEvent.put(v_simEta         , prefix_ + "simEta"          + suffix_);
    iEvent.put(v_simPhi         , prefix_ + "simPhi"          + suffix_);
    iEvent.put(v_size           , prefix_ + "size"            + suffix_);
}
