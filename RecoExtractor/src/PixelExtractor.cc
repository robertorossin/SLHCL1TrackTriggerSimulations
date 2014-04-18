
#include "../interface/PixelExtractor.h"


PixelExtractor::PixelExtractor(edm::InputTag tag, bool doTree, bool doMatch)
{
  m_OK = false;
  m_tag = tag;
  

  m_matching = doMatch;

  // Tree definition

  m_pixclus_x        = new std::vector<float>;    
  m_pixclus_y        = new std::vector<float>;  
  m_pixclus_z        = new std::vector<float>;    
  m_pixclus_e        = new std::vector<float>;    
  m_pixclus_row      = new std::vector<int>;      
  m_pixclus_column   = new std::vector<int>;      
  m_pixclus_simhit   = new std::vector<int>;      
  m_pixclus_simhitID = new std::vector< std::vector<int> >;  
  m_pixclus_evtID    = new std::vector< std::vector<int> >;  

  m_pixclus_layer    = new std::vector<int>;    
  m_pixclus_module   = new std::vector<int>; 
  m_pixclus_ladder   = new std::vector<int>;    
  m_pixclus_nrow     = new std::vector<int>;    
  m_pixclus_ncolumn  = new std::vector<int>;  
  m_pixclus_pitchx   = new std::vector<float>;  
  m_pixclus_pitchy   = new std::vector<float>; 

  PixelExtractor::reset();

  if (doTree)
  {
    m_OK = true;

    m_tree      = new TTree("Pixels","RECO Pixel info") ;

    // Branches definition

    m_tree->Branch("PIX_n",         &m_pclus,    "PIX_n/I");
    m_tree->Branch("PIX_nPU",       &m_nPU,      "PIX_nPU/I");

    m_tree->Branch("PIX_x",         &m_pixclus_x);
    m_tree->Branch("PIX_y",         &m_pixclus_y);
    m_tree->Branch("PIX_z",         &m_pixclus_z);
    m_tree->Branch("PIX_charge",    &m_pixclus_e);
    m_tree->Branch("PIX_row",       &m_pixclus_row);
    m_tree->Branch("PIX_column",    &m_pixclus_column);
    m_tree->Branch("PIX_simhit",    &m_pixclus_simhit);
    m_tree->Branch("PIX_simhitID",  &m_pixclus_simhitID);
    m_tree->Branch("PIX_evtID",     &m_pixclus_evtID);
    m_tree->Branch("PIX_layer",     &m_pixclus_layer);
    m_tree->Branch("PIX_module",    &m_pixclus_module);
    m_tree->Branch("PIX_ladder",    &m_pixclus_ladder);
    m_tree->Branch("PIX_nrow",      &m_pixclus_nrow);
    m_tree->Branch("PIX_ncolumn",   &m_pixclus_ncolumn);
    m_tree->Branch("PIX_pitchx",    &m_pixclus_pitchx);
    m_tree->Branch("PIX_pitchy",    &m_pixclus_pitchy);
  }
}

