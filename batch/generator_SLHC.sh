#!/bin/bash
#
# This script is the main interface for event production
#
# It is called by launch_SLHC.csh

# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# You're not supposed to touch anything here
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


#
# First we retrieve the input variables
#

EVTS=${1}               # Number of events to produce
PTYPE=${2}              # Production type
TYPE=${3}               # "
GTAG=${4}               # Global tag
NRUN=${5}               # Run number
CMSSW_PROJECT_SRC=${6}  # CMSSW release dir
PACK_DIR=${7}           # Batch package dir 
STOR_DIR=${8}           # Storage dir
PTMIN=${9}              #
PTMAX=${10}             # 
PHIMIN=${11}            # ==> The particle gun parameters
PHIMAX=${12}            #
ETAMIN=${13}            #
ETAMAX=${14}            #
PUDIR=${15}             # Minbias storage repository
NPU=${16}               # PU events number
THRESH=${17}            # pt threshold for stub windows
PU=0 
BANK=0

#
# Setting up environment variables
#

TOP=$PWD

cd $CMSSW_PROJECT_SRC
export SCRAM_ARCH=slc5_amd64_gcc472
eval `scramv1 runtime -sh`   
voms-proxy-info


# We take care of random numbers, so that they are different for all runs

MAXCOUNT=$NRUN
START=$(($NRUN * $EVTS)) # To keep track of the right event number (for parallel production)

echo $START

count=0
tag=${TYPE}_${NRUN}

while [ "$count" -le $MAXCOUNT ]      # Generate 10 ($MAXCOUNT) random integers.
do
  echo $RANDOM
  let "count += 1"  # Increment count.
done

SEED1=$RANDOM
SEED2=$RANDOM
SEED3=$RANDOM
SEED4=$RANDOM
SEED5=$RANDOM
SEED6=$RANDOM
SEED7=$RANDOM
SEED8=$RANDOM

echo $PTYPE

#
# And we tweak the python generation script according to our needs
#

cd $TOP

# Download the main config script
cp $PACK_DIR/test/base/SLHC_PGUN_BASE.py BH_dummy.py 

# Script for Jets production
if [ "$PTYPE" -eq 1 ]; then
    cp $PACK_DIR/test/base/SLHC_JETS_BASE.py BH_dummy.py 
fi

# Script for MinBias production
if [ "$PTYPE" -eq 666 ]; then
    cp $PACK_DIR/test/base/SLHC_MBIAS_BASE.py BH_dummy.py 
fi

# Scripts for Track trigger production

if [ "$PTYPE" -eq 1013 ]; then
    PTYPE=13
    BANK=1
    cp $PACK_DIR/test/base/SLHC_BANKSIM_BASE.py BH_dummy.py 
    cp $PACK_DIR/test/base/SLHC_BANK_BASE.py BH_dummy2.py 
fi

if [ "$PTYPE" -eq -1013 ]; then
    PTYPE=-13
    BANK=1
    cp $PACK_DIR/test/base/SLHC_BANKSIM_BASE.py BH_dummy.py 
    cp $PACK_DIR/test/base/SLHC_BANK_BASE.py BH_dummy2.py 
fi

if [ "$PTYPE" -eq 999 ]; then
    echo "here!!!"
    PTYPE=$NRUN
    cp $PACK_DIR/test/base/SLHC_translate_BASE.py BH_dummy.py 
fi

# Script for Pileup production

# Pile up requires a bit of tweaking
# Indeed we choose two random MinBias files in which the PU events will be chosen
# They are downloaded on the batch machine
#

FILE1=`echo $(( $RANDOM % 200 ))`
FILE2=`echo $(( $RANDOM % 200 ))`

if [ "$PTYPE" -eq 777 ]; then
    PTYPE=-13
    PU=1 
    cp $PACK_DIR/test/base/SLHC_MBIAS_BASE.py BH_dummyMinBias.py 
    cp $PACK_DIR/test/base/SLHC_PU_BASE.py    BH_dummy.py 
    tag=${TYPE}_${NPU}_${NRUN}
fi

if [ "$PTYPE" -eq 888 ]; then
    PU=1 
    cp $PACK_DIR/test/base/SLHC_MBIAS_BASE.py BH_dummyMinBias.py 
    cp $PACK_DIR/test/base/SLHC_PU_4T_BASE.py BH_dummy.py 
    tag=${TYPE}_${NPU}_${NRUN}
fi

# Finally the script is modified according to the requests

