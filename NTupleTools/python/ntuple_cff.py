import FWCore.ParameterSet.Config as cms

from SLHCL1TrackTriggerSimulations.NTupleTools.ntuplegen_cfi import *
from SLHCL1TrackTriggerSimulations.NTupleTools.ntuplemaker_cfi import *

ntuple = cms.Sequence(ntuplegen * ntuplemaker)
