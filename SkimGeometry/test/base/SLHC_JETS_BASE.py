import FWCore.ParameterSet.Config as cms

process = cms.Process('EXTR')

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
process.load('SLHCL1TrackTriggerSimulations.SkimGeometry.mixNoPU_SKIM_cfi')
process.load('SLHCL1TrackTriggerSimulations.SkimGeometry.Digi_SKIM_cff')


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(NEVTS)
)

# Input source
process.source = cms.Source("EmptySource")

# Additional output definition


# Other statements
process.genstepfilter.triggerConditions=cms.vstring("generation_step")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'MYGLOBALTAG', '')

# Load the extracto
process.load("Extractors.RecoExtractor.MIB_extractor_cff")

process.MIBextraction.doMC             = True
process.MIBextraction.doPixel          = True
process.MIBextraction.doMatch          = True
process.MIBextraction.doSTUB           = True

process.RandomNumberGeneratorService.generator.initialSeed      = NSEEDA
process.RandomNumberGeneratorService.VtxSmeared.initialSeed     = NSEEDB
process.RandomNumberGeneratorService.g4SimHits.initialSeed      = NSEEDC
process.RandomNumberGeneratorService.mix.initialSeed            = NSEEDD

# for jets
from Configuration.Generator.PythiaUESettings_cfi import *
process.load("Configuration.Generator.TTbar_cfi")
process.generator.comEnergy = cms.double(14000.0)
#process.generator.pythiaHepMCVerbosity = cms.untracked.bool(True)
process.generator.PythiaParameters.processParameters = cms.vstring(
    'MSEL=0          !  for user specification of sub-processes',
    'MSUB(11)=1      !  qq->qq   ON, if one needs quark jets',
#    'MSUB(68)=1      !  gg->gg   ON, if one needs gluon jets',
    'MSUB(81)=1      !  qq->qq   qq->QQ massive',
    'MSUB(82)=1      !  gg->gg   gg->QQ massive',
    'CKIN(3)=300.    !  Pt low cut but also the Et jet required',
    'CKIN(4)=1000.    ! Pt hat upper cut', 
    'CKIN(13)=0.     !  etamin',
    'CKIN(14)=ETAMAX   !  etamax',
    'CKIN(15)=ETAMIN   ! -etamax',
    'CKIN(16)=0.     ! -etamin',
#    'MSTP(7)=2       ! 2 for UU_bar',
#    'MSTP(7)=4       ! 4 for CC_bar',
    'MSTP(7)   = 5     ! 5 for BB_bar',
#    'MSTP(7)   = 6     ! flavour = top', 
#    'PMAS(6,1) = 175.  ! top quark mass'
    )

# Output definition

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RAWSIMEventContent.outputCommands,
    fileName = cms.untracked.string('JETS_example.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    )
)

process.RAWSIMoutput.outputCommands.append('keep  *_*_MergedTrackTruth_*')

process.MIBextraction.doL1TT           = True

process.MIBextraction.analysisSettings = cms.untracked.vstring(
    "evtNum RUN",
    "matchedStubs 0",
    "posMatching  1",
    "zMatch  0",
    "maxClusWdth  4",
    "thresh THRESHOLD",  
    "windowSize   -1",
    "pdgSel -1",
    "verbose 0"
    )


# Path and EndPath definitions
process.generation_step      = cms.Path(process.pgen)
process.simulation_step      = cms.Path(process.psim)
process.genfiltersummary_step= cms.EndPath(process.genFilterSummary)
process.digitisation_step    = cms.Path(process.pdigi)
process.L1TrackTrigger_step  = cms.Path(process.TrackTriggerClustersStubs)
process.L1TTAssociator_step  = cms.Path(process.TrackTriggerAssociatorClustersStubs)
process.endjob_step          = cms.EndPath(process.endOfProcess)
process.p                    = cms.Path(process.MIBextraction)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)

process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.digitisation_step,process.L1TrackTrigger_step,process.L1TTAssociator_step,process.p,process.endjob_step,process.RAWSIMoutput_step)

# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq
	
# Automatic addition of the customisation function

from SLHCUpgradeSimulations.Configuration.phase2TkCustomsBE5D import customise as customiseBE5D
from SLHCUpgradeSimulations.Configuration.phase2TkCustomsBE5D import l1EventContent as customise_ev_BE5D

process=customiseBE5D(process)
process=customise_ev_BE5D(process)


