#!/bin/sh

########################################
# Run amsim with CRAB
#
# copied from HiggsAnalysis/CombinedLimit/test/combine_crab.sh
#
########################################

i="$1"
if [ "$i" == "help" ]; then
  echo "usage: amsim_crab.sh <job index> [<max events>]"
  exit 0;
fi
if [ "$i" = "" ]; then
  echo "Error: missing job index"
  exit 1;
fi

n="$MaxEvents" 
if [ "$n" = "" ]; then
  n="$2"
fi
if [ "$n" = "" ]; then
  echo "Error: missing number of events"
  exit 2;
fi

# Rename
if ls input_*.txt > /dev/null 2>&1; then
    mv input_*.txt input.txt
fi
if ls patternBank_*.root > /dev/null 2>&1; then
    mv patternBank_*.root patternBank.root
fi

# Take line i from input.txt
sed -i "$i"'q;d' input.txt

echo "Running on input.txt:"
cat input.txt
echo "  max events: $n"

./amsim.exe -R -i input.txt -o roads.root -B patternBank.root --bank_nDCBits 0 -n "$n" --no-color >& amsimR.log
./amsim.exe -F -i roads.root -o tracks.root -n "$n" --no-color >& amsimF.log
./amsim.exe -W -i input.txt -o results.root --roads roads.root --tracks tracks.root -n "$n" --no-color >& amsimW.log
cat amsimR.log amsimF.log amsimW.log

