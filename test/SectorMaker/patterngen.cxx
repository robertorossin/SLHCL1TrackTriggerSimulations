// Class for the rates determination
// For more info, look at the header file

#include "patterngen.h"

// Main constructor

patterngen::patterngen(std::string filename, std::string outfile,int npatt)
{
  patterngen::initTuple(filename,outfile);
  patterngen::get_patterns(npatt);
}


//////////////////////////////////////////////
//
// 
//////////////////////////////////////////////

void patterngen::get_patterns(int npatt)
{
  // Initialize some params
 
  int B_id,E_id; // The detector module IDs (defined in the header)

  int disk;
  int layer,ladder,module,chip;

  int seg,nseg;

  int n_entries = L1TT->GetEntries();

  // Then loop over events

  std::vector<std::vector <int> > evt_mix;

  std::vector<int> evt_signature;
  std::vector<int> evt_tmp;

  evt_mix.clear();

  int mod_num  = 31020;
  int seg_side = 1;
  int n_seq    = npatt;

  int pt = 0;

  for (int j=0;j<n_entries;++j)
  {    
    evt_signature.clear();
    evt_tmp.clear();

    evt_signature.push_back(j);

    L1TT->GetEntry(j); 
 
    //    if (j%100==0) cout << j << endl;

    if (m_stub == 0) continue; // No stubs, don't go further

    for (int i=0;i<m_stub;++i)
    {  
      // First of all we compute the ID of the stub's module

      nseg  = m_clus_nseg[m_stub_clust1[i]];

      if (nseg>2) continue; // Only 2S modules for the moment

      disk  = 0;
      layer = m_stub_layer[i]; 

      if (layer>10 && layer<=17) disk=(layer-10)%8;
      if (layer>17 && layer<=24) disk=(layer-17)%8+7;

      ladder= m_stub_ladder[i]; 
      module= m_stub_module[i]; 
      seg   = m_stub_seg[i]; 
      chip  = m_stub_chip[i]; 

      //      cout << layer << " / " << m_stub_pt.at(i) << endl;


      pt = abs(static_cast<int>(4*m_stub_deltas.at(i)));
      if (pt>31) pt = 31;       

      if (layer>10 && layer<=17) disk=(layer-10)%8;
      if (layer>17 && layer<=24) disk=(layer-17)%8+7;
         
      B_id = (layer-5)*10000 + ladder*100 + module;
      E_id = (disk-1)*10000 + ladder*100 + module;

      if (disk == 0 && B_id == mod_num && seg/(nseg/2)==seg_side)
      {
	evt_tmp.push_back(chip);
	evt_tmp.push_back(m_stub_strip[i]%128);
	evt_tmp.push_back(pt);

	//	cout << m_stub_pt.at(i) << " / " << pt << endl;
      }
    }
  

    // evt_tmp contains all the info related to the 8 CBC chips considered

    // We now apply the CBC filtering 
    // If more than 3 stubs in a chip, skip the others. Keep only the highest pt ones
    //
    // Store everything, in chip order, in evt_signature

    if (evt_tmp.size()!=0)
    {    
      int nstubs=static_cast<int>(evt_tmp.size())/3;

      int idx[3],ptrank[3];
      
      int npchip;

      /*
      cout << "evt " << j << " / " << nstubs << " stub(s) " << endl;

      for (int k=0;k<nstubs;++k)
      {
	cout << "stub " << k << " / " << evt_tmp.at(3*k)
	     << " / " << evt_tmp.at(3*k+1) 
	     << " / " << evt_tmp.at(3*k+2) << endl;
      }
      */

      for (int i=0;i<8;++i)
      { 
	for (int k=0;k<3;++k) idx[k]=-1;
	for (int k=0;k<3;++k) ptrank[k]=-1;
	npchip=0;

	for (int k=0;k<nstubs;++k)
	{
	  if (evt_tmp.at(3*k)!=i) continue;
	  
	  if (evt_tmp.at(3*k+2)>ptrank[0])
	  {
	    idx[2]    = idx[1];
	    ptrank[2] = ptrank[1];
	    idx[1]    = idx[0];
	    ptrank[1] = ptrank[0];
	    idx[0]    = k;
	    ptrank[0] = evt_tmp.at(3*k+2);	
	  }
	  else if (evt_tmp.at(3*k+2)>ptrank[1])
	  {
	    idx[2]    = idx[1];
	    ptrank[2] = ptrank[1];
	    idx[1]    = k;
	    ptrank[1] = evt_tmp.at(3*k+2);
	  }
	  else if (evt_tmp.at(3*k+2)>ptrank[2])
	  {
	    idx[2]    = k;
	    ptrank[2] = evt_tmp.at(3*k+2);
	  }
	}

	if (idx[0]!=-1 && idx[1]==-1) npchip=1;
	if (idx[0]!=-1 && idx[1]!=-1 && idx[2]==-1) npchip=2;
	if (idx[0]!=-1 && idx[1]!=-1 && idx[2]!=-1) npchip=3;

	evt_signature.push_back(-(i+1));
	evt_signature.push_back(npchip);
      
	for (int k=0;k<npchip;++k)
	{
	  evt_signature.push_back(evt_tmp.at(3*idx[k]+1));
	  evt_signature.push_back(evt_tmp.at(3*idx[k]+2));
	}
      }
    }

    evt_mix.push_back(evt_signature);
  }

  
  int n_data = evt_mix.size(); 
  /* initialize random seed: */
  srand (time(NULL));

  int evnum;
  std::vector<int> cbc;

  // Create the sequence

  for (int i=0;i<n_seq;++i)
  { 
    m_outbinary << "## Event train " << i << "\n";

    for (int j=0;j<8;++j)
    { 
      evnum= rand()%n_data;
      cbc = evt_mix.at(evnum);

      m_outbinary << "## CBC content for event " << std::bitset<3>(j) << " (" << cbc.at(0) << ") is: " << "\n";

      for (unsigned int j=1;j<cbc.size();++j)
      {	
	if (cbc.at(j)<0)
	{
	  m_outbinary << "##Chip " << std::bitset<3>(-cbc.at(j)-1) << "  " ;	  

	  if (cbc.at(j+1)>0)
	  {
	    for (int k=0;k<cbc.at(j+1);++k)
	    {
	      m_outbinary << std::bitset<8>(cbc.at(j+2+2*k)) << std::bitset<5>(cbc.at(j+2+2*k+1));
	    }	    
	  }

	  m_outbinary << "\n";
	}
      }
    
      m_outbinary << "\n";
    }

    m_outbinary  << "#########################################" << "\n";

  }


  m_outbinary.close();

  delete L1TT;
}


