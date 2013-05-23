#include "jobparams.h"

//tclap
#include <tclap/CmdLine.h>
using namespace TCLAP;

jobparams::jobparams(int argc, char** argv){
  


   try {
     // command line parser
     CmdLine cmd("Command option", ' ', "0.9");

              
     ValueArg<int> eta("e","eta","how many sectors in eta (bet -2.5 and 2.5)",
			  false, 1, "int");
     cmd.add(eta);

     ValueArg<int> phi("p","phi","how many sectors in phi",
			  false, 8, "int");
     cmd.add(phi);

     ValueArg<int> oeta("a","oeta","eta overlap between sectors (in %)",
			  false, 0, "int");
     cmd.add(oeta);

     ValueArg<int> ophi("b","ophi","phi overlap between sectors (in %)",
			  false, 0, "int");
     cmd.add(ophi);

     ValueArg<std::string> inputfile("i","input","path and name of the input file",
				false, "/scratch/viret/data.root", "string");
     cmd.add(inputfile);
  
     ValueArg<std::string> outfile("o","output","name of the output file",
				false, "output.root", "string");
     cmd.add(outfile);
     
     ValueArg<bool> rates("r","rates","do we perform the rate calculation? ",
			   false, 0, "bool");
     cmd.add(rates);

     ValueArg<int> sectors("s","sectors","do we perform the sector calculation?",
			   false, 0, "int");
     cmd.add(sectors);
  
     // parse
     cmd.parse(argc, argv);
     
     m_inputfile = inputfile.getValue();
     m_outfile   = outfile.getValue();
     m_sectors   = sectors.getValue();
     m_rates     = rates.getValue();
     m_eta       = eta.getValue();
     m_phi       = phi.getValue();
     m_oeta      = oeta.getValue();
     m_ophi      = ophi.getValue();
   }
   catch (ArgException &e){ // catch exception from parse
     std::cerr << "ERROR: " << e.error() << " for arg " << e.argId()  << std::endl;
     abort();
   }
}