sed "s/NEVTS/$EVTS/"                                   -i BH_dummy.py
sed "s/RUN/$START/"                                    -i BH_dummy.py
sed "s/PTYPE/$PTYPE/"                                  -i BH_dummy.py
sed "s#INPUTFILENAME#$TOP/SLHC_extr_PU_${NRUN}.root#"  -i BH_dummy.py
sed "s/NSEEDA/$SEED1/g"                                -i BH_dummy.py
sed "s/NSEEDB/$SEED2/g"                                -i BH_dummy.py
sed "s/NSEEDC/$SEED3/g"                                -i BH_dummy.py
sed "s/NSEEDD/$SEED4/g"                                -i BH_dummy.py
sed "s/MYGLOBALTAG/$GTAG/"                             -i BH_dummy.py
sed "s/PTMIN/$PTMIN/"                                  -i BH_dummy.py
sed "s/PTMAX/$PTMAX/"                                  -i BH_dummy.py
sed "s/PHIMIN/$PHIMIN/"                                -i BH_dummy.py
sed "s/PHIMAX/$PHIMAX/"                                -i BH_dummy.py
sed "s/ETAMIN/$ETAMIN/"                                -i BH_dummy.py
sed "s/ETAMAX/$ETAMAX/"                                -i BH_dummy.py
sed "s/NPU/$NPU/"                                      -i BH_dummy.py
sed "s/THRESHOLD/$THRESH/"                             -i BH_dummy.py
sed "s#PUFILEA#$TOP/MBiasSample.root#"                 -i BH_dummy.py



if [ "$PU" -eq 1 ]; then
    PTYPE=777
    EVTS=200
    sed "s/NEVTS/$EVTS/"                                   -i BH_dummyMinBias.py
    sed "s/PTYPE/$PTYPE/"                                  -i BH_dummyMinBias.py
    sed "s#INPUTFILENAME#$TOP/SLHC_extr_PU_${NRUN}.root#"  -i BH_dummyMinBias.py
    sed "s/NSEEDA/$SEED5/g"                                -i BH_dummyMinBias.py
    sed "s/NSEEDB/$SEED6/g"                                -i BH_dummyMinBias.py
    sed "s/NSEEDC/$SEED7/g"                                -i BH_dummyMinBias.py
    sed "s/NSEEDD/$SEED8/g"                                -i BH_dummyMinBias.py
    sed "s/MYGLOBALTAG/$GTAG/"                             -i BH_dummyMinBias.py
    sed "s/PTMIN/$PTMIN/"                                  -i BH_dummyMinBias.py
    sed "s/PTMAX/$PTMAX/"                                  -i BH_dummyMinBias.py
    sed "s/PHIMIN/$PHIMIN/"                                -i BH_dummyMinBias.py
    sed "s/PHIMAX/$PHIMAX/"                                -i BH_dummyMinBias.py
    sed "s/ETAMIN/$ETAMIN/"                                -i BH_dummyMinBias.py
    sed "s/ETAMAX/$ETAMAX/"                                -i BH_dummyMinBias.py
    sed "s/NPU/$NPU/"                                      -i BH_dummyMinBias.py
    sed "s/THRESHOLD/$THRESH/"                             -i BH_dummyMinBias.py
fi

if [ "$BANK" -eq 1 ]; then
    sed "s/NEVTS/$EVTS/"                                   -i BH_dummy2.py
    sed "s/PTYPE/$PTYPE/"                                  -i BH_dummy2.py
    sed "s#INPUTFILENAME#$TOP/SLHC_extr_PU_${NRUN}.root#"  -i BH_dummy2.py
    sed "s/NSEEDA/$SEED5/g"                                -i BH_dummy2.py
    sed "s/NSEEDB/$SEED6/g"                                -i BH_dummy2.py
    sed "s/NSEEDC/$SEED7/g"                                -i BH_dummy2.py
    sed "s/NSEEDD/$SEED8/g"                                -i BH_dummy2.py
    sed "s/MYGLOBALTAG/$GTAG/"                             -i BH_dummy2.py
    sed "s/PTMIN/$PTMIN/"                                  -i BH_dummy2.py
    sed "s/PTMAX/$PTMAX/"                                  -i BH_dummy2.py
    sed "s/PHIMIN/$PHIMIN/"                                -i BH_dummy2.py
    sed "s/PHIMAX/$PHIMAX/"                                -i BH_dummy2.py
    sed "s/ETAMIN/$ETAMIN/"                                -i BH_dummy2.py
    sed "s/ETAMAX/$ETAMAX/"                                -i BH_dummy2.py
    sed "s/THRESHOLD/$THRESH/"                             -i BH_dummy2.py
    sed "s/NPU/$NPU/"                                      -i BH_dummy2.py
fi

# Set output filenames
#

DATA_NAME=SLHC_extr_$tag.root

# Launch the whole thing
#

if [ "$PTYPE" -eq 777 ]; then
    cmsRun BH_dummyMinBias.py
fi

cmsRun BH_dummy.py

if [ "$BANK" -eq 1 ]; then
    cmsRun BH_dummy2.py
    mv extracted_skimmed.root extracted.root
fi

# Recover the data
#

ls -l
lcg-cp file://$TOP/extracted.root $STOR_DIR/$DATA_NAME
lcg-cp file://$TOP/PGun_example.root $STOR_DIR/EDM_$DATA_NAME