/////////////////////////////////////////////////////////////
//
// Basic methods, initializations,...
//
/////////////////////////////////////////////////////////////

void patterngen::initTuple(std::string in,std::string out)
{
  L1TT   = new TChain("L1TrackTrigger"); 
  
  // Input data file

  std::size_t found = in.find(".root");

  // Case 1, it's a root file
  if (found!=std::string::npos)
  {
    L1TT->Add(in.c_str());
  }
  else // This is a list provided into a text file
  {
    std::string STRING;
    std::ifstream in2(in.c_str());
    if (!in2)
    {
      std::cout << "Please provide a valid data filename list" << std::endl;
      return;
    }
  
    while (!in2.eof())
    {
      getline(in2,STRING);

      found = STRING.find(".root");
      if (found!=std::string::npos) L1TT->Add(STRING.c_str());
    }

    in2.close();
  }



  pm_stub_layer=&m_stub_layer;
  pm_stub_ladder=&m_stub_ladder;
  pm_stub_module=&m_stub_module;
  pm_stub_pt=&m_stub_pt;
  pm_stub_deltas=&m_stub_deltas;
  pm_stub_strip=&m_stub_strip;
  pm_stub_seg=&m_stub_seg;
  pm_clus_nseg=&m_clus_nseg;
  pm_stub_chip=&m_stub_chip;
  pm_stub_clust1=&m_stub_clust1;


  L1TT->SetBranchAddress("STUB_n",         &m_stub);
  L1TT->SetBranchAddress("STUB_layer",     &pm_stub_layer);
  L1TT->SetBranchAddress("STUB_ladder",    &pm_stub_ladder);
  L1TT->SetBranchAddress("STUB_module",    &pm_stub_module);
  L1TT->SetBranchAddress("STUB_pt",        &pm_stub_pt);
  L1TT->SetBranchAddress("STUB_deltas",    &pm_stub_deltas);
  L1TT->SetBranchAddress("STUB_strip",     &pm_stub_strip);
  L1TT->SetBranchAddress("STUB_seg",       &pm_stub_seg);
  L1TT->SetBranchAddress("STUB_chip",      &pm_stub_chip);
  L1TT->SetBranchAddress("STUB_clust1",    &pm_stub_clust1);
  L1TT->SetBranchAddress("CLUS_PS",        &pm_clus_nseg);

  m_outbinary.open("concentrator_input.txt");
}
