#include "../interface/InfoExtractor.h"


InfoExtractor::InfoExtractor()
{
  InfoExtractor::reset();

  // Tree definition

  m_tree    = new TTree("MIB_info","Run properties");
  m_tree_ps = new TTree("PS_info","Prescale factors");  

  // Branches definition

  m_tree->Branch("run_number",      &m_run,"run_number/I");
  m_tree->Branch("n_LB",            &m_nLB,"n_LB/I");
  m_tree->Branch("n_evt",           &nevent,"n_evt/I");
  m_tree->Branch("LBmin",           &m_LBmin,"LBmin/I");
  m_tree->Branch("LBmax",           &m_LBmax,"LBmax/I");
  m_tree->Branch("L1_tech_mask",    &m_tech_trig_act,"L1_tech_mask[64]/I");
  m_tree->Branch("L1_algo_mask",    &m_alg_trig_act,"L1_algo_mask[128]/I");
  m_tree->Branch("B1_bx_Imax",      &i_bx_B1,"B1_bx_Imax[3564]/F");
  m_tree->Branch("B2_bx_Imax",      &i_bx_B2,"B2_bx_Imax[3564]/F");
  m_tree->Branch("n_paths",  &m_n_HLTs,"n_paths/I");       
  m_tree->Branch("HLT_vector","vector<string>",&m_HLT_vector);  
  m_tree->Branch("L1_tech_pres",    &m_tech_trig_pre,"L1_tech_pres[64]/I");
  m_tree->Branch("L1_algo_pres",    &m_alg_trig_pre,"L1_algo_pres[128]/I");
  m_tree->Branch("HLT_pscale","vector<int>",&m_HLT_prescales);

  m_tree_ps->Branch("LB_number",       &m_LBnum,"LB_number/I");
  m_tree_ps->Branch("L1_tech_pres",    &m_tech_trig_pre,"L1_tech_pres[64]/I");
  m_tree_ps->Branch("L1_algo_pres",    &m_alg_trig_pre,"L1_algo_pres[128]/I");
  m_tree_ps->Branch("HLT_pscale","vector<int>",&m_HLT_prescales);
}



void InfoExtractor::init(const edm::Run *run, const edm::EventSetup *setup)
{
  m_l1GtUtils.retrieveL1EventSetup(*setup);

  const std::string processName = "HLT"; 
  bool changed(true);

  hltConfig.init( *run, *setup, processName, changed );

  InfoExtractor::reset();
}

InfoExtractor::~InfoExtractor()
{}

void InfoExtractor::writeInfo(const edm::LuminosityBlock *lumi) 
{

  m_nLB++;

  int m_LB      = lumi->luminosityBlock();

  if (m_LBmin==-1 && m_LBmax==-1)
  {
    m_LBmin=m_LB;
    m_LBmax=m_LB;
  }

  m_LBnum=m_LB;

  if (m_LBmin>m_LB)     m_LBmin=m_LB;
  if (m_LBmax<m_LB)     m_LBmax=m_LB;

  
  edm::Handle<LumiDetails> lumiDetails;
  lumi->getByLabel("lumiProducer", lumiDetails); 
  
  if (!lumiDetails.isValid()) return;

  //  std::cout << lumiDetails->lumiBeam1Intensities().size() << std::endl;
  //  std::cout << lumiDetails->lumiBeam2Intensities().size() << std::endl;

  if (!lumiDetails->lumiBeam1Intensities().size()) return;
  if (!lumiDetails->lumiBeam2Intensities().size()) return;
  
  for (int i=0;i<3564;++i) 
  {
    //    std::cout << lumiDetails->lumiBeam1Intensity(i) << std::endl;
    //    std::cout << lumiDetails->lumiBeam2Intensity(i) << std::endl;

    if (lumiDetails->lumiBeam1Intensity(i)/10000000000. > i_bx_B1[i])
      i_bx_B1[i]=lumiDetails->lumiBeam1Intensity(i)/10000000000.;

    if (lumiDetails->lumiBeam2Intensity(i)/10000000000. > i_bx_B2[i])
      i_bx_B2[i]=lumiDetails->lumiBeam2Intensity(i)/10000000000.;
  }
}

//
// Method filling the main particle tree
//

