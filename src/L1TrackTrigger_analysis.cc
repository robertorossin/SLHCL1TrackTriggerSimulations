/*

The private Cluster and STUB maker

Adapted to CMSSW 6_2_0_SLHC7

The private cluster maker is not optimal, particularly in Pixel modules
Please use the official cluster/stub maker for official analysis

Contact: viret@in2p3.fr
Last review : 13/02/2014
 */


#include "../interface/L1TrackTrigger_analysis.h"

L1TrackTrigger_analysis::L1TrackTrigger_analysis(AnalysisSettings *settings, int start_evt)
{
  std::cout << "Entering L1TrackTrigger analysis" << std::endl;


  /// Analysis settings (you define them in your python script)

  // If the setting is not defined the method returns -1. In this 
  // case we set a default value for the cut, in order to avoid 
  // unwanted crash


  // Verbose mode (Huge amount of debugging printouts)
  (settings->getSetting("verbose")!=-1)
    ? m_verb = (static_cast<bool>(settings->getSetting("verbose")))
    : m_verb = false;

  // If you want to make stubs only with matched digis from TP 0 (for BANK generation)
  (settings->getSetting("matchedStubs")!=-1)
    ? m_matchStubs = (static_cast<bool>(settings->getSetting("matchedStubs")))
    : m_matchStubs = false;

  // Do we match the stubs with digis or not (should be ON if you use matchedStubs)
  (settings->getSetting("posMatching")!=-1)
    ? m_posMatch = (static_cast<bool>(settings->getSetting("posMatching")))
    : m_posMatch = false;

  // If you want to apply the Z segment matching in the PS modules (default is true)
  (settings->getSetting("zMatch")!=-1)
    ? m_zMatch = (static_cast<bool>(settings->getSetting("zMatch")))
    : m_zMatch = true;

  // Cluster width cut (in strip units)
  (settings->getSetting("maxClusWdth")!=-1)
    ? m_max_wclus = settings->getSetting("maxClusWdth")
    : m_max_wclus = 1000;

  // Stub width (in half-strip units)
  (settings->getSetting("windowSize")!=-1)
    ? m_window_size = settings->getSetting("windowSize")
    : m_window_size = -1;

  // If you want to select only specific particles for making stubs 
  (settings->getSetting("pdgSel")!=-1)
    ? m_PDG_id = settings->getSetting("pdgSel")
    : m_PDG_id = -1;

  // The pt threshold for the barrel stubs 
  (settings->getSetting("thresh")!=-1)
    ? m_pTthresh = settings->getSetting("thresh")
    : m_pTthresh = 2;

  // To keep track of the event number for pileup
  (settings->getSetting("evtNum")!=-1)
    ? m_evtNum = settings->getSetting("evtNum")
    : m_evtNum = -1;

  n_tot_evt = start_evt;
  if (m_evtNum!=-1) n_tot_evt=m_evtNum; 

  m_thresh = 0; // No threshold for digits (0 or 1) 

  // Initialize everything
  L1TrackTrigger_analysis::initialize();
}

L1TrackTrigger_analysis::~L1TrackTrigger_analysis()
{}

/*
Here is the main method, called for each event
 */


void L1TrackTrigger_analysis::do_stubs(PixelExtractor *pix, MCExtractor *mc)
{
  // Start by resetting some particles
  L1TrackTrigger_analysis::reset();

 // No particle simulated, don't go further
  if (mc->getNGen() == 0) return; 

  if (m_verb) cout << "Analyzing event " << n_tot_evt << endl;


  // First get and match the digis
  L1TrackTrigger_analysis::get_digis(pix,mc);

  if (m_digi_ref->size()<2) return; // Not enough digis, pointless...

  // Then get the clusters
  L1TrackTrigger_analysis::get_clusters(pix,mc);

  // Finally loop over layers to get the stubs
  for (int i=5;i<25;++i) L1TrackTrigger_analysis::get_stubs(i,mc); 

}


  

/*

Method extracting the digis using the pixel info and, if requested, 
performs the matching with the MC truth

 */



void L1TrackTrigger_analysis::get_digis(PixelExtractor *pix, MCExtractor *mc)
{

  int ndigis = pix->getNDigis(); 
 
  if (ndigis==0) return;
   
  int nst       = -1; // The sim track index
  int nid       = -1; // The encoded event ID
  int tp        = -1; // 

  std::vector<int> matching_tps;
  std::vector<int> matching_hits;
  std::vector<int> mods;

  // Remove some hits from the TPs in order to save some 
  // time for the matching (in particular for high-PU event)
  //
  // mc->clearTP(A,B)
  //
  // Means we won't match the TP with pt<A GeV/c or |eta|>5.5 or d0>B mm
  //


  (m_posMatch==false)
    ? mc->clearTP(0.5,2.0) // Here we just keep the primaries 
    : mc->clearTP(0.001,10000000.0); // Here you match everything

  // Loop over the pixel digis

  for (int i=0;i<ndigis;++i)
  {
    if (i%10000==0 && m_verb) cout << i << " / " << ndigis << endl;

    if (pix->layer(i)<5) continue; // We exclude pixel digis

    matching_tps.clear();
    
    for (int ik=0;ik<pix->isSimHit(i);++ik) // Loop over simhit (matching)
    {
      if (m_verb) cout << "Hit " << ik << endl;

      if (m_posMatch==false)
      {
        nst = -1;
	nid = -1;
      }
      else
      {
        nst = pix->tpIndex(i,ik);  // The sim track index
        nid = pix->evtIndex(i,ik); // The evt id of the sim track
      }

      tp         = -1; // 

      if (m_verb) cout << "Try to match hit " << i << " from simtrack " << nst << endl;
       
      mc->findMatchingTP(nst,nid,tp,m_verb);

      if (m_verb) cout << "Matched with TP " << tp << endl;

      if (tp!=-1) matching_tps.push_back(tp);
    }	

    if (m_verb) cout << "End of TP matching " << endl;

    if (static_cast<int>(matching_tps.size())==0) matching_tps.push_back(-1);

    // Update the global params
    m_digi_ref->push_back(i);
    m_digi_tp->push_back(matching_tps);
  } // End of the digi loop

  if (m_verb) // Printouts (for verbose mode)
  {
    int idx;

    cout << "Event " << n_tot_evt << " contains " << ndigis << " digis" << endl;
    cout << " covering " << m_digi_modids->size() << " modules" << endl;

    for (int i=0;i<static_cast<int>(m_digi_ref->size());++i) // Loop over pixel hits
    {
      idx = static_cast<int>(m_digi_ref->at(i));

      cout << " Digi " << idx << " : "
	   << pix->layer(idx) << " / " << pix->ladder(idx) << " / " << pix->module(idx) << " / " 
	   << pix->column(idx) << " / " << pix->row(idx) << " / "
	   << pix->x(idx) << " / " << pix->y(idx) << " / " << pix->z(idx)
	   << " was matched with " << m_digi_tp->at(i).size() << " tracking particle(s) " << std::endl;
    }
  }
}


