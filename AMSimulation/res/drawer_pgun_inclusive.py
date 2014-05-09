#!/usr/bin/env python

from ROOT import TH1, TH1F, TH2F, TProfile, TFile, TChain, TCanvas, TLegend, TLatex, TLine, gROOT, gInterpreter, gStyle, gSystem, gPad
from rootcolors import *
from math import sqrt
#from operator import itemgetter
#from ordereddict import OrderedDict
#import numpy

# For init
class DrawerInit:
    def __init__(self):
        # ROOT
        gROOT.LoadMacro("tdrstyle.C")
        gROOT.LoadMacro("Helper.h")
        gROOT.LoadMacro("../src/AMSimulationLinkDef.h")
        gROOT.ProcessLine("setTDRStyle()")

        gStyle.SetEndErrorSize(2)
        gStyle.SetLabelSize(0.04, "Y")

        gStyle.SetOptStat(111110)
        gStyle.SetStatX(0.94)
        gStyle.SetStatY(0.93)
        gStyle.SetStatH(0.20)
        gStyle.SetStatW(0.24)

        TH1.SetDefaultSumw2()

# ______________________________________________________________________________
# Text
latex = TLatex()
latex.SetNDC()
latex.SetTextFont(42)
latex.SetTextSize(0.026)

line = TLine()
line.SetLineColor(kGray+2)
line.SetLineStyle(2)

# Very nice color palette stolen from tkgeometry
# https://code.google.com/p/tkgeometry/source/browse/trunk/src/Palette.cc
palette      = map(lambda x: TColor.GetColor(x), ("#004586","#FF420E","#579D1C","#FFD320","#7E0021","#83CAFF","#314004","#AECF00","#4B1F6F","#FF950E","#C5000B","#0084D1"))
lightpalette = map(lambda x: TColor.GetColor(x), ("#79B7F2","#F29379","#B1F279","#FFEDA6","#F27999","#C2DEF2","#D4F279","#DFF279","#C791F2","#F2BD79","#F27980","#AAD7F2"))

# ______________________________________________________________________________
# Configurations

# FIXME: factorize this
drawerInit = DrawerInit()
chain = TChain("ntupler/tree", "")
infiles = [
    "root://cmsxrootd-site.fnal.gov//store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140420.root",
    ]
for f in infiles:
    chain.Add(f)
tree = chain

sections = {}
sections["genParts"   ] = False
sections["simTracks"  ] = False
sections["simVertices"] = False
sections["trkParts"   ] = False
sections["trkVertices"] = False
sections["eventInfo"  ] = False
sections["geometry"   ] = False
sections["clusters"   ] = False
sections["stubs"      ] = False
sections["fixme"      ] = True

imgdir = "figures_pgun_inclusive/"
if not imgdir.endswith("/"):  imgdir += "/"
if gSystem.AccessPathName(imgdir):
    gSystem.mkdir(imgdir)


# ______________________________________________________________________________
# Functions

# Book
def book(params, binning):
    histos = []
    for i, p in enumerate(params):
        h = TH1F("h_"+p[0], "; "+binning[0], binning[1], binning[2], binning[3])
        h.SetLineWidth(2)
        h.SetMarkerSize(0)
        h.SetLineColor(p[1])
        h.SetMarkerColor(p[1])
        h.SetFillColor(p[2])
        histos.append(h)
    return histos

def book4(params, binning):
    histos = []
    for i, p in enumerate(params):
        for j in xrange(4):
            h = TH1F(("h%i_" % j)+p[0], "; "+binning[0], binning[1], binning[2], binning[3])
            h.SetLineWidth(2)
            h.SetMarkerSize(0)
            h.SetLineColor(palette[j])
            h.SetMarkerColor(palette[j])
            h.SetFillColor(lightpalette[j])
            histos.append(h)
    return histos

def book_prof(params, binning, option=""):
    histos = []
    for i, p in enumerate(params):
        h = TProfile("p_"+p[0], "; "+binning[0], binning[1], binning[2], binning[3], binning[4], binning[5])
        h.SetLineWidth(2)
        h.SetLineColor(p[1])
        h.SetMarkerColor(p[1])
        histos.append(h)
    if "s" in option:
        for h in histos:
            h.SetErrorOption("s")
    return histos

def book_scat(params, binning, ybinnings):
    histos = []
    for i, (p, ybinning) in enumerate(zip(params, ybinnings)):
        h = TH2F("h2_"+p[0], "; "+binning[0]+"; "+ybinning[0], binning[1], binning[2], binning[3], ybinning[1], ybinning[2], ybinning[3])
        histos.append(h)
    return histos

# Project
def fixOverflow(h):
    nbins = h.GetNbinsX()
    if h.GetBinContent(nbins+1) > 0:
        h.SetBinContent(nbins, h.GetBinContent(nbins) + h.GetBinContent(nbins+1))
        h.SetBinError(nbins, sqrt(h.GetBinError(nbins)**2 + h.GetBinError(nbins+1)**2))
        h.SetBinContent(nbins+1, 0)
        h.SetBinError(nbins+1, 0)
        h.SetEntries(h.GetEntries() - 2)  # SetBinContent() automatically increases # entries by one

