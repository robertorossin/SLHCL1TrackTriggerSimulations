#!/usr/bin/env python

from rootdrawing import *
from math import pi
import json

# ______________________________________________________________________________
# Configurations

samples = {}
samples["nu140"]   = False
samples["mu140"]   = False
samples["minbias"] = False

sections = {}
sections["rate_by_module"      ] = False
sections["rate_by_layer"       ] = False
sections["rate_by_layer2"      ] = False
sections["rate_by_track"       ] = True
sections["efficiency_by_module"] = False
sections["efficiency_by_layer" ] = False
sections["efficiency_by_track" ] = False
sections["purity_by_track"     ] = False
sections["fixme"               ] = False

drawerInit = DrawerInit()
gStyle.SetNumberContours(100)
gStyle.SetPalette(55)  # rainbow color map

# Muon gun
#infiles = ["/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1_ntuple/SingleMuMinus_20140714/ntuple_1_1_1TX.root"]
#infiles = ["/uscms_data/d2/jiafu/L1TrackTrigger/CRAB_amsim/ntuple_1_1_1TX_redux.root"]
#infiles = ["/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1/SingleMuMinus_Barrel_20140821/SingleMu_Barrel_E2023TTI_ntuple_1_1_Wll.root"]
infiles = ["/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1/SingleMuMinus_20140821/SingleMu_E2023TTI_ntuple_1_1_oGy.root"]

col  = TColor.GetColor("#1f78b4")  # Paired
fcol = TColor.GetColor("#a6cee3")
imgdir = "figures_layerdisk/"

# Neutrino gun (140PU)
if samples["nu140"]:
    infiles = ["/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1_ntuple/Neutrino_Pt2to20_gun_20140821/ntuple_1_1_9cK.root", "/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1_ntuple/Neutrino_Pt2to20_gun_20140821/ntuple_2_1_HJw.root"]
    col  = TColor.GetColor("#e31a1c")  # Paired
    fcol = TColor.GetColor("#fb9a99")
    imgdir = "figures_layerdisk_nu140/"

# Muon gun (140PU)
if samples["mu140"]:
    infiles = ["/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1_ntuple/SingleMuMinusFlatPt0p2To150_20140821/ntuple_1_1_dzk.root", "/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1_ntuple/SingleMuMinusFlatPt0p2To150_20140821/ntuple_2_1_j6Z.root"]
    col  = TColor.GetColor("#6a3d9a")  # Paired
    fcol = TColor.GetColor("#cab2d6")
    imgdir = "figures_layerdisk_mu140/"

# MinBias
if samples["minbias"]:
    infiles = get_infiles("/uscms_data/d2/jiafu/L1TrackTrigger/CRAB_amsim/input_MinBias_20140821.txt", fast=False)
    col  = TColor.GetColor("#ff7f00")  # Paired
    fcol = TColor.GetColor("#fdbf6f")
    imgdir = "figures_layerdisk_minbias/"


nentries = 1000000
#nentries = 50000

chain = TChain("ntupler/tree", "")
for f in infiles:
    chain.Add(f)
tree = chain

mapfile = "../data/trigger_sector_map.json"
ttmap = json.load(open(mapfile))

if not imgdir.endswith("/"):  imgdir += "/"
if gSystem.AccessPathName(imgdir):
    gSystem.mkdir(imgdir)


# ______________________________________________________________________________
# Geometry

b_nlads = [16, 24, 34, 48, 62, 76]
b_nmods = [63, 55, 54, 24, 24, 24]

e_offset = 5
e_nrings = [15 + e_offset]
e_nmods  = [20, 24, 28, 28, 32, 36, 36, 40, 40, 52, 56, 64, 68, 76, 80]

tt_netas = 6
tt_nphis = 8

def getReverseMap(directMap):
    reverseMap = {}
    for i in xrange(6*8):
        for m in directMap[str(i)]:
            if m in reverseMap:
                reverseMap[m].append(i)
            else:
                reverseMap[m] = [i]
    return reverseMap

ttrmap = getReverseMap(ttmap)

class EtaBinning:
    def __init__(self, xtitle, nbinsx, xlow, xup):
        self.xtitle = xtitle
        self.nbinsx, self.xlow, self.xup = nbinsx, xlow, xup
        self.binwidth = float(self.xup - self.xlow) / self.nbinsx

    def findBin(self, x):
        if x <  self.xlow: x = self.xlow
        if x >= self.xup : x = self.xup - 1e-6
        x = float(x - self.xlow) / self.binwidth
        return int(x)

    def getBinCenter(self, b):
        if b < 0           : b = 0
        if b >= self.nbinsx: b = self.nbinsx - 1
        b = self.xlow + (0.5+b) * self.binwidth
        return b

eb = EtaBinning("#eta", 60, -3, 3)
pb = EtaBinning("#phi", 64, -3.2, 3.2)


# ______________________________________________________________________________
# Rates

