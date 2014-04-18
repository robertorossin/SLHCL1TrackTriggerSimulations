#include "../interface/L1TrackExtractor.h"


L1TrackExtractor::L1TrackExtractor(edm::InputTag STUB_tag, edm::InputTag PATT_tag, edm::InputTag TRCK_tag, bool doTree)
{
  m_OK = false;
  n_tot_evt=0;

  m_STUB_tag   = STUB_tag;
  m_PATT_tag   = PATT_tag;
  m_TRCK_tag   = TRCK_tag;

  // Tree definition
 
  m_patt_links   = new  std::vector< std::vector<int> >;
  m_patt_secid   = new  std::vector<int>;
  
  m_trk_pt       = new  std::vector<float>;
  m_trk_eta      = new  std::vector<float>;
  m_trk_phi      = new  std::vector<float>;
  m_trk_z        = new  std::vector<float>;
  m_trk_links    = new  std::vector< std::vector<int> >;
  m_trk_secid    = new  std::vector<int>;

  L1TrackExtractor::reset();

  if (doTree)
  {
    m_OK = true;

    m_tree      = new TTree("L1tracks","Official L1-AM tracks info") ;

    // Branches definition

    m_tree->Branch("L1evt", &n_tot_evt); // Simple evt number or event ID

    m_tree->Branch("L1PATT_n",           &m_patt);
    m_tree->Branch("L1PATT_links",       &m_patt_links);
    m_tree->Branch("L1PATT_secid",       &m_patt_secid);

    m_tree->Branch("L1TRK_n",            &m_trk);
    m_tree->Branch("L1TRK_links",        &m_trk_links);
    m_tree->Branch("L1TRK_secid",        &m_trk_secid);
    m_tree->Branch("L1TRK_pt",           &m_trk_pt);
    m_tree->Branch("L1TRK_phi",          &m_trk_phi);
    m_tree->Branch("L1TRK_z",            &m_trk_z);
    m_tree->Branch("L1TRK_eta",          &m_trk_eta);
  }
}

L1TrackExtractor::L1TrackExtractor(TFile *a_file)
{
  std::cout << "L1TrackExtractor object is retrieved" << std::endl;
 
  m_patt_links   = new  std::vector< std::vector<int> >;
  m_patt_secid   = new  std::vector<int>;
  
  m_trk_pt       = new  std::vector<float>;
  m_trk_eta      = new  std::vector<float>;
  m_trk_phi      = new  std::vector<float>;
  m_trk_z        = new  std::vector<float>;
  m_trk_links    = new  std::vector< std::vector<int> >;
  m_trk_secid    = new  std::vector<int>;

  // Tree definition
  m_OK = false;


  L1TrackExtractor::reset();

  m_tree = dynamic_cast<TTree*>(a_file->Get("L1tracks"));

  if (!m_tree)
  {
    std::cout << "This tree (L1tracks) doesn't exist!!!" << std::endl;
    return;
  }

  m_OK = true;

  m_n_events = m_tree->GetEntries();

  m_tree->SetBranchAddress("L1evt", &n_tot_evt); // Simple evt number or event ID
  
  m_tree->SetBranchAddress("L1PATT_n",           &m_patt);
  m_tree->SetBranchAddress("L1PATT_links",       &m_patt_links);
  m_tree->SetBranchAddress("L1PATT_secid",       &m_patt_secid);
  
  m_tree->SetBranchAddress("L1TRK_n",            &m_trk);
  m_tree->SetBranchAddress("L1TRK_links",        &m_trk_links);
  m_tree->SetBranchAddress("L1TRK_secid",        &m_trk_secid);
  m_tree->SetBranchAddress("L1TRK_pt",           &m_trk_pt);
  m_tree->SetBranchAddress("L1TRK_phi",          &m_trk_phi);
  m_tree->SetBranchAddress("L1TRK_z",            &m_trk_z);
  m_tree->SetBranchAddress("L1TRK_eta",          &m_trk_eta);
  
  std::cout << "This file contains " << m_n_events << " events..." << std::endl;
}



L1TrackExtractor::~L1TrackExtractor()
{}


