#include "../interface/TkLayout_Translator.h"

TkLayout_Translator::TkLayout_Translator(TFile *a_file)
{
  std::cout << "Entering TkLayout Translator" << std::endl;


  m_OK = false;

  m_eventn = new std::vector<unsigned int>; 
  m_trackn = new std::vector<unsigned int>; 
  m_eta    = new  std::vector<double>;  
  m_phi0   = new  std::vector<double>;  
  m_z0     = new  std::vector<double>;   
  m_pt     = new  std::vector<double>;  
  m_glox   = new  std::vector<double>;  
  m_gloy   = new  std::vector<double>;  
  m_gloz   = new  std::vector<double>;  
  m_locx   = new  std::vector<double>;  
  m_locy   = new  std::vector<double>;  
  // pterr    = new  std::vector<float>; 
  //  hitprob  = new  std::vector<char>;
  m_deltas = new  std::vector<float>;
  m_cnt    = new  std::vector<char>;
  m_z      = new  std::vector<char>;
  m_rho    = new  std::vector<char>;
  m_phi    = new  std::vector<char>;
    
  m_stub_pt     = new  std::vector<float>;
  m_stub_ptMC   = new  std::vector<float>; 
  m_stub_ptGEN  = new  std::vector<float>; 
  m_stub_pzGEN  = new  std::vector<float>; 
  m_stub_PHI0   = new  std::vector<float>; 
  m_stub_IP     = new  std::vector<float>; 
  m_stub_Z0     = new  std::vector<float>; 
  m_stub_layer  = new  std::vector<int>; 
  m_stub_module = new  std::vector<int>;  
  m_stub_ladder = new  std::vector<int>; 
  m_stub_seg    = new  std::vector<int>;  
  m_stub_strip  = new  std::vector<int>; 
  m_stub_x      = new  std::vector<float>;  
  m_stub_y      = new  std::vector<float>;  
  m_stub_z      = new  std::vector<float>;  
  m_stub_clust1 = new  std::vector<int>;  
  m_stub_clust2 = new  std::vector<int>;  
  m_stub_deltas = new  std::vector<float>;  
  m_stub_tp     = new  std::vector<int>;  
  m_stub_pdg    = new  std::vector<int>;  

  reset();

  m_tree_L1TrackTrigger = new TTree("L1TrackTrigger","L1TrackTrigger Analysis info");  
  m_tree_retrieved      = dynamic_cast<TTree*>(a_file->Get("trackhits"));
  
  /// Branches definition


  if (!m_tree_retrieved)
  {
    std::cout << "This tree doesn't exist!!!" << std::endl;
    return;
  }

  m_tree_retrieved->SetBranchAddress("tracks.eventn", &m_eventn);
  m_tree_retrieved->SetBranchAddress("tracks.trackn", &m_trackn);
  m_tree_retrieved->SetBranchAddress("tracks.eta",    &m_eta);
  m_tree_retrieved->SetBranchAddress("tracks.phi0",   &m_phi0);
  m_tree_retrieved->SetBranchAddress("tracks.z0",     &m_z0);
  m_tree_retrieved->SetBranchAddress("tracks.pt",     &m_pt);
  m_tree_retrieved->SetBranchAddress("hits.cnt",      &m_cnt);
  m_tree_retrieved->SetBranchAddress("hits.glox",     &m_glox);
  m_tree_retrieved->SetBranchAddress("hits.gloy",     &m_gloy);
  m_tree_retrieved->SetBranchAddress("hits.gloz",     &m_gloz);
  m_tree_retrieved->SetBranchAddress("hits.locx",     &m_locx);
  m_tree_retrieved->SetBranchAddress("hits.locy",     &m_locy);
  m_tree_retrieved->SetBranchAddress("hits.z",        &m_z);
  m_tree_retrieved->SetBranchAddress("hits.rho",      &m_rho);
  m_tree_retrieved->SetBranchAddress("hits.phi",      &m_phi);
  m_tree_retrieved->SetBranchAddress("hits.deltas",   &m_deltas);

  m_OK = true;

  m_tree_L1TrackTrigger->Branch("evt",            &n_tot_evt);         // Simple evt number or event ID

  m_tree_L1TrackTrigger->Branch("STUB_n",         &m_stub);
  m_tree_L1TrackTrigger->Branch("STUB_pt",        &m_stub_pt);
  m_tree_L1TrackTrigger->Branch("STUB_ptMC",      &m_stub_ptMC);
  m_tree_L1TrackTrigger->Branch("STUB_ptGEN",     &m_stub_ptGEN);
  m_tree_L1TrackTrigger->Branch("STUB_pzGEN",     &m_stub_pzGEN);
  m_tree_L1TrackTrigger->Branch("STUB_layer",     &m_stub_layer);
  m_tree_L1TrackTrigger->Branch("STUB_module",    &m_stub_module);
  m_tree_L1TrackTrigger->Branch("STUB_ladder",    &m_stub_ladder);
  m_tree_L1TrackTrigger->Branch("STUB_seg",       &m_stub_seg);
  m_tree_L1TrackTrigger->Branch("STUB_strip",     &m_stub_strip);
  m_tree_L1TrackTrigger->Branch("STUB_x",         &m_stub_x);
  m_tree_L1TrackTrigger->Branch("STUB_y",         &m_stub_y);
  m_tree_L1TrackTrigger->Branch("STUB_z",         &m_stub_z);
  m_tree_L1TrackTrigger->Branch("STUB_clust1",    &m_stub_clust1);
  m_tree_L1TrackTrigger->Branch("STUB_clust2",    &m_stub_clust2);
  m_tree_L1TrackTrigger->Branch("STUB_deltas",    &m_stub_deltas);
  m_tree_L1TrackTrigger->Branch("STUB_tp",        &m_stub_tp);
  m_tree_L1TrackTrigger->Branch("STUB_pdgID",     &m_stub_pdg);
  m_tree_L1TrackTrigger->Branch("STUB_IP",        &m_stub_IP);
  m_tree_L1TrackTrigger->Branch("STUB_Z0",        &m_stub_Z0);
  m_tree_L1TrackTrigger->Branch("STUB_PHI0",      &m_stub_PHI0);

  /// Analysis settings (you define them in your python script)


  m_n_events = m_tree_retrieved->GetEntries();

  TkLayout_Translator::reset();
}

