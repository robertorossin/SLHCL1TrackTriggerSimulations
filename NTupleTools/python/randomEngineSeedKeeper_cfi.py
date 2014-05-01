import FWCore.ParameterSet.Config as cms

randomEngineSeedKeeper = cms.EDProducer("RandomEngineSeedKeeper",
    seeds = cms.vstring('generator','g4SimHits','VtxSmeared','mix')
)
