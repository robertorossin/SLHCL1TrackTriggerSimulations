#!/bin/bash

# Pass in name and status
function die { echo $1: status $2 ;  exit $2; }

[ -e test_ntuple.root ] || die 'Failure getting test_ntuple.root' $?
PYTHONTEST=${CMSSW_BASE}/src/SLHCL1TrackTriggerSimulations/AMSimulation/python/test

#(amsim --help) || die 'Failure getting help message' $?

(amsim -C -i test_ntuple.root -o stubs.root -n 100 --timing) || die 'Failure during stub cleaning' $?
(python ${PYTHONTEST}/testStubCleaning.py ${LOCAL_TOP_DIR}/stubs.root) || die 'Failure using tesStubCleaning.py' $?

(amsim -B -i stubs.root -o bank.root -n 100 --timing) || die 'Failure during pattern bank generation' $?
#WONTFIX# (python ${PYTHONTEST}/testBankGeneration.py ${LOCAL_TOP_DIR}/bank.root) || die 'Failure using testBankGeneration.py' $?

(amsim -R -i test_ntuple.root -o roads.root -b bank.root -n 100 --timing) || die 'Failure during pattern recognition' $?
#WONTFIX# (python ${PYTHONTEST}/testPatternRecognition.py ${LOCAL_TOP_DIR}/roads.root) || die 'Failure using testPatternRecognition.py' $?

(amsim -M -i stubs.root -o matrices.txt -n 100 --timing) || die 'Failure during matrix building' $?
#WONTFIX# (python ${PYTHONTEST}/testMatrixBuilding.py ${LOCAL_TOP_DIR}/matrices.txt) || die 'Failure using testMatrixBuilding.py' $?

(amsim -T -i roads.root -o tracks.root -m matrices.txt -n 100 --timing) || die 'Failure during track fitting' $?
#WONTFIX# (python ${PYTHONTEST}/testTrackFitting.py ${LOCAL_TOP_DIR}/tracks.root) || die 'Failure using testTrackFitting.py' $?

(amsim -A -i stubs.root -o attribs.root -b bank.root -n 100 --timing) || die 'Failure during pattern bank analysis' $?
#WONTFIX# (python ${PYTHONTEST}/testBankAnalysis.py ${LOCAL_TOP_DIR}/attribs.root) || die 'Failure using testBankAnalysis.py' $?

(amsim -U -i stubs.root -o tracks_test.root -m matrices.txt -n 100 --timing) || die 'Failure during matrix testing' $?
#WONTFIX# (python ${PYTHONTEST}/testMatrixTesting.py ${LOCAL_TOP_DIR}/tracks_test.root) || die 'Failure using testMatrixTesting.py' $?

(amsim -W -i test_ntuple.root -o results.root --roads roads.root --tracks tracks.root -n 100 --timing) || die 'Failure during ntuple writing' $?
#WONTFIX# (python ${PYTHONTEST}/testWriting.py ${LOCAL_TOP_DIR}/results.root) || die 'Failure using testWriting.py' $?
