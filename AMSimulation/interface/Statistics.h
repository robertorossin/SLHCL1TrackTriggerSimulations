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
    long int n_;
    float mean_;
    float variance_;

    Statistics();
    ~Statistics() {}

    void fill(float x);

    long int getEntries()   const;
    float   getMean()      const;
    float   getVariance()  const;
    float   getSigma()     const;
};

class ShortStatistics {
  public:
    long int n_;
    float mean_;

    ShortStatistics();
    ~ShortStatistics() {}

    void fill(float x);

    long int getEntries()   const;
    float   getMean()      const;
};

class Statistics2 {
  public:
    long int n_;
    float mean1_;
    float mean2_;
    float covariance_;

    Statistics2();
    ~Statistics2() {}

    void fill(float x, float y);

    long int getEntries()     const;
    float   getMeanX()       const;
    float   getMeanY()       const;
    float   getCovariance()  const;
    float   getSigma()       const;
};

#endif /* defined(__LinearizedTrackFitting__Statistics__) */
