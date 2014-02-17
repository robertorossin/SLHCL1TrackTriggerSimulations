#include "../interface/RecoExtractor.h"

using namespace std;
using namespace edm;

RecoExtractor::RecoExtractor(const edm::ParameterSet& config) :
  do_fill_       (config.getUntrackedParameter<bool>("fillTree", true)),
  do_PIX_        (config.getUntrackedParameter<bool>("doPixel",    false)),
  do_MC_         (config.getUntrackedParameter<bool>("doMC",       false)),
  do_STUB_       (config.getUntrackedParameter<bool>("doSTUB",     false)),
  do_L1TRK_      (config.getUntrackedParameter<bool>("doL1TRK",    false)),
  do_BANK_       (config.getUntrackedParameter<bool>("doBANK",     false)),
  do_MATCH_      (config.getUntrackedParameter<bool>("doMatch",    false)),
  do_L1tt_       (config.getUntrackedParameter<bool>("doL1TT", false)),
  nevts_         (config.getUntrackedParameter<int>("n_events", 10000)),
  skip_          (config.getUntrackedParameter<int>("skip_events", 0)),
  CLUS_tag       (config.getParameter<std::string>("CLUS_container")),
  STUB_tag       (config.getParameter<std::string>("STUB_container")),
  CLUS_name      (config.getParameter<std::string>("CLUS_name")),
  STUB_name      (config.getParameter<std::string>("STUB_name")),
  PIX_tag_       (config.getParameter<edm::InputTag>("pixel_tag")),
  outFilename_   (config.getParameter<std::string>("extractedRootFile")),
  inFilename_    (config.getParameter<std::string>("inputRootFile")),
  m_settings_    (config.getUntrackedParameter<std::vector<std::string> >("analysisSettings"))
{
  // We parse the analysis settings
  m_ana_settings = new AnalysisSettings(&m_settings_);
  m_ana_settings->parseSettings();

 
}



void RecoExtractor::beginJob() 
{
  // Initializations

  // If do_fill is set to True, you extract the whole data, otherwise you start 
  // from a file already extracted (inFilename_)

  (do_fill_) 
    ? RecoExtractor::initialize()
    : RecoExtractor::retrieve();

  if (do_MC_ && do_PIX_ && do_L1tt_) 
    m_L1TT_analysis = new L1TrackTrigger_analysis(m_ana_settings,skip_);

  if (do_BANK_ && do_STUB_) 
    m_BK = new StubTranslator();

  skip_=0; // Temporary hack, process files separately..

  nevent_tot = skip_;

}


void RecoExtractor::beginRun(Run const& run, EventSetup const& setup) 
{
  nevent = 0;

  if (do_fill_) // We are filling the ntuple, first init the geom stuff
  {
    if (do_PIX_)      m_PIX->init(&setup);
    if (do_MC_)       m_MC->init(&setup);
    if (do_STUB_)     m_STUB->init(&setup);
    if (do_L1TRK_)    m_L1TRK->init(&setup);
  }

  // If we start from existing file we don't have to loop over events
  if (!do_fill_ && do_PIX_ && m_PIX->n_events()) 
  {    
    // If you start from an extracted file, the number of events you want to loop on
    // is defined as an option, not in CMSSW...
    
    nevent = min(skip_+nevts_,m_PIX->n_events()); 

    for (int i=skip_;i<nevent;++i) 
    {
      if (i%10000 == 0)
	std::cout << "Processing " << i << "th event" << std::endl;

      RecoExtractor::getInfo(i);// Retrieve the info from an existing ROOTuple      
      RecoExtractor::doAna();   // Then do the analysis on request  

      ++nevent_tot; 
    }
  }
}


void RecoExtractor::analyze(const edm::Event& event, const edm::EventSetup& setup)
{
  using namespace reco;
  
  if (do_fill_) 
  {
    RecoExtractor::fillInfo(&event); // Fill the ROOTuple
    RecoExtractor::doAna();          // Then do the analysis on request    
  }

  ++nevent;
  ++nevent_tot; 
}
 

void RecoExtractor::endRun(Run const&, EventSetup const&) {
  std::cout << "Total # of events for this run   = "<< nevent  << std::endl;
}

void RecoExtractor::endJob() {
  
  std::cout << "Total # of events for this job   = "<< nevent_tot     << std::endl;

  if (do_fill_) 
  {
    m_outfile->Write();
    m_outfile->Close();
  }
  else
  {
    m_infile->Close();
    m_outfile->Write();
    m_outfile->Close();
  }
}
    

// Here we fill the rootuple with info coming from the RECO file

void RecoExtractor::fillInfo(const edm::Event *event) 
{
  if (do_PIX_)               m_PIX->writeInfo(event);
  if (do_MC_)                m_MC->writeInfo(event);
  if (do_STUB_ && do_MC_)    m_STUB->writeInfo(event,m_MC);
  if (do_STUB_ && do_L1TRK_)  m_L1TRK->writeInfo(event,m_STUB);
}   


// Here we retrieve the info from an existing extracted ROOTuple 

void RecoExtractor::getInfo(int ievent) 
{
  if (do_MC_)       m_MC->getInfo(ievent);
  if (do_PIX_)      m_PIX->getInfo(ievent);
  if (do_STUB_)     m_STUB->getInfo(ievent);
  if (do_L1TRK_)    m_L1TRK->getInfo(ievent);
}


// Here are the initializations when starting from scratch (need to create the extracted stuff)

void RecoExtractor::initialize() 
{
  m_outfile  = new TFile(outFilename_.c_str(),"RECREATE");
  m_MC       = new MCExtractor(do_MC_);
  m_STUB     = new StubExtractor(CLUS_tag,CLUS_name,STUB_tag,STUB_name,do_STUB_);
  m_L1TRK    = new L1TrackExtractor(do_L1TRK_);
  m_PIX      = new PixelExtractor(PIX_tag_,do_PIX_,do_MATCH_);
}  

// Here are the initializations when starting from already extracted stuff

void RecoExtractor::retrieve() 
{
  m_infile     = m_infile->Open(inFilename_.c_str());
  m_outfile    = new TFile(outFilename_.c_str(),"RECREATE");

  // RECO content

  m_MC         = new MCExtractor(m_infile);
  m_PIX        = new PixelExtractor(m_infile);
  m_STUB       = new StubExtractor(m_infile);

  // We set some variables wrt the info retrieved 
  // (if the tree is not there, don't go further...)  

  do_PIX_      = m_PIX->isOK();
  do_MC_       = m_MC->isOK();
  do_STUB_     = m_STUB->isOK();

}


// Here we define all things which are post event extraction
//
// In other words this is where the event-by-event analysis is done
//

void RecoExtractor::doAna() 
{
  
  if (do_MC_ && do_PIX_ && do_L1tt_) 
  {  
    m_L1TT_analysis->do_stubs(m_PIX,m_MC);
    m_L1TT_analysis->fillTree();
  }

  if (do_STUB_ && do_BANK_) 
  {  
    m_BK->do_translation(m_STUB);
    m_BK->fillTree();
  }
}
