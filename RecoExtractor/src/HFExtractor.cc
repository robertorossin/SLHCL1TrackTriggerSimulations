#include "../interface/HFExtractor.h"


HFExtractor::HFExtractor(edm::InputTag tag, bool skim)
{
  //std::cout << "HFExtractor objet is created" << std::endl;


  m_tag = tag;
  m_skim= skim;

  // Tree definition

  m_tree      = new TTree("HF","RECO HF info") ;

  // Branches definition

  m_tree->Branch("HF_mcharge_M", &m_mE_hfm,"HF_mcharge_M/F");
  m_tree->Branch("HF_mcharge_P", &m_mE_hfp,"HF_mcharge_P/F");
  m_tree->Branch("HF_charge_M",  &m_E_hfm,"HF_charge_M/F");
  m_tree->Branch("HF_charge_P",  &m_E_hfp,"HF_charge_P/F");
  m_tree->Branch("HF_n",         &m_nHF,"HF_n/I");
  m_tree->Branch("HF_ETrings",   &m_Etring,"HF_ETrings[4]/I");
  m_tree->Branch("HF_ETrings_R", &m_Etring_real,"HF_ETrings_R[4]/F");
  m_tree->Branch("HF_ETtower_R", &m_Ettower_real,"HF_ETower_R[4][18]/F");
  m_tree->Branch("HF_asym",      &m_HF_asym,"HF_asym[100]/F");

  if (!m_skim)
  {
    m_tree->Branch("HF_eta",       &m_HF_eta,"HF_eta[HF_n]/F");
    m_tree->Branch("HF_phi",       &m_HF_phi,"HF_phi[HF_n]/F");
    m_tree->Branch("HF_z",         &m_HF_z,"HF_z[HF_n]/F");
    m_tree->Branch("HF_e",         &m_HF_e,"HF_e[HF_n]/F"); 
    m_tree->Branch("HF_time",      &m_HF_time,"HF_time[HF_n]/F"); 
  }

  // Set everything to 0

  HFExtractor::reset();
}

HFExtractor::~HFExtractor()
{}


void HFExtractor::init(const edm::EventSetup *setup)
{
  setup->get<CaloGeometryRecord>().get(caloGeometry);
  HFgeom = caloGeometry->getSubdetectorGeometry(DetId::Hcal, HcalForward);
}

//
// Method filling the main particle tree
//

