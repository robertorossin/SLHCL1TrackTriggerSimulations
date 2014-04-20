import FWCore.ParameterSet.Config as cms

# Reduce particle gun sample size
def customise_pgun_RAWSIMoutput(process):
    process.RAWSIMoutput.outputCommands += [
        'drop *_MEtoEDMConverter_*_*', 
        'drop *_randomEngineStateProducer_*_*', 
        'drop *_logErrorHarvester_*_*',
        'drop *_simEcalDigis_*_*',
        'drop *_simHcalUnsuppressedDigis_*_*',
        'drop recoGenJets_*_*_*',
        'keep recoGenJets_ak5GenJets_*_*',
        'drop recoGenMETs_*_*_*',
        'keep recoGenMETs_genMetTrue_*_*'
    ]
    return (process)

