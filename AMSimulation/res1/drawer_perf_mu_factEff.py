#!/usr/bin/env python

from rootdrawing import *
from parser import *
import math
from array import array

col = TColor.GetColor("#BA0000")
fcol = TColor.GetColor("#E58080")


# ______________________________________________________________________________
factorized = [
    "00TP",
    "01det",
    "02digi",
    "03clus",
    "04stub",
    "05cable",
    "06tower",
    "07road",
    "08comb",
    "09track",
    "10dedup",
]

factorized_plots=[factorized[0],factorized[7],factorized[10]]

#parnames = ["#phi", "cot #theta", "z_{0} [cm]", "1/p_{T} [1/GeV]"]
nparameters = 4
parnames = ["p_{T} [GeV/c]", "p_{T} [GeV/c]", "p_{T} [GeV/c]", "#eta"]
varRanges= [[0,20],[0,200],[0,2000],[-3.0,3.0]]
varnames = ["Pt_%d_%d" % (varRanges[0][0],varRanges[0][1]),"Pt_%d_%d" % (varRanges[1][0],varRanges[1][1]),"Pt_%d_%d" % (varRanges[2][0],varRanges[2][1]),"Eta"]
# nvariables = 12

# parrmses = [0.000453, 0.002403, 0.083406, 0.001283]
parrmses = [0.001283, 0.002403, 0.000453, 0.083406]


# ______________________________________________________________________________
# Load trigger tower map
ttmap = json.load(open("../data/trigger_sector_map.json"), object_pairs_hook=convert_key_to_int)
ttrmap = get_reverse_map(ttmap)

def use_TEfficiency():
    del ROOT.TEfficiency
    gROOT.ProcessLine(".L ./fixes/TEfficiency.cxx+")
    #gSystem.Load("./fixes/TEfficiency_cxx.so")

    global TEfficiency
    TEfficiency = ROOT.TEfficiency


# ______________________________________________________________________________
def drawer_book(options):
    histos = {}

    prefix = "efficiency_"
    ytitle = "#varepsilon_{factorized}"

    nPTs = 3
    varPtName = ["pt","ppt","pppt"]
    upperPt   = [500.,50.,10.]

    for fact in factorized_plots:
        for iPt in range(nPTs):
            hname = prefix + ("%s_" % fact) + varPtName[iPt]  # pt
            histos[hname] = TH1F(hname, "; p_{T} [GeV]; %s" % ytitle, 100, 0., upperPt[iPt])

        hname = prefix + ("%s_" % fact) + "eta"
        histos[hname] = TH1F(hname, "; #eta; %s" % ytitle,        120, -3.0, 3.0)

        hname = prefix + ("%s_" % fact) + "phi"
        histos[hname] = TH1F(hname, "; #phi; %s" % ytitle,        128, -3.2, 3.2)

        hname = prefix + ("%s_" % fact) + "vz"
        histos[hname] = TH1F(hname, "; v_{z} [cm]; %s" % ytitle,  120, -30, 30)

#         hname = prefix + ("%s_" % fact) + "charge"
#         histos[hname] = TH1F(hname, "; charge; %s" % ytitle,      5, -2.5, 2.5)

    for i in xrange(nparameters):
        hname = "errpar%i" % i
        histos[hname] = TH2F(hname, "#Deltap_{T}_p_{T}; %s" % parnames[i], 100, varRanges[i][0], varRanges[i][1], 100, -0.5, 0.5)
        histos[hname].rms = parrmses[i]

    # Style
    for hname, h in histos.iteritems():
        h.SetLineWidth(2); h.SetMarkerSize(0)
        h.SetMarkerStyle(24); h.SetMarkerSize(1)
        h.SetMarkerColor(col); h.SetLineColor(col); h.SetFillColor(fcol)
    donotdelete.append(histos)
    return histos

def repr_cppvector(v):
    s = ",".join([str(x) for x in v])
    s = "[" + s + "]"
    return s

