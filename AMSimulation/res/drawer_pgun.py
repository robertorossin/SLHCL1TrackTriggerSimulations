#!/usr/bin/env python

from ROOT import TH1, TH1F, TH2F, TProfile, TFile, TChain, TCanvas, TLegend, TLatex, TLine, gROOT, gInterpreter, gStyle, gSystem, gPad
from rootcolors import *
from math import sqrt
from random import randint
#from operator import itemgetter
#from ordereddict import OrderedDict
#import numpy

# For init
class DrawerInit:
    def __init__(self):
        # ROOT
        gROOT.LoadMacro("tdrstyle.C")
        gROOT.LoadMacro("../interface/HelperMath.h")
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
# Globals
latex = TLatex()
latex.SetNDC()
latex.SetTextFont(42)
latex.SetTextSize(0.026)

line = TLine()
line.SetLineColor(kGray+2)
line.SetLineStyle(2)

# ______________________________________________________________________________
# Configurations

sections = {}
sections["genParts"   ] = False
sections["simTracks"  ] = False
sections["simVertices"] = False
sections["trkParts"   ] = False
sections["trkVertices"] = False
sections["eventInfo"  ] = False
sections["clusters"   ] = False
sections["stubs"      ] = False
sections["filtering"  ] = False
sections["filtering2" ] = False
sections["fixme"      ] = True

drawerInit = DrawerInit()
chain = TChain("ntupler/tree", "")
infiles = [
"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_10_1_20p.root",
#"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_11_1_A3c.root",
#"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_12_1_upT.root",
#"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_13_1_eSt.root",
#"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_14_1_6MI.root",
#"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_15_1_f0k.root",
#"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_16_1_z6q.root",
#"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_17_1_8BO.root",
#"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_18_1_zHQ.root",
#"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_19_1_FlR.root",
"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_20_1_WKb.root",
#"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_1_1_XY0.root",
#"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_2_1_Gjj.root",
#"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_3_1_tpR.root",
#"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_4_1_tTQ.root",
#"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_5_1_cYV.root",
#"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_6_1_gAu.root",
#"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_7_1_KMc.root",
#"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_8_1_lrx.root",
#"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_9_1_VTj.root",
    ]
if sections["clusters"] or sections["stubs"]:
    infiles = [f.replace(".root","_cut.root") for f in infiles]


for f in infiles:
    chain.Add(f)
tree = chain

imgdir = "figures_pgun/"
if not imgdir.endswith("/"):  imgdir += "/"
if gSystem.AccessPathName(imgdir):
    gSystem.mkdir(imgdir)


# ______________________________________________________________________________
# Classes

class Param(object):
    # as suggested in http://stackoverflow.com/questions/35988/c-like-structures-in-python
    def __init__(self, **kwds):
        self.__dict__.update(kwds)

class _HistViewBase(object):
    def __init__(self, p):
        self.h = None
        self.name = p.name
        self.var = p.var
        self.cut = p.cut

    def style(self, p):
        self.h.SetLineWidth(2)
        self.h.SetMarkerSize(0)
        if hasattr(p, "linecolor"  ):  self.h.SetLineColor(p.linecolor)
        if hasattr(p, "markercolor"):  self.h.SetMarkerColor(p.markercolor)
        if hasattr(p, "fillcolor"  ):  self.h.SetFillColor(p.fillcolor)
        if hasattr(p, "linestyle"  ):  self.h.SetLineStyle(p.linestyle)
        if hasattr(p, "markerstyle"):  self.h.SetMarkerStyle(p.markerstyle)
        if hasattr(p, "fillstyle"  ):  self.h.SetFillStyle(p.fillstyle)
        if hasattr(p, "linewidth"  ):  self.h.SetLineWidth(p.linewidth)
        if hasattr(p, "markersize" ):  self.h.SetMarkerSize(p.markersize)

class HistView(_HistViewBase):
    def __init__(self, p):
        super(HistView, self).__init__(p)
        self.h = TH1F("h1_%i" % randint(0,65535), "; %s" % p.xtitle, p.nbinsx, p.xlow, p.xup)
        self.randname = self.h.GetName()
        self.style(p)

    def fixOverflow(self):
        nbins = self.h.GetNbinsX()
        if self.h.GetBinContent(nbins+1) > 0:
            self.h.SetBinContent(nbins, self.h.GetBinContent(nbins) + self.h.GetBinContent(nbins+1))
            self.h.SetBinError(nbins, sqrt(self.h.GetBinError(nbins)**2 + self.h.GetBinError(nbins+1)**2))
            self.h.SetBinContent(nbins+1, 0)
            self.h.SetBinError(nbins+1, 0)
            self.h.SetEntries(self.h.GetEntries() - 2)  # SetBinContent() automatically increases # entries by one

    def fixUnderflow(self):
        if self.h.GetBinContent(0) > 0:
            self.h.SetBinContent(1, self.h.GetBinContent(0) + self.h.GetBinContent(1))
            self.h.SetBinError(1, sqrt(self.h.GetBinError(0)**2 + self.h.GetBinError(1)**2))
            self.h.SetBinContent(0, 0)
            self.h.SetBinError(0, 0)
            self.h.SetEntries(self.h.GetEntries() - 2)  # SetBinContent() automatically increases # entries by one

class HistViewProf(_HistViewBase):
    def __init__(self, p):
        super(HistViewProf, self).__init__(p)
        self.h = TH2F("hp1_%i" % randint(0,65535), "; %s; %s" % (p.xtitle, p.ytitle), p.nbinsx, p.xlow, p.xup, p.ylow, p.yup)
        self.randname = self.h.GetName()
        self.style(p)

class HistView2D(_HistViewBase):
    def __init__(self, p):
        super(HistView2D, self).__init__(p)
        self.h = TH2F("h2_%i" % randint(0,65535), "; %s; %s" % (p.xtitle, p.ytitle), p.nbinsx, p.xlow, p.xup, p.nbinsy, p.ylow, p.yup)
        self.randname = self.h.GetName()
        self.style(p)
        self.h.SetStats(0)