/*

Method extracting the cluster using the digi info

 */


void L1TrackTrigger_analysis::get_clusters(PixelExtractor *pix, MCExtractor *mc)
{
  int   nsat       = 0;
  int   nstrips    = 0;
  float bary_strip = 0.;
  float bary_col   = 0.;

  int ref_lay  = -1;
  int ref_lad  = -1;
  int ref_mod  = -1;
  int ref_seg  = -1;
  int ref_nseg = -1;
  int ref_nrow = -1;
 
  float bary_sum=0.;
  float bary_x=0.;
  float bary_y=0.;
  float bary_z=0.;
 
  int m_clus_prev   = -1;
  int idx;

  int is_there=0;

  std::vector<int> matching_tps_clus;
  std::vector<int> pix_list;
  
  pix_list.clear();
  matching_tps_clus.clear();  
  clus_row.clear();  
  clus_col.clear();  

  for (int i=0;i<static_cast<int>(m_digi_ref->size());++i) // Loop over modules
  { 

    idx = static_cast<int>(m_digi_ref->at(i));
  
    // First we check is this cluster is adjacent to the previous one 
    // (in both directions)

    if (m_verb)
      cout << idx << " / "   
	   << pix->layer(idx) << " / "  
	   << pix->ladder(idx) << " / "  
	   << pix->module(idx) << " / "  
	   << pix->column(idx) << " / "  
	   << pix->row(idx) << " / "  
	   << pix->pitchx(idx) << " / "  
	   << m_clus << endl;

    if (pix->e(idx)>m_thresh)
    {
      if (is_neighbour(pix,idx,ref_lay,ref_lad,ref_mod)) // This is not perfect
      {                                                  // Could lead to two clusters instead of 1 

	// To see a good implementation, based on detid and map of digis, see:
	//
	// https://github.com/cms-sw/cmssw/blob/CMSSW_6_2_0_SLHC7/L1Trigger/TrackTrigger/src/TTClusterAlgorithm_2d2013.cc
	//

	// If so we add it to the growing cluster
	
	if (m_verb) cout << "ACCUMULATE" << endl;

	clus_row.push_back(pix->row(idx));
	clus_col.push_back(pix->column(idx));
	pix_list.push_back(idx);  

	for (int k=0;k<static_cast<int>(m_digi_tp->at(i).size());++k) // Loop over match tps
	{
	  is_there=0;

	  for (int l=0;l<static_cast<int>(matching_tps_clus.size());++l) 
	  {
	    if (matching_tps_clus.at(l)==(m_digi_tp->at(i)).at(k))
	    {
	      is_there=1;
	      break;
	    }
	  }

	  if (is_there==0) matching_tps_clus.push_back((m_digi_tp->at(i)).at(k));
	}	

	++bary_sum;
	bary_x     += pix->x(idx);
	bary_y     += pix->y(idx);
	bary_z     += pix->z(idx);
	bary_strip += pix->row(idx);
	bary_col   += pix->column(idx);
      
	if (pix->e(idx)==255) ++nsat;
	++nstrips;
      } else {       // This is the start of a new cluster

	if (m_verb) cout << "NEW CLUSTER" << endl;

	clus_row.clear();  
	clus_col.clear();  
	matching_tps_clus.clear();  
	pix_list.clear();  

	nstrips      = 0;
	nsat         = 0;

	clus_row.push_back(pix->row(idx));
	clus_col.push_back(pix->column(idx));
	pix_list.push_back(idx);  	

	for (int k=0;k<static_cast<int>(m_digi_tp->at(i).size());++k) // Loop over pixel hits
	  matching_tps_clus.push_back((m_digi_tp->at(i)).at(k));

	ref_lay  = pix->layer(idx);
	ref_lad  = pix->ladder(idx);
	ref_mod  = pix->module(idx);
	ref_seg  = pix->column(idx);
	ref_nseg = pix->ncolumn(idx);
	ref_nrow = pix->nrow(idx);

	bary_sum   = 1;
	bary_x     = pix->x(idx);
	bary_y     = pix->y(idx);
	bary_z     = pix->z(idx);
	bary_strip = pix->row(idx);
	bary_col   = pix->column(idx);

	if (pix->e(idx)==255) ++nsat;
	++nstrips;
	++m_clus;
      }
    } // End of cluster test loop

    // New cluster fill info

    if (m_clus==m_clus_prev)
    {
      m_clus_pix->at(m_clus-1)     = pix_list;
      m_clus_tp->at(m_clus-1)      = matching_tps_clus;
      m_clus_matched->at(m_clus-1) = matching_tps_clus.size();
      m_clus_x->at(m_clus-1)       = bary_x/bary_sum;
      m_clus_y->at(m_clus-1)       = bary_y/bary_sum;
      m_clus_z->at(m_clus-1)       = bary_z/bary_sum;
      m_clus_sat->at(m_clus-1)     = nsat;
      m_clus_nstrips->at(m_clus-1) = nstrips;
      m_clus_layer->at(m_clus-1)   = ref_lay;
      m_clus_ladder->at(m_clus-1)  = ref_lad;
      m_clus_module->at(m_clus-1)  = ref_mod;
      m_clus_seg->at(m_clus-1)     = ref_seg;
      m_clus_strip->at(m_clus-1)   = bary_strip/bary_sum-fmod(bary_strip/bary_sum,0.5);
    }
    else
    {
      m_clus_prev=m_clus;
      m_clus_pix->push_back(pix_list);
      m_clus_tp->push_back(matching_tps_clus);
      m_clus_matched->push_back(matching_tps_clus.size());
      m_clus_x->push_back(bary_x/bary_sum);
      m_clus_y->push_back(bary_y/bary_sum);
      m_clus_z->push_back(bary_z/bary_sum);
      m_clus_used->push_back(0);
      m_clus_sat->push_back(nsat);
      m_clus_nstrips->push_back(nstrips);
      m_clus_layer->push_back(ref_lay);
      m_clus_ladder->push_back(ref_lad);
      m_clus_module->push_back(ref_mod);
      m_clus_seg->push_back(ref_seg);
      m_clus_PS->push_back(ref_nseg);
      m_clus_nrows->push_back(ref_nrow);
      m_clus_strip->push_back(bary_strip/bary_sum-fmod(bary_strip/bary_sum,0.5));
    }
  }


  // Clusters are built, now look at the occupancies

  int chip_idx;
  std::vector<std::vector <int> > chip_mult;
  bool found;
  std::vector<int> chip_st;

  chip_mult.clear();
  m_clus_rank->clear();

  for (unsigned int i=0;i<m_clus_layer->size();++i) 
  {
    chip_idx=0;
    chip_idx+=(m_clus_layer->at(i)-1)*1000000;
    chip_idx+=(m_clus_ladder->at(i)-1)*10000;
    chip_idx+=((m_clus_module->at(i)-2+m_clus_module->at(i)%2)/2)*100;
    chip_idx+=8*(2*m_clus_seg->at(i))/m_clus_PS->at(i);
    chip_idx+=m_clus_strip->at(i)/(m_clus_nrows->at(i)/8);


    // Is this idx already registered?
    
    found=false;

    for (unsigned int k=0;k<chip_mult.size();++k) 
    {  
      if (found) continue;

      if (chip_idx==chip_mult.at(k).at(0))
      {
	found=true;
	chip_mult.at(k).at(1)+=1;
      }
    }

    if (!found)
    {
      chip_st.clear(); 
      chip_st.push_back(chip_idx); 
      chip_st.push_back(1); 
      chip_mult.push_back(chip_st);
    }
  }

  for (unsigned int i=0;i<m_clus_layer->size();++i) 
  {
    chip_idx=0;
    chip_idx+=(m_clus_layer->at(i)-1)*1000000;
    chip_idx+=(m_clus_ladder->at(i)-1)*10000;
    chip_idx+=((m_clus_module->at(i)-2+m_clus_module->at(i)%2)/2)*100;
    chip_idx+=8*(2*m_clus_seg->at(i))/m_clus_PS->at(i);
    chip_idx+=m_clus_strip->at(i)/(m_clus_nrows->at(i)/8);

    found=false;

    for (unsigned int k=0;k<chip_mult.size();++k) 
    {  
      if (found) continue;

      if (chip_idx==chip_mult.at(k).at(0))
      {
	found=true;
	m_clus_rank->push_back(chip_mult.at(k).at(1));
      }
    }
  }
}




