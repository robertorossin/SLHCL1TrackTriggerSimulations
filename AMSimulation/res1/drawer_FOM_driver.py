#!/usr/bin/env python

import subprocess
from rootdrawing import *
from parser import *
from drawer_perf_mu_resolution import main as main_muReso

# ______________________________________________________________________________
# Main function
def main(options):
    
    #
    # Choose the base directory where all the samples are stored
    #    
    inDir = "/data/rossin/EOS/"
#     inDir = "/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC25p3_results/test1/" # cmslpc
    
    options.infile = ""
    options.npatterns = 1862700 # this is to obtain 95% coverge on TT27 and sf1_nz4
    
    ###########################################################################################
    # task 1 calculates analytic efficiencies. You need extended ntuples (with extra branches) to run this option. 
    # calls the macro drawer_perf_mu_resolution.py
    #
    if (options.task==1): 
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
        if   (options.pu==0): options.infile = inDir+"SingleMuonTest_PU0_tt27_sf1_nz4_pt3_ml5_20150511/tracks_LTF_SingleMuonTest_PU0_tt27_sf1_nz4_pt3_5oo6_20150511.root"
#         if   (options.pu==0): options.infile = inDir+"SingleMuonTest_PU0_tt27_sf1_nz4_pt3_ml5_20150511/tracks.root"
        elif (options.pu==1): 
            options.infile = inDir+"SingleMuonTest_PU140_tt27_sf1_nz4_pt3_ml5_20150511/results_LTF_SingleMuonTest_PU140_tt27_sf1_nz4_pt3_5oo6_95c_20150511.root"
            options.pu=0 # this will process only the first track in the event, i.e. the muon
        elif (options.pu==140): options.infile = inDir+"Neutrino_PU140_tt27_sf1_nz4_pt3_20151107/tracks_LTF_Neutrino_PU140_tt27_sf1_nz4_pt3_5oo6_95c_14k.root"
        elif (options.pu==200): options.infile = inDir+"Neutrino_PU200_tt27_sf1_nz4_pt3_20151029/tracks_LTF_Neutrino_PU200_tt27_sf1_nz4_pt3_5oo6_95c.root"
        else                  : raise ValueError("PU option not valid")

    ##########################################################################################
    # task 3. calculates # of roads and combinations
    #
    elif (options.task==3):
        if   (options.pu==0  ): options.infile = inDir+"SingleMuonTest_PU0_tt27_sf1_nz4_pt3_ml5_20150511/tracks_LTF_SingleMuonTest_PU0_tt27_sf1_nz4_pt3_5oo6_20150511.root"
        elif (options.pu==140): options.infile = inDir+"Neutrino_PU140_tt27_sf1_nz4_pt3_20151107/tracks_LTF_Neutrino_PU140_tt27_sf1_nz4_pt3_5oo6_95c_14k.root"
        elif (options.pu==200): options.infile = inDir+"Neutrino_PU200_tt27_sf1_nz4_pt3_20151029/tracks_LTF_Neutrino_PU200_tt27_sf1_nz4_pt3_5oo6_95c.root"
        else                  : raise ValueError("PU option not valid.")
     
     # Make input file list
    if not options.infile.endswith(".root") and not options.infile.endswith(".txt"):
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
    outString = outString + "_" + puString + "_" +options.ss + "_pt" + stringPt +"_"
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
        stringCommand = "python drawer_perf_CombTrack_analysis.py %s %s %d -n %d -b --outdir %s --pu %d --minPt %f --outstring %s" % (options.infile,options.ss,options.npatterns,options.nentries,options.outdir,options.pu,options.minPt,options.outstring)
        print "Executing: %s" %stringCommand
        subprocess.call(stringCommand, shell=True)

# ______________________________________________________________________________
if __name__ == '__main__':

    # Setup argument parser
    parser = argparse.ArgumentParser()

    # Add default arguments
#     add_drawer_arguments(parser)

    # Add more arguments
    parser.add_argument("task", type=int, help="FOM to produce: analyticEff=1. syntheticEff=2. #combinations=3")
    parser.add_argument("-n", "--nentries", type=int, default=50000, help="number of entries (default: %(default)s)")
    parser.add_argument("--pu", type=int, default=0, help="number of pileup interactions to be processed (PU=1 will process one track/event in a mu+PU sample) (default: %(default)s)")
    parser.add_argument("--outdir", type=str, default="test", help="output directory (default: %(default)s)")
    parser.add_argument("--ss", type=str, default="sf1_nz4", help="short name of superstrip definition (default: %(default)s)")
#     parser.add_argument("--signal", action="store_true", help="select signal process (default: %(default)s)")
#     parser.add_argument("npatterns", type=int, help="number of patterns to reach the desired coverage")
#     parser.add_argument("--coverage", type=float, default=0.95, help="desired coverage (default: %(default)s)")
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
