#include "../interface/TrackExtractor.h"


TrackExtractor::TrackExtractor(edm::InputTag tag)
{
  //std::cout << "TrackExtractor objet is created" << std::endl;


  m_tag = tag;


  // Tree definition

  m_tree      = new TTree("Track","RECO Track info") ;

  // Branches definition

  m_tree->Branch("n_tracks",  &m_n_tracks,  "n_tracks/I");
  m_tree->Branch("track_px",  &m_tracks_px,   "track_px[n_tracks]/F");  
  m_tree->Branch("track_py",  &m_tracks_py,   "track_py[n_tracks]/F");  
  m_tree->Branch("track_pz",  &m_tracks_pz,   "track_pz[n_tracks]/F"); 
  m_tree->Branch("track_vx",  &m_tracks_vx,   "track_vx[n_tracks]/F");  
  m_tree->Branch("track_vy",  &m_tracks_vy,   "track_vy[n_tracks]/F");  
  m_tree->Branch("track_vz",  &m_tracks_vz,   "track_vz[n_tracks]/F"); 
  m_tree->Branch("track_chi2",  &m_tracks_normChi2,   "track_chi2[n_tracks]/F"); 
  m_tree->Branch("track_dedx",  &m_tracks_dedx,       "track_dedx[n_tracks]/F");  
  m_tree->Branch("track_dedx_n",&m_tracks_dedx_n,     "track_dedx_n[n_tracks]/F"); 
  m_tree->Branch("track_nhits", &m_tracks_nhits,      "track_nhits[n_tracks]/F"); 
  m_tree->Branch("track_xhits","vector<int>",&m_tracks_xhit);
  m_tree->Branch("track_yhits","vector<int>",&m_tracks_yhit);
  m_tree->Branch("track_zhits","vector<int>",&m_tracks_zhit);

  // Set everything to 0

  TrackExtractor::reset();
}

TrackExtractor::~TrackExtractor()
{}



//
// Method filling the main particle tree
//

void TrackExtractor::writeInfo(const edm::Event *event, const edm::EventSetup *setup) 
{
  TrackExtractor::reset();

  edm::Handle<reco::TrackCollection> trackHandle;
  event->getByLabel(m_tag, trackHandle);

  if (trackHandle.isValid()) 
  {
    const reco::TrackCollection trackCollection = *(trackHandle.product());

    //std::cout << "Number of tracks " << trackCollection.size() << std::endl;

    m_n_tracks=static_cast<int>(trackCollection.size());

     
    if (m_n_tracks)
    {    

      if (m_tag.label()=="generalTracks")
      {	  
	edm::Handle<edm::ValueMap<reco::DeDxData> > dEdxTrackHandle;
	event->getByLabel("dedxHarmonic2", dEdxTrackHandle);
	const edm::ValueMap<reco::DeDxData> dEdxTrack = *(dEdxTrackHandle.product());

	for(int i=0; i<m_n_tracks; i++)
	{
	  if (i>=m_tracks_MAX) continue;

	  reco::TrackRef track  = reco::TrackRef( trackHandle, i );
	  m_tracks_dedx[i]   = dEdxTrack[track].dEdx();
	  m_tracks_dedx_n[i] = dEdxTrack[track].numberOfMeasurements();
	}
      }
      
      
      setup->get<TransientRecHitRecord>().get("WithTrackAngle",theTrackerRecHitBuilder);

      for(int i=0; i<m_n_tracks; i++)
      {
	if (i>=m_tracks_MAX) continue;
	

	reco::Track currentTrk = trackCollection.at(i);
	
	m_tracks_vx[i]         = currentTrk.vx();
	m_tracks_vy[i]         = currentTrk.vy();
	m_tracks_vz[i]         = currentTrk.vz();
	m_tracks_px[i]         = currentTrk.px();
	m_tracks_py[i]         = currentTrk.py();
	m_tracks_pz[i]         = currentTrk.pz();
	m_tracks_normChi2[i]   = currentTrk.normalizedChi2();
	

	if (m_tag.label()!="generalTracks")
	{	
	  reco::TrackRef track  = reco::TrackRef( trackHandle, i );

	  const reco::HitPattern& p = currentTrk.hitPattern();

	  int nghits =0;

	  for (int k=0; k<p.numberOfHits(); k++) 
	  {
	    TrackingRecHitRef rhit = currentTrk.recHit(k);
	    
	    if (rhit->isValid()) 
	    {
	      ++nghits;
	      TransientTrackingRecHit::RecHitPointer tthit = theTrackerRecHitBuilder->build(&*rhit);
	      GlobalPoint gPosition =  tthit->globalPosition();
	      
	      m_tracks_xhit.push_back(1000.*gPosition.x());
	      m_tracks_yhit.push_back(1000.*gPosition.y());
	      m_tracks_zhit.push_back(1000.*gPosition.z());
	      
	      
	      //std::cout << "valid hit found with global position = "<< gPosition << std::endl;
	    }
	  }

	  m_tracks_nhits[i]=nghits;
	}
      } 
    }
  }


  TrackExtractor::fillTree();
}


// Method initializing everything (to do for each event)

void TrackExtractor::reset()
{

  m_n_tracks = 0;

  m_tracks_xhit.clear();
  m_tracks_yhit.clear();
  m_tracks_zhit.clear();

  for (int i=0;i<m_tracks_MAX;++i) 
  {
    m_tracks_px[i] = 0.;
    m_tracks_py[i] = 0.;
    m_tracks_pz[i] = 0.;
    m_tracks_vx[i] = 0.;
    m_tracks_vy[i] = 0.;
    m_tracks_vz[i] = 0.;
    m_tracks_normChi2[i] = 0.;
    m_tracks_dedx[i]   = 0.;
    m_tracks_dedx_n[i] = 0.;
    m_tracks_nhits[i] = 0.;
  }
}


void TrackExtractor::fillTree()
{
  m_tree->Fill(); 
}
 
void TrackExtractor::fillSize(int size)
{
  m_n_tracks=size;
}

int  TrackExtractor::getSize()
{
  return m_n_tracks;
}

