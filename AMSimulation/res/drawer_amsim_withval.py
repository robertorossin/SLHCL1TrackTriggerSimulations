#!/usr/bin/env python

from rootdrawing import *
import os

"""
Dedicated script for coverage studies where there is no pileup
"""

# ______________________________________________________________________________
# Global

latex.SetTextSize(0.036)

# ______________________________________________________________________________
# Configurations

samples = {}
samples["nu140"     ] = False
samples["tt27"      ] = False
samples["tt27_nu140"] = True

sections = {}
sections["coverage"     ] = False
sections["roads"        ] = True


drawerInit = DrawerInit()
gStyle.SetNumberContours(100)
gStyle.SetPalette(55)  # rainbow color map


chain = TChain("ntupler/tree", "")
chain_val = TChain("ntupler/tree", "")

EOS = "/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1_results/"
EOS_val = EOS.replace("_results","_results_val")

RES = "SingleMuPlusMinus_sp16_%s"
settings = [
    ("ss1024_20140821"    ,   8246923),
    ("ss512_20140821"     ,  17990289),
    ("ss256_20140821"     ,  39435020),
    ("ss128_20140821"     ,  76364339),
    ("ss64_20140821"      , 125888937),
    ("ss32_20140821"      , 178122282),
]

settings_barrel = [
    ("ss1024_20140821"    ,   276831),
    ("ss512_20140821"     ,   748757),
    ("ss256_20140821"     ,  2126119),
    ("ss128_20140821"     ,  5856686),
    ("ss64_20140821"      , 14496333),
    ("ss32_20140821"      , 32277270),
]

col  = TColor.GetColor("#1f78b4")  # Paired
fcol = TColor.GetColor("#a6cee3")
imgdir = "figures_amsim/"


if samples["tt27"]:
    RES = "SingleMuMinus_Barrel_sp16_%s"
    settings = settings_barrel

if samples["nu140"]:
    EOS = EOS.replace("_results","_results_PU140bx25")
    RES = "Neutrino_sp16_%s"

    col = TColor.GetColor("#e31a1c")  # Paired
    fcol = TColor.GetColor("#fb9a99")
    imgdir = "figures_amsim_nu140/"

if samples["tt27_nu140"]:
    EOS = EOS.replace("_results","_results_PU140bx25")
    RES = "Neutrino_Barrel_sp16_%s"
    settings = settings_barrel

    col = TColor.GetColor("#e31a1c")  # Paired
    fcol = TColor.GetColor("#fb9a99")
    imgdir = "figures_amsim_nu140/"

if not imgdir.endswith("/"):  imgdir += "/"
if gSystem.AccessPathName(imgdir):
    gSystem.mkdir(imgdir)


# ______________________________________________________________________________
# Coverage

