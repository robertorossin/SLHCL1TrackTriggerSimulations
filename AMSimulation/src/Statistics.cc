//
//  Statistics.cpp
//  LinearizedTrackFitting
//
//  Created by Luciano Ristori on 5/5/14.
//  Copyright (c) 2014 Luciano Ristori. All rights reserved.
//
//
//  2015/03/12: Included in this package and modified with permission from
//              Luciano Ristori.
//

#include <cmath>
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Statistics.h"


Statistics::Statistics()
:   nEntries(0),
    mean(0.),
    variance(0){}

void Statistics::fill(double x) {
    ++ nEntries;
    mean += (x - mean)/nEntries;
    if(nEntries >=2) variance += (x - mean)*(x - mean)/(nEntries-1) - variance/nEntries;
}

long int Statistics::getEntries() const {
    return nEntries;
};
double Statistics::getMean() const {
    return mean;
};
double Statistics::getVariance() const {
    return variance;
};
double Statistics::getSigma() const {
    return std::sqrt(variance);
};
