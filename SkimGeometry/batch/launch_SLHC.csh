#!/bin/csh


###########################################
#
# Main script for data generation
#
# The jobs themselves are launched by generator_SLHC.sh
#
# Usage:
# source launch_SLHC.sh p1 p2 p3 p4 p5 p6 p7 ...
# with:
# p1 : global tag for MC production (eg POSTLS161_V15::All)
# p2 : the number of runs
# p3 : the number of events per run 
# p4 : MU/ANTIMU/PIP/PIM/....
# p5 : PTMIN 
# p6 : PTMAX
# p7 : PHIMIN
# p8 : PHIMAX
# p9 : ETAMIN
# p10: ETAMAX
# p11: THRESH: the pt threshold for the stub maker
# p12: SUFFIX: a specific nickname for this production 
# p13: BATCH or nothing: launch lxbatch jobs or not
#
# For more details, and examples, have a look at:
# 
# http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.HLLHCTuto (STEP II)
# 
# Author: S.Viret (viret@in2p3.fr)
# Date  : 12/04/2013
#
# Script tested with release CMSSW_6_2_0_SLHC4
#
###########################################



###################################
#
# The list of parameters you can modify is here
#
###################################

# You have to adapt this to the storage element you plan to use

setenv LFC_HOST 'lyogrid06.in2p3.fr'

set STORAGEDIR  = /dpm/in2p3.fr/home/cms/data/store/user/sviret/SLHC/GEN # The directory where the data will be stored
set STORAGEDIR2 = srm://$LFC_HOST/$STORAGEDIR                            # The directory GRID-friendly address

# The directory is the one containing the minbias events necessary for PU generation
set STORAGEPU   = srm://$LFC_HOST/$STORAGEDIR/MINBIAS_forPU_2            
set NPU         = 140 # How many pile up events you want            

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

set GTAG         = ${1}           # Global tag
set N_RUN        = ${2}           # Number of samples 
set EVTS_PER_RUN = ${3}           # Number of events per sample
set MATTER       = ${4}           # Type of event
set PTYPE        = -13            # Default particle type (MUON)

@ i = 0

# The particle type is set depending on the MATTER type

# First, single particle guns

if ($MATTER == "MU") then  
	set PTYPE = -13 
endif  

if ($MATTER == "ANTIMU") then
	set PTYPE = 13 
endif  
 
if ($MATTER == "PIP") then
	set PTYPE = 211 
endif  

if ($MATTER == "PIM") then
	set PTYPE = -211
endif 

if ($MATTER == "ELE") then
	set PTYPE = -11 
endif  

if ($MATTER == "POS") then
	set PTYPE = 11
endif 

if ($MATTER == "JETS") then  
	set PTYPE = 1 
endif 

# Parameters for particle gun

set PTMIN=${5}
set PTMAX=${6}
set PHIMIN=${7}
set PHIMAX=${8}
set ETAMIN=${9}
set ETAMAX=${10}
set THRESH=${11}

# Then MinBias events production

if ($MATTER == "MINBIAS") then
	set PTYPE = 666 
endif 

# PileUp production

if ($MATTER == "PILEUP") then
	set PTYPE = 777 
endif 

if ($MATTER == "PILEUP4T") then
	set PTYPE = 888 
endif 

# Specific types for L1 Track Trigger studies

if ($MATTER == "PILEUPREDO") then
	set PTYPE = 7777 	
endif 

if ($MATTER == "ANTIMUBANK") then
	set PTYPE = 1013 
endif 

if ($MATTER == "MUBANK") then
	set PTYPE = -1013 
endif  

if ($MATTER == "TKLAYOUT") then
	set PTYPE = 999 
	@ j = 5000
endif 

# Then we setup some environment variables

cd  ..
set PACKDIR      = $PWD           # This is where the package is installed 
cd  ../..
set RELEASEDIR   = $PWD           # This is where the release is installed

cd $PACKDIR/batch




# Finally we create the batch scripts and launch the jobs

echo 'Creating directory '$STORAGEDIR/${MATTER}_${12}' for type '$PTYPE

set OUTPUTDIR  = $STORAGEDIR2/${MATTER}_${12}
set OUTPUTDIR2 = $STORAGEDIR/${MATTER}_${12}

lfc-mkdir $OUTPUTDIR2 

while ($i != $N_RUN)

    rm *.log

    echo $i	 	 

    if ($MATTER == "MUBANK") then
	set deal = `lcg-ls $OUTPUTDIR/SLHC_extr_MUBANK_${i}.root | wc -l`

	if ($deal != "0") then
	    set run  = `ls gen_job_${MATTER}_${11}_${12}_${i}.sh | wc -l`

	    if ($run != "0") then
		rm gen_job_${MATTER}_${11}_${12}_${i}.sh
	    endif
	    @ i++
	    continue
	endif

	set run  = `ls gen_job_${MATTER}_${11}_${12}_${i}.sh | wc -l`

	if ($run != "0") then
	    @ i++
	    continue
	endif
    endif  

    echo "#\!/bin/bash" > gen_job_${MATTER}_${11}_${12}_${i}.sh
    echo "source $PACKDIR/batch/generator_SLHC.sh $EVTS_PER_RUN $PTYPE $MATTER $GTAG $i $RELEASEDIR $PACKDIR $OUTPUTDIR ${PTMIN} ${PTMAX} ${PHIMIN} ${PHIMAX} ${ETAMIN} ${ETAMAX} $STORAGEPU $NPU ${THRESH}" >> gen_job_${MATTER}_${11}_${12}_${i}.sh
    chmod 755 gen_job_${MATTER}_${11}_${12}_${i}.sh

    if (${13} == "BATCH") then	
	bsub -q 8nh -e /dev/null -o /tmp/${LOGNAME}_out.txt gen_job_${MATTER}_${11}_${12}_${i}.sh
    endif

    @ i++
end 