def fixUnderflow(h):
    if h.GetBinContent(0) > 0:
        h.SetBinContent(1, h.GetBinContent(0) + h.GetBinContent(1))
        h.SetBinError(1, sqrt(h.GetBinError(0)**2 + h.GetBinError(1)**2))
        h.SetBinContent(0, 0)
        h.SetBinError(0, 0)
        h.SetEntries(h.GetEntries() - 2)  # SetBinContent() automatically increases # entries by one

def project(params, histos, normalize=-1, drawOverflow=True, drawUnderflow=True):
    for i, p in enumerate(params):
        tree.Project("h_"+p[0], p[3], p[4], "goff")
        if normalize > 0:
            histos[i].Scale(normalize / histos[i].GetSumOfWeights())
        if drawOverflow:
            fixOverflow(histos[i])
        if drawUnderflow:
            fixUnderflow(histos[i])
    return

def project4(params, histos, normalize=-1, drawOverflow=True, drawUnderflow=True, exclusive=False):
    tt = "TTStubs" if params[0][4].startswith("TTStubs") else "TTClusters"
    if exclusive:
        selections = [
            " %s_barrel &&  %s_psmodule" % (tt,tt),
            " %s_barrel && !%s_psmodule" % (tt,tt),
            "!%s_barrel &&  %s_psmodule" % (tt,tt),
            "!%s_barrel && !%s_psmodule" % (tt,tt),
        ]
    else:
        selections = [
            " %s_barrel &&  %s_psmodule" % (tt,tt),
            " %s_barrel" % (tt),
            " %s_barrel ||  %s_psmodule" % (tt,tt),
            " 1",
        ]
    for i, p in enumerate(params):
        for j in xrange(4):
            tree.Project(("h%i_" % j)+p[0], p[3], selections[j], "goff")
            if normalize > 0:
                histos[j].Scale(normalize / histos[j].GetSumOfWeights())
            if drawOverflow:
                fixOverflow(histos[j])
            if drawUnderflow:
                fixUnderflow(histos[j])
    return

def project_prof(params, histos):
    for i, p in enumerate(params):
        # Error is standard error of the mean, e(J)  =  s(J)/sqrt(L(J))
        tree.Project("p_"+p[0], p[3], p[4], "prof goff")
    return

def project_scat(params, histos):
    for i, p in enumerate(params):
        tree.Project("h2_"+p[0], p[3], p[4], "goff")
    return

# Draw
def CMS_label():
    old = (latex.GetTextFont(), latex.GetTextSize())
    latex.SetTextFont(62); latex.SetTextSize(0.028)
    latex.DrawLatex(0.670, 0.968, "CMS Very Preliminary")
    latex.SetTextFont(old[0]); latex.SetTextSize(old[1])
    return

def draw(params, histos, ytitle="Events", logx=False, logy=False, text=False):
    ymax = getMaximum(histos)
    histos[0].SetMaximum(ymax * 1.4)
    if not logy:  histos[0].SetMinimum(0.)
    histos[0].GetYaxis().SetTitle(ytitle)
    histos[0].Draw("hist")
    gPad.SetLogx(logx); gPad.SetLogy(logy)
    if text:
        histos[0].SetMarkerSize(2)
        histos[0].Draw("hist text0")
    CMS_label()
    return

def draw4(params, histos, ytitle="Events", logx=False, logy=False, exclusive=False):
    ymax = getMaximum(histos)
    histos[3].SetMaximum(ymax * 1.4)
    if not logy:  histos[0].SetMinimum(0.)
    histos[3].GetYaxis().SetTitle(ytitle)
    if exclusive:
        for h in histos:
            h.SetFillStyle(3003)
        histos[3].Draw("hist")
        histos[2].Draw("hist same")
        histos[1].Draw("hist same")
        histos[0].Draw("hist same")
    else:
        histos[3].Draw("hist")
        histos[2].Draw("hist same")
        histos[1].Draw("hist same")
        histos[0].Draw("hist same")
    gPad.SetLogx(logx); gPad.SetLogy(logy)
    CMS_label()
    return

def draw_scat(params, histo, logx=False, logy=False, logz=False):
    histo.GetYaxis().SetTitleOffset(1)
    histo.Draw("COLZ")
    gPad.SetLogx(logx); gPad.SetLogy(logy); gPad.SetLogz(logz)
    gPad.SetLeftMargin(0.13); gPad.SetRightMargin(0.10)
    gPad.Modified(); gPad.Update()
    palette = histo.FindObject("palette")
    xy = (0.91, 0.13, 0.95, 0.95)
    palette.SetX1NDC(xy[0]); palette.SetY1NDC(xy[1]); palette.SetX2NDC(xy[2]); palette.SetY2NDC(xy[3])
    palette.SetTitleSize(0.024); palette.SetLabelSize(0.024)
    xy = (0.64, 0.70, 0.88, 0.93)
    stats = histo.FindObject("stats")
    stats.SetX1NDC(xy[0]); stats.SetY1NDC(xy[1]); stats.SetX2NDC(xy[2]); stats.SetY2NDC(xy[3])
    gPad.Modified(); gPad.Update()
    CMS_label()
    return (palette,stats)

def label(histos, legend=(0.70,0.74,0.96,0.94)):
    leg1 = TLegend(legend[0], legend[1], legend[2], legend[3])
    leg1.SetFillStyle(0); leg1.SetLineColor(0); leg1.SetShadowColor(0); leg1.SetBorderSize(0)
    leg1.Draw()
    return (leg1)

