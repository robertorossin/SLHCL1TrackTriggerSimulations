#!/bin/csh


###########################################
#
# Main script for standalone pattern bank generation 
# and recognition jobs
#
# The jobs themselves are launched by PG.sh
#
# --> Usage for bank generation:
#
# This script uses the bank files stored on the following SE:
#
# srm://lyogrid06.in2p3.fr//dpm/in2p3.fr/home/cms/data/store/user/sviret/SLHC/GEN/ 
#
# If you are registered on the CMS VO you have access to it
#
# source launch_PG.sh BG p1 p2 p3 p4
# with:
# p1 : The SE subdirectory containing the bank root files
# p2 : The number of the sector you want to build the bank on
# p3 : The directory where you will store the bank files
# p4 : BATCH or nothing: launch lxbatch jobs or not
#
# The rest of the options are set on the lines below
# Please be aware that BG jobs use a great amount of RAM, therefore they might crash 
# on batch machine
#
# For more details, and examples, have a look at:
# 
# http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.HLLHCTuto (STEP IV)
# 
#
# --> Usage for standalone pattern recognition:
#
#
# source launch_PG.sh PR p1 p2 p3 p4 p5 p6
# with:
# p1 : The SE subdirectory containing the data file you want to analyze
# p2 : The number of the sector you want to test
# p3 : The directory where you will retrieve the bank files
# p4 : How many events you want to test in total? 
# p5 : How many events per job (should be below p3...)?
# p6 : BATCH or nothing: launch lxbatch jobs or not 
#
# For more details, and examples, have a look at:
# 
# http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.HLLHCTuto (STEP V.1)
#
#
# --> Usage for CMSSW pattern recognition:
#
#
# source launch_PG.sh FASTPR p1 p2 p3 p4 p5 p6
# with:
# p1 : The SE subdirectory containing the data file you want to analyze
# p2 : Not used here as you will use all available bank files (put -1)
# p3 : The directory where you will retrieve the bank files
# p4 : How many events per data file? 
# p5 : How many events per job (should be below p3...)?
# p6 : BATCH or nothing: launch lxbatch jobs or not 
#
# For more details, and examples, have a look at:
# 
# http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.HLLHCTuto (STEP V.2)
#
# Author: S.Viret (viret@in2p3.fr)
# Date  : 24/09/2013
#
# Script tested with release CMSSW_6_1_2_SLHC6_patch1
#
###########################################


# Here we retrieve the main parameters for the job 

set STEP    = ${1}   # PR or BG
set MATTER  = ${2}   # Directory where the input root files are
set SECID   = ${3}   # The sector number  
set BANKDIR = ${4}   # Directory where the bank (.pbk) files are
set NTOT    = ${5}   # How many events per data file (PR step only)?
set NPFILE  = ${6}   # How many events per job (PR step only)?

###################################
#
# The list of parameters you can modify is here
#
###################################

# You have to adapt this to your situation

# Useful for all steps

# The SE directory containing the input data (bank/samples/...)
set INDIR       = /dpm/in2p3.fr/home/cms/data/store/user/sviret/SLHC/GEN/$MATTER 

# The SE directory containing the PR output data
set OUTDIR      = /dpm/in2p3.fr/home/cms/data/store/user/sviret/SLHC/PR/$MATTER

setenv LFC_HOST 'lyogrid06.in2p3.fr'
set INDIR_GRID  = srm://$LFC_HOST/$INDIR
set INDIR_XROOT = root://$LFC_HOST/$INDIR
set OUTDIR_GRID = srm://$LFC_HOST/$OUTDIR
                  
# Options for bank generation only 

set SS_SIZE = 128                               # The superstrip size (in strip units)
set DC      = 2                                 # The number of DC bits
set COV     = 20                                # The coverage (in %)
set SECTOR  = $BANKDIR/Eta7_Phi8_sector_map.csv # Where to find the sector definition
set LAY     = '5 6 7 8 9 10'                    # The layer to include in the bank generation

###########################################################
###########################################################
# You are not supposed to touch the rest of the script !!!!
###########################################################
###########################################################


# Following lines suppose that you have a certificate installed on lxplus. To do that follow the instructions given here:
#
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideLcgAccess
#

source /afs/cern.ch/cms/LCG/LCG-2/UI/cms_ui_env.csh
voms-proxy-init --voms cms --valid 100:00 -out $HOME/.globus/gridproxy.cert
setenv X509_USER_PROXY ${HOME}'/.globus/gridproxy.cert'


# Then we setup some environment variables

cd  ..
set PACKDIR      = $PWD           # This is where the package is installed 
cd  ../..
set RELEASEDIR   = $PWD           # This is where the release is installed

