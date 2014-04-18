import FWCore.ParameterSet.Config as cms

process = cms.Process("EXTR")
runOnMC = True

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

## Options and Output Report
process.options = cms.untracked.PSet(
    #wantSummary = cms.untracked.bool( True ),
    #SkipEvent = cms.untracked.vstring('ProductNotFound')
)

## Source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
"/store/user/l1upgrades/SLHC/GEN/620_SLHC10/SingleMuPlusMinus_20140417/SingleMuPlusMinus_E2023TTI_21_1_JHj.root"
    )
)
## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

## Write the TTree
process.TFileService = cms.Service("TFileService",
    fileName = cms.string(
        "tree.root"
    )
)

process.load("SLHCL1TrackTriggerSimulations.NTupleTools.ntuple_cff")

process.p = cms.Path(process.ntuple)