def getMaximum(histos):
    maxima = []
    for h in histos:
        maxima.append(h.GetMaximum())
    return max(maxima)

def save(imgdir, imgname):
    gPad.RedrawAxis()
    gPad.Print(imgdir+imgname+".pdf")
    gPad.Print(imgdir+imgname+".png")


# ______________________________________________________________________________
# genParticles
if sections["genParts"]:
    # hname, linecolor, fillcolor, varname, selection
    params = [ ("genParts_size", sBase03, ssBase3, "genParts_size", "") ]
    # xtitle, nbins, xlow, xup
    binning = ("muon # gen particles", 4, 0, 4)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("genParts_pt", sBase03, ssBase3, "genParts_pt", "") ]
    binning = ("muon gen p_{T} [GeV]", 50, 0, 100)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("genParts_oneoverpt", sBase03, ssBase3, "1.0/genParts_pt", "") ]
    binning = ("muon gen 1/p_{T} [1/GeV]", 50, 0, 1)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("genParts_eta", sBase03, ssBase3, "genParts_eta", "") ]
    binning = ("muon gen #eta", 60, -3, 3)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("genParts_phi", sBase03, ssBase3, "genParts_phi", "") ]
    binning = ("muon gen #phi [rad]", 64, -3.2, 3.2)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("genParts_E", sBase03, ssBase3, "genParts_E", "") ]
    binning = ("muon gen energy [GeV]", 50, 0, 1000)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("genParts_M", sBase03, ssBase3, "genParts_M", "") ]
    binning = ("muon gen mass [GeV]", 50, 0, 1)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("genParts_vx", sBase03, ssBase3, "genParts_vx", "") ]
    binning = ("muon gen vertex x_{0} [mm]", 60, -3, 3)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("genParts_vy", sBase03, ssBase3, "genParts_vy", "") ]
    binning = ("muon gen vertex y_{0} [mm]", 60, -3, 3)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("genParts_vz", sBase03, ssBase3, "genParts_vz", "") ]
    binning = ("muon gen vertex z_{0} [mm]", 60, -30, 30)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("genParts_pdgId", sBase03, ssBase3, "genParts_pdgId", "") ]
    binning = ("muon gen PDG id", 50, -25, 25)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("genParts_charge", sBase03, ssBase3, "genParts_charge", "") ]
    binning = ("muon gen charge", 8, -4, 4)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("genParts_status", sBase03, ssBase3, "genParts_status", "") ]
    binning = ("muon gen status", 4, 0, 4)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

# ______________________________________________________________________________
if sections["simTracks"]:
    params = [ ("simTracks_size", sBase03, ssBase3, "simTracks_size", "") ]
    binning = ("# simTracks", 50, 0, 100)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])
    # max = 1930, 1853, 1782

    params = [ ("simTracks_pt", sBase03, ssBase3, "simTracks_pt", "") ]
    binning = ("simTrack p_{T} [GeV]", 50, 0, 100)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("simTracks_oneoverpt", sBase03, ssBase3, "simTracks_pt==0 ? 9999 : 1.0/simTracks_pt", "") ]
    binning = ("simTrack 1/p_{T} [1/GeV]", 50, 0, 1)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("simTracks_eta", sBase03, ssBase3, "simTracks_eta", "") ]
    binning = ("simTrack #eta", 60, -3, 3)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("simTracks_phi", sBase03, ssBase3, "simTracks_phi", "") ]
    binning = ("simTrack #phi [rad]", 64, -3.2, 3.2)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("simTracks_pdgId", sBase03, ssBase3, "simTracks_pdgId", "") ]
    binning = ("simTrack PDG id", 50, -25, 25)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("simTracks_evtId", sBase03, ssBase3, "simTracks_evtId", "") ]
    binning = ("simTrack event id", 4, 0, 4)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("simTracks_trkId", sBase03, ssBase3, "simTracks_trkId", "") ]
    binning = ("simTrack id", 50, 0, 200)
    histos = book(params, binning); project(params, histos); draw(params, histos, logy=True); legs = label(histos)
    save(imgdir, params[0][0])
    # max = 2466515

    params = [ ("simTracks_vtxId", sBase03, ssBase3, "simTracks_vtxId", "") ]
    binning = ("simTrack vertex id", 50, 0, 100)
    histos = book(params, binning); project(params, histos); draw(params, histos, logy=True); legs = label(histos)
    save(imgdir, params[0][0])
    # max = 1560

    params = [ ("simTracks_genpId", sBase03, ssBase3, "simTracks_genpId", "") ]
    binning = ("simTrack genParticle id", 6, -2, 4)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    # Special attention
    params = [ ("sp_simTracks_trkId", sBase03, ssBase3, "simTracks_trkId", "") ]
    binning = ("simTrack track id", 50, 0, 200)
    histos = book(params, binning); project(params, histos); draw(params, histos, logy=True); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("sp_simTracks_vtxId", sBase03, ssBase3, "simTracks_vtxId", "") ]
    binning = ("simTrack vertex id", 50, 0, 100)
    histos = book(params, binning); project(params, histos); draw(params, histos, logy=True); legs = label(histos)
    save(imgdir, params[0][0])

    #params = [ ("sp_simTracks_trkId_vs_pt", sBase03, ssBase3, "simTracks_trkId:simTracks_pt" , "") ]
    #binning = ("simTrack p_{T} [GeV]", 50, 0, 100)
    #ybinnings = [ ("simTrack track id", 50, 0, 200) ]
    #histos = book_scat(params, binning, ybinnings); project_scat(params, histos); ret=draw_scat(params, histos[0])
    #save(imgdir, params[0][0])


