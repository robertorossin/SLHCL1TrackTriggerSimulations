import FWCore.ParameterSet.Config as cms

ntupleGenJets = cms.EDProducer("NTupleGenJets",
    InputTag = cms.InputTag('ak5GenJets'),
    Prefix = cms.string('GenJet.'),
    Suffix = cms.string(''),
    minPt = cms.double(-1),
    maxAbsoluteEta = cms.double(999),
    MaxSize = cms.uint32(99)
)

ntupleGenMET = cms.EDProducer( "NTupleGenMET",
    InputTag = cms.InputTag( 'genMetTrue' ),
    Prefix = cms.string( 'GenMET.' ),
    Suffix = cms.string( '' )
)
