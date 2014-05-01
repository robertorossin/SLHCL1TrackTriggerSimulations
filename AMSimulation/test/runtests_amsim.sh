#!/bin/bash

# Pass in name and status
function die { echo $1: status $2 ;  exit $2; }

[ -e test_ntuple.root ] || die 'Failure getting ntuple root file' $?
[ -e test_ntuple.txt ] || echo "test_ntuple.root" > test_ntuple.txt
(python ${LOCAL_TEST_DIR}/testGenerateBank.py) || die 'Failure using testGenerateBank.py' $?
(python ${LOCAL_TEST_DIR}/testRecognizePatterns.py) || die 'Failure using testRunMatching.py' $?