# ______________________________________________________________________________
if sections["simVertices"]:
    params = [ ("simVertices_size", sBase03, ssBase3, "simVertices_size", "") ]
    binning = ("# simVertices", 50, 0, 100)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("simVertices_vx", sBase03, ssBase3, "simVertices_vx", "") ]
    binning = ("simVertex x_{0} [mm]", 60, -3, 3)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("simVertices_vy", sBase03, ssBase3, "simVertices_vy", "") ]
    binning = ("simVertex y_{0} [mm]", 60, -3, 3)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("simVertices_vz", sBase03, ssBase3, "simVertices_vz", "") ]
    binning = ("simVertex z_{0} [mm]", 60, -30, 30)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("simVertices_tof", sBase03, ssBase3, "simVertices_tof", "") ]
    binning = ("simVertex time of flight [s]", 50, -1e-7, 1e-7)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("simVertices_trkId", sBase03, ssBase3, "simVertices_trkId", "") ]
    binning = ("simVertex track id", 50, 0, 200)
    histos = book(params, binning); project(params, histos); draw(params, histos, logy=True); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("simVertices_vtxId", sBase03, ssBase3, "simVertices_vtxId", "") ]
    binning = ("simVertex vertex id", 50, 0, 100)
    histos = book(params, binning); project(params, histos); draw(params, histos, logy=True); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("simBeamSpot_vx", sBase03, ssBase3, "simBeamSpot_bx", "") ]
    binning = ("simBeamSpot x_{0} [mm]", 60, -3, 3)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("simBeamSpot_vy", sBase03, ssBase3, "simBeamSpot_by", "") ]
    binning = ("simBeamSpot y_{0} [mm]", 60, -3, 3)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("simBeamSpot_vz", sBase03, ssBase3, "simBeamSpot_bz", "") ]
    binning = ("simBeamSpot z_{0} [mm]", 60, -3, 3)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("simBeamSpot_sigmaZ", sBase03, ssBase3, "simBeamSpot_sigmaZ", "") ]
    binning = ("simBeamSpot #sigma_{z} [mm]", 50, 0, 0.1)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])


# ______________________________________________________________________________
if sections["trkParts"]:
    params = [ ("trkParts_size", sBase03, ssBase3, "trkParts_size", "") ]
    binning = ("# trkParticles", 50, 0, 100)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("trkParts_nSimTracks", sBase03, ssBase3, "@trkParts_trkIds.size()", "") ]
    binning = ("trkParticle # simTracks", 20, 0, 20)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("trkParts_pt", sBase03, ssBase3, "trkParts_pt", "") ]
    binning = ("trkParticle p_{T} [GeV]", 50, 0, 100)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("trkParts_oneoverpt", sBase03, ssBase3, "trkParts_pt==0 ? 9999 : 1.0/trkParts_pt", "") ]
    binning = ("trkParticle 1/p_{T} [1/GeV]", 50, 0, 1)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("trkParts_eta", sBase03, ssBase3, "trkParts_eta", "") ]
    binning = ("trkParticle #eta", 60, -3, 3)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("trkParts_phi", sBase03, ssBase3, "trkParts_phi", "") ]
    binning = ("trkParticle #phi [rad]", 64, -3.2, 3.2)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("trkParts_vx", sBase03, ssBase3, "trkParts_vx", "") ]
    binning = ("trkParticle x_{0} [mm]", 60, -3, 3)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("trkParts_vy", sBase03, ssBase3, "trkParts_vy", "") ]
    binning = ("trkParticle y_{0} [mm]", 60, -3, 3)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("trkParts_vz", sBase03, ssBase3, "trkParts_vz", "") ]
    binning = ("trkParticle z_{0} [mm]", 60, -30, 30)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("trkParts_pdgId", sBase03, ssBase3, "trkParts_pdgId", "") ]
    binning = ("trkParticle PDG id", 50, -25, 25)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("trkParts_evtId", sBase03, ssBase3, "trkParts_evtId", "") ]
    binning = ("trkParticle event id", 4, 0, 4)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("trkParts_trkId", sBase03, ssBase3, "trkParts_trkId", "") ]
    binning = ("trkParticle simTrack id", 50, 0, 200)
    histos = book(params, binning); project(params, histos); draw(params, histos, logy=True); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("trkParts_genpId", sBase03, ssBase3, "trkParts_genpId", "") ]
    binning = ("trkParticle genParticle id", 6, -2, 4)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("trkParts_nhits", sBase03, ssBase3, "trkParts_nhits", "") ]
    binning = ("trkParticle # hits in tracker + muon", 50, 0, 50)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("trkParts_ntkhits", sBase03, ssBase3, "trkParts_ntkhits", "") ]
    binning = ("trkParticle # hits in tracker", 50, 0, 50)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("trkParts_ntklayers", sBase03, ssBase3, "trkParts_ntklayers", "") ]
    binning = ("trkParticle # hits in tracker layers", 50, 0, 50)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    # Special attention

    #params = [ ("sp_trkParts_size_vs_simTracks_size", sBase03, ssBase3, "trkParts_size:simTracks_size" , "") ]
    #binning = ("# simTracks", 50, 0, 100)
    #ybinnings = [ ("# trkParticles", 50, 0, 100) ]
    #histos = book_scat(params, binning, ybinnings); project_scat(params, histos); ret=draw_scat(params, histos[0])
    #save(imgdir, params[0][0])
    #
    #params = [ ("sp_trkParts_trkId_vs_pt", sBase03, ssBase3, "trkParts_trkIds[][0]:trkParts_pt" , "") ]
    #binning = ("trkParticle p_{T} [GeV]", 50, 0, 100)
    #ybinnings = [ ("trkParticle simTrack id", 50, 0, 200) ]
    #histos = book_scat(params, binning, ybinnings); project_scat(params, histos); ret=draw_scat(params, histos[0])
    #save(imgdir, params[0][0])

