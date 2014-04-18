import FWCore.ParameterSet.Config as cms

from SimG4Core.Application.hectorParameter_cfi import *

common_heavy_suppression = cms.PSet(
    NeutronThreshold = cms.double(30.0),
    ProtonThreshold = cms.double(30.0),
    IonThreshold = cms.double(30.0)
)

common_maximum_time = cms.PSet(
    MaxTrackTime  = cms.double(500.0),
    MaxTimeNames  = cms.vstring('ZDCRegion','QuadRegion','InterimRegion'),
    MaxTrackTimes = cms.vdouble(2000.0,0.,0.)
)

common_UsePMT = cms.PSet(
    UseR7600UPMT  = cms.bool(False)
)

g4SimHits = cms.EDProducer("OscarProducer",
    NonBeamEvent = cms.bool(False),
    G4EventManagerVerbosity = cms.untracked.int32(0),
    G4StackManagerVerbosity = cms.untracked.int32(0),
    G4TrackingManagerVerbosity = cms.untracked.int32(0),
    UseMagneticField = cms.bool(True),
    OverrideUserStackingAction = cms.bool(True),
    StoreRndmSeeds = cms.bool(False),
    RestoreRndmSeeds = cms.bool(False),
    PhysicsTablesDirectory = cms.string('PhysicsTables'),
    StorePhysicsTables = cms.bool(False),
    RestorePhysicsTables = cms.bool(False),
    CheckOverlap = cms.untracked.bool(False),
    G4Commands = cms.vstring(),
    FileNameGDML = cms.untracked.string(''),                           
    Watchers = cms.VPSet(),
    theLHCTlinkTag = cms.InputTag("LHCTransport"),
    MagneticField = cms.PSet(
        UseLocalMagFieldManager = cms.bool(False),
        Verbosity = cms.untracked.bool(False),
        ConfGlobalMFM = cms.PSet(
            Volume = cms.string('OCMS'),
            OCMS = cms.PSet(

                Stepper = cms.string('G4ClassicalRK4'),
                Type = cms.string('CMSIMField'),
                G4ClassicalRK4 = cms.PSet(
                    MaximumEpsilonStep = cms.untracked.double(0.01), ## in mm

                    DeltaOneStep = cms.double(0.001), ## in mm

                    MaximumLoopCounts = cms.untracked.double(1000.0),
                    DeltaChord = cms.double(0.001), ## in mm

                    MinStep = cms.double(0.1), ## in mm

                    DeltaIntersectionAndOneStep = cms.untracked.double(-1.0),
                    DeltaIntersection = cms.double(0.0001), ## in mm

                    MinimumEpsilonStep = cms.untracked.double(1e-05) ## in mm

                )
            )
        ),
        delta = cms.double(1.0)
    ),
    Physics = cms.PSet(
        # NOTE : if you want EM Physics only,
        # please select "SimG4Core/Physics/DummyPhysics" for type
        # and turn ON DummyEMPhysics
        #
        type = cms.string('SimG4Core/Physics/QGSP_FTFP_BERT_EML'),
        DummyEMPhysics = cms.bool(False),
        CutsPerRegion = cms.bool(True),
        DefaultCutValue = cms.double(1.0), ## cuts in cm
        G4BremsstrahlungThreshold = cms.double(0.5), ## cut in GeV
        Verbosity = cms.untracked.int32(0),
        # 1 will print cuts as they get set from DD
        # 2 will do as 1 + will dump Geant4 table of cuts
        MonopoleCharge = cms.untracked.int32(1),
        MonopoleDeltaRay = cms.untracked.bool(True),
        MonopoleMultiScatter = cms.untracked.bool(False),
        MonopoleTransport = cms.untracked.bool(True),
        MonopoleMass = cms.untracked.double(0),
        Region = cms.string(' '),
        TrackingCut = cms.bool(True),
        SRType = cms.bool(True),
        EMPhysics = cms.untracked.bool(True),
        HadPhysics = cms.untracked.bool(True),
        FlagBERT = cms.untracked.bool(False),
        FlagCHIPS = cms.untracked.bool(False),
        FlagFTF = cms.untracked.bool(False),
        FlagGlauber = cms.untracked.bool(False),
        FlagHP = cms.untracked.bool(False),
        GflashEcal = cms.bool(False),
        bField = cms.double(3.8),
        energyScaleEB = cms.double(1.032),
        energyScaleEE = cms.double(1.024),
        GflashHcal = cms.bool(False),
        RusRoGammaEnergyLimit = cms.double(0.0),
        RusRoEcalGamma = cms.double(1.0),
        RusRoHcalGamma = cms.double(1.0),
        RusRoQuadGamma = cms.double(1.0),
        RusRoMuonIronGamma = cms.double(1.0),
        RusRoPreShowerGamma = cms.double(1.0),
        RusRoCastorGamma = cms.double(1.0),
        RusRoBeamPipeOutGamma = cms.double(1.0),
        RusRoWorldGamma = cms.double(1.0),
        RusRoElectronEnergyLimit = cms.double(0.0),
        RusRoEcalElectron = cms.double(1.0),
        RusRoHcalElectron = cms.double(1.0),
        RusRoQuadElectron = cms.double(1.0),
        RusRoMuonIronElectron = cms.double(1.0),
        RusRoPreShowerElectron = cms.double(1.0),
        RusRoCastorElectron = cms.double(1.0),
        RusRoBeamPipeOutElectron = cms.double(1.0),
        RusRoWorldElectron = cms.double(1.0)
    ),
    Generator = cms.PSet(
        HectorEtaCut,
        # string HepMCProductLabel = "VtxSmeared"
        HepMCProductLabel = cms.string('generator'),
        ApplyPCuts = cms.bool(True),
        MinPCut = cms.double(0.04), ## the pt-cut is in GeV (CMS conventions)
        MaxPCut = cms.double(99999.0), ## the ptmax=99.TeV in this case
        ApplyEtaCuts = cms.bool(True),
        MinEtaCut = cms.double(-5.5),
        MaxEtaCut = cms.double(5.5),
        ApplyPhiCuts = cms.bool(False),
        MinPhiCut = cms.double(-3.14159265359), ## in radians
        MaxPhiCut = cms.double(3.14159265359), ## according to CMS conventions
        RDecLenCut = cms.double(2.9), ## the minimum decay length in cm (!) for mother tracking
        Verbosity = cms.untracked.int32(0)
    ),
    RunAction = cms.PSet(
        StopFile = cms.string('StopRun')
    ),
    EventAction = cms.PSet(
        debug = cms.untracked.bool(False),
        StopFile = cms.string('StopRun'),
        CollapsePrimaryVertices = cms.bool(False)
    ),
    StackingAction = cms.PSet(
        common_heavy_suppression,
        common_maximum_time,
        KillDeltaRay = cms.bool(False),
        TrackNeutrino = cms.bool(False),
        KillHeavy = cms.bool(False),
        SaveFirstLevelSecondary = cms.untracked.bool(False),
        SavePrimaryDecayProductsAndConversionsInTracker = cms.untracked.bool(True),
        SavePrimaryDecayProductsAndConversionsInCalo = cms.untracked.bool(False),
        SavePrimaryDecayProductsAndConversionsInMuon = cms.untracked.bool(False),
        RusRoNeutronEnergyLimit = cms.double(0.0),
        RusRoEcalNeutron = cms.double(1.0),
        RusRoHcalNeutron = cms.double(1.0),
        RusRoQuadNeutron = cms.double(1.0),
        RusRoMuonIronNeutron = cms.double(1.0),
        RusRoPreShowerNeutron = cms.double(1.0),
        RusRoCastorNeutron = cms.double(1.0),
        RusRoBeamPipeOutNeutron = cms.double(1.0),
        RusRoWorldNeutron = cms.double(1.0),
        RusRoProtonEnergyLimit = cms.double(0.0),
        RusRoEcalProton = cms.double(1.0),
        RusRoHcalProton = cms.double(1.0),
        RusRoQuadProton = cms.double(1.0),
        RusRoMuonIronProton = cms.double(1.0),
        RusRoPreShowerProton = cms.double(1.0),
        RusRoCastorProton = cms.double(1.0),
        RusRoBeamPipeOutProton = cms.double(1.0),
        RusRoWorldProton = cms.double(1.0)
    ),                           
    TrackingAction = cms.PSet(
        DetailedTiming = cms.untracked.bool(False)
    ),
    SteppingAction = cms.PSet(
        common_maximum_time,
        KillBeamPipe            = cms.bool(True),
        CriticalEnergyForVacuum = cms.double(2.0),
        CriticalDensity         = cms.double(1e-15),
        EkinNames               = cms.vstring(),
        EkinThresholds          = cms.vdouble(),
        EkinParticles           = cms.vstring(),
        Verbosity = cms.untracked.int32(0)
    ),
    TrackerSD = cms.PSet(
        ZeroEnergyLoss = cms.bool(False),
        PrintHits = cms.bool(False),
        ElectronicSigmaInNanoSeconds = cms.double(12.06),
        NeverAccumulate = cms.bool(False),
        EnergyThresholdForPersistencyInGeV = cms.double(0.001),
        EnergyThresholdForHistoryInGeV = cms.double(0.001)
    )
)



