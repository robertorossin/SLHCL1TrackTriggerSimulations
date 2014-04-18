#########################
#
# Configuration file for fast pattern recognition
# using a pattern bank
#
# Instruction to run this script are provided on this page:
#
# http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.HLLHCTuto
#
# Look at STEP V
#
# Author: S.Viret (viret@in2p3.fr)
# Date        : 16/01/2014
#
# Script tested with release CMSSW_6_2_0_SLHC5
#
#########################

import FWCore.ParameterSet.Config as cms

process = cms.Process('PReco')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('L1Trigger.TrackFindingAM.L1AMTrack_cfi')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('L1Trigger.TrackTrigger.TrackTrigger_cff')
process.load('SimTracker.TrackTriggerAssociation.TrackTriggerAssociator_cff')

# Special geometry (Tracker only)
process.load('SLHCL1TrackTriggerSimulations.SkimGeometry.GeometryExtendedPhase2TkBEReco_SKIM_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

# Input source
process.source = cms.Source("PoolSource",
                            #fileNames = cms.untracked.vstring('file:PGun_example.root'),
                            fileNames = cms.untracked.vstring('file:PU_10_sample_off.root'),       
                            duplicateCheckMode = cms.untracked.string( 'noDuplicateCheck' )
)

# Additional output definition
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgradePLS3', '')

# Some pattern recognition options
process.TTTracksFromPixelDigisAM.inputBankFile = cms.string('/afs/cern.ch/work/s/sviret/testarea/PatternBanks/BE_5D/Eta7_Phi8/ss32_cov40/612_SLHC6_MUBANK_lowmidhig_sec30_ss32_cov40.pbk')
process.TTTracksFromPixelDigisAM.threshold     = cms.int32(5)

# Output definition

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RAWSIMEventContent.outputCommands,
    fileName = cms.untracked.string('output_AM.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM')
    )
)

process.RAWSIMoutput.outputCommands.append('keep  *_*_MergedTrackTruth_*')
process.RAWSIMoutput.outputCommands.append('keep *_TTTracksFromPixelDigisAM_*_*')

# Path and EndPath definitions
process.L1AMPR_step          = cms.Path(process.TTTracksFromPixelDigisAM)
process.endjob_step          = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step    = cms.EndPath(process.RAWSIMoutput)

process.schedule = cms.Schedule(process.L1AMPR_step,process.endjob_step,process.RAWSIMoutput_step)
	
# Automatic addition of the customisation function

from SLHCUpgradeSimulations.Configuration.phase2TkCustomsBE5D import customise as customiseBE5D
from SLHCUpgradeSimulations.Configuration.phase2TkCustomsBE5D import l1EventContent as customise_ev_BE5D

process=customiseBE5D(process)
process=customise_ev_BE5D(process)