# ______________________________________________________________________________
if sections["trkVertices"]:
    params = [ ("trkVertices_size", sBase03, ssBase3, "trkVertices_size", "") ]
    binning = ("# trkVertices", 50, 0, 100)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("trkVertices_nSimVertices", sBase03, ssBase3, "@trkVertices_vtxIds.size()", "") ]
    binning = ("trkVertex # simVertices", 20, 0, 20)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("trkVertices_vx", sBase03, ssBase3, "trkVertices_vx", "") ]
    binning = ("trkVertex x_{0} [mm]", 60, -3, 3)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("trkVertices_vy", sBase03, ssBase3, "trkVertices_vy", "") ]
    binning = ("trkVertex y_{0} [mm]", 60, -3, 3)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("trkVertices_vz", sBase03, ssBase3, "trkVertices_vz", "") ]
    binning = ("trkVertex z_{0} [mm]", 60, -30, 30)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("trkVertices_tof", sBase03, ssBase3, "trkVertices_tof", "") ]
    binning = ("trkVertex time of flight [s]", 50, -1e-7, 1e-7)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("trkVertices_vtxId", sBase03, ssBase3, "trkVertices_vtxId", "") ]
    binning = ("trkVertex simVertex id", 50, 0, 100)
    histos = book(params, binning); project(params, histos); draw(params, histos, logy=True); legs = label(histos)
    save(imgdir, params[0][0])

    # Special attention

    #params = [ ("sp_trkVertices_size_vs_simVertices_size", sBase03, ssBase3, "trkVertices_size:simVertices_size" , "") ]
    #binning = ("# simVertices", 50, 0, 100)
    #ybinnings = [ ("# trkVertices", 50, 0, 100) ]
    #histos = book_scat(params, binning, ybinnings); project_scat(params, histos); ret=draw_scat(params, histos[0])
    #save(imgdir, params[0][0])


# ______________________________________________________________________________
if sections["eventInfo"]:
    params = [ ("event_run", sBase03, ssBase3, "run", "") ]
    binning = ("run number", 4, 0, 4)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("event_lumi", sBase03, ssBase3, "lumi", "") ]
    binning = ("lumi section", 50, 1, 101)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("event_event", sBase03, ssBase3, "event", "") ]
    binning = ("event number", 50, 1, 10001)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("event_bx", sBase03, ssBase3, "bx", "") ]
    binning = ("bunch crossing", 4, -3, 1)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("gen_nPV", sBase03, ssBase3, "gen_nPV", "") ]
    binning = ("sim # PV", 50, 0, 150)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("gen_trueNPV", sBase03, ssBase3, "gen_trueNPV", "") ]
    binning = ("gen # PV", 50, 0, 150)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("gen_randomSeed", sBase03, ssBase3, "gen_randomSeed", "") ]  # bugged
    binning = ("generator seed", 50, 0, 100000)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])


# ______________________________________________________________________________
if sections["geometry"]:
    pass

