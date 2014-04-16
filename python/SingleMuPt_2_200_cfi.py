import FWCore.ParameterSet.Config as cms

generator = cms.EDProducer("FlatRandomPtGunProducer",
    PGunParameters = cms.PSet(
        MaxPt = cms.double(200.0),
        MinPt = cms.double(2.0),
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

    psethack = cms.string('single mu+- pt 2-200'),
    AddAntiParticle = cms.bool(True),
    firstRun = cms.untracked.uint32(1)
)