void HFExtractor::writeInfo(const edm::Event *event) 
{
  HFExtractor::reset();

  edm::Handle<HFRecHitCollection> HF_rechit;
  event->getByLabel(m_tag,HF_rechit); 

  edm::Handle< L1GctHFRingEtSumsCollection > hwHFEtSumsColl ;
  event->getByLabel( "gctDigis", hwHFEtSumsColl ) ;

  L1GctHFRingEtSumsCollection::const_iterator hwHFEtSumsItr =
    hwHFEtSumsColl->begin() ;
  L1GctHFRingEtSumsCollection::const_iterator hwHFEtSumsEnd =
    hwHFEtSumsColl->end() ;
  
  int iEtSums = 0 ;
  for( ; hwHFEtSumsItr != hwHFEtSumsEnd ; ++hwHFEtSumsItr, ++iEtSums )
  {
    if (hwHFEtSumsItr->bx()==0) 
    {      
      m_Etring[0] = hwHFEtSumsItr->etSum(0); // High eta +
      m_Etring[1] = hwHFEtSumsItr->etSum(1); // High eta -
      m_Etring[2] = hwHFEtSumsItr->etSum(2); // Low eta +
      m_Etring[3] = hwHFEtSumsItr->etSum(3); // Low eta -
    }
  }

  int n_hf[2]     = {0,0};
  double e_hf[2]  = {0.,0.};

  double nrj = 0.;
  double lsb = 0.5; // Et tower threshold, in GeV

  double PI=4.*atan(1.);

  double ratio=0.;
  int index=0;
  double phi=0.;

  for (HFRecHitCollection::const_iterator HF=HF_rechit->begin();HF!=HF_rechit->end();++HF)
  {
    if (m_nHF < m_HFclus_MAX) 
    {
      HcalDetId cell(HF->id());
      const CaloCellGeometry* cellGeometry = HFgeom->getGeometry(cell);

      m_HF_eta[m_nHF] = cellGeometry->getPosition().eta () ;
      m_HF_e[m_nHF]   = HF->energy();
      
      //      nrj = m_HF_e[m_nHF]/cosh(m_HF_eta[m_nHF]) - fmod(m_HF_e[m_nHF]/cosh(m_HF_eta[m_nHF]),lsb);

      nrj   = m_HF_e[m_nHF]/cosh(m_HF_eta[m_nHF]);
      phi   = cellGeometry->getPosition().phi(); // Need to do this cast otherwise phi is TLorentz type

      ratio = 18.*(phi+PI)/(2.*PI);
      index = static_cast<int>(ratio); 

      if (m_HF_eta[m_nHF]>4.5)                         m_Ettower_real[0][index] += nrj; 
      if (m_HF_eta[m_nHF]<-4.5)                        m_Ettower_real[1][index] += nrj;  
      if (m_HF_eta[m_nHF]>4. && m_HF_eta[m_nHF]<4.5)   m_Ettower_real[2][index] += nrj;  
      if (m_HF_eta[m_nHF]>-4.5 && m_HF_eta[m_nHF]<-4.) m_Ettower_real[3][index] += nrj;  


      if (!m_skim)
      {
	m_HF_phi[m_nHF] = cellGeometry->getPosition().phi () ;
	m_HF_z[m_nHF]   = cellGeometry->getPosition().z ();
	m_HF_time[m_nHF]= HF->time();
      }



      if (m_HF_eta[m_nHF]<-4)
      {
	for (int i=0;i<100;++i) 
	{
	  if (m_HF_e[m_nHF]>0.2*i) m_HF_asym[i] -= m_HF_e[m_nHF];
	}
      }

      if (m_HF_eta[m_nHF]>4)
      {
	for (int i=0;i<100;++i) 
	{
	  if (m_HF_e[m_nHF]>0.2*i) m_HF_asym[i] += m_HF_e[m_nHF];
	}
      }

      m_nHF++;

      if (HF->energy()>5.) 
      {
	if (HF->id().zside() == -1)
	{
	  ++n_hf[0];
	  e_hf[0] += HF->energy();
	}
	else
	{
	  ++n_hf[1];
	  e_hf[1] += HF->energy();
	}
      }
    }
  }

  for (int i=0;i<4;++i)
  {  
    for (int j=0;j<18;++j)
      m_Etring_real[i] += m_Ettower_real[i][j]-fmod(m_Ettower_real[i][j],lsb); 
  }


  m_E_hfm = e_hf[0];
  m_E_hfp = e_hf[1];

  // Get HF asymmetries

  for (int i=0;i<2;++i) 
  {
    if (n_hf[i]) e_hf[i] /= n_hf[i];
  }

  m_mE_hfm = e_hf[0];
  m_mE_hfp = e_hf[1];

  HFExtractor::fillTree();
}


// Method initializing everything (to do for each event)

void HFExtractor::reset()
{

  m_nHF     = 0;
  m_E_hfm   = 0.;
  m_E_hfp   = 0.;
  m_mE_hfm  = 0.;
  m_mE_hfp  = 0.;

  for (int i=0;i<4;++i) 
  {
    m_Etring[i]      = 0;
    m_Etring_real[i] = 0.;

    for (int j=0;j<18;++j) m_Ettower_real[i][j] = 0.;
  }

  for (int i=0;i<100;++i) 
  {
    m_HF_asym[i] = 0.;
  }

  for (int i=0;i<m_HFclus_MAX;++i) 
  {
    m_HF_eta[i] = 0.;
    m_HF_phi[i] = 0.;
    m_HF_z[i]   = 0.;
    m_HF_e[i]   = 0.;
    m_HF_time[i]= 0.;
  }

}


void HFExtractor::fillTree()
{
  m_tree->Fill(); 
}
 
void HFExtractor::fillSize(int size)
{
  m_nHF=size;
}

int  HFExtractor::getSize()
{
  return m_nHF;
}