# ______________________________________________________________________________
if sections["clusters"]:
    params = [ ("TTClusters_size", sBase03, ssBase3, "TTClusters_size", "") ]
    binning = ("# clusters", 100, 0, 100)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_x", sBase03, ssBase3, "TTClusters_x", "") ]
    binning = ("Cluster global pos x [cm]", 120, -120, 120)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_y", sBase03, ssBase3, "TTClusters_y", "") ]
    binning = ("Cluster global pos y [cm]", 120, -120, 120)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_z", sBase03, ssBase3, "TTClusters_z", "") ]
    binning = ("Cluster global pos z [cm]", 120, -300, 300)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_r", sBase03, ssBase3, "TTClusters_r", "") ]
    binning = ("Cluster global pos r [cm]", 60, 0, 120)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_phi", sBase03, ssBase3, "TTClusters_phi", "") ]
    binning = ("Cluster global pos phi [rad]", 64, -3.2, 3.2)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_localx", sBase03, ssBase3, "TTClusters_localx", "") ]
    binning = ("Cluster local pos x [unit?]", 50, -5, 5)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_localy", sBase03, ssBase3, "TTClusters_localy", "") ]
    binning = ("Cluster local pos y [unit?]", 50, -5, 5)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_coordx", sBase03, ssBase3, "TTClusters_coordx", "") ]
    binning = ("Cluster local coord x", 129, 0, 1024+8)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_coordy", sBase03, ssBase3, "TTClusters_coordy", "") ]
    binning = ("Cluster local coord y", 33, 0, 33)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_surfx", sBase03, ssBase3, "TTClusters_surfx", "") ]
    binning = ("Cluster surface pos x [cm]", 120, -120, 120)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_surfy", sBase03, ssBase3, "TTClusters_surfy", "") ]
    binning = ("Cluster surface pos y [cm]", 120, -120, 120)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_surfz", sBase03, ssBase3, "TTClusters_surfz", "") ]
    binning = ("Cluster surface pos z [cm]", 120, -300, 300)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    ##
    params = [ ("TTClusters_simPt", sBase03, ssBase3, "TTClusters_simPt", "") ]
    binning = ("Cluster simTrack p_{T} [GeV]", 50, 0, 100)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_simOneoverpt", sBase03, ssBase3, "TTClusters_simPt==0 ? 9999 : 1.0/TTClusters_simPt", "") ]
    binning = ("Cluster simTrack 1/p_{T} [1/GeV]", 50, 0, 1)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_simEta", sBase03, ssBase3, "TTClusters_simEta", "") ]
    binning = ("Cluster simTrack #eta", 60, -3, 3)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_simPhi", sBase03, ssBase3, "TTClusters_simPhi", "") ]
    binning = ("Cluster simTrack #phi [rad]", 64, -3.2, 3.2)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_trkId", sBase03, ssBase3, "TTClusters_trkId", "") ]
    binning = ("Cluster simTrack track id", 50, 0, 200)
    histos = book(params, binning); project(params, histos); draw(params, histos, logy=True)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_tpId", sBase03, ssBase3, "TTClusters_tpId", "") ]
    binning = ("Cluster trkParticle id", 50, 0, 200)
    histos = book(params, binning); project(params, histos); draw(params, histos, logy=True)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_pdgId", sBase03, ssBase3, "TTClusters_pdgId", "") ]
    binning = ("Cluster PDG id", 50, -25, 25)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_isGenuine", sBase03, ssBase3, "TTClusters_isGenuine", "") ]
    binning = ("Cluster is genuine", 4, 0, 4)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_isCombinatoric", sBase03, ssBase3, "TTClusters_isCombinatoric", "") ]
    binning = ("Cluster is combinatoric", 4, 0, 4)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_isUnknown", sBase03, ssBase3, "TTClusters_isUnknown", "") ]
    binning = ("Cluster is unknown", 4, 0, 4)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_modId", sBase03, ssBase3, "TTClusters_modId", "") ]
    binning = ("Cluster module id", 120, 0, 300000)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_iModLayer", sBase03, ssBase3, "TTClusters_iModLayer", "") ]
    binning = ("Cluster layer number", 30, 0, 30)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_iModLadder", sBase03, ssBase3, "TTClusters_iModLadder", "") ]
    binning = ("Cluster ladder number", 100, 0, 100)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_iModModule", sBase03, ssBase3, "TTClusters_iModModule", "") ]
    binning = ("Cluster module number", 100, 0, 200)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_iModPitchX", sBase03, ssBase3, "TTClusters_iModPitchX", "") ]
    binning = ("Cluster pitch x [unit?]", 10, 0, 10)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_iModPitchY", sBase03, ssBase3, "TTClusters_iModPitchY", "") ]
    binning = ("Cluster pitch y [unit?]", 10, 0, 10)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_iModCols", sBase03, ssBase3, "TTClusters_iModCols", "") ]
    binning = ("Cluster # columns in module", 33, 0, 32+1)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_iModRows", sBase03, ssBase3, "TTClusters_iModRows", "") ]
    binning = ("Cluster # rows in module", 100, 950, 1050)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_nhits", sBase03, ssBase3, "TTClusters_nhits", "") ]
    binning = ("Cluster # hits", 20, 0, 20)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_hitADCs", sBase03, ssBase3, "TTClusters_hitADCs", "") ]
    binning = ("Cluster hit ADC count (for each hit)", 129, 0, 256+2)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_hitChans", sBase03, ssBase3, "TTClusters_hitChans", "") ]
    binning = ("Cluster hit channel id (for each hit)", 125, 0, 2500000)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_hitCols", sBase03, ssBase3, "TTClusters_hitCols", "") ]
    binning = ("Cluster hit column number (for each hit)", 33, 0, 32+1)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_hitRows", sBase03, ssBase3, "TTClusters_hitRows", "") ]
    binning = ("Cluster hit row number (for each hit)", 129, 0, 1024+8)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_hitFracs", sBase03, ssBase3, "TTClusters_hitFracs", "") ]
    binning = ("Cluster hit charge fraction (for each hit)", 100, 0, 1)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_hitTrkIds", sBase03, ssBase3, "TTClusters_hitTrkIds", "") ]
    binning = ("Cluster hit simTrack track id (for each hit)", 50, 0, 200)
    histos = book(params, binning); project(params, histos); draw(params, histos, logy=True)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_barrel", sBase03, ssBase3, "TTClusters_barrel", "") ]
    binning = ("Cluster is in barrel", 4, 0, 4)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_psmodule", sBase03, ssBase3, "TTClusters_psmodule", "") ]
    binning = ("Cluster is a PS module", 4, 0, 4)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_stackId", sBase03, ssBase3, "TTClusters_stackId", "") ]
    binning = ("Cluster CMSSW module id", 150, 5E8, 5.3E8)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    ##
    params = [ ("TTClusters_geoId", sBase03, ssBase3, "TTClusters_geoId", "") ]
    binning = ("Cluster geographical id", 150, 3e8, 3.6e8)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_stack", sBase03, ssBase3, "TTClusters_stack", "") ]
    binning = ("Cluster stack member number", 4, 0 ,4)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])

    params = [ ("TTClusters_width", sBase03, ssBase3, "TTClusters_width", "") ]
    binning = ("Cluster width in xy", 16, 0 ,16)
    histos = book(params, binning); project(params, histos); draw(params, histos); legs = label(histos)
    save(imgdir, params[0][0])


