#include <iostream>
#include <fstream>
#include <iomanip>

// Internal includes

#include "rates.h"
#include "patterngen.h"
#include "sector_test.h"
#include "track_eff.h"
#include "efficiencies.h"
#include "jobparams.h"
#include "TROOT.h"

using namespace std;

///////////////////////////////////
//
//
// Base code for the AM analysis tool
//
// This code is extensively documented on the AM-tracking tutorial page:
//
// http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.HLLHCTuto620
//
//
//  Author: viret@in2p3_dot_fr
//  Date       : 23/05/2013
//  Maj. update: 13/11/2013
//
///////////////////////////////////

int main(int argc, char** argv) {

  // Necessary lines to make branches containing vectors
  gROOT->ProcessLine(".L Loader.C+");

  // read jobParams
  jobparams params(argc,argv);

  // Depending on the option chosen, process the information


  // Option 1: just do the rate calculation
  // Documented in part 3.2.2 of the mentioned tutorial page
  if (params.option()=="rates")
  {
    rates* my_rates = new rates(params.inputfile(),params.outfile());
    delete my_rates;    
  }
  

  // Option 2: test the sector coverage efficiency
  if (params.option()=="sector_eff")
  {
    sector_test* my_test = new sector_test(params.testfile(),params.inputfile(),
					   "",params.outfile(),
					   params.nevt(),params.dbg());
    delete my_test;
  }
  

  // Option 3: basic stub efficiencies calculation
  if (params.option()=="stub_eff")
  {
    efficiencies* my_effs = new efficiencies(params.inputfile(),params.outfile(),params.type());
    delete my_effs;
  }


  // Option 4: pattern reco efficiency from private stubs 
  if (params.option()=="PR_eff")
  {
    sector_test* my_test = new sector_test(params.testfile(),params.inputfile(),
					   params.pattfile(),params.outfile(),
					   params.nevt(),params.dbg());

    delete my_test;
  }


  // Option 5: pattern reco efficiency from official stubs 
  if (params.option()=="PR_off_eff")
  {
    track_eff* my_test = new track_eff(params.testfile(),params.inputfile(),
				       params.outfile(),params.nevt(),params.ptmin(),
				       params.qmax(),params.dbg());

    delete my_test;
  }


  // Option 6: generate a serie of patterns for the concentrator
  if (params.option()=="pattgen_conc")
  {
    patterngen* my_pgen = new patterngen(params.inputfile(),params.outfile(),params.nevt(),params.rate());
    delete my_pgen;
  }
  

  // Option 7: generate a serie of events for the MPA chip
  if (params.option()=="pattgen_MPA")
  {
    patterngen* my_pgen = new patterngen(params.inputfile(),params.outfile(),params.nevt());
    delete my_pgen;
  }

  return 0;
}
