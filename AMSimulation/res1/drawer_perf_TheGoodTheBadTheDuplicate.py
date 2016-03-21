#!/usr/bin/env python

from collections import defaultdict
from rootdrawing import *
from parser import *
import operator
from _bisect import bisect_left

col  = TColor.GetColor("#1f78b4")  # mu0
fcol = TColor.GetColor("#a6cee3")  # mu0

#col  = TColor.GetColor("#e31a1c")  # nu140
#fcol = TColor.GetColor("#fb9a99")  # nu140

#col  = TColor.GetColor("#6a3d9a")  # tttt140
#fcol = TColor.GetColor("#cab2d6")  # tttt140

col1 = TColor.GetColor("#BA0000")
col2 = TColor.GetColor("#BA00BA")
col3 = TColor.GetColor("#3333BA")
fcol1 = TColor.GetColor("#E58080")
fcol2 = TColor.GetColor("#E580E5")
fcol3 = TColor.GetColor("#9999E5")

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

# ______________________________________________________________________________
def use_TEfficiency():
    del ROOT.TEfficiency
    gROOT.ProcessLine(".L ./fixes/TEfficiency.cxx+")
    #gSystem.Load("./fixes/TEfficiency_cxx.so")

    global TEfficiency
    TEfficiency = ROOT.TEfficiency


def ncommonSS(road1,road2):
    # if distanceLogic == "ss" the distance is calculated as Abs(DeltaSS)
    # if distanceLogic == "layer" the distance is calculated as # of differing layers
#     assert(road1.size()==road2.size())
    distance=0
    for ilayer, SS in enumerate(road1[3]):
        if (SS == road2[3][ilayer] and ilayer not in road1[2] and ilayer not in road2[2]): distance = distance+1 # count common SS
    return distance

parnames    = ["reducedChi2"      ,"invPt"           , "eta"         , "phi"          , "z0"        ]
parlabel    = ["reduced #chi^{2}" ,"1/p_{T} [c/GeV]" , "#eta"        , "#phi"         , "z0 [cm]"   ]
parnbins    = [200                ,200               , 100           , 100            , 100         ]
parxmin     = [0.                 , -0.4             , -0.5          , 1.57/5         , -15.        ]
parxmax     = [50.                ,  0.4             ,  1.5          , 1.57/0.8       ,  15.        ]
parxminD    = [0.                 , -0.05            , -0.05         , -0.01          , -1.         ]
parxmaxD    = [50.                ,  0.05            ,  0.05         ,  0.01          ,  1.         ]
                                  
# parsN_1min  = [[0.,0.]            ,[-0.04 ,-0.025]   ,[-0.016,-0.007],[-0.005, -0.003],[-0.5,-0.25] ]
# parsN_1max  = [[5.,5.]            ,[ 0.025, 0.015]   ,[ 0.016, 0.007],[ 0.005,  0.003],[ 0.5, 0.25] ]
parsN_1min  = [[0. ,0. ]          ,[-0.0048,-0.0045] ,[-0.011,-0.006],[-0.0018, -0.0016],[-0.454,-0.223] ]
parsN_1max  = [[50.,50.]          ,[ 0.0052, 0.0045] ,[ 0.011, 0.006],[ 0.0017,  0.0016],[ 0.504, 0.218] ]

# ______________________________________________________________________________
def drawer_book(options):
    histos = {}

    hname = "nroads_5oo6_per_event"
    nbins, xmin, xmax = 400, 0., 400.
    histos[hname] = TH1F(hname, "; # roads [5/6]/tower/BX"                , nbins, xmin, xmax)

    hname = "nroads_6oo6_per_event"
    nbins, xmin, xmax = 40, 0., 40.
    histos[hname] = TH1F(hname, "; # roads [6/6]/tower/BX"                , nbins, xmin, xmax)
    
    hname = "nroads_per_event"
    nbins, xmin, xmax = 400, 0., 400.
    histos[hname] = TH1F(hname, "; # roads/tower/BX"                , nbins, xmin, xmax)
    
    hname = "ncombinations_per_event"
    nbins, xmin, xmax = 1600, 0., 1600.*options.xscale
    histos[hname] = TH1F(hname, "; # fits/tower/BX"                 , nbins, xmin, xmax)

    hname = "ntracks_per_event"
    nbins, xmin, xmax = 200, 0., 200.*options.xscale
    histos[hname] = TH1F(hname, "; # tracks/tower/BX"               , nbins, xmin, xmax)

    hname = "ngoods_per_event"
    nbins, xmin, xmax = 20, 0., 20.*options.xscale
    histos[hname] = TH1F(hname, "; # good tracks/tower/BX"          , nbins, xmin, xmax)

    hname = "nduplicates_per_event"
    nbins, xmin, xmax = 200, 0., 200.*options.xscale
    histos[hname] = TH1F(hname, "; # duplicate tracks/tower/BX"     , nbins, xmin, xmax)

    hname = "nfakes_per_event"
    nbins, xmin, xmax = 150, 0., 150.*options.xscale
    histos[hname] = TH1F(hname, "; # fake tracks/tower/BX"          , nbins, xmin, xmax)

    histos_SSclean = {}
