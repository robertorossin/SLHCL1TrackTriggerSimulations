#include <iostream>
#include <fstream>
#include <iomanip>

// Internal includes

#include "rates.h"
#include "patterngen.h"
#include "sector.h"
#include "sector_test.h"
#include "efficiencies.h"
#include "jobparams.h"
#include "TROOT.h"

using namespace std;

///////////////////////////////////
//
//
// Base code for the AM analysis tool
//
// Available methods are described in the tutorial (part III)
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
  if (params.option()=="rates")
  {
    rates* my_rates = new rates(params.inputfile(),params.outfile());
    delete my_rates;    
  }
  

  // Option 2: just do the sectors calculation (please note that this option is
  // for debugging only, official sector determination is made by the TkLayout tool)
  if (params.option()=="sectors")
  {
    sector* my_sectors = new sector(params.inputfile(),params.outfile(),
				    params.eta(),params.phi(),
				    params.oeta(),params.ophi());
    delete my_sectors;
  }

  // Option 3: do both steps

  if (params.option()=="rate_n_sec")
  {
    rates* my_rates = new rates(params.inputfile(),"my_rates_temp.root");
    delete my_rates;

    sector* my_sectors = new sector("my_rates_temp.root",params.outfile(),
				    params.eta(),params.phi(),
				    params.oeta(),params.ophi());
    delete my_sectors;
  }

  // Option 4: do sectors and test

  if (params.option()=="sec_n_test")
  {
    sector* my_sectors = new sector(params.inputfile(),"my_sectors_temp.root",
				    params.eta(),params.phi(),
				    params.oeta(),params.ophi());
    delete my_sectors;

    sector_test* my_test = new sector_test(params.testfile(),"my_sectors_temp.root",
					   params.pattfile(),params.outfile(),params.nevt(),params.dbg());

    delete my_test;
  }
  
  // Option 5: efficiencies

  if (params.option()=="stub_eff")
  {
    efficiencies* my_effs = new efficiencies(params.inputfile(),params.outfile());
    delete my_effs;
  }

  // Option 6: sector test only 

  if (params.option()=="PR_eff")
  {
    sector_test* my_test = new sector_test(params.testfile(),params.inputfile(),
					   params.pattfile(),params.outfile(),
					   params.nevt(),params.dbg());

    delete my_test;
  }

  // Option 7: generate a serie of patterns for the concentrator
  if (params.option()=="pattgen")
  {
    patterngen* my_pgen = new patterngen(params.inputfile(),params.outfile(),params.nevt());
    delete my_pgen;
  }
  
  return 0;
}