# ______________________________________________________________________________
# Functions

# Struct
def struct(*args):
    p = Param(name=args[0],
        var=args[1],
        cut=args[2],
        xtitle=args[3],
        nbinsx=args[4],
        xlow=args[5],
        xup=args[6],
        linecolor=args[7], markercolor=args[7],
        fillcolor=args[8]
        )
    return p

def struct2D(*args):
    p = Param(name=args[0],
        var=args[1],
        cut=args[2],
        xtitle=args[3],
        nbinsx=args[4],
        xlow=args[5],
        xup=args[6],
        ytitle=args[7],
        nbinsy=args[8],
        ylow=args[9],
        yup=args[10],
        linecolor=args[11], markercolor=args[11],
        fillcolor=args[12]
        )
    return p

# Book
def book(params):
    histos = []
    for i, p in enumerate(params):
        hv = HistView(p)
        histos.append(hv)
    return histos

def bookProf(params):
    histos = []
    for i, p in enumerate(params):
        hv = HistViewProf(p)
        histos.append(hv)
    return histos

def book2D(params):
    histos = []
    for i, p in enumerate(params):
        hv = HistView2D(p)
        histos.append(hv)
    return histos

# Project
def project(histos, normalize=-1, drawOverflow=True, drawUnderflow=True):
    for i, h in enumerate(histos):
        tree.Project(h.randname, h.var, h.cut, "goff")
        if normalize > 0:  h.Scale(normalize / h.GetSumOfWeights())
        if drawOverflow :  h.fixOverflow()
        if drawUnderflow:  h.fixUnderflow()
    return

def projectProf(histos):
    for i, h in enumerate(histos):
        tree.Project(h.randname, h.var, h.cut, "prof goff")
    return

def project2D(histos):
    for i, h in enumerate(histos):
        tree.Project(h.randname, h.var, h.cut, "goff")
    return

# Draw
def draw(histos, ytitle="Events", logx=False, logy=False, text=False):
    h = histos[0].h
    h.SetMaximum(h.GetMaximum() * 1.4)
    if not logy:  h.SetMinimum(0.)
    h.GetYaxis().SetTitle(ytitle)
    h.Draw("hist")
    gPad.SetLogx(logx); gPad.SetLogy(logy)
    if text:
        h.SetMarkerSize(2)
        h.Draw("hist text0")
    CMS_label()
    return

def draws(histos, ytitle="Events", logx=False, logy=False):
    h = histos[0].h
    ymax = max([hv.h.GetMaximum() for hv in histos])
    if not logy:  h.SetMinimum(0.)
    h.GetYaxis().SetTitle(ytitle)
    h.Draw("hist")
    gPad.SetLogx(logx); gPad.SetLogy(logy)
    for hv in histos[1:]:
        hv.h.Draw("same hist")
    CMS_label()
    return

def draw2D(histos, logx=False, logy=False):
    h = histos[0].h
    h.Draw("col")
    gPad.SetLogx(logx); gPad.SetLogy(logy)
    CMS_label()
    return

# ______________________________________________________________________________
# Auxiliary

def label(histos, legend=(0.70,0.74,0.96,0.94)):
    leg1 = TLegend(legend[0], legend[1], legend[2], legend[3])
    leg1.SetFillStyle(0); leg1.SetLineColor(0); leg1.SetShadowColor(0); leg1.SetBorderSize(0)
    leg1.Draw()
    return (leg1)

def CMS_label():
    old = (latex.GetTextFont(), latex.GetTextSize())
    latex.SetTextFont(62); latex.SetTextSize(0.028)
    latex.DrawLatex(0.670, 0.968, "CMS Very Preliminary")
    latex.SetTextFont(old[0]); latex.SetTextSize(old[1])
    return

def getMaximum(histos):
    maxima = []
    for h in histos:
        maxima.append(h.h.GetMaximum())
    return max(maxima)

def save(imgdir, imgname):
    gPad.RedrawAxis()
    gPad.Print(imgdir+imgname+".pdf")
    gPad.Print(imgdir+imgname+".png")


# ______________________________________________________________________________
# genParticles

