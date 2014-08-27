#!/usr/bin/env python

from rootdrawing import *
from math import pi

# ______________________________________________________________________________
# Global

latex.SetTextSize(0.06)

# ______________________________________________________________________________
# Configurations

sections = {}
sections["rates"                ] = True
sections["rates_by_layer"       ] = True
sections["rates_by_tower"       ] = True
sections["efficiencies"         ] = True
sections["efficiencies_by_layer"] = False
#sections["efficiencies_by_tower"] = False

drawerInit = DrawerInit()
infiles = get_infiles("input.txt", fast=False)

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

histos = {}

# 2D
def book2D():
    for i in xrange(5,11):  # barrel
        histos["b%i" % i] = TH2F("b%i" % i, "; Barrel module index; Barrel ladder index", b_nmods[i-5], 0, b_nmods[i-5], b_nlads[i-5], 0, b_nlads[i-5])

    for i in xrange(11,23):  # endcap
        if i == 16 or i == 17:  continue
        histos["e%i" % i] = TH2F("e%i" % i, "; r: Endcap ring index+%i; #theta: Endcap module index" % e_offset, 2*e_nrings[0], -e_nrings[0], e_nrings[0], 2*e_nrings[0], -e_nrings[0], e_nrings[0])

        for j in xrange(0,15):  # endcap rings
            histos["e%i_r%i" % (i,j)] = TH2F("e%i_r%i" % (i,j), "", e_nmods[j], 0, 2*pi, e_nrings[0], 0, e_nrings[0])

# 1D
def book1D(xtitle="", nbinsx=50, xlow=0, xup=50):
    for i, j in enumerate(["b_ps", "b_ss", "e_ps", "e_ss"]):
        h = TH1F(j, xtitle, nbinsx, xlow, xup)
        h.SetLineWidth(2)
        h.SetLineColor(palette[i])
        h.SetMarkerColor(palette[i])
        h.SetFillColor(lightpalette[i])
        histos[j] = h

spalette = [sOrange, sMagenta, sViolet, sBlue, sCyan, sGreen]
def bookEff1D(isPt, xtitle="", nbinsx=50, xlow=0, xup=50):
    for i in xrange(5,16):
        j = "l%i_pt" % i if isPt else "l%i_eta" % i
        h = TH1F(j, xtitle, nbinsx, xlow, xup)
        h.SetLineWidth(2)
        h.SetLineColor(spalette[(i-5)%6])
        h.SetMarkerColor(spalette[(i-5)%6])
        histos[j] = h

def draw1D():
    # Stack them by hand
    histos["b_ss"].Add(histos["b_ps"])
    histos["e_ps"].Add(histos["b_ss"])
    histos["e_ss"].Add(histos["e_ps"])

    # Draw
    draws([histos["e_ss"], histos["e_ps"], histos["b_ss"], histos["b_ps"]], stats=False)

