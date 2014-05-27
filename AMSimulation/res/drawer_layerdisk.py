#!/usr/bin/env python

from rootdrawing import *
from math import pi

# ______________________________________________________________________________
# Global

latex.SetTextSize(0.06)

# ______________________________________________________________________________
# Configurations

sections = {}
sections["rates"         ] = True
sections["rates_by_layer"] = True
sections["rates_by_tower"] = True
sections["efficiencies"  ] = False

drawerInit = DrawerInit()
infiles = get_infiles("input.txt", fast=True)

chain = TChain("ntupler/tree", "")
for f in infiles:
    chain.Add(f)
tree = chain

layoutfile = "../data/trigger_sector_map.csv"

imgdir = "figures_layerdisk/"
if not imgdir.endswith("/"):  imgdir += "/"
if gSystem.AccessPathName(imgdir):
    gSystem.mkdir(imgdir)


# ______________________________________________________________________________
# Geometry

b_nlads = [16,24,34,48,62,76]
b_nmods = [63,55,54,24,24,24]

e_offset = 5
e_nrings = [15 + e_offset]
e_nmods  = [20,24,28,28,32,36,36,40,40,52,56,64,68,76,80]

# 2D
histos = {}
histos["b5"]  = TH2F("b5" , "; Barrel module index; Barrel ladder index", b_nmods[0], 0, b_nmods[0], b_nlads[0], 0, b_nlads[0])
histos["b6"]  = TH2F("b6" , "; Barrel module index; Barrel ladder index", b_nmods[1], 0, b_nmods[1], b_nlads[1], 0, b_nlads[1])
histos["b7"]  = TH2F("b7" , "; Barrel module index; Barrel ladder index", b_nmods[2], 0, b_nmods[2], b_nlads[2], 0, b_nlads[2])
histos["b8"]  = TH2F("b8" , "; Barrel module index; Barrel ladder index", b_nmods[3], 0, b_nmods[3], b_nlads[3], 0, b_nlads[3])
histos["b9"]  = TH2F("b9" , "; Barrel module index; Barrel ladder index", b_nmods[4], 0, b_nmods[4], b_nlads[4], 0, b_nlads[4])
histos["b10"] = TH2F("b10", "; Barrel module index; Barrel ladder index", b_nmods[5], 0, b_nmods[5], b_nlads[5], 0, b_nlads[5])

histos["e11"] = TH2F("e11", "; r: Endcap ring index+%i; #theta: Endcap module index" % e_offset, 2*e_nrings[0], -e_nrings[0], e_nrings[0], 2*e_nrings[0], -e_nrings[0], e_nrings[0])
histos["e12"] = TH2F("e12", "; r: Endcap ring index+%i; #theta: Endcap module index" % e_offset, 2*e_nrings[0], -e_nrings[0], e_nrings[0], 2*e_nrings[0], -e_nrings[0], e_nrings[0])
histos["e13"] = TH2F("e13", "; r: Endcap ring index+%i; #theta: Endcap module index" % e_offset, 2*e_nrings[0], -e_nrings[0], e_nrings[0], 2*e_nrings[0], -e_nrings[0], e_nrings[0])
histos["e14"] = TH2F("e14", "; r: Endcap ring index+%i; #theta: Endcap module index" % e_offset, 2*e_nrings[0], -e_nrings[0], e_nrings[0], 2*e_nrings[0], -e_nrings[0], e_nrings[0])
histos["e15"] = TH2F("e15", "; r: Endcap ring index+%i; #theta: Endcap module index" % e_offset, 2*e_nrings[0], -e_nrings[0], e_nrings[0], 2*e_nrings[0], -e_nrings[0], e_nrings[0])