if sections["coverage"]:

    if samples["nu140"] or samples["tt27_nu140"]:
        print "ERROR"
        exit

    def bookCoverage():
        histos = {}

        # pt
        for i in xrange(3):
            hname = "pt_%i" % i
            h = TProfile(hname, "; p_{T} [GeV]; coverage", 100, 0., 100., 0., 2., "")
            h.SetMarkerColor(blkrgb[i % 3])
            h.SetLineColor(blkrgb[i % 3])
            histos[hname] = h
            histos[hname + "_val"] = h.Clone(hname + "_val")

        # eta
        for i in xrange(3):
            hname = "eta_%i" % i
            h = TProfile(hname, "; #eta; coverage", 120, -3.0, 3.0, 0., 2., "")
            h.SetMarkerColor(blkrgb[i % 3])
            h.SetLineColor(blkrgb[i % 3])
            histos[hname] = h
            histos[hname + "_val"] = h.Clone(hname + "_val")

        # phi
        for i in xrange(3):
            hname = "phi_%i" % i
            h = TProfile(hname, "; #phi; coverage", 128, -3.2, 3.2, 0., 2., "")
            h.SetMarkerColor(blkrgb[i % 3])
            h.SetLineColor(blkrgb[i % 3])
            histos[hname] = h
            histos[hname + "_val"] = h.Clone(hname + "_val")

        # vz
        for i in xrange(3):
            hname = "vz_%i" % i
            h = TProfile(hname, "; vertex z_{0} [cm]; coverage", 120, -30, 30, 0., 2., "")
            h.SetMarkerColor(blkrgb[i % 3])
            h.SetLineColor(blkrgb[i % 3])
            histos[hname] = h
            histos[hname + "_val"] = h.Clone(hname + "_val")

        for k, v in histos.iteritems():
            v.SetFillStyle(0)
            v.SetMarkerStyle(24)
            v.SetMarkerSize(0.9)
            v.SetLineWidth(2)

        # eta-phi map
        hname = "etaphi_0"
        h = TProfile2D(hname, "; #eta; #phi; coverage", 60, -3, 3, 64, -3.2, 3.2, 0., 2., "")
        histos[hname] = h
        return histos

    def projectCoverage(tree, histos, is_val):
        val = "" if not is_val else "_val"

        # Loop over events
        for i_ievt, ievt in enumerate(tree):
            pt, eta, phi, vz = ievt.genParts_pt[0], ievt.genParts_eta[0], ievt.genParts_phi[0], ievt.genParts_vz[0]

            fired = (ievt.AMTTRoads_nHitLayers.size() > 0)

            if abs(eta) < 0.8:
                histos["pt_0"  + val].Fill(pt , fired)
            elif abs(eta) < 1.6:
                histos["pt_1"  + val].Fill(pt , fired)
            elif abs(eta) < 2.2:
                histos["pt_2"  + val].Fill(pt , fired)
            else:
                pass

            if pt >= 20:
                histos["eta_0" + val].Fill(eta, fired)
                histos["phi_0" + val].Fill(phi, fired)
                histos["vz_0"  + val].Fill(vz, fired)
            elif pt >= 5:
                histos["eta_1" + val].Fill(eta, fired)
                histos["phi_1" + val].Fill(phi, fired)
                histos["vz_1"  + val].Fill(vz, fired)
            elif pt >= 2:
                histos["eta_2" + val].Fill(eta, fired)
                histos["phi_2" + val].Fill(phi, fired)
                histos["vz_2"  + val].Fill(vz, fired)
            else:
                pass

            if not is_val and pt >= 20:
                histos["etaphi_0"].Fill(eta, phi, fired)

    def drawCoverage(histos, setting, banksize, imgdir, result):
        for var in ["pt", "eta", "phi", "vz"]:
            h = histos["%s_0_val" % var]
            h.SetStats(0); h.SetMinimum(0); h.SetMaximum(1.2)

            g = TGraphAsymmErrors(h)
            for i in xrange(g.GetN()):
                g.SetPointError(i, 1, 1, 0, 10)
            g.SetFillStyle(1001)
            g.SetFillColor(kGray2)
            histos["%s_0_val_g"] = g
            h.Reset()

            h.Draw()
            g.Draw("3")
            histos["%s_0" % var].Draw("same")
            histos["%s_1" % var].Draw("same")
            histos["%s_2" % var].Draw("same")

            xmin, xmax = h.GetXaxis().GetXmin(), h.GetXaxis().GetXmax()
            line.DrawLine(xmin, 1.0, xmax, 1.0)
            line.DrawLine(xmin, 0.4, xmax, 0.4)

            if var == "pt":
                leg1 = TLegend(0.42,0.22,0.94,0.39)
                leg1.SetTextFont(42)
                leg1.SetFillStyle(0); leg1.SetLineColor(0); leg1.SetShadowColor(0); leg1.SetBorderSize(0)
                leg1.AddEntry(histos["%s_0" % var], "Single muon 0.0 #leq |#eta| < 0.8", "lp")
                leg1.AddEntry(histos["%s_1" % var], "Single muon 0.8 #leq |#eta| < 1.6", "lp")
                leg1.AddEntry(histos["%s_2" % var], "Single muon 1.6 #leq |#eta| < 2.2", "lp")
                leg1.Draw()
            else:
                leg1 = TLegend(0.42,0.22,0.94,0.39)
                leg1.SetTextFont(42)
                leg1.SetFillStyle(0); leg1.SetLineColor(0); leg1.SetShadowColor(0); leg1.SetBorderSize(0)
                leg1.AddEntry(histos["%s_0" % var], "Single muon 20 #leq p_{T} < #infty  GeV", "lp")
                leg1.AddEntry(histos["%s_1" % var], "Single muon   5 #leq p_{T} < 20 GeV", "lp")
                leg1.AddEntry(histos["%s_2" % var], "Single muon   2 #leq p_{T} <   5 GeV", "lp")
                leg1.Draw()

            latex.DrawLatex(0.55, 0.185, "%s [%.1fM bank]" % (setting, banksize))

            CMS_label()
            save(imgdir, "coverage_vs_%s_%s" % (var, result))

        oldRightMargin = gPad.GetRightMargin()
        h = histos["etaphi_0"]
        h.SetStats(0); h.SetMinimum(0); h.SetMaximum(1.2)
        draw2D([h], stats=False)

        latex.DrawLatex(0.5, 0.22, "Single muon p_{T} > 20 GeV")
        latex.DrawLatex(0.5, 0.185, "%s [%.1fM bank]" % (setting, banksize))
        save(imgdir, "coverage_vs_etaphi_%s" % (result))

        gPad.SetRightMargin(oldRightMargin)

    # __________________________________________________________________________
    histos = {}
    for sett, sett1 in settings[0:]:
        chain.Reset()
        chain_val.Reset()
        histos.clear()

        result = RES % sett
        infiles = map(lambda x: EOS+"/"+result+"/"+x, os.listdir(EOS+"/"+result))
        infiles_val = map(lambda x: EOS_val+"/"+result+"/"+x, os.listdir(EOS_val+"/"+result))
        print sett, len(infiles), len(infiles_val)
        for f in infiles:
            chain.Add(f)
        for f in infiles_val:
            chain_val.Add(f)

        histos = bookCoverage()
        projectCoverage(chain, histos, is_val=False)
        projectCoverage(chain_val, histos, is_val=True)

        setting = sett[:-9].replace("_", " ")
        banksize = 1e-6*sett1
        drawCoverage(histos, setting, banksize, imgdir, result)

