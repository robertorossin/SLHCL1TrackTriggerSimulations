import FWCore.ParameterSet.Config as cms

ntuplemaker = cms.EDAnalyzer("NTupleMaker",
    outputCommands = cms.untracked.vstring(
        "drop *",
        "keep *_ntuple*_*_*",
    )
)
