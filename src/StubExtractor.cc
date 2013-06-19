#include "../interface/StubExtractor.h"


StubExtractor::StubExtractor(bool doTree)
{
  m_OK = false;
  n_tot_evt=0;

  // Tree definition
 
  m_clus_x       = new  std::vector<float>;
  m_clus_y       = new  std::vector<float>;
  m_clus_z       = new  std::vector<float>; 
  m_clus_xmc     = new  std::vector<float>; 
  m_clus_ymc     = new  std::vector<float>; 
  m_clus_zmc     = new  std::vector<float>; 
  m_clus_e       = new  std::vector<float>;
  m_clus_layer   = new  std::vector<int>;
  m_clus_module  = new  std::vector<int>;  
  m_clus_ladder  = new  std::vector<int>;  
  m_clus_seg     = new  std::vector<int>; 
  m_clus_strip   = new  std::vector<float>; 
  m_clus_used    = new  std::vector<int>;  
  m_clus_sat     = new  std::vector<int>; 
  m_clus_nstrips = new  std::vector<int>; 
  m_clus_matched = new  std::vector<int>;  
  m_clus_PS      = new  std::vector<int>;  
  m_clus_nrows   = new  std::vector<int>;
  m_clus_pid     = new  std::vector<int>;  
  m_clus_tp      = new  std::vector< std::vector<int> >;  
  m_clus_hits    = new  std::vector< std::vector<int> >; 
  
  m_stub_pt      = new  std::vector<float>;
  m_stub_ptMC    = new  std::vector<float>; 
  m_stub_pxGEN   = new  std::vector<float>; 
  m_stub_pyGEN   = new  std::vector<float>; 
  m_stub_etaGEN  = new  std::vector<float>; 
  m_stub_X0      = new  std::vector<float>; 
  m_stub_Y0      = new  std::vector<float>; 
  m_stub_Z0      = new  std::vector<float>; 
  m_stub_PHI0    = new  std::vector<float>; 
  m_stub_layer   = new  std::vector<int>; 
  m_stub_module  = new  std::vector<int>;  
  m_stub_ladder  = new  std::vector<int>; 
  m_stub_seg     = new  std::vector<int>;  
  m_stub_chip    = new  std::vector<int>;  
  m_stub_strip   = new  std::vector<int>; 
  m_stub_x       = new  std::vector<float>;  
  m_stub_y       = new  std::vector<float>;  
  m_stub_z       = new  std::vector<float>;  
  m_stub_clust1  = new  std::vector<int>;  
  m_stub_clust2  = new  std::vector<int>;  
  m_stub_cw1     = new  std::vector<int>;  
  m_stub_cw2     = new  std::vector<int>;  
  m_stub_deltas  = new  std::vector<float>;  
  m_stub_cor     = new  std::vector<float>;  
  m_stub_tp      = new  std::vector<int>;  
  m_stub_pdg     = new  std::vector<int>;  
  m_stub_pid     = new  std::vector<int>;  

  StubExtractor::reset();

  if (doTree)
  {
    m_OK = true;

    m_tree      = new TTree("TkStubs","Official stub info") ;

    // Branches definition

    m_tree->Branch("L1Tkevt", &n_tot_evt); // Simple evt number or event ID

    // If we don't request only matched stubs, we keep all the info
    // otherwise we skim the data file (useful for BANK generation)

    m_tree->Branch("L1TkCLUS_n",         &m_clus);
    m_tree->Branch("L1TkCLUS_x",         &m_clus_x);
    m_tree->Branch("L1TkCLUS_y",         &m_clus_y);
    m_tree->Branch("L1TkCLUS_z",         &m_clus_z);
    m_tree->Branch("L1TkCLUS_xmc",       &m_clus_xmc);
    m_tree->Branch("L1TkCLUS_ymc",       &m_clus_ymc);
    m_tree->Branch("L1TkCLUS_zmc",       &m_clus_zmc);
    m_tree->Branch("L1TkCLUS_charge",    &m_clus_e);
    m_tree->Branch("L1TkCLUS_layer",     &m_clus_layer);
    m_tree->Branch("L1TkCLUS_module",    &m_clus_module);
    m_tree->Branch("L1TkCLUS_ladder",    &m_clus_ladder);
    m_tree->Branch("L1TkCLUS_seg",       &m_clus_seg);
    m_tree->Branch("L1TkCLUS_strip",     &m_clus_strip);
    m_tree->Branch("L1TkCLUS_nstrip",    &m_clus_nstrips);
    m_tree->Branch("L1TkCLUS_nsat",      &m_clus_sat);
    m_tree->Branch("L1TkCLUS_match",     &m_clus_matched);
    m_tree->Branch("L1TkCLUS_PS",        &m_clus_PS);
    m_tree->Branch("L1TkCLUS_nrows",     &m_clus_nrows);
    // m_tree->Branch("L1TkCLUS_tp",        &m_clus_tp);
    //m_tree->Branch("L1TkCLUS_hits",      &m_clus_hits);
    m_tree->Branch("L1TkCLUS_process",   &m_clus_pid);

    //m_tree->Branch("L1TkSTUB_ptMC",      &m_stub_ptMC);
    m_tree->Branch("L1TkSTUB_clust1",    &m_stub_clust1);
    m_tree->Branch("L1TkSTUB_clust2",    &m_stub_clust2);
    //m_tree->Branch("L1TkSTUB_cw1",       &m_stub_cw1);
    //m_tree->Branch("L1TkSTUB_cw2",       &m_stub_cw2);
    m_tree->Branch("L1TkSTUB_cor",       &m_stub_cor);
    m_tree->Branch("L1TkSTUB_PHI0",      &m_stub_PHI0);
    m_tree->Branch("L1TkSTUB_tp",        &m_stub_tp);
    m_tree->Branch("L1TkSTUB_pdgID",     &m_stub_pdg);
    m_tree->Branch("L1TkSTUB_process",   &m_stub_pid);
    m_tree->Branch("L1TkSTUB_n",         &m_stub);
    //m_tree->Branch("L1TkSTUB_pt",        &m_stub_pt);
    m_tree->Branch("L1TkSTUB_pxGEN",     &m_stub_pxGEN);
    m_tree->Branch("L1TkSTUB_pyGEN",     &m_stub_pyGEN);
    m_tree->Branch("L1TkSTUB_etaGEN",    &m_stub_etaGEN);
    m_tree->Branch("L1TkSTUB_layer",     &m_stub_layer);
    m_tree->Branch("L1TkSTUB_module",    &m_stub_module);
    m_tree->Branch("L1TkSTUB_ladder",    &m_stub_ladder);
    m_tree->Branch("L1TkSTUB_seg",       &m_stub_seg);
    m_tree->Branch("L1TkSTUB_chip",      &m_stub_chip);
    m_tree->Branch("L1TkSTUB_strip",     &m_stub_strip);
    m_tree->Branch("L1TkSTUB_x",         &m_stub_x);
    m_tree->Branch("L1TkSTUB_y",         &m_stub_y);
    m_tree->Branch("L1TkSTUB_z",         &m_stub_z);
    m_tree->Branch("L1TkSTUB_deltas",    &m_stub_deltas);
    m_tree->Branch("L1TkSTUB_X0",        &m_stub_X0);
    m_tree->Branch("L1TkSTUB_Y0",        &m_stub_Y0);
    m_tree->Branch("L1TkSTUB_Z0",        &m_stub_Z0);
  }
}