if sections["rate_by_module"]:

    def bookModule():
        histos = {}
        for i in xrange(5,11):  # barrel
            histos["b%i" % i] = TH2F("b%i" % i, "; Barrel module index; Barrel ladder index", b_nmods[i-5], 0, b_nmods[i-5], b_nlads[i-5], 0, b_nlads[i-5])

        for i in xrange(11,23):  # endcap
            if i == 16 or i == 17:  continue
            histos["e%i" % i] = TH2F("e%i" % i, "; r: Endcap ring index+%i; #theta: Endcap module index" % e_offset, 2*e_nrings[0], -e_nrings[0], e_nrings[0], 2*e_nrings[0], -e_nrings[0], e_nrings[0])  # a square frame

            for j in xrange(0,15):  # endcap rings
                histos["e%i_r%i" % (i,j)] = TH2F("e%i_r%i" % (i,j), "", e_nmods[j], 0, 2*pi, e_nrings[0], 0, e_nrings[0])

        histos["tt"] = TH2F("tt", "; Trigger tower eta index; Trigger tower phi index", tt_netas, 1, tt_netas+1, tt_nphis, 1, tt_nphis+1)
        return histos

    def projectModule(tree, histos, nentries=1000000000, sim=True):
        tree.SetBranchStatus("*", 0)
        if sim:
            tree.SetBranchStatus("TTStubs_modId"         , 1)
            tree.SetBranchStatus("TTStubs_simPt"         , 1)
            tree.SetBranchStatus("TTStubs_simEta"        , 1)
            tree.SetBranchStatus("TTStubs_trkId"         , 1)
        else:
            tree.SetBranchStatus("TTStubs_modId"         , 1)

        def fill(moduleId):
            lay = (moduleId / 10000) % 100
            lad = (moduleId / 100) % 100
            mod = (moduleId) % 100

            if lay < 11:  # barrel
                histos["b%i" % (lay)].Fill(mod, lad, 1)

            else:  # endcap
                rad = 2.*pi * mod / e_nmods[lad]
                histos["e%i_r%i" % (lay,lad)].Fill(rad, lad + e_offset, 1)

            if moduleId in ttrmap:
                for tt in ttrmap[moduleId]:
                    tteta = (tt / tt_nphis) + 1
                    ttphi = (tt % tt_nphis) + 1
                    histos["tt"].Fill(tteta, ttphi, 1)
            return

        # Loop over events
        for i_ievt, ievt in enumerate(tree):
            if (i_ievt == nentries):  break

            # Loop over stubs
            if sim:
                for pt, eta, trkId, moduleId in izip(ievt.TTStubs_simPt, ievt.TTStubs_simEta, ievt.TTStubs_trkId, ievt.TTStubs_modId):
                    if pt > 2 and abs(eta)<2.5 and trkId == 1:
                        fill(moduleId)
            else:
                for moduleId in ievt.TTStubs_modId:
                    fill(moduleId)

        tree.SetBranchStatus("*", 1)
        return

    def drawModule(histos):
        oldTextSize = latex.GetTextSize()
        latex.SetTextSize(0.06)

        # Get zmax
        zmax = 0
        for k, v in histos.iteritems():
            if k.startswith("b") or k.startswith("e"):
                zmax = max(zmax, v.GetMaximum())
        for k, v in histos.iteritems():
            if k.startswith("b") or k.startswith("e"):
                v.SetMinimum(0)
                v.SetMaximum(zmax)

        # Style fix
        for k, v in histos.iteritems():
            v.SetLabelSize(0.05, "XY")
            v.SetNdivisions(510, "XY")

        # Draw barrel
        for i in xrange(5,11):
            hname = "b%i" % i
            draw2D([histos[hname]], stats=False)
            latex.DrawLatex(0.68, 0.88, "Barrel %i" % (i-4))
            save(imgdir, "rate_by_module_%s" % hname)

        # Draw endcap
        for i in xrange(11,23):
            if i == 16 or i == 17:  continue
            hname = "e%i" % i
            draw2D([histos[hname]], stats=False, palette=False)

            for j in xrange(0,15):
                hname1 = "e%i_r%i" % (i,j)
                if j == 0:
                    histos[hname1].Draw("same pol colz")
                    gPad.Modified(); gPad.Update()
                    xy = (0.91, 0.13, 0.95, 0.95)
                    paletteObj = histos[hname1].FindObject("palette")
                    paletteObj.SetX1NDC(xy[0]); paletteObj.SetY1NDC(xy[1]); paletteObj.SetX2NDC(xy[2]); paletteObj.SetY2NDC(xy[3])
                    paletteObj.SetTitleSize(0.024); paletteObj.SetLabelSize(0.024)
                    gPad.Modified(); gPad.Update()
                else:
                    histos[hname1].Draw("same pol col")
            if i < 18:
                latex.DrawLatex(0.62, 0.88, "Endcap +%i" % (i-10))
            else:
                latex.DrawLatex(0.62, 0.88, "Endcap -%i" % (i-17))
            save(imgdir, "rate_by_module_%s" % hname)

        # Draw trigger tower
        hname = "tt"
        histos[hname].SetMinimum(0)
        draw2D([histos[hname]], stats=False)
        save(imgdir, "rate_by_module_%s" % hname)

        latex.SetTextSize(oldTextSize)
        return

    histos = bookModule()
    projectModule(tree, histos, nentries=nentries)
    print tree.GetEntries()
    drawModule(histos)


