import FWCore.ParameterSet.Config as cms

ntupleStubs= cms.EDProducer('NTupleStubs',
    inputTagClus = cms.InputTag('TTClustersFromPixelDigis', 'ClusterInclusive'),
    inputTagStub = cms.InputTag('TTStubsFromPixelDigis', 'StubAccepted'),
    inputTagTrack = cms.InputTag('TTTracksFromPixelDigis', 'Level1TTTracks'),
    inputTagDigi = cms.InputTag('simSiPixelDigis'),
    inputTagClusMCAssoc = cms.InputTag('TTClusterAssociatorFromPixelDigis', 'ClusterInclusive'),
    inputTagStubMCAssoc = cms.InputTag('TTStubAssociatorFromPixelDigis', 'StubAccepted'),
    inputTagTrackMCAssoc = cms.InputTag('TTTrackAssociatorFromPixelDigis', 'Level1TTTracks'),
    prefixClus = cms.string('TTClusters@'),
    prefixStub = cms.string('TTStubs@'),
    prefixTrack = cms.string('TTTracks@'),
    prefixDigi = cms.string('simPixelDigis@'),
    suffix = cms.string(''),
    #cut = cms.string(''),
    #maxN = cms.uint32(999999)
)

# No track fitting yet
ntupleL1Track = cms.Sequence(ntupleStubs)

