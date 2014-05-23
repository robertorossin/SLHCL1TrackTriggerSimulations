import FWCore.ParameterSet.Config as cms

# reference: https://github.com/cms-sw/genproductions/blob/master/python/EightTeV/SingleMuMinusFlatPt0p2To100_cff.py
generator = cms.EDProducer("FlatRandomPtGunProducer",
    PGunParameters = cms.PSet(
        MaxPt = cms.double(100.0),
        MinPt = cms.double(1.0),
        PartID = cms.vint32(-13),
        MaxEta = cms.double(2.5),
        MaxPhi = cms.double(3.14159265359),
        MinEta = cms.double(-2.5),
        MinPhi = cms.double(-3.14159265359), ## in radians
        XFlatSpread = cms.double(1.5), ## in mm
        YFlatSpread = cms.double(1.5), ## in mm
        ZFlatSpread = cms.double(150.), ## in mm
    ),
    Verbosity = cms.untracked.int32(0), ## set to 1 (or greater)  for printouts
    psethack = cms.string('single mu+ pt 1 to 100'),
    AddAntiParticle = cms.bool(False),
    firstRun = cms.untracked.uint32(1)
)
