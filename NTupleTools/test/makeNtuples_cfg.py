import FWCore.ParameterSet.Config as cms

process = cms.Process("EXTR")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
"/store/user/l1upgrades/SLHC/GEN/620_SLHC10/SingleMuPlusMinus_20140417/SingleMuPlusMinus_E2023TTI_21_1_JHj.root"
    )
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string(
        "tree.root"
    )
)

process.load("Extractors.NTupleTools.ntuple_cff")

process.p = cms.Path(process.ntuple)

