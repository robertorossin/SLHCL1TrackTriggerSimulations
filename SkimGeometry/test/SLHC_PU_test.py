#########################
#
# Configuration file for PileUp events
# production in tracker only
#
# Instruction to run this script are provided on this page:
#
# http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.HLLHCTuto
#
# Look at STEP II
#
# Author: S.Viret (viret@in2p3.fr)
# Date  : 12/04/2013
# Maj. modif  : 17/06/2013 (adding the official stub producer)
#
# Script tested with release CMSSW_6_2_0_SLHC5
#
#########################

import FWCore.ParameterSet.Config as cms

process = cms.Process('SIM')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedHLLHC_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('L1Trigger.TrackTrigger.TrackTrigger_cff')
process.load('SimTracker.TrackTriggerAssociation.TrackTriggerAssociator_cff')

# Special geometry (Tracker only)
process.load('SLHCL1TrackTriggerSimulations.SkimGeometry.Sim_SKIM_cff')
process.load('SLHCL1TrackTriggerSimulations.SkimGeometry.GeometryExtendedPhase2TkBEReco_SKIM_cff')
process.load('SLHCL1TrackTriggerSimulations.SkimGeometry.mixPU_SKIM_cfi')
process.load('SLHCL1TrackTriggerSimulations.SkimGeometry.Digi_SKIM_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(20)
)

# Input source
process.source = cms.Source("EmptySource")

process.mix.input.nbPileupEvents.averageNumber = cms.double(15.0)  # The average number of pileup events you want  
process.mix.input.fileNames     = cms.untracked.vstring('file:MBias_10_test.root') # The file where to pick them up

# Additional output definition

# Other statements
# Global tag for PromptReco
process.genstepfilter.triggerConditions=cms.vstring("generation_step")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgradePLS3', '')

process.RandomNumberGeneratorService.generator.initialSeed      = 20
process.RandomNumberGeneratorService.VtxSmeared.initialSeed     = 2
process.RandomNumberGeneratorService.g4SimHits.initialSeed      = 178
process.RandomNumberGeneratorService.mix.initialSeed            = 210

# Generate particle gun events
process.generator = cms.EDProducer("FlatRandomPtGunProducer",
    PGunParameters = cms.PSet(
        MinPt  = cms.double(10.),
        MaxPt  = cms.double(50.),
	#XFlatSpread = cms.double(1.5),  # In mm (requires an update 
	#YFlatSpread = cms.double(1.5),  # In mm  of the official 
	#ZFlatSpread = cms.double(150.), # In mm  PGUN code, see tutorial)	
        PartID = cms.vint32(-13),
        MinEta = cms.double(-2.5),
        MaxEta = cms.double(2.5),
        MinPhi = cms.double(0.),
	MaxPhi = cms.double(6.28)
    ),
    Verbosity = cms.untracked.int32(0),
    AddAntiParticle = cms.bool(False),
)


# Output definition

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RAWSIMEventContent.outputCommands,
    fileName = cms.untracked.string('PU_20_sample_test.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    )
)

process.RAWSIMoutput.outputCommands.append('keep  *_*_MergedTrackTruth_*')


# Path and EndPath definitions
process.generation_step      = cms.Path(process.pgen)
process.simulation_step      = cms.Path(process.psim)
process.genfiltersummary_step= cms.EndPath(process.genFilterSummary)
process.digitisation_step    = cms.Path(process.pdigi)
process.L1TrackTrigger_step  = cms.Path(process.TrackTriggerClustersStubs)
process.L1TTAssociator_step  = cms.Path(process.TrackTriggerAssociatorClustersStubs)
process.endjob_step          = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step    = cms.EndPath(process.RAWSIMoutput)

process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.digitisation_step,process.L1TrackTrigger_step,process.L1TTAssociator_step,process.endjob_step,process.RAWSIMoutput_step)

# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq
	
# Automatic addition of the customisation function

from SLHCUpgradeSimulations.Configuration.phase2TkCustomsBE5D import customise as customiseBE5D
from SLHCUpgradeSimulations.Configuration.phase2TkCustomsBE5D import l1EventContent as customise_ev_BE5D

process=customiseBE5D(process)
process=customise_ev_BE5D(process)
