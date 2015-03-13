//
//  Attributes.h
//  BuildPatternBank
//
//  Created by Luciano Ristori on 3/8/15.
//  Copyright (c) 2015 Luciano Ristori. All rights reserved.
//
//
//  2015/03/12: Included in this package and modified with permission from
//              Luciano Ristori.
//

#ifndef __BuildPatternBank__Attributes__
#define __BuildPatternBank__Attributes__

#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Statistics.h"

// Pattern attributes

class Attributes {
public:
    long int id;         // identifier
    long int n;          // popularity
    Statistics invPt;    // mean and sigma of track parameters
    Statistics cotTheta; // mean and sigma of track parameters
    Statistics phi;      // mean and sigma of track parameters
    Statistics z0;       // mean and sigma of track parameters

    Attributes();
    ~Attributes() {}
};

#endif /* defined(__BuildPatternBank__Attributes__) */
