#include <iostream>
#include <fstream>
#include <iomanip>

// Internal includes

#include "rates.h"
#include "sector.h"
#include "sector_test.h"
#include "jobparams.h"
#include "TROOT.h"

using namespace std;

int main(int argc, char** argv) {

  // Necessary lines to make branches containing vectors
  gROOT->ProcessLine(".L Loader.C+");

  // read jobParams
  jobparams params(argc,argv);

  // Depending on the option chosen, process the information


  // Option 1: just do the rate calculation
  if (params.rates()==1 && params.sectors()==0)
  {
    rates* my_rates = new rates(params.inputfile(),params.outfile());
    delete my_rates;
  }
  

  // Option 2: just do the sectors calculation
  if (params.sectors()==1 && params.rates()==0)
  {
    sector* my_sectors = new sector(params.inputfile(),params.outfile(),
				    params.eta(),params.phi(),
				    params.oeta(),params.ophi());
    delete my_sectors;
  }

  // Option 3: do both steps
  if (params.rates()==1 && params.sectors()==1)
  {
    rates* my_rates = new rates(params.inputfile(),"my_rates_temp.root");
    delete my_rates;

    sector* my_sectors = new sector("my_rates_temp.root",params.outfile(),
				    params.eta(),params.phi(),
				    params.oeta(),params.ophi());
    delete my_sectors;
  }


  // Option 4: do sectors and test

  if (params.sectors()==1 && params.rates()==0 && params.test()==1)
  {
    sector* my_sectors = new sector(params.inputfile(),"my_sectors_temp.root",
				    params.eta(),params.phi(),
				    params.oeta(),params.ophi());
    delete my_sectors;

    sector_test* my_test = new sector_test(params.testfile(),"my_sectors_temp.root",
					   params.outfile(),params.nevt());

    delete my_test;
  }

  return 0;
}
