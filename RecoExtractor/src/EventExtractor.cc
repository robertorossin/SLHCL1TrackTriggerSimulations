#include "../interface/EventExtractor.h"


EventExtractor::EventExtractor(edm::InputTag tag)
{
  m_tag = tag;

  // Tree definition

  m_tree    = new TTree("event","Event info");  

  // Branches definition

  m_tree->Branch("evtID",  &m_evtID,"evtID/I");
  m_tree->Branch("lumi",   &m_lumi,"lumi/I");
  m_tree->Branch("run",    &m_run,"run/I");
  m_tree->Branch("BCID",   &m_BCID,"BCID/I");
  m_tree->Branch("PHYS",   &m_phy_decl,"PHYS/I");
  m_tree->Branch("time",   &m_time,"time/I");
  m_tree->Branch("offset", &m_offset,"offset/I");
  m_tree->Branch("I_B1",   &m_lumi_1,"IB1/F");
  m_tree->Branch("I_B2",   &m_lumi_2,"IB2/F");
  m_tree->Branch("n_paths",  &m_n_HLTs,"n_paths/I");       
  m_tree->Branch("HLT_vector","vector<string>",&m_HLT_vector);

  m_tree->Branch("L1_tech_bits",    &m_tech_trig_ind,"L1_tech_bits[64]/I");
  m_tree->Branch("L1_algo_bits",    &m_alg_trig_ind,"L1_algo_bits[128]/I");

  // Set everything to 0
  
  for (int i=0;i<3564;i++)
  {    
    i_bx_B1_now[i] = 0.;
    i_bx_B2_now[i] = 0.;
  }

  EventExtractor::reset();
}

EventExtractor::~EventExtractor()
{}

void EventExtractor::writeInfo(const edm::LuminosityBlock *lumi) 
{

  edm::Handle<LumiDetails> lumiDetails;
  lumi->getByLabel("lumiProducer", lumiDetails); 

  if (!lumiDetails.isValid())  return;
  if (!lumiDetails->lumiBeam1Intensities().size()) return;
  if (!lumiDetails->lumiBeam2Intensities().size()) return;

  for (int i=0;i<3564;++i) 
  {
    i_bx_B1_now[i]=lumiDetails->lumiBeam1Intensity(i)/10000000000.;
    i_bx_B2_now[i]=lumiDetails->lumiBeam2Intensity(i)/10000000000.;
  }
}

//
// Method filling the main particle tree
//

void EventExtractor::writeInfo(const edm::Event *event, bool MC) 
{
  EventExtractor::reset();

  m_evtID   = static_cast<int>((event->eventAuxiliary()).id().event());
  m_BCID    = (event->eventAuxiliary()).bunchCrossing();
  m_time    = static_cast<int>((event->eventAuxiliary()).time().unixTime());
  m_offset  = static_cast<int>((event->eventAuxiliary()).time().microsecondOffset());
  m_lumi    = (event->eventAuxiliary()).luminosityBlock();
  m_run     = static_cast<int>((event->eventAuxiliary()).run());

  // Now we got the real intensity per BCID

  m_lumi_1 = i_bx_B1_now[m_BCID];
  m_lumi_2 = i_bx_B2_now[m_BCID];

 // 
  // Then get trigger information
  //  


  edm::Handle<L1GlobalTriggerReadoutRecord> l1GtRR;
  //event->getByLabel("gtDigis",l1GtRR);
  event->getByLabel(m_tag,l1GtRR);

  L1GlobalTriggerReadoutRecord L1GTRR   = *l1GtRR.product();              
  DecisionWord gtDecisionWord           = L1GTRR.decisionWord();

  for (int iBit=0;iBit<128;++iBit) 
  {
    if (gtDecisionWord[iBit]) // Is the path accepted??
      m_alg_trig_ind[iBit] = 1;
  }

  if (!MC) m_selected=false;

  // Look at the technical bits

  m_phy_decl = L1GTRR.gtFdlWord().physicsDeclared();

  const L1GtPsbWord psb = L1GTRR.gtPsbWord(0xbb09, 0); 
  
  for (int i=0;i<16;++i) 
  {
    m_tech_trig_ind[i]    = (psb.aData(0)>>i)&0x1;
    m_tech_trig_ind[i+16] = (psb.bData(0)>>i)&0x1;
    m_tech_trig_ind[i+32] = (psb.aData(1)>>i)&0x1;
    m_tech_trig_ind[i+48] = (psb.bData(1)>>i)&0x1;
  }

  edm::Handle<edm::TriggerResults> triggerResults ;
  edm::InputTag tag("TriggerResults", "", "HLT");
  event->getByLabel(tag,triggerResults);

  if (triggerResults.isValid())
  {
    const edm::TriggerNames & triggerNames = event->triggerNames(*triggerResults);
    
    for(int i = 0 ; i < static_cast<int>(triggerResults->size()); i++) 
    {
      if (triggerResults->accept(i)!=0)
      {
	m_selected=true;
	m_HLT_vector.push_back(triggerNames.triggerName(i));
	m_n_HLTs++;
      }
    }
  }

  if (m_selected) m_tree->Fill(); 
}


// Method initializing everything (to do for each event)

void EventExtractor::reset()
{
  m_selected = true;
  m_evtID    =  0;
  m_BCID     =  0;
  m_time     =  0;
  m_offset   =  0;
  m_lumi     =  0;
  m_run      =  0;
  m_phy_decl = 0; 

  m_n_HLTs = 0;
  m_HLT_vector.clear();

  for (int i=0;i<64;i++) m_tech_trig_ind[i] = 0; 
  for (int i=0;i<128;i++) m_alg_trig_ind[i] = 0;
}


 
bool EventExtractor::isSelected()
{
  return m_selected;
}
