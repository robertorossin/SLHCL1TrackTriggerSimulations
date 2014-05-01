#!/bin/bash

# Pass in name and status
function die { echo $1: status $2 ;  exit $2; }

[ -e test_ntuple.root ] || die 'Failure getting ntuple root file' $?
[ -e test_ntuple.txt ] || echo "test_ntuple.root" > test_ntuple.txt
PYTHONTEST=${CMSSW_BASE}/src/SLHCL1TrackTriggerSimulations/AMSimulation/python/test

(amsim --help) || die 'Failure getting help message' $?
(amsim -G -i test_ntuple.txt -o patternBank.root -L src/SLHCL1TrackTriggerSimulations/AMSimulation/data/trigger_sector_map2.csv -n 100 --no-filter) || die 'Failure during pattern bank generation' $?
(python ${PYTHONTEST}/testGenerateBank.py ${LOCAL_TOP_DIR}/patternBank.root) || die 'Failure using testGenerateBank.py' $?

(amsim -R -i test_ntuple.txt -o results.root -B patternBank.root -n 100) || die 'Failure during pattern recognition' $?
(python ${PYTHONTEST}/testPatternRecognition.py ${LOCAL_TOP_DIR}/results.root) || die 'Failure using testPatternRecognition.py' $?