StubExtractor::StubExtractor(TFile *a_file)
{
  std::cout << "StubExtractor object is retrieved" << std::endl;
 
  m_clus_x       = new  std::vector<float>;
  m_clus_y       = new  std::vector<float>;
  m_clus_z       = new  std::vector<float>; 
  m_clus_xmc     = new  std::vector<float>; 
  m_clus_ymc     = new  std::vector<float>; 
  m_clus_zmc     = new  std::vector<float>; 
  m_clus_e       = new  std::vector<float>;
  m_clus_layer   = new  std::vector<int>;
  m_clus_module  = new  std::vector<int>;  
  m_clus_ladder  = new  std::vector<int>;  
  m_clus_seg     = new  std::vector<int>; 
  m_clus_strip   = new  std::vector<float>; 
  m_clus_used    = new  std::vector<int>;  
  m_clus_sat     = new  std::vector<int>; 
  m_clus_nstrips = new  std::vector<int>; 
  m_clus_matched = new  std::vector<int>;  
  m_clus_PS      = new  std::vector<int>;  
  m_clus_nrows   = new  std::vector<int>;
  m_clus_pid     = new  std::vector<int>;  
  m_clus_tp      = new  std::vector< std::vector<int> >;  
  m_clus_hits    = new  std::vector< std::vector<int> >; 
  
  m_stub_pt      = new  std::vector<float>;
  m_stub_ptMC    = new  std::vector<float>; 
  m_stub_pxGEN   = new  std::vector<float>; 
  m_stub_pyGEN   = new  std::vector<float>; 
  m_stub_etaGEN  = new  std::vector<float>; 
  m_stub_X0      = new  std::vector<float>; 
  m_stub_Y0      = new  std::vector<float>; 
  m_stub_Z0      = new  std::vector<float>; 
  m_stub_PHI0    = new  std::vector<float>; 
  m_stub_layer   = new  std::vector<int>; 
  m_stub_module  = new  std::vector<int>;  
  m_stub_ladder  = new  std::vector<int>; 
  m_stub_seg     = new  std::vector<int>;  
  m_stub_strip   = new  std::vector<int>; 
  m_stub_chip    = new  std::vector<int>; 
  m_stub_x       = new  std::vector<float>;  
  m_stub_y       = new  std::vector<float>;  
  m_stub_z       = new  std::vector<float>;  
  m_stub_clust1  = new  std::vector<int>;  
  m_stub_clust2  = new  std::vector<int>;  
  m_stub_cw1     = new  std::vector<int>;  
  m_stub_cw2     = new  std::vector<int>;  
  m_stub_deltas  = new  std::vector<float>;  
  m_stub_cor     = new  std::vector<float>;  
  m_stub_tp      = new  std::vector<int>;  
  m_stub_pdg     = new  std::vector<int>;  
  m_stub_pid     = new  std::vector<int>; 

  // Tree definition
  m_OK = false;


  StubExtractor::reset();

  m_tree = dynamic_cast<TTree*>(a_file->Get("TkStubs"));

  if (!m_tree)
  {
    std::cout << "This tree (TkStubs) doesn't exist!!!" << std::endl;
    return;
  }

  m_OK = true;
  m_matching = false;

  m_n_events = m_tree->GetEntries();

  m_tree->SetBranchAddress("L1Tkevt", &n_tot_evt); // Simple evt number or event ID
  
  // If we don't request only matched stubs, we keep all the info
  // otherwise we skim the data file (useful for BANK generation)
  
  m_tree->SetBranchAddress("L1TkCLUS_n",         &m_clus);
  m_tree->SetBranchAddress("L1TkCLUS_x",         &m_clus_x);
  m_tree->SetBranchAddress("L1TkCLUS_y",         &m_clus_y);
  m_tree->SetBranchAddress("L1TkCLUS_z",         &m_clus_z);
  m_tree->SetBranchAddress("L1TkCLUS_xmc",       &m_clus_xmc);
  m_tree->SetBranchAddress("L1TkCLUS_ymc",       &m_clus_ymc);
  m_tree->SetBranchAddress("L1TkCLUS_zmc",       &m_clus_zmc);
  m_tree->SetBranchAddress("L1TkCLUS_charge",    &m_clus_e);
  m_tree->SetBranchAddress("L1TkCLUS_layer",     &m_clus_layer);
  m_tree->SetBranchAddress("L1TkCLUS_module",    &m_clus_module);
  m_tree->SetBranchAddress("L1TkCLUS_ladder",    &m_clus_ladder);
  m_tree->SetBranchAddress("L1TkCLUS_seg",       &m_clus_seg);
  m_tree->SetBranchAddress("L1TkCLUS_strip",     &m_clus_strip);
  m_tree->SetBranchAddress("L1TkCLUS_nstrip",    &m_clus_nstrips);
  m_tree->SetBranchAddress("L1TkCLUS_nsat",      &m_clus_sat);
  m_tree->SetBranchAddress("L1TkCLUS_match",     &m_clus_matched);
  m_tree->SetBranchAddress("L1TkCLUS_PS",        &m_clus_PS);
  m_tree->SetBranchAddress("L1TkCLUS_nrows",     &m_clus_nrows);
  //  m_tree->SetBranchAddress("L1TkCLUS_tp",        &m_clus_tp);
  // m_tree->SetBranchAddress("L1TkCLUS_hits",      &m_clus_hits);
  m_tree->SetBranchAddress("L1TkCLUS_process",   &m_clus_pid);
  
  //m_tree->SetBranchAddress("L1TkSTUB_ptMC",      &m_stub_ptMC);
  m_tree->SetBranchAddress("L1TkSTUB_clust1",    &m_stub_clust1);
  m_tree->SetBranchAddress("L1TkSTUB_clust2",    &m_stub_clust2);
  //  m_tree->SetBranchAddress("L1TkSTUB_cw1",       &m_stub_cw1);
  //  m_tree->SetBranchAddress("L1TkSTUB_cw2",       &m_stub_cw2);
  m_tree->SetBranchAddress("L1TkSTUB_cor",       &m_stub_cor);
  m_tree->SetBranchAddress("L1TkSTUB_PHI0",      &m_stub_PHI0);
  m_tree->SetBranchAddress("L1TkSTUB_tp",        &m_stub_tp);
  m_tree->SetBranchAddress("L1TkSTUB_pdgID",     &m_stub_pdg);
  m_tree->SetBranchAddress("L1TkSTUB_process",   &m_stub_pid);
  m_tree->SetBranchAddress("L1TkSTUB_n",         &m_stub);
  //  m_tree->SetBranchAddress("L1TkSTUB_pt",        &m_stub_pt);
  m_tree->SetBranchAddress("L1TkSTUB_pxGEN",     &m_stub_pxGEN);
  m_tree->SetBranchAddress("L1TkSTUB_pyGEN",     &m_stub_pyGEN);
  m_tree->SetBranchAddress("L1TkSTUB_etaGEN",    &m_stub_etaGEN);
  m_tree->SetBranchAddress("L1TkSTUB_layer",     &m_stub_layer);
  m_tree->SetBranchAddress("L1TkSTUB_module",    &m_stub_module);
  m_tree->SetBranchAddress("L1TkSTUB_ladder",    &m_stub_ladder);
  m_tree->SetBranchAddress("L1TkSTUB_seg",       &m_stub_seg);
  m_tree->SetBranchAddress("L1TkSTUB_strip",     &m_stub_strip);
  m_tree->SetBranchAddress("L1TkSTUB_chip",      &m_stub_chip);
  m_tree->SetBranchAddress("L1TkSTUB_x",         &m_stub_x);
  m_tree->SetBranchAddress("L1TkSTUB_y",         &m_stub_y);
  m_tree->SetBranchAddress("L1TkSTUB_z",         &m_stub_z);
  m_tree->SetBranchAddress("L1TkSTUB_deltas",    &m_stub_deltas);
  m_tree->SetBranchAddress("L1TkSTUB_X0",        &m_stub_X0);
  m_tree->SetBranchAddress("L1TkSTUB_Y0",        &m_stub_Y0);
  m_tree->SetBranchAddress("L1TkSTUB_Z0",        &m_stub_Z0);

  std::cout << "This file contains " << m_n_events << " events..." << std::endl;

}



