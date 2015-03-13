//
//  Attributes.h
//  BuildPatternBank
//
//  Created by Luciano Ristori on 3/8/15.
//  Copyright (c) 2015 Luciano Ristori. All rights reserved.
//

#ifndef __BuildPatternBank__Attributes__
#define __BuildPatternBank__Attributes__

#include <iostream>
#include "Statistics.h"

// pattern attributes

class Attributes {
public:
    long int n; // popularity
    Statistics par[4]; // mean and sigma of track parameters
    
    
};

#endif /* defined(__BuildPatternBank__Attributes__) */
