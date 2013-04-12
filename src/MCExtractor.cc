#include "../interface/MCExtractor.h"


MCExtractor::MCExtractor(bool doTree)
{
  // Set everything to 0
  resol_x    = 0.05;
  resol_y    = 0.05;
  resol_r    = 0.3;
  resol_phi  = 0.004;

  chi_lim    = 2.;

  m_OK = false;


  m_gen_x       = new std::vector<float>;
  m_gen_y       = new std::vector<float>;
  m_gen_z       = new std::vector<float>; 
  m_gen_px      = new std::vector<float>;
  m_gen_py      = new std::vector<float>;
  m_gen_pz      = new std::vector<float>;
  m_gen_proc    = new std::vector<int>;
  m_gen_pdg     = new std::vector<int>; 

  m_part_pdgId  = new std::vector<int>;
  m_part_px     = new std::vector<float>; 
  m_part_py     = new std::vector<float>; 
  m_part_pz     = new std::vector<float>;	
  m_part_eta    = new std::vector<float>;   
  m_part_phi    = new std::vector<float>; 	
  m_part_x      = new std::vector<float>;   
  m_part_y      = new std::vector<float>;   
  m_part_z      = new std::vector<float>;   
  m_part_used   = new std::vector<int>;  

  m_hits        = new std::vector<int>;      
  m_hits_x      = new std::vector<float>;  
  m_hits_y      = new std::vector<float>;  
  m_hits_z      = new std::vector<float>;  
  m_hits_e      = new std::vector<float>;  
  m_hits_id     = new std::vector<int>;   
  m_hits_layer  = new std::vector<int>;    
  m_hits_ladder = new std::vector<int>;   
  m_hits_module = new std::vector<int>;   
  m_hits_used   = new std::vector<int>;     

  m_st          = new std::vector<int>;    
  m_st_id       = new std::vector<int>;  


  MCExtractor::reset();


  //
  // Tree definition
  //

  if (doTree)
  {
    m_OK = true;
    MCExtractor::createTree();
  }
}


