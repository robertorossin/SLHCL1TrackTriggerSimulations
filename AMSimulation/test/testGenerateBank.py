import subprocess

subprocess.call("amsim -G -i test_ntuple.txt -o patternBank.root -L src/SLHCL1TrackTriggerSimulations/AMSimulation/data/trigger_sector_map2.csv -n 100", shell=True)
#subprocess.call("amsim -G -i test_ntuple.txt -o patternBank.root -L src/SLHCL1TrackTriggerSimulations/AMSimulation/data/trigger_sector_map2.csv -n 5 -v 3 -f 0 -t 1", shell=True)
