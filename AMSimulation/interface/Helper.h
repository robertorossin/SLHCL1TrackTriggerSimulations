#ifndef AMSimulation_Helper_h_
#define AMSimulation_Helper_h_

#include <algorithm>
//#include <functional>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <memory>
#include <vector>
#include <string>
#include "TString.h"


namespace slhcl1tt {

TString Color(const TString& c);
TString EndColor();
TString Error();
TString Warning();
TString Info();
TString Debug();

std::size_t hashFileEvent(TString src, unsigned evt);

}  // namespace slhcl1tt

#endif