MCExtractor::MCExtractor(TFile *a_file)
{
  std::cout << "MCExtractor object is retrieved" << std::endl;
  resol_x    = 0.05;
  resol_y    = 0.05;
  resol_r    = 0.3;
  resol_phi  = 0.004;

  chi_lim    = 1.;
  //createTree();

  // Tree definition
  m_OK = false;

  m_gen_x       = new std::vector<float>;
  m_gen_y       = new std::vector<float>;
  m_gen_z       = new std::vector<float>; 
  m_gen_px      = new std::vector<float>;
  m_gen_py      = new std::vector<float>;
  m_gen_pz      = new std::vector<float>;
  m_gen_proc    = new std::vector<int>;
  m_gen_pdg     = new std::vector<int>; 

  m_part_pdgId  = new std::vector<int>;
  m_part_px     = new std::vector<float>; 
  m_part_py     = new std::vector<float>; 
  m_part_pz     = new std::vector<float>;	
  m_part_eta    = new std::vector<float>;   
  m_part_phi    = new std::vector<float>; 	
  m_part_x      = new std::vector<float>;   
  m_part_y      = new std::vector<float>;   
  m_part_z      = new std::vector<float>;   
  m_part_used   = new std::vector<int>;  

  m_hits        = new std::vector<int>;      
  m_hits_x      = new std::vector<float>;  
  m_hits_y      = new std::vector<float>;  
  m_hits_z      = new std::vector<float>;  
  m_hits_e      = new std::vector<float>;  
  m_hits_id     = new std::vector<int>;   
  m_hits_layer  = new std::vector<int>;    
  m_hits_ladder = new std::vector<int>;   
  m_hits_module = new std::vector<int>;   
  m_hits_used   = new std::vector<int>;     

  m_st          = new std::vector<int>;    
  m_st_id       = new std::vector<int>;  

  MCExtractor::reset();

  m_tree_retrieved = dynamic_cast<TTree*>(a_file->Get("MC"));

  if (!m_tree_retrieved)
  {
    std::cout << "This tree doesn't exist!!!" << std::endl;
    return;
  }


  m_OK = true;

  
  m_tree_retrieved->SetBranchAddress("gen_n",   &m_gen_n);
  m_tree_retrieved->SetBranchAddress("gen_proc",&m_gen_proc);
  m_tree_retrieved->SetBranchAddress("gen_pdg", &m_gen_pdg);
  m_tree_retrieved->SetBranchAddress("gen_px",  &m_gen_px);
  m_tree_retrieved->SetBranchAddress("gen_py",  &m_gen_py);
  m_tree_retrieved->SetBranchAddress("gen_pz",  &m_gen_pz);
  m_tree_retrieved->SetBranchAddress("gen_x",   &m_gen_x);
  m_tree_retrieved->SetBranchAddress("gen_y",   &m_gen_y);
  m_tree_retrieved->SetBranchAddress("gen_z",   &m_gen_z);
  
  // Infos related to the subsequent tracking particles
  
  m_tree_retrieved->SetBranchAddress("subpart_n",        &m_part_n);
  m_tree_retrieved->SetBranchAddress("subpart_hits",     &m_hits);
  m_tree_retrieved->SetBranchAddress("subpart_st",       &m_st);
  m_tree_retrieved->SetBranchAddress("subpart_pdgId",    &m_part_pdgId);
  m_tree_retrieved->SetBranchAddress("subpart_px",       &m_part_px);
  m_tree_retrieved->SetBranchAddress("subpart_py",       &m_part_py);
  m_tree_retrieved->SetBranchAddress("subpart_pz",       &m_part_pz);
  m_tree_retrieved->SetBranchAddress("subpart_eta",      &m_part_eta);
  m_tree_retrieved->SetBranchAddress("subpart_phi",      &m_part_phi);
  m_tree_retrieved->SetBranchAddress("subpart_x",        &m_part_x);
  m_tree_retrieved->SetBranchAddress("subpart_y",        &m_part_y);
  m_tree_retrieved->SetBranchAddress("subpart_z",        &m_part_z);
  
  m_tree_retrieved->SetBranchAddress("subpart_nhit",     &m_part_nhit);
  m_tree_retrieved->SetBranchAddress("subpart_hits_x",   &m_hits_x);
  m_tree_retrieved->SetBranchAddress("subpart_hits_y",   &m_hits_y);
  m_tree_retrieved->SetBranchAddress("subpart_hits_z",   &m_hits_z);
  m_tree_retrieved->SetBranchAddress("subpart_hits_e",   &m_hits_e);
  m_tree_retrieved->SetBranchAddress("subpart_hits_id",  &m_hits_id);
  m_tree_retrieved->SetBranchAddress("subpart_hits_layer",   &m_hits_layer);
  m_tree_retrieved->SetBranchAddress("subpart_hits_ladder",  &m_hits_ladder);
  m_tree_retrieved->SetBranchAddress("subpart_hits_module",  &m_hits_module);

  m_tree_retrieved->SetBranchAddress("subpart_nsimtracks",   &m_part_nstracks);
  m_tree_retrieved->SetBranchAddress("subpart_st_ids",       &m_st_id);


}



void MCExtractor::init(const edm::EventSetup *setup)
{
  //std::cout << "MCExtractor objet is created" << std::endl;

  //
  // Initializations 
  //

  // Here we build the whole detector
  // We need that to retrieve all the hits

  setup->get<TrackerDigiGeometryRecord>().get(theTrackerGeometry);

}

//
// Method filling the main event
//