histos["e18"] = TH2F("e18", "; r: Endcap ring index+%i; #theta: Endcap module index" % e_offset, 2*e_nrings[0], -e_nrings[0], e_nrings[0], 2*e_nrings[0], -e_nrings[0], e_nrings[0])
histos["e19"] = TH2F("e19", "; r: Endcap ring index+%i; #theta: Endcap module index" % e_offset, 2*e_nrings[0], -e_nrings[0], e_nrings[0], 2*e_nrings[0], -e_nrings[0], e_nrings[0])
histos["e20"] = TH2F("e20", "; r: Endcap ring index+%i; #theta: Endcap module index" % e_offset, 2*e_nrings[0], -e_nrings[0], e_nrings[0], 2*e_nrings[0], -e_nrings[0], e_nrings[0])
histos["e21"] = TH2F("e21", "; r: Endcap ring index+%i; #theta: Endcap module index" % e_offset, 2*e_nrings[0], -e_nrings[0], e_nrings[0], 2*e_nrings[0], -e_nrings[0], e_nrings[0])
histos["e22"] = TH2F("e22", "; r: Endcap ring index+%i; #theta: Endcap module index" % e_offset, 2*e_nrings[0], -e_nrings[0], e_nrings[0], 2*e_nrings[0], -e_nrings[0], e_nrings[0])

histos["e11_r0"]  = TH2F("e11_r0" , "", e_nmods[0] , 0, 2*pi, e_nrings[0], 0, e_nrings[0])
histos["e11_r1"]  = TH2F("e11_r1" , "", e_nmods[1] , 0, 2*pi, e_nrings[0], 0, e_nrings[0])
histos["e11_r2"]  = TH2F("e11_r2" , "", e_nmods[2] , 0, 2*pi, e_nrings[0], 0, e_nrings[0])
histos["e11_r3"]  = TH2F("e11_r3" , "", e_nmods[3] , 0, 2*pi, e_nrings[0], 0, e_nrings[0])
histos["e11_r4"]  = TH2F("e11_r4" , "", e_nmods[4] , 0, 2*pi, e_nrings[0], 0, e_nrings[0])
histos["e11_r5"]  = TH2F("e11_r5" , "", e_nmods[5] , 0, 2*pi, e_nrings[0], 0, e_nrings[0])
histos["e11_r6"]  = TH2F("e11_r6" , "", e_nmods[6] , 0, 2*pi, e_nrings[0], 0, e_nrings[0])
histos["e11_r7"]  = TH2F("e11_r7" , "", e_nmods[7] , 0, 2*pi, e_nrings[0], 0, e_nrings[0])
histos["e11_r8"]  = TH2F("e11_r8" , "", e_nmods[8] , 0, 2*pi, e_nrings[0], 0, e_nrings[0])
histos["e11_r9"]  = TH2F("e11_r9" , "", e_nmods[9] , 0, 2*pi, e_nrings[0], 0, e_nrings[0])
histos["e11_r10"] = TH2F("e11_r10", "", e_nmods[10], 0, 2*pi, e_nrings[0], 0, e_nrings[0])
histos["e11_r11"] = TH2F("e11_r11", "", e_nmods[11], 0, 2*pi, e_nrings[0], 0, e_nrings[0])
histos["e11_r12"] = TH2F("e11_r12", "", e_nmods[12], 0, 2*pi, e_nrings[0], 0, e_nrings[0])
histos["e11_r13"] = TH2F("e11_r13", "", e_nmods[13], 0, 2*pi, e_nrings[0], 0, e_nrings[0])
histos["e11_r14"] = TH2F("e11_r14", "", e_nmods[14], 0, 2*pi, e_nrings[0], 0, e_nrings[0])
for i in xrange(12,16):
    for j in xrange(0,15):
        histos["e%i_r%i" % (i,j)] = histos["e%i_r%i" % (11,j)].Clone("e%i_r%i" % (i,j))
for i in xrange(18,23):
    for j in xrange(0,15):
        histos["e%i_r%i" % (i,j)] = histos["e%i_r%i" % (11,j)].Clone("e%i_r%i" % (i,j))

# 1D
def book1D(xtitle="", nbinsx=50, xlow=0, xup=50):
    for i, j in enumerate(["b_ps", "b_ss", "e_ps", "e_ss"]):
        h = TH1F(j, xtitle, nbinsx, xlow, xup)
        h.SetLineWidth(2)
        h.SetLineColor(palette[i])
        h.SetMarkerColor(palette[i])
        h.SetFillColor(lightpalette[i])
        histos[j] = h

