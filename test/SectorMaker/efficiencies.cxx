// Class for the efficiencies determination
// For more info, look at the header file

#include "efficiencies.h"

// Main constructor

efficiencies::efficiencies(std::string filename, std::string outfile)
{
  efficiencies::initTuple(filename,outfile);
  efficiencies::reset();
  efficiencies::initVars();
  efficiencies::get_efficiencies();
}


//////////////////////////////////////////////
//
// 
//////////////////////////////////////////////

void efficiencies::get_efficiencies()
{
  // Initialize some params
 
  double PTGEN=0;
  double D0GEN=0;

  int nh_tot = 0;

  int pix_i;
  int clus_i;
  int stub_i;
 
  int i_lay;
  int i_mod;
  int i_seg;
  float i_eta;

  double dphi = 0;
  double dphi_min = 0;

  int n_entries = L1TT_P->GetEntries();

  // Then loop over events

  for (int j=0;j<n_entries;++j)
  {
    efficiencies::reset();

    PTGEN=0;
    D0GEN=0;

    pix_i=-1;
    clus_i=-1;
    stub_i=-1;

    nh_tot=0;

    if (j%1000==0)
      cout << j <<endl;

    // Principle is the following:
    // We loop over all primary TrackingParticles hits
    // and look if those hits have induced DIGI/CLUSTER/STUBS
    // 
    // Efficiencies are then simply N_object/N_TPhits

    MC->GetEntry(j,1); // Get the MC info
    Pix->GetEntry(j,1);
    L1TT_O->GetEntry(j,1);
    L1TT_P->GetEntry(j,1);

    for (int k=0;k<m_part_n;++k) // Loop over TPs
    {
      D0GEN = sqrt(m_part_x->at(k)*m_part_x->at(k)+m_part_y->at(k)*m_part_y->at(k));
      if (D0GEN > 0.3) continue; // Not a primary, skip

      PTGEN = sqrt(m_part_px->at(k)*m_part_px->at(k)+m_part_py->at(k)*m_part_py->at(k));

      for (int l=nh_tot;l<m_part_hits->at(k)+nh_tot;++l) // Loop over TP simhits
      {
	if (m_hits_layer->at(l)<5) continue; // Don't care about pixel hits

	dphi_min = 1.;

	i_eta = m_part_eta->at(k);
	i_lay = m_hits_layer->at(l);
	i_mod = m_hits_module->at(l)-1;

	if (PTGEN<20) entries_pt[i_lay-5][static_cast<int>(5*PTGEN)]+=1;   
	if (fabs(i_eta)<2.5 && PTGEN>10) entries_eta[i_lay-5][static_cast<int>(25+10*i_eta)]+=1;   

	i_seg  = -1;
	pix_i  = -1; // The index of the induced DIGI
	clus_i = -1; // The index of the induced CLUSTER
	stub_i = -1; // The index of the induced STUB


	if (m_pclus==0) continue;
	
	// We have the simhits, now look at the digis
	for (int i=0;i<m_pclus;++i)
	{
	  if (pix_i!=-1) break; // We found something
	  if (m_pixclus_layer->at(i)!=i_lay) continue;
	  if (m_pixclus_module->at(i)!=i_mod+1) continue;	      
	  if (m_pixclus_ladder->at(i)!= m_hits_ladder->at(l)) continue;

	  dphi = fabs(atan2(m_hits_y->at(l),m_hits_x->at(l))-atan2(m_pixclus_y->at(i),m_pixclus_x->at(i)));

	  if (dphi>0.002) continue;

	  if (dphi<dphi_min)
	  {
	    dphi_min = dphi;
	    pix_i=i;
	  }
	} // End of loop over digis

	if (pix_i==-1) continue; 

	i_seg = m_pixclus_column->at(pix_i);

	if (PTGEN<20) digi_pt[i_lay-5][static_cast<int>(5*PTGEN)]+=1;   
	if (fabs(i_eta)<2.5 && PTGEN>10) digi_eta[i_lay-5][static_cast<int>(25+10*i_eta)]+=1;   
	
	// We have the digis, now look at the clusters

	// Private tool first
	for (int i=0;i<m_clus;++i)
	{
	  if (clus_i!=-1) break;
	  if (m_clus_nstrips->at(i)>3) continue;
	  if (m_clus_layer->at(i)!=i_lay) continue;
	  if (m_clus_module->at(i)!=i_mod+1) continue;	
	  if (m_clus_seg->at(i)!=i_seg) continue;
	  if (m_clus_ladder->at(i)!=m_hits_ladder->at(l)) continue;
	  if (fabs(m_clus_strip->at(i)-m_pixclus_row->at(pix_i))>m_clus_nstrips->at(i)) continue;
	  clus_i = i;
	}

	
	if (clus_i!=-1) 
	{
	  if (PTGEN<20) clus_pri_pt[i_lay-5][static_cast<int>(5*PTGEN)]+=1;   
	  if (fabs(i_eta)<2.5 && PTGEN>10) clus_pri_eta[i_lay-5][static_cast<int>(25+10*i_eta)]+=1;  
	  
	  // We have the clusters, now look at the stubs
	  for (int i=0;i<m_stub;++i)
	  {
	    if (stub_i!=-1) break;
	    if (m_stub_tp->at(i)!=k) continue;
	    if (m_stub_layer->at(i)!=i_lay) continue;
	    if (m_stub_clust1->at(i)==clus_i) stub_i = i;
	    if (m_stub_clust2->at(i)==clus_i) stub_i = i;
	  }
	  
	  if (stub_i!=-1) 
	  {
	    if (PTGEN<20) stub_pri_pt[i_lay-5][static_cast<int>(5*PTGEN)]+=1;   
	    if (fabs(i_eta)<2.5 && PTGEN>10) stub_pri_eta[i_lay-5][static_cast<int>(25+10*i_eta)]+=1;  
	  }	  
	}
	
	clus_i = -1; //
	stub_i = -1; //

	// Then the official one
	for (int i=0;i<m_tkclus;++i)
	{

	  if (m_tkclus_nstrips->at(i)>3) continue;
	  if (m_tkclus_layer->at(i)!=i_lay) continue;
	  if (m_tkclus_module->at(i)!=i_mod+1) continue;	
	  if (m_tkclus_seg->at(i)!=i_seg) continue;
	  if (m_tkclus_ladder->at(i)!=m_hits_ladder->at(l)) continue;
	  if (fabs(m_tkclus_strip->at(i)-m_pixclus_row->at(pix_i))>m_tkclus_nstrips->at(i)) continue;
	  clus_i = i;
	}

	if (clus_i!=-1) 
	{
	  if (PTGEN<20) clus_off_pt[i_lay-5][static_cast<int>(5*PTGEN)]+=1;   
	  if (fabs(i_eta)<2.5 && PTGEN>10) clus_off_eta[i_lay-5][static_cast<int>(25+10*i_eta)]+=1;  

	  // We have the clusters, now look at the stubs
	  for (int i=0;i<m_tkstub;++i)
	  {
	    if (stub_i!=-1) break;
	    if (m_tkstub_tp->at(i)!=k) continue;
	    if (m_tkstub_layer->at(i)!=i_lay) continue;
	    if (m_tkstub_clust1->at(i)==clus_i) stub_i = i;
	    if (m_tkstub_clust2->at(i)==clus_i) stub_i = i;
	  }

	  if (stub_i!=-1) 
	  {
	    if (PTGEN<20) stub_off_pt[i_lay-5][static_cast<int>(5*PTGEN)]+=1;   
	    if (fabs(i_eta)<2.5 && PTGEN>10) stub_off_eta[i_lay-5][static_cast<int>(25+10*i_eta)]+=1;  
	  }
	}
      } // End of loop over SimHits
    } // End of loop over TPs

    ///    efficiencies::reset();

  } // End of loop over events

  // Finally get the efficiencies

  for (int i=0;i<20;++i) 
  {
    for (int j=0;j<100;++j) 
    {
      if (entries_pt[i][j]!=0.)
      {
	digi_pt[i][j]     = digi_pt[i][j]/entries_pt[i][j]; 
	clus_off_pt[i][j] = clus_off_pt[i][j]/entries_pt[i][j];
	stub_off_pt[i][j] = stub_off_pt[i][j]/entries_pt[i][j];
	clus_pri_pt[i][j] = clus_pri_pt[i][j]/entries_pt[i][j];
	stub_pri_pt[i][j] = stub_pri_pt[i][j]/entries_pt[i][j];
      }     
    }

    for (int j=0;j<50;++j) 
    {
      if (entries_eta[i][j]!=0.)
      {
	digi_eta[i][j]     = digi_eta[i][j]/entries_eta[i][j]; 
	clus_off_eta[i][j] = clus_off_eta[i][j]/entries_eta[i][j];
	stub_off_eta[i][j] = stub_off_eta[i][j]/entries_eta[i][j];
	clus_pri_eta[i][j] = clus_pri_eta[i][j]/entries_eta[i][j];
	stub_pri_eta[i][j] = stub_pri_eta[i][j]/entries_eta[i][j];
      }     
    }
  }

  m_tree->Fill();


  m_outfile->Write();
  m_outfile->Close();
}


