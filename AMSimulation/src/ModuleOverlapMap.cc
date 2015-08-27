#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/ModuleOverlapMap.h"
using namespace slhcl1tt;

#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "TFile.h"
#include "TTree.h"



// _____________________________________________________________________________
void ModuleOverlapMap::readModuleOverlapMap(TString datadir) {
    TString crootfile = datadir + "stubOverlapHistosFramesOnlyTree.root";

    TFile fModuleOverlap(crootfile);
		assert(fModuleOverlap.IsOpen());
		TTree* tmodule = (TTree*)fModuleOverlap.Get("tmodule");
		assert(tmodule);

		Int_t id   ;
		Int_t layer;
		Int_t x1   ;
		Int_t y1   ;
		Int_t x2   ;
		Int_t y2   ;
	  tmodule->SetBranchAddress("id"   ,&id   );
	  tmodule->SetBranchAddress("layer",&layer);
	  tmodule->SetBranchAddress("x1"   ,&x1   );
	  tmodule->SetBranchAddress("x2"   ,&x2   );
	  tmodule->SetBranchAddress("y1"   ,&y1   );
	  tmodule->SetBranchAddress("y2"   ,&y2   );

	  Long64_t nModules = tmodule->GetEntries();
	  for (Long64_t i=0;i<nModules;i++) {
	  	tmodule->GetEntry(i);
	  	ModuleOverlap m(id,layer,x1,y1,x2,y2);
	  	std::pair<std::map <unsigned , ModuleOverlap>::iterator,bool> ret;
	  	ret = moduleOverlap_map_.insert(std::make_pair(id,m));
	  	assert(ret.second);
	  }
	  std::cout << Info() << "Read " << nModules << " module overlap regions." << std::endl;
	  fModuleOverlap.Close();
}
