#!/usr/bin/env python

from rootdrawing import *

"""
**PLOTS**

# nstubs per module, per concentrator, per chip, per superstrip-32 [per layer]

# number of hits, number of clusters

# number of modules? number of superstrips

# hit rate, stub rate, hit occupancy in module (%), in layer, module occupancy in tower

# hit efficiency, stub efficiency, layer efficiency
  (subsume drawer_layerdisk.py)

# stub purity

# trigger efficiency, trigger purity

# refer to tkgeometry plots

# refer to Luciano's plots

# muon + PU140 after pattern matching
"""

# ______________________________________________________________________________
# Configurations

samples = {}
samples["nu140"] = True
samples["mu140"] = False

sections = {}
sections["geometry"   ] = False
sections["trigger"    ] = True
sections["fixme"      ] = False

drawerInit = DrawerInit()

# Muon gun
#infiles = ["/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1_ntuple/SingleMuMinus_20140714/ntuple_1_1_1TX.root", "/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1_ntuple/SingleMuMinus_20140714/ntuple_2_1_t3v.root"]
infiles = ["/uscms_data/d2/jiafu/L1TrackTrigger/CRAB_amsim/ntuple_1_1_1TX_redux.root"]

#FIXME
infiles = ["/uscms_data/d2/jiafu/L1TrackTrigger/CMSSW_6_2_0_SLHC12_patch1/src/SLHCL1TrackTriggerSimulations/Configuration/test/jftest2/roads.root"]
#FIXME

col  = TColor.GetColor("#1f78b4")  # Paired
fcol = TColor.GetColor("#a6cee3")
gcol = kGray+1
imgdir = "figures_pileup/"

# Neutrino gun (140PU)
if samples["nu140"]:
    infiles = ["/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1_ntuple/Neutrino_Pt2to20_gun_20140821/ntuple_1_1_9cK.root", "/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1_ntuple/Neutrino_Pt2to20_gun_20140821/ntuple_2_1_HJw.root"]

    #FIXME
    infiles = ["/uscms_data/d2/jiafu/L1TrackTrigger/CRAB_amsim/mycondor_20140821/SingleMuPlusMinus_sp16_ss256_20140714_jftest2/roads.root"]
    #FIXME

    col  = TColor.GetColor("#e31a1c")  # Paired
    fcol = TColor.GetColor("#fb9a99")
    gcol = kGray+1
    imgdir = "figures_pileup_nu140/"

# Muon gun (140PU)
if samples["mu140"]:
    infiles = ["/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1_ntuple/SingleMuMinusFlatPt0p2To150_20140821/ntuple_1_1_dzk.root", "/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1_ntuple/SingleMuMinusFlatPt0p2To150_20140821/ntuple_2_1_j6Z.root"]
    col  = TColor.GetColor("#6a3d9a")  # Paired
    fcol = TColor.GetColor("#cab2d6")
    gcol = kGray+1
    imgdir = "figures_pileup_mu140/"


nentries = 100000
#nentries = 1000
#nentries = 10

cut1, cut2, cut3 = "(TTStubs_isGenuine==1)", "(TTStubs_isCombinatoric==1)", "(TTStubs_isUnknown==1)"
cuts = ((cut1,col), (cut2,fcol), (cut3,gcol))

chain = TChain("ntupler/tree", "")
for f in infiles:
    chain.Add(f)
tree = chain

if not imgdir.endswith("/"):  imgdir += "/"
if gSystem.AccessPathName(imgdir):
    gSystem.mkdir(imgdir)


# ______________________________________________________________________________
# Profiles

def myBookProf(hname, xtitle, nbinsx, xlow, xup, ytitle, ymax):
    params = [
        struct2D(hname, "dummy_var", "dummy_cut", xtitle, nbinsx, xlow, xup, ytitle, 240, 0, ymax, col, col)
        for cut, col in cuts
        ]

    params[2].markerstyle = 24
    histos = bookProf(params)
    return (params, histos)


# ______________________________________________________________________________
# geometry
# all the plots are after stub finding!