void MCExtractor::writeInfo(const edm::Event *event) 
{
  using namespace reco;

  // Reset Tree Variables :
  MCExtractor::reset();


  // First of all, we get some info on the generated event
  MCExtractor::getGenInfo(event); 


  //
  // Then loop on tracking particles (TPs): 
  //



  // Get the different Calo hits

  int n_part        = 0; // The total number of stored TPs
  int n_hits        = 0; // The total number of hits per TP
  int n_hits_tot    = 0; // The total number of hits per event
  int n_st_tot      = 0; // The total number of simtrack

  edm::Handle<TrackingParticleCollection>  TPCollection ;
  event->getByLabel("mergedtruth","MergedTrackTruth",TPCollection);       
  const TrackingParticleCollection tpColl = *(TPCollection.product());
  

  std::vector<PSimHit>::const_iterator itp; 	
      
  GlobalPoint hit_position;
  int subdetID;

  bool barrel;
  bool endcap;

  int disk;
  int cols;    
  int rows;    
  float pitchX;
  float pitchY;

  float rowhit;
  float colhit;
  LocalPoint hitpos;

  // Loop on tracking particles 
  for (TrackingParticleCollection::size_type tpIt=0; tpIt<tpColl.size(); tpIt++)
  { 
    //if (n_part > m_part_nMAX) continue; // Sanity check

    TrackingParticleRef tpr(TPCollection, tpIt);
    TrackingParticle* tp=const_cast<TrackingParticle*>(tpr.get());
    
    // Fill tracking particle variables

    m_st->push_back(tp->g4Track_end() - tp->g4Track_begin());   // Number of hits
    m_part_pdgId->push_back(tp->pdgId());                       // Particle type
    m_part_px->push_back(tp->momentum().x());                   // 
    m_part_py->push_back(tp->momentum().y());                   // Momentum
    m_part_pz->push_back(tp->momentum().z());                   //
    m_part_eta->push_back(tp->momentum().eta());                // Eta
    m_part_phi->push_back(tp->momentum().phi());                // Phi
    m_part_x->push_back(tp->parentVertex()->position().x());    //
    m_part_y->push_back(tp->parentVertex()->position().y());    // Vertex of gen
    m_part_z->push_back(tp->parentVertex()->position().z());    //
    
    for (TrackingParticle::g4t_iterator g4T=tp->g4Track_begin(); g4T!=tp->g4Track_end(); ++g4T) 
    {
      m_st_id->push_back(g4T->trackId());
      ++n_st_tot;  
    }
    
    // Then loop on the simulated hits for the corresponding TP

    n_hits   = 0;
    subdetID = 0;

    // 3. Tracker hits

    for (itp = tp->pSimHit_begin(); itp < tp->pSimHit_end(); itp++)
    {      
      DetId theDetUnitId(itp->detUnitId());
      
      // Go away if not in a tracking detector
      
      if (theDetUnitId.det() != DetId::Tracker && theDetUnitId.det() != DetId::Muon)
	continue;
      
      if (theDetUnitId.det() == DetId::Tracker)
      {
	subdetID      = theDetUnitId.subdetId();
	hit_position  = theTrackerGeometry->idToDet(theDetUnitId)->surface().toGlobal(itp->localPosition());
	
	barrel = (subdetID == static_cast<int>(PixelSubdetector::PixelBarrel));
	endcap = (subdetID == static_cast<int>(PixelSubdetector::PixelEndcap));

	if (!barrel && !endcap) continue;

	const PixelGeomDetUnit* theGeomDet = dynamic_cast<const PixelGeomDetUnit*>(theTrackerGeometry->idToDet(theDetUnitId));      
	const PixelTopology* topol         = &(theGeomDet->specificTopology());

	cols     = topol->ncolumns();
	rows     = topol->nrows();
	pitchX   = topol->pitch().first;
	pitchY   = topol->pitch().second;

	rowhit   = itp->localPosition().x()/pitchX+float(rows)/2.;
	colhit   = itp->localPosition().y()/pitchY+float(cols)/2.;


	hitpos   = topol->localPosition( MeasurementPoint(rowhit,colhit));
	  
	hit_position= theGeomDet->surface().toGlobal(itp->localPosition());
	//hit_position = theGeomDet->surface().toGlobal(hitpos);
	
  
	if (barrel)
	{
	  PXBDetId bdetid(theDetUnitId);
	  m_hits_layer->push_back(static_cast<int>(bdetid.layer())); 
	  m_hits_module->push_back(static_cast<int>(bdetid.module())); 
	  m_hits_ladder->push_back(static_cast<int>(bdetid.ladder()));  


	  //std::cout << static_cast<int>(bdetid.layer()) << " / " 
	  //	    << static_cast<int>(bdetid.module()) << " / " 
	  //	    << cols << " / " << rows << " / " 
	  //	    << rowhit << " / " << colhit << std::endl;

	}

	if (endcap)
	{
	  PXFDetId fdetid(theDetUnitId);
	  
	  disk = (static_cast<int>(fdetid.side())*2-3)*static_cast<int>(fdetid.disk());
      
	  if (disk>=4)  m_hits_layer->push_back(7+disk); 
	  if (disk<=-4) m_hits_layer->push_back(14-disk); 
	  if (disk<4 && disk>-4) m_hits_layer->push_back(-1); 
	  
	  if (static_cast<int>(fdetid.disk())<4)
	  {
	    m_hits_ladder->push_back(static_cast<int>(fdetid.blade())); 
	  }
	  else
	  {
	    m_hits_ladder->push_back(static_cast<int>(fdetid.ring())); 
	  }
	
	  m_hits_module->push_back(static_cast<int>(fdetid.module())); 
	}

	m_hits_x->push_back(static_cast<float>(hit_position.x()));   
	m_hits_y->push_back(static_cast<float>(hit_position.y()));   
	m_hits_z->push_back(static_cast<float>(hit_position.z()));
	m_hits_e->push_back(itp->energyLoss());   
	m_hits_id->push_back(subdetID);  
	

	++n_hits;
	++n_hits_tot;

      }
    }//end loop on sim hits
    
    m_hits->push_back(n_hits);
    ++n_part;	      

  }//end loop for on tracking particle
  
  m_part_n    = n_part;
  m_part_nhit = n_hits_tot;
  m_part_nstracks = n_st_tot;

  //___________________________
  //
  // Fill the tree :
  //___________________________

  MCExtractor::fillTree();
}



