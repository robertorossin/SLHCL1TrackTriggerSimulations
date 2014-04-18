import FWCore.ParameterSet.Config as cms

process = cms.Process('GEN')

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

# Special geometry (Tracker only)
process.load('SLHCL1TrackTriggerSimulations.SkimGeometry.Sim_SKIM_cff')
process.load('SLHCL1TrackTriggerSimulations.SkimGeometry.GeometryExtendedPhase2TkBEReco_SKIM_cff')


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


process.RandomNumberGeneratorService.generator.initialSeed      = NSEEDA
process.RandomNumberGeneratorService.VtxSmeared.initialSeed     = NSEEDB
process.RandomNumberGeneratorService.g4SimHits.initialSeed      = NSEEDC
process.RandomNumberGeneratorService.mix.initialSeed            = NSEEDD


# Output definition
process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RAWSIMEventContent.outputCommands,
    fileName = cms.untracked.string('MBiasSample.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('SIM')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('simulation_step')
    )
)


process.generator = cms.EDFilter("Pythia6GeneratorFilter",
				 pythiaPylistVerbosity = cms.untracked.int32(0),
				 filterEfficiency = cms.untracked.double(1.0),
				 pythiaHepMCVerbosity = cms.untracked.bool(False),
				 comEnergy = cms.double(14000.0),
				 maxEventsToPrint = cms.untracked.int32(0),
				 PythiaParameters = cms.PSet(
	pythiaUESettings = cms.vstring('MSTJ(11)=3     ! Choice of the fragmentation function',
				       'MSTJ(22)=2     ! Decay those unstable particles', 
				       'PARJ(71)=10 .  ! for which ctau  10 mm', 
				       'MSTP(2)=1      ! which order running alphaS',
				       'MSTP(33)=0     ! no K factors in hard cross sections',
				       'MSTP(51)=10042 ! structure function chosen (external PDF CTEQ6L1)',
				       'MSTP(52)=2     ! work with LHAPDF', 
				       'MSTP(81)=1     ! multiple parton interactions 1 is Pythia default',
				       'MSTP(82)=4     ! Defines the multi-parton model', 
				       'MSTU(21)=1     ! Check on possible errors during program execution',
				       'PARP(82)=1.8387   ! pt cutoff for multiparton interactions', 
				       'PARP(89)=1960. ! sqrts for which PARP82 is set', 
				       'PARP(83)=0.5   ! Multiple interactions: matter distrbn parameter',
				       'PARP(84)=0.4   ! Multiple interactions: matter distribution parameter',
				       'PARP(90)=0.16  ! Multiple interactions: rescaling power', 
				       'PARP(67)=2.5    ! amount of initial-state radiation', 
				       'PARP(85)=1.0  ! gluon prod. mechanism in MI', 
				       'PARP(86)=1.0  ! gluon prod. mechanism in MI',
				       'PARP(62)=1.25   ! ', 
				       'PARP(64)=0.2    ! ',
				       'MSTP(91)=1      !', 
				       'PARP(91)=2.1   ! kt distribution',
				       'PARP(93)=15.0  ! '),
	processParameters = cms.vstring('MSEL=0         ! User defined processes',
					'MSUB(11)=1     ! Min bias process',
					'MSUB(12)=1     ! Min bias process',
					'MSUB(13)=1     ! Min bias process',
					'MSUB(28)=1     ! Min bias process',
					'MSUB(53)=1     ! Min bias process',
					'MSUB(68)=1     ! Min bias process',
					'MSUB(92)=1     ! Min bias process, single diffractive',
					'MSUB(93)=1     ! Min bias process, single diffractive',
					'MSUB(94)=1     ! Min bias process, double diffractive',
					'MSUB(95)=1     ! Min bias process'),
         parameterSets = cms.vstring('pythiaUESettings',
				     'processParameters')
	)
)




# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.simulation_step   = cms.Path(process.psim)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.endjob_step,process.RAWSIMoutput_step)

# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq 