TkLayout_Translator::~TkLayout_Translator()
{}

void TkLayout_Translator::reset()
{
  /*
  m_eventn->clear();     
  m_trackn->clear();     
  m_eta->clear();        
  m_phi0->clear();       
  m_z0->clear();         
  m_pt->clear();         
  m_glox->clear();       
  m_gloy->clear();       
  m_gloz->clear();       
  m_locx->clear();       
  m_locy ->clear();      
  m_deltas->clear();     
  m_cnt->clear();        
  m_z->clear();          
  m_rho->clear();        
  m_phi->clear();        
  */
  m_stub = 0;
  
  m_stub_IP->clear();     
  m_stub_Z0->clear();     
  m_stub_PHI0->clear();     
  m_stub_tp->clear();     
  m_stub_pt->clear();     
  m_stub_ptMC->clear();   
  m_stub_ptGEN->clear();  
  m_stub_pzGEN->clear();  
  m_stub_layer->clear();  
  m_stub_module->clear(); 
  m_stub_ladder->clear(); 
  m_stub_seg->clear();    
  m_stub_strip->clear(); 
  m_stub_x->clear(); 
  m_stub_y->clear(); 
  m_stub_z->clear(); 
  m_stub_clust1->clear(); 
  m_stub_clust2->clear(); 
  m_stub_deltas->clear(); 
  m_stub_pdg->clear();

}