def drawer_project(tree, histos, options):
    moduleIds_tt = set()
    for moduleId in ttmap[options.tower]:
        moduleIds_tt.add(moduleId)

    class CombinationBuilder:
        '''Super unintelligent combination builder'''
        def __init__(self):
            self._map = {
                5: 99,
                6: 99,
                7: 99,
                8: 99,
                9: 99,
                10: 99,
            }
            self._fun = lambda x: 0
        def is_good(self, lay, ds, patternRef):
            return abs(ds - self._fun(patternRef)) < self._map[lay]


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

    tree.SetBranchStatus("mixedSimHits_tpId" , 1)
    tree.SetBranchStatus("mixedSimHits_modId", 1)
    tree.SetBranchStatus("mixedSimHits_outer", 1)

    tree.SetBranchStatus("pixelDigis_tpIds"     , 1)
    tree.SetBranchStatus("pixelDigis_modId"     , 1)
    tree.SetBranchStatus("pixelDigis_outer"     , 1)
    tree.SetBranchStatus("pixelDigis_simHitRefs", 1)
    tree.SetBranchStatus("pixelDigis_fractions" , 1)

    tree.SetBranchStatus("TTClusters_tpIds"   , 1)
    tree.SetBranchStatus("TTClusters_modId"   , 1)
    tree.SetBranchStatus("TTClusters_outer"   , 1)
    tree.SetBranchStatus("TTClusters_digiRefs", 1)

    tree.SetBranchStatus("TTStubs_tpIds"   , 1)
    tree.SetBranchStatus("TTStubs_modId"   , 1)
    tree.SetBranchStatus("TTStubs_trigBend", 1)
    tree.SetBranchStatus("TTStubs_clusRef0", 1)
    tree.SetBranchStatus("TTStubs_clusRef1", 1)

    tree.SetBranchStatus("AMTTRoads_patternRef" , 1)
    tree.SetBranchStatus("AMTTRoads_stubRefs"   , 1)
    tree.SetBranchStatus("AMTTTracks_patternRef", 1)
    tree.SetBranchStatus("AMTTTracks_stubRefs"  , 1)
    tree.SetBranchStatus("AMTTTracks_isGhost"   , 1)

    tree.SetBranchStatus("AMTTTracks_invPt"     , 1)
    tree.SetBranchStatus("AMTTTracks_phi0"      , 1)
    tree.SetBranchStatus("AMTTTracks_cottheta"  , 1)
    tree.SetBranchStatus("AMTTTracks_z0"        , 1)
    tree.SetBranchStatus("AMTTTracks_chi2"      , 1)
    tree.SetBranchStatus("AMTTTracks_ndof"      , 1)
    tree.SetBranchStatus("AMTTTracks_synTpId"   , 1)

    def absDiff(lhs, rhs):
        return abs(lhs - rhs)

    def squaredNormDiff(lhs, rhs, scale):
        return ((lhs - rhs)/scale)**2

    trkparts_allevts = []
    tracks_allevts = []


    # Loop over events
    numEntries, denomEntries = 0, 0

    for ievt, evt in enumerate(tree):
        if (ievt == options.nentries):  break

        if (ievt % 1000 == 0):  print "Processing event: %i" % ievt

        trkparts_ievt_fact = {}
        for fact in factorized:
            trkparts_ievt_fact[fact] = []

        nparts_all = evt.trkParts_primary.size()
        trkparts = {}
        trkparts_modules = {}
        trkparts_simhits = {}
        trkparts_digis = {}
        trkparts_cluss = {}
        trkparts_stubs = {}

        for ipart in xrange(nparts_all):
            if options.pu == 0:  # single-track events
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

            trkparts[ipart] = (pt, eta, phi, vz, charge, pdgId)
            trkparts_modules[ipart] = []
            trkparts_simhits[ipart] = []
            trkparts_digis[ipart] = []
            trkparts_cluss[ipart] = []
            trkparts_stubs[ipart] = []
            if options.verbose:  print ievt, "part ", ipart, trkparts[ipart]

        for ipart, part in trkparts.iteritems():
            trkparts_ievt_fact["00TP"].append(part)

        # ______________________________________________________________________
        # Sim hits

        nsimhits_all = evt.mixedSimHits_modId.size()
        simhits = {}

        # Find sim hits from the primary tracking particles
        for isimhit in xrange(nsimhits_all):
            tpId     = evt.mixedSimHits_tpId [isimhit]

            if tpId not in trkparts:
                continue

            moduleId = evt.mixedSimHits_modId[isimhit]
            outer    = evt.mixedSimHits_outer[isimhit]
            outer    = int(outer)                       # bool. is outer hit of the (willbe) stub

            if options.verbose:  print ievt, "simhit ", isimhit, (tpId, moduleId, outer)

            if tpId not in simhits:
                simhits[tpId] = {}

            if moduleId not in simhits[tpId]:
                simhits[tpId][moduleId] = {}

            if outer not in simhits[tpId][moduleId]:
                simhits[tpId][moduleId][outer] = []

            simhits[tpId][moduleId][outer] += [isimhit]

        # Require at least 5 modules, in different layers (5-10), each module containing
        # at least 2 sim hits, in different sensors (0-1)
        for tpId, m in simhits.iteritems():

            layers = set()
            for moduleId, mm in m.iteritems():
                lay = decodeLayer(moduleId)

                if 5 <= lay <= 10 and (0 in mm) and (1 in mm):
                    if options.verbose:  print ievt, "-- det: ", tpId, moduleId, mm.keys()
                    layers.add(lay)

                    trkparts_modules[tpId] += [moduleId]
                    trkparts_simhits[tpId] += mm[0]
                    trkparts_simhits[tpId] += mm[1]

            if len(layers) >= options.minNLayers:
                if options.verbose:  print ievt, "-- det: >>", len(layers)
                trkparts_ievt_fact["01det"].append(trkparts[tpId])
                trkparts_modules[tpId].sort()
                trkparts_simhits[tpId].sort()

            if options.verbose:  print ievt, "-- det: >>", len(trkparts_modules[tpId]), trkparts_modules[tpId]
            if options.verbose:  print ievt, "-- det: >>", len(trkparts_simhits[tpId]), trkparts_simhits[tpId]


        # ______________________________________________________________________
        # Digis

        ndigis_all = evt.pixelDigis_modId.size()
        digis = {}

        # Find digis due to the sim hits from the primary tracking particles
        for idigi in xrange(ndigis_all):
            tpIds      = evt.pixelDigis_tpIds[idigi]
            simHitRefs = evt.pixelDigis_simHitRefs[idigi]
            fractions  = evt.pixelDigis_fractions[idigi]

            temp_tpIds = []
            for tpId, simHitRef, fraction in izip(tpIds, simHitRefs, fractions):
                #print tpId, simHitRef, evt.mixedSimHits_tpId[simHitRef]
                if tpId in trkparts and tpId in trkparts_simhits and fraction > options.minDigiFraction:
                    if simHitRef in trkparts_simhits[tpId]:
                        temp_tpIds.append(tpId)