#     for hname, h in histos.iteritems():
#         if hname.find("_per_event") == -1: continue
#         hnameSS = hname.replace("_per_event", "_SSclean_per_event")
#         histos[hnameSS] = h.Clone(hnameSS)
#         print histos_SSclean[hnameSS].GetYaxis().GetTitle(), "pippo"

    logics = ["_5oo6","_6oo6"]
    objects = ["tracks","trkparts","roads"]
    for iobj in range(len(objects)):
        for ipar in range(len(parnames)):
            if (iobj==0 or ipar>0):
                for logic in logics:
                    if (iobj>0):
                        logic=""
                    hname = objects[iobj]+"_"+parnames[ipar]+logic
                    hlabel = "; "+parlabel[ipar]
                    histos[hname] = TH1F(hname, hlabel, parnbins[ipar], parxmin[ipar], parxmax[ipar])
                    if (iobj>0): continue
    
    pref = ["Nminus1_","Delta_"]
    prefl= ["N-1 "    ,"#Delta"]
    prefixes = ["","_SSclean"]
    
    for prefix in prefixes:
        for ipar in range(0,len(parnames)):
            for logic in logics:
                    hname = pref[bool(ipar)]+parnames[ipar]+logic+prefix
                    hlabel = "; "+prefl[bool(ipar)]+parlabel[ipar]+prefix
                    histos[hname] = TH1F(hname, hlabel, parnbins[ipar], parxminD[ipar], parxmaxD[ipar])
    
    prefixes = ["efficiency_","efficiency_SSclean_"]
    ytitle = "#varepsilon_{synthetic}"

    for prefix in prefixes:
        hname = prefix + "pt"  # pt, 0-500 GeV
        histos[hname] = TEfficiency(hname, "; p_{T} [GeV]; %s" % ytitle, 50, 0., 500.)
    
        hname = prefix + "ppt"  # pt, 0-50 GeV
        histos[hname] = TEfficiency(hname, "; p_{T} [GeV]; %s" % ytitle, 50, 0., 50.)
    
        hname = prefix + "pppt"  # pt, 0-10 GeV
        histos[hname] = TEfficiency(hname, "; p_{T} [GeV]; %s" % ytitle, 50, 0., 10.)
    
        hname = prefix + "eta"
        histos[hname] = TEfficiency(hname, "; #eta; %s" % ytitle,        120, -3.0, 3.0)
    
        hname = prefix + "phi"
        histos[hname] = TEfficiency(hname, "; #phi; %s" % ytitle,        128, -3.2, 3.2)
    
        hname = prefix + "vz"
        histos[hname] = TEfficiency(hname, "; v_{z} [cm]; %s" % ytitle,  120, -30, 30)

    
    # Error statistics
    for hname, h in histos.iteritems():
        if h.ClassName() == "TEfficiency":
            h.SetConfidenceLevel(0.682689492137)  # 1-sigma
            h.SetStatisticOption(0)  # kFCP
            #h.SetStatisticOption(6)  # kBUniform

    for hname, h in histos.iteritems():
        print hname

#     histo_tracks_SSclean = {}
#     for hname, h in histos.iteritems():
#         for c in ["ntracks", "ngoods", "nduplicates", "nfakes"]:
#             if (hname.find(c)==0):
#                 hnameSS = hname.replace(c,c+"_SSclean")
#                 histo_tracks_SSclean[hnameSS] = h.Clone(hnameSS)
#     for hname, h in histo_tracks_SSclean.iteritems():
#         histos[hname] = h
#     del histo_tracks_SSclean
               
#     for hname, h in histos.iteritems():
#         print hname, h

#     for c in ["good", "duplicate", "fake"]:
#         hname = "pt_%s" % c
#         nbins, xmin, xmax = 80, 0., 80.
#         histos[hname] = TH1F(hname, "; track p_{T} [GeV]", nbins, xmin, xmax)

#         hname = "eta_%s" % c
#         nbins, xmin, xmax = 40, 0., 0.8
#         histos[hname] = TH1F(hname, "; track #eta", nbins, xmin, xmax)


    # Style
    for hname, h in histos.iteritems():
#         if h.ClassName() != "TEfficiency": continue
        h.SetLineWidth(2); h.SetMarkerSize(0)
        h.SetMarkerStyle(24); h.SetMarkerSize(1)
        h.SetMarkerColor(col); h.SetLineColor(col); h.SetFillColor(fcol)
        if (hname.find("SSclean")>=0): h.SetMarkerColor(col1); h.SetLineColor(col1); h.SetFillColor(fcol1)
    donotdelete.append(histos)
    return histos

def repr_cppvector(v):
    s = ",".join([str(x) for x in v])
    s = "[" + s + "]"
    return s

def drawer_project(tree, treepattern, treepatternA, histos, options):
    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("trkParts_pt"     , 1)
    tree.SetBranchStatus("trkParts_eta"    , 1)
    tree.SetBranchStatus("trkParts_phi"    , 1)
    #tree.SetBranchStatus("trkParts_vx"     , 1)
    #tree.SetBranchStatus("trkParts_vy"     , 1)
    tree.SetBranchStatus("trkParts_vz"     , 1)
    tree.SetBranchStatus("trkParts_charge" , 1)
    tree.SetBranchStatus("trkParts_primary", 1)
    tree.SetBranchStatus("trkParts_signal" , 1)
    tree.SetBranchStatus("trkParts_pdgId"  , 1)
    tree.SetBranchStatus("AMTTRoads_patternRef" , 1)
    tree.SetBranchStatus("AMTTRoads_stubRefs"   , 1)
    tree.SetBranchStatus("AMTTRoads_patternInvPt"   , 1)
    tree.SetBranchStatus("AMTTRoads_superstripIds"   , 1)
    tree.SetBranchStatus("AMTTTracks_invPt"     , 1)
    tree.SetBranchStatus("AMTTTracks_phi0"      , 1)
    tree.SetBranchStatus("AMTTTracks_cottheta"  , 1)
    tree.SetBranchStatus("AMTTTracks_z0"        , 1)
    tree.SetBranchStatus("AMTTTracks_pt"        , 1)
    tree.SetBranchStatus("AMTTTracks_eta"       , 1)
    tree.SetBranchStatus("AMTTTracks_chi2"      , 1)
    tree.SetBranchStatus("AMTTTracks_ndof"      , 1)
    tree.SetBranchStatus("AMTTTracks_synTpId"   , 1)
    tree.SetBranchStatus("AMTTTracks_patternRef", 1)
    tree.SetBranchStatus("AMTTTracks_stubRefs"  , 1)
    tree.SetBranchStatus("AMTTTracks_roadRef"   , 1)

    # Get the pattern tree branch
    treepattern.SetBranchStatus("frequency", 1)

    icount = 0
    # Loop over events
    numEntries, denomEntries = 0, 0
    numEntries_SSclean = 0

    for ievt, evt in enumerate(tree):
        if (ievt == options.nentries):  break

        if   (ievt % 1000 == 0               ):  print "Processing event: %i" % ievt
        elif (ievt %  500 == 0 and options.pu):  print "Processing event: %i" % ievt

        nparts_all = evt.trkParts_primary.size()
        trkparts = []

        for ipart in xrange(nparts_all):
            if options.pu <=1:  # single-track events
                if ipart > 0:
                    break
 
            charge  = evt.trkParts_charge [ipart]
            primary = evt.trkParts_primary[ipart]
            signal  = evt.trkParts_signal [ipart]
 
            if not (charge!=0 and primary):
                continue
 
            if options.signal and not signal:
                continue
 
            pt      = evt.trkParts_pt     [ipart]
            eta     = evt.trkParts_eta    [ipart]
            phi     = evt.trkParts_phi    [ipart]
            #vx      = evt.trkParts_vx     [ipart]
            #vy      = evt.trkParts_vy     [ipart]
            vz      = evt.trkParts_vz     [ipart]
            pdgId   = evt.trkParts_pdgId  [ipart]
 
            trkparts.append([pt, eta, phi, vz, charge, pdgId,[]])
            if options.verbose:  print ievt, "part ", ipart, trkparts[-1]
            
