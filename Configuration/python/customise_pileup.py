import FWCore.ParameterSet.Config as cms

pileup_input = [
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00001/E0C50F1A-C4D1-E311-B819-0026B95B8CC9.root",
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00001/94EE28AF-C0D1-E311-9056-842B2B2922E2.root",
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00001/143D4B0B-C4D1-E311-A5BC-AC853D9DACF7.root",
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00001/E21827A9-C3D1-E311-928B-AC853D9F5256.root",
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00001/08B35081-C0D1-E311-B1EA-AC853D9DACD7.root",
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00002/8EB77EAA-EDD1-E311-86C9-003048344C20.root",
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00002/448CC32B-EDD1-E311-A563-003048F5B69E.root",
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00002/62097CFB-ECD1-E311-9171-00238BCE45F2.root",
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00002/94E110AB-E9D1-E311-A374-00238BBD7674.root",
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00002/724C7F76-E9D1-E311-8378-0025907254BC.root",
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00001/0EAF8EDC-CFD1-E311-A4B2-0019B9F72F97.root",
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00001/9EAC7429-CED1-E311-919F-02163E00E9E5.root",
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00001/3EC126FD-CAD1-E311-A5CD-02163E00E9A3.root",
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00001/4030FFAA-C9D1-E311-ABD4-001D09F242EF.root",
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00001/64940974-C8D1-E311-ADD0-001D09F24399.root",
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00000/60864153-B4D1-E311-8798-002590E3A0D4.root",
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00000/3433B82E-B4D1-E311-BA70-003048F5ADEC.root",
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00000/748326F9-B3D1-E311-9499-0025908653C4.root",
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00000/CA404658-B3D1-E311-AAC2-002590E3A0D4.root",
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00000/56F0E22F-B3D1-E311-99C3-003048F5ADEC.root",
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00001/0A98176C-C3D1-E311-A211-00238BBD7678.root",
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00001/7470E263-C3D1-E311-BB1A-003048F5B2A6.root",
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00001/9602542F-C3D1-E311-AB5D-003048F7CC8A.root",
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00001/4269D771-C2D1-E311-BD0C-003048F7CC92.root",
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00001/32DEE13E-C2D1-E311-8E00-003048F7CC8A.root",
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00001/7CF29C46-9DD0-E311-BBE6-003048FEC15C.root",
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00001/B8A4543A-9AD0-E311-9676-02163E00E9C7.root",
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00001/82F208AE-9BD0-E311-822F-02163E00EB8E.root",
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00001/FC755EB8-98D0-E311-A1EF-02163E00EB8E.root",
"/store/mc/TTI2023Upg14/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v1/00001/0672938B-99D0-E311-AC65-003048FEFFF8.root",
]

def cust_pileup_input(process, sequential=False):
    # Modify pileup input source
    process.mix.input.fileNames = pileup_input
    process.mix.input.sequential = sequential
    return (process)


