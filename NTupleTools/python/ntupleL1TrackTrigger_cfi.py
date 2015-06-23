import FWCore.ParameterSet.Config as cms

ntuplePixelDigis = cms.EDProducer('NTuplePixelDigis',
    inputTag = cms.InputTag('simSiPixelDigis'),
    inputTagTP = cms.InputTag('mix', 'MergedTrackTruth'),
    prefix = cms.string('pixelDigis@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleTTClusters = cms.EDProducer('NTupleTTClusters',
    inputTag = cms.InputTag('TTClustersFromPixelDigis', 'ClusterInclusive'),
    inputTagMC = cms.InputTag('TTClusterAssociatorFromPixelDigis', 'ClusterInclusive'),
    inputTagDigi = cms.InputTag('simSiPixelDigis'),
    prefix = cms.string('TTClusters@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleTTStubs = cms.EDProducer('NTupleTTStubs',
    inputTag = cms.InputTag('TTStubsFromPixelDigis', 'StubAccepted'),
    inputTagMC = cms.InputTag('TTStubAssociatorFromPixelDigis', 'StubAccepted'),
    inputTagClus = cms.InputTag('TTClustersFromPixelDigis', 'ClusterInclusive'),
    inputTagDigi = cms.InputTag('simSiPixelDigis'),
    prefix = cms.string('TTStubs@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleTTTracks = cms.EDProducer('NTupleTTTracks',
    inputTag = cms.InputTag('TTTracksFromPixelDigis', 'Level1TTTracks'),
    inputTagMC = cms.InputTag('TTTrackAssociatorFromPixelDigis', 'Level1TTTracks'),
    inputTagStub = cms.InputTag('TTStubsFromPixelDigis', 'StubAccepted'),
    nparameters = cms.int32(4),
    prefix = cms.string('TrackletTTTracks@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleTTClustersForTTI = ntupleTTClusters.clone(
    inputTag = cms.InputTag('TTStubsFromPixelDigis', 'ClusterAccepted'),
    inputTagMC = cms.InputTag('TTClusterAssociatorFromPixelDigis', 'ClusterAccepted'),
    inputTagDigi = cms.InputTag(''),
)

ntupleTTStubsForTTI = ntupleTTStubs.clone(
    inputTagClus = cms.InputTag('TTStubsFromPixelDigis', 'ClusterAccepted'),
    inputTagDigi = cms.InputTag(''),
)

ntupleL1TrackTrigger = cms.Sequence(ntuplePixelDigis * ntupleTTClusters * ntupleTTStubs * ntupleTTTracks)
ntupleL1TrackTrigger_TTI = cms.Sequence(ntupleTTClustersForTTI * ntupleTTStubsForTTI * ntupleTTTracks)