#             if not (options.ptmin < pt < options.ptmax and options.etamin < eta < options.etamax and options.phimin < phi < options.phimax):
#                 continue
 
            hname = "trkparts_"+parnames[1]
            histos[hname].Fill(charge/pt)
            hname = "trkparts_"+parnames[2]
            histos[hname].Fill(eta      )
            hname = "trkparts_"+parnames[3]
            histos[hname].Fill(phi      )
            hname = "trkparts_"+parnames[4]
            histos[hname].Fill(vz      )
 

#         if options.pu <= 1:  # single-track events
#             pt = evt.trkParts_pt[0]
#             if not (options.ptmin < pt < options.ptmax and options.etamin < eta < options.etamax and options.phimin < phi < options.phimax):
#                 continue

        nroads_all = evt.AMTTRoads_patternRef.size()
        nroads5, nroads6, ncombs = 0, 0, 0
        roadList = []
        roads={}
        icount = icount+1
        
        for iroad, (patternRef, patternInvPt, superstripIdsA, stubRefsA) in enumerate(
            izip(evt.AMTTRoads_patternRef, evt.AMTTRoads_patternInvPt, evt.AMTTRoads_superstripIds, evt.AMTTRoads_stubRefs)):

            if not (patternRef < options.npatterns):
                continue

            misslayers = []
            # Collect superstrips and stubs
            for ilayer, stubRefsB in enumerate (stubRefsA):
                if (not stubRefsB.size()): misslayers.append(ilayer)

            if (len(misslayers)): nroads5 += 1
            else                : nroads6 += 1

            roads[patternRef] = (misslayers,evt.AMTTRoads_superstripIds, evt.AMTTRoads_stubRefs)

            ncombs_per_road = 1
            for v in stubRefsA:
                n = v.size()
                if n != 0:
                    ncombs_per_road *= n
            ncombs += ncombs_per_road
            treepattern.GetEntry(patternRef)
            treepatternA.GetEntry(patternRef)
            
            roadList.append((patternRef,6-len(misslayers),misslayers,superstripIdsA,stubRefsA, treepatternA.invPt_mean,treepatternA.phi_mean))

#         roadListSorted = sorted(roadList      , key = operator.itemgetter(2)              )
#         roadListSorted = sorted(roadListSorted, key = operator.itemgetter(1), reverse=True)

        roadListCluster = []
#         for iroad, road in enumerate(roadListSorted):
#             nCommonLayers = 0
#             roadclusterindex = -1
#             for iroadclean in range(len(roadListCluster)):
#                 roadclean = roadListCluster[iroadclean][0]
#                 ncommon = ncommonSS(road,roadclean)
#                 if nCommonLayers < ncommon:
#                     nCommonLayers = ncommon
#                     roadclusterindex = iroadclean
#             if (nCommonLayers<options.distance):
#                 roadListCluster.append([road])
#             else:
#                 roadListCluster[roadclusterindex].append(road)

        roadListClean = {}
        maxsiblings = 0
        
        nroadsSSfilter = 0
        ncombsSSfilter = 0
#         for iroadC, roadcluster in enumerate(roadListCluster):
#             for iroad, road in enumerate(roadcluster):
#                 if iroad>maxsiblings: break
#                 nroadsSSfilter += 1
#                 stubRefsA = road[4]
#                 roadListClean[road[0]]=road
#                 ncombs_per_road = 1
#                 for v in stubRefsA:
#                     n = v.size()
#                     if n != 0:
#                         ncombs_per_road *= n
#                     ncombsSSfilter += ncombs_per_road

        ntracks_all = evt.AMTTTracks_patternRef.size()
        ntracks, ngoods, nduplicates, nfakes = 0, 0, 0, 0
        ntracks_SSclean, ngoods_SSclean, nduplicates_SSclean, nfakes_SSclean = 0, 0, 0, 0

        trkparts_trigger = {}
        trkparts_trigger_vars = {}
        trkparts_trigger_SSclean = {}
        trkparts_trigger_vars_SSclean = {}