PixelExtractor::PixelExtractor(TFile *a_file)
{
  std::cout << "PixelExtractor object is retrieved" << std::endl;

  // Tree definition
  m_OK = false;

  m_pixclus_x        = new std::vector<float>;    
  m_pixclus_y        = new std::vector<float>;  
  m_pixclus_z        = new std::vector<float>;    
  m_pixclus_e        = new std::vector<float>;    
  m_pixclus_row      = new std::vector<int>;      
  m_pixclus_column   = new std::vector<int>;      
  m_pixclus_simhit   = new std::vector<int>;      
  m_pixclus_simhitID = new std::vector< std::vector<int> >;  
  m_pixclus_evtID    = new std::vector< std::vector<int> >; 
  m_pixclus_layer    = new std::vector<int>;    
  m_pixclus_module   = new std::vector<int>; 
  m_pixclus_ladder   = new std::vector<int>;    
  m_pixclus_nrow     = new std::vector<int>;    
  m_pixclus_ncolumn  = new std::vector<int>;  
  m_pixclus_pitchx   = new std::vector<float>;  
  m_pixclus_pitchy   = new std::vector<float>; 

  PixelExtractor::reset();

  m_tree = dynamic_cast<TTree*>(a_file->Get("Pixels"));

  if (!m_tree)
  {
    std::cout << "This tree (Pixels) doesn't exist!!!" << std::endl;
    return;
  }

  m_OK = true;
  m_matching = false;

  m_n_events = m_tree->GetEntries();

  std::cout << "This file contains " << m_n_events << " events..." << std::endl;

  m_tree->SetBranchAddress("PIX_n",         &m_pclus);
  m_tree->SetBranchAddress("PIX_nPU",       &m_nPU);

  m_tree->SetBranchAddress("PIX_x",         &m_pixclus_x);
  m_tree->SetBranchAddress("PIX_y",         &m_pixclus_y);
  m_tree->SetBranchAddress("PIX_z",         &m_pixclus_z);
  m_tree->SetBranchAddress("PIX_charge",    &m_pixclus_e);
  m_tree->SetBranchAddress("PIX_row",       &m_pixclus_row);
  m_tree->SetBranchAddress("PIX_column",    &m_pixclus_column);
  m_tree->SetBranchAddress("PIX_simhit",    &m_pixclus_simhit);
  m_tree->SetBranchAddress("PIX_simhitID",  &m_pixclus_simhitID);
  m_tree->SetBranchAddress("PIX_evtID",     &m_pixclus_evtID);
  m_tree->SetBranchAddress("PIX_layer",     &m_pixclus_layer);
  m_tree->SetBranchAddress("PIX_module",    &m_pixclus_module);
  m_tree->SetBranchAddress("PIX_ladder",    &m_pixclus_ladder);
  m_tree->SetBranchAddress("PIX_nrow",      &m_pixclus_nrow);
  m_tree->SetBranchAddress("PIX_ncolumn",   &m_pixclus_ncolumn);
}



PixelExtractor::~PixelExtractor()
{}


void PixelExtractor::init(const edm::EventSetup *setup)
{
  setup->get<TrackerDigiGeometryRecord>().get(theTrackerGeometry);
}

//
// Method filling the main particle tree
//