// Method retrieving the generated info of the event


void MCExtractor::getGenInfo(const edm::Event *event) 
{
  edm::Handle<reco::GenParticleCollection> genParticles;
  event->getByLabel("genParticles", genParticles);

  m_gen_n=static_cast<int>(genParticles->size());

  //std::cout << "Number of GEN particles : " << m_gen_n << std::endl; 

  int npartg=0;

  for (reco::GenParticleCollection::const_iterator mcIter=genParticles->begin(); mcIter != genParticles->end(); mcIter++ ) 
  {
    m_gen_x->push_back(mcIter->vx()); //
    m_gen_y->push_back(mcIter->vy()); // Gen of the initial MIB particle
    m_gen_z->push_back(mcIter->vz()); //
    m_gen_px->push_back(mcIter->px());                      //
    m_gen_py->push_back(mcIter->py());                      // Momentum
    m_gen_pz->push_back(mcIter->pz());                      //
    m_gen_pdg->push_back(mcIter->pdgId());   

    ++npartg;
  }
}


//
// Method getting the info from an input file
//

void MCExtractor::getInfo(int ievt) 
{
  reset();
  m_tree_retrieved->GetEntry(ievt); 
}

// Method initializing everything (to do before each event)

void MCExtractor::reset()
{

  m_gen_n         = 0;
  m_part_n        = 0;
  m_part_nhit     = 0;
  m_part_nstracks = 0;


  m_gen_x->clear();
  m_gen_y->clear();
  m_gen_z->clear();
  m_gen_px->clear();
  m_gen_py->clear();  
  m_gen_pz->clear();  
  m_gen_proc->clear();
  m_gen_pdg->clear(); 
  
  m_part_px->clear(); 
  m_part_py->clear(); 
  m_part_pz->clear(); 
  m_part_eta->clear();
  m_part_phi->clear();
  m_part_pdgId->clear();  
  m_part_x->clear();      
  m_part_y->clear();       
  m_part_z->clear();       
  m_part_used->clear();    
  m_hits->clear();         
  m_st->clear();           
 
  m_st_id->clear();  
  
  m_hits_used->clear();  
  m_hits_x->clear();  
  m_hits_y->clear();  
  m_hits_z->clear();  
  m_hits_e->clear();  
  m_hits_id->clear();  
  m_hits_layer->clear();  
  m_hits_ladder->clear();  
  m_hits_module->clear();  
}    

  
void MCExtractor::fillTree()
{
  m_tree_new->Fill(); 
}
 
