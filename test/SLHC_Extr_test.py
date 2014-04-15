#########
#
# Example script to run the python extractor on MC events
# for the skimmed geometry
# 
# Usage: cmsRun SLHC_Extr_test.py
#
# More info:
# http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.HLLHCTuto
#
# Look at STEP III
#
# Author: S.Viret (viret@in2p3.fr)
# Date  : 12/04/2013
# Maj. modif  : 17/06/2013 (adding the official stub producer)
# Maj. modif  : 10/01/2014 (going to new CMSSW release)
#
# Script tested with release CMSSW_6_2_0_SLHC5
#
#########


import FWCore.ParameterSet.Config as cms

process = cms.Process("MIBextractor")

process.load('Configuration/StandardSequences/Services_cff')
process.load('Configuration/StandardSequences/EndOfProcess_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")
process.load('L1Trigger.TrackTrigger.TrackTrigger_cff')
process.load('SimTracker.TrackTriggerAssociation.TrackTriggerAssociator_cff')


# Special geometry (Tracker only)
process.load('DataProduction.SkimGeometry.GeometryExtendedPhase2TkBEReco_SKIM_cff')
process.load('DataProduction.SkimGeometry.Digi_SKIM_cff')

# Other statements

# Global tag for PromptReco
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgradePLS3', '')

process.options = cms.untracked.PSet(
    SkipEvent = cms.untracked.vstring('ProductNotFound')
)

# Number of events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# The file you want to extract
process.source = cms.Source("PoolSource",
                            #fileNames = cms.untracked.vstring('file:PGun_example.root'),
                            fileNames = cms.untracked.vstring('file:PU_20_sample_test.root'),       
                            duplicateCheckMode = cms.untracked.string( 'noDuplicateCheck' )
)

# Load the extracto
process.load("Extractors.RecoExtractor.MIB_extractor_cff")

# Tune some options (see MIB_extractor_cfi.py for details)

process.MIBextraction.doMC             = True
process.MIBextraction.doSTUB           = True
process.MIBextraction.doPixel          = True
process.MIBextraction.doL1TT           = True
process.MIBextraction.doMatch          = True

process.MIBextraction.analysisSettings = cms.untracked.vstring(
    "matchedStubs 0",
    "posMatching  1",
    "zMatch  0",
    "maxClusWdth  4",
    "windowSize   -1",
    "pdgSel -1",
    "verbose 0"
    )

process.p = cms.Path(process.MIBextraction)


