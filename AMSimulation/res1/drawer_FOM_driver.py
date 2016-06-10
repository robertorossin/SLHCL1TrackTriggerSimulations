#!/usr/bin/env python

import subprocess
from rootdrawing import *
from parser import *

# ______________________________________________________________________________
# Main function
def main(options):
    
    #
    # Choose the base directory where all the samples are stored
    #    
    inDir = "/data/rossin/EOS/"
#     inDir = "/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC25p3_results/test1/" # cmslpc
    
    options.infile = ""
    options.npatterns = 0 # this is to obtain 95% coverge on TT27 and sf1_nz4
    if (options.ss=="sf1_nz4"): 
        if (options.coverage == 0.90): options.npatterns = 1184200 # 90%
        if (options.coverage == 0.95): options.npatterns = 1764800 # 95%
    if (options.ss=="sf1L0x2_nz4"): 
        if (options.coverage == 0.90): options.npatterns = 882300  # 90%
        if (options.coverage == 0.95): options.npatterns = 1309600 # 95%
    if (options.ss=="sf1L0x2L5x2_nz4"): 
        if (options.coverage == 0.90): options.npatterns =  675500 # 90%
        if (options.coverage == 0.95): options.npatterns = 1001100 # 95%
#     if (options.ss=="sf1_nz333555"): options.npatterns = 1826400 
#     if (options.ss=="sf1_nz444445"): options.npatterns = 2115200 
#     if (options.ss=="sf1_nz444454"): options.npatterns = 2227300 
#     if (options.ss=="sf1_nz444544"): options.npatterns = 2025100 
    if (options.ss=="sf1_nz444555"): options.npatterns = 2442300 
    if (options.ss==   "sf0p5_nz4"): options.npatterns = 11866600 
    if (options.ss==   "sf0p6_nz4"): options.npatterns = 6683600 
    if (options.ss==   "sf0p7_nz4"): options.npatterns = 4395600 
#     if (options.ss=="sf1_nz445444"): options.npatterns = 1970500 
#     if (options.ss=="sf1_nz454444"): options.npatterns = 1974200 
    if (options.ss=="sf1_nz544443"): options.npatterns = 1966700 