if sections["genParts"]:
    def doit(x,imgdir=None):
        histos = book(x)
        project(histos)
        draw(histos)
        if imgdir:  save(imgdir, x[0].name)
        return histos

    cut = ""
    col = sBase03
    fcol = ssBase3

    p0 = struct("genParts_size", "genParts_size" if not cut else "Sum$(%s)" % cut, "",
        "muon # gen particles", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_pt", "genParts_pt", cut,
        "muon gen p_{T} [GeV]", 50, 0, 100, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_oneoverpt", "1.0/genParts_pt", cut,
        "muon gen 1/p_{T} [1/GeV]", 50, 0, 1, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_eta", "genParts_eta", cut,
        "muon gen #eta", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_phi", "genParts_phi", cut,
        "muon gen #phi [rad]", 64, -3.2, 3.2, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_E", "genParts_E", cut,
        "muon gen energy [GeV]", 50, 0, 1000, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_M", "genParts_M", cut,
        "muon gen mass [GeV]", 50, 0, 1, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_vx", "genParts_vx", cut,
        "muon gen vertex x_{0} [mm]", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_vy", "genParts_vy", cut,
        "muon gen vertex y_{0} [mm]", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_vz", "genParts_vz", cut,
        "muon gen vertex z_{0} [mm]", 60, -30, 30, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_pdgId", "genParts_pdgId", cut,
        "muon gen PDG id", 50, -25, 25, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_charge", "genParts_charge", cut,
        "muon gen charge", 8, -4, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("genParts_status", "genParts_status", cut,
        "muon gen status", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)


# ______________________________________________________________________________
# simTracks

if sections["simTracks"]:
    def doit(x,imgdir=None):
        histos = book(x)
        project(histos)
        draw(histos)
        if imgdir:  save(imgdir, x[0].name)
        return histos

    cut = "simTracks_trkId==1"
    col = sBase03
    fcol = ssBase4

    p0 = struct("simTracks_size", "simTracks_size" if not cut else "Sum$(%s)" % cut, "",
        "# simTracks", 50, 0, 100, col, fcol)
    histos = doit([p0], imgdir)
    # max = 1930, 1853, 1782

    p0 = struct("simTracks_pt", "simTracks_pt", cut,
        "simTrack p_{T} [GeV]", 50, 0, 100, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("simTracks_oneoverpt", "simTracks_pt==0 ? 9999 : 1.0/simTracks_pt", cut,
        "simTrack 1/p_{T} [1/GeV]", 50, 0, 1, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("simTracks_eta", "simTracks_eta", cut,
        "simTrack #eta", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("simTracks_phi", "simTracks_phi", cut,
        "simTrack #phi [rad]", 64, -3.2, 3.2, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("simTracks_pdgId", "simTracks_pdgId", cut,
        "simTrack PDG id", 50, -25, 25, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("simTracks_evtId", "simTracks_evtId", cut,
        "simTrack event id", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("simTracks_trkId", "simTracks_trkId", cut,
        "simTrack id", 50, 0, 200, col, fcol)
    #histos = doit([p0], imgdir)
    histos = book([p0]); project(histos); draw(histos, logy=True); save(imgdir, p0.name)
    # max = 2466515

    p0 = struct("simTracks_vtxId", "simTracks_vtxId", cut,
        "simTrack vertex id", 50, 0, 100, col, fcol)
    #histos = doit([p0], imgdir)
    histos = book([p0]); project(histos); draw(histos, logy=True); save(imgdir, p0.name)
    # max = 1560

    p0 = struct("simTracks_genpId", "simTracks_genpId", cut,
        "simTrack genParticle id", 6, -2, 4, col, fcol)
    histos = doit([p0], imgdir)

    # Special attention

    #p0 = struct("sp_simTracks_trkId_vs_pt", "simTracks_trkId:simTracks_pt" , "") ]
    #    "simTrack p_{T} [GeV]", 50, 0, 100, col, fcol)
    #ybinnings = [ ("simTrack track id", 50, 0, 200) ]
    #histos = book_scat(params, binning, ybinnings); project_scat(params, histos); ret=draw_scat(params, histos[0])
    #save(imgdir, params[0][0])


# ______________________________________________________________________________
if sections["simVertices"]:
    def doit(x,imgdir=None):
        histos = book(x)
        project(histos)
        draw(histos)
        if imgdir:  save(imgdir, x[0].name)
        return histos

    cut = "simVertices_vtxId==0"
    col = sBase03
    fcol = ssBase4

    p0 = struct("simVertices_size", "simVertices_size" if not cut else "Sum$(%s)" % cut, "",
        "# simVertices", 50, 0, 100, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("simVertices_vx", "simVertices_vx", cut,
        "simVertex x_{0} [mm]", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("simVertices_vy", "simVertices_vy", cut,
        "simVertex y_{0} [mm]", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("simVertices_vz", "simVertices_vz", cut,
        "simVertex z_{0} [mm]", 60, -30, 30, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("simVertices_tof", "simVertices_tof", cut,
        "simVertex time of flight [s]", 50, -1e-7, 1e-7, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("simVertices_trkId", "simVertices_trkId", cut,
        "simVertex track id", 50, 0, 200, col, fcol)
    #histos = doit([p0], imgdir)
    histos = book([p0]); project(histos); draw(histos, logy=True); save(imgdir, p0.name)

    p0 = struct("simVertices_vtxId", "simVertices_vtxId", cut,
        "simVertex vertex id", 50, 0, 100, col, fcol)
    #histos = doit([p0], imgdir)
    histos = book([p0]); project(histos); draw(histos, logy=True); save(imgdir, p0.name)

    #p0 = struct("simBeamSpot_vx", "simBeamSpot_bx", cut,
    #    "simBeamSpot x_{0} [mm]", 60, -3, 3, col, fcol)
    #histos = doit([p0], imgdir)
    #
    #p0 = struct("simBeamSpot_vy", "simBeamSpot_by", cut,
    #    "simBeamSpot y_{0} [mm]", 60, -3, 3, col, fcol)
    #histos = doit([p0], imgdir)
    #
    #p0 = struct("simBeamSpot_vz", "simBeamSpot_bz", cut,
    #    "simBeamSpot z_{0} [mm]", 60, -3, 3, col, fcol)
    #histos = doit([p0], imgdir)
    #
    #p0 = struct("simBeamSpot_sigmaZ", "simBeamSpot_sigmaZ", cut,
    #    "simBeamSpot #sigma_{z} [mm]", 50, 0, 0.1, col, fcol)
    #histos = doit([p0], imgdir)


# ______________________________________________________________________________
if sections["trkParts"]:
    def doit(x,imgdir=None):
        histos = book(x)
        project(histos)
        draw(histos)
        if imgdir:  save(imgdir, x[0].name)
        return histos

    cut = "trkParts_trkId==1"
    col = sBase03
    fcol = ssBase4

    p0 = struct("trkParts_size", "trkParts_size" if not cut else "Sum$(%s)" % cut, "",
        "# trkParticles", 50, 0, 100, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_nSimTracks", "@trkParts_trkIds.size()", cut,
        "trkParticle # simTracks", 20, 0, 20, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_pt", "trkParts_pt", cut,
        "trkParticle p_{T} [GeV]", 50, 0, 100, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_oneoverpt", "trkParts_pt==0 ? 9999 : 1.0/trkParts_pt", cut,
        "trkParticle 1/p_{T} [1/GeV]", 50, 0, 1, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_eta", "trkParts_eta", cut,
        "trkParticle #eta", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_phi", "trkParts_phi", cut,
        "trkParticle #phi [rad]", 64, -3.2, 3.2, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_vx", "trkParts_vx", cut,
        "trkParticle x_{0} [mm]", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_vy", "trkParts_vy", cut,
        "trkParticle y_{0} [mm]", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_vz", "trkParts_vz", cut,
        "trkParticle z_{0} [mm]", 60, -30, 30, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_pdgId", "trkParts_pdgId", cut,
        "trkParticle PDG id", 50, -25, 25, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_evtId", "trkParts_evtId", cut,
        "trkParticle event id", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_trkId", "trkParts_trkId", cut,
        "trkParticle simTrack id", 50, 0, 200, col, fcol)
    histos = book([p0]); project(histos); draw(histos, logy=True); save(imgdir, p0.name)

    p0 = struct("trkParts_genpId", "trkParts_genpId", cut,
        "trkParticle genParticle id", 6, -2, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_nhits", "trkParts_nhits", cut,
        "trkParticle # hits in tracker + muon", 50, 0, 50, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_ntkhits", "trkParts_ntkhits", cut,
        "trkParticle # hits in tracker", 50, 0, 50, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkParts_ntklayers", "trkParts_ntklayers", cut,
        "trkParticle # hits in tracker layers", 50, 0, 50, col, fcol)
    histos = doit([p0], imgdir)

    # Special attention

    #p0 = struct("sp_trkParts_size_vs_simTracks_size", "trkParts_size:simTracks_size" , "") ]
    #    "# simTracks", 50, 0, 100)
    #ybinnings = [ ("# trkParticles", 50, 0, 100) ]
    #histos = book_scat(params, binning, ybinnings); project_scat(params, histos); ret=draw_scat(params, histos[0])
    #save(imgdir, params[0][0])
    #
    #p0 = struct("sp_trkParts_trkId_vs_pt", "trkParts_trkIds[][0]:trkParts_pt" , "") ]
    #    "trkParticle p_{T} [GeV]", 50, 0, 100)
    #ybinnings = [ ("trkParticle simTrack id", 50, 0, 200) ]
    #histos = book_scat(params, binning, ybinnings); project_scat(params, histos); ret=draw_scat(params, histos[0])
    #save(imgdir, params[0][0])


# ______________________________________________________________________________
if sections["trkVertices"]:
    def doit(x,imgdir=None):
        histos = book(x)
        project(histos)
        draw(histos)
        if imgdir:  save(imgdir, x[0].name)
        return histos

    #cut = "trkVertices_vtxId==0"
    cut = "abs(trkVertices_vx)<1 && abs(trkVertices_vy)<1"
    col = sBase03
    fcol = ssBase4

    p0 = struct("trkVertices_size", "trkVertices_size" if not cut else "Sum$(%s)" % cut, "",
        "# trkVertices", 50, 0, 100, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkVertices_nSimVertices", "@trkVertices_vtxIds.size()", cut,
        "trkVertex # simVertices", 20, 0, 20, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkVertices_vx", "trkVertices_vx", cut,
        "trkVertex x_{0} [mm]", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkVertices_vy", "trkVertices_vy", cut,
        "trkVertex y_{0} [mm]", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkVertices_vz", "trkVertices_vz", cut,
        "trkVertex z_{0} [mm]", 60, -30, 30, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkVertices_tof", "trkVertices_tof", cut,
        "trkVertex time of flight [s]", 50, -1e-7, 1e-7, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("trkVertices_vtxId", "trkVertices_vtxId", cut,
        "trkVertex simVertex id", 50, 0, 100, col, fcol)
    histos = book([p0]); project(histos); draw(histos, logy=True); save(imgdir, p0.name)

    # Special attention

    #p0 = struct("sp_trkVertices_size_vs_simVertices_size", "trkVertices_size:simVertices_size" , "") ]
    #    "# simVertices", 50, 0, 100)
    #ybinnings = [ ("# trkVertices", 50, 0, 100) ]
    #histos = book_scat(params, binning, ybinnings); project_scat(params, histos); ret=draw_scat(params, histos[0])
    #save(imgdir, params[0][0])


# ______________________________________________________________________________
if sections["eventInfo"]:
    def doit(x,imgdir=None):
        histos = book(x)
        project(histos)
        draw(histos)
        if imgdir:  save(imgdir, x[0].name)
        return histos

    cut = ""
    col = sBase03
    fcol = ssBase3

    p0 = struct("event_run", "run", cut,
        "run number", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("event_lumi", "lumi", cut,
        "lumi section", 50, 1, 101, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("event_event", "event", cut,
        "event number", 50, 1, 10001, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("event_bx", "bx", cut,
        "bunch crossing", 4, -3, 1, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("gen_nPV", "gen_nPV", cut,
        "sim # PV", 50, 0, 150, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("gen_trueNPV", "gen_trueNPV", cut,
        "gen # PV", 50, 0, 150, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("gen_randomSeed", "gen_randomSeed", cut,  # bugged
        "generator seed", 50, 0, 100000, col, fcol)
    histos = doit([p0], imgdir)


# ______________________________________________________________________________
if sections["clusters"]:
    def doit(x,imgdir=None):
        histos = book(x)
        project(histos)
        draw(histos)
        if imgdir:  save(imgdir, x[0].name)
        return histos

    cut = "TTClusters_trkId==1"
    col = sBase03
    fcol = ssBase4

    p0 = struct("TTClusters_size", "TTClusters_size" if not cut else "Sum$(%s)" % cut, "",
        "# clusters", 60, 0, 60, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_x", "TTClusters_x", cut,
        "Cluster global pos x [cm]", 120, -120, 120, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_y", "TTClusters_y", cut,
        "Cluster global pos y [cm]", 120, -120, 120, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_z", "TTClusters_z", cut,
        "Cluster global pos z [cm]", 120, -300, 300, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_r", "TTClusters_r", cut,
        "Cluster global pos r [cm]", 60, 0, 120, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_phi", "TTClusters_phi", cut,
        "Cluster global pos phi [rad]", 64, -3.2, 3.2, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_localx", "TTClusters_localx", cut,
        "Cluster local pos x [unit?]", 50, -5, 5, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_localy", "TTClusters_localy", cut,
        "Cluster local pos y [unit?]", 50, -5, 5, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_coordx", "TTClusters_coordx", cut,
        "Cluster local coord x", 129, 0, 1024+8, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_coordy", "TTClusters_coordy", cut,
        "Cluster local coord y", 33, 0, 33, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_surfx", "TTClusters_surfx", cut,
        "Cluster surface pos x [cm]", 120, -120, 120, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_surfy", "TTClusters_surfy", cut,
        "Cluster surface pos y [cm]", 120, -120, 120, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_surfz", "TTClusters_surfz", cut,
        "Cluster surface pos z [cm]", 120, -300, 300, col, fcol)
    histos = doit([p0], imgdir)

    ##
    p0 = struct("TTClusters_simPt", "TTClusters_simPt", cut,
        "Cluster simTrack p_{T} [GeV]", 50, 0, 100, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_simOneoverpt", "TTClusters_simPt==0 ? 9999 : 1.0/TTClusters_simPt", cut,
        "Cluster simTrack 1/p_{T} [1/GeV]", 50, 0, 1, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_simEta", "TTClusters_simEta", cut,
        "Cluster simTrack #eta", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_simPhi", "TTClusters_simPhi", cut,
        "Cluster simTrack #phi [rad]", 64, -3.2, 3.2, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_trkId", "TTClusters_trkId", cut,
        "Cluster simTrack track id", 50, 0, 200, col, fcol)
    histos = book([p0]); project(histos); draw(histos, logy=True); save(imgdir, p0.name)

    p0 = struct("TTClusters_tpId", "TTClusters_tpId", cut,
        "Cluster trkParticle id", 50, 0, 200, col, fcol)
    histos = book([p0]); project(histos); draw(histos, logy=True); save(imgdir, p0.name)

    p0 = struct("TTClusters_pdgId", "TTClusters_pdgId", cut,
        "Cluster PDG id", 50, -25, 25, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_isGenuine", "TTClusters_isGenuine", cut,
        "Cluster is genuine", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_isCombinatoric", "TTClusters_isCombinatoric", cut,
        "Cluster is combinatoric", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_isUnknown", "TTClusters_isUnknown", cut,
        "Cluster is unknown", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_modId", "TTClusters_modId", cut,
        "Cluster module id", 120, 0, 300000, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_iModLayer", "TTClusters_iModLayer", cut,
        "Cluster layer number", 30, 0, 30, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_iModLadder", "TTClusters_iModLadder", cut,
        "Cluster ladder number", 100, 0, 100, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_iModModule", "TTClusters_iModModule", cut,
        "Cluster module number", 100, 0, 200, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_iModPitchX", "TTClusters_iModPitchX", cut,
        "Cluster pitch x [unit?]", 10, 0, 10, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_iModPitchY", "TTClusters_iModPitchY", cut,
        "Cluster pitch y [unit?]", 10, 0, 10, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_iModCols", "TTClusters_iModCols", cut,
        "Cluster # columns in module", 33, 0, 32+1, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_iModRows", "TTClusters_iModRows", cut,
        "Cluster # rows in module", 100, 950, 1050, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_nhits", "TTClusters_nhits", cut,
        "Cluster # hits", 20, 0, 20, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_hitADCs", "TTClusters_hitADCs", cut,
        "Cluster hit ADC count (for each hit)", 129, 0, 256+2, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_hitChans", "TTClusters_hitChans", cut,
        "Cluster hit channel id (for each hit)", 125, 0, 2500000, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_hitCols", "TTClusters_hitCols", cut,
        "Cluster hit column number (for each hit)", 33, 0, 32+1, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_hitRows", "TTClusters_hitRows", cut,
        "Cluster hit row number (for each hit)", 129, 0, 1024+8, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_hitFracs", "TTClusters_hitFracs", cut,
        "Cluster hit charge fraction (for each hit)", 100, 0, 1, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_hitTrkIds", "TTClusters_hitTrkIds", cut,
        "Cluster hit simTrack track id (for each hit)", 50, 0, 200, col, fcol)
    histos = book([p0]); project(histos); draw(histos, logy=True); save(imgdir, p0.name)

    p0 = struct("TTClusters_barrel", "TTClusters_barrel", cut,
        "Cluster is in barrel", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_psmodule", "TTClusters_psmodule", cut,
        "Cluster is a PS module", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_stackId", "TTClusters_stackId", cut,
        "Cluster CMSSW module id", 150, 5E8, 5.3E8, col, fcol)
    histos = doit([p0], imgdir)

    ##
    p0 = struct("TTClusters_geoId", "TTClusters_geoId", cut,
        "Cluster geographical id", 150, 3e8, 3.6e8, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_stack", "TTClusters_stack", cut,
        "Cluster stack member number", 4, 0 ,4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTClusters_width", "TTClusters_width", cut,
        "Cluster width in xy", 16, 0 ,16, col, fcol)
    histos = doit([p0], imgdir)


# ______________________________________________________________________________
if sections["stubs"]:
    def doit(x,imgdir=None):
        histos = book(x)
        project(histos)
        draw(histos)
        if imgdir:  save(imgdir, x[0].name)
        return histos

    cut = "TTStubs_trkId==1"
    col = sBase03
    fcol = ssBase4


    p0 = struct("TTStubs_size", "TTStubs_size" if not cut else "Sum$(%s)" % cut, "",
        "# stubs", 30, 0, 30, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_x", "TTStubs_x", cut,
        "Stub global pos x [cm]", 120, -120, 120, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_y", "TTStubs_y", cut,
        "Stub global pos y [cm]", 120, -120, 120, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_z", "TTStubs_z", cut,
        "Stub global pos z [cm]", 120, -300, 300, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_r", "TTStubs_r", cut,
        "Stub global pos r [cm]", 60, 0, 120, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_phi", "TTStubs_phi", cut,
        "Stub global pos phi [rad]", 64, -3.2, 3.2, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_dirx", "TTStubs_dirx", cut,
        "Stub global dir x", 100, -2.5, 2.5, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_diry", "TTStubs_diry", cut,
        "Stub global dir y", 100, -2.5, 2.5, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_dirz", "TTStubs_dirz", cut,
        "Stub global dir z", 100, -2.5, 2.5, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_roughPt", "TTStubs_roughPt", cut,
        "Stub 'rough' p_{T} [GeV]", 100, 0, 100, col, fcol)
    histos = doit([p0], imgdir)

    ##
    p0 = struct("TTStubs_simPt", "TTStubs_simPt", cut,
        "Stub simTrack p_{T} [GeV]", 50, 0, 100, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_simOneoverpt", "TTStubs_simPt==0 ? 9999 : 1.0/TTStubs_simPt", cut,
        "Stub simTrack 1/p_{T} [1/GeV]", 50, 0, 1, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_simEta", "TTStubs_simEta", cut,
        "Stub simTrack #eta", 60, -3, 3, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_simPhi", "TTStubs_simPhi", cut,
        "Stub simTrack #phi [rad]", 64, -3.2, 3.2, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_trkId", "TTStubs_trkId", cut,
        "Stub simTrack track id", 50, 0, 200, col, fcol)
    histos = book([p0]); project(histos); draw(histos, logy=True); save(imgdir, p0.name)

    p0 = struct("TTStubs_tpId", "TTStubs_tpId", cut,
        "Stub trkParticle id", 50, 0, 200, col, fcol)
    histos = book([p0]); project(histos); draw(histos, logy=True); save(imgdir, p0.name)

    p0 = struct("TTStubs_pdgId", "TTStubs_pdgId", cut,
        "Stub PDG id", 50, -25, 25, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_isGenuine", "TTStubs_isGenuine", cut,
        "Stub is genuine", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_isCombinatoric", "TTStubs_isCombinatoric", cut,
        "Stub is combinatoric", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_isUnknown", "TTStubs_isUnknown", cut,
        "Stub is unknown", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_modId", "TTStubs_modId", cut,
        "Stub module id", 120, 0, 300000, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_iModLayer", "TTStubs_iModLayer", cut,
        "Stub layer number", 30, 0, 30, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_iModLadder", "TTStubs_iModLadder", cut,
        "Stub ladder number", 100, 0, 100, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_iModModule", "TTStubs_iModModule", cut,
        "Stub module number", 100, 0, 200, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_iModPitchX", "TTStubs_iModPitchX", cut,
        "Stub pitch x [unit?]", 10, 0, 10, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_iModPitchY", "TTStubs_iModPitchY", cut,
        "Stub pitch y [unit?]", 10, 0, 10, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_iModCols", "TTStubs_iModCols", cut,
        "Stub # columns in module", 33, 0, 32+1, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_iModRows", "TTStubs_iModRows", cut,
        "Stub # rows in module", 100, 950, 1050, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_nhits", "TTStubs_nhits", cut,
        "Stub # hits", 20, 0, 20, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_hitADCs", "TTStubs_hitADCs", cut,
        "Stub hit ADC count (for each hit)", 129, 0, 256+2, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_hitChans", "TTStubs_hitChans", cut,
        "Stub hit channel id (for each hit)", 125, 0, 2500000, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_hitCols", "TTStubs_hitCols", cut,
        "Stub hit column number (for each hit)", 33, 0, 32+1, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_hitRows", "TTStubs_hitRows", cut,
        "Stub hit row number (for each hit)", 129, 0, 1024+8, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_hitFracs", "TTStubs_hitFracs", cut,
        "Stub hit charge fraction (for each hit)", 100, 0, 1, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_hitTrkIds", "TTStubs_hitTrkIds", cut,
        "Stub hit simTrack track id (for each hit)", 50, 0, 200, col, fcol)
    histos = book([p0]); project(histos); draw(histos, logy=True); save(imgdir, p0.name)

    p0 = struct("TTStubs_barrel", "TTStubs_barrel", cut,
        "Stub is in barrel", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_psmodule", "TTStubs_psmodule", cut,
        "Stub is a PS module", 4, 0, 4, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_stackId", "TTStubs_stackId", cut,
        "Stub CMSSW module id", 150, 5E8, 5.3E8, col, fcol)
    histos = doit([p0], imgdir)

    ##
    p0 = struct("TTStubs_geoId0", "TTStubs_geoId0", cut,
        "Stub inner cluster geographical id", 150, 3e8, 3.6e8, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_geoId1", "TTStubs_geoId1", cut,
        "Stub outer cluster geographical id", 150, 3e8, 3.6e8, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_clusId0", "TTStubs_clusId0", cut,
        "Stub inner cluster index in the ntuple", 60, 0, 300, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_clusId1", "TTStubs_clusId1", cut,
        "Stub outer cluster index in the ntuple", 60, 0, 300, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_trigDist", "TTStubs_trigDist", cut,
        "Stub trigger displacement [strip unit]", 20, -10, 10, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_trigOffset", "TTStubs_trigOffset", cut,
        "Stub trigger offset [strip unit]", 10, -5, 5, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_trigPos", "TTStubs_trigPos", cut,
        "Stub trigger local coord x [strip unit]", 129, 0, 1024+8, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_trigBend", "TTStubs_trigBend", cut,
        "Stub trigger bend [strip unit]", 20, -10, 10, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_separation", "TTStubs_separation", cut,
        "Stub separation [#Delta R or #Delta z]", 50, 0, 1, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_detWindow", "TTStubs_detWindow", cut,
        "Stub window (ignore for now)", 80, -1000, 3000, col, fcol)
    histos = doit([p0], imgdir)

    p0 = struct("TTStubs_trivial", "TTStubs_trivial", cut,
        "Stub trivial match", 4, 0, 4, col, fcol)
    histos = book([p0]); project(histos); draw(histos, text=True); save(imgdir, p0.name)

    p0 = struct("TTStubs_passed", "TTStubs_passed", cut,
        "Stub pattern match (by event)", 4, 0, 4, col, fcol)
    histos = book([p0]); project(histos); draw(histos, text=True); save(imgdir, p0.name)

    p0 = struct("TTStubs_modPassed", "TTStubs_modPassed", cut,
        "Stub pattern match (by module)", 4, 0, 4, col, fcol)
    histos = book([p0]); project(histos); draw(histos, text=True); save(imgdir, p0.name)

    p0 = struct("TTStubs_hitPasseds", "TTStubs_hitPasseds", cut,
        "Stub pattern match (by hit, for each hit)", 4, 0, 4, col, fcol)
    histos = book([p0]); project(histos); draw(histos, text=True); save(imgdir, p0.name)


# ______________________________________________________________________________
if sections["filtering"]:

    eventSelect_ = "(genParts_pt[0]>2 && abs(genParts_eta[0])<2.2 && Sum$(TTStubs_trkId==1 && abs(atan2(TTStubs_r,TTStubs_z)-atan2(genParts_pt[0],genParts_pz[0]))<0.05 && abs(deltaPhi(atan2(TTStubs_y,TTStubs_x),genParts_phi[0]))<0.03)>=Sum$(TTStubs_trkId==1)-2)";

    col = sBase03

    fcol0 = ssBase4
    fcol0_sel = sMagenta
    fcol1 = ssBase6
    fcol1_sel = sGreen

    cut0 = "(TTStubs_trkId==1 && genParts_pt[0]>2 && abs(genParts_eta[0])<1.0)"
    cut1 = "(TTStubs_trkId==1 && genParts_pt[0]>2 && abs(genParts_eta[0])<2.2 && abs(genParts_eta[0])>1.0)"
    lab0 = " {|#eta|<1.0}"
    lab1 = " {1.0<|#eta|<2.2}"

    var = "atan2(TTStubs_r,TTStubs_z)-atan2(genParts_pt[0],genParts_pz[0])"
    p0 = struct("filtering_deltaTheta_etabin0", var, cut0,
        "#Delta#theta(stub,gen muon)"+lab0, 40, -0.8, 0.8, col, fcol0)
    p1 = struct("filtering_deltaTheta_etabin0_sel", var, "*".join([cut0, eventSelect_]),
        "#Delta#theta(stub,gen muon)"+lab0, 40, -0.8, 0.8, col, fcol0_sel)
    histos = book([p0,p1]); project(histos); draws(histos, logy=True); save(imgdir, p0.name)

    p0 = struct("filtering_deltaTheta_etabin1", var, cut1,
        "#Delta#theta(stub,gen muon)"+lab1, 40, -0.8, 0.8, col, fcol1)
    p1 = struct("filtering_deltaTheta_etabin1_sel", var, "*".join([cut1, eventSelect_]),
        "#Delta#theta(stub,gen muon)"+lab1, 40, -0.8, 0.8, col, fcol1_sel)
    histos = book([p0,p1]); project(histos); draws(histos, logy=True); save(imgdir, p0.name)

    var = "deltaPhi(atan2(TTStubs_y,TTStubs_x),genParts_phi[0])"
    p0 = struct("filtering_deltaPhi_etabin0", var, cut0,
        "#Delta#phi(stub,gen muon)"+lab0, 40, -0.8, 0.8, col, fcol0)
    p1 = struct("filtering_deltaPhi_etabin0_sel", var, "*".join([cut0, eventSelect_]),
        "#Delta#phi(stub,gen muon)"+lab0, 40, -0.8, 0.8, col, fcol0_sel)
    histos = book([p0,p1]); project(histos); draws(histos, logy=True); save(imgdir, p0.name)

    p0 = struct("filtering_deltaPhi_etabin1", var, cut1,
        "#Delta#phi(stub,gen muon)"+lab1, 40, -0.8, 0.8, col, fcol1)
    p1 = struct("filtering_deltaPhi_etabin1_sel", var, "*".join([cut1, eventSelect_]),
        "#Delta#phi(stub,gen muon)"+lab1, 40, -0.8, 0.8, col, fcol1_sel)
    histos = book([p0,p1]); project(histos); draws(histos, logy=True); save(imgdir, p0.name)

    cut0 = "(genParts_pt[0]>2 && abs(genParts_eta[0])<1.0)"
    cut1 = "(genParts_pt[0]>2 && abs(genParts_eta[0])<2.2 && abs(genParts_eta[0])>1.0)"
    lab0 = " {|#eta|<1.0}"
    lab1 = " {1.0<|#eta|<2.2}"

    var = "Sum$(TTStubs_trkId==1)"
    p0 = struct("filtering_nstubs_etabin0", var, cut0,
        "# stubs"+lab0, 25, 0, 25, col, fcol0)
    p1 = struct("filtering_nstubs_etabin0_sel", var, "*".join([cut0, eventSelect_]),
        "# stubs"+lab0, 25, 0, 25, col, fcol0_sel)
    histos = book([p0,p1]); project(histos); draws(histos, logy=True); save(imgdir, p0.name)

    p0 = struct("filtering_nstubs_etabin1", var, cut1,
        "# stubs"+lab1, 25, 0, 25, col, fcol1)
    p1 = struct("filtering_nstubs_etabin1_sel", var, "*".join([cut1, eventSelect_]),
        "# stubs"+lab1, 25, 0, 25, col, fcol1_sel)
    histos = book([p0,p1]); project(histos); draws(histos, logy=True); save(imgdir, p0.name)

    var_tmp = "(Sum$(TTStubs_trkId==1 && TTStubs_iModLayer==%i)>0)"
    var = "+".join([(var_tmp % i) for i in (5,6,7,8,9,10,11,12,13,14,15,18,19,20,21,22)])
    p0 = struct("filtering_nlayers_etabin0", var, cut0,
        "# layers with stubs"+lab0, 25, 0, 25, col, fcol0)
    p1 = struct("filtering_nlayers_etabin0_sel", var, "*".join([cut0, eventSelect_]),
        "# layers with stubs"+lab0, 25, 0, 25, col, fcol0_sel)
    histos = book([p0,p1]); project(histos); draws(histos, logy=True); save(imgdir, p0.name)

    p0 = struct("filtering_nlayers_etabin1", var, cut1,
        "# layers with stubs"+lab1, 25, 0, 25, col, fcol1)
    p1 = struct("filtering_nlayers_etabin1_sel", var, "*".join([cut1, eventSelect_]),
        "# layers with stubs"+lab1, 25, 0, 25, col, fcol1_sel)
    histos = book([p0,p1]); project(histos); draws(histos, logy=True); save(imgdir, p0.name)

    var = "trkParts_ntkhits[0]"
    p0 = struct("filtering_tp_ntkhits_etabin0", var, cut0,
        "trkParticle # hits"+lab0, 50, 0, 50, col, fcol0)
    p1 = struct("filtering_tp_ntkhits_etabin0_sel", var, "*".join([cut0, eventSelect_]),
        "trkParticle # hits"+lab0, 50, 0, 50, col, fcol0_sel)
    histos = book([p0,p1]); project(histos); draws(histos, logy=True); save(imgdir, p0.name)

    p0 = struct("filtering_tp_ntkhits_etabin1", var, cut1,
        "trkParticle # hits"+lab1, 50, 0, 50, col, fcol1)
    p1 = struct("filtering_tp_ntkhits_etabin1_sel", var, "*".join([cut1, eventSelect_]),
        "trkParticle # hits"+lab1, 50, 0, 50, col, fcol1_sel)
    histos = book([p0,p1]); project(histos); draws(histos, logy=True); save(imgdir, p0.name)

    var = "trkParts_ntklayers[0]"
    p0 = struct("filtering_tp_ntklayers_etabin0", var, cut0,
        "trkParticle # layers"+lab0, 50, 0, 50, col, fcol0)
    p1 = struct("filtering_tp_ntklayers_etabin0_sel", var, "*".join([cut0, eventSelect_]),
        "trkParticle # layers"+lab0, 50, 0, 50, col, fcol0_sel)
    histos = book([p0,p1]); project(histos); draws(histos, logy=True); save(imgdir, p0.name)

    p0 = struct("filtering_tp_ntklayers_etabin1", var, cut1,
        "trkParticle # layers"+lab1, 50, 0, 50, col, fcol1)
    p1 = struct("filtering_tp_ntklayers_etabin1_sel", var, "*".join([cut1, eventSelect_]),
        "trkParticle # layers"+lab1, 50, 0, 50, col, fcol1_sel)
    histos = book([p0,p1]); project(histos); draws(histos, logy=True); save(imgdir, p0.name)

    # __________________________________________________________________________
    # Scatter
    var = "Sum$(TTStubs_trkId==1):trkParts_ntkhits[0]"
    p0 = struct2D("filtering_scat_nhits_etabin0", var, cut0,
        "trkParticle # hits"+lab0, 50, 0, 50, "# stubs", 25, 0, 25, col, fcol0)
    histos = book2D([p0]); project2D(histos); draw2D(histos); save(imgdir, p0.name)

    p0 = struct2D("filtering_scat_nhits_etabin1", var, cut1,
        "trkParticle # hits"+lab1, 50, 0, 50, "# stubs", 25, 0, 25, col, fcol1)
    histos = book2D([p0]); project2D(histos); draw2D(histos); save(imgdir, p0.name)

    var_tmp = "(Sum$(TTStubs_trkId==1 && TTStubs_iModLayer==%i)>0)"
    var = "+".join([(var_tmp % i) for i in (5,6,7,8,9,10,11,12,13,14,15,18,19,20,21,22)])
    var = var + ":trkParts_ntklayers[0]"
    p0 = struct2D("filtering_scat_nlayers_etabin0", var, cut0,
        "trkParticle # layers"+lab0, 50, 0, 50, "# layers with stubs", 25, 0, 25, col, fcol0)
    histos = book2D([p0]); project2D(histos); draw2D(histos); save(imgdir, p0.name)

    p0 = struct2D("filtering_scat_nlayers_etabin1", var, cut1,
        "trkParticle # layers"+lab1, 50, 0, 50, "# layers with stubs", 25, 0, 25, col, fcol1)
    histos = book2D([p0]); project2D(histos); draw2D(histos); save(imgdir, p0.name)


# ______________________________________________________________________________
if sections["filtering2"]:
    pass


# ______________________________________________________________________________
if sections["fixme"]:
    cut = "TTStubs_trkId==1"
    col = sBase03

    fcol = ssBase3
    p0 = struct("TTStubs_size", "TTStubs_size", "",
        "# stubs", 30, 0, 30, col, fcol)
    histos = book([p0]); project(histos); draws(histos, logy=True); save(imgdir, p0.name+"_logy")

    fcol = ssBase4
    p0 = struct("TTStubs_size", "Sum$(%s)" % cut, "",
        "# stubs {simTrack id==1}", 30, 0, 30, col, fcol)
    histos = book([p0]); project(histos); draws(histos, logy=True); save(imgdir, p0.name+"_id1_logy")

    fcol = ssBase4
    p0 = struct("TTStubs_size", "Sum$(%s)" % cut, "",
        "# stubs {# simTracks==1}", 30, 0, 30, col, fcol)
    histos = book([p0]); project(histos); draws(histos, logy=True); save(imgdir, p0.name+"_ns1_logy")

    fcol = ssBase3
    p0 = struct("TTStubs_pdgId", "TTStubs_pdgId", "",
        "Stub PDG id", 50, -25, 25, col, fcol)
    histos = book([p0]); project(histos); draws(histos, logy=True); save(imgdir, p0.name+"_logy")
