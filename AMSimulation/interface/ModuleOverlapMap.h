#ifndef AMSimulation_ModuleOverlapMap_h_
#define AMSimulation_ModuleOverlapMap_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Helper.h"
#include <map>
#include <string>
#include <vector>
#include "TString.h"


namespace slhcl1tt {

class ModuleOverlap {
public:
	Int_t id   ;
	Int_t layer;
	Int_t x1   ;
	Int_t y1   ;
	Int_t x2   ;
	Int_t y2   ;

//		ModuleOverlap() {}
	ModuleOverlap(unsigned iid,unsigned ilayer,unsigned ix1,unsigned iy1,unsigned ix2,unsigned iy2) {
		id    = iid   ;
		layer = ilayer;
		x1    = ix1   ;
		y1    = iy1   ;
		x2    = ix2   ;
		y2    = iy2   ;
	}
	~ModuleOverlap() {}
};


class ModuleOverlapMap {
public:
	// Define boundaries of each layer

	// Constructor
	ModuleOverlapMap() {}

  // Destructor
  ~ModuleOverlapMap() {}

  // Functions
  // Read overlap modules map root file
  void readModuleOverlapMap(TString datadir);

//private:
  // Module overlap data
  std::map<unsigned, ModuleOverlap>                moduleOverlap_map_;

};

}  // namespace slhcl1tt

#endif
