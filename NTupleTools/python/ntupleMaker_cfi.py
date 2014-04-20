import FWCore.ParameterSet.Config as cms

ntupler = cms.EDAnalyzer("NTupleMaker",
    outputCommands = cms.untracked.vstring(
        "drop *",
        "keep *_ntuple*_*_*",
    )
)
