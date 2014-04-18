import FWCore.ParameterSet.Config as cms

# this is the configuration to model pileup for final scheme
from SLHCL1TrackTriggerSimulations.SkimGeometry.aliases_SKIM_cfi import * 
from SimGeneral.MixingModule.mixObjects_cfi import *
from SimGeneral.MixingModule.mixPoolSource_cfi import *
from SimGeneral.MixingModule.pixelDigitizer_cfi import *
from SimGeneral.MixingModule.stripDigitizer_cfi import *
from SimGeneral.MixingModule.trackingTruthProducer_cfi import *

trackingParticles.simHitCollections = cms.PSet(
    	muon = cms.VInputTag( ),
        tracker = cms.VInputTag( cms.InputTag('g4SimHits','TrackerHitsTIBLowTof'),
                                 cms.InputTag('g4SimHits','TrackerHitsTIBHighTof'),
                                 cms.InputTag('g4SimHits','TrackerHitsTIDLowTof'),
                                 cms.InputTag('g4SimHits','TrackerHitsTIDHighTof'),
                                 cms.InputTag('g4SimHits','TrackerHitsTOBLowTof'),
                                 cms.InputTag('g4SimHits','TrackerHitsTOBHighTof'),
                                 cms.InputTag('g4SimHits','TrackerHitsTECLowTof'),
                                 cms.InputTag('g4SimHits','TrackerHitsTECHighTof') ),
        pixel = cms.VInputTag(cms.InputTag( 'g4SimHits','TrackerHitsPixelBarrelLowTof'),
                              cms.InputTag('g4SimHits','TrackerHitsPixelBarrelHighTof'),
                              cms.InputTag('g4SimHits','TrackerHitsPixelEndcapLowTof'),
                              cms.InputTag('g4SimHits','TrackerHitsPixelEndcapHighTof') )

    )

mix = cms.EDProducer("MixingModule",
    digitizers = cms.PSet(
      pixel = cms.PSet(
        pixelDigitizer
      ),
      strip = cms.PSet(
        stripDigitizer
      ),
      mergedtruth = cms.PSet(
       trackingParticles
      )
    ),
    LabelPlayback = cms.string(''),
    maxBunch = cms.int32(3),
    minBunch = cms.int32(-5), ## in terms of 25 ns

    bunchspace = cms.int32(25), ## ns
    mixProdStep1 = cms.bool(False),
    mixProdStep2 = cms.bool(False),

    playback = cms.untracked.bool(False),
    useCurrentProcessOnly = cms.bool(False),

    input = cms.SecSource("PoolSource",
        nbPileupEvents = cms.PSet(
            averageNumber = cms.double(1.0)
        ),
        type = cms.string('poisson'),
	sequential = cms.untracked.bool(False),
        fileNames = FileNames
    ),
    mixObjects = cms.PSet(
#        mixCH = cms.PSet(
#            mixCaloHits
#        ),
        mixTracks = cms.PSet(
            mixSimTracks
        ),
        mixVertices = cms.PSet(
            mixSimVertices
        ),
        mixSH = cms.PSet(
            mixSimHits
        ),
        mixHepMC = cms.PSet(
            mixHepMCProducts
        )
    )
)


