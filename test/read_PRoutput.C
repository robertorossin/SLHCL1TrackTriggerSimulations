#include <vector>
#ifdef __MAKECINT__
#pragma link C++ class vector<vector<int> >+;
#pragma link C++ class vector<vector<float> >+;
#endif

//////////////////////
//
// Small macro for looking at the CMSSW PR output
// 
// Use:
// 
// root [0] .L read_PRoutput.C
// root [1] read_evt(A,B)
//
// A : the root file containing the info
// B : the number of the event you want to output
//
// Information about this macro may be found in the following page:
//
// http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.HLLHCTuto (STEP V)
//
// Author: viret@in2p3_dot_fr
// Date: 26/09/2013
//////////////////////


void read_evt(const std::string PR_output, const int evt) 
{
  std::vector< std::vector<int> > *m_links = new std::vector< std::vector<int> >;
  std::vector<int> *m_secid = new std::vector<int> ;

  int evtID;
  int m_patt;

  TChain *data = new TChain("L1PatternReco");

  data->SetBranchAddress("evt",            &evtID); // Simple evt number or event ID
  data->SetBranchAddress("PATT_n",         &m_patt);
  data->SetBranchAddress("PATT_links",     &m_links);
  data->SetBranchAddress("PATT_secID",     &m_secid);
  data->Add(PR_output.c_str());

  int ndat = data->GetEntries();

  if (evt>=ndat) return;

  data->GetEntry(evt);

  cout << "Analyzing event " << evt << " from the PR output file..." << endl;
  cout << "...corresponding to event " << evtID << " from the initial data file" << endl;
  cout << endl;
  cout << "This event contains " << m_patt << " matched patterns in total..." << endl;
  cout << endl;

  if (m_patt==0) return;

  for(int k=0;k<m_patt;k++)
  {
    cout << "Pattern " << k+1 << " was found in sector " << m_secid->at(k) << " and contains the following stub indexes :" << endl;

    for(int kk=0;kk<m_links->at(k).size();kk++)
    {
      cout << m_links->at(k).at(kk) << " / ";
    }

    cout << endl;
    cout << endl;
  } 
}
