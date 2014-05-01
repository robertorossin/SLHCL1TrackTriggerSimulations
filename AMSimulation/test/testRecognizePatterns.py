import subprocess

subprocess.call("amsim -R -i test_ntuple.txt -o results.root -B patternBank.root -n 100", shell=True)
#subprocess.call("amsim -R -i test_ntuple.txt -o results.root -B patternBank.root -n 5 -v 3 -t 1", shell=True)
