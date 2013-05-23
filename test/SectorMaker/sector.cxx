// Class for the sector determination
// For more info, look at the header file

#include "sector.h"

sector::sector(std::string filename,std::string outfile, int neta,int nphi,int oeta,int ophi)
{
  m_nphi = nphi;
  m_neta = neta;
  m_ophi = ophi;
  m_oeta = oeta;
  m_cov  = 0.01; // If a module shares less than m_cov of 
                 // its active area with the sector
                 // It is not taken into account

  sector::initTuple(filename,outfile);
  sector::initVars();
  sector::do_sector();

}

void sector::do_sector()
{
  float phi_min;
  float phi_max;
  float eta_min;
  float eta_max;

  // Get the rate information
  m_ratetree->GetEntry(0);


  // Loop over the sectors
  for (int k=0;k<m_neta;++k)
  { 
    for (int j=0;j<m_nphi;++j)
    { 
      cout << k << " / " << j << endl;

      // Define The sector coordinates

      phi_min = j*m_phi_step-m_phi_size/2.;;
      phi_max = j*m_phi_step+m_phi_size/2.;
      eta_min = -2.5+(k+0.5)*m_eta_step-m_eta_size/2.;
      eta_max = -2.5+(k+0.5)*m_eta_step+m_eta_size/2.;

      cout << phi_min << " / " << phi_max << " / " << eta_min << " / " << eta_max << endl;


      std::vector<int> barrel_mod;
      std::vector<int> endcap_mod;
      std::vector<int> sector;
      std::vector<float> coord;

      barrel_mod.clear();
      endcap_mod.clear();
      sector.clear();
      coord.clear();
      
      // Adapt to CMS coordinate system
      if (phi_min>m_PI) phi_min-=2*m_PI;
      if (phi_max>m_PI) phi_max-=2*m_PI;
      
      coord.push_back(phi_min);
      coord.push_back(phi_max);
      coord.push_back(eta_min);
      coord.push_back(eta_max);
      
      // Loop over barrel modules to check if they are in the sector
      for (int i=0;i<58000;++i)
      { 
	if (!sector::is_in_eta(m_b_etamax[i],m_b_etamin[i],eta_min,eta_max,m_cov)) continue;
	if (!sector::is_in_phi(m_b_phimax[i],m_b_phimin[i],phi_min,phi_max,m_cov)) continue;
	
	///	cout << k << " / " << j << " / " << i << endl; 

	// Module is in, add it...
	barrel_mod.push_back(i);
	sector.push_back(i+50000);
      }
	
      // Loop over endcap modules to check if they are in the sector
      for (int i=0;i<142000;++i)
      { 
	if (!sector::is_in_eta(m_e_etamax[i],m_e_etamin[i],eta_min,eta_max,m_cov)) continue;
	if (!sector::is_in_phi(m_e_phimax[i],m_e_phimin[i],phi_min,phi_max,m_cov)) continue;
	
	// Module is in, add it...
	endcap_mod.push_back(i);
	sector.push_back(i+110000);
      }

      // End of modules loop 
      // Push the sectors in the sector trees

      m_endcap_mod_tot->push_back(endcap_mod);
      m_barrel_mod_tot->push_back(barrel_mod);
      m_sectors->push_back(sector);
      m_coords->push_back(coord);
    }
  }

  cout << "We have made " << m_barrel_mod_tot->size() << " sectors " << endl; 

  // Increment the counter tables

  for (int i=0;i<static_cast<int>(m_barrel_mod_tot->size());++i)
  {
    for (int j=0;j<static_cast<int>(m_barrel_mod_tot->at(i).size());++j) 
      ++m_counter_barrel[m_barrel_mod_tot->at(i).at(j)];
  }

  for (int i=0;i<static_cast<int>(m_endcap_mod_tot->size());++i)
  { 
    for (int j=0;j<static_cast<int>(m_endcap_mod_tot->at(i).size());++j) 
      ++m_counter_endcap[m_endcap_mod_tot->at(i).at(j)];
  }

  // Fill the data

  m_sectree->Fill();
  m_outfile->Write();
  m_outfile->Close();

}

/////////////////////////////////////////////////////////////
//
// Basic methods, initializations,...
//
/////////////////////////////////////////////////////////////


// Method checking that a module is within 
// the eta range of the sector


bool sector::is_in_eta(float mod_max,float mod_min,float sec_min,float sec_max,float cov)
{  

  //  cout << mod_min << " / " << mod_max << " / " << sec_min << " / " << sec_max << endl;


  if (mod_max<sec_min) return false; 
  if (mod_min>sec_max) return false;

  // Is all the module in the sector?
  if (mod_max<sec_max && mod_min>sec_min) return true;

  // Is a part of the module in the upper etas of the sector?
  if (mod_max<sec_max && (mod_max-sec_min)/(mod_max-mod_min)>cov) return true; 

  // Is a part of the module in the lower etas of the sector?
  if (mod_min>sec_min && (sec_max-mod_min)/(mod_max-mod_min)>cov) return true; 

  return false;
}