if sections["geometry"]:
    def doit(params, histos, f, imgdir=None, logy=False, ymax=-1):
        if not params or not histos:
            print "ERROR! no histogram"
            return
        if not imgdir:
            print "ERROR! no output directory"
            return

        sim = True

        setBranchStatus(tree, sim=sim)

        p = params[0]
        eb = EtaBinning(p.xtitle, p.nbinsx, p.xlow, p.xup)

        f(tree, histos, eb, nentries=nentries, sim=sim)

        drawsProf(histos, logy=logy, ymax=ymax)
        if not sim:
            save(imgdir, p.name)
        else:
            save(imgdir, p.name + "_sim")
        return histos

    def setBranchStatus(tree, branches=[], sim=False):
        tree.SetBranchStatus("*"                     , 0)
        if sim:
            tree.SetBranchStatus("TTStubs_trkId"         , 1)
            tree.SetBranchStatus("TTStubs_simEta"        , 1)
            #tree.SetBranchStatus("TTStubs_simPhi"        , 1)
        else:
            tree.SetBranchStatus("TTStubs_isGenuine"     , 1)
            tree.SetBranchStatus("TTStubs_isCombinatoric", 1)
            tree.SetBranchStatus("TTStubs_isUnknown"     , 1)
            tree.SetBranchStatus("TTStubs_eta"           , 1)
            #tree.SetBranchStatus("TTStubs_phi"           , 1)

        for br in branches:
            tree.SetBranchStatus(br                      , 1)

    def myProjectProf(tree, histos, eb, nentries=1000000000, sim=False):
        # 2D array of 3 * nbinsx
        yrr = []
        for i in xrange(3):
            yrr.append([0] * eb.nbinsx)

        if sim:
            # Loop over events
            for i_ievt, ievt in enumerate(tree):
                if (i_ievt == nentries):  break
                #print "++ i_ievt: ", i_ievt

                # Loop over stubs
                for br in izip(ievt.TTStubs_trkId, ievt.TTStubs_simEta):
                    #print "br: ", br

                    for i in xrange(1):
                        if br[i] == 1:      # if Genuine (or if Combinatoric, if Unknown)
                            x = br[1]
                            ix = eb.findBin(x)
                            #print "x, ix", x, ix

                            yrr[i][ix] += 1 # accummulate y variable

                # Fill profiles
                for i in xrange(1):
                    # Loop over eta bins
                    for ix in xrange(eb.nbinsx):
                        if yrr[i][ix]:
                            histos[i].h.Fill(eb.getBinCenter(ix), yrr[i][ix])
                            #print "i, ix, x, y", i, ix, getBinCenter(ix), yrr[i][ix]

                            yrr[i][ix] = 0  # rezero

        else:
            # Loop over events
            for i_ievt, ievt in enumerate(tree):
                if (i_ievt == nentries):  break
                #print "++ i_ievt: ", i_ievt

                # Loop over stubs
                for br in izip(ievt.TTStubs_isGenuine, ievt.TTStubs_isCombinatoric, ievt.TTStubs_isUnknown, ievt.TTStubs_eta):
                    #print "br: ", br

                    for i in xrange(3):
                        if br[i]:           # if Genuine (or if Combinatoric, if Unknown)
                            x = br[3]
                            ix = eb.findBin(x)
                            #print "x, ix", x, ix

                            yrr[i][ix] += 1 # accummulate y variable

                # Fill profiles
                for i in xrange(3):
                    # Loop over eta bins
                    for ix in xrange(eb.nbinsx):
                        if yrr[i][ix]:
                            histos[i].h.Fill(eb.getBinCenter(ix), yrr[i][ix])
                            #print "i, ix, x, y", i, ix, getBinCenter(ix), yrr[i][ix]

                            yrr[i][ix] = 0  # rezero

        # Set the maximum in the first profile
        ymax = 0
        for i in xrange(3):
            ymax = max(histos[i].h.GetMaximum(), ymax)
        histos[0].h.SetMaximum(ymax)


    # __________________________________________________________________________
    params, histos = myBookProf("geometry_nstubs", "#eta", 120, -3, 3, "<# stubs>", 24000)
    histos = doit(params, histos, myProjectProf, imgdir)


# ______________________________________________________________________________
# trigger (trial)

if False:

    def doit(x, imgdir=None, logy=False):
        histos = book(x)
        project(tree, histos, nentries=nentries)
        draw(histos, logy=logy)
        save(imgdir, x[0].name)
        return histos

    cut = ""

    #p0 = struct("AMTTRoads_size", "@AMTTRoads_nHitLayers.size()" if not cut else "Sum$(%s)" % cut, "",
    #    "# roads", 50, 0, 50, col, fcol)
    #histos = doit([p0], imgdir)

    #p0 = struct("AMTTRoads_nHitLayers", "AMTTRoads_nHitLayers", cut,
    #    "# superstrip per road", 10, 0, 10, col, fcol)
    #histos = doit([p0], imgdir)