#         tracks = {}
        trackList = []

        pref = ["Nminus1_","Delta_"]
        logics   = ["_5oo6","_6oo6"]
        ndoffit = [6,8]
        for itrack in xrange(ntracks_all):

            trigger = True
            trigger_SSclean = True
 
            patternRef = evt.AMTTTracks_patternRef[itrack]
            if not (patternRef < options.npatterns):
                continue
 
            chi2     = evt.AMTTTracks_chi2[itrack]
            ndof     = evt.AMTTTracks_ndof[itrack]
            chi2Red  = chi2/ndof

            track_pt      = evt.AMTTTracks_pt        [itrack]
            track_invpt   = evt.AMTTTracks_invPt     [itrack]
            track_eta     = evt.AMTTTracks_eta       [itrack]
            track_phi0    = evt.AMTTTracks_phi0      [itrack]
            track_z0      = evt.AMTTTracks_z0        [itrack]
            track_pattRef = evt.AMTTTracks_patternRef[itrack]
            track_roadRef = evt.AMTTTracks_roadRef   [itrack]
 
            for i in range(len(logics)):
                if (ndof==ndoffit[i]):
                    hname = "tracks_"+parnames[0]+logics[i]
                    histos[hname].Fill(chi2Red)
                    hname = "tracks_"+parnames[1]+logics[i]
                    histos[hname].Fill(track_invpt)
                    hname = "tracks_"+parnames[2]+logics[i]
                    histos[hname].Fill(track_eta)
                    hname = "tracks_"+parnames[3]+logics[i]
                    histos[hname].Fill(track_phi0)
                    hname = "tracks_"+parnames[4]+logics[i]
                    histos[hname].Fill(track_z0)
            
            ntracks += 1

            isSSclean = 0
            if (track_pattRef in roadListClean): isSSclean = 1
#             tracks[track_pattRef] = (track_invpt,track_eta,track_phi0,track_z0,track_roadRef,chi2,ndof)
            trackList.append((ndof,track_pattRef,track_invpt,track_eta,track_phi0,track_z0,track_roadRef,chi2,isSSclean))
        trackListSorted = sorted(trackList      , key = operator.itemgetter(1)              )
        trackListSorted = sorted(trackListSorted, key = operator.itemgetter(0), reverse=True)
            
        tracksMatched         = {}
        tracksCleanMatched         = {}
        
        for itrack, track in enumerate(trackListSorted):
            ndof            = track[0]
            track_pattRef   = track[1]
            track_invpt     = track[2]
            track_eta       = track[3]
            track_phi0      = track[4]
            track_z0        = track[5]
            chi2            = track[7]
            isSSclean       = track[8]
            track_partmatch = -2
            track_partCleanmatch = -2
            i = 0
            if (ndof==6): i=1
            isFirstMatch = True
            isFirstCleanMatch = True
            for itrkpart in range(len(trkparts)):
                [pt, eta, phi, z0, charge, pdgId,trackmatches]=trkparts[itrkpart]
                if                         chi2/ndof             < parsN_1max[0][i] \
                    and parsN_1min[1][i] < track_invpt-charge/pt < parsN_1max[1][i] \
                    and parsN_1min[2][i] < track_eta-eta         < parsN_1max[2][i] \
                    and parsN_1min[3][i] < track_phi0-phi        < parsN_1max[3][i] \
                    and parsN_1min[4][i] < track_z0-z0           < parsN_1max[4][i]:
                    trkparts[itrkpart][6].append(itrack)
                    if isFirstMatch:
                        track_partmatch = itrkpart
                        isFirstMatch=False
                    else:
                        track_partmatch = -1000000-itrkpart
                    if not isSSclean: continue
                    if isFirstCleanMatch:
                        track_partCleanmatch = itrkpart
                        isFirstCleanMatch=False
                    else:
                        track_partCleanmatch = -1000000-itrkpart
            if track_partmatch     ==-2: nfakes         += 1
            if track_partCleanmatch==-2: nfakes_SSclean += 1

            tracksMatched[itrack]=(ndof,track_pattRef,track_invpt,track_eta,track_phi0,track_z0,chi2,track_partmatch,track_partCleanmatch)
            
        for itrack, vtrack in tracksMatched.iteritems():
            ndof,track_pattRef,track_invpt,track_eta,track_phi0,track_z0,chi2,track_partmatch,track_partCleanmatch = vtrack
#         for itrack, vtrack in tracks.iteritems():
#             track_invpt, track_eta, track_phi0, track_z0,track_roadRef,chi2,ndof = vtrack
            for itrkpart in range(len(trkparts)):
                [pt, eta, phi, z0, charge, pdgId,trackmatches]=trkparts[itrkpart]
                if not (options.ptmin < pt < options.ptmax and options.etamin < eta < options.etamax and options.phimin < phi < options.phimax): continue
                for i in range(len(logics)):
                    if (ndof==ndoffit[i]):
                        if      parsN_1min[1][i] < track_invpt-charge/pt < parsN_1max[1][i] \
                            and parsN_1min[2][i] < track_eta-eta         < parsN_1max[2][i] \
                            and parsN_1min[3][i] < track_phi0-phi        < parsN_1max[3][i] \
                            and parsN_1min[4][i] < track_z0-z0           < parsN_1max[4][i]:
                            hname = pref[0]+parnames[0]+logics[i]
                            histos[hname].Fill(chi2Red)
                        if      parsN_1min[2][i] < track_eta-eta         < parsN_1max[2][i] \
                            and parsN_1min[3][i] < track_phi0-phi        < parsN_1max[3][i] \
                            and parsN_1min[4][i] < track_z0-z0           < parsN_1max[4][i]:
                            hname = "Delta_"+parnames[1]+logics[i]
                            histos[hname].Fill((track_invpt-charge/pt))
                        if      parsN_1min[1][i] < track_invpt-charge/pt < parsN_1max[1][i] \
                            and parsN_1min[3][i] < track_phi0-phi        < parsN_1max[3][i] \
                            and parsN_1min[4][i] < track_z0-z0           < parsN_1max[4][i]:
                            hname = "Delta_"+parnames[2]+logics[i]
                            histos[hname].Fill((track_eta-eta))
                        if      parsN_1min[1][i] < track_invpt-charge/pt < parsN_1max[1][i] \
                            and parsN_1min[2][i] < track_eta-eta         < parsN_1max[2][i] \
                            and parsN_1min[4][i] < track_z0-z0           < parsN_1max[4][i]:
                            hname = "Delta_"+parnames[3]+logics[i]
                            histos[hname].Fill((track_phi0-phi))
                        if      parsN_1min[1][i] < track_invpt-charge/pt < parsN_1max[1][i] \
                            and parsN_1min[2][i] < track_eta-eta         < parsN_1max[2][i] \
                            and parsN_1min[3][i] < track_phi0-phi        < parsN_1max[3][i]:
                            hname = "Delta_"+parnames[4]+logics[i]
                            histos[hname].Fill((track_z0-z0))
            
        if options.verbose:  print ievt, nroads5, nroads6, ncombs, ntracks, ngoods, nduplicates, nfakes
        for itrkpart in range(len(trkparts)):
            [pt, eta, phi, z0, charge, pdgId,trackmatches]=trkparts[itrkpart]
            triggerSSclean = False
            trigger = False
            for imatch in range(len(trackmatches)):
                if (not imatch): 
                    ngoods += 1
                    trigger = True
                else: nduplicates += 1
            prefix = "efficiency_"
            if options.etamin < eta < options.etamax and options.phimin < phi < options.phimax:
                histos[prefix + "pt"      ].Fill(trigger, pt)
                histos[prefix + "ppt"     ].Fill(trigger, pt)
                histos[prefix + "pppt"    ].Fill(trigger, pt)
                denomEntries += 1
                if trigger:  numEntries += 1
            if options.ptmin < pt < options.ptmax and options.phimin < phi < options.phimax:
                histos[prefix + "eta"     ].Fill(trigger, eta)
            if options.ptmin < pt < options.ptmax and options.etamin < eta < options.etamax:
                histos[prefix + "phi"     ].Fill(trigger, phi)
            if options.ptmin < pt < options.ptmax and options.etamin < eta < options.etamax and options.phimin < phi < options.phimax:
                histos[prefix + "vz"      ].Fill(trigger, vz)

        histos["nroads_5oo6_per_event"  ].Fill(nroads5)
        histos["nroads_6oo6_per_event"  ].Fill(nroads6)
        histos["nroads_per_event"       ].Fill(nroads5+nroads6)
        histos["ncombinations_per_event"].Fill(ncombs)
        histos["ntracks_per_event"      ].Fill(ntracks)
        histos["ngoods_per_event"       ].Fill(ngoods)
        histos["nduplicates_per_event"  ].Fill(nduplicates)
        histos["nfakes_per_event"       ].Fill(nfakes)
    prefix = "efficiency_"
    histos[prefix + "pt"      ].numEntries = numEntries
    histos[prefix + "pt"      ].denomEntries = denomEntries