def draw1D():
    # Stack them by hand
    histos["b_ss"].Add(histos["b_ps"])
    histos["e_ps"].Add(histos["b_ss"])
    histos["e_ss"].Add(histos["e_ps"])

    # Draw
    draws([histos["e_ss"], histos["e_ps"], histos["b_ss"], histos["b_ps"]], stats=False)


# ______________________________________________________________________________
# Setup trigger tower

ttmap = {}
with open(layoutfile) as f:
    for i, line in enumerate(f):
        if i == 0:  continue
        split = map(int, line.strip().split(","))
        #tt = tuple(split[0:2])
        tt = (split[0]-1)*8 + (split[1]-1)
        ttmap[tt] = split[2:]

ttmap_reversed = {}
for k, v in ttmap.iteritems():
    for iv in v:
        if iv not in ttmap_reversed:
            ttmap_reversed[iv] = [k]
        else:
            ttmap_reversed[iv].append(k)


# ______________________________________________________________________________
# Rates

if sections["rates"]:
    for hname, h in histos.iteritems():
        h.Reset()

    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("TTStubs_modId", 1)
    tree.SetBranchStatus("TTStubs_simPt", 1)
    tree.SetBranchStatus("TTStubs_simEta", 1)

    for ievt in tree:
        # Loop over stubs
        for moduleId, simPt, simEta in zip(ievt.TTStubs_modId, ievt.TTStubs_simPt, ievt.TTStubs_simEta):
            if simPt < 2 or abs(simEta) > 2.2:  continue

            ilay = (moduleId / 10000) % 100
            ilad = (moduleId / 100) % 100
            imod = (moduleId) % 100

            if ilay < 11:  # barrel
                histos["b%i" % (ilay)].Fill(imod, ilad, 1)

            else:  # endcap
                irad = 2*pi * imod / e_nmods[ilad]
                histos["e%i_r%i" % (ilay,ilad)].Fill(irad, ilad + e_offset, 1)

    nentries = tree.GetEntriesFast()
    print nentries

    ## Normalize
    #for hname, h in histos.iteritems():
    #    h.Scale(1.0/nentries)

    # Barrel
    for i in xrange(5,11):
        hname = "b%i" % i
        draw2D([histos[hname]], stats=False)
        latex.DrawLatex(0.68, 0.88, "Barrel %i" % (i-4))
        save(imgdir, "rates_%s" % hname)

    # Endcap
    for i in xrange(11,23):
        if i == 16 or i == 17:  continue

        hname = "e%s" % i
        draw2D([histos[hname]], stats=False, palette=False)

        zmax = 0
        for j in xrange(0,15):
            hname1 = "e%s_r%i" % (i,j)
            zmax = max(zmax, histos[hname1].GetMaximum())

        for j in xrange(0,15):
            hname1 = "e%s_r%i" % (i,j)
            if j == 0:
                histos[hname1].SetMaximum(zmax)
                histos[hname1].Draw("same pol colz")
                gPad.Modified(); gPad.Update()
                xy = (0.91, 0.13, 0.95, 0.95)
                paletteObj = histos[hname1].FindObject("palette")
                paletteObj.SetX1NDC(xy[0]); paletteObj.SetY1NDC(xy[1]); paletteObj.SetX2NDC(xy[2]); paletteObj.SetY2NDC(xy[3])
                paletteObj.SetTitleSize(0.024); paletteObj.SetLabelSize(0.024)
                gPad.Modified(); gPad.Update()
            else:
                histos[hname1].SetMaximum(zmax)
                histos[hname1].Draw("same pol col")
        if i < 18:
            latex.DrawLatex(0.62, 0.88, "Endcap +%i" % (i-10))
        else:
            latex.DrawLatex(0.62, 0.88, "Endcap -%i" % (i-17))
        save(imgdir, "rates_%s" % hname)


# ______________________________________________________________________________
# Rates (1D)