# ______________________________________________________________________________
# trigger

if sections["trigger"]:

    def doit(x, myProject, imgdir=None, logy=False):
        histos = book(x)
        myProject(tree, histos, nentries=nentries)
        draw(histos, ytitle="", logy=logy)
        save(imgdir, x[0].name)
        return histos

    cut = ""


    # __________________________________________________________________________
    def myProject(tree, histos, nentries=1000000000):
        for i_ievt, ievt in enumerate(tree):
            if (i_ievt == nentries):  break

            n = ievt.AMTTRoads_nHitLayers.size()
            histos[0].h.Fill(n)

    p0 = struct("trigger_nroads_per_event", "dummy", cut, "# roads per event", 60, 0, 60, col, fcol)
    histos = doit([p0], myProject, imgdir)

    # __________________________________________________________________________
    def myProject(tree, histos, nentries=1000000000):
        for i_ievt, ievt in enumerate(tree):
            if (i_ievt == nentries):  break

            for br in izip(ievt.AMTTRoads_nHitLayers, ievt.AMTTRoads_hitSuperstripIds):
                n = br[0]
                histos[0].h.Fill(n)

    p0 = struct("trigger_nsuperstrips_per_road", "dummy", cut, "# superstrips per road", 20, 0, 20, col, fcol)
    histos = doit([p0], myProject, imgdir, logy=True)

    # __________________________________________________________________________
    def myProject(tree, histos, nentries=1000000000):
        for i_ievt, ievt in enumerate(tree):
            if (i_ievt == nentries):  break

            for br in izip(ievt.AMTTRoads_nHitLayers, ievt.AMTTRoads_hitSuperstripIds):
                superstripIds = br[1]

                nmap = {}
                for ss in superstripIds:
                    if ss not in nmap:
                        nmap[ss] = 1
                    else:
                        nmap[ss] += 1

                n = 0
                for k, v in nmap.iteritems():
                    histos[0].h.Fill(v)
                    n += v

                if n != br[1].size():  print "ERROR"

    p0 = struct("trigger_nstubs_per_superstrip", "dummy", cut, "# stubs per superstrip", 20, 0, 20, col, fcol)
    histos = doit([p0], myProject, imgdir, logy=True)

    # __________________________________________________________________________
    def myProject(tree, histos, nentries=1000000000):
        for i_ievt, ievt in enumerate(tree):
            if (i_ievt == nentries):  break

            for br in izip(ievt.AMTTRoads_nHitLayers, ievt.AMTTRoads_hitSuperstripIds):
                n = br[1].size()
                histos[0].h.Fill(n)

    p0 = struct("trigger_nstubs_per_road", "dummy", cut, "# stubs per road", 40, 0, 40, col, fcol)
    histos = doit([p0], myProject, imgdir, logy=True)

    # __________________________________________________________________________
    def myProject(tree, histos, nentries=1000000000):
        for i_ievt, ievt in enumerate(tree):
            if (i_ievt == nentries):  break

            for br in izip(ievt.AMTTRoads_nHitLayers, ievt.AMTTRoads_hitSuperstripIds):
                superstripIds = br[1]

                n = 1.
                nmap = {}
                for ss in superstripIds:
                    if ss not in nmap:
                        nmap[ss] = 1
                    else:
                        nmap[ss] += 1

                for k, v in nmap.iteritems():
                    n *= v
                histos[0].h.Fill(n)

    p0 = struct("trigger_ncombinations_per_road", "dummy", cut, "# combinations per road", 50, 0, 200, col, fcol)
    histos = doit([p0], myProject, imgdir, logy=True)

    # __________________________________________________________________________
    def myProject(tree, histos, nentries=1000000000):
        for i_ievt, ievt in enumerate(tree):
            if (i_ievt == nentries):  break

            nn = 0
            for br in izip(ievt.AMTTRoads_nHitLayers, ievt.AMTTRoads_hitSuperstripIds):
                superstripIds = br[1]

                n = 1.
                nmap = {}
                for ss in superstripIds:
                    if ss not in nmap:
                        nmap[ss] = 1
                    else:
                        nmap[ss] += 1

                for k, v in nmap.iteritems():
                    n *= v
                nn += n

            histos[0].h.Fill(nn)

    p0 = struct("trigger_ncombinations_per_event", "dummy", cut, "# combinations per event", 60, 0, 300, col, fcol)
    histos = doit([p0], myProject, imgdir, logy=True)


# ______________________________________________________________________________
if sections["fixme"]:

    pass
