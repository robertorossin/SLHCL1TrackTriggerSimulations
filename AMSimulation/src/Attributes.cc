//
//  Attributes.cpp
//  BuildPatternBank
//
//  Created by Luciano Ristori on 3/8/15.
//  Copyright (c) 2015 Luciano Ristori. All rights reserved.
//
//
//  2015/03/12: Included in this package and modified with permission from
//              Luciano Ristori.
//

#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Attributes.h"

Attributes::Attributes()
:   id(0), n(0), invPt(), cotTheta(), phi(), z0() {}

ShortAttributes::ShortAttributes()
:   invPt(), cotTheta(), phi(), z0() {}