if sections["rates_by_layer"]:
    for hname, h in histos.iteritems():
        h.Reset()

    book1D("; layer index; rate", 25, 0, 25)

    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("TTStubs_modId", 1)
    tree.SetBranchStatus("TTStubs_barrel", 1)
    tree.SetBranchStatus("TTStubs_psmodule", 1)
    tree.SetBranchStatus("TTStubs_simPt", 1)
    tree.SetBranchStatus("TTStubs_simEta", 1)

    for ievt in tree:
        # Loop over stubs
        for moduleId, barrel, psmodule, simPt, simEta in zip(ievt.TTStubs_modId, ievt.TTStubs_barrel, ievt.TTStubs_psmodule, ievt.TTStubs_simPt, ievt.TTStubs_simEta):
            if simPt < 2 or abs(simEta) > 2.2:  continue

            ilay = (moduleId / 10000) % 100
            ilad = (moduleId / 100) % 100
            imod = (moduleId) % 100

            if barrel:
                if psmodule:
                    histos["b_ps"].Fill(ilay, 1)
                else:
                    histos["b_ss"].Fill(ilay, 1)
            else:
                if psmodule:
                    histos["e_ps"].Fill(ilay, 1)
                else:
                    histos["e_ss"].Fill(ilay, 1)

    draw1D()
    save(imgdir, "rates_by_layer")


if sections["rates_by_tower"]:
    for hname, h in histos.iteritems():
        h.Reset()

    book1D("; tower index; rate", 50, 0, 50)

    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("TTStubs_modId", 1)
    tree.SetBranchStatus("TTStubs_barrel", 1)
    tree.SetBranchStatus("TTStubs_psmodule", 1)
    tree.SetBranchStatus("TTStubs_simPt", 1)
    tree.SetBranchStatus("TTStubs_simEta", 1)

    for ievt in tree:
        # Loop over stubs
        for moduleId, barrel, psmodule, simPt, simEta in zip(ievt.TTStubs_modId, ievt.TTStubs_barrel, ievt.TTStubs_psmodule, ievt.TTStubs_simPt, ievt.TTStubs_simEta):
            if simPt < 2 or abs(simEta) > 2.2:  continue

            if moduleId not in ttmap_reversed:
                towers = [49]  # not listed in any trigger tower
            else:
                towers = ttmap_reversed[moduleId]

            for itower in towers:
                if barrel:
                    if psmodule:
                        histos["b_ps"].Fill(itower, 1)
                    else:
                        histos["b_ss"].Fill(itower, 1)
                else:
                    if psmodule:
                        histos["e_ps"].Fill(itower, 1)
                    else:
                        histos["e_ss"].Fill(itower, 1)

    # Draw
    draw1D()
    save(imgdir, "rates_by_tower")

# ______________________________________________________________________________
# Efficiencies

if sections["efficiencies"]:
    for hname, h in histos.iteritems():
        h.Reset()

    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("simPixelDigis_modId", 1)
    tree.SetBranchStatus("simPixelDigis_trkId", 1)
    tree.SetBranchStatus("simPixelDigis_chan" , 1)
    tree.SetBranchStatus("TTStubs_modId"      , 1)
    tree.SetBranchStatus("TTStubs_trkId"      , 1)
    tree.SetBranchStatus("TTStubs_hitChans"   , 1)

    for ievt in tree:
        # Loop over stubs
        for moduleId, trkId, chan in zip(ievt.simPixelDigis_modId, ievt.simPixelDigis_trkId, ievt.simPixelDigis_chan):
            if trkId != 1:  continue

            ilay = (moduleId / 10000) % 100
            ilad = (moduleId / 100) % 100
            imod = (moduleId) % 100

            for b_moduleId, b_trkId, b_hitChans in zip(ievt.TTStubs_modId, ievt.TTStubs_trkId, ievt.TTStubs_hitChans):
                if b_trkId != 1:  continue
                if moduleId != b_moduleId:  continue

                for h_chan in b_hitChans:
                    if chan != h_chan:  continue
                    print "FOUND: ", moduleId, chan