// Method checking that a module is within 
// the phi range of the sector
//
// !!! The PI/-PI transition requires a special attention


bool sector::is_in_phi(float mod_max,float mod_min,float sec_min,float sec_max,float cov)
{ 
  if (sec_min>sec_max) // The sector is at the PI/-PI transition
  {
    if (mod_max<mod_min) return true; // In this case all the module is in the sector

    if (mod_max>0 && mod_max<sec_min) return false;
    if (mod_min<0 && mod_min>sec_max) return false;

    if (mod_max>0 && (mod_max-sec_min)/(mod_max-mod_min)>cov) return true;
    if (mod_min<0 && (sec_max-mod_min)/(mod_max-mod_min)>cov) return true;

    return false;
  }
  else // Normal case
  {
    if (mod_max<mod_min) // The module is at the PI/-PI transition 
    {
      if (sec_max>0 && (sec_max-mod_min)/(mod_max+8*atan(1.)-mod_min)>cov) return true;
      if (sec_min<0 && (mod_max-sec_min)/(mod_max+8*atan(1.)-mod_min)>cov) return true;
      
      return false;
    }
    else // The basic case, looks like the eta situation
    {
      if (mod_max<sec_max && mod_min>sec_min) return true; // All in the sector
      if (mod_max<sec_max && (mod_max-sec_min)/(mod_max-mod_min)>cov) return true; 
      if (mod_min>sec_min && (sec_max-mod_min)/(mod_max-mod_min)>cov) return true; 

      return false;
    }
  }
}

void sector::initVars()
{

  m_barrel_mod_tot = new std::vector< std::vector<int> >;
  m_endcap_mod_tot = new std::vector< std::vector<int> >;
  m_sectors        = new std::vector< std::vector<int> >;
  m_coords         = new std::vector< std::vector<float> >;

  m_barrel_mod_tot->clear();
  m_endcap_mod_tot->clear();
  m_sectors->clear();
  m_coords->clear();


  for (int i=0;i<58000;++i) m_counter_barrel[i]=0;
  for (int i=0;i<142000;++i) m_counter_endcap[i]=0;

  m_eta_size = 5.*(100.+static_cast<float>(m_oeta))/(100.*static_cast<float>(m_neta));
  m_phi_size = 8.*atan(1.)*(100.+static_cast<float>(m_ophi))/(100.*static_cast<float>(m_nphi));

  m_eta_step = 5./static_cast<float>(m_neta);
  m_phi_step = 8.*atan(1.)/static_cast<float>(m_nphi);

  cout << m_neta << "/" << m_eta_size << "/" << m_eta_step << endl;
  cout << m_nphi << "/" << m_phi_size << "/" << m_phi_step << endl;

  m_PI = 4.*atan(1.);
}




void sector::initTuple(std::string in,std::string out)
{
  m_infile   = TFile::Open(in.c_str());
  m_ratetree = (TTree*)m_infile->Get("L1Rates");

  m_outfile = new TFile(out.c_str(),"recreate");
  m_sectree = new TTree("Sectors","Tree containing tracker sec info");


  m_ratetree->SetBranchAddress("STUB_b_rates",         &m_b_rate);
  m_ratetree->SetBranchAddress("STUB_b_phi_b",         &m_b_phimin);
  m_ratetree->SetBranchAddress("STUB_b_phi_t",         &m_b_phimax);
  m_ratetree->SetBranchAddress("STUB_b_eta_b",         &m_b_etamin);
  m_ratetree->SetBranchAddress("STUB_b_eta_t",         &m_b_etamax);

  m_ratetree->SetBranchAddress("STUB_e_rates",         &m_e_rate);
  m_ratetree->SetBranchAddress("STUB_e_phi_b",         &m_e_phimin);
  m_ratetree->SetBranchAddress("STUB_e_phi_t",         &m_e_phimax);
  m_ratetree->SetBranchAddress("STUB_e_eta_b",         &m_e_etamin);
  m_ratetree->SetBranchAddress("STUB_e_eta_t",         &m_e_etamax);

  m_sectree->Branch("sectors_barrel",   &m_barrel_mod_tot);
  m_sectree->Branch("sectors_endcap",   &m_endcap_mod_tot);
  m_sectree->Branch("sectors",          &m_sectors);
  m_sectree->Branch("sectors_coord",    &m_coords);
  m_sectree->Branch("counter_barrel",   &m_counter_barrel , "counter_barrel[58000]/I");
  m_sectree->Branch("counter_endcap",   &m_counter_endcap , "counter_endcap[142000]/I");

}