void efficiencies::initVars()
{

  for (int j=0;j<100;++j) pt_val[j]=0.2*j;
  for (int j=0;j<50;++j)  eta_val[j]=-2.5+0.1*j;

  for (int i=0;i<20;++i) 
  {
    for (int j=0;j<100;++j) 
    {
      digi_pt[i][j]=0.;
      entries_pt[i][j]=0.;     

      clus_off_pt[i][j]=0.;
      stub_off_pt[i][j]=0.;
      clus_pri_pt[i][j]=0.;
      stub_pri_pt[i][j]=0.;
    }

    for (int j=0;j<50;++j) 
    {
      digi_eta[i][j]=0.;
      entries_eta[i][j]=0.;     

      clus_off_eta[i][j]=0.;
      stub_off_eta[i][j]=0.;
      clus_pri_eta[i][j]=0.;
      stub_pri_eta[i][j]=0.;
    }
  }

}

void efficiencies::reset()
{
  m_pixclus_row->clear();
  m_pixclus_column->clear();
  m_pixclus_layer->clear();
  m_pixclus_module->clear();
  m_pixclus_ladder->clear();
  m_pixclus_x->clear();
  m_pixclus_y->clear();
  
  m_part_hits->clear();
  m_part_px->clear();
  m_part_py->clear();
  m_part_eta->clear();
  m_part_x->clear();
  m_part_y->clear();
  m_hits_x->clear();
  m_hits_y->clear();
  m_hits_layer->clear();
  m_hits_ladder->clear();
  m_hits_module->clear();
  
  m_clus_nstrips->clear();
  m_clus_layer->clear(); 
  m_clus_module->clear();								       
  m_clus_ladder->clear();				      
  m_clus_seg->clear();   
  m_clus_strip->clear();
  m_stub_layer->clear();
  m_stub_ladder->clear();
  m_stub_clust1->clear();
  m_stub_clust2->clear();
  m_stub_tp->clear();
   
  m_tkclus_nstrips->clear();
  m_tkclus_layer->clear(); 
  m_tkclus_module->clear();								       
  m_tkclus_ladder->clear();				      
  m_tkclus_seg->clear();   
  m_tkclus_strip->clear();
  m_tkstub_layer->clear();
  m_tkstub_ladder->clear();
  m_tkstub_clust1->clear();
  m_tkstub_clust2->clear();
  m_tkstub_tp->clear();
}


