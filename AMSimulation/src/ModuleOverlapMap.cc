#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/ModuleOverlapMap.h"
using namespace slhcl1tt;

#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "TFile.h"
#include "TTree.h"



// _____________________________________________________________________________
void ModuleOverlapMap::readModuleOverlapMap(TString datadir) {
    TString ctxtfile  = datadir + "stubOverlapHistosFramesOnlyTree.txt";

    std::string line;
    std::ifstream overlapFile(ctxtfile);
    Int_t iLine = 0;
    Int_t nModules = 0;
    while (std::getline(overlapFile, line))
    {
        Int_t id   ;
        Int_t layer;
        Int_t x1   ;
        Int_t y1   ;
        Int_t x2   ;
        Int_t y2   ;
        std::istringstream iss(line);
        if (!(iss >> id >> layer >> x1 >> y1 >> x2 >> y2)) { break; } // error
        ++iLine;
        if (iLine==1) {
        	nModules = id; // first line contains the total # of modules in the txt file
        	continue;
        }

    	ModuleOverlap m(id,layer,x1,y1,x2,y2);
    	std::pair<std::map <unsigned , ModuleOverlap>::iterator,bool> ret;
    	ret = moduleOverlap_map_.insert(std::make_pair(id,m));
    	assert(ret.second);
    }

    assert(iLine-1==nModules);
    std::cout << Info() << "Read " << nModules << " module overlap regions." << std::endl;
}
