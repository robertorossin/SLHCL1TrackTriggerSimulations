#include "jobparams.h"

//tclap
#include <tclap/CmdLine.h>
using namespace TCLAP;

jobparams::jobparams(int argc, char** argv){
  


   try {
     // command line parser
     CmdLine cmd("Command option", ' ', "0.9");

     ValueArg<std::string> option("c","case","type of analysis (rates/sectors/rate_n_sec/sec_n_test/stub_eff/PR_eff)",
				false, "rates", "string");
     cmd.add(option);

     ValueArg<std::string> pattfile("d","data","name of the pattern data file",
				   false, "", "string");
     cmd.add(pattfile);

     ValueArg<std::string> testfile("f","fortest","name of the input file used to test",
				    false, "test.root", "string");
     cmd.add(testfile);

     ValueArg<bool> dbg("g","gdb","debug mode (standalone PR used) or not",
			false, 0, "bool");
     cmd.add(dbg);

     ValueArg<std::string> inputfile("i","input","path and name of the input file",
				false, "/scratch/viret/data.root", "string");
     cmd.add(inputfile);

     ValueArg<int> nevt("n","nevt","number of events for the eff test?",
			false, 0, "int");
     cmd.add(nevt);

     ValueArg<std::string> outfile("o","output","name of the output file",
				false, "output.root", "string");
     cmd.add(outfile);
         
     ValueArg<float> ptmin("p","ptmin","minimal pt value for primary selection (in GeV/c)",
			   false, 2., "float");
     cmd.add(ptmin);
 
     ValueArg<float> rmax("i","rmax","maximal d0 value for primary selection (in cm)",
			  false, 0.1, "float");
     cmd.add(rmax);

     ValueArg<int> type("t","type","PDG id of the particle to analyze",
			false, 13, "int");
     cmd.add(type);

     ValueArg<int> rate("r","type","L1 rate (for pattern gen, in kHz)",
			false, 100, "int");
     cmd.add(type);

     // parse
     cmd.parse(argc, argv);
     
     m_inputfile    = inputfile.getValue();
     m_outfile      = outfile.getValue();
     m_testfile     = testfile.getValue();
     m_pattfile     = pattfile.getValue();
     m_opt          = option.getValue();
     m_nevt         = nevt.getValue();
     m_dbg          = dbg.getValue();
     m_type         = type.getValue();
     m_rmax         = rmax.getValue();
     m_ptmin        = ptmin.getValue();
     m_rate         = rate.getValue();
   }
   catch (ArgException &e){ // catch exception from parse
     std::cerr << "ERROR: " << e.error() << " for arg " << e.argId()  << std::endl;
     abort();
   }
}