# ______________________________________________________________________________
if sections["rate_by_layer"]:

    def bookLayer():
        histos = {}
        for i in xrange(5,11):  # barrel
            histos["b%i" % i] = TH1F("b%i" % i, "; %s" % eb.xtitle, eb.nbinsx, eb.xlow, eb.xup)

        for i in xrange(11,23):  # endcap
            if i == 16 or i == 17:  continue
            histos["e%i" % i] = TH1F("e%i" % i, "; %s" % eb.xtitle, eb.nbinsx, eb.xlow, eb.xup)

        for i in xrange(48):
            histos["tt%i" % i] = TH1F("tt%i" % i, "; %s" % eb.xtitle, eb.nbinsx, eb.xlow, eb.xup)
        return histos

    def projectLayer(tree, histos, nentries=1000000000, sim=True):
        tree.SetBranchStatus("*", 0)
        if sim:
            tree.SetBranchStatus("TTStubs_modId"         , 1)
            tree.SetBranchStatus("TTStubs_simPt"         , 1)
            tree.SetBranchStatus("TTStubs_simEta"        , 1)
            tree.SetBranchStatus("TTStubs_trkId"         , 1)
        else:
            tree.SetBranchStatus("TTStubs_modId"         , 1)
            tree.SetBranchStatus("TTStubs_eta"           , 1)

        def fill(moduleId, eta):
            lay = (moduleId / 10000) % 100
            if lay < 11:  # barrel
                histos["b%i" % (lay)].Fill(eta)
            else:  # endcap
                histos["e%i" % (lay)].Fill(eta)

            if moduleId in ttrmap:
                for tt in ttrmap[moduleId]:
                    histos["tt%i" % (tt)].Fill(eta)
            return

        # Loop over events
        for i_ievt, ievt in enumerate(tree):
            if (i_ievt == nentries):  break

            # Loop over stubs
            if sim:
                for pt, eta, trkId, moduleId in izip(ievt.TTStubs_simPt, ievt.TTStubs_simEta, ievt.TTStubs_trkId, ievt.TTStubs_modId):
                    if pt > 2 and abs(eta)<2.5 and trkId == 1:
                        fill(moduleId, eta)
            else:
                for moduleId, stubEta in izip(ievt.TTStubs_modId, ievt.TTStubs_eta):
                    fill(moduleId, stubEta)

        tree.SetBranchStatus("*", 1)
        return

    def drawLayer(histos):
        # Get ymax
        ymax = 0
        for k, v in histos.iteritems():
            ymax = max(ymax, v.GetMaximum())

        # Style fix
        for k, v in histos.iteritems():
            v.SetLineWidth(2)
            v.SetMarkerSize(0)

        # Draw barrel
        for i in xrange(5,11):
            hname = "b%i" % i
            histos[hname].SetLineColor(paletteSet1[i-5])

        i = 5
        hname = "b%i" % i
        h = histos[hname]
        h.SetStats(0); h.SetMinimum(0); h.SetMaximum(ymax * 1.4)
        h.Draw("hist")
        for i in reversed(xrange(5,11)):
            histos["b%i" % i].Draw("same hist")

        leg1 = TLegend(0.56,0.70,0.94,0.94)
        leg1.SetFillStyle(0); leg1.SetLineColor(0); leg1.SetShadowColor(0); leg1.SetBorderSize(0)
        for i in xrange(5,11):
            leg1.AddEntry(histos["b%i" % i] , "Barrel layer %i" % (i-4), "l")
        leg1.Draw()
        CMS_label()
        save(imgdir, "rate_by_layer_%s" % hname)

        # Draw endcap +
        for i in xrange(11,16):
            hname = "e%i" % i
            histos[hname].SetLineColor(paletteSet1[i-11])

        i = 11
        hname = "e%i" % i
        h = histos[hname]
        h.SetStats(0); h.SetMinimum(0); h.SetMaximum(ymax * 1.4)
        h.Draw("hist")
        for i in reversed(xrange(11,16)):
            histos["e%i" % i].Draw("same hist")

        leg1 = TLegend(0.56,0.74,0.94,0.94)
        leg1.SetFillStyle(0); leg1.SetLineColor(0); leg1.SetShadowColor(0); leg1.SetBorderSize(0)
        for i in xrange(11,16):
            leg1.AddEntry(histos["e%i" % i] , "Endcap disk +%i" % (i-10), "l")
        leg1.Draw()
        CMS_label()
        save(imgdir, "rate_by_layer_%s" % hname)

        # Draw endcap -
        for i in xrange(18,23):
            hname = "e%i" % i
            histos[hname].SetLineColor(paletteSet1[i-18])

        i = 18
        hname = "e%i" % i
        h = histos[hname]
        h.SetStats(0); h.SetMinimum(0); h.SetMaximum(ymax * 1.4)
        h.Draw("hist")
        for i in reversed(xrange(18,23)):
            histos["e%i" % i].Draw("same hist")

        leg1 = TLegend(0.56,0.74,0.94,0.94)
        leg1.SetFillStyle(0); leg1.SetLineColor(0); leg1.SetShadowColor(0); leg1.SetBorderSize(0)
        for i in xrange(18,23):
            leg1.AddEntry(histos["e%i" % i] , "Endcap disk -%i" % (i-17), "l")
        leg1.Draw()
        CMS_label()
        save(imgdir, "rate_by_layer_%s" % hname)

        # Make TLine
        boundaries = [-2.5, -1.3, -1.6, -0.5, -0.8, 0.2, -0.2, 0.8, 0.5, 1.6, 1.3, 2.5]
        tlines = [TLine(b, 0, b, ymax) for b in boundaries]
        for l in tlines:
            l.SetLineStyle(7)
            l.SetLineColor(kGray)

        # Draw trigger tower
        for iphi in xrange(8):
            for i in xrange(6):
                hname = "tt%i" % (i*8 + iphi)
                histos[hname].SetLineColor(paletteSet1[i])

            i = 0
            hname = "tt%i" % (i*8 + iphi)
            h = histos[hname]
            h.SetStats(0); h.SetMinimum(0); h.SetMaximum(ymax * 1.4)
            h.Draw("hist")
            for l in tlines:
                l.Draw()
            for i in reversed(xrange(6)):
                histos["tt%i" % (i*8 + iphi)].Draw("same hist")

            leg1 = TLegend(0.56,0.70,0.94,0.94)
            leg1.SetFillStyle(0); leg1.SetLineColor(0); leg1.SetShadowColor(0); leg1.SetBorderSize(0)
            for i in xrange(6):
                leg1.AddEntry(histos["tt%i" % (i*8 + iphi)] , "Trigger tower %i" % (i*8 + iphi), "l")
            leg1.Draw()
            CMS_label()
            save(imgdir, "rate_by_layer_%s" % hname)

        donotdelete = [tlines]
        return donotdelete

    histos = bookLayer()
    projectLayer(tree, histos, nentries=nentries)
    print tree.GetEntries()
    d = drawLayer(histos)