void MCExtractor::fillSize(int size)
{
  m_gen_n=size;
}

int  MCExtractor::getSize()
{
  return m_gen_n;
}

  
  
void MCExtractor::createTree()
{  
  m_tree_new = new TTree("MC","MC info");  

  m_tree_new->Branch("gen_n",   &m_gen_n);
  m_tree_new->Branch("gen_proc",&m_gen_proc);
  m_tree_new->Branch("gen_pdg", &m_gen_pdg);
  m_tree_new->Branch("gen_px",  &m_gen_px);
  m_tree_new->Branch("gen_py",  &m_gen_py);
  m_tree_new->Branch("gen_pz",  &m_gen_pz);
  m_tree_new->Branch("gen_x",   &m_gen_x);
  m_tree_new->Branch("gen_y",   &m_gen_y);
  m_tree_new->Branch("gen_z",   &m_gen_z);

  // Infos related to the subsequent tracking particles
  
  m_tree_new->Branch("subpart_n",            &m_part_n);
  m_tree_new->Branch("subpart_hits",         &m_hits);
  m_tree_new->Branch("subpart_st",           &m_st);
  m_tree_new->Branch("subpart_pdgId",        &m_part_pdgId);
  m_tree_new->Branch("subpart_px",           &m_part_px);
  m_tree_new->Branch("subpart_py",           &m_part_py);
  m_tree_new->Branch("subpart_pz",           &m_part_pz);
  m_tree_new->Branch("subpart_eta",          &m_part_eta);
  m_tree_new->Branch("subpart_phi",          &m_part_phi);
  m_tree_new->Branch("subpart_x",            &m_part_x);
  m_tree_new->Branch("subpart_y",            &m_part_y);
  m_tree_new->Branch("subpart_z",            &m_part_z);

  m_tree_new->Branch("subpart_nsimtracks",   &m_part_nstracks);
  m_tree_new->Branch("subpart_st_ids",       &m_st_id);

  m_tree_new->Branch("subpart_nhit",         &m_part_nhit);
  m_tree_new->Branch("subpart_hits_x",       &m_hits_x);
  m_tree_new->Branch("subpart_hits_y",       &m_hits_y);
  m_tree_new->Branch("subpart_hits_z",       &m_hits_z);
  m_tree_new->Branch("subpart_hits_e",       &m_hits_e);
  m_tree_new->Branch("subpart_hits_id",      &m_hits_id);
  m_tree_new->Branch("subpart_hits_layer",   &m_hits_layer);
  m_tree_new->Branch("subpart_hits_ladder",  &m_hits_ladder);
  m_tree_new->Branch("subpart_hits_module",  &m_hits_module);

} 
void MCExtractor::printhits(float x, float y, float z)
{
 int n_HITS= getNHITS();

 for (int i=0;i<n_HITS;++i) // Loop over tracking particles
 {
   if (getTP_hitsid(i)   == 2)      continue; // In the endcap, don't care

   std::cout << "Hit " << i << " : " << getTP_hitsx(i)-x << " / " 
	     << getTP_hitsy(i)-y << " / " 
	     << getTP_hitsz(i)-z << std::endl;   
 }

 std::cout << n_HITS << std::endl;
}
void MCExtractor::clearTP(float ptmin,float rmax)
{
 int n_TP= getNTP();

 for (int i=0;i<n_TP;++i) // Loop over tracking particles
 {
   if (getTP_r(i)>rmax || getTP_pt(i)<ptmin || fabs(getTP_eta(i))>5.5) 
   {        
     m_part_used->push_back(1);
   }
   else
   {
     m_part_used->push_back(0);
   }
 }
}