#         print trkparts_trigger,
#         print bcolors.OKGREEN,trkparts_trigger_SSclean,bcolors.ENDC
#         print


#         ntracks_all = evt.AMTTTracks_patternRef.size()
#         trkparts_trigger = {}
#         trkparts_trigger_vars = {}

#         for itrack in xrange(ntracks_all):
#             trigger = False
#  
#             synTpId  = evt.AMTTTracks_synTpId[itrack]
#             if synTpId >= 0:
#                 trigger = True
#  
#             patternRef = evt.AMTTTracks_patternRef[itrack]
#             if not (patternRef < options.npatterns):
#                 trigger = False
#  
#             chi2     = evt.AMTTTracks_chi2[itrack]
#             ndof     = evt.AMTTTracks_ndof[itrack]
#             chi2Red  = chi2/ndof
#  
#             if not (chi2Red < options.maxChi2):
#                 trigger = False
#  
#             track_pt   = evt.AMTTTracks_pt      [itrack]
#             track_eta  = evt.AMTTTracks_eta     [itrack]
#  
#             if trigger:
#                 trkparts_trigger[synTpId] = True
#                 trkparts_trigger_vars[synTpId] = (track_pt, track_eta)
#  
#                 if options.verbose and (synTpId in trkparts):  print ievt, "track", itrack, track_pt, track_eta, repr_cppvector(evt.AMTTTracks_stubRefs[itrack])

       # ______________________________________________________________________
        # N-1 conditions

#         for k, v in trkparts.iteritems():
#             pt, eta, phi, vz, charge, pdgId = v
#             trigger = k in trkparts_trigger
#             trigger_SSclean = k in trkparts_trigger_SSclean
#  
#             prefix = "efficiency_"
#             prefix_SSclean = "efficiency_SSclean_"
#             if options.etamin < eta < options.etamax and options.phimin < phi < options.phimax:
#                 histos[prefix + "pt"      ].Fill(trigger, pt)
#                 histos[prefix + "ppt"     ].Fill(trigger, pt)
#                 histos[prefix + "pppt"    ].Fill(trigger, pt)
#                 denomEntries += 1
#                 if trigger:  numEntries += 1
#                 
#                 histos[prefix_SSclean + "pt"      ].Fill(trigger_SSclean, pt)
#                 histos[prefix_SSclean + "ppt"     ].Fill(trigger_SSclean, pt)
#                 histos[prefix_SSclean + "pppt"    ].Fill(trigger_SSclean, pt)
#                 if trigger_SSclean:  numEntries_SSclean += 1
#             if options.ptmin < pt < options.ptmax and options.phimin < phi < options.phimax:
#                 histos[prefix + "eta"     ].Fill(trigger, eta)
#                 histos[prefix_SSclean + "eta"      ].Fill(trigger_SSclean, eta)
#             if options.ptmin < pt < options.ptmax and options.etamin < eta < options.etamax:
#                 histos[prefix + "phi"     ].Fill(trigger, phi)
#                 histos[prefix_SSclean + "phi"      ].Fill(trigger_SSclean, phi)
#             if options.ptmin < pt < options.ptmax and options.etamin < eta < options.etamax and options.phimin < phi < options.phimax:
#                 histos[prefix + "vz"      ].Fill(trigger, vz)
#                 histos[prefix_SSclean + "vz"      ].Fill(trigger_SSclean, vz)
# 
# 
#     prefix = "efficiency_"
#     histos[prefix + "pt"      ].numEntries = numEntries
#     histos[prefix + "pt"      ].denomEntries = denomEntries
# 
#     prefix = "efficiency_SSclean_"
#     histos[prefix + "pt"      ].numEntries = numEntries_SSclean
#     histos[prefix + "pt"      ].denomEntries = denomEntries

    tree.SetBranchStatus("*", 1)
    # Now fill the CDF histos