#     if (options.ss=="sf1_nz544444"): options.npatterns = 2127900 
    if (options.ss=="sf1_nz555333"): options.npatterns = 1886400 
    if (options.ss=="sf1_nz555444"): options.npatterns = 2357500 
    if (options.ss=="sf1_nz555555"): options.npatterns = 2922800
    if (options.ss=="sf1_nz666222"): options.npatterns = 1693700 
    if (options.ss=="sf1_nz666333"): options.npatterns = 2338200 
    if (options.ss=="sf1_nz666444"): options.npatterns = 2958900
    if (options.ss=="sf1_nz666555"): options.npatterns = 3627500 
    if (options.ss=="sf1_nz6"): 
        if (options.coverage == 0.90): options.npatterns = 2593800 # 90%
        if (options.coverage == 0.95): options.npatterns = 3905700 # 95%
    if (options.ss=="sf1L0x2L5x2_nz4_x8"): 
        if (options.coverage == 0.95): options.npatterns = 9999999 # 95%
    if (options.ss=="sf1L0x2L5x2_nz6_x8"): 
        if (options.coverage == 0.95): options.npatterns = 9999999 # 95%
    if (options.ss=="sf1L0x2L5x2_nz8_x8"): 
        if (options.coverage == 0.95): options.npatterns = 9999999 # 95%
    if (options.ss=="sf1L0x2_nz6"): 
        if (options.coverage == 0.90): options.npatterns = 1915900 # 90%
        if (options.coverage == 0.95): options.npatterns = 2851200 # 95%
    if (options.ss=="sf1L0x2L5x2_nz6"): 
        if (options.coverage == 0.90): options.npatterns = 1469600 # 90%
        if (options.coverage == 0.95): options.npatterns = 2180400 # 95%
    if (options.ss=="sf1_nz777111"): options.npatterns = 1546000 
    if (options.ss=="sf1_nz777222"): options.npatterns = 2142300 
    if (options.ss=="sf1_nz777333"): options.npatterns = 2836000
    if (options.ss=="sf1_nz8"): 
        if (options.coverage == 0.90): options.npatterns = 4521400 # 90%
        if (options.coverage == 0.95): options.npatterns = 6712700 # 95%
    if (options.ss=="sf1L0x2_nz8"): 
        if (options.coverage == 0.90): options.npatterns = 3374200 # 90%
        if (options.coverage == 0.95): options.npatterns = 4953500 # 95%
    if (options.ss=="sf1L0x2L5x2_nz8"): 
        if (options.coverage == 0.90): options.npatterns = 2586500 # 90%
        if (options.coverage == 0.95): options.npatterns = 3775200 # 95%
    if (options.ss=="scs1_nz8"): 
        if (options.coverage == 0.90): options.npatterns = 4625000 # 90%
        if (options.coverage == 0.95): options.npatterns = 6840200 # 95%
    if (options.ss=="sf1p26_nz4"): 
        if (options.coverage == 0.90): options.npatterns =  710200 # 90%
        if (options.coverage == 0.95): options.npatterns = 1053200 # 95%
    if (options.ss=="sf1p26_nz6"): 
        if (options.coverage == 0.90): options.npatterns = 1550600 # 90%
        if (options.coverage == 0.95): options.npatterns = 2304700 # 95%
    if (options.ss=="sf1p26_nz8"): 
        if (options.coverage == 0.90): options.npatterns = 2728900 # 90%
        if (options.coverage == 0.95): options.npatterns = 3990200 # 95%
    if (options.ss=="sf1_nz888111"): options.npatterns = 1899700 
    if (options.ss=="sf1_nz888222"): options.npatterns = 2585300
    if (options.ss=="sf1_nz8887910"):options.npatterns = 8024700

    str_coverage = str(int(options.coverage*100.0001))
    str_pu       = str(options.pu)

    ###########################################################################################
    # task 0 runs all 3 options  
    if (options.task==0): 
        stringCommand = "python drawer_FOM_driver.py 1 --pu 0" # call analytic effs
        print "Executing: %s" %stringCommand
        subprocess.call(stringCommand, shell=True)
        stringCommand = "python drawer_FOM_driver.py 2 --pu 0" # call synthetic effs on singleMu
        print "Executing: %s" %stringCommand
        subprocess.call(stringCommand, shell=True)
        stringCommand = "python drawer_FOM_driver.py 2 --pu 140" # call synthetic effs on PU events
        print "Executing: %s" %stringCommand
        subprocess.call(stringCommand, shell=True)
        stringCommand = "python drawer_FOM_driver.py 3 --pu 140" # call combinatorics calculation on PU events
        print "Executing: %s" %stringCommand
        subprocess.call(stringCommand, shell=True)
        
        return

    ###########################################################################################
    # task 1 calculates analytic efficiencies. You need extended ntuples (with extra branches) to run this option. 
    # calls the macro drawer_perf_mu_resolution.py
    #
    elif (options.task==1): 
        if (options.pu==0): options.infile = inDir+"SingleMuonTest_tt27_PU0_20150815_fullNtuple/results_LTF_SingleMuonTest_tt27_PU0_sf1_nz4_pt3_5oo6_95c_100k.root"
        else: raise Exception("Analytic efficiencies can be calculated only on singleMuon samples with no PU")

    #########################################################################################
    # task 2 calculates synthetic efficiency and track resolutions
    # PU options
    # PU = 0. running on single track sample
    # PU = 140. running on MinBias sample. considering all the tracks in the event with Pt > minPt
    # PU = 200. same as above
    # PU = 1.   running on a SingleTrack + PU sample, but considering only the SingleTrack for efficiency caclulation
    #
    elif (options.task==2):
        ssString = options.ss
        isX8 = False
#         print options.ss
        if (options.ss.find("sf1L0x2L5x2_nz")>=0 or options.ss.find("sf1L0x2_nz")>=0):
            if (options.ss.find("_x8")>0):
                isX8 = True
                ssString = options.ss.replace("_x8","")
            inDir = inDir+"/tt27_"+ssString+"_pt3_20160308/"
        if   (options.pu==0): 
            if   (options.ss=="sf1_nz4"): options.infile = inDir+"SingleMuonTest_PU0_tt27_sf1_nz4_pt3_ml5_20150511/tracks_LTF_SingleMuonTest_PU0_tt27_sf1_nz4_pt3_5oo6_20150511.root"
            elif (options.ss.find("sf1_nz") and len(options.ss)==12): options.infile = inDir+"SSscan/tracks_LTF_SingleMuonTest_PU0_tt27_"+options.ss+"_pt3_5oo6_95c_20151204.root"
            if (isX8): options.infile = inDir+"tracks_LTF_SingleMuonTest_PU0_tt27_"+ssString+"_pt3_5oo6_100c_chi100_300M_x8_.root"
            else     : options.infile = inDir+"tracks_LTF_SingleMuonTest_PU0_tt27_"+ssString+"_pt3_5oo6_95c_chi100_300M.root"