#                         histos["digiFraction"].Fill(fraction)
            tpIds = temp_tpIds
            if not tpIds:
                continue

            moduleId = evt.pixelDigis_modId[idigi]
            outer    = evt.pixelDigis_outer[idigi]
            outer    = int(outer)

            if options.verbose:  print ievt, "digi ", idigi, (tpIds, moduleId, outer)

            for tpId in tpIds:
                if tpId not in digis:
                    digis[tpId] = {}

                if moduleId not in digis[tpId]:
                    digis[tpId][moduleId] = {}

                if outer not in digis[tpId][moduleId]:
                    digis[tpId][moduleId][outer] = []

                digis[tpId][moduleId][outer] += [idigi]

        # Require at least 5 modules, in different layers (5-10), each module containing
        # at least 2 sim hits and 2 corresponding digis, in different sensors (0-1)
        for tpId, m in digis.iteritems():
            temp_modules = []

            layers = set()
            for moduleId, mm in m.iteritems():
                lay = decodeLayer(moduleId)

                if moduleId in trkparts_modules[tpId] and (0 in mm) and (1 in mm):
                    if options.verbose:  print ievt, "-- digi: ", tpId, moduleId, mm.keys()
                    layers.add(lay)

                    temp_modules += [moduleId]
                    trkparts_digis[tpId] += mm[0]
                    trkparts_digis[tpId] += mm[1]

            trkparts_modules[tpId] = temp_modules

            if len(layers) >= options.minNLayers:
                if options.verbose:  print ievt, "-- digi: >>", len(layers)
                trkparts_ievt_fact["02digi"].append(trkparts[tpId])
                trkparts_modules[tpId].sort()
                trkparts_digis[tpId].sort()

            if options.verbose:  print ievt, "-- digi: >>", len(trkparts_modules[tpId]), trkparts_modules[tpId]
            if options.verbose:  print ievt, "-- digi: >>", len(trkparts_digis[tpId]), trkparts_digis[tpId]


        # ______________________________________________________________________
        # Clusters

        ncluss_all = evt.TTClusters_modId.size()
        cluss = {}

        # Find clusters that contain the digis due to the sim hits from the primary tracking particles
        for iclus in xrange(ncluss_all):
            tpIds    = evt.TTClusters_tpIds[iclus]
            digiRefs = evt.TTClusters_digiRefs[iclus]

            temp_tpIds = []
            for digiRef in digiRefs:
                for tpId, m in trkparts_digis.iteritems():
                    if digiRef in m:
                        temp_tpIds.append(tpId)
            temp_tpIds = list(set(temp_tpIds))
            #print temp_tpIds, repr_cppvector(tpIds), repr_cppvector(digiRefs)
            #assert([tpId for tpId in tpIds if tpId != -1] == temp_tpIds)  # Not true
            tpIds = temp_tpIds
            if not tpIds:
                continue

            moduleId = evt.TTClusters_modId[iclus]
            outer    = evt.TTClusters_outer[iclus]
            outer    = int(outer)

            if options.verbose:  print ievt, "clus ", iclus, (tpIds, moduleId, outer)

            for tpId in tpIds:
                if tpId not in cluss:
                    cluss[tpId] = {}

                if moduleId not in cluss[tpId]:
                    cluss[tpId][moduleId] = {}

                if outer not in cluss[tpId][moduleId]:
                    cluss[tpId][moduleId][outer] = []

                cluss[tpId][moduleId][outer] += [iclus]

        # Require at least 5 modules, in different layers (5-10), each module containing
        # at least 2 sim hits, 2 corresponding digis and 2 corresponding clusters, in different sensors (0-1)
        for tpId, m in cluss.iteritems():
            temp_modules = []

            layers = set()
            for moduleId, mm in m.iteritems():
                lay = decodeLayer(moduleId)

                if moduleId in trkparts_modules[tpId] and (0 in mm) and (1 in mm):
                    if options.verbose:  print ievt, "-- clus: ", tpId, moduleId, mm.keys()
                    layers.add(lay)

                    temp_modules += [moduleId]
                    trkparts_cluss[tpId] += mm[0]
                    trkparts_cluss[tpId] += mm[1]

            trkparts_modules[tpId] = temp_modules

            if len(layers) >= options.minNLayers:
                if options.verbose:  print ievt, "-- clus: >>", len(layers)
                trkparts_ievt_fact["03clus"].append(trkparts[tpId])
                trkparts_modules[tpId].sort()
                trkparts_cluss[tpId].sort()

            if options.verbose:  print ievt, "-- clus: >>", len(trkparts_modules[tpId]), trkparts_modules[tpId]
            if options.verbose:  print ievt, "-- clus: >>", len(trkparts_cluss[tpId]), trkparts_cluss[tpId]


        # ______________________________________________________________________
        # Stubs

        nstubs_all = evt.TTStubs_modId.size()
        stubs = {}

        # Find stubs that are a pair of clusters that contain the digis due to the sim hits from the primary tracking particles
        for istub in xrange(nstubs_all):
            tpIds    = evt.TTStubs_tpIds[istub]
            clusRef0 = evt.TTStubs_clusRef0[istub]
            clusRef1 = evt.TTStubs_clusRef1[istub]

            temp_tpIds = []
            for tpId, m in trkparts_cluss.iteritems():
                if clusRef0 in m and clusRef1 in m:
                    temp_tpIds.append(tpId)
            temp_tpIds = list(set(temp_tpIds))
            #print temp_tpIds, repr_cppvector(tpIds), clusRef0, clusRef1
            #assert([tpId for tpId in tpIds if tpId != -1] == temp_tpIds)  # Not true
            tpIds = temp_tpIds
            if not tpIds:
                continue

            moduleId = evt.TTStubs_modId[istub]

            if options.verbose:  print ievt, "stub ", istub, (tpIds, moduleId, clusRef0, clusRef1)

            for tpId in tpIds:
                if tpId not in stubs:
                    stubs[tpId] = {}

                if moduleId not in stubs[tpId]:
                    stubs[tpId][moduleId] = []

                stubs[tpId][moduleId] += [istub]

        # Require at least 5 modules, in different layers (5-10), each module containing
        # at least 2 sim hits, 2 corresponding digis, 2 corresponding clusters, and 1 corresponding stub
        for tpId, m in stubs.iteritems():
            temp_modules = []

            layers = set()
            for moduleId, v in m.iteritems():
                lay = decodeLayer(moduleId)

                if moduleId in trkparts_modules[tpId]:
                    if options.verbose:  print ievt, "-- stub: ", tpId, moduleId, v
                    layers.add(lay)

                    temp_modules += [moduleId]
                    trkparts_stubs[tpId] += v

            trkparts_modules[tpId] = temp_modules

            if len(layers) >= options.minNLayers:
                if options.verbose:  print ievt, "-- stub: >>", len(layers)
                trkparts_ievt_fact["04stub"].append(trkparts[tpId])
                trkparts_modules[tpId].sort()
                trkparts_stubs[tpId].sort()

            if options.verbose:  print ievt, "-- stub: >>", len(trkparts_modules[tpId]), trkparts_modules[tpId]
            if options.verbose:  print ievt, "-- stub: >>", len(trkparts_stubs[tpId]), trkparts_stubs[tpId]


        # ______________________________________________________________________
        # Cable
        #  - Find the subset of stubs transmitted successfully by cables, repeat the same thing
        #  - NOT IMPLEMENTED

        for part in trkparts_ievt_fact["04stub"]:
            trkparts_ievt_fact["05cable"].append(part)

        # ______________________________________________________________________
        # Tower
        #  - Find the subset of stubs in this trigger tower, repeat the same thing

        for tpId, v in trkparts_stubs.iteritems():
            temp_modules = []
            temp_stubs = []

            layers = set()
            for istub in v:
                moduleId = evt.TTStubs_modId[istub]
                lay = decodeLayer(moduleId)

                if moduleId in moduleIds_tt:
                    if options.verbose:  print ievt, "-- tower: ", tpId, moduleId, v
                    layers.add(lay)

                    temp_modules += [moduleId]
                    temp_stubs += [istub]

            trkparts_modules[tpId] = temp_modules
            trkparts_stubs[tpId] = temp_stubs

            if len(layers) >= options.minNLayers:
                if options.verbose:  print ievt, "-- tower: >>", len(layers)
                trkparts_ievt_fact["06tower"].append(trkparts[tpId])

            if options.verbose:  print ievt, "-- tower: >>", len(trkparts_modules[tpId]), trkparts_modules[tpId]
            if options.verbose:  print ievt, "-- tower: >>", len(trkparts_stubs[tpId]), trkparts_stubs[tpId]


        # ______________________________________________________________________
        # Road + Combination
        # - Find at least a road (or a combination) that have those stubs

        nroads_all = evt.AMTTRoads_patternRef.size()

        dummyCB = CombinationBuilder()

        for tpId, v in trkparts_stubs.iteritems():
            trigger = False

            for iroad in xrange(nroads_all):
                patternRef = evt.AMTTRoads_patternRef[iroad]
                if not (patternRef < options.npatterns):
                    continue

                stubRefs_iroad = evt.AMTTRoads_stubRefs[iroad]

                layers = set()
                assert(stubRefs_iroad.size() == 6)
                for isuperstrip in xrange(6):
                    stubRefs_isuperstrip = stubRefs_iroad[isuperstrip]
                    for istub in stubRefs_isuperstrip:
                        moduleId = evt.TTStubs_modId[istub]
                        lay = decodeLayer(moduleId)

                        if istub in v:
                            if options.verbose:  print ievt, "-- road: ", tpId, moduleId, istub
                            layers.add(lay)

                if len(layers) >= options.minNLayers:
                    if options.verbose:  print ievt, "-- road: >>", len(layers)
                    trigger = True
                    break

            if trigger:
                trkparts_ievt_fact["07road"].append(trkparts[tpId])


            trigger = False

            for iroad in xrange(nroads_all):
                patternRef = evt.AMTTRoads_patternRef[iroad]
                if not (patternRef < options.npatterns):
                    continue

                stubRefs_iroad = evt.AMTTRoads_stubRefs[iroad]

                layers = set()
                assert(stubRefs_iroad.size() == 6)
                for isuperstrip in xrange(6):
                    stubRefs_isuperstrip = stubRefs_iroad[isuperstrip]
                    for istub in stubRefs_isuperstrip:
                        moduleId = evt.TTStubs_modId[istub]
                        ds = evt.TTStubs_trigBend[istub]
                        lay = decodeLayer(moduleId)

                        if not dummyCB.is_good(lay, ds, patternRef):
                            continue

                        if istub in v:
                            if options.verbose:  print ievt, "-- comb: ", tpId, moduleId, istub
                            layers.add(lay)

                if len(layers) >= options.minNLayers:
                    if options.verbose:  print ievt, "-- comb: >>", len(layers)
                    trigger = True
                    break

            if trigger:
                trkparts_ievt_fact["08comb"].append(trkparts[tpId])


        # ______________________________________________________________________
        # Track + deduplication
        # - Find at least a track (before or after duplicate removal) that have those stubs

        ntracks_all = evt.AMTTTracks_patternRef.size()

        for tpId, v in trkparts_stubs.iteritems():
            trigger = False

            for itrack in xrange(ntracks_all):
                patternRef = evt.AMTTTracks_patternRef[itrack]
                if not (patternRef < options.npatterns):
                    continue

                stubRefs_itrack = evt.AMTTTracks_stubRefs[itrack]

                layers = set()
                assert(stubRefs_itrack.size() == 6)
                for istub in stubRefs_itrack:
                    if istub == 999999999:
                        continue

                    moduleId = evt.TTStubs_modId[istub]
                    lay = decodeLayer(moduleId)

                    if istub in v:
                        if options.verbose:  print ievt, "-- track: ", tpId, moduleId, istub
                        layers.add(lay)

                if len(layers) >= options.minNLayers:
                    if options.verbose:  print ievt, "-- track: >>", len(layers)
                    trigger = True
                    break

            if trigger:
                trkparts_ievt_fact["09track"].append(trkparts[tpId])


            trigger = False

            for itrack in xrange(ntracks_all):
                patternRef = evt.AMTTTracks_patternRef[itrack]
                if not (patternRef < options.npatterns):
                    continue

                stubRefs_itrack = evt.AMTTTracks_stubRefs[itrack]
                isGhost = evt.AMTTTracks_isGhost[itrack]

                if isGhost:
                    continue

                layers = set()
                assert(stubRefs_itrack.size() == 6)
                for istub in stubRefs_itrack:
                    if istub == 999999999:
                        continue

                    moduleId = evt.TTStubs_modId[istub]
                    lay = decodeLayer(moduleId)

                    if istub in v:
                        if options.verbose:  print ievt, "-- dedup: ", tpId, moduleId, istub
                        layers.add(lay)

                if len(layers) >= options.minNLayers:
                    if options.verbose:  print ievt, "-- dedup: >>", len(layers)
                    trigger = True
                    break

            if trigger:
                trkparts_ievt_fact["10dedup"].append(trkparts[tpId])


        # ______________________________________________________________________
        tracks = {}  
        for itrack in xrange(ntracks_all):
            patternRef = evt.AMTTTracks_patternRef[itrack]
            if not (patternRef < options.npatterns):
                continue
 
            chi2     = evt.AMTTTracks_chi2[itrack]
            ndof     = evt.AMTTTracks_ndof[itrack]
            chi2Red  = chi2/ndof
 
            if not (chi2Red < options.maxChi2):
                continue
 
            invPt      = evt.AMTTTracks_invPt     [itrack]
            cottheta   = evt.AMTTTracks_cottheta  [itrack]
            phi0       = evt.AMTTTracks_phi0      [itrack]
            z0         = evt.AMTTTracks_z0        [itrack]
 
            tracks[itrack] = (1.0/abs(invPt), math.asinh(cottheta), phi0, z0) #pt, eta, phi0, z0
 
        # Sanity check. Double loop over simtracks and fitted tracks
        # For every simTrack look for the closest fitted track (in normalized track pars square diff)
        for ipart, part in trkparts.iteritems():
            minMatch, minMatch_i = 999999., 0
            for itrack, track in tracks.iteritems():
                match = 0.
                match += squaredNormDiff(1/part[0], 1/track[0], parrmses[0]) #InvPt
                match += squaredNormDiff(math.sinh(part[1]), math.sinh(track[1]), parrmses[1]) #CotTheta
                for i in xrange(2,4):
                    match += squaredNormDiff(part[i], track[i], parrmses[i]) #phi0 , z0
 
                if minMatch > match:
                    minMatch = match
                    minMatch_i = itrack
                #print ievt, ipart, itrack, "(%i)" % evt.AMTTTracks_synTpId[itrack], match, minMatch, minMatch_i

            # If there is a good match (minMatch<...)
            # And the simtrack (ipart) does not match the syntethic tpid (can happen for 5/6 patterns)