def drawEff1D(isPt, isBarrel):
    if isBarrel:
        h = histos["l%i_pt" % 5 if isPt else "l%i_eta" % 5]
    else:
        h = histos["l%i_pt" % 11 if isPt else "l%i_eta" % 11]
    h.SetStats(0)
    h.SetMaximum(1.1)
    h.SetMinimum(0.)
    h.Draw("e1")
    if isBarrel:
        for i in xrange(6,11):
            h = histos["l%i_pt" % i if isPt else "l%i_eta" % i]
            h.Draw("e1 same")
    else:
        for i in xrange(12,16):
            h = histos["l%i_pt" % i if isPt else "l%i_eta" % i]
            h.Draw("e1 same")
    CMS_label()


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
    book2D()

    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("TTStubs_modId"      , 1)
    tree.SetBranchStatus("genParts_pt"        , 1)
    tree.SetBranchStatus("genParts_eta"       , 1)

    for ievt in tree:
        # Check genParticle
        if ievt.genParts_pt[0] < 2 or abs(ievt.genParts_eta[0]) > 2.2:
            continue

        # Loop over stubs
        for moduleId in ievt.TTStubs_modId:
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
    book1D("; layer index; rate", 25, 0, 25)

    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("TTStubs_modId"      , 1)
    tree.SetBranchStatus("TTStubs_barrel"     , 1)
    tree.SetBranchStatus("TTStubs_psmodule"   , 1)
    tree.SetBranchStatus("genParts_pt"        , 1)
    tree.SetBranchStatus("genParts_eta"       , 1)

    for ievt in tree:
        # Check genParticle
        if ievt.genParts_pt[0] < 2 or abs(ievt.genParts_eta[0]) > 2.2:
            continue

        # Loop over stubs
        for moduleId, barrel, psmodule in zip(ievt.TTStubs_modId, ievt.TTStubs_barrel, ievt.TTStubs_psmodule):
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
    book1D("; tower index; rate", 50, 0, 50)

    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("TTStubs_modId"      , 1)
    tree.SetBranchStatus("TTStubs_barrel"     , 1)
    tree.SetBranchStatus("TTStubs_psmodule"   , 1)
    tree.SetBranchStatus("genParts_pt"        , 1)
    tree.SetBranchStatus("genParts_eta"       , 1)

    for ievt in tree:
        # Check genParticle
        if ievt.genParts_pt[0] < 2 or abs(ievt.genParts_eta[0]) > 2.2:
            continue

        # Loop over stubs
        for moduleId, barrel, psmodule in zip(ievt.TTStubs_modId, ievt.TTStubs_barrel, ievt.TTStubs_psmodule):

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
    book2D()

    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("simPixelDigis_modId", 1)
    tree.SetBranchStatus("simPixelDigis_trkId", 1)
    tree.SetBranchStatus("simPixelDigis_chan" , 1)
    tree.SetBranchStatus("TTStubs_modId"      , 1)
    tree.SetBranchStatus("TTStubs_trkId"      , 1)
    tree.SetBranchStatus("TTStubs_hitChans"   , 1)
    tree.SetBranchStatus("genParts_pt"        , 1)
    tree.SetBranchStatus("genParts_eta"       , 1)

    numerators = {}
    denominators = {}

    for ievt in tree:
        # Check genParticle
        if ievt.genParts_pt[0] < 2 or abs(ievt.genParts_eta[0]) > 2.2:
            continue

        # Loop over digis
        keys_denom = []
        keys_num = []
        for moduleId, trkId, chan in zip(ievt.simPixelDigis_modId, ievt.simPixelDigis_trkId, ievt.simPixelDigis_chan):
            if trkId != 1:  continue

            ilay = (moduleId / 10000) % 100
            ilad = (moduleId / 100) % 100
            imod = (moduleId) % 100

            key = (ilay, ilad, imod)
            if key not in keys_denom:
                keys_denom.append(key)

            for b_moduleId, b_trkId, b_hitChans in zip(ievt.TTStubs_modId, ievt.TTStubs_trkId, ievt.TTStubs_hitChans):
                if b_trkId != 1:  continue
                if moduleId != b_moduleId:  continue

                for h_chan in b_hitChans:
                    if chan != h_chan:  continue
                    if key not in keys_num:
                        keys_num.append(key)
                    break
                break

        for key in keys_denom:
            if key not in denominators:
                denominators[key] = 1
            else:
                denominators[key] += 1

        for key in keys_num:
            if key not in numerators:
                numerators[key] = 1
            else:
                numerators[key] += 1

    ratios = {}
    for k, d in denominators.iteritems():
        ilay = k[0]
        ilad = k[1]
        imod = k[2]

        if k in numerators:
            n = numerators[k]
            assert(n <= d)
            ratio = float(n) / float(d)
            error = sqrt(ratio * (1-ratio) / float(d))
            ratios[k] = ratio

            if ilay < 11:  # barrel
                b = histos["b%i" % (ilay)].FindFixBin(imod, ilad)
                histos["b%i" % (ilay)].SetBinContent(b, ratio)
                histos["b%i" % (ilay)].SetBinError(b, error)

            else:  # endcap
                irad = 2*pi * imod / e_nmods[ilad]
                b = histos["e%i_r%i" % (ilay,ilad)].FindFixBin(irad, ilad + e_offset)
                histos["e%i_r%i" % (ilay,ilad)].SetBinContent(b, ratio)
                histos["e%i_r%i" % (ilay,ilad)].SetBinError(b, error)

    nentries = tree.GetEntriesFast()
    print nentries

    # Barrel
    for i in xrange(5,11):
        hname = "b%i" % i
        histos[hname].SetMaximum(1.1)
        histos[hname].SetMinimum(0.)
        draw2D([histos[hname]], stats=False)
        latex.DrawLatex(0.68, 0.88, "Barrel %i" % (i-4))
        save(imgdir, "efficiencies_%s" % hname)

    # Endcap
    for i in xrange(11,23):
        if i == 16 or i == 17:  continue

        hname = "e%s" % i
        draw2D([histos[hname]], stats=False, palette=False)

        for j in xrange(0,15):
            hname1 = "e%s_r%i" % (i,j)
            if j == 0:
                histos[hname1].SetMaximum(1.1)
                histos[hname1].SetMinimum(0.)
                histos[hname1].Draw("same pol colz")
                gPad.Modified(); gPad.Update()
                xy = (0.91, 0.13, 0.95, 0.95)
                paletteObj = histos[hname1].FindObject("palette")
                paletteObj.SetX1NDC(xy[0]); paletteObj.SetY1NDC(xy[1]); paletteObj.SetX2NDC(xy[2]); paletteObj.SetY2NDC(xy[3])
                paletteObj.SetTitleSize(0.024); paletteObj.SetLabelSize(0.024)
                gPad.Modified(); gPad.Update()
            else:
                histos[hname1].SetMaximum(1.1)
                histos[hname1].SetMinimum(0.)
                histos[hname1].Draw("same pol col")
        if i < 18:
            latex.DrawLatex(0.62, 0.88, "Endcap +%i" % (i-10))
        else:
            latex.DrawLatex(0.62, 0.88, "Endcap -%i" % (i-17))
        save(imgdir, "efficiencies_%s" % hname)

# ______________________________________________________________________________
# Efficiencies (1D)

