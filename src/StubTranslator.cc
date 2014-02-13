#include "../interface/StubTranslator.h"

StubTranslator::StubTranslator()
{
  std::cout << "Entering OffStub Translator" << std::endl;

  m_stub_etaGEN  = new  std::vector<float>; 
  m_stub_strip   = new  std::vector<float>; 
  m_stub_ptGEN   = new  std::vector<float>;  
  m_stub_modid   = new  std::vector<int>;  

  StubTranslator::reset();

  // Then the rootuple

  m_tree_L1TrackTrigger = new TTree("BankStubs","Stubs for bank");  

  m_tree_L1TrackTrigger->Branch("STUB_n",           &m_stub);
  m_tree_L1TrackTrigger->Branch("STUB_ptGEN",       &m_stub_ptGEN);
  m_tree_L1TrackTrigger->Branch("STUB_etaGEN",      &m_stub_etaGEN);
  m_tree_L1TrackTrigger->Branch("STUB_modid",       &m_stub_modid);
  m_tree_L1TrackTrigger->Branch("STUB_strip",       &m_stub_strip);


}

StubTranslator::~StubTranslator()
{}

void StubTranslator::reset()
{
  m_stub = 0;

  m_stub_etaGEN->clear();  
  m_stub_strip->clear(); 
  m_stub_ptGEN->clear();
  m_stub_modid->clear();  
}

void StubTranslator::do_translation(StubExtractor *st)
{

  int n_stubs = st->getNStub();

  StubTranslator::reset();

  for (int i=0;i<n_stubs;++i)
  {
    if (st->getStub_tp(i)!=0) continue;

    ++m_stub;
    
    m_stub_etaGEN->push_back(st->getStub_etaGen(i));  
    m_stub_strip->push_back(st->getStub_strip(i)); 
    m_stub_ptGEN->push_back(st->getStub_ptGen(i));
    m_stub_modid->push_back(st->getStub_ID(i)); 
  }
}

// Fill the root tree containing analysis results

void StubTranslator::fillTree()
{
  m_tree_L1TrackTrigger->Fill(); 
}
