import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")
runOnMC = True

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1

## Options and Output Report
process.options = cms.untracked.PSet(
    #wantSummary = cms.untracked.bool( True ),
    #SkipEvent = cms.untracked.vstring('ProductNotFound')
)

## Source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring("file:rawsim_numEvent20.root")
)
## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

## Geometry and Global Tags
process.load('Configuration.Geometry.GeometryExtended2023TTIReco_cff')
process.load('Configuration.Geometry.GeometryExtended2023TTI_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgradePLS3', '')

process.load('Geometry.TrackerGeometryBuilder.StackedTrackerGeometry_cfi')

## Write the TTree
process.TFileService = cms.Service("TFileService",
    fileName = cms.string("test_ntuple.root")
)

process.load("SLHCL1TrackTriggerSimulations.NTupleTools.sequences_cff")

process.p = cms.Path(process.ntupleSequence)