#         if   (options.pu==0): options.infile = inDir+"SingleMuonTest_PU0_tt27_sf1_nz4_pt3_ml5_20150511/tracks.root"
        elif (options.pu==1): 
            options.infile = inDir+"SingleMuonTest_PU140_tt27_sf1_nz4_pt3_ml5_20150511/tracks_LTF_SingleMuonTest_PU140_tt27_sf1_nz4_pt3_5oo6_95c_20150511.root"
            if (isX8): options.infile = inDir+"tracks_LTF_SingleMuonTest_PU140_tt27_"+ssString+"_pt3_5oo6_100c_chi100_300M_x8_.root"
            else     : options.infile = inDir+"tracks_LTF_SingleMuonTest_PU140_tt27_"+ssString+"_pt3_5oo6_95c_chi100_300M.root"
            options.pu=0 # this will process only the first track in the event, i.e. the muon
        elif (options.pu==140): options.infile = inDir+"Neutrino_PU140_tt27_sf1_nz4_pt3_20151107/tracks_LTF_Neutrino_PU140_tt27_sf1_nz4_pt3_5oo6_95c_14k.root"
        elif (options.pu==200): options.infile = inDir+"Neutrino_PU200_tt27_sf1_nz4_pt3_20151029/tracks_LTF_Neutrino_PU200_tt27_sf1_nz4_pt3_5oo6_95c.root"
        else                  : raise ValueError("PU option not valid")

#         print options.infile, isX8
    ##########################################################################################
    # task 3. calculates # of roads and combinations
    #
    elif (options.task==3):
        if   (options.pu==0  ): 
            if   (options.ss=="sf1_nz4"): options.infile = inDir+"SingleMuonTest_PU0_tt27_sf1_nz4_pt3_ml5_20150511/tracks_LTF_SingleMuonTest_PU0_tt27_sf1_nz4_pt3_5oo6_20150511.root"
#             elif (options.ss.find("sf1_nz") and len(options.ss)==12): options.infile = inDir+"SSscan/tracks_LTF_SingleMuonTest_PU0_tt27_"+options.ss+"_pt3_5oo6_95c_20151204.root"
        elif (options.pu==140): 
#             if   (options.ss=="sf1_nz4"): options.infile = inDir+"Neutrino_PU140_tt27_sf1_nz4_pt3_20151107/tracks_LTF_Neutrino_PU140_tt27_sf1_nz4_pt3_5oo6_95c_14k.root"
            if   (options.ss.find("sf1_nz")>=0 and len(options.ss)==7): options.infile = inDir+"tt27_"+options.ss+"_pt3_20160308/tracks_LTF_Neutrino_PU"+str_pu+"_tt27_"+options.ss+"_pt3_5oo6_"+str_coverage+"c_chi100_300M.root"
            elif (options.ss=="sf1_nz8"): options.infile = inDir+"Neutrino_PU140_tt27_sf1_nz8_pt3_20151209/tracks_LTF_Neutrino_PU140_tt27_sf1_nz888888_pt3_5oo6_95c_20151204.root"
            elif (options.ss.find("sf1p26_nz")>=0): options.infile = inDir+"tt27_sf1p26_pt3_20160308/tracks_LTF_Neutrino_PU140_tt27_"+options.ss+"_pt3_5oo6_95c_chi100_300M.root"
            elif (options.ss.find("sf1_nz")>=0 and len(options.ss)>=12): options.infile = inDir+"SSscan/tracks_LTF_Neutrino_PU140_tt27_"+options.ss+"_pt3_5oo6_95c_20151204.root"
            elif (options.ss=="sf0p5_nz4" or options.ss=="sf0p6_nz4" or options.ss=="sf0p7_nz4"): options.infile = inDir+"tt27_"+options.ss+"_pt3_20160228/tracks_LTF_Neutrino_PU140_tt27_"+options.ss+"_pt3_5oo6_95c_20160228.root"
            elif (options.ss.find("sf1L0x2_nz")>=0 and len(options.ss)==11): options.infile = inDir+"tt27_"+options.ss+"_pt3_20160308/tracks_LTF_Neutrino_PU"+str_pu+"_tt27_"+options.ss+"_pt3_5oo6_"+str_coverage+"c_chi100_300M.root"
            elif (options.ss.find("sf1L0x2L5x2_nz")>=0 and len(options.ss)==15): options.infile = inDir+"tt27_"+options.ss+"_pt3_20160308/tracks_LTF_Neutrino_PU"+str_pu+"_tt27_"+options.ss+"_pt3_5oo6_"+str_coverage+"c_chi100_300M.root"
        elif (options.pu==200): 
            if   (options.ss.find("sf1_nz")>=0 and len(options.ss)==7): options.infile = inDir+"tt27_"+options.ss+"_pt3_20160308/tracks_LTF_Neutrino_PU"+str_pu+"_tt27_"+options.ss+"_pt3_5oo6_"+str_coverage+"c_chi100_300M.root"
