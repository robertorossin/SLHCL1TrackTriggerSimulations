#!/bin/bash
#
# This script is the main interface for bank generation and production
#
# It is called by PG.sh

# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# You're not supposed to touch anything here
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


#
# Case 1: Pattern recognition
#

if [ ${1} = "PR" ]; then

    INPUT=${2} 
    BK=${3}  
    OUTPUT=${4}  
    OUTPUTFULL=${7}/${4}  
    START=${5}  
    STOP=${6}  
    CMSSW_PROJECT_SRC=${8}

    #
    # Setting up environment variables
    #   

    cd $CMSSW_PROJECT_SRC
    export SCRAM_ARCH=slc5_amd64_gcc472
    eval `scramv1 runtime -sh`   
    voms-proxy-info

    cd /tmp/$USER
    TOP=$PWD

    #
    # And we tweak the python generation script according to our needs
    #  

    cd $TOP

    $CMSSW_PROJECT_SRC/src/trunk/AMSimulation --findPatterns --inputFile $INPUT --bankFile $BK --outputFile $OUTPUT --ss_threshold 5 --startEvent $START --stopEvent $STOP

    # Recover the data
    #  

    ls -l
    lcg-cp file://$TOP/$OUTPUT $OUTPUTFULL

fi



#
# Case 2: Pattern bank generation
#

if [ ${1} = "BG" ]; then

    #echo "source $PACKDIR/batch/PG.sh BG $SS_SIZE $DC $INPUT $OUTPUT 0.${COV} $SECTOR ${i} $LAY $RELEASEDIR" >> bg_job_${BKNAME}.sh

    SS=${2}
    DC=${3}
    INPUT=${4} 
    OUTPUT=${5}  
    COV=${6}
    SECNAME=${7}
    SECID=${8}  
    LAY=${9}
    CMSSW_PROJECT_SRC=${10}

    #
    # Setting up environment variables
    #   

    cd $CMSSW_PROJECT_SRC
    export SCRAM_ARCH=slc5_amd64_gcc472
    eval `scramv1 runtime -sh`   
    voms-proxy-info

    cd /tmp/$USER
    TOP=$PWD

    #
    # And we tweak the python generation script according to our needs
    #  

    cd $TOP
    cp $CMSSW_PROJECT_SRC/src/trunk/amsimulation_BASE.cfg amsimulation.cfg

    # The AM base script is modified
    sed "s/SSSIZE/$SS/"                    -i amsimulation.cfg
    sed "s/NDC/$DC/"                       -i amsimulation.cfg
    sed "s#INPUTFILES#$INPUT#"             -i amsimulation.cfg
    sed "s#OUTPUTFILE#$OUTPUT#"            -i amsimulation.cfg
    sed "s/COVVALUE/$COV/"                 -i amsimulation.cfg
    sed "s#SECTORFILE#$SECNAME#"           -i amsimulation.cfg
    sed "s/SECID/$SECID/"                  -i amsimulation.cfg
    sed "s/ALAY/$LAY/"                     -i amsimulation.cfg

    $CMSSW_PROJECT_SRC/src/trunk/AMSimulation --generateBank 

fi



#
# Case 3: CMSSW pattern recognition
#


if [ ${1} = "FASTPR" ]; then

    # echo "source $PACKDIR/batch/PG.sh FASTPR ${INDIR_XROOT}/$l $BANKDIR2/$k $OUT.root  ${i} $NPFILE $OUTDIR_GRID $RELEASEDIR" >> fpr_job_${OUT}.sh

    INPUT=${2} 
    BK=${3}  
    OUTPUT=${4}  
    OUTPUTFULL=${7}/${4}  
    START=${5}  
    STOP=${6}  
    CMSSW_PROJECT_SRC=${8}
    USEID=${9}

    #
    # Setting up environment variables
    #   

    cd $CMSSW_PROJECT_SRC
    export SCRAM_ARCH=slc5_amd64_gcc472
    eval `scramv1 runtime -sh`   
    voms-proxy-info

    cd /tmp/$USER
    TOP=$PWD

    #
    # And we tweak the python generation script according to our needs
    #  

    cd $TOP
    cp $CMSSW_PROJECT_SRC/src/DataProduction/test/base/SLHC_PR_BASE.py BH_dummy.py 

    # Finally the script is modified according to the requests
    
    sed "s/NEVTS/$STOP/"                                   -i BH_dummy.py
    sed "s/NSKIP/$START/"                                  -i BH_dummy.py
    sed "s/USEID/$USEID/"                                  -i BH_dummy.py
    sed "s#INPUTFILENAME#$INPUT#"                          -i BH_dummy.py
    sed "s#OUTPUTFILENAME#$OUTPUT#"                        -i BH_dummy.py
    sed "s#BANKFILENAME#$BK#"                              -i BH_dummy.py

    cmsRun BH_dummy.py -j4

    # Recover the data
    #  

    ls -l
    lcg-cp file://$TOP/$OUTPUT $OUTPUTFULL

fi