# ______________________________________________________________________________
if sections["rate_by_layer2"]:

    def bookLayer():
        histos = {}
        for i in xrange(5,11):  # barrel
            histos["b%i" % i] = TH1F("b%i" % i, "; %s" % pb.xtitle, pb.nbinsx, pb.xlow, pb.xup)

        for i in xrange(11,23):  # endcap
            if i == 16 or i == 17:  continue
            histos["e%i" % i] = TH1F("e%i" % i, "; %s" % pb.xtitle, pb.nbinsx, pb.xlow, pb.xup)

        for i in xrange(48):
            histos["tt%i" % i] = TH1F("tt%i" % i, "; %s" % pb.xtitle, pb.nbinsx, pb.xlow, pb.xup)
        return histos

    def projectLayer(tree, histos, nentries=1000000000, sim=True):
        tree.SetBranchStatus("*", 0)
        if sim:
            tree.SetBranchStatus("TTStubs_modId"         , 1)
            tree.SetBranchStatus("TTStubs_simPt"         , 1)
            tree.SetBranchStatus("TTStubs_simEta"        , 1)
            tree.SetBranchStatus("TTStubs_simPhi"        , 1)
            tree.SetBranchStatus("TTStubs_trkId"         , 1)
        else:
            tree.SetBranchStatus("TTStubs_modId"         , 1)
            tree.SetBranchStatus("TTStubs_phi"           , 1)

        def fill(moduleId, phi):
            lay = (moduleId / 10000) % 100
            if lay < 11:  # barrel
                histos["b%i" % (lay)].Fill(phi)
            else:  # endcap
                histos["e%i" % (lay)].Fill(phi)

            if moduleId in ttrmap:
                for tt in ttrmap[moduleId]:
                    histos["tt%i" % (tt)].Fill(phi)
            return

        # Loop over events
        for i_ievt, ievt in enumerate(tree):
            if (i_ievt == nentries):  break

            # Loop over stubs
            if sim:
                for pt, eta, phi, trkId, moduleId in izip(ievt.TTStubs_simPt, ievt.TTStubs_simEta, ievt.TTStubs_simPhi, ievt.TTStubs_trkId, ievt.TTStubs_modId):
                    if pt > 2 and abs(eta)<2.5 and trkId == 1:
                        fill(moduleId, phi)
            else:
                for moduleId, stubPhi in izip(ievt.TTStubs_modId, ievt.TTStubs_phi):
                    fill(moduleId, stubPhi)

        tree.SetBranchStatus("*", 1)
        return

    def drawLayer(histos):
        # Get ymax
        ymax = 0
        for k, v in histos.iteritems():
            ymax = max(ymax, v.GetMaximum())

        # Style fix
        for k, v in histos.iteritems():
            v.SetLineWidth(2)
            v.SetMarkerSize(0)

        # Draw barrel
        for i in xrange(5,11):
            hname = "b%i" % i
            histos[hname].SetLineColor(paletteSet1[i-5])

        i = 5
        hname = "b%i" % i
        h = histos[hname]
        h.SetStats(0); h.SetMinimum(0); h.SetMaximum(ymax * 1.4)
        h.Draw("hist")
        for i in reversed(xrange(5,11)):
            histos["b%i" % i].Draw("same hist")

        leg1 = TLegend(0.56,0.70,0.94,0.94)
        leg1.SetFillStyle(0); leg1.SetLineColor(0); leg1.SetShadowColor(0); leg1.SetBorderSize(0)
        for i in xrange(5,11):
            leg1.AddEntry(histos["b%i" % i] , "Barrel layer %i" % (i-4), "l")
        leg1.Draw()
        CMS_label()
        save(imgdir, "rate_by_layer2_%s" % hname)

        # Draw endcap +
        for i in xrange(11,16):
            hname = "e%i" % i
            histos[hname].SetLineColor(paletteSet1[i-11])

        i = 11
        hname = "e%i" % i
        h = histos[hname]
        h.SetStats(0); h.SetMinimum(0); h.SetMaximum(ymax * 1.4)
        h.Draw("hist")
        for i in reversed(xrange(11,16)):
            histos["e%i" % i].Draw("same hist")

        leg1 = TLegend(0.56,0.74,0.94,0.94)
        leg1.SetFillStyle(0); leg1.SetLineColor(0); leg1.SetShadowColor(0); leg1.SetBorderSize(0)
        for i in xrange(11,16):
            leg1.AddEntry(histos["e%i" % i] , "Endcap disk +%i" % (i-10), "l")
        leg1.Draw()
        CMS_label()
        save(imgdir, "rate_by_layer2_%s" % hname)

        # Draw endcap -
        for i in xrange(18,23):
            hname = "e%i" % i
            histos[hname].SetLineColor(paletteSet1[i-18])

        i = 18
        hname = "e%i" % i
        h = histos[hname]
        h.SetStats(0); h.SetMinimum(0); h.SetMaximum(ymax * 1.4)
        h.Draw("hist")
        for i in reversed(xrange(18,23)):
            histos["e%i" % i].Draw("same hist")

        leg1 = TLegend(0.56,0.74,0.94,0.94)
        leg1.SetFillStyle(0); leg1.SetLineColor(0); leg1.SetShadowColor(0); leg1.SetBorderSize(0)
        for i in xrange(18,23):
            leg1.AddEntry(histos["e%i" % i] , "Endcap disk -%i" % (i-17), "l")
        leg1.Draw()
        CMS_label()
        save(imgdir, "rate_by_layer2_%s" % hname)

        # Make TLine
        boundaries = [0.6, 1.7, 1.4, 2.5, 2.2, -3.1, 3.0, -2.3, -2.6, -1.5, -1.8, -0.7, -1.0, 0.1, -0.2, 0.9]
        tlines = [TLine(b, 0, b, ymax) for b in boundaries]
        for l in tlines:
            l.SetLineStyle(7)
            l.SetLineColor(kGray)

        # Draw trigger tower
        for ieta in xrange(6):
            for i in xrange(8):
                hname = "tt%i" % (ieta*8 + i)
                histos[hname].SetLineColor(paletteSet1[i])

            i = 0
            hname = "tt%i" % (ieta*8 + i)
            h = histos[hname]
            h.SetStats(0); h.SetMinimum(0); h.SetMaximum(ymax * 1.4)
            h.Draw("hist")
            for l in tlines:
                l.Draw()
            for i in reversed(xrange(8)):
                histos["tt%i" % (ieta*8 + i)].Draw("same hist")

            leg1 = TLegend(0.56,0.70,0.94,0.94)
            leg1.SetFillStyle(0); leg1.SetLineColor(0); leg1.SetShadowColor(0); leg1.SetBorderSize(0)
            for i in xrange(8):
                leg1.AddEntry(histos["tt%i" % (ieta*8 + i)] , "Trigger tower %i" % (ieta*8 + i), "l")
            leg1.Draw()
            CMS_label()
            save(imgdir, "rate_by_layer2_%s" % hname)

        donotdelete = [tlines]
        return donotdelete

    histos = bookLayer()
    projectLayer(tree, histos, nentries=nentries)
    print tree.GetEntries()
    d = drawLayer(histos)


