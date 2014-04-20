import FWCore.ParameterSet.Config as cms

ntupleEventInfo = cms.EDProducer("NTupleEventInfo",
    prefix = cms.string(''),
    suffix = cms.string(''),
)

ntupler = cms.EDAnalyzer("NTupleMaker",
    outputCommands = cms.untracked.vstring(
        "drop *",
        "keep *_ntuple*_*_*",
    )
)