void efficiencies::initTuple(std::string in,std::string out)
{

  L1TT_P  = new TChain("L1TrackTrigger"); 
  L1TT_O  = new TChain("TkStubs"); 
  Pix     = new TChain("Pixels");   
  MC      = new TChain("MC");   
 
  L1TT_P->Add(in.c_str());
  L1TT_O->Add(in.c_str());
  Pix->Add(in.c_str());
  MC->Add(in.c_str()); 

  m_pixclus_row      = new std::vector<int>;      
  m_pixclus_column   = new std::vector<int>;      
  m_pixclus_layer    = new std::vector<int>;    
  m_pixclus_module   = new std::vector<int>; 
  m_pixclus_ladder   = new std::vector<int>;    
  m_pixclus_x        = new std::vector<float>;  
  m_pixclus_y        = new std::vector<float>; 

  m_part_hits   = new std::vector<int>; 
  m_part_px     = new std::vector<float>; 
  m_part_py     = new std::vector<float>; 
  m_part_eta    = new std::vector<float>;   
  m_part_x      = new std::vector<float>;   
  m_part_y      = new std::vector<float>;   
 

  m_hits_x      = new std::vector<float>;  
  m_hits_y      = new std::vector<float>;  
  m_hits_layer  = new std::vector<int>;    
  m_hits_ladder = new std::vector<int>;   
  m_hits_module = new std::vector<int>;   

  m_clus_layer   = new  std::vector<int>;
  m_clus_module  = new  std::vector<int>;  
  m_clus_ladder  = new  std::vector<int>;  
  m_clus_seg     = new  std::vector<int>; 
  m_clus_strip   = new  std::vector<float>; 
  m_clus_nstrips = new  std::vector<int>; 
  
  m_stub_layer   = new  std::vector<int>; 
  m_stub_ladder  = new  std::vector<int>; 
  m_stub_clust1  = new  std::vector<int>;  
  m_stub_clust2  = new  std::vector<int>;  
  m_stub_tp      = new  std::vector<int>;  

  m_tkclus_layer   = new  std::vector<int>;
  m_tkclus_module  = new  std::vector<int>;  
  m_tkclus_ladder  = new  std::vector<int>;  
  m_tkclus_seg     = new  std::vector<int>; 
  m_tkclus_strip   = new  std::vector<float>; 
  m_tkclus_nstrips = new  std::vector<int>; 
  
  m_tkstub_layer   = new  std::vector<int>; 
  m_tkstub_ladder  = new  std::vector<int>; 
  m_tkstub_clust1  = new  std::vector<int>;  
  m_tkstub_clust2  = new  std::vector<int>;  
  m_tkstub_tp      = new  std::vector<int>; 


  Pix->SetBranchStatus("*",0);
  MC->SetBranchStatus("*",0);
  L1TT_O->SetBranchStatus("*",0);
  L1TT_P->SetBranchStatus("*",0);

  Pix->SetBranchStatus("PIX_n",1);
  Pix->SetBranchStatus("PIX_layer",1);   
  Pix->SetBranchStatus("PIX_module",1);  
  Pix->SetBranchStatus("PIX_ladder",1);  
  Pix->SetBranchStatus("PIX_x",1);       
  Pix->SetBranchStatus("PIX_y",1);       
  Pix->SetBranchStatus("PIX_row",1);     
  Pix->SetBranchStatus("PIX_column",1);  

  MC->SetBranchStatus("subpart_nhit",1); 
  MC->SetBranchStatus("subpart_n",1);    
  MC->SetBranchStatus("subpart_x",1);    
  MC->SetBranchStatus("subpart_y",1);    
  MC->SetBranchStatus("subpart_px",1);   
  MC->SetBranchStatus("subpart_py",1);   
  MC->SetBranchStatus("subpart_eta",1);  

  MC->SetBranchStatus("subpart_hits",1);       
  MC->SetBranchStatus("subpart_hits_x",1);     
  MC->SetBranchStatus("subpart_hits_y",1);     
  MC->SetBranchStatus("subpart_hits_layer",1); 
  MC->SetBranchStatus("subpart_hits_ladder",1);
  MC->SetBranchStatus("subpart_hits_module",1);

  L1TT_P->SetBranchStatus("CLUS_n",1);         
  L1TT_P->SetBranchStatus("CLUS_layer",1);     
  L1TT_P->SetBranchStatus("CLUS_module",1);    
  L1TT_P->SetBranchStatus("CLUS_ladder",1);    
  L1TT_P->SetBranchStatus("CLUS_seg",1);       
  L1TT_P->SetBranchStatus("CLUS_strip",1);     
  L1TT_P->SetBranchStatus("CLUS_nstrip",1);    

  L1TT_P->SetBranchStatus("STUB_n",1);         
  L1TT_P->SetBranchStatus("STUB_layer",1);     
  L1TT_P->SetBranchStatus("STUB_ladder",1);    
  L1TT_P->SetBranchStatus("STUB_tp",1);        
  L1TT_P->SetBranchStatus("STUB_clust1",1);     
  L1TT_P->SetBranchStatus("STUB_clust2",1);     

  L1TT_O->SetBranchStatus("L1TkSTUB_n",1);      
  L1TT_O->SetBranchStatus("L1TkSTUB_layer",1);  
  L1TT_O->SetBranchStatus("L1TkSTUB_ladder",1); 
  L1TT_O->SetBranchStatus("L1TkSTUB_tp",1);     
  L1TT_O->SetBranchStatus("L1TkSTUB_clust1",1); 
  L1TT_O->SetBranchStatus("L1TkSTUB_clust2",1); 

  L1TT_O->SetBranchStatus("L1TkCLUS_n",1);      
  L1TT_O->SetBranchStatus("L1TkCLUS_layer",1);  
  L1TT_O->SetBranchStatus("L1TkCLUS_module",1); 
  L1TT_O->SetBranchStatus("L1TkCLUS_ladder",1); 
  L1TT_O->SetBranchStatus("L1TkCLUS_seg",1);    
  L1TT_O->SetBranchStatus("L1TkCLUS_strip",1);  
  L1TT_O->SetBranchStatus("L1TkCLUS_nstrip",1); 

  Pix->SetBranchAddress("PIX_n",         &m_pclus);
  Pix->SetBranchAddress("PIX_layer",     &m_pixclus_layer);
  Pix->SetBranchAddress("PIX_module",    &m_pixclus_module);
  Pix->SetBranchAddress("PIX_ladder",    &m_pixclus_ladder);
  Pix->SetBranchAddress("PIX_x",         &m_pixclus_x);
  Pix->SetBranchAddress("PIX_y",         &m_pixclus_y);
  Pix->SetBranchAddress("PIX_row",       &m_pixclus_row);
  Pix->SetBranchAddress("PIX_column",    &m_pixclus_column);  

  MC->SetBranchAddress("subpart_nhit",     &m_part_nhit); 
  MC->SetBranchAddress("subpart_n",        &m_part_n);    
  MC->SetBranchAddress("subpart_x",        &m_part_x);    
  MC->SetBranchAddress("subpart_y",        &m_part_y);    
  MC->SetBranchAddress("subpart_px",       &m_part_px);   
  MC->SetBranchAddress("subpart_py",       &m_part_py);   
  MC->SetBranchAddress("subpart_eta",      &m_part_eta);  

  MC->SetBranchAddress("subpart_hits",          &m_part_hits); 
  MC->SetBranchAddress("subpart_hits_x",        &m_hits_x);
  MC->SetBranchAddress("subpart_hits_y",        &m_hits_y);
  MC->SetBranchAddress("subpart_hits_layer",    &m_hits_layer);
  MC->SetBranchAddress("subpart_hits_ladder",   &m_hits_ladder);
  MC->SetBranchAddress("subpart_hits_module",   &m_hits_module);

  L1TT_P->SetBranchAddress("CLUS_n",             &m_clus);
  L1TT_P->SetBranchAddress("CLUS_layer",         &m_clus_layer);
  L1TT_P->SetBranchAddress("CLUS_module",        &m_clus_module);
  L1TT_P->SetBranchAddress("CLUS_ladder",        &m_clus_ladder);
  L1TT_P->SetBranchAddress("CLUS_seg",           &m_clus_seg);
  L1TT_P->SetBranchAddress("CLUS_strip",         &m_clus_strip);
  L1TT_P->SetBranchAddress("CLUS_nstrip",        &m_clus_nstrips);

  L1TT_P->SetBranchAddress("STUB_n",             &m_stub);    
  L1TT_P->SetBranchAddress("STUB_layer",         &m_stub_layer);
  L1TT_P->SetBranchAddress("STUB_ladder",        &m_stub_ladder);
  L1TT_P->SetBranchAddress("STUB_tp",            &m_stub_tp);
  L1TT_P->SetBranchAddress("STUB_clust1",        &m_stub_clust1);
  L1TT_P->SetBranchAddress("STUB_clust2",        &m_stub_clust2);

  L1TT_O->SetBranchAddress("L1TkSTUB_n",         &m_tkstub);    
  L1TT_O->SetBranchAddress("L1TkSTUB_layer",     &m_tkstub_layer);
  L1TT_O->SetBranchAddress("L1TkSTUB_ladder",    &m_tkstub_ladder);
  L1TT_O->SetBranchAddress("L1TkSTUB_tp",        &m_tkstub_tp);
  L1TT_O->SetBranchAddress("L1TkSTUB_clust1",    &m_tkstub_clust1);
  L1TT_O->SetBranchAddress("L1TkSTUB_clust2",    &m_tkstub_clust2);

  L1TT_O->SetBranchAddress("L1TkCLUS_n",         &m_tkclus);
  L1TT_O->SetBranchAddress("L1TkCLUS_layer",     &m_tkclus_layer);
  L1TT_O->SetBranchAddress("L1TkCLUS_module",    &m_tkclus_module);
  L1TT_O->SetBranchAddress("L1TkCLUS_ladder",    &m_tkclus_ladder);
  L1TT_O->SetBranchAddress("L1TkCLUS_seg",       &m_tkclus_seg);
  L1TT_O->SetBranchAddress("L1TkCLUS_strip",     &m_tkclus_strip);
  L1TT_O->SetBranchAddress("L1TkCLUS_nstrip",    &m_tkclus_nstrips);


  m_outfile  = new TFile(out.c_str(),"recreate");
  m_tree     = new TTree("Efficiencies","Efficiencies info");

  m_tree->Branch("pt_val",&pt_val,"pt_val[100]");
  m_tree->Branch("eta_val",&eta_val,"eta_val[50]");

  m_tree->Branch("digi_pt",&digi_pt,"digi_pt[20][100]");
  m_tree->Branch("digi_eta",&digi_eta,"digi_eta[20][50]");
  m_tree->Branch("clus_off_pt",&clus_off_pt,"clus_off_pt[20][100]");
  m_tree->Branch("stub_off_pt",&stub_off_pt,"stub_off_pt[20][100]");
  m_tree->Branch("clus_off_eta",&clus_off_eta,"clus_off_eta[20][50]");
  m_tree->Branch("stub_off_eta",&stub_off_eta,"stub_off_eta[20][50]");
  m_tree->Branch("clus_pri_pt",&clus_pri_pt,"clus_pri_pt[20][100]");
  m_tree->Branch("stub_pri_pt",&stub_pri_pt,"stub_pri_pt[20][100]");
  m_tree->Branch("clus_pri_eta",&clus_pri_eta,"clus_pri_eta[20][50]");
  m_tree->Branch("stub_pri_eta",&stub_pri_eta,"stub_pri_eta[20][50]");
}