# ______________________________________________________________________________
if sections["rate_by_track"]:

    def bookTrack():
        histos = {}
        # for different pT?
        histos["module_1"] = TProfile  ("module_1", "; %s; <# modules>" % eb.xtitle, eb.nbinsx, eb.xlow, eb.xup, 0, 100000)
        histos["module_2"] = TProfile2D("module_2", "; %s; %s; <# modules>" % (eb.xtitle, pb.xtitle), eb.nbinsx, eb.xlow, eb.xup, pb.nbinsx, pb.xlow, pb.xup, 0, 100000)
        return histos

    def projectTrack(tree, histos, nentries=1000000000, sim=True):
        tree.SetBranchStatus("*", 0)
        if sim:
            tree.SetBranchStatus("TTStubs_modId"         , 1)
            tree.SetBranchStatus("TTStubs_simPt"         , 1)
            tree.SetBranchStatus("TTStubs_simEta"        , 1)
            tree.SetBranchStatus("TTStubs_simPhi"        , 1)
            tree.SetBranchStatus("TTStubs_trkId"         , 1)
        else:
            tree.SetBranchStatus("TTStubs_modId"         , 1)
            tree.SetBranchStatus("TTStubs_eta"           , 1)

        def fill(results, eta, phi):
            histos["module_1"].Fill(eta, len(results))
            histos["module_2"].Fill(eta, phi, len(results))
            return

        # Loop over events
        for i_ievt, ievt in enumerate(tree):
            if (i_ievt == nentries):  break

            results = {}
            theEta = 999999
            thePhi = 999999

            # Loop over stubs
            if sim:
                for pt, eta, phi, trkId, moduleId in izip(ievt.TTStubs_simPt, ievt.TTStubs_simEta, ievt.TTStubs_simPhi, ievt.TTStubs_trkId, ievt.TTStubs_modId):
                    if pt > 2 and abs(eta)<2.5 and trkId == 1:
                        theEta = eta
                        thePhi = phi
                        results[moduleId] = True
            else:
                for moduleId, stubEta in izip(ievt.TTStubs_modId, ievt.TTStubs_eta):
                    pass  # FIXME
            fill(results, theEta, thePhi)

        tree.SetBranchStatus("*", 1)
        return

    def drawTrack(histos):
        # Style fix
        for k, v in histos.iteritems():
            if k.endswith("_1"):
                v.SetMarkerSize(1.3)
                v.SetMarkerColor(paletteSet1[3])

        hname = "module_1"
        h = histos[hname]
        ymax = h.GetMaximum()
        h.SetStats(0); h.SetMinimum(0); h.SetMaximum(ymax * 1.4)
        h.Draw("hist p")
        CMS_label()
        save(imgdir, "rate_by_track_%s" % hname)

        hname = "module_2"
        h = histos[hname]
        ymax = h.GetMaximum()
        h.SetStats(0); h.SetMinimum(0); h.SetMaximum(ymax * 1.2)
        draw2D([h], stats=False)
        save(imgdir, "rate_by_track_%s" % hname)

    histos = bookTrack()
    projectTrack(tree, histos, nentries=nentries)
    print tree.GetEntries()
    d = drawTrack(histos)