if sections["efficiencies_by_layer"]:
    bookEff1D(True, "; p_{T} [GeV]; efficiency", 50, 0, 100)
    bookEff1D(False, "; #eta; efficiency", 50, -2.5, 2.5)

    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("simPixelDigis_modId", 1)
    tree.SetBranchStatus("simPixelDigis_trkId", 1)
    tree.SetBranchStatus("simPixelDigis_chan" , 1)
    tree.SetBranchStatus("TTStubs_modId"      , 1)
    tree.SetBranchStatus("TTStubs_trkId"      , 1)
    tree.SetBranchStatus("TTStubs_hitChans"   , 1)
    tree.SetBranchStatus("genParts_pt"        , 1)
    tree.SetBranchStatus("genParts_eta"       , 1)

    numerators = {}
    denominators = {}

    for ievt in tree:
        # Check genParticle
        ipt = int(ievt.genParts_pt[0]+0.5)  # round up
        scaled_eta = (10.*ievt.genParts_eta[0])
        ieta = int(scaled_eta+0.5 if scaled_eta > 0 else scaled_eta-0.5)
        #print ievt.genParts_pt[0], ievt.genParts_eta[0], ipt, ieta

        # Loop over digis
        keys_denom = []
        keys_num = []
        for moduleId, trkId, chan in zip(ievt.simPixelDigis_modId, ievt.simPixelDigis_trkId, ievt.simPixelDigis_chan):
            if trkId != 1:  continue

            ilay = (moduleId / 10000) % 100
            ilad = (moduleId / 100) % 100
            imod = (moduleId) % 100

            if ilay >= 18:  ilay -= 7  # combine positive and negative sides

            key = (ilay, ipt, ieta)
            if key not in keys_denom:
                keys_denom.append(key)

            for b_moduleId, b_trkId, b_hitChans in zip(ievt.TTStubs_modId, ievt.TTStubs_trkId, ievt.TTStubs_hitChans):
                if b_trkId != 1:  continue
                if moduleId != b_moduleId:  continue

                for h_chan in b_hitChans:
                    if chan != h_chan:  continue
                    if key not in keys_num:
                        keys_num.append(key)
                    break
                break

        for key in keys_denom:
            if key not in denominators:
                denominators[key] = 1
            else:
                denominators[key] += 1

        for key in keys_num:
            if key not in numerators:
                numerators[key] = 1
            else:
                numerators[key] += 1

    ratios = {}
    for k, d in denominators.iteritems():
        ilay = k[0]
        ipt = k[1]
        ieta = k[2]

        if k in numerators:
            n = numerators[k]
            assert(n <= d)
            ratio = float(n) / float(d)
            error = sqrt(ratio * (1-ratio) / float(d))
            ratios[k] = ratio

            pt = float(ipt)
            eta = float(ieta)/10

            if abs(eta) <= 2.5:
                b = histos["l%i_pt" % (ilay)].FindFixBin(pt)
                histos["l%i_pt" % (ilay)].SetBinContent(b, ratio)
                histos["l%i_pt" % (ilay)].SetBinError(b, error)

            if pt >= 2:
                b = histos["l%i_eta" % (ilay)].FindFixBin(eta)
                histos["l%i_eta" % (ilay)].SetBinContent(b, ratio)
                histos["l%i_eta" % (ilay)].SetBinError(b, error)

    drawEff1D(True, True)
    leg1 = TLegend(0.60,0.16,0.94,0.40)
    leg1.SetFillStyle(0); leg1.SetLineColor(0); leg1.SetShadowColor(0); leg1.SetBorderSize(0)
    leg1.AddEntry(histos["l5_pt"], "Barrel layer 1")
    leg1.AddEntry(histos["l6_pt"], "Barrel layer 2")
    leg1.AddEntry(histos["l7_pt"], "Barrel layer 3")
    leg1.AddEntry(histos["l8_pt"], "Barrel layer 4")
    leg1.AddEntry(histos["l9_pt"], "Barrel layer 5")
    leg1.AddEntry(histos["l10_pt"], "Barrel layer 6")
    leg1.Draw()
    save(imgdir, "efficiencies_by_layer_pt_barrel")

    drawEff1D(False, True)
    leg1.Draw()
    save(imgdir, "efficiencies_by_layer_eta_barrel")

    drawEff1D(True, False)
    leg1 = TLegend(0.56,0.20,0.94,0.40)
    leg1.SetFillStyle(0); leg1.SetLineColor(0); leg1.SetShadowColor(0); leg1.SetBorderSize(0)
    leg1.AddEntry(histos["l11_pt"], "Endcap disks +/-1")
    leg1.AddEntry(histos["l12_pt"], "Endcap disks +/-2")
    leg1.AddEntry(histos["l13_pt"], "Endcap disks +/-3")
    leg1.AddEntry(histos["l14_pt"], "Endcap disks +/-4")
    leg1.AddEntry(histos["l15_pt"], "Endcap disks +/-5")
    leg1.Draw()
    save(imgdir, "efficiencies_by_layer_pt_endcap")

    drawEff1D(False, False)
    leg1.Draw()
    save(imgdir, "efficiencies_by_layer_eta_endcap")