void InfoExtractor::writeInfo(const edm::Event *event, const edm::EventSetup *setup) 
{

  m_run = (event->id()).run();
  m_n_HLTs = 0;
  m_HLT_vector.clear();
  m_HLT_prescales.clear();

  int err = -1;

  L1GtUtils::TriggerCategory trigCategory = L1GtUtils::AlgorithmTrigger;
  const std::vector<int>& pfSetAlgorithmTrigger =
    m_l1GtUtils.prescaleFactorSet(*event, trigCategory, err);
  const std::vector<unsigned int>& tmSetAlgorithmTrigger =
    m_l1GtUtils.triggerMaskSet(trigCategory, err);

  int iBit = -1;
  for (std::vector<int>::const_iterator cItBit =
	 pfSetAlgorithmTrigger.begin(); cItBit
	 != pfSetAlgorithmTrigger.end(); ++cItBit) 
    {    
      ++iBit;  
      m_alg_trig_pre[iBit] = (*cItBit); 
      //if (m_alg_trig_pre[iBit])
	//std::cout << iBit << " / " << m_alg_trig_pre[iBit] << std::endl;
    }

  iBit=-1;

  for (std::vector<unsigned int>::const_iterator cItBit =
	 tmSetAlgorithmTrigger.begin(); cItBit
	 != tmSetAlgorithmTrigger.end(); ++cItBit) 
    {    
      iBit++;
      m_alg_trig_act[iBit] = static_cast<int>(*cItBit); 
      //std::cout << iBit << " / " << m_alg_trig_act[iBit] << std::endl;
    }

  trigCategory = L1GtUtils::TechnicalTrigger;
  
  const std::vector<int>& pfSetTechnicalTrigger =
    m_l1GtUtils.prescaleFactorSet(*event, trigCategory, err);
  const std::vector<unsigned int>& tmSetTechnicalTrigger =
    m_l1GtUtils.triggerMaskSet(trigCategory, err);

  iBit = -1;
  for (std::vector<int>::const_iterator cItBit =
	 pfSetTechnicalTrigger.begin(); cItBit
	 != pfSetTechnicalTrigger.end(); ++cItBit) 
    {    
      iBit++;
      m_tech_trig_pre[iBit] = (*cItBit); 
    }
  
  iBit=-1;

  for (std::vector<unsigned int>::const_iterator cItBit =
	 tmSetTechnicalTrigger.begin(); cItBit
	 != tmSetTechnicalTrigger.end(); ++cItBit) 
    {    
      iBit++;
      m_tech_trig_act[iBit] = static_cast<int>(*cItBit); 
    }
  
  edm::Handle<edm::TriggerResults> triggerResults ;
  edm::InputTag tag("TriggerResults", "", "HLT");
  event->getByLabel(tag,triggerResults);

  if (triggerResults.isValid())
  {
    const edm::TriggerNames & triggerNames = event->triggerNames(*triggerResults);
    
    for(int i = 0 ; i < static_cast<int>(triggerResults->size()); i++) 
    {
      size_t found;
      
      found=(triggerNames.triggerName(i)).find("BeamGas");
      
      if (int(found)==-1)
	found=(triggerNames.triggerName(i)).find("Interbunch");

      if (int(found)==-1)
	found=(triggerNames.triggerName(i)).find("PreCollisions");

      if (int(found)==-1)
	found=(triggerNames.triggerName(i)).find("BeamHalo");
      
      if (int(found)==-1) continue;
      //std::cout << triggerNames.triggerName(i) << " / " 
      //		<< hltConfig.prescaleValue( *event, *setup,triggerNames.triggerName(i)) << std::endl;

      m_HLT_vector.push_back(triggerNames.triggerName(i));
      m_HLT_prescales.push_back(hltConfig.prescaleValue( *event, *setup,triggerNames.triggerName(i)));
      m_n_HLTs++;
    }
  }
}


// Method initializing everything (to do for each event)

void InfoExtractor::reset()
{
  m_run        = 0;
  nevent       = 0;
  m_nLB        = 0;
  m_LBnum      = 0;
  m_LBmin      = -1;
  m_LBmax      = -1;

  for (int i=0;i<64;i++)
  {
    m_tech_trig_pre[i] = 0; 
    m_tech_trig_act[i] = 0; 
  }

  for (int i=0;i<128;i++)
  {
    m_alg_trig_pre[i] = 0;   
    m_alg_trig_act[i] = 0; 
  }

  for (int i=0;i<3564;i++)
  {    
    i_bx_B1[i] = 0.;
    i_bx_B2[i] = 0.;
  }

  m_n_HLTs = 0;
  m_HLT_vector.clear();
  m_HLT_prescales.clear();

}


void InfoExtractor::fillTree(int nevt)
{
  nevent=nevt;

  m_tree->Fill(); 
}

void InfoExtractor::fillPSTree()
{
  m_tree_ps->Fill(); 
}