StubExtractor::~StubExtractor()
{}


void StubExtractor::init(const edm::EventSetup *setup)
{
  setup->get<TrackerDigiGeometryRecord>().get(theTrackerGeometry);
  setup->get<StackedTrackerGeometryRecord>().get(theStackedTrackerGeometry);
  theStackedGeometry = theStackedTrackerGeometry.product(); 

}

//
// Method filling the main particle tree
//

void StubExtractor::writeInfo(const edm::Event *event, MCExtractor *mc) 
{
  StubExtractor::reset();
  ++n_tot_evt;    

  mc->clearTP(0.001,10000000.0);
  /// Sim Tracks and Vtx
  event->getByLabel( "g4SimHits", SimTrackHandle );
  event->getByLabel( "g4SimHits", SimVtxHandle );

  /// Track Trigger
  event->getByLabel( "L1TkClustersFromPixelDigis",             PixelDigiL1TkClusterHandle );
  event->getByLabel( "L1TkStubsFromPixelDigis", "StubsPass",   PixelDigiL1TkStubHandle );
  event->getByLabel( "L1TkStubsFromPixelDigis", "StubsFail",   PixelDigiL1TkFailedStubHandle );

  int layer  = 0;
  int ladder = 0;
  int module = 0;
  int segs   = 0;
  int rows   = 0;
  int disk   = 0;
  int lad_cor= 0;

  /// Go on only if there are L1TkCluster from PixelDigis
  if ( PixelDigiL1TkClusterHandle->size() > 0 )
  {
    /// Loop over L1TkClusters
    L1TkCluster_PixelDigi_Collection::const_iterator iterL1TkCluster;
    for ( iterL1TkCluster = PixelDigiL1TkClusterHandle->begin();
          iterL1TkCluster != PixelDigiL1TkClusterHandle->end();
          ++iterL1TkCluster )
    {
      StackedTrackerDetId detIdClu( iterL1TkCluster->getDetId() );
      GlobalPoint posClu      = theStackedGeometry->findAverageGlobalPosition( &(*iterL1TkCluster) );
      MeasurementPoint coords = iterL1TkCluster->findAverageLocalCoordinates();
      int    stack            = iterL1TkCluster->getStackMember();


      //      hCluster_RZ->Fill( posClu.z(), posClu.perp() );
      /*
      if (iterL1TkCluster->findSimTrackId()==0)     
      {
	std::cout << "EVT : "  << n_tot_evt << std::endl;
	std::cout << "Cluster width : " << iterL1TkCluster->findWidth() << std::endl; 
	std::cout << "Cluster pos : " 
		  << posClu.x() << " / " 
		  << posClu.y() << " / " 
		  << posClu.z() << std::endl;
	std::cout << "Row : "  << coords.x() << std::endl;
	std::cout << "Column : "  << coords.y() << std::endl;
	std::cout << "ID : "  << iterL1TkCluster->findSimTrackId()  << std::endl;
      }
      */
      //      std::cout << "EVT : "  << n_tot_evt << std::endl;

      ++m_clus;

      m_clus_x->push_back(posClu.x());
      m_clus_y->push_back(posClu.y());
      m_clus_z->push_back(posClu.z());

      m_clus_seg->push_back(coords.y());
      m_clus_strip->push_back(coords.x());
      m_clus_nstrips->push_back(iterL1TkCluster->findWidth());


      segs=2;

      if ( detIdClu.isBarrel() )
      {
	layer  = detIdClu.iLayer()+4;
	ladder = detIdClu.iPhi();
	module = detIdClu.iZ()*2-1+stack;
	if (layer<8 && stack==0) segs=32;	
      }
      else if ( detIdClu.isEndcap() )
      {	
	layer  = 10+detIdClu.iZ()+abs(detIdClu.iSide()-2)*7;
	ladder = detIdClu.iRing();
	module = detIdClu.iPhi()*2-1+stack;
	if (ladder<9 && stack==0) segs=32;
      }

      //std::cout << layer << "/" << ladder << "/" << module << "/" << stack << std::endl;

      m_clus_layer->push_back(layer);
      m_clus_ladder->push_back(ladder);
      m_clus_module->push_back(module);
      m_clus_PS->push_back(segs);

      m_clus_xmc->push_back(0);
      m_clus_ymc->push_back(0);
      m_clus_zmc->push_back(0);
      m_clus_sat->push_back(0);
      m_clus_matched->push_back(0);
      m_clus_nrows->push_back(0);
     
    } /// End of Loop over L1TkClusters
  } /// End of if ( PixelDigiL1TkClusterHandle->size() > 0 )

  int hit       = -1; // 
  int tp        = -1; // 

  int clust1     = -1;
  int clust2     = -1;

  /// Go on only if there are L1TkStub from PixelDigis
  if ( PixelDigiL1TkStubHandle->size() > 0 )
  {
    /// Loop over L1TkStubs
    L1TkStub_PixelDigi_Collection::const_iterator iterL1TkStub;
    for ( iterL1TkStub = PixelDigiL1TkStubHandle->begin();
          iterL1TkStub != PixelDigiL1TkStubHandle->end();
          ++iterL1TkStub )
    {
      StackedTrackerDetId detIdStub( iterL1TkStub->getDetId() );

      // The stub position is the inner cluster position
      //
      // See: http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/Geometry/TrackerGeometryBuilder/interface/StackedTrackerGeometry.h?view=markup
      //
      GlobalPoint posStub = theStackedGeometry->findGlobalPosition( &(*iterL1TkStub) );
      //MeasurementPoint coords =  iterL1TkStub->findLocalCoordinates();

      ++m_stub;

      double displStub    = iterL1TkStub->getTriggerDisplacement();
      double offsetStub   = iterL1TkStub->getTriggerOffset();

      const GeomDetUnit* det0 = theStackedGeometry->idToDetUnit( detIdStub, 0 );

      /// Find pixel pitch and topology related information
      const PixelGeomDetUnit* pix0 = dynamic_cast< const PixelGeomDetUnit* >( det0 );
      const PixelTopology* top0    = dynamic_cast< const PixelTopology* >( &(pix0->specificTopology()) );

      segs = top0->ncolumns();
      rows = top0->nrows();

      clust1 = StubExtractor::getClust1Idx(posStub.x(),posStub.y(),posStub.z());
      clust2 = StubExtractor::getClust2Idx(clust1,displStub);

      m_stub_x->push_back(posStub.x());
      m_stub_y->push_back(posStub.y());
      m_stub_z->push_back(posStub.z());
      m_stub_clust1->push_back(clust1);
      m_stub_clust2->push_back(clust2);
      m_stub_seg->push_back(m_clus_seg->at(clust1));
      m_stub_chip->push_back(m_clus_strip->at(clust1)/(rows/8));
      m_stub_strip->push_back(m_clus_strip->at(clust1));
      m_stub_deltas->push_back(offsetStub-displStub);
      m_stub_cor->push_back(offsetStub);

      hit        = -1; // 
      tp         = -1; // 

      if ( detIdStub.isBarrel() )
      {
	layer  = detIdStub.iLayer()+4;
	ladder = detIdStub.iPhi();
	module = detIdStub.iZ()*2-1;
      }
      else if ( detIdStub.isEndcap() )
      {	
	layer  = 10+detIdStub.iZ()+abs(detIdStub.iSide()-2)*7;

	if (layer>10 && layer<=17) disk=(layer-10)%8;
	if (layer>17 && layer<=24) disk=(layer-17)%8;
	if (disk>=5) lad_cor = (disk-4)%4;

	ladder = detIdStub.iRing()-1+lad_cor;
	module = detIdStub.iPhi()*2-1;
      }

      m_stub_layer->push_back(layer);
      m_stub_ladder->push_back(ladder);
      m_stub_module->push_back(module);


      if (iterL1TkStub->findSimTrackId()!=0)
      {
	//std::cout << "Try to match stub " << m_stub << " with simtrack " << iterL1TkStub->findSimTrackId() << std::endl;
    
	mc->findMatchingTP(iterL1TkStub->findSimTrackId(),
			   layer,ladder,module,
			   segs,
			   posStub.x(),
			   posStub.y(),
			   posStub.z(),
			   tp,hit,false);
	
	//std::cout << "Matched with hit " << hit << " from TP " << tp << std::endl;
	
	m_stub_tp->push_back(tp);



      }
      else
      {
	//std::cout << "No simtrack ID for stub " << iterL1TkStub->findSimTrackId() << std::endl;
      	m_stub_tp->push_back(-2);
      }


      if (tp!=-1)
      {
	m_stub_pxGEN->push_back(mc->getTP_px(tp));
	m_stub_pyGEN->push_back(mc->getTP_py(tp));
	m_stub_etaGEN->push_back(asinh(mc->getTP_pz(tp)/sqrt(mc->getTP_px(tp)*mc->getTP_px(tp)
								      +mc->getTP_py(tp)*mc->getTP_py(tp))));
	m_stub_pdg->push_back(mc->getTP_ID(tp));
	//	m_stub_pid->push_back(m_clus_pid->at(i_bs));
	m_stub_pid->push_back(0);
	m_stub_X0->push_back(mc->getTP_x(tp));
	m_stub_Y0->push_back(mc->getTP_y(tp));
	m_stub_Z0->push_back(mc->getTP_z(tp));
	m_stub_PHI0->push_back(atan2(mc->getTP_py(tp),mc->getTP_px(tp)));
      }
      else
      {
	m_stub_pxGEN->push_back(0);
	m_stub_pyGEN->push_back(0);
	m_stub_etaGEN->push_back(0);
	m_stub_X0->push_back(0);
	m_stub_Y0->push_back(0);
	m_stub_Z0->push_back(0);
	m_stub_PHI0->push_back(0);
	m_stub_pdg->push_back(0);
	m_stub_pid->push_back(0);
      }


    } /// End of loop over L1TkStubs
  } /// End of if ( PixelDigiL1TkStubHandle->size() > 0 )



  StubExtractor::fillTree();
}