void TkLayout_Translator::do_translation()
{
  reset();

  int nhits = m_cnt->size();

  if (nhits == 0)  // No particle simulated, don't go further
  {
    ++n_tot_evt;
    return; 
  }

  float TIB_xdim = 96.;
  float TIB_ydim = 46.26;
  float TOB_xdim = 91.44;
  float TOB_ydim = 100.5;

  float TIB_pitchx = TIB_xdim/1024.;
  float TIB_pitchy = TIB_ydim/2.;
  float TOB_pitchx = TOB_xdim/1024.;
  float TOB_pitchy = TOB_ydim/2.;

  int type=0;
  int disk=0;
  int layer=0;
  int ladder=0;
  int module=0;
  int stripx=0;
  int stripy=0;  


  m_stub_pzGEN->clear();  


  m_stub_pdg->clear();

  for (int j=0;j<nhits;++j) 
  {

    m_stub_pt->push_back(0.);  
    m_stub_ptMC->push_back(0.);  
    m_stub_clust1->push_back(0); 
    m_stub_clust2->push_back(0); 
    m_stub_pdg->push_back(0); 
    m_stub_IP->push_back(0.);  
    m_stub_Z0->push_back(m_z0->at(0));  
    m_stub_PHI0->push_back(m_phi0->at(0));     

    m_stub_pzGEN->push_back(m_pt->at(0)*sinh(m_eta->at(0))); 
    
    type   = static_cast<int>(m_cnt->at(j));

    disk   = -1;
    layer  = -1;
    ladder = -1;
    module = -1;
    stripx = -1;
    stripy = -1;  
    
    // TIB
    if (type == 1) 
    {
      disk   = 0;
      layer  = 4+static_cast<int>(m_rho->at(j));  
      ladder = static_cast<int>(m_phi->at(j));

      (m_z->at(j)<0)
	? module = static_cast<int>(m_z->at(j))
	: module = static_cast<int>(m_z->at(j)-1);  

      stripx = static_cast<int>((TIB_xdim/2.-m_locx->at(j))/TIB_pitchx);
      stripy = static_cast<int>((TIB_ydim/2.-m_locy->at(j))/TIB_pitchy);
    }
    
    // TOB
    if (type == 2) 
    {
      disk   = 0;
      layer = 7+static_cast<int>(m_rho->at(j));
      ladder = static_cast<int>(m_phi->at(j));

      (m_z->at(j)<0)
	? module = static_cast<int>(m_z->at(j))
	: module = static_cast<int>(m_z->at(j)-1);      

      stripx = static_cast<int>((TOB_xdim/2.-m_locx->at(j))/TOB_pitchx);
      stripy = static_cast<int>((TOB_ydim/2.-m_locy->at(j))/TOB_pitchy);  
    }

    // Endcap

    if (type == 3) 
    {
      (m_z->at(j)<0)
	? disk   = -m_z->at(j)+17
	: disk   = m_z->at(j)+10;

      layer = 0;
      ladder = static_cast<int>(m_rho->at(j));
      module = static_cast<int>(m_phi->at(j));

      if (ladder<=8)
      {
       (m_z->at(j)<0)
	 ? stripx = static_cast<int>((-TIB_xdim/2.+m_locx->at(j))/TIB_pitchx)
	 : stripx = static_cast<int>((TIB_xdim/2.-m_locx->at(j))/TIB_pitchx);

	stripy = static_cast<int>((TIB_ydim/2.-m_locy->at(j))/TIB_pitchy);  
      }
      else
      {
       (m_z->at(j)<0)
	 ? stripx = static_cast<int>((-TOB_xdim/2.+m_locx->at(j))/TOB_pitchx)
	 : stripx = static_cast<int>((TOB_xdim/2.-m_locx->at(j))/TOB_pitchx);

	stripy = static_cast<int>((TOB_ydim/2.-m_locy->at(j))/TOB_pitchy);  
      }
    }
    
    m_stub_tp->push_back(static_cast<int>(0));     

    //    m_stub_disk->push_back(static_cast<int>(disk));
    m_stub_layer->push_back(static_cast<int>(layer+disk));
    m_stub_ladder->push_back(static_cast<int>(ladder)); 
    m_stub_module->push_back(static_cast<int>(module)); 
    
    m_stub_seg->push_back(static_cast<int>(stripy));    
    m_stub_strip->push_back(stripx);  
    m_stub_ptGEN->push_back(m_pt->at(0)); 
    m_stub_x->push_back(m_glox->at(j)); 
    m_stub_y->push_back(m_gloy->at(j)); 
    m_stub_z->push_back(m_gloz->at(j)); 
    m_stub_deltas->push_back(static_cast<int>(m_deltas->at(j))); 
    
    ++m_stub;
  }  

  ++n_tot_evt;
}

// Fill the root tree containing analysis results

void TkLayout_Translator::fillTree()
{
  m_tree_L1TrackTrigger->Fill(); 
}

//
// Method getting the info from an input file
//

void TkLayout_Translator::getInfo(int ievt) 
{
  m_tree_retrieved->GetEntry(ievt); 
}