cd $PACKDIR/batch

# First, bank generation 

if ($STEP == "BG") then  
 
    # The first thing we do is to create a list of the root files contained in
    # the bank directory

    set INPUTDIR2  = $INDIR_GRID/
    set INPUT      = $BANKDIR/${MATTER}_bank_files.txt

    rm $BANKDIR/${MATTER}_bank_files.txt

    touch $BANKDIR/${MATTER}_bank_files.txt

    foreach l (`lcg-ls $INPUTDIR2 | cut -d/ -f15`) 

	echo 'Adding '$l' to '$INPUT  
	echo $INDIR_XROOT'/'$l >> $INPUT

    end

    rm *.log

    set BKNAME = ${MATTER}_sec${SECID}_ss${SS_SIZE}_cov${COV} # Bank file name 
    set OUTPUT = $BANKDIR/${BKNAME}.pbk # Bank file full name

    echo "#\!/bin/bash" > bg_job_${BKNAME}.sh
    echo "source $PACKDIR/batch/PG.sh BG $SS_SIZE $DC $INPUT $OUTPUT 0.${COV} $SECTOR ${i} "\'"${LAY}"\'" $RELEASEDIR" >> bg_job_${BKNAME}.sh
    chmod 755 bg_job_${BKNAME}.sh

    if (${5} == "BATCH") then	
	bsub -q 1nd -e /dev/null -o /tmp/${LOGNAME}_out.txt bg_job_${BKNAME}.sh
    endif
      
endif  

# Then, pattern recognition 

if ($STEP == "PR") then  

    echo 'The data will be read from directory: '$INDIR_XROOT
    echo 'The pattern reco output files will be written in: '$OUTDIR

    lfc-mkdir $OUTDIR

    # We loop over all the banks for the corresponding sector

    foreach k (`ls $BANKDIR | grep _sec${SECID}_`) 

	echo 'Working with bank file '$k

	@ i = 0
	@ j = $NPFILE

	# We loop over the data directory in order to find all the files to analyse
	
	foreach l (`lcg-ls $INDIR_GRID/ | cut -d/ -f15`) 

	    echo 'Working with file '$l

	    while ($i < $NTOT)

		set OUT = `echo $l | cut -d. -f1`_`echo $k | cut -d. -f1`_${i}_${j}

		echo "#\!/bin/bash" > pr_job_${OUT}.sh
		echo "source $PACKDIR/batch/PG.sh PR ${INDIR_XROOT}/$l $BANKDIR/$k $OUT.root  ${i} ${j} $OUTDIR_GRID $RELEASEDIR" >> pr_job_${OUT}.sh
		chmod 755 pr_job_${OUT}.sh

		if (${7} == "BATCH") then	
		    bsub -q 1nd -e /dev/null -o /tmp/${LOGNAME}_out.txt pr_job_${OUT}.sh
		endif

		@ i += $NPFILE
		@ j += $NPFILE

	    end 
	end
    end
endif  


# Finally, CMSSW pattern recognition 

if ($STEP == "FASTPR") then  

    echo 'The data will be read from directory: '$INDIR_XROOT
    echo 'The pattern reco output files will be written in: '$OUTDIR

    lfc-mkdir $OUTDIR

    # By default, for CMSSW, we loop over all available bank (considering that we want
    # to work over all the detector)

    foreach k (`ls $BANKDIR | grep _sec`) 

	echo 'Working with bank file '$k

	# We loop over the data directory in order to find all the files to analyse
	
	foreach l (`lcg-ls $INDIR_GRID/ | cut -d/ -f15`) 
	
	    @ i = 0
	    @ j = $NPFILE

	    echo 'Working with file '$l

	    while ($i < $NTOT)

		set OUT = `echo $l | cut -d. -f1`_`echo $k | cut -d. -f1`_${i}_${j}


		# Check if the file has already been processed
		set deal = `lcg-ls $OUTDIR_GRID/$OUT.root | wc -l`

		if ($deal == "0") then

		    echo "#\!/bin/bash" > fpr_job_${OUT}.sh
		    echo "source $PACKDIR/batch/PG.sh FASTPR ${INDIR_XROOT}/$l $BANKDIR/$k $OUT.root  ${i} $NPFILE $OUTDIR_GRID $RELEASEDIR 1" >> fpr_job_${OUT}.sh
		    chmod 755 fpr_job_${OUT}.sh

		    if (${7} == "BATCH") then	
			bsub -q 8nh -e /dev/null -o /tmp/${LOGNAME}_out.txt fpr_job_${OUT}.sh			
		    endif
		endif

		@ i += $NPFILE
		@ j += $NPFILE

	    end 
	end
    end
endif  

