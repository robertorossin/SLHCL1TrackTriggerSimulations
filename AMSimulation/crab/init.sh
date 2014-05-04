cp $CMSSW_BASE/bin/$SCRAM_ARCH/amsim amsim.exe

[ -e patternBank.root ] || die 'Failure getting patternBank.root' $?
[ -e input.txt ] || die 'Failure getting input.txt' $?

echo "Using patternBank.root:"
ls -l patternBank.root
echo "Running on input.txt with # files:"
wc -l input.txt