# ______________________________________________________________________________
if sections["efficiency_by_module"]:

    def bookModule():
        histos = {}
        for i in xrange(5,11):  # barrel
            histos["b%i_denom" % i] = TH2F("b%i_denom" % i, "; Barrel module index; Barrel ladder index", b_nmods[i-5], 0, b_nmods[i-5], b_nlads[i-5], 0, b_nlads[i-5])
            histos["b%i_num" % i] = TH2F("b%i_num" % i, "; Barrel module index; Barrel ladder index", b_nmods[i-5], 0, b_nmods[i-5], b_nlads[i-5], 0, b_nlads[i-5])

        for i in xrange(11,23):  # endcap
            if i == 16 or i == 17:  continue
            histos["e%i" % i] = TH2F("e%i" % i, "; r: Endcap ring index+%i; #theta: Endcap module index" % e_offset, 2*e_nrings[0], -e_nrings[0], e_nrings[0], 2*e_nrings[0], -e_nrings[0], e_nrings[0])  # a square frame

            for j in xrange(0,15):  # endcap rings
                histos["e%i_r%i_denom" % (i,j)] = TH2F("e%i_r%i_denom" % (i,j), "", e_nmods[j], 0, 2*pi, e_nrings[0], 0, e_nrings[0])
                histos["e%i_r%i_num" % (i,j)] = TH2F("e%i_r%i_num" % (i,j), "", e_nmods[j], 0, 2*pi, e_nrings[0], 0, e_nrings[0])
        return histos

    def projectModule(tree, histos, nentries=1000000000):
        tree.SetBranchStatus("*", 0)
        tree.SetBranchStatus("TTStubs_modId"         , 1)
        tree.SetBranchStatus("TTStubs_simPt"         , 1)
        tree.SetBranchStatus("TTStubs_simEta"        , 1)
        tree.SetBranchStatus("TTStubs_trkId"         , 1)
        tree.SetBranchStatus("TTStubs_hitChans"      , 1)
        tree.SetBranchStatus("simPixelDigis_modId"   , 1)
        tree.SetBranchStatus("simPixelDigis_trkId"   , 1)
        tree.SetBranchStatus("simPixelDigis_chan"    , 1)

        def fill(results_by_moduleId):
            for k, v in results_by_moduleId.iteritems():
                moduleId = k
                lay = (moduleId / 10000) % 100
                lad = (moduleId / 100) % 100
                mod = (moduleId) % 100

                if lay < 11:  # barrel
                    histos["b%i_denom" % (lay)].Fill(mod, lad, 1)
                    if v:
                        histos["b%i_num" % (lay)].Fill(mod, lad, 1)

                else:  # endcap
                    rad = 2.*pi * mod / e_nmods[lad]
                    histos["e%i_r%i_denom" % (lay,lad)].Fill(rad, lad + e_offset, 1)
                    if v:
                        histos["e%i_r%i_num" % (lay,lad)].Fill(rad, lad + e_offset, 1)
            return

        # Loop over events
        for i_ievt, ievt in enumerate(tree):
            if (i_ievt == nentries):  break

            lookup = {}
            results = {}

            # Loop over stubs
            for pt, eta, trkId, moduleId, hitChans in izip(ievt.TTStubs_simPt, ievt.TTStubs_simEta, ievt.TTStubs_trkId, ievt.TTStubs_modId, ievt.TTStubs_hitChans):
                if pt > 2 and abs(eta)<2.5 and trkId == 1:
                    for chan in hitChans:
                        key = (moduleId, chan)
                        lookup[key] = True

            # Loop over digis
            for trkId, moduleId, chan in izip(ievt.simPixelDigis_trkId, ievt.simPixelDigis_modId, ievt.simPixelDigis_chan):
                if trkId == 1:
                    key = (moduleId, chan)
                    results[key] = (key in lookup)

            results_by_moduleId = {}
            for k, v in results.iteritems():
                moduleId = k[0]
                if moduleId not in results_by_moduleId:
                    results_by_moduleId[moduleId] = False
                if v:
                    results_by_moduleId[moduleId] = True

            fill(results_by_moduleId)

        tree.SetBranchStatus("*", 1)
        return

    def drawModule(histos):
        oldTextSize = latex.GetTextSize()
        latex.SetTextSize(0.06)

        # Style fix
        for k, v in histos.iteritems():
            v.SetLabelSize(0.05, "XY")
            v.SetNdivisions(510, "XY")

        # Make ratio plots
        zmax = 1
        for i in xrange(5,11):
            h = histos["b%i_num" % i].Clone("b%i" % i)
            h.Divide(histos["b%i_num" % i], histos["b%i_denom" % i], 1, 1, "b")
            h.SetMinimum(0)
            h.SetMaximum(zmax)
            histos["b%i" % i] = h

        for i in xrange(11,23):
            if i == 16 or i == 17:  continue

            for j in xrange(0,15):
                h = histos["e%i_r%i_num" % (i,j)].Clone("e%i_r%i" % (i,j))
                h.Divide(histos["e%i_r%i_num" % (i,j)], histos["e%i_r%i_denom" % (i,j)], 1, 1, "b")
                h.SetMinimum(0)
                h.SetMaximum(zmax)
                histos["e%i_r%i" % (i,j)] = h

        # Debug
        #for i in xrange(5,11):
        #    histos["b%i" % i].Print("all")

        # Draw barrel
        for i in xrange(5,11):
            hname = "b%i" % i
            draw2D([histos[hname]], stats=False)
            latex.DrawLatex(0.68, 0.88, "Barrel %i" % (i-4))
            save(imgdir, "efficiency_by_module_%s" % hname)

        # Draw endcap
        for i in xrange(11,23):
            if i == 16 or i == 17:  continue
            hname = "e%i" % i
            draw2D([histos[hname]], stats=False, palette=False)

            for j in xrange(0,15):
                hname1 = "e%i_r%i" % (i,j)
                if j == 0:
                    histos[hname1].Draw("same pol colz")
                    gPad.Modified(); gPad.Update()
                    xy = (0.91, 0.13, 0.95, 0.95)
                    paletteObj = histos[hname1].FindObject("palette")
                    paletteObj.SetX1NDC(xy[0]); paletteObj.SetY1NDC(xy[1]); paletteObj.SetX2NDC(xy[2]); paletteObj.SetY2NDC(xy[3])
                    paletteObj.SetTitleSize(0.024); paletteObj.SetLabelSize(0.024)
                    gPad.Modified(); gPad.Update()
                else:
                    histos[hname1].Draw("same pol col")
            if i < 18:
                latex.DrawLatex(0.62, 0.88, "Endcap +%i" % (i-10))
            else:
                latex.DrawLatex(0.62, 0.88, "Endcap -%i" % (i-17))
            save(imgdir, "efficiency_by_module_%s" % hname)

        latex.SetTextSize(oldTextSize)
        return

    histos = bookModule()
    projectModule(tree, histos, nentries=nentries)
    print tree.GetEntries()
    drawModule(histos)


