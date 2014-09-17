#ifndef AMSimulationIO_Helper_h_
#define AMSimulationIO_Helper_h_

#include <algorithm>
//#include <functional>
#include <cassert>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <vector>
#include <string>

#include "TROOT.h"  // for gROOT
class TTree;

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

void ResetDeleteBranches(TTree* tree);

}  // namespace slhcl1tt

#endif