#             if   (options.ss=="sf1_nz4"    ): options.infile = inDir+"Neutrino_PU200_tt27_sf1_nz4_pt3_20151029/tracks_LTF_Neutrino_PU200_tt27_sf1_nz4_pt3_5oo6_"+str_coverage+"c.root"
#             elif (options.ss=="sf1L0x2_nz4"): options.infile = inDir+"tt27_sf1L0x2_nz4_pt3_20160308/tracks_LTF_Neutrino_PU200_tt27_sf1L0x2_nz4_pt3_5oo6_"+str_coverage+"c_chi100_300M.root"
            elif (options.ss=="sf1_nz6"    ): options.infile = inDir+"tt27_sf1_nz6_pt3_20160308/tracks_LTF_Neutrino_PU200_tt27_sf1_nz6_pt3_5oo6_"        +str_coverage+"c_chi100_272M.root"
            elif (options.ss=="sf1_nz8"    ): options.infile = inDir+"tt27_sf1_nz8_pt3_20160308/tracks_LTF_Neutrino_PU200_tt27_sf1_nz8_pt3_5oo6_"        +str_coverage+"c_chi100_300M.root"
#             elif (options.ss=="sf1L0x2_nz8"): options.infile = inDir+"tt27_sf1L0x2_nz8_pt3_20160308/tracks_LTF_Neutrino_PU200_tt27_sf1L0x2_nz8_pt3_5oo6_"+str_coverage+"c_chi100_300M.root"
            elif (options.ss=="scs1_nz8"   ): options.infile = inDir+"candleStick/tracks_LTF_Neutrino_PU200_tt27_scs1_nz8_pt3_5oo6_"+str_coverage+"c_chi100_300M.root"
#             elif (options.ss=="sf1_nz8"): options.infile = inDir+"Neutrino_PU200_tt27_sf1_nz8_pt3_20151209/tracks_LTF_Neutrino_PU200_tt27_sf1_nz888888_pt3_5oo6_95c_20151204.root"
            elif (options.ss.find("sf1L0x2_nz")>=0 and len(options.ss)==11): options.infile = inDir+"tt27_"+options.ss+"_pt3_20160308/tracks_LTF_Neutrino_PU"+str_pu+"_tt27_"+options.ss+"_pt3_5oo6_"+str_coverage+"c_chi100_300M.root"
            elif (options.ss.find("sf1L0x2L5x2_nz")>=0 and len(options.ss)==15): options.infile = inDir+"tt27_"+options.ss+"_pt3_20160308/tracks_LTF_Neutrino_PU"+str_pu+"_tt27_"+options.ss+"_pt3_5oo6_"+str_coverage+"c_chi100_300M.root"
            elif (options.ss.find("sf1_nz")>=0 and len(options.ss)>=12): options.infile = inDir+"SSscan/tracks_LTF_Neutrino_PU140_tt27_"+options.ss+"_pt3_5oo6_95c_20151204.root"
        else                  : raise ValueError("PU option not valid.")
     
     # Make input file list
    if not options.infile.endswith(".root") and not options.infile.endswith(".txt"):
