import FWCore.ParameterSet.Config as cms

# CMSSW/Geant4 interface
from DataProduction.SkimGeometry.g4SimHits_SKIM_cfi import *

psim = cms.Sequence(cms.SequencePlaceholder("randomEngineStateProducer")*g4SimHits)