#     for hname, h in histos.iteritems():
#         if hname.find("_integral")>0: # find the CDF histo
#             hname0 = hname.replace("_integral","") # get the PDF histo name
#             sum = 0.0
#             for ibin in list(reversed(range(histos[hname0].GetNbinsX()))):
# #                 print hname0, ibin+1, histos[hname0].GetBinCenter(ibin+1), histos[hname0].GetBinContent(ibin+1), sum
#                 sum += histos[hname0].GetBinContent(ibin+1)
#                 h.SetBinContent(ibin+1,sum)
#                 h.SetEntries(sum)
#             h.Scale(1.0/sum)
#             h.GetYaxis().SetRangeUser(0.1,2.0)
    return

def drawer_draw(histos, options):
    
    def displayGaus(h, ignorebins=0, scalebox=(1.,1.)):
        # Display gaussian fit parameters
        fitxmin, fitxmax = h.GetBinCenter(ignorebins+1), h.GetBinCenter(h.GetNbinsX()-1-ignorebins+1)
        r = h.Fit("gaus", "INS", "", fitxmin, fitxmax)
        p1, e1, p2, e2 = r.Parameter(1), r.ParError(1), r.Parameter(2), r.ParError(2)

        gPad.Modified(); gPad.Update()
        ps = h.FindObject("stats")
        ps.SetName("mystats")

        n = 2
        newX1NDC = ps.GetX2NDC() - (ps.GetX2NDC() - ps.GetX1NDC()) * scalebox[0]
        newY1NDC = ps.GetY2NDC() - ((ps.GetY2NDC() - ps.GetY1NDC()) / 5 * (5 + n)) * scalebox[1]
        ps.SetX1NDC(newX1NDC)
        ps.SetY1NDC(newY1NDC)

        ps.AddText("Gaus #mu = %6.4g" % (p1))
        ps.AddText("Gaus #sigma = %6.4g" % (p2))
        #ps.AddText("Gaus #mu = %6.4g #pm %6.4g" % (p1, e1))
        #ps.AddText("Gaus #sigma = %6.4g #pm %6.4g" % (p2, e2))
        h.stats = [p1, e1, p2, e2]

#         h.SetStats(0)
#         gPad.Modified(); gPad.Update()
        ps.Draw()

    def displayQuantiles(h, in_quantiles=[0.95,0.99,0.999], scalebox=(1.,1.)):
        # Display one-sided confidence intervals, a.k.a quantiles
        n = len(in_quantiles)
        in_quantiles = array('d', in_quantiles)
        quantiles = array('d', [0. for i in xrange(n)])
        h.GetQuantiles(n, quantiles, in_quantiles)
#         print in_quantiles, quantiles
#         getQuantilesWithOverflow(h,quantiles,in_quantiles)
        gPad.Modified(); gPad.Update()
        ps = h.FindObject("stats")
        ps.SetName("mystats")

        newX1NDC = ps.GetX2NDC() - (ps.GetX2NDC() - ps.GetX1NDC()) * scalebox[0]
        newY1NDC = ps.GetY2NDC() - ((ps.GetY2NDC() - ps.GetY1NDC()) / 5 * (5 + n)) * scalebox[1]
        ps.SetX1NDC(newX1NDC)
        ps.SetY1NDC(newY1NDC)

        for iq, q in enumerate(in_quantiles):
            ps.AddText("%4.1f%% CI = %6.4g" % ((q*100), quantiles[iq]))
        h.stats = [h.GetMean()] + quantiles.tolist()

#         h.SetStats(0)
        #gPad.Modified(); gPad.Update()
        ps.Draw()
        print h.GetName(), in_quantiles, quantiles
    
    for hname, h in histos.iteritems():

        h.additional = []
        if h.ClassName() == "TEfficiency":
            ymax = 1.2

            h1 = h.GetCopyTotalHisto(); h1.SetName(h1.GetName()+"_frame"); h1.Reset()
            h1.SetMinimum(0); h1.SetMaximum(ymax)
            h1.SetStats(0); h1.Draw()

            # Reference lines for 0.9, 0.95 and 1.0
            xmin, xmax = h1.GetXaxis().GetXmin(), h1.GetXaxis().GetXmax()
            for y in [0.5, 0.8, 0.9, 0.95, 1.0]:
                tline.DrawLine(xmin, y, xmax, y)

            h.gr = h.CreateGraph()
            h.gr.Draw("p")

            h.additional += [h.GetCopyPassedHisto(), h.GetCopyTotalHisto()]

        elif h.ClassName() == "TH1F":
            ymax = h.GetMaximum() * 1.4
            h.SetMinimum(0); h.SetMaximum(ymax)

            h.Draw("hist")
            if (h.GetName().find("Delta")>=0 and h.GetName().find("SSclean")==-1):
                displayQuantiles(h,[0.0005,0.005,0.995,0.9995])
