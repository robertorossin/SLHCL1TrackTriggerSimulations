import FWCore.ParameterSet.Config as cms

generator = cms.EDProducer("FlatRandomOneOverPtGunProducer",
    PGunParameters = cms.PSet(
        MaxOneOverPt = cms.double(0.5),
        MinOneOverPt = cms.double(0.005),
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

    psethack = cms.string('single mu+- 1/pt 0.005-0.5'),
    AddAntiParticle = cms.bool(True),
    firstRun = cms.untracked.uint32(1)
)
