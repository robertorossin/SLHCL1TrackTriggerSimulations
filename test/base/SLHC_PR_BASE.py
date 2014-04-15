import FWCore.ParameterSet.Config as cms

process = cms.Process("PReco")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.options = cms.untracked.PSet(
    SkipEvent = cms.untracked.vstring('ProductNotFound')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1) # Don't change this!!!!!!
)

process.source = cms.Source("EmptySource")

process.preco = cms.EDAnalyzer('PatternExtractor',
                               inputRootFile = cms.string('INPUTFILENAME'),
                               outputRootFile= cms.string('OUTPUTFILENAME'),
                               inputBankFile = cms.string('BANKFILENAME'),
                               n_events      = cms.untracked.int32(NEVTS),
                               skip_events   = cms.untracked.int32(NSKIP),
                               keepID        = cms.untracked.int32(USEID),
                               threshold     = cms.int32(5))

process.p = cms.Path(process.preco)
