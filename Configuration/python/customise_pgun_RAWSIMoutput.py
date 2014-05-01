import FWCore.ParameterSet.Config as cms

# Reduce particle gun sample size
def customise_pgun_RAWSIMoutput(process):
    process.load('SLHCL1TrackTriggerSimulations.NTupleTools.randomEngineSeedKeeper_cfi')
    process.generation_step += process.randomEngineSeedKeeper

    process.RAWSIMoutput.outputCommands += [
        'drop *_MEtoEDMConverter_*_*', 
        'drop *_randomEngineStateProducer_*_*', 
        'drop *_logErrorHarvester_*_*',
        'drop *_simEcalDigis_*_*',
        'drop *_simHcalUnsuppressedDigis_*_*',
        'drop recoGenJets_*_*_*',
        'keep recoGenJets_ak5GenJets_*_*',
        'drop recoGenMETs_*_*_*',
        'keep recoGenMETs_genMetTrue_*_*',
        'keep *_randomEngineSeedKeeper_*_*',
    ]
    return (process)