# ______________________________________________________________________________
# Roads

if sections["roads"]:

    def bookRoads():
        histos = {}

        hname = "nroads_per_event"
        h = TH1F(hname, "; # roads per event", 60, 0, 60)
        histos[hname] = h

        hname = "nsuperstrips_per_road"
        h = TH1F(hname, "; # superstrips per road", 20, 0, 20)
        histos[hname] = h

        hname = "nstubs_per_superstrip"
        h = TH1F(hname, "; # stubs per superstrip", 20, 0, 20)
        histos[hname] = h

        hname = "nstubs_per_road"
        h = TH1F(hname, "; # stubs per road", 40, 0, 40)
        histos[hname] = h

        hname = "ncombinations_per_road"
        h = TH1F(hname, "; # combinations per road", 50, 0, 200)
        histos[hname] = h

        hname = "ncombinations_per_event"
        h = TH1F(hname, "; # combinations per event", 60, 0, 300)
        histos[hname] = h

        for k, v in histos.iteritems():
            v.SetLineColor(col)
            v.SetFillColor(fcol)
        return histos

    def projectRoads(tree, histos):
        for i_ievt, ievt in enumerate(tree):

            nroads_per_event = 0
            ncombinations_per_event = 0

            for nHitLayers, superstripIds in izip(ievt.AMTTRoads_nHitLayers, ievt.AMTTRoads_hitSuperstripIds):
                ssmap = {}
                for ss in superstripIds:
                    if ss not in ssmap:
                        ssmap[ss] = 1
                    else:
                        ssmap[ss] += 1

                nsuperstrips_per_road = len(ssmap)
                nroads_per_event += 1

                assert(nsuperstrips_per_road == nHitLayers)
                histos["nsuperstrips_per_road"].Fill(nsuperstrips_per_road)

                nstubs_per_road = 0
                ncombinations_per_road = 1.

                for k, v in ssmap.iteritems():
                    nstubs_per_superstrip = v
                    nstubs_per_road += v
                    ncombinations_per_road *= v

                    histos["nstubs_per_superstrip"].Fill(nstubs_per_superstrip)

                ncombinations_per_event += ncombinations_per_road

                assert(nstubs_per_road == superstripIds.size())
                histos["nstubs_per_road"].Fill(nstubs_per_road)
                histos["ncombinations_per_road"].Fill(ncombinations_per_road)

            assert(nroads_per_event == ievt.AMTTRoads_nHitLayers.size())
            histos["nroads_per_event"].Fill(nroads_per_event)
            histos["ncombinations_per_event"].Fill(ncombinations_per_event)

    def drawRoads(histos, setting, banksize, imgdir, result, logy=False):
        for hname, h in histos.iteritems():
            draw([h], ytitle="", logy=logy)

            latex.DrawLatex(0.62, 0.185, "%s [%.1fM bank]" % (setting, banksize))
            save(imgdir, "%s_%s" % (hname, result))

    # __________________________________________________________________________
    histos = {}
    for sett, sett1 in settings[0:]:
        chain.Reset()
        histos.clear()

        result = RES % sett
        infiles = map(lambda x: EOS+"/"+result+"/"+x, os.listdir(EOS+"/"+result))
        print sett, len(infiles)
        for f in infiles:
            chain.Add(f)

        histos = bookRoads()
        projectRoads(chain, histos)

        setting = sett[:-9].replace("_", " ")
        banksize = 1e-6*sett1
        drawRoads(histos, setting, banksize, imgdir, result, logy=True)