#         print options.ss, options.coverage, str_coverage
        print options.infile
        raise ValueError("infile must be .root file or .txt file")

    if options.infile.endswith(".root"):
        # Create a temporary file with one line
        with tempfile.NamedTemporaryFile() as infile:
            infile.write(options.infile)
            infile.flush()
            options.tfilecoll = TFileCollection("fc", "", infile.name)
    else:
        options.tfilecoll = TFileCollection("fc", "", options.infile)

    # Batch mode. In interactive mode the canvases saved in png are not updated
    gROOT.SetBatch(True)
    
    # prepare an outString for output labelling
    inName = options.infile
    fileName = inName.split("/")[-1]
    fileNameList = fileName.split("_")
    outString = ""
    puString = "PU"  #used also to handle the special case of SingleTrack+PU when only the single track is used for the studies
    for token in fileNameList:
        if (token.lower().find("single")>=0 or token.lower().find("neutrino")>=0 or token.lower().find("tth")>=0 or token.lower().find("ttbar")>=0):
            outString = outString + "_" + token
        if (token.find("PU")>=0):
            puFromFileName = int(token.replace("PU",""))
            if (puFromFileName == options.pu):
                puString = puString + str(puFromFileName)
            else:
                puString = puString + str(puFromFileName) + "SingleTrack"
    
    stringPt = "{:.0f}".format(options.minPt)
    outString = outString + "_" + puString + "_" +options.ss + "_pt" + stringPt +"_"+str_coverage+"c_"
    options.outstring = outString

    # Init
    tchain = TChain("ntupler/tree", "")
    tchain.AddFileInfoList(options.tfilecoll.GetList())
    
    
    if (tchain.GetEntries()==0):
        print "Cannot open file %s or file empty." % options.infile
        return
    
    has_pixelDigis = 0
    has_mixedSimHits = 0
    if (options.task==1):
        listOfBranches=tchain.GetListOfBranches()
        for i in range(len(listOfBranches)):
            branchName = listOfBranches.At(i).GetName()
            if ("pixelDigis_"   in branchName): has_pixelDigis   = 1
            if ("mixedSimHits_" in branchName): has_mixedSimHits = 1
        if (not has_mixedSimHits or not has_pixelDigis):
            raise Exception("Analytic efficiencies can be calculated only on singleMuon samples with pixelDigis_* and mixedSimHits_* branches")
        stringCommand = "python drawer_perf_mu_factEff.py %s %s %d -n %d -b --outdir %s --pu %d --minPt %f --outstring %s" % (options.infile,options.ss,options.npatterns,options.nentries,options.outdir,options.pu,options.minPt,options.outstring)
        print "Executing: %s" %stringCommand
        subprocess.call(stringCommand, shell=True)
    elif (options.task==2):
        stringCommand = "python drawer_perf_eff2.py %s %s %d -n %d -b --outdir %s --pu %d --minPt %f --outstring %s" % (options.infile,options.ss,options.npatterns,options.nentries,options.outdir,options.pu,options.minPt,options.outstring)
        print "Executing: %s" %stringCommand
        subprocess.call(stringCommand, shell=True)
    elif (options.task==3):
        stringCommand = "python drawer_perf_CombTrack_analysis.py %s %s %d -n %d -b --outdir %s --pu %d --minPt %f --outstring %s" % (options.infile,options.ss,options.npatterns,options.nentries,options.outdir+options.outstring,options.pu,options.minPt,options.outstring)
        print "Executing: %s" %stringCommand
        subprocess.call(stringCommand, shell=True)

# ______________________________________________________________________________
if __name__ == '__main__':

    # Setup argument parser
    parser = argparse.ArgumentParser()

    # Add default arguments
#     add_drawer_arguments(parser)

    # Add more arguments
    parser.add_argument("task", type=int, help="FOM to produce: all=0. analyticEff=1. syntheticEff=2. #combinations=3")
    parser.add_argument("-n", "--nentries", type=int, default=50000, help="number of entries (default: %(default)s)")
    parser.add_argument("--pu", type=int, default=0, help="number of pileup interactions to be processed (PU=1 will process one track/event in a mu+PU sample) (default: %(default)s)")
    parser.add_argument("--outdir", type=str, default="test", help="output directory (default: %(default)s)")
    parser.add_argument("--ss", type=str, default="sf1_nz4", help="short name of superstrip definition (default: %(default)s)")
#     parser.add_argument("--signal", action="store_true", help="select signal process (default: %(default)s)")
#     parser.add_argument("npatterns", type=int, help="number of patterns to reach the desired coverage")
    parser.add_argument("--coverage", type=float, default=0.95, help="desired coverage (default: %(default)s)")
    parser.add_argument("--minPt", type=float, default=3, help="min pT for gen particle (default: %(default)s)")
#     parser.add_argument("--maxChi2", type=float, default=5, help="max reduced chi-squared (default: %(default)s)")
#     parser.add_argument("--low-stat", action="store_true", help="low statistics (default: %(default)s)")
#     parser.add_argument("--low-low-stat", action="store_true", help="low low statistics (default: %(default)s)")

    # Parse default arguments
    options = parser.parse_args()
#     parse_drawer_options(options)
#     options.ptmin = options.minPt

    # Call the main function
    main(options)