#             if minMatch != 999999. and minMatch < 5:
#                 if ipart != evt.AMTTTracks_synTpId[minMatch_i]:
#                     print "WARNING: not match: ", ipart, evt.AMTTTracks_synTpId[minMatch_i]

         # Fill histograms
        for ipart, part in trkparts.iteritems():
            trkparts_allevts.append(part)

        for itrack, track in tracks.iteritems():
            tracks_allevts.append(track)

        for ipart, part in trkparts.iteritems():
            for itrack, track in tracks.iteritems():
                #if not (ipart == evt.AMTTTracks_synTpId[itrack]):
                #    continue

                accept = 1;
                if (accept) : accept *= (absDiff(        1/part[0],          1/track[0])  < parrmses[0] * 5)
                if (accept) : accept *= (absDiff(math.sinh(part[1]), math.sinh(track[1])) < parrmses[1] * 5)
                if (accept) : accept *= (absDiff(          part[2],            track[2])  < parrmses[2] * 5)
                if (accept) : accept *= (absDiff(          part[3],            track[3])  < parrmses[3] * 5)

                if accept:
                    histos["errpar%i"         % 0].Fill(part[0]           , (track[0] - part[0])/part[0]) # VS Pt
                    histos["errpar%i"         % 1].Fill(part[0]           , (track[0] - part[0])/part[0]) # VS Pt
                    histos["errpar%i"         % 2].Fill(part[0]           , (track[0] - part[0])/part[0]) # VS Pt
                    histos["errpar%i"         % 3].Fill(math.sinh(part[1]), (track[0] - part[0])/part[0]) # VS Eta

        # ______________________________________________________________________
        # N-1 conditions

        for fact in factorized_plots:
            for part in trkparts_ievt_fact[fact]:
                pt, eta, phi, vz, charge, pdgId = part

                prefix = "efficiency_"
                if options.etamin < eta < options.etamax and options.phimin < phi < options.phimax:
                    histos[prefix + ("%s_" % fact) + "pt"      ].Fill(pt)
                    histos[prefix + ("%s_" % fact) + "ppt"     ].Fill(pt)
                    histos[prefix + ("%s_" % fact) + "pppt"    ].Fill(pt)
                if options.ptmin < pt < options.ptmax and options.phimin < phi < options.phimax:
                    histos[prefix + ("%s_" % fact) + "eta"     ].Fill(eta)
                if options.ptmin < pt < options.ptmax and options.etamin < eta < options.etamax:
                    histos[prefix + ("%s_" % fact) + "phi"     ].Fill(phi)
                if options.ptmin < pt < options.ptmax and options.etamin < eta < options.etamax and options.phimin < phi < options.phimax:
                    histos[prefix + ("%s_" % fact) + "vz"      ].Fill(vz)
