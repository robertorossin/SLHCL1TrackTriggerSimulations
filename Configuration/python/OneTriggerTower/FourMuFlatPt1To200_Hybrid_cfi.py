import FWCore.ParameterSet.Config as cms

# reference: Configuration/Generator/python/FourMuPt_1_200_cfi.py
generator = cms.EDProducer("FlatRandomPtGunProducer",
    PGunParameters = cms.PSet(
        MaxPt = cms.double(200.0),
        MinPt = cms.double(1.0),
        PartID = cms.vint32(13,-13,13,-13),
        MaxEta = cms.double(1.6),
        MaxPhi = cms.double(1.7),
        MinEta = cms.double(0.5),
        MinPhi = cms.double(0.6),
        #XFlatSpread = cms.double(1.5),  ## in mm
        #YFlatSpread = cms.double(1.5),  ## in mm
        #ZFlatSpread = cms.double(150.), ## in mm
    ),
    Verbosity = cms.untracked.int32(0),
    psethack = cms.string('Four mu pt 1 to 200 tower 35'),
    AddAntiParticle = cms.bool(False),
    firstRun = cms.untracked.uint32(1)
)