# ______________________________________________________________________________
if sections["efficiency_by_layer"]:

    def bookLayer():
        histos = {}
        for i in xrange(5,11):  # barrel
            histos["b%i_denom" % i] = TH1F("b%i_denom" % i, "; %s; Layer efficiency" % eb.xtitle, eb.nbinsx, eb.xlow, eb.xup)
            histos["b%i_num" % i] = TH1F("b%i_num" % i, "; %s" % eb.xtitle, eb.nbinsx, eb.xlow, eb.xup)

        for i in xrange(11,23):  # endcap
            if i == 16 or i == 17:  continue
            histos["e%i_denom" % i] = TH1F("e%i_denom" % i, "; %s; Layer efficiency" % eb.xtitle, eb.nbinsx, eb.xlow, eb.xup)
            histos["e%i_num" % i] = TH1F("e%i_num" % i, "; %s" % eb.xtitle, eb.nbinsx, eb.xlow, eb.xup)
        return histos

    def projectLayer(tree, histos, nentries=1000000000, sim=True):
        tree.SetBranchStatus("*", 0)
        tree.SetBranchStatus("TTStubs_modId"         , 1)
        tree.SetBranchStatus("TTStubs_simPt"         , 1)
        tree.SetBranchStatus("TTStubs_simEta"        , 1)
        tree.SetBranchStatus("TTStubs_trkId"         , 1)
        tree.SetBranchStatus("TTStubs_hitChans"      , 1)
        tree.SetBranchStatus("simPixelDigis_modId"   , 1)
        tree.SetBranchStatus("simPixelDigis_trkId"   , 1)
        tree.SetBranchStatus("simPixelDigis_chan"    , 1)

        def fill(results_by_layer, eta):
            for k, v in results_by_layer.iteritems():
                lay = k

                if lay < 11:  # barrel
                    histos["b%i_denom" % (lay)].Fill(eta)
                    if v:
                        histos["b%i_num" % (lay)].Fill(eta)

                else:  # endcap
                    histos["e%i_denom" % (lay)].Fill(eta)
                    if v:
                        histos["e%i_num" % (lay)].Fill(eta)
            return

        # Loop over events
        for i_ievt, ievt in enumerate(tree):
            if (i_ievt == nentries):  break

            lookup = {}
            results = {}
            theEta = 999999

            # Loop over stubs
            for pt, eta, trkId, moduleId, hitChans in izip(ievt.TTStubs_simPt, ievt.TTStubs_simEta, ievt.TTStubs_trkId, ievt.TTStubs_modId, ievt.TTStubs_hitChans):
                if pt > 2 and abs(eta)<2.5 and trkId == 1:
                    theEta = eta
                    for chan in hitChans:
                        key = (moduleId, chan)
                        lookup[key] = True

            # Loop over digis
            for trkId, moduleId, chan in izip(ievt.simPixelDigis_trkId, ievt.simPixelDigis_modId, ievt.simPixelDigis_chan):
                if trkId == 1:
                    key = (moduleId, chan)
                    results[key] = (key in lookup)

            results_by_layer = {}
            for k, v in results.iteritems():
                layer = (k[0] / 10000) % 100
                if layer not in results_by_layer:
                    results_by_layer[layer] = False
                if v:
                    results_by_layer[layer] = True

            fill(results_by_layer, theEta)

        tree.SetBranchStatus("*", 1)
        return

    def drawLayer(histos):
        # Style fix
        for k, v in histos.iteritems():
            v.SetLineWidth(2)
            v.SetMarkerSize(0)

        # Make ratio plots
        for i in xrange(5,11):
            h = histos["b%i_num" % i].Clone("b%i" % i)
            h.Divide(histos["b%i_num" % i], histos["b%i_denom" % i], 1, 1, "b")
            histos["b%i" % i] = h

        for i in xrange(11,23):
            if i == 16 or i == 17:  continue
            h = histos["e%i_num" % i].Clone("e%i" % i)
            h.Divide(histos["e%i_num" % i], histos["e%i_denom" % i], 1, 1, "b")
            histos["e%i" % i] = h

        # Make TLine
        ymax = 1.
        oneline = TLine(eb.xlow, ymax, eb.xup, ymax)
        oneline.SetLineStyle(7)
        oneline.SetLineColor(kGray)

        # Debug
        #for i in xrange(5,11):
        #    histos["b%i" % i].Print("all")

        # Draw barrel
        for i in xrange(5,11):
            hname = "b%i" % i
            histos[hname].SetLineColor(paletteSet1[i-5])

        i = 5
        hname = "b%i" % i
        h = histos[hname]
        h.SetStats(0); h.SetMinimum(0); h.SetMaximum(ymax * 1.5)
        h.Draw("hist")
        oneline.Draw()
        for i in reversed(xrange(5,11)):
            histos["b%i" % i].Draw("same hist")

        leg1 = TLegend(0.56,0.70,0.94,0.94)
        leg1.SetFillStyle(0); leg1.SetLineColor(0); leg1.SetShadowColor(0); leg1.SetBorderSize(0)
        for i in xrange(5,11):
            leg1.AddEntry(histos["b%i" % i] , "Barrel layer %i" % (i-4), "l")
        leg1.Draw()
        CMS_label()
        save(imgdir, "efficiency_by_layer_%s" % hname)

        # Draw endcap +
        for i in xrange(11,16):
            hname = "e%i" % i
            histos[hname].SetLineColor(paletteSet1[i-11])

        i = 11
        hname = "e%i" % i
        h = histos[hname]
        h.SetStats(0); h.SetMinimum(0); h.SetMaximum(ymax * 1.5)
        h.Draw("hist")
        oneline.Draw()
        for i in reversed(xrange(11,16)):
            histos["e%i" % i].Draw("same hist")

        leg1 = TLegend(0.56,0.74,0.94,0.94)
        leg1.SetFillStyle(0); leg1.SetLineColor(0); leg1.SetShadowColor(0); leg1.SetBorderSize(0)
        for i in xrange(11,16):
            leg1.AddEntry(histos["e%i" % i] , "Endcap disk +%i" % (i-10), "l")
        leg1.Draw()
        CMS_label()
        save(imgdir, "efficiency_by_layer_%s" % hname)

        # Draw endcap -
        for i in xrange(18,23):
            hname = "e%i" % i
            histos[hname].SetLineColor(paletteSet1[i-18])

        i = 18
        hname = "e%i" % i
        h = histos[hname]
        h.SetStats(0); h.SetMinimum(0); h.SetMaximum(ymax * 1.5)
        h.Draw("hist")
        oneline.Draw()
        for i in reversed(xrange(18,23)):
            histos["e%i" % i].Draw("same hist")

        leg1 = TLegend(0.56,0.74,0.94,0.94)
        leg1.SetFillStyle(0); leg1.SetLineColor(0); leg1.SetShadowColor(0); leg1.SetBorderSize(0)
        for i in xrange(18,23):
            leg1.AddEntry(histos["e%i" % i] , "Endcap disk -%i" % (i-17), "l")
        leg1.Draw()
        CMS_label()
        save(imgdir, "efficiency_by_layer_%s" % hname)

    histos = bookLayer()
    projectLayer(tree, histos, nentries=nentries)
    print tree.GetEntries()
    d = drawLayer(histos)


# ______________________________________________________________________________
if sections["fixme"]:

    connectmap = json.load(open("../data/module_connections.json"))

    p0 = struct("rate_by_module_nconnections", "dummy", "dummy",
        "# connections", 6, 0, 6, kBlack, palette[1])
    histos = book([p0])

    for k, v in connectmap.iteritems():
        n = 0
        k = int(k)
        if k in ttrmap:
            assert(len(v) == len(ttrmap[k]))
            n = len(v)
        histos[0].h.Fill(n)
    draw(histos, ytitle="# modules")
    save(imgdir, p0.name)
