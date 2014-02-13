#include "../interface/MCExtractor.h"


MCExtractor::MCExtractor(bool doTree)
{
  // Set everything to 0
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
  m_part_evtId  = new std::vector<int>;
  m_part_px     = new std::vector<float>; 
  m_part_py     = new std::vector<float>; 
  m_part_pz     = new std::vector<float>;	
  m_part_eta    = new std::vector<float>;   
  m_part_phi    = new std::vector<float>; 	
  m_part_x      = new std::vector<float>;   
  m_part_y      = new std::vector<float>;   
  m_part_z      = new std::vector<float>;   
  m_part_used   = new std::vector<int>;  
  m_part_stId   = new std::vector< std::vector<int> >;  

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
  m_part_stId   = new std::vector< std::vector<int> >;  
  m_part_evtId  = new std::vector<int>;

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
  m_tree_retrieved->SetBranchAddress("subpart_pdgId",    &m_part_pdgId);
  m_tree_retrieved->SetBranchAddress("subpart_evtId",    &m_part_evtId);
  m_tree_retrieved->SetBranchAddress("subpart_px",       &m_part_px);
  m_tree_retrieved->SetBranchAddress("subpart_py",       &m_part_py);
  m_tree_retrieved->SetBranchAddress("subpart_pz",       &m_part_pz);
  m_tree_retrieved->SetBranchAddress("subpart_eta",      &m_part_eta);
  m_tree_retrieved->SetBranchAddress("subpart_phi",      &m_part_phi);
  m_tree_retrieved->SetBranchAddress("subpart_x",        &m_part_x);
  m_tree_retrieved->SetBranchAddress("subpart_y",        &m_part_y);
  m_tree_retrieved->SetBranchAddress("subpart_z",        &m_part_z);
  m_tree_retrieved->SetBranchAddress("subpart_stId",     &m_part_stId);
}



void MCExtractor::init(const edm::EventSetup *setup)
{
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
  

  edm::Handle<TrackingParticleCollection>  TPCollection ;
  event->getByLabel("mix","MergedTrackTruth",TPCollection);       
  const TrackingParticleCollection tpColl = *(TPCollection.product());
  

  std::vector<PSimHit>::const_iterator itp; 	
      
  GlobalPoint hit_position;
  LocalPoint hitpos;

  // Loop on tracking particles 
  for (TrackingParticleCollection::size_type tpIt=0; tpIt<tpColl.size(); tpIt++)
  { 
    //if (n_part > m_part_nMAX) continue; // Sanity check

    TrackingParticleRef tpr(TPCollection, tpIt);
    TrackingParticle* tp=const_cast<TrackingParticle*>(tpr.get());
    
    // Fill tracking particle variables

    m_part_pdgId->push_back(tp->pdgId());                       // Particle type
    m_part_px->push_back(tp->momentum().x());                   // 
    m_part_py->push_back(tp->momentum().y());                   // Momentum
    m_part_pz->push_back(tp->momentum().z());                   //
    m_part_eta->push_back(tp->momentum().eta());                // Eta
    m_part_phi->push_back(tp->momentum().phi());                // Phi
    m_part_x->push_back(tp->parentVertex()->position().x());    //
    m_part_y->push_back(tp->parentVertex()->position().y());    // Vertex of gen
    m_part_z->push_back(tp->parentVertex()->position().z());    //
    m_part_evtId->push_back(tp->eventId().rawId());  


    the_ids.clear();

    for (TrackingParticle::g4t_iterator g4T=tp->g4Track_begin(); g4T!=tp->g4Track_end(); ++g4T) 
      the_ids.push_back(g4T->trackId());
    
    m_part_stId->push_back(the_ids); 

    ++n_part;	      

  }//end loop for on tracking particle
  
  m_part_n    = n_part;

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
  m_part_evtId->clear();  
  m_part_stId->clear();  
  m_part_x->clear();      
  m_part_y->clear();       
  m_part_z->clear();       
  m_part_used->clear();    

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
  m_tree_new->Branch("subpart_pdgId",        &m_part_pdgId);
  m_tree_new->Branch("subpart_evtId",        &m_part_evtId);
  m_tree_new->Branch("subpart_stId",         &m_part_stId);
  m_tree_new->Branch("subpart_px",           &m_part_px);
  m_tree_new->Branch("subpart_py",           &m_part_py);
  m_tree_new->Branch("subpart_pz",           &m_part_pz);
  m_tree_new->Branch("subpart_eta",          &m_part_eta);
  m_tree_new->Branch("subpart_phi",          &m_part_phi);
  m_tree_new->Branch("subpart_x",            &m_part_x);
  m_tree_new->Branch("subpart_y",            &m_part_y);
  m_tree_new->Branch("subpart_z",            &m_part_z);
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

int MCExtractor::getMatchingTP(float x,float y, float z,
			       float px,float py, float pz)
{
  int idx = -1;

  for (int i=0;i<getNTP();++i) // Loop over TPs
  { 
    if (idx!=-1) return idx;

    if (fabs(getTP_x(i)-x)>0.001) continue;
    if (fabs(getTP_y(i)-y)>0.001) continue;
    if (fabs(getTP_z(i)-z)>0.001) continue;
    if (fabs(getTP_px(i)-px)>0.001) continue;
    if (fabs(getTP_py(i)-py)>0.001) continue;
    if (fabs(getTP_pz(i)-pz)>0.001) continue;

    idx=i;
  }

  return idx;
}



void MCExtractor::findMatchingTP(const int &stID,const int &evtID,
				 int &itp, bool verb)
{
  if (verb)
    std::cout << " Into new matching " << std::endl;

  n_TP       = getNTP();

  if (itp!=-1) return;


  for (int i=0;i<n_TP;++i) // Loop over tracking particles
  {
    if (m_part_evtId->at(i)!=evtID) continue;

    if (verb)
      std::cout << " Try to match " << std::endl;

    for (unsigned int j=0;j<m_part_stId->at(i).size();++j) // Loop on simtrack
    {
      if (m_part_stId->at(i).at(j)!=stID) continue;

      if (itp!=-1 && itp!=i) 
	std::cout << " More than one tracking particle for one simtrack ID: problem!!!! " << std::endl;

      itp = i;
    }
  }

  return;
}