void L1TrackExtractor::init(const edm::EventSetup *setup)
{
  setup->get<TrackerDigiGeometryRecord>().get(theTrackerGeometry);
  setup->get<StackedTrackerGeometryRecord>().get(theStackedTrackerGeometry);
  theStackedGeometry = theStackedTrackerGeometry.product(); 

  /// Magnetic Field
  edm::ESHandle< MagneticField > magneticFieldHandle;
  setup->get< IdealMagneticFieldRecord >().get(magneticFieldHandle);
  const MagneticField* theMagneticField = magneticFieldHandle.product();
  mMagneticFieldStrength = theMagneticField->inTesla(GlobalPoint(0,0,0)).z();

}

//
// Method filling the main particle tree
//

void L1TrackExtractor::writeInfo(const edm::Event *event, StubExtractor *stub) 
{
  L1TrackExtractor::reset();
  ++n_tot_evt;    


  /// Get the Stubs already stored away
  edm::Handle< edmNew::DetSetVector< TTStub< Ref_PixelDigi_ > > > TTStubHandle;
  edm::Handle< std::vector< TTTrack< Ref_PixelDigi_ > > > TTPatternHandle;
  edm::Handle< std::vector< TTTrack< Ref_PixelDigi_ > > > TTTrackHandle;

  event->getByLabel( m_STUB_tag, TTStubHandle );
  event->getByLabel( m_PATT_tag, TTPatternHandle );

  if (m_TRCK_tag.label()!="" && m_TRCK_tag.instance()!="") // Do this to run extractor only on PR info
    event->getByLabel( m_TRCK_tag, TTTrackHandle );

  int layer  = 0;
  int ladder = 0;
  int module = 0;
  std::vector<int> stub_list;

  /// STEP 1
  /// Loop over patterns

  edmNew::DetSetVector< TTStub< Ref_PixelDigi_ > >::const_iterator inputIter;
  edmNew::DetSet< TTStub< Ref_PixelDigi_ > >::const_iterator stubIter;

  std::vector< TTTrack< Ref_PixelDigi_ > >::const_iterator iterTTTrack;
  
  /// Go on only if there are Patterns from PixelDigis
  if ( TTPatternHandle->size() > 0 )
  {
    /// Loop over Patterns
    unsigned int patCnt = 0;

    for ( iterTTTrack = TTPatternHandle->begin();
	  iterTTTrack != TTPatternHandle->end();
	  ++iterTTTrack )
    {
      edm::Ptr< TTTrack< Ref_PixelDigi_ > > tempTrackPtr( TTPatternHandle, patCnt++ );

      /// Get everything is relevant

      std::vector< edm::Ref< edmNew::DetSetVector< TTStub< Ref_PixelDigi_  > >, TTStub< Ref_PixelDigi_  > > > trackStubs = tempTrackPtr->getStubRefs();

      // Loop over stubs contained in the pattern to recover the info
      // and match them with the stubs in the STUB_extractor container

      m_patt_secid->push_back(tempTrackPtr->getSector());

      stub_list.clear();

      for(unsigned int i=0;i<trackStubs.size();i++)
      {

	edm::Ref< edmNew::DetSetVector< TTStub< Ref_PixelDigi_ > >, TTStub< Ref_PixelDigi_ > > tempStubRef = trackStubs.at(i);

	/// Calculate average coordinates col/row for inner/outer Cluster
	/// These are already corrected for being at the center of each pixel
	//	MeasurementPoint mp0 = tempStubRef->getClusterRef(0)->findAverageLocalCoordinates();
	GlobalPoint posStub  = theStackedGeometry->findGlobalPosition( &(*tempStubRef) );
	
	StackedTrackerDetId detIdStub( tempStubRef->getDetId() );
	
	// Here we rearrange the numbers
	if ( detIdStub.isBarrel() )
	{
	  layer  = detIdStub.iLayer()+4;
	  ladder = detIdStub.iPhi()-1;
	  module = detIdStub.iZ()-1;
	}
	else if ( detIdStub.isEndcap() )
	{
	  layer  = 10+detIdStub.iZ()+abs(detIdStub.iSide()-2)*7;
	  ladder = detIdStub.iRing()-1;
	  module = detIdStub.iPhi()-1;
	}

	stub_list.push_back(stub->get_id(layer,ladder,module,posStub.x(),posStub.y(),posStub.z()));


      } /// End of loop over track stubs

      m_patt_links->push_back(stub_list);


    } // End of loop over patterns

    m_patt =  patCnt;
  }


  /// STEP 2
  /// Loop over tracks

  
  /// Go on only if there are Tracks from Patterns


  if (m_TRCK_tag.label()!="" && m_TRCK_tag.instance()!="")
  {
  if ( TTTrackHandle->size() > 0 )
  {
    /// Loop over Patterns
    unsigned int tkCnt = 0;

    for ( iterTTTrack = TTTrackHandle->begin();
	  iterTTTrack != TTTrackHandle->end();
	  ++iterTTTrack )
    {
      edm::Ptr< TTTrack< Ref_PixelDigi_ > > tempTrackPtr( TTTrackHandle, tkCnt++ );

      /// Get everything is relevant

      std::vector< edm::Ref< edmNew::DetSetVector< TTStub< Ref_PixelDigi_  > >, TTStub< Ref_PixelDigi_  > > > trackStubs = tempTrackPtr->getStubRefs();

      // Loop over stubs contained in the pattern to recover the info
      // and match them with the stubs in the STUB_extractor container

      m_trk_secid->push_back(tempTrackPtr->getSector());
      m_trk_pt->push_back(tempTrackPtr->getMomentum().perp() );
      m_trk_eta->push_back(tempTrackPtr->getMomentum().eta());
      m_trk_phi->push_back(tempTrackPtr->getMomentum().phi());
      m_trk_z->push_back(tempTrackPtr->getPOCA().z());

      //      std::cout << tempTrackPtr->getMomentum().perp() << " / " 
      //		<< tempTrackPtr->getMomentum().eta() << " / " 
      //		<< tempTrackPtr->getPOCA().z() << std::endl;
      
      stub_list.clear();

      for(unsigned int i=0;i<trackStubs.size();i++)
      {

	edm::Ref< edmNew::DetSetVector< TTStub< Ref_PixelDigi_ > >, TTStub< Ref_PixelDigi_ > > tempStubRef = trackStubs.at(i);

	/// Calculate average coordinates col/row for inner/outer Cluster
	/// These are already corrected for being at the center of each pixel
	//	MeasurementPoint mp0 = tempStubRef->getClusterRef(0)->findAverageLocalCoordinates();
	GlobalPoint posStub  = theStackedGeometry->findGlobalPosition( &(*tempStubRef) );
	
	StackedTrackerDetId detIdStub( tempStubRef->getDetId() );
	
	// Here we rearrange the numbers
	if ( detIdStub.isBarrel() )
	{
	  layer  = detIdStub.iLayer()+4;
	  ladder = detIdStub.iPhi()-1;
	  module = detIdStub.iZ()-1;
	}
	else if ( detIdStub.isEndcap() )
	{
	  layer  = 10+detIdStub.iZ()+abs(detIdStub.iSide()-2)*7;
	  ladder = detIdStub.iRing()-1;
	  module = detIdStub.iPhi()-1;
	}

	//	std::cout << layer << "//" << ladder << "//" << module << std::endl;
	stub_list.push_back(stub->get_id(layer,ladder,module,posStub.x(),posStub.y(),posStub.z()));


      } /// End of loop over track stubs

      m_trk_links->push_back(stub_list);


    } // End of loop over patterns

    m_trk =  tkCnt;
  }
  }      
 
  L1TrackExtractor::fillTree();
}


//
// Method getting the info from an input file
//

void L1TrackExtractor::getInfo(int ievt) 
{
  m_tree->GetEntry(ievt); 
}

// Method initializing everything (to do for each event)

void L1TrackExtractor::reset()
{
  m_patt = 0;
  m_trk  = 0;
 
  m_patt_links->clear(); 
  m_patt_secid->clear(); 
  
  m_trk_pt->clear();    
  m_trk_eta->clear();   
  m_trk_phi->clear();   
  m_trk_z->clear();     
  m_trk_links->clear(); 
  m_trk_secid->clear(); 
}


void L1TrackExtractor::fillTree()
{
  m_tree->Fill(); 
}
 
void L1TrackExtractor::fillSize(int size)
{
  m_patt=size;
}

int  L1TrackExtractor::getSize()
{
  return m_patt;
}