# ______________________________________________________________________________
if sections["stubs"]:
    params = [ ("TTStubs_size", sBase03, ssBase3, "TTStubs_size", "") ]
    binning = ("# stubs", 50, 0, 50)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_x", sBase03, ssBase3, "TTStubs_x", "") ]
    binning = ("Stub global pos x [cm]", 120, -120, 120)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_y", sBase03, ssBase3, "TTStubs_y", "") ]
    binning = ("Stub global pos y [cm]", 120, -120, 120)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_z", sBase03, ssBase3, "TTStubs_z", "") ]
    binning = ("Stub global pos z [cm]", 120, -300, 300)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_r", sBase03, ssBase3, "TTStubs_r", "") ]
    binning = ("Stub global pos r [cm]", 60, 0, 120)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_phi", sBase03, ssBase3, "TTStubs_phi", "") ]
    binning = ("Stub global pos phi [rad]", 64, -3.2, 3.2)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_dirx", sBase03, ssBase3, "TTStubs_dirx", "") ]
    binning = ("Stub global dir x", 100, -2.5, 2.5)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_diry", sBase03, ssBase3, "TTStubs_diry", "") ]
    binning = ("Stub global dir y", 100, -2.5, 2.5)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_dirz", sBase03, ssBase3, "TTStubs_dirz", "") ]
    binning = ("Stub global dir z", 100, -2.5, 2.5)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_roughPt", sBase03, ssBase3, "TTStubs_roughPt", "") ]
    binning = ("Stub 'rough' p_{T} [GeV]", 100, 0, 100)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    ##
    params = [ ("TTStubs_simPt", sBase03, ssBase3, "TTStubs_simPt", "") ]
    binning = ("Stub simTrack p_{T} [GeV]", 50, 0, 100)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_simOneoverpt", sBase03, ssBase3, "TTStubs_simPt==0 ? 9999 : 1.0/TTStubs_simPt", "") ]
    binning = ("Stub simTrack 1/p_{T} [1/GeV]", 50, 0, 1)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_simEta", sBase03, ssBase3, "TTStubs_simEta", "") ]
    binning = ("Stub simTrack #eta", 60, -3, 3)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_simPhi", sBase03, ssBase3, "TTStubs_simPhi", "") ]
    binning = ("Stub simTrack #phi [rad]", 64, -3.2, 3.2)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_trkId", sBase03, ssBase3, "TTStubs_trkId", "") ]
    binning = ("Stub simTrack track id", 50, 0, 200)
    histos = book(params, binning); project(params, histos); draw(params, histos, logy=True)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_tpId", sBase03, ssBase3, "TTStubs_tpId", "") ]
    binning = ("Stub trkParticle id", 50, 0, 200)
    histos = book(params, binning); project(params, histos); draw(params, histos, logy=True)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_pdgId", sBase03, ssBase3, "TTStubs_pdgId", "") ]
    binning = ("Stub PDG id", 50, -25, 25)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_isGenuine", sBase03, ssBase3, "TTStubs_isGenuine", "") ]
    binning = ("Stub is genuine", 4, 0, 4)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_isCombinatoric", sBase03, ssBase3, "TTStubs_isCombinatoric", "") ]
    binning = ("Stub is combinatoric", 4, 0, 4)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_isUnknown", sBase03, ssBase3, "TTStubs_isUnknown", "") ]
    binning = ("Stub is unknown", 4, 0, 4)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_modId", sBase03, ssBase3, "TTStubs_modId", "") ]
    binning = ("Stub module id", 120, 0, 300000)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_iModLayer", sBase03, ssBase3, "TTStubs_iModLayer", "") ]
    binning = ("Stub layer number", 30, 0, 30)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_iModLadder", sBase03, ssBase3, "TTStubs_iModLadder", "") ]
    binning = ("Stub ladder number", 100, 0, 100)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_iModModule", sBase03, ssBase3, "TTStubs_iModModule", "") ]
    binning = ("Stub module number", 100, 0, 200)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_iModPitchX", sBase03, ssBase3, "TTStubs_iModPitchX", "") ]
    binning = ("Stub pitch x [unit?]", 10, 0, 10)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_iModPitchY", sBase03, ssBase3, "TTStubs_iModPitchY", "") ]
    binning = ("Stub pitch y [unit?]", 10, 0, 10)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_iModCols", sBase03, ssBase3, "TTStubs_iModCols", "") ]
    binning = ("Stub # columns in module", 33, 0, 32+1)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_iModRows", sBase03, ssBase3, "TTStubs_iModRows", "") ]
    binning = ("Stub # rows in module", 100, 950, 1050)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_nhits", sBase03, ssBase3, "TTStubs_nhits", "") ]
    binning = ("Stub # hits", 20, 0, 20)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_hitADCs", sBase03, ssBase3, "TTStubs_hitADCs", "") ]
    binning = ("Stub hit ADC count (for each hit)", 129, 0, 256+2)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_hitChans", sBase03, ssBase3, "TTStubs_hitChans", "") ]
    binning = ("Stub hit channel id (for each hit)", 125, 0, 2500000)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_hitCols", sBase03, ssBase3, "TTStubs_hitCols", "") ]
    binning = ("Stub hit column number (for each hit)", 33, 0, 32+1)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_hitRows", sBase03, ssBase3, "TTStubs_hitRows", "") ]
    binning = ("Stub hit row number (for each hit)", 129, 0, 1024+8)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_hitFracs", sBase03, ssBase3, "TTStubs_hitFracs", "") ]
    binning = ("Stub hit charge fraction (for each hit)", 100, 0, 1)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_hitTrkIds", sBase03, ssBase3, "TTStubs_hitTrkIds", "") ]
    binning = ("Stub hit simTrack track id (for each hit)", 50, 0, 200)
    histos = book(params, binning); project(params, histos); draw(params, histos, logy=True)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_barrel", sBase03, ssBase3, "TTStubs_barrel", "") ]
    binning = ("Stub is in barrel", 4, 0, 4)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_psmodule", sBase03, ssBase3, "TTStubs_psmodule", "") ]
    binning = ("Stub is a PS module", 4, 0, 4)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_stackId", sBase03, ssBase3, "TTStubs_stackId", "") ]
    binning = ("Stub CMSSW module id", 150, 5E8, 5.3E8)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    ##
    params = [ ("TTStubs_geoId0", sBase03, ssBase3, "TTStubs_geoId0", "") ]
    binning = ("Stub inner cluster geographical id", 150, 3e8, 3.6e8)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_geoId1", sBase03, ssBase3, "TTStubs_geoId1", "") ]
    binning = ("Stub outer cluster geographical id", 150, 3e8, 3.6e8)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_clusId0", sBase03, ssBase3, "TTStubs_clusId0", "") ]
    binning = ("Stub inner cluster index in the ntuple", 60, 0, 300)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_clusId1", sBase03, ssBase3, "TTStubs_clusId1", "") ]
    binning = ("Stub outer cluster index in the ntuple", 60, 0, 300)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_trigDist", sBase03, ssBase3, "TTStubs_trigDist", "") ]
    binning = ("Stub trigger displacement [strip unit]", 20, -10, 10)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_trigOffset", sBase03, ssBase3, "TTStubs_trigOffset", "") ]
    binning = ("Stub trigger offset [strip unit]", 10, -5, 5)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_trigPos", sBase03, ssBase3, "TTStubs_trigPos", "") ]
    binning = ("Stub trigger local coord x [strip unit]", 129, 0, 1024+8)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_trigBend", sBase03, ssBase3, "TTStubs_trigBend", "") ]
    binning = ("Stub trigger bend [strip unit]", 20, -10, 10)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_separation", sBase03, ssBase3, "TTStubs_separation", "") ]
    binning = ("Stub separation [#Delta R or #Delta z]", 50, 0, 1)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_detWindow", sBase03, ssBase3, "TTStubs_detWindow", "") ]
    binning = ("Stub window (ignore for now)", 80, -1000, 3000)
    histos = book(params, binning); project(params, histos); draw(params, histos)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_trivial", sBase03, ssBase3, "TTStubs_trivial", "") ]
    binning = ("Stub trivial match", 4, 0, 4)
    histos = book(params, binning); project(params, histos); draw(params, histos, text=True)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_passed", sBase03, ssBase3, "TTStubs_passed", "") ]
    binning = ("Stub pattern match (by event)", 4, 0, 4)
    histos = book(params, binning); project(params, histos); draw(params, histos, text=True)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_modPassed", sBase03, ssBase3, "TTStubs_modPassed", "") ]
    binning = ("Stub pattern match (by module)", 4, 0, 4)
    histos = book(params, binning); project(params, histos); draw(params, histos, text=True)
    save(imgdir, params[0][0])

    params = [ ("TTStubs_hitPasseds", sBase03, ssBase3, "TTStubs_hitPasseds", "") ]
    binning = ("Stub pattern match (by hit, for each hit)", 4, 0, 4)
    histos = book(params, binning); project(params, histos); draw(params, histos, text=True)
    save(imgdir, params[0][0])


# ______________________________________________________________________________
if sections["fixme"]:
    #params = [ ("TTClusters_hitTrkIds", sBase03, ssBase3, "TTClusters_hitTrkIds", "") ]
    #binning = ("Cluster hit simTrack track id (for each hit)", 50, 0, 200)
    #histos = book(params, binning); project(params, histos); draw(params, histos, logy=True)
    #save(imgdir, params[0][0])

    #params = [ ("TTStubs_hitTrkIds", sBase03, ssBase3, "TTStubs_hitTrkIds", "") ]
    #binning = ("Stub hit simTrack track id (for each hit)", 50, 0, 200)
    #histos = book(params, binning); project(params, histos); draw(params, histos, logy=True)
    #save(imgdir, params[0][0])

    pass