#                     histos[prefix + ("%s_" % fact) + "charge"  ].Fill(charge)

    tree.SetBranchStatus("*", 1)
    return

def drawer_draw(histos, options):
    def displayGaus(h, ignorebins=0, scalebox=(1.,1.)):
        # Display gaussian fit parameters
        print "Fitting " + str(h.GetName())
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

        h.SetStats(0)
        #gPad.Modified(); gPad.Update()
        ps.Draw()

#     hvars = ["pt", "ppt", "pppt", "eta", "phi", "vz", "charge"]
    hvars = ["pt", "ppt", "pppt", "eta", "phi", "vz"]

    prefix  = "efficiency_"
    prefix1 = "efficiencyUpTo_"
#     prefix2 = "efficiencyFactor_"

    for i, fact in enumerate(factorized_plots):
        if fact == "00TP":
           continue

        for j, hvar in enumerate(hvars):
            hname  = prefix + ("%s_%s" % (fact, hvar))
            hname1 = prefix + ("%s_%s" % (factorized_plots[0], hvar))
#             hname2 = prefix + ("%s_%s" % (factorized[i-1], hvar))

            h1 = TEfficiency(histos[hname], histos[hname1])
            hname1 = hname.replace(prefix, prefix1)
            h1.SetName(hname1)
            histos[hname1] = h1

