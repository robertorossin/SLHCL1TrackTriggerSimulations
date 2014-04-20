import FWCore.ParameterSet.Config as cms

ntupleGenParticles = cms.EDProducer('NTupleGenParticles',
    inputTag = cms.InputTag('genParticles'),
    prefix = cms.string('genParticles@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999)
)

ntupleGenJets = cms.EDProducer('NTupleGenJets',
    inputTag = cms.InputTag('ak5GenJets'),
    prefix = cms.string('genJets@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999)
)

ntupleGenMET = cms.EDProducer('NTupleGenMET',
    inputTag = cms.InputTag('genMetTrue'),
    prefix = cms.string('genMET@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999)
)

ntupleGenEventInfo = cms.EDProducer('NTupleGenEventInfo',
    genEventInfo = cms.InputTag('generator'),
    pileupInfo = cms.InputTag('addPileupInfo'),
    pileupWeight = cms.InputTag(''),
    pdfWeight = cms.InputTag(''),
    prefix = cms.string('gen@'),
    suffix = cms.string(''),
)

ntupleGen = cms.Sequence(ntupleGenParticles * ntupleGenJets * ntupleGenMET * ntupleGenEventInfo)