//
// Method getting the info from an input file
//

void StubExtractor::getInfo(int ievt) 
{
  m_tree->GetEntry(ievt); 
}

// Method initializing everything (to do for each event)

void StubExtractor::reset()
{
  m_clus = 0;
  m_stub = 0;

  m_clus_x->clear(); 
  m_clus_y->clear(); 
  m_clus_z->clear(); 
  m_clus_xmc->clear();
  m_clus_ymc->clear();
  m_clus_zmc->clear();
  m_clus_e->clear(); 
  m_clus_layer->clear(); 
  m_clus_module->clear();
  m_clus_ladder->clear();
  m_clus_seg->clear();   
  m_clus_strip->clear(); 
  m_clus_sat->clear();   
  m_clus_nstrips->clear();
  m_clus_used->clear();   
  m_clus_matched->clear();
  m_clus_PS->clear();
  m_clus_nrows->clear();
  m_clus_pid->clear();

  m_clus_tp->clear();
  m_clus_hits->clear();
  
  m_stub_X0->clear();     
  m_stub_Y0->clear();     
  m_stub_Z0->clear();     
  m_stub_PHI0->clear();     
  m_stub_tp->clear();     
  m_stub_pt->clear();     
  m_stub_ptMC->clear();   
  m_stub_pxGEN->clear();  
  m_stub_pyGEN->clear();  
  m_stub_etaGEN->clear();  
  m_stub_layer->clear();  
  m_stub_module->clear(); 
  m_stub_ladder->clear(); 
  m_stub_seg->clear();  
  m_stub_chip->clear();   
  m_stub_strip->clear(); 
  m_stub_x->clear(); 
  m_stub_y->clear(); 
  m_stub_z->clear(); 
  m_stub_clust1->clear(); 
  m_stub_clust2->clear(); 
  m_stub_cw1->clear(); 
  m_stub_cw2->clear(); 
  m_stub_deltas->clear(); 
  m_stub_cor->clear(); 
  m_stub_pdg->clear();
  m_stub_pid->clear();


}