#             h2 = TEfficiency(histos[hname], histos[hname2])
#             hname2 = hname.replace(prefix, prefix2)
#             h2.SetName(hname2)
#             histos[hname2] = h2

    # Error statistics
    for hname, h in histos.iteritems():
        if h.ClassName() == "TEfficiency":
            h.SetConfidenceLevel(0.682689492137)  # 1-sigma
            h.SetStatisticOption(0)  # kFCP
            #h.SetStatisticOption(6)  # kBUniform

    # Re-style
    for hname, h in histos.iteritems():
        h.SetLineWidth(2); h.SetMarkerSize(0)
        h.SetMarkerStyle(24); h.SetMarkerSize(1)
        h.SetMarkerColor(col); h.SetLineColor(col); h.SetFillColor(fcol)

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

#         if hname.find("errpar")>=0:
# #             ymax = h.GetMaximum() * 1.4
# #             h.SetMinimum(0); h.SetMaximum(ymax)
#  
# #             h.Draw("hist")
# # #             displayGaus(h, ignorebins=10)
# #             h.SetMarkerStyle(20); h.SetMarkerSize(0.3)
# #             CMS_label()
# #             save(options.outdir, "%s_%s" % (hname, options.ss), dot_pdf=False, dot_root=False)
# 
#             # Slicing the TH2D and getting the RMS of each bin as resolution.
#             # Fill arrays with the RMSs and plot them as TGraph
#             nBinsX = h.GetNbinsX() 
#             vX=[]
#             vRMSs=[]
#             for iBin in range(nBinsX):
#                 vX.append(h.GetXaxis().GetBinCenter(iBin+1))
#                 vRMSs.append(h.ProjectionY("_RMS",iBin+1,iBin+2).GetRMS())
#             gr = TGraph(len(vRMSs),array('d',vX),array('d',vRMSs))
#             gr.Draw("AP")
#             gr.SetMarkerStyle(24); gr.SetMarkerSize(1.0); gr.SetMarkerColor(col)
#             gr.GetYaxis().SetTitle("#Deltap_{T}/p_{T}")
#             ivar=int(hname.replace("errpar",""))
#             gr.GetXaxis().SetTitle(parnames[ivar])
#             CMS_label()
#             save(options.outdir, "%s_%s" % ("trackPtRelativeRMSvs"+varnames[ivar], options.ss), dot_pdf=False, dot_root=False)
           
        if (h.ClassName() == "TH1F" or h.ClassName() == "TH2F"):
            continue

        CMS_label()
        save(options.outdir, "%s_%s" % (hname, options.outstring), dot_pdf=False, dot_root=False, additional=h.additional)

    # Misc
