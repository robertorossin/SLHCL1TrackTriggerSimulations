import FWCore.ParameterSet.Config as cms

pileup_input = [
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_100.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_101.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_102.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_103.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_104.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_105.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_106.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_107.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_108.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_109.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_110.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_111.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_112.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_113.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_114.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_115.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_116.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_117.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_118.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_119.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_120.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_121.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_122.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_123.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_124.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_125.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_126.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_127.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_128.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_129.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_140.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_141.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_142.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_143.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_144.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_145.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_146.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_147.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_148.root",
"/store/user/jiafu/L1TrackTrigger/from_TTI/620_SLHC10/Extended2023TTI/MinBias/MinBias_TuneZ2star_GEN_SIM_149.root",
]

def customise_pileup_input(process,num=100):
    process.mix.input.fileNames = cms.untracked.vstring()
    process.mix.input.fileNames += pileup_input[0:num]
    return (process)

