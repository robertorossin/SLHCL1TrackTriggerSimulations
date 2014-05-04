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


# Take line i from input.txt
sed -i "$i"'q;d' input.txt

echo "Running on input.txt:"
cat input.txt
echo "  max events: $n"

./amsim.exe -R -i input.txt -o results.root -B patternBank.root -n "$n" --no-color >& amsim.log