# Run in tracker only, ignoring calorimeter and muon system
# In CMSSW_6_2_0_SLHC12_patch1, processing speed is reduced to ~0.06 s/evt
def cust_useTrackerOnly(process, sequential=False, intime=False, ntuple=True):

    # __________________________________________________________________________
    # Customise generation step

    # Remove jets & MET
    process.pgen = process.pgen.copyAndExclude([process.genJetMET])

    # Keep track of random number seeds
    process.load('SLHCL1TrackTriggerSimulations.NTupleTools.randomEngineSeedKeeper_cfi')
    process.pgen += process.randomEngineSeedKeeper

    # __________________________________________________________________________
    # Customise simulation step

    # Remove sensitive detectors
    for removee in ['MuonSD', 'CaloSD', 'CaloResponse', 'ECalSD', 'HCalSD', 'CaloTrkProcessing', 'HFShower', 'HFShowerLibrary', 'HFShowerPMT', 'HFShowerStraightBundle', 'HFShowerConicalBundle', 'HFGflash', 'CastorSD', 'CastorShowerLibrary', 'TotemSD', 'ZdcSD', 'ZdcShowerLibrary', 'FP420SD', 'BscSD', 'BHMSD', 'CFCSD', 'HGCSD', 'ShashlikSD', 'PltSD', 'HcalTB02SD', 'EcalTBH4BeamSD', 'HcalTB06BeamSD']:
        if hasattr(process.g4SimHits, removee):
            delattr(process.g4SimHits, removee)

    # Kill delta rays
    #process.g4SimHits.StackingAction.KillDeltaRay = True

    # Modify geometry
    geoms_orig = process.XMLIdealGeometryESSource.geomXMLFiles
    geoms_modified = []
    for geom in geoms_orig:
        keep = True
        for removee in ['EcalCommonData', 'HcalCommonData', 'MuonCommonData', 'ForwardCommonData', 'EcalSimData', 'HcalSimData', 'MuonSimData', 'ForwardSimData', 'DTGeometryBuilder', 'CSCGeometryBuilder', 'RPCGeometryBuilder', 'GEMGeometryBuilder']:
            if geom.startswith('Geometry/%s/' % removee):
                keep = False
                break
        for removee in ['caloBase.xml', 'cmsCalo.xml', 'muonBase.xml', 'cmsMuon.xml', 'mgnt.xml', 'muonMB.xml', 'muonMagnet.xml', 'cavern.xml']:
            if geom.startswith('Geometry/CMSCommonData/data/%s' % removee):
                keep = False
                break
        if keep:
            geoms_modified.append(geom)
    process.XMLIdealGeometryESSource.geomXMLFiles = geoms_modified

    # __________________________________________________________________________
    # Customise digitisation step

    # Modify pileup input source
    process.mix.input.fileNames = pileup_input
    process.mix.input.sequential = sequential

    # Drop calorimeter and muon system
    process.pdigi_valid = process.pdigi_valid.copyAndExclude([process.doAllDigi])

    # No OOT mixing
    if intime:
        process.mix.minBunch = 0
        process.mix.maxBunch = 0

    # Drop sim hits in muon system
    process.mix.digitizers.mergedtruth.simHitCollections.muon = cms.VInputTag()

    # Reduce number of tracking particles
    process.mix.digitizers.mergedtruth.alwaysAddAncestors = cms.bool(False)
    process.mix.digitizers.mergedtruth.ignoreTracksOutsideVolume = cms.bool(True)
    process.mix.digitizers.mergedtruth.select.ptMinTP = cms.double(0.2)
    #process.mix.digitizers.mergedtruth.select.minHitTP = cms.int32(0)
    process.mix.digitizers.mergedtruth.select.tipTP = process.mix.digitizers.mergedtruth.volumeRadius
    process.mix.digitizers.mergedtruth.select.lipTP = process.mix.digitizers.mergedtruth.volumeZ

    # Drop digitizers
    for removee in ['ecal', 'hcal', 'castor']:
        if hasattr(process.mix.digitizers, removee):
            delattr(process.mix.digitizers, removee)

    # Drop crossing frames in muon system
    for removee in ['MuonCSCHits', 'MuonDTHits', 'MuonRPCHits']:
        if removee in process.mix.mixObjects.mixSH.crossingFrames:
            process.mix.mixObjects.mixSH.crossingFrames.remove(removee)

    # Drop sub detectors
    for removee in ['BSCHits', 'FP420SI', 'MuonCSCHits', 'MuonDTHits', 'MuonRPCHits', 'TotemHitsRP', 'TotemHitsT1', 'TotemHitsT2Gem', 'TrackerHitsTECHighTof', 'TrackerHitsTECLowTof', 'TrackerHitsTIBHighTof', 'TrackerHitsTIBLowTof', 'TrackerHitsTIDHighTof', 'TrackerHitsTIDLowTof', 'TrackerHitsTOBHighTof', 'TrackerHitsTOBLowTof']:
        if removee in process.mix.mixObjects.mixSH.subdets:
            process.mix.mixObjects.mixSH.subdets.remove(removee)
        removeee = cms.InputTag("g4SimHits", removee)
        if removeee in process.mix.mixObjects.mixSH.input:
            process.mix.mixObjects.mixSH.input.remove(removeee)

    # Drop calo hits
    for removee in ['mixCH']:
        if hasattr(process.mix.mixObjects, removee):
            delattr(process.mix.mixObjects, removee)

    # Drop EDAliases
    for removee in ['simCastorDigis', 'simEcalUnsuppressedDigis', 'simHcalUnsuppressedDigis']:
        if hasattr(process, removee):
            delattr(process, removee)

    # __________________________________________________________________________
    # Customise L1TrackTrigger step

    # Drop tracklet method
    process.L1TrackTrigger = process.L1TrackTrigger.copyAndExclude([process.TrackTriggerTTTracks, process.TrackTriggerAssociatorTracks])

    # __________________________________________________________________________
    # Customise output steps

    if ntuple:
        # Drop RAWSIM output
        for removee in [process.genfiltersummary_step, process.endjob_step, process.RAWSIMoutput_step]:
            if removee in process.schedule:
                process.schedule.remove(removee)

        # Substitute with TFileService
        outputFileName = process.RAWSIMoutput.fileName._value
        outputFileName = outputFileName.replace(".root", "_ntuple.root")
        process.TFileService = cms.Service("TFileService",
            fileName = cms.string(outputFileName)
        )

        # Straight to ntuples
        process.load("SLHCL1TrackTriggerSimulations.NTupleTools.sequences_cff")
        process.p = cms.Path(process.ntupleSequence_TTI)
        process.schedule.append(process.p)

    return (process)