void PixelExtractor::writeInfo(const edm::Event *event) 
{
  PixelExtractor::reset();


  edm::Handle< edm::DetSetVector<PixelDigi> > pDigiColl;

  event->getByLabel(m_tag, pDigiColl);

  edm::DetSetVector<PixelDigi>::const_iterator DSViterDigi = pDigiColl->begin();

  if (m_matching)
  {
    edm::InputTag m_pileupcollection("addPileupInfo");
    edm::Handle<std::vector<PileupSummaryInfo> > pileupinfos;
    event->getByLabel(m_pileupcollection,pileupinfos);

    std::vector<PileupSummaryInfo>::const_iterator PVI;
    
    for(PVI = pileupinfos->begin(); PVI != pileupinfos->end(); ++PVI) 
    {
      if (PVI->getBunchCrossing()==0) m_nPU = PVI->getPU_NumInteractions();      
    }

    event->getByLabel(m_tag, pDigiLinkColl);
  }

  bool barrel;
  bool endcap;

  int disk;  

  int cols;    
  int rows;    
  float pitchX;
  float pitchY;

  LocalPoint  clustlp;
  GlobalPoint pos;    

  edm::DetSet<PixelDigi>::const_iterator begin;
  edm::DetSet<PixelDigi>::const_iterator end;  
  edm::DetSet<PixelDigi>::const_iterator iter;
  
  for( ; DSViterDigi != pDigiColl->end(); DSViterDigi++) 
  {
    begin             = DSViterDigi->data.begin(); 
    end               = DSViterDigi->data.end();

    DetId detIdObject(DSViterDigi->detId());
    
    barrel = (detIdObject.subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel));
    endcap = (detIdObject.subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap));


    if (!barrel && !endcap) continue;

    const PixelGeomDetUnit* theGeomDet = dynamic_cast<const PixelGeomDetUnit*>(theTrackerGeometry->idToDet(detIdObject));      
    const PixelTopology* topol         = &(theGeomDet->specificTopology());

    cols     = topol->ncolumns();
    rows     = topol->nrows();
    pitchX = topol->pitch().first;
    pitchY = topol->pitch().second;

    if (m_matching)
    {
      edm::DetSetVector<PixelDigiSimLink>::const_iterator isearch = pDigiLinkColl->find(DSViterDigi->id);

      if(isearch != pDigiLinkColl->end())      //if it is not empty 
	pDigiLinks = (*pDigiLinkColl)[DSViterDigi->id];	        
    }

    for (iter = begin; iter != end; ++iter)
    {
      clustlp = topol->localPosition( MeasurementPoint(float((*iter).row())+0.5,float((*iter).column())+0.5));
      pos     =  theGeomDet->surface().toGlobal(clustlp);

      the_ids.clear();
      the_eids.clear();

      m_pixclus_x->push_back(pos.x());
      m_pixclus_y->push_back(pos.y());
      m_pixclus_z->push_back(pos.z());
      m_pixclus_e->push_back((*iter).adc());
      m_pixclus_row->push_back((*iter).row()); 
      m_pixclus_column->push_back((*iter).column());

      if (m_matching)
      {
	if (pDigiLinks.data.size() != 0)
	{
	  edm::DetSet<PixelDigiSimLink>::const_iterator it;
	  // Loop over DigisSimLink in this det unit

	  for(it = pDigiLinks.data.begin();  it != pDigiLinks.data.end(); it++) 
	  {         
	    if (static_cast<int>(it->channel())!=static_cast<int>((*iter).channel()))
	      continue;

	    //std::cout << it->SimTrackId() << " ##### " << it->eventId().rawId() << std::endl;

	    the_ids.push_back(it->SimTrackId()); 
	    the_eids.push_back(it->eventId().rawId()); 
	  }
	}
      }

      //      std::cout << the_ids.size() << " #///# " << the_eids.size() << std::endl;

      m_pixclus_simhit->push_back(the_ids.size());
      m_pixclus_simhitID->push_back(the_ids);
      m_pixclus_evtID->push_back(the_eids);

      if (barrel)
      {
	PXBDetId bdetid(detIdObject);
	m_pixclus_layer->push_back(static_cast<int>(bdetid.layer())); 
      	m_pixclus_module->push_back(static_cast<int>(bdetid.module())); 
      	m_pixclus_ladder->push_back(static_cast<int>(bdetid.ladder())); 
      }

      if (endcap)
      {
	PXFDetId fdetid(detIdObject);

	disk = (static_cast<int>(fdetid.side())*2-3)*static_cast<int>(fdetid.disk());

	if (disk>=4)  m_pixclus_layer->push_back(7+disk); 
	if (disk<=-4) m_pixclus_layer->push_back(14-disk); 
	if (disk<4 && disk>-4) m_pixclus_layer->push_back(-1); 

	if (static_cast<int>(fdetid.disk())<4)
	{
	  m_pixclus_ladder->push_back(static_cast<int>(fdetid.blade())); 
	}
	else
	{
	  m_pixclus_ladder->push_back(static_cast<int>(fdetid.ring())); 
	}

	m_pixclus_module->push_back(static_cast<int>(fdetid.module())); 
      }

      m_pixclus_nrow->push_back(rows);
      m_pixclus_ncolumn->push_back(cols);
      m_pixclus_pitchx->push_back(pitchX);
      m_pixclus_pitchy->push_back(pitchY);
      
      m_pclus++;
    }
  }

  PixelExtractor::fillTree();
}


//
// Method getting the info from an input file
//

void PixelExtractor::getInfo(int ievt) 
{
  m_tree->GetEntry(ievt); 
}

// Method initializing everything (to do for each event)

void PixelExtractor::reset()
{
  m_pclus = 0;
  m_nPU = 0;

  m_pixclus_x->clear(); 
  m_pixclus_y->clear(); 
  m_pixclus_z->clear(); 
  m_pixclus_e->clear(); 
  m_pixclus_row->clear();   
  m_pixclus_column->clear();
  m_pixclus_simhit->clear();
  m_pixclus_simhitID->clear();
  m_pixclus_evtID->clear();
  m_pixclus_layer->clear();  
  m_pixclus_module->clear(); 
  m_pixclus_ladder->clear(); 
  m_pixclus_nrow->clear();   
  m_pixclus_ncolumn->clear();
  m_pixclus_pitchx->clear(); 
  m_pixclus_pitchy->clear(); 
}


void PixelExtractor::fillTree()
{
  m_tree->Fill(); 
}
 
void PixelExtractor::fillSize(int size)
{
  m_pclus=size;
}

int  PixelExtractor::getSize()
{
  return m_pclus;
}