void StubExtractor::fillTree()
{
  m_tree->Fill(); 
}
 
void StubExtractor::fillSize(int size)
{
  m_clus=size;
}

int  StubExtractor::getSize()
{
  return m_clus;
}

int  StubExtractor::getClust1Idx(float x, float y, float z)
{
  for (int i=0;i<m_clus;++i) // Loop over clusters
  { 
    if (fabs(m_clus_x->at(i)-x) > 0.001) continue;
    if (fabs(m_clus_y->at(i)-y) > 0.001) continue;
    if (fabs(m_clus_z->at(i)-z) > 0.001) continue;

    return i;
  }

  std::cout << "STRANGE: a stub without matching 1 cluster..." << std::endl;

  return -1;
}

int  StubExtractor::getClust2Idx(int idx1, float dist)
{
  for (int i=0;i<m_clus;++i) // Loop over clusters
  { 
    if (m_clus_layer->at(i)!=m_clus_layer->at(idx1)) continue;
    if (m_clus_ladder->at(i)!=m_clus_ladder->at(idx1)) continue;
    if (m_clus_module->at(i)-1!=m_clus_module->at(idx1)) continue;

    //    std::cout  << fabs(m_clus_strip->at(i)-m_clus_strip->at(idx1)) << " / " << dist << std::endl;

    if (m_clus_strip->at(i)-m_clus_strip->at(idx1)!=dist) continue;

    return i;
  }

  std::cout << "STRANGE: a stub without matching 2 cluster..." << std::endl;

  return -1;
}
