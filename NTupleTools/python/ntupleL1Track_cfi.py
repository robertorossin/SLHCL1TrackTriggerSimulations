import FWCore.ParameterSet.Config as cms

ntupleStubs= cms.EDProducer('NTupleStubs',
    inputTagClus = cms.InputTag('TTClustersFromPixelDigis', 'ClusterInclusive'),
    inputTagStub = cms.InputTag('TTStubsFromPixelDigis', 'StubAccepted'),
    inputTagTrack = cms.InputTag('TTTracksFromPixelDigis', 'Level1TTTracks'),
    inputTagClusMCAssoc = cms.InputTag('TTClusterAssociatorFromPixelDigis', 'ClusterInclusive'),
    inputTagStubMCAssoc = cms.InputTag('TTStubAssociatorFromPixelDigis', 'StubAccepted'),
    inputTagTrackMCAssoc = cms.InputTag('TTTrackAssociatorFromPixelDigis', 'Level1TTTracks'),
    inputTagTrkPart = cms.InputTag('mix', 'MergedTrackTruth'),
    inputTagTrkVertex = cms.InputTag('mix', 'MergedTrackTruth'),
    inputTagSimTrack = cms.InputTag('g4SimHits'),
    inputTagSimVertex = cms.InputTag('g4SimHits'),
    inputTagSimPixelDigi = cms.InputTag('simSiPixelDigis'),
    beamSpotTag = cms.InputTag('BeamSpotFromSim', 'BeamSpot'),
    prefixClus = cms.string('TTClusters@'),
    prefixStub = cms.string('TTStubs@'),
    prefixTrack = cms.string('TTTracks@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

# No track fitting yet
ntupleL1Track = cms.Sequence(ntupleStubs)