void MCExtractor::findMatchingTP(const int &stID,const int &layer,
				 const int &ladder,const int &module, 
				 const int &nseg, 
				 const float &x, const float &y, const float &z, 
				 int &itp, int &ihit, bool verb)
{
  if (verb)
    std::cout << " Into matching " << std::endl;

  n_st_part  = 0;
  n_hit_part = 0;
  chi        = 0.;
  n_hit      = 0;
  n_st       = 0;
  n_hit2     = 0;
  n_st2      = 0;
  n_TP       = getNTP();
  chi_lim    = 10.;
  start      = 0;

  if (itp!=-1)
  {
    start=itp;
    itp=-1;
  }

  for (int i=start;i<n_TP;++i) // Loop over tracking particles
  {
    n_hit = m_hits->at(i);
    n_st  = m_st->at(i);
    n_st2 = n_st_part+n_st;
    n_hit2= n_hit_part+n_hit;

    if (m_part_used->at(i)==1)
    {
      n_st_part  = n_st2;
      n_hit_part = n_hit2;
      continue;
    } 

    if (stID != -1)
    {
      if (verb)
	std::cout << " Try to match " << std::endl;

      match = false;

      if (m_st_id->at(n_st2-1)>=stID)
      {
	for (int j=n_st_part;j<n_st2;++j) // Loop on simtrack
	{
	  //	  if (verb)
	  //	    std::cout << m_st_id->at(j) << " / " << stID << std::endl;
	  
	  if (m_st_id->at(j)==stID) 
	  {
	    match = true;
	    break;
	  }
	  if (m_st_id->at(j)>stID) 
	  {
	    break;
	  }
	}  
      }

      if (!match) 
      {
	if (verb)
	  std::cout << " No match " << std::endl;

	n_st_part  = n_st2;
	n_hit_part = n_hit2;
	continue;         
      }
    }

    
    for (int k=n_hit_part;k<n_hit2;++k) // Loop on hits
    {
      if (verb)
	std::cout << k << " / " <<  n_hit << std::endl;

      if (m_hits_module->at(k) != module ||  
	  m_hits_ladder->at(k) != ladder ||
	  m_hits_layer->at(k)  != layer)  continue;

      dz = fabs(m_hits_z->at(k)-z);
      dr = fabs(sqrt(m_hits_x->at(k)*m_hits_x->at(k)+m_hits_y->at(k)*m_hits_y->at(k))-sqrt(x*x+y*y)); 

      /*
      if (layer<=7 && dz>2.5/static_cast<float>(nseg)) continue;
      if (layer<=10 && layer>7 && dz>5./static_cast<float>(nseg))  continue; 
      if (layer>10 && ladder<=5 && dr>2.5/static_cast<float>(nseg))  continue;    
      if (layer>10 && ladder>5 && dr>5./static_cast<float>(nseg))  continue;    
      */

      bool pass=true;

      if (sqrt(x*x+y*y)<60)
      {
	if (layer>10 && dr>2.5/static_cast<float>(nseg)) pass=false;
	if (layer<=10 && dz>2.5/static_cast<float>(nseg)) pass=false;
      }
      else
      {
	if (layer>10 && dr>5./static_cast<float>(nseg)) pass=false;
	if (layer<=10 && dz>5./static_cast<float>(nseg)) pass=false;
      }

      if (!pass) continue;

      dx  = fabs(m_hits_x->at(k)-x);
      dy  = fabs(m_hits_y->at(k)-y);     
 
      dphi= fabs(atan2(m_hits_y->at(k),m_hits_x->at(k))-atan2(y,x)); 
     
      if (layer>10)
      {
	chi = (dphi/resol_phi)*(dphi/resol_phi);
      }
      else
      {
	chi = (dx/resol_x)*(dx/resol_x)+(dy/resol_y)*(dy/resol_y);
      }

	//chi = (dr/resol_r)*(dr/resol_r)+(dphi/resol_phi)*(dphi/resol_phi);
      //std::cout << sqrt(x*x+y*y) << " / " << sqrt(dx*dx+dy*dy) << std::endl;

      if (verb)
	std::cout << x << " / " << m_hits_x->at(k) << " / " 
		  << y << " / " << m_hits_y->at(k) << " / " 
		  << z << " / " << m_hits_z->at(k) << " / " << chi << std::endl;

      if (chi<chi_lim)
      {
	chi_lim=chi;
	itp=i;
	ihit=k;

	if (verb)
	  std::cout << "MATCH!!" << std::endl; 

	if (chi_lim<1.) return;	
      }
    }
   
    n_st_part  = n_st2;
    n_hit_part = n_hit2;
  }

  return;
}
