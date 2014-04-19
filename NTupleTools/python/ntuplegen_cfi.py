import FWCore.ParameterSet.Config as cms

ntupleGenJets = cms.EDProducer("NTupleGenJets",
    inputTag = cms.InputTag('ak5GenJets'),
    prefix = cms.string('genJets@'),
    suffix = cms.string(''),
    minPt = cms.double(-1),
    maxEta = cms.double(999),
    maxN = cms.uint32(99)
)

ntupleGenMET = cms.EDProducer( "NTupleGenMET",
    inputTag = cms.InputTag('genMetTrue'),
    prefix = cms.string('genMET@'),
    suffix = cms.string('')
)