#     hname = "digiFraction"
#     h = histos[hname]
#     h.SetLineColor(col); h.SetFillColor(fcol)
#     h.Draw("hist")
#     gPad.SetLogy(1)
#     CMS_label()
#     save(options.outdir, "%s_%s" % (hname, options.ss), dot_root=True)
#     gPad.SetLogy(0)

    return

def drawer_sitrep(histos, options):
    print "--- SITREP ---------------------------------------------------------"


# ______________________________________________________________________________
# Main function
def main():

    # Setup argument parser
    parser = argparse.ArgumentParser()

    # Add default arguments
    add_drawer_arguments(parser)

    # Add more arguments
    parser.add_argument("ss", help="short name of superstrip definition (e.g. ss256)")
    parser.add_argument("npatterns", type=int, help="number of patterns to reach the desired coverage")
    parser.add_argument("--coverage", type=float, default=0.95, help="desired coverage (default: %(default)s)")
    parser.add_argument("--minPt", type=float, default=2, help="min pT for gen particle (default: %(default)s)")
    parser.add_argument("--maxChi2", type=float, default=5, help="max reduced chi-squared (default: %(default)s)")
    parser.add_argument("--minNLayers", type=int, default=5, help="min number of layers (default: %(default)s)")
    parser.add_argument("--minDigiFraction", type=float, default=0.3, help="min contribution to a digi (default: %(default)s)")
    parser.add_argument("--outstring", type=str, help="string for output file labelling (default: %(default)s)")

    # Parse default arguments
    options = parser.parse_args()
    parse_drawer_options(options)
    options.ptmin = options.minPt


    # Init
    drawerInit = DrawerInit()
    tchain = TChain("ntupler/tree", "")
    tchain.AddFileInfoList(options.tfilecoll.GetList())

    gStyle.SetNdivisions(510, "Y")
    gStyle.SetEndErrorSize(0)
    use_TEfficiency()

    # Process
    histos = drawer_book(options)
    drawer_project(tchain, histos, options)
    drawer_draw(histos, options)
    drawer_sitrep(histos, options)


# ______________________________________________________________________________
if __name__ == '__main__':

    # Call the main function
    main()