#                 displayGaus(h, ignorebins=10)

        elif h.ClassName() == "TH2F":
            ymax = 0.05
            nbinsx = h.GetNbinsX()
            gr1 = TGraphAsymmErrors(nbinsx)
            gr1.SetName(hname + "_gr1"); setattr(h, "gr1", gr1)
            gr1.SetMarkerSize(1); gr1.SetMarkerColor(col_res)
            gr2 = TGraphAsymmErrors(nbinsx)
            gr2.SetName(hname + "_gr2"); setattr(h, "gr2", gr2)
            gr2.SetMarkerSize(1); gr2.SetMarkerColor(col_res)

            for ibin in xrange(nbinsx):
                hpy = h.ProjectionY(hname + "_py%i" % ibin, ibin+1, ibin+1); setattr(h, "py%i" % ibin, hpy)
                x = h.GetXaxis().GetBinCenter(ibin+1)

                if hpy.GetEntries() < 30:
                    print "WARNING: not enough entries in %s: %i" % (hpy.GetName(), hpy.GetEntries())
                    gr1.SetPoint(ibin, x, 999999)
                    gr2.SetPoint(ibin, x, 999999)
                    continue

                if hpy.GetEntries() < 100:
                    print "INFO: rebinning in %s: %i" % (hpy.GetName(), hpy.GetEntries())
                    hpy.Rebin(2)

                assert(hpy.GetNbinsX() > 20)
                fitxmin, fitxmax = hpy.GetBinCenter(10+1), hpy.GetBinCenter(hpy.GetNbinsX()-1-10+1)
                r = hpy.Fit("gaus", "INS", "", fitxmin, fitxmax)
                p1, e1, p2, e2 = r.Parameter(1), r.ParError(1), r.Parameter(2), r.ParError(2)
                gr1.SetPoint(ibin, x, p1)
                gr1.SetPointError(ibin, 0, 0, e1, e1)
                gr2.SetPoint(ibin, x, p2)
                gr2.SetPointError(ibin, 0, 0, e2, e2)

            hpx2 = h.ProjectionX(hname + "_px2"); hpx2.Reset(); setattr(h, "px2", hpx2)
            hpx2.SetYTitle("p_{T} resolution/p_{T}")
            hpx2.SetMinimum(0); hpx2.SetMaximum(ymax)
            hpx2.SetStats(0); hpx2.Draw()
            gr2.Draw("p")

        CMS_label()
        save(options.outdir, "%s_%s" % (hname, options.outstring), dot_root=False, dot_pdf=False, additional=h.additional)

    def find_le(a, x):
        'Find rightmost value less than or equal to x'
        i = bisect_right(a, x)
        if i:
            return a[i-1]
        raise ValueError


#     def getQuantilesWithOverflow(h,q,in_q):
#         assert(len(q)==len(in_q))
#         assert(h.ClassName().find("TH1")>=0)
#         nbins = h.GetXaxis().GetNbins()
#         hCDF = [0. for i in range(nbins+3)]
#         for i in range(len(hCDF)-1):
#             hCDF[i+1]=hCDF[i]+h.GetBinContent(nbins+1-i)
#         
#         hsum=hCDF[nbins+2]
#         hCDF[:] = [x / hsum for x in hCDF]
#         print hsum, hCDF[0],hCDF[1],hCDF[2],hCDF[3],hCDF[-4],hCDF[-3],hCDF[-2],hCDF[-1]
#         for quantile, in_quantile in zip(q,in_q):
#             lowxq = bisect_left(hCDF, 1-in_quantile)
#             print 1-in_quantile, lowxq
# #             quantile=(in_quantile-hCDF[lowxq])/(hCDF[lowxq+1]-hCDF[lowxq])+lowxq
#         return
    

    options.logy = True
    for hname, h in histos.iteritems():
        if h.ClassName() == "TEfficiency": continue
        if options.logy:
            h.SetMaximum(h.GetMaximum() * 1.4); h.SetMinimum(0.5)
            if hname.find("_integral")>0:
                h.SetMinimum(0.2/h.GetEntries())
        else:
            h.SetMaximum(h.GetMaximum() * 1.4); h.SetMinimum(0.)

        h.Draw("hist")
        gPad.SetLogy(options.logy)
        if hname.endswith("_per_event"):
            displayQuantiles(h)

        CMS_label()
        save(options.outdir, "%s_%s" % (hname, options.outstring), dot_root=False, dot_pdf=False)
    return

def drawer_draw2(histos, options):

    # Specialized
    options.logy = False
    for v in ["pt"]:
        hname1 = "%s_good" % v
        hname2 = "%s_duplicate" % v
        hname3 = "%s_fake" % v

        h1 = histos[hname1]
        h2 = histos[hname2]
        h3 = histos[hname3]

        h1.SetLineColor(col1); h1.SetFillColor(fcol1)
        h2.SetLineColor(col2); h2.SetFillColor(fcol2)
        h3.SetLineColor(col3); h3.SetFillColor(fcol3)

        # Stack
        hstack1 = h1.Clone(hname1 + "_stack")
        hstack2 = h2.Clone(hname2 + "_stack")
        hstack3 = h3.Clone(hname3 + "_stack")

        hstack2.Add(hstack1)
        hstack3.Add(hstack2)

        hstack3.SetMaximum(hstack3.GetMaximum()*1.4); hstack3.SetMinimum(0)
        hstack3.SetStats(0); hstack3.Draw("hist")
        hstack2.Draw("hist same")
        hstack1.Draw("hist same")
        gPad.SetLogy(options.logy)

        moveLegend(0.64,0.82,0.94,0.94); tlegend.Clear()
        tlegend.AddEntry(h1, "good", "f")
        tlegend.AddEntry(h2, "duplicate", "f")
        tlegend.AddEntry(h3, "fake", "f")
        tlegend.Draw()

        CMS_label()
        save(options.outdir, "%s_stack_%s" % (v, options.outstring), dot_root=False, dot_pdf=False)

        # Ratio
        hratio1 = h1.Clone(hname1 + "_ratio")
        hratio2 = h2.Clone(hname2 + "_ratio")
        hratio3 = h3.Clone(hname3 + "_ratio")

        hratio1.Divide(hstack3)
        hratio2.Divide(hstack3)
        hratio3.Divide(hstack3)

        hratio1.SetFillStyle(0)
        hratio2.SetFillStyle(0)
        hratio3.SetFillStyle(0)

        hratio3.SetMaximum(1.1); hratio3.SetMinimum(0)
        hratio3.SetStats(0); hratio3.Draw("hist")
        hratio2.SetMaximum(1.1); hratio2.SetMinimum(0)
        hratio2.Draw("hist same")
        hratio1.SetMaximum(1.1); hratio1.SetMinimum(0)
        hratio1.Draw("hist same")
        gPad.SetLogy(options.logy)

        moveLegend(0.64,0.82,0.94,0.94); tlegend.Clear()
        tlegend.AddEntry(h1, "good", "l")
        tlegend.AddEntry(h2, "duplicate", "l")
        tlegend.AddEntry(h3, "fake", "l")
        tlegend.Draw()

        CMS_label()
        save(options.outdir, "%s_ratio_%s" % (v, options.ss), dot_root=False, dot_pdf=False)

        # Norm
        hnorm1 = h1.Clone(hname1 + "_norm")
        hnorm2 = h2.Clone(hname2 + "_norm")
        hnorm3 = h3.Clone(hname3 + "_norm")

        hnorm1.Scale(1.0/hnorm1.Integral())
        hnorm2.Scale(1.0/hnorm2.Integral())
        hnorm3.Scale(1.0/hnorm3.Integral())

        hnorm1.SetFillStyle(0)
        hnorm2.SetFillStyle(0)
        hnorm3.SetFillStyle(0)

        ymax = max(hnorm1.GetMaximum(), hnorm2.GetMaximum(), hnorm3.GetMaximum())
        hnorm3.SetMaximum(ymax*1.4); hnorm3.SetMinimum(0)
        hnorm3.SetStats(0); hnorm3.Draw("hist")
        hnorm2.Draw("hist same")
        hnorm1.Draw("hist same")
        gPad.SetLogy(options.logy)

        moveLegend(0.64,0.82,0.94,0.94); tlegend.Clear()
        tlegend.AddEntry(h1, "good", "l")
        tlegend.AddEntry(h2, "duplicate", "l")
        tlegend.AddEntry(h3, "fake", "l")
        tlegend.Draw()

        CMS_label()
        save(options.outdir, "%s_norm_%s" % (v, options.outstring), dot_pdf=False, dot_root=False)

        donotdelete.append([hstack1, hstack2, hstack3])
        donotdelete.append([hratio1, hratio2, hratio3])
        donotdelete.append([hnorm1, hnorm2, hnorm3])
    return

