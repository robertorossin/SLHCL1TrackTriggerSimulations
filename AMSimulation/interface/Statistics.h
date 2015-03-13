//
//  Statistics.h
//  LinearizedTrackFitting
//
//  Created by Luciano Ristori on 5/5/14.
//  Copyright (c) 2014 Luciano Ristori. All rights reserved.
//

#ifndef __LinearizedTrackFitting__Statistics__
#define __LinearizedTrackFitting__Statistics__

#include <iostream>

class Statistics {
    
public:
    
    long int nEntries;
    double mean;
    double variance;
    
    
    Statistics();
    
    void fill(double x);
    
    long int getEntries();
    double getMean();
    double getVariance();
    double getSigma();    
};

#endif /* defined(__LinearizedTrackFitting__Statistics__) */
