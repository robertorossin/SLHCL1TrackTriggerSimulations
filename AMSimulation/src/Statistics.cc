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


// _____________________________________________________________________________
Statistics::Statistics()
: n_(0),
  mean_(0.),
  variance_(0.) {}

void Statistics::fill(double x) {
    ++ n_;
    mean_ += (x - mean_)/n_;
    if (n_ > 1)  variance_ += (x - mean_)*(x - mean_)/(n_-1) - variance_/n_;
}

long int Statistics::getEntries() const {
    return n_;
};

double Statistics::getMean() const {
    return mean_;
};

double Statistics::getVariance() const {
    return variance_;
};

double Statistics::getSigma() const {
    return std::sqrt(variance_);
};


// _____________________________________________________________________________
Statistics2::Statistics2()
: n_(0),
  mean1_(0.),
  mean2_(0.),
  covariance_(0.) {}

void Statistics2::fill(double x, double y) {
    ++ n_;
    mean1_ += (x - mean1_)/n_;
    mean2_ += (y - mean2_)/n_;
    if (n_ > 1)  covariance_ += (x - mean1_)*(y - mean2_)/(n_-1) - covariance_/n_;
}

long int Statistics2::getEntries() const {
    return n_;
};

double Statistics2::getMeanX() const {
    return mean1_;
};

double Statistics2::getMeanY() const {
    return mean2_;
};

double Statistics2::getCovariance() const {
    return covariance_;
};

double Statistics2::getSigma() const {
    return std::sqrt(covariance_);
};
