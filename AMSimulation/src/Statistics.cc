//
//  Statistics.cpp
//  LinearizedTrackFitting
//
//  Created by Luciano Ristori on 5/5/14.
//  Copyright (c) 2014 Luciano Ristori. All rights reserved.
//

#include <cmath>
#include "Statistics.h"


Statistics::Statistics() {
    
    nEntries = 0;;
    mean = 0.;
    variance = 0.;
    
};

void Statistics::fill(double x){
    ++ nEntries;
    mean += (x - mean)/nEntries;
    if(nEntries >=2) variance += (x - mean)*(x - mean)/(nEntries-1) - variance/nEntries;
}

long int Statistics::getEntries(){
    return nEntries;
};
double Statistics::getMean() {
    return mean;
};
double Statistics::getVariance(){
    return variance;
};
double Statistics::getSigma(){
    return sqrt(variance);
};