def drawer_sitrep(histos, options):
    print "--- SITREP ---------------------------------------------------------"
    print "--- Using tt{0}, pu{1}, ss={2}, npatterns={3}".format(options.tower, options.pu, options.ss, options.npatterns)
    print "--- Variable, mean, 95%% CI, 99%% CI:"
    h = histos["nroads_per_event"]
    print "nroads per event\t{0:6.4g}\t{1:6.4g}\t{2:6.4g}".format(*h.stats)
    h = histos["ncombinations_per_event"]
    print "ncombs per event\t{0:6.4g}\t{1:6.4g}\t{2:6.4g}".format(*h.stats)
    h = histos["ntracks_per_event"]
    print "ntrks  per event\t{0:6.4g}\t{1:6.4g}\t{2:6.4g}".format(*h.stats)
    h = histos["ngoods_per_event"]
    print "ngoods per event\t{0:6.4g}\t{1:6.4g}\t{2:6.4g}".format(*h.stats)
    h = histos["nduplicates_per_event"]
    print "ndupls per event\t{0:6.4g}\t{1:6.4g}\t{2:6.4g}".format(*h.stats)
    h = histos["nfakes_per_event"]
    print "nfakes per event\t{0:6.4g}\t{1:6.4g}\t{2:6.4g}".format(*h.stats)
    prefix = "efficiency_"
    h = histos[prefix + "pt"]
    print "eff        ={0}/{1}={2:.3f}".format(h.numEntries, h.denomEntries, float(h.numEntries)/h.denomEntries)
    prefix = "efficiency_SSclean_"
    h = histos[prefix + "pt"]
    print "eff_SSclean={0}/{1}={2:.3f}".format(h.numEntries, h.denomEntries, float(h.numEntries)/h.denomEntries)


# ______________________________________________________________________________
# Main function
def main():

    # Setup argument parser
    parser = argparse.ArgumentParser()

    # Add default arguments
    add_drawer_arguments(parser)

    # Add more arguments
    parser.add_argument("inpatternfile", help="input pattern bank file (a .root file)")
    parser.add_argument("ss", help="short name of superstrip definition (e.g. ss256)")
    parser.add_argument("npatterns", type=int, help="number of patterns to reach the desired coverage")
    parser.add_argument("--distance", type=int, default=4, help="min # of layers with same SS needed for merging (default: %(default)s)")
    parser.add_argument("--coverage", type=float, default=0.95, help="desired coverage (default: %(default)s)")
    parser.add_argument("--minPt", type=float, default=3, help="min pT for gen particle (default: %(default)s)")
    parser.add_argument("--maxChi2", type=float, default=5, help="max reduced chi-squared (default: %(default)s)")
    parser.add_argument("--xScale", type=float, default=1, help="scale factor for the x-axis range (default: %(default)s)")
    parser.add_argument("--outstring", type=str, help="string for output file labelling (default: %(default)s)")

    # Parse default arguments
    options = parser.parse_args()
    parse_drawer_options(options)
    options.xscale = options.xScale

    # Init
    drawerInit = DrawerInit()
    tchain = TChain("ntupler/tree", "")
    tchain.AddFileInfoList(options.tfilecoll.GetList())
    if options.inpatternfile.endswith(".root"):
        # Create a temporary file with one line
        with tempfile.NamedTemporaryFile() as infile:
            infile.write(options.inpatternfile)
            infile.flush()
            options.tfilepatterncoll = TFileCollection("fc", "", infile.name)
    else:
        raise ValueError("inpatternfile must be .root file")
        
    tchainPatt = TChain("patternBank", "")
    tchainPatt.AddFileInfoList(options.tfilepatterncoll.GetList())
    tchainPattA = TChain("patternAttributes", "")
    tchainPattA.AddFileInfoList(options.tfilepatterncoll.GetList())

    use_TEfficiency()

    # Process
    options.ptmin = options.minPt
    histos = drawer_book(options)
    drawer_project(tchain, tchainPatt, tchainPattA, histos, options)
    drawer_draw(histos, options)
#     drawer_draw2(histos, options)
#     drawer_sitrep(histos, options)


# ______________________________________________________________________________
if __name__ == '__main__':

    # Call the main function
    main()