/*

Method extracting the stubs using the cluster info

 */


void L1TrackTrigger_analysis::get_stubs(int layer,MCExtractor *mc)
{  
  // These values are extracted from here:
  // https://github.com/cms-sw/cmssw/blob/CMSSW_6_2_0_SLHC5/Geometry/
  // TrackerCommonData/data/PhaseII/BarrelEndcap5D/tracker.xml

  double n_lad_barrel[6]  = {16,24,34,48,62,76};
  double n_lad_endcap[15] = {20,24,28,28,32,36,36,40,40,48,56,64,68,72,80};

  int disk = 0;
  int lad_cor = 0;

  if (layer>10 && layer<=17) disk=(layer-10)%8; // Bet. 11 and 15
  if (layer>17 && layer<=24) disk=(layer-17)%8; // Bet. 18 and 22

  // Here this is the optimized list of SW cuts for the CMSSW_6_2_0_SLHC5 Geometry

  double layer_cut[6]     = {2.5,2.5,3,4.5,5.5,6.5};
  
  if (m_pTthresh>=3)
  {
    layer_cut[0] = 2.;
    layer_cut[1] = 2.;
    layer_cut[2] = 2.5;
    layer_cut[3] = 3.;
    layer_cut[4] = 4.;
    layer_cut[5] = 4.5;
  }

  if (m_pTthresh>=4)
  {
    layer_cut[0] = 2.;
    layer_cut[1] = 2.;
    layer_cut[2] = 2.;
    layer_cut[3] = 2.5;
    layer_cut[4] = 3.;
    layer_cut[5] = 3.5;
  }

  if (m_pTthresh>=5)
  {
    layer_cut[0] = 1.5;
    layer_cut[1] = 1.5;
    layer_cut[2] = 1.5;
    layer_cut[3] = 2.;
    layer_cut[4] = 2.5;
    layer_cut[5] = 3.;
  }

  double ladder_cut[7][15];

  for (int i=0;i<7;++i) 
  {
    ladder_cut[0][0] = 2.;
    ladder_cut[0][1] = 2.;
    ladder_cut[0][2] = 2.;
    ladder_cut[0][3] = 2.;
    ladder_cut[0][4] = 2.5;
    ladder_cut[0][5] = 2.5;
    ladder_cut[0][6] = 2.5;
    ladder_cut[0][7] = 3.;
    ladder_cut[0][8] = 3.5;
    ladder_cut[0][9] = 4.5;
    ladder_cut[0][10]= 3.;
    ladder_cut[0][11]= 3.5;
    ladder_cut[0][12]= 4.;
    ladder_cut[0][13]= 4.5;
    ladder_cut[0][14]= 5.;

    ladder_cut[1][0] = 1.5;
    ladder_cut[1][1] = 2.;
    ladder_cut[1][2] = 2.;
    ladder_cut[1][3] = 2.;
    ladder_cut[1][4] = 2.;
    ladder_cut[1][5] = 2.5;
    ladder_cut[1][6] = 2.5;
    ladder_cut[1][7] = 2.5;
    ladder_cut[1][8] = 3.;
    ladder_cut[1][9] = 4.;
    ladder_cut[1][10]= 2.5;
    ladder_cut[1][11]= 3.;
    ladder_cut[1][12]= 3.5;
    ladder_cut[1][13]= 4.;
    ladder_cut[1][14]= 4.5;

    ladder_cut[2][0] = 1.5;
    ladder_cut[2][1] = 2.;
    ladder_cut[2][2] = 2.;
    ladder_cut[2][3] = 2.;
    ladder_cut[2][4] = 2.;
    ladder_cut[2][5] = 2.;
    ladder_cut[2][6] = 2.5;
    ladder_cut[2][7] = 2.5;
    ladder_cut[2][8] = 2.5;
    ladder_cut[2][9] = 3.5;
    ladder_cut[2][10]= 4.;
    ladder_cut[2][11]= 2.5;
    ladder_cut[2][12]= 3.;
    ladder_cut[2][13]= 3.5;
    ladder_cut[2][14]= 4.;

    ladder_cut[3][0] = 1.5;
    ladder_cut[3][1] = 1.5;
    ladder_cut[3][2] = 1.5;
    ladder_cut[3][3] = 2.;
    ladder_cut[3][4] = 2.;
    ladder_cut[3][5] = 2.;
    ladder_cut[3][6] = 2.;
    ladder_cut[3][7] = 2.5;
    ladder_cut[3][8] = 2.5;
    ladder_cut[3][9] = 3.;
    ladder_cut[3][10]= 3.5;
    ladder_cut[3][11]= 2.5;
    ladder_cut[3][12]= 2.5;
    ladder_cut[3][13]= 3.;
    ladder_cut[3][14]= 3.5;

    ladder_cut[4][0] = 1.5;
    ladder_cut[4][1] = 1.5;
    ladder_cut[4][2] = 1.5;
    ladder_cut[4][3] = 1.5;
    ladder_cut[4][4] = 1.5;
    ladder_cut[4][5] = 2.;
    ladder_cut[4][6] = 2.;
    ladder_cut[4][7] = 2.;
    ladder_cut[4][8] = 2.5;
    ladder_cut[4][9] = 2.5;
    ladder_cut[4][10]= 3.;
    ladder_cut[4][11]= 3.5;
    ladder_cut[4][12]= 2.5;
    ladder_cut[4][13]= 2.5;
    ladder_cut[4][14]= 3.;

    // The two last disk are not used anymore

    ladder_cut[5][0] = 0.;
    ladder_cut[5][1] = 0.;
    ladder_cut[5][2] = 0.;
    ladder_cut[5][3] = 0.;
    ladder_cut[5][4] = 0.;
    ladder_cut[5][5] = 0.;
    ladder_cut[5][6] = 0.;
    ladder_cut[5][7] = 0.;
    ladder_cut[5][8] = 0.;
    ladder_cut[5][9] = 0.;
    ladder_cut[5][10]= 0.;
    ladder_cut[5][11]= 0.;
    ladder_cut[5][12]= 0.;
    ladder_cut[5][13]= 0.;
    ladder_cut[5][14]= 0.;

    ladder_cut[6][0] = 0.;
    ladder_cut[6][1] = 0.;
    ladder_cut[6][2] = 0.;
    ladder_cut[6][3] = 0.;
    ladder_cut[6][4] = 0.;
    ladder_cut[6][5] = 0.;
    ladder_cut[6][6] = 0.;
    ladder_cut[6][7] = 0.;
    ladder_cut[6][8] = 0.;
    ladder_cut[6][9] = 0.;
    ladder_cut[6][10]= 0.;
    ladder_cut[6][11]= 0.;
    ladder_cut[6][12]= 0.;
    ladder_cut[6][13]= 0.;
    ladder_cut[6][14]= 0.;
  }

  if (m_window_size!=-1) // If precised one could apply a generic cut
  {
    for (int i=0;i<6;++i)  layer_cut[i]  = m_window_size;
    for (int i=0;i<15;++i) 
    {  
      for (int j=0;j<7;++j) ladder_cut[j][i] = m_window_size;
    }
  }
  //else 
  //{
   // std::cout << "Applying the optimal SW windows cuts" << std::endl;
  //}


  double B  = 3.8;
  int n_stub=0;
	
  double d_b = 0.;
  double d_t = 0.;

  int i_t = 0;
  int i_b = 0;

  int i_ts = 0;
  int i_bs = 0;

  double dR = 0.;
  double R  = 0.;

  double R1   = 0.;
  double R2   = 0.;
  double Z1   = 0.;
  double Z2   = 0.;
  double phi1 = 0.;
  double phi2 = 0.;


  double d_i = 0.;
  double d_j = 0.;

  double philadder = 0.;


  double dphi    = 0.;
  double pt_stub = 0.;


  double strip_cor= 0.;
  double strip_cor_chosen= 0.;
  double SW       = 0.;
  double SW_min   = 0.;
  double SW_min_s = 0.;
  int matching_tp = -1;

  int n_candidates = 0;
 
  double PI = 4.*atan(1.);

  bool found;

  std::vector<std::vector<int> > chip_ranks;
  chip_ranks.clear();

  int chip_idx;
  std::vector<int> chip_st; 

  for (int i=0;i<m_clus;++i) // Loop over clusters
  {
    n_candidates    = 0;

    // Selection
    if (m_clus_layer->at(i)   != layer ||             // Don't use cluster of another layer
	m_clus_module->at(i)%2!= 1     ||             // Start from an odd module
	m_clus_used->at(i)    == 1     ||             // Don't use already used clusters 
	m_clus_nstrips->at(i) >  m_max_wclus ||       // Cut on cluster width
	(m_clus_matched->at(i) == 0 && m_matchStubs)) // Use only matched clusters (for tests)
      continue;    

    if (m_verb)
      std::cout << "Dealing with clus " << i << endl;

    phi1  = atan2(m_clus_y->at(i),m_clus_x->at(i));
    R1    = sqrt(m_clus_x->at(i)*m_clus_x->at(i)+m_clus_y->at(i)*m_clus_y->at(i));  
    Z1    = m_clus_z->at(i);  

    if (disk==0) // Barrel
    {
      philadder  = 2.*PI/n_lad_barrel[m_clus_layer->at(i)-5]*(m_clus_ladder->at(i)-1);
      SW_min     = layer_cut[layer-5]+0.1;
      d_i        = R1*cos(phi1-philadder);
    }
    else // Endcap
    {
      philadder  = 2.*PI/n_lad_endcap[m_clus_ladder->at(i)-1+lad_cor]*((m_clus_module->at(i)-1)/2);  
      SW_min     = ladder_cut[disk-1][m_clus_ladder->at(i)+lad_cor-1]+0.1;
      d_i        = fabs(Z1);
    }


    for (int j=i+1;j<m_clus;++j) // Loop over other clusters
    {      
      if (m_clus_layer->at(j) !=layer ||     
	  m_clus_used->at(j)  ==1     ||                  
	  m_clus_ladder->at(i)!=m_clus_ladder->at(j) ||    
	  (m_clus_module->at(j)-m_clus_module->at(i))!=1 ||
	  m_clus_nstrips->at(j)>m_max_wclus ||          
	  (m_clus_matched->at(j)==0 && m_matchStubs))       
	continue;  

      //Special treatment for the Z segment matching

      // SS module, no bypass
      if (m_clus_PS->at(i)==m_clus_PS->at(j) && 
	  (2*m_clus_seg->at(i))/m_clus_PS->at(i) !=  (2*m_clus_seg->at(j))/m_clus_PS->at(j)) 
	continue;

      // PS module, bypass if the m_zMatch tag is set to false
      if (m_clus_PS->at(i)!=m_clus_PS->at(j) && m_zMatch &&
	  (2*m_clus_seg->at(i))/m_clus_PS->at(i) !=  (2*m_clus_seg->at(j))/m_clus_PS->at(j)) 
	continue;

      if (getMatchingTP(i,j)!=0 && m_matchStubs) continue;  

      // Here we have two consecutive clusters on the same module in 
      // the same phi range, there are good stub candidates

      if (m_verb)
	std::cout << "...and with clus " << j << endl;

      phi2  = atan2(m_clus_y->at(j),m_clus_x->at(j));
      R2    = sqrt(m_clus_x->at(j)*m_clus_x->at(j)+m_clus_y->at(j)*m_clus_y->at(j));  
      Z2    = m_clus_z->at(j);  

      d_j = R2*cos(phi2-philadder);
      if (disk!=0) d_j = fabs(Z2);

      if (d_i<d_j)
      {
	d_t = d_j;
	d_b = d_i;
	i_t = j;
	i_b = i;
      }
      else
      {
	d_t = d_i;
	d_b = d_j;
	i_t = i;
	i_b = j;
      }

      strip_cor = (d_t/d_b-1.)*(m_clus_strip->at(i_b)-m_clus_nrows->at(i_b)/2.+0.5);

      if (strip_cor<0)
      {
	strip_cor = -floor(fabs(2*strip_cor))/2.;
      }
      else
      {
	strip_cor = floor(fabs(2*strip_cor))/2.;
      }
      

      SW = m_clus_strip->at(i_t) - m_clus_strip->at(i_b); 
      SW -= strip_cor; 

      // Here we apply the stub width cut
      if (fabs(SW)>SW_min) continue;

      ++n_candidates;

      // If more than one cand, take the closest one
      SW_min=fabs(SW);
      strip_cor_chosen=strip_cor;
      SW_min_s=SW;
      i_ts = i_t;
      i_bs = i_b;

    } // End of loop over second cluster

    // We didn't found the stub, we stop
    if (n_candidates==0) continue;

    if (m_verb)
    {
      std::cout << "Found " << n_candidates << " pair(s)..." << endl;
      std::cout << "Bottom/Top: " << i_bs << "/" << i_ts << endl;
      std::cout << "SW = " << SW_min_s << endl;
    }
    
    matching_tp = getMatchingTP(i_ts,i_bs);

    if (m_matchStubs && matching_tp != 0) continue; // We use this option only for bank production

    if (m_verb)
      std::cout << "TP index " << matching_tp << endl;

    // matching_tp is the reference to the tracking particle responsible for the stub
    // clus_chosen contains the one chosen to make the stub (the one minimizing the SW)

    R1   = sqrt(m_clus_x->at(i_bs)*m_clus_x->at(i_bs)+m_clus_y->at(i_bs)*m_clus_y->at(i_bs));
    R2   = sqrt(m_clus_x->at(i_ts)*m_clus_x->at(i_ts)+m_clus_y->at(i_ts)*m_clus_y->at(i_ts));
    phi1 = atan2(m_clus_y->at(i_bs),m_clus_x->at(i_bs));
    phi2 = atan2(m_clus_y->at(i_ts),m_clus_x->at(i_ts));
    dphi = phi2-phi1;   
    dR   = R2-R1;
    SW   = SW_min_s;

    chip_idx=0;
    chip_idx+=(m_clus_layer->at(i)-1)*1000000;
    chip_idx+=(m_clus_ladder->at(i)+lad_cor-1)*10000;
    chip_idx+=((m_clus_module->at(i_bs)-1)/2)*100;
    chip_idx+=8*(2*m_clus_seg->at(i_bs))/m_clus_PS->at(i_bs);
    chip_idx+=m_clus_strip->at(i_bs)/(m_clus_nrows->at(i_bs)/8);

    // Is this idx already registered?
    
    found=false;

    for (unsigned int k=0;k<chip_ranks.size();++k) 
    {  
      if (chip_idx==chip_ranks.at(k).at(0))
      {
	found=true;
	chip_ranks.at(k).push_back(m_stub_pt->size());
	//	break;
      }
    }

    if (!found)
    {
      chip_st.clear(); 
      chip_st.push_back(chip_idx); 
      chip_st.push_back(m_stub_pt->size()); 
      chip_ranks.push_back(chip_st);
    }
    //    cout << chip_idx << " / " << chip_ranks.size() << endl; 

    if (m_verb) 
      cout << m_stub << " / " 
	   << m_clus_layer->at(i_bs) << " / " 
	   << m_clus_ladder->at(i_bs) << " / " 
	   << i_bs << " / " 
	   << SW << " / " 
	   << m_clus_strip->at(i_bs) << endl;

    if (dphi<0.)    dphi = -dphi;
    if (dphi>PI)    dphi = 2.*PI-dphi;
  

    if (m_clus_used->at(i_bs))
      std::cout<< "Problem!" << std::endl;

    m_clus_used->at(i_bs) = 1;
    if (m_clus_PS->at(i_bs)!=32) m_clus_used->at(i_ts) = 1; // This produces double stubs, like in the official maker
    //m_clus_used->at(i_ts) = 1;
    
    pt_stub    = 0.15*B*dR/dphi*sqrt(1+(R*dphi/dR)*(R*dphi/dR))/100.;

    m_stub_pt->push_back(pt_stub);
    m_stub_tp->push_back(matching_tp);

    m_stub_layer->push_back(m_clus_layer->at(i));
    m_stub_module->push_back((m_clus_module->at(i_bs)-1)/2);
    m_stub_ladder->push_back(m_clus_ladder->at(i)+lad_cor-1);
    m_stub_seg->push_back(m_clus_seg->at(i_bs));

    m_stub_modid->push_back(1000000*m_clus_layer->at(i)+10000*(m_clus_ladder->at(i)+lad_cor-1)
			    +100*((m_clus_module->at(i_bs)-1)/2)+m_clus_seg->at(i_bs));

    m_stub_strip->push_back(m_clus_strip->at(i_bs)); 
    m_stub_chip->push_back(m_clus_strip->at(i_bs)/(m_clus_nrows->at(i_bs)/8)); 
    m_stub_clust1->push_back(i_bs);
    m_stub_clust2->push_back(i_ts);
    m_stub_cw1->push_back(m_clus_nstrips->at(i_bs));
    m_stub_cw2->push_back(m_clus_nstrips->at(i_ts));

    m_stub_x->push_back(m_clus_x->at(i_bs));
    m_stub_y->push_back(m_clus_y->at(i_bs));
    m_stub_z->push_back(m_clus_z->at(i_bs));
 
    m_stub_deltas->push_back(SW);
    m_stub_cor->push_back(strip_cor_chosen);
    m_stub_rank->push_back(0);

    if (matching_tp!=-1)
    {
      m_stub_ptGEN->push_back(sqrt(mc->getTP_px(matching_tp)*mc->getTP_px(matching_tp)+mc->getTP_py(matching_tp)*mc->getTP_py(matching_tp)));
      m_stub_pxGEN->push_back(mc->getTP_px(matching_tp));
      m_stub_pyGEN->push_back(mc->getTP_py(matching_tp));
      m_stub_etaGEN->push_back(asinh(mc->getTP_pz(matching_tp)/sqrt(mc->getTP_px(matching_tp)*mc->getTP_px(matching_tp)
+mc->getTP_py(matching_tp)*mc->getTP_py(matching_tp))));
      m_stub_pdg->push_back(mc->getTP_ID(matching_tp));
      m_stub_pid->push_back(0);
      m_stub_X0->push_back(mc->getTP_x(matching_tp));
      m_stub_Y0->push_back(mc->getTP_y(matching_tp));
      m_stub_Z0->push_back(mc->getTP_z(matching_tp));
      m_stub_PHI0->push_back(atan2(mc->getTP_py(matching_tp),mc->getTP_px(matching_tp)));

      if (m_verb)
	cout << m_stub << " / "
	     << pt_stub << " / "
	     << sqrt(mc->getTP_px(matching_tp)*mc->getTP_px(matching_tp)+
		     mc->getTP_py(matching_tp)*mc->getTP_py(matching_tp))
	     << endl;

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

    ++m_stub; 
    ++n_stub;   
  }


  // End of loop over stub, time to fill the ranks

  if (n_stub<2) return;

  if (m_verb) 
    cout << "Found " << n_stub << " stubs belonging to " << chip_ranks.size() << " chips" << endl; 

  std::vector<int> rank;
  std::vector<float> bends;
  std::vector<float> strip;

  float b_min;
  float s_min;
  unsigned int   i_min;

  for (unsigned int k=0;k<chip_ranks.size();++k) 
  { 
    chip_st=chip_ranks.at(k); 

    if (m_verb) 
      cout << "Chip " << chip_st.at(0) << " contains " << chip_st.size()-1 << " stubs" << endl; 

    if (chip_st.size()==2) continue;

    rank.clear();
    bends.clear();
    strip.clear();

    for (unsigned int l=1;l<chip_st.size();++l) 
    { 
      rank.push_back(chip_st.at(l));
      bends.push_back(fabs(m_stub_deltas->at(chip_st.at(l))));
      strip.push_back(m_stub_strip->at(chip_st.at(l)));
    }

    // We now have the list, we do the sorting


    for (unsigned int l=0;l<rank.size();++l) 
    { 
      i_min=l;
      b_min=bends.at(l);
      s_min=strip.at(l);

      for (unsigned int ll=l;ll<rank.size();++ll) 
      { 
	if (bends.at(ll)<b_min)
	{
	  i_min=ll;
	  b_min=bends.at(ll);
	  s_min=strip.at(ll);
	}
	if (bends.at(ll)==b_min && strip.at(ll)<s_min)
	{
	  i_min=ll;
	  b_min=bends.at(ll);
	  s_min=strip.at(ll);
	}
      }
      
      if (i_min!=l)
      {
	rank.at(i_min)=rank.at(l);
	bends.at(i_min)=bends.at(l);
	strip.at(i_min)=strip.at(l);
	
	rank.at(l)=i_min;
	bends.at(l)=b_min;
	strip.at(l)=s_min;
      }
    }


    for (unsigned int l=0;l<rank.size();++l) 
    {
      if (m_verb) 
	cout << rank.at(l) << " / " << bends.at(l) << " / " << strip.at(l) << endl;

      m_stub_rank->at(rank.at(l)) = l;
    }
  }

}


//
// Few technical methods of less importance
//
//


void L1TrackTrigger_analysis::fillTree()
{
  m_tree_L1TrackTrigger->Fill();
  ++n_tot_evt;
}


int L1TrackTrigger_analysis::getMatchingTP(int i, int j)
{
  
  if (m_clus_matched->at(i)==0 || m_clus_matched->at(j)==0) return -1;


  i_match = m_clus_tp->at(i);
  j_match = m_clus_tp->at(j);

  i_match_s = static_cast<int>(i_match.size());
  j_match_s = static_cast<int>(j_match.size());

  for (int ii=0;ii<i_match_s;++ii) // Loop over other clusters
  { 
    if (i_match.at(ii)==-1) continue;

    for (int jj=0;jj<j_match_s;++jj) // Loop over other clusters
    {	
      if (j_match.at(jj)==-1) continue;

      if (i_match.at(ii) == j_match.at(jj)) return i_match.at(ii);	     
    }
  }
 
  return -1;
}


bool L1TrackTrigger_analysis::is_neighbour(PixelExtractor *pix, int idx, int lay, int lad, int mod)
{
  if (pix->layer(idx) !=lay) return false;
  if (pix->ladder(idx)!=lad) return false;
  if (pix->module(idx)!=mod) return false;
  
  int drow = 0;
  int dseg = 0;
  
  int nrow = static_cast<int>(clus_row.size());

  for (int j=0;j<nrow;++j) // Loop over already stored digis
  { 
    drow = abs(clus_row.at(j)-pix->row(idx));
    dseg = abs(clus_col.at(j)-pix->column(idx));

    if (drow <= 1 && dseg == 0) return true;
    if (drow == 0 && dseg <= 1 && pix->ncolumn(idx)>2) return true;
  }
  
  return false;
}


//
// In this method one initialize all the infos which will be stored in
// the final data ntuple
//

void L1TrackTrigger_analysis::initialize()
{  
  m_digi_ref     = new  std::vector<int>;
  m_digi_tp      = new  std::vector< std::vector<int> >; 
  m_digi_modids  = new  std::vector< std::vector<int> >; 

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
  m_clus_rank    = new  std::vector<int>;  
  m_clus_tp      = new  std::vector< std::vector<int> >;  
  m_clus_hits    = new  std::vector< std::vector<int> >; 
  m_clus_pix     = new  std::vector< std::vector<int> >;  

  m_stub_pt      = new  std::vector<float>;
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
  m_stub_strip   = new  std::vector<float>; 
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
  m_stub_rank    = new  std::vector<int>;  
  m_stub_ptGEN   = new  std::vector<float>;  
  m_stub_modid   = new  std::vector<int>;  


  L1TrackTrigger_analysis::reset();


  // Then the rootuple

  m_tree_L1TrackTrigger = new TTree("L1TrackTrigger","L1TrackTrigger Analysis info");  
  
  /// Branches definition

  // If we don't request only matched stubs, we keep all the info
  // otherwise we skim the data file (useful for BANK generation)

  if (!m_matchStubs)
  {
    m_tree_L1TrackTrigger->Branch("evt", &n_tot_evt); // Simple evt number or event ID

    m_tree_L1TrackTrigger->Branch("CLUS_n",         &m_clus);
    m_tree_L1TrackTrigger->Branch("CLUS_x",         &m_clus_x);
    m_tree_L1TrackTrigger->Branch("CLUS_y",         &m_clus_y);
    m_tree_L1TrackTrigger->Branch("CLUS_z",         &m_clus_z);
    m_tree_L1TrackTrigger->Branch("CLUS_xmc",       &m_clus_xmc);
    m_tree_L1TrackTrigger->Branch("CLUS_ymc",       &m_clus_ymc);
    m_tree_L1TrackTrigger->Branch("CLUS_zmc",       &m_clus_zmc);
    m_tree_L1TrackTrigger->Branch("CLUS_charge",    &m_clus_e);
    m_tree_L1TrackTrigger->Branch("CLUS_layer",     &m_clus_layer);
    m_tree_L1TrackTrigger->Branch("CLUS_module",    &m_clus_module);
    m_tree_L1TrackTrigger->Branch("CLUS_ladder",    &m_clus_ladder);
    m_tree_L1TrackTrigger->Branch("CLUS_seg",       &m_clus_seg);
    m_tree_L1TrackTrigger->Branch("CLUS_strip",     &m_clus_strip);
    m_tree_L1TrackTrigger->Branch("CLUS_nstrip",    &m_clus_nstrips);
    m_tree_L1TrackTrigger->Branch("CLUS_nsat",      &m_clus_sat);
    m_tree_L1TrackTrigger->Branch("CLUS_match",     &m_clus_matched);
    m_tree_L1TrackTrigger->Branch("CLUS_PS",        &m_clus_PS);
    m_tree_L1TrackTrigger->Branch("CLUS_nrows",     &m_clus_nrows);
    m_tree_L1TrackTrigger->Branch("CLUS_tp",        &m_clus_tp);
    m_tree_L1TrackTrigger->Branch("CLUS_hits",      &m_clus_hits);
    m_tree_L1TrackTrigger->Branch("CLUS_pix" ,      &m_clus_pix);
    m_tree_L1TrackTrigger->Branch("CLUS_process",   &m_clus_pid);
    m_tree_L1TrackTrigger->Branch("CLUS_rank",      &m_clus_rank);

    m_tree_L1TrackTrigger->Branch("STUB_clust1",    &m_stub_clust1);
    m_tree_L1TrackTrigger->Branch("STUB_clust2",    &m_stub_clust2);
    m_tree_L1TrackTrigger->Branch("STUB_cw1",       &m_stub_cw1);
    m_tree_L1TrackTrigger->Branch("STUB_cw2",       &m_stub_cw2);
    m_tree_L1TrackTrigger->Branch("STUB_cor",       &m_stub_cor);
    m_tree_L1TrackTrigger->Branch("STUB_PHI0",      &m_stub_PHI0);
    m_tree_L1TrackTrigger->Branch("STUB_tp",        &m_stub_tp);
    m_tree_L1TrackTrigger->Branch("STUB_pdgID",     &m_stub_pdg);
    m_tree_L1TrackTrigger->Branch("STUB_process",   &m_stub_pid);
    m_tree_L1TrackTrigger->Branch("STUB_chip",      &m_stub_chip);
    m_tree_L1TrackTrigger->Branch("STUB_rank",      &m_stub_rank);

    m_tree_L1TrackTrigger->Branch("STUB_pt",        &m_stub_pt);
    m_tree_L1TrackTrigger->Branch("STUB_x",         &m_stub_x);
    m_tree_L1TrackTrigger->Branch("STUB_y",         &m_stub_y);
    m_tree_L1TrackTrigger->Branch("STUB_z",         &m_stub_z);
    m_tree_L1TrackTrigger->Branch("STUB_deltas",    &m_stub_deltas);
    m_tree_L1TrackTrigger->Branch("STUB_X0",        &m_stub_X0);
    m_tree_L1TrackTrigger->Branch("STUB_Y0",        &m_stub_Y0);
    m_tree_L1TrackTrigger->Branch("STUB_Z0",        &m_stub_Z0);

    m_tree_L1TrackTrigger->Branch("STUB_pxGEN",     &m_stub_pxGEN);
    m_tree_L1TrackTrigger->Branch("STUB_pyGEN",     &m_stub_pyGEN);
    m_tree_L1TrackTrigger->Branch("STUB_layer",     &m_stub_layer);
    m_tree_L1TrackTrigger->Branch("STUB_module",    &m_stub_module);
    m_tree_L1TrackTrigger->Branch("STUB_ladder",    &m_stub_ladder);
    m_tree_L1TrackTrigger->Branch("STUB_seg",       &m_stub_seg);
  }

  m_tree_L1TrackTrigger->Branch("STUB_n",           &m_stub);
  m_tree_L1TrackTrigger->Branch("STUB_ptGEN",       &m_stub_ptGEN);
  m_tree_L1TrackTrigger->Branch("STUB_etaGEN",      &m_stub_etaGEN);
  m_tree_L1TrackTrigger->Branch("STUB_modid",       &m_stub_modid);
  m_tree_L1TrackTrigger->Branch("STUB_strip",       &m_stub_strip);

}



void L1TrackTrigger_analysis::reset()
{
  m_clus = 0;
  m_stub = 0;

  m_digi_tp->clear();
  m_digi_ref->clear();
  m_digi_modids->clear();

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
  m_clus_pix->clear();
  m_clus_pid->clear();
  m_clus_rank->clear();

  m_clus_tp->clear();
  m_clus_hits->clear();
  m_clus_pix->clear();
  
  m_stub_X0->clear();     
  m_stub_Y0->clear();     
  m_stub_Z0->clear();     
  m_stub_PHI0->clear();     
  m_stub_tp->clear();     
  m_stub_pt->clear();     
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
  m_stub_rank->clear();
  m_stub_ptGEN->clear();
  m_stub_modid->clear();
}
