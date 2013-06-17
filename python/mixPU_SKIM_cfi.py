import FWCore.ParameterSet.Config as cms

# this is the configuration to model pileup for final scheme
from DataProduction.SkimGeometry.aliases_SKIM_cfi import * 
from SimGeneral.MixingModule.mixObjects_cfi import *
from SimGeneral.MixingModule.mixPoolSource_cfi import *
from SimGeneral.MixingModule.pixelDigitizer_cfi import *
from SimGeneral.MixingModule.stripDigitizer_cfi import *

mix = cms.EDProducer("MixingModule",
    digitizers = cms.PSet(
      pixel = cms.PSet(
        pixelDigitizer
      ),
      strip = cms.PSet(
        stripDigitizer
      ),
    ),
    LabelPlayback = cms.string(''),
    maxBunch = cms.int32(3),
    minBunch = cms.int32(-12), ## in terms of 25 ns

    bunchspace = cms.int32(25), ## ns
    mixProdStep1 = cms.bool(False),
    mixProdStep2 = cms.bool(False),

    playback = cms.untracked.bool(False),
    useCurrentProcessOnly = cms.bool(False),

    input = cms.SecSource("PoolSource",
        nbPileupEvents = cms.PSet(
            averageNumber = cms.double(1.0)
        ),
        type = cms.string('fixed'),
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


