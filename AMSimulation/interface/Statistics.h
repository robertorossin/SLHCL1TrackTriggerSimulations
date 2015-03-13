//
//  Statistics.h
//  LinearizedTrackFitting
//
//  Created by Luciano Ristori on 5/5/14.
//  Copyright (c) 2014 Luciano Ristori. All rights reserved.
//
//
//  2015/03/12: Included in this package and modified with permission from
//              Luciano Ristori.
//

#ifndef __LinearizedTrackFitting__Statistics__
#define __LinearizedTrackFitting__Statistics__

class Statistics {
public:
    long int nEntries;
    double mean;
    double variance;

    Statistics();
    ~Statistics() {}

    void fill(double x);

    long int getEntries()   const;
    double   getMean()      const;
    double   getVariance()  const;
    double   getSigma()     const;
};

#endif /* defined(__LinearizedTrackFitting__Statistics__) */
