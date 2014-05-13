#ifndef AMSimulation_Helper_h_
#define AMSimulation_Helper_h_

#include <algorithm>
//#include <functional>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <map>
#include <memory>
#include <vector>
#include <string>
#include "TROOT.h"
#include "TString.h"
#include "TStopwatch.h"


namespace slhcl1tt {

TString Color(const TString& c);
TString EndColor();
void NoColor();

void Timing(bool start=false);
void ShowTiming();

TString Error();
TString Warning();
TString Info();
TString Debug();

std::size_t hashFileEvent(TString src, unsigned evt);

}  // namespace slhcl1tt


#include "TTree.h"
#include "TBranchElement.h"
void ResetDeleteBranches(TTree* tree);

#endif

