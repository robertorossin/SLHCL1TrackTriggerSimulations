from ROOT import TH1, TH1F, TH2F, TProfile, TFile, TChain, TCanvas, TLegend, TLatex, TLine, gROOT, gInterpreter, gStyle, gSystem, gPad
from rootcolors import *
from math import sqrt
from random import randint

# For init
class DrawerInit:
    def __init__(self):
        # ROOT
        gROOT.LoadMacro("tdrstyle.C")
        gROOT.LoadMacro("../interface/HelperMath.h")
        gROOT.LoadMacro("../src/AMSimulationLinkDef.h")
        gROOT.ProcessLine("setTDRStyle()")

        gStyle.SetEndErrorSize(2)
        gStyle.SetTitleOffset(1.1, "Y")
        #gStyle.SetLabelSize(0.04, "Y")

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

def bookProf(params, option="s"):
    histos = []
    for i, p in enumerate(params):
        hv = HistViewProf(p)
        if "s" in option:
            hv.h.SetErrorOption("s")
        histos.append(hv)
    return histos

def book2D(params):
    histos = []
    for i, p in enumerate(params):
        hv = HistView2D(p)
        histos.append(hv)
    return histos

# Project
def project(tree, histos, normalize=-1, drawOverflow=True, drawUnderflow=True):
    for i, h in enumerate(histos):
        tree.Project(h.randname, h.var, h.cut, "goff")
        if normalize > 0:  h.Scale(normalize / h.GetSumOfWeights())
        if drawOverflow :  h.fixOverflow()
        if drawUnderflow:  h.fixUnderflow()
    return

def projectProf(tree, histos):
    for i, h in enumerate(histos):
        tree.Project(h.randname, h.var, h.cut, "prof goff")
    return

def project2D(tree, histos):
    for i, h in enumerate(histos):
        tree.Project(h.randname, h.var, h.cut, "goff")
    return

# Draw
def draw(histos, ytitle="Events", logx=False, logy=False, text=False):
    if isinstance(histos[0], HistView):
        h = histos[0].h
    else:
        h = histos[0]
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
    if isinstance(histos[0], HistView):
        h = histos[0].h
        ymax = max([hv.h.GetMaximum() for hv in histos])
    else:
        h = histos[0]
        ymax = max([h.GetMaximum() for h in histos])
    if not logy:  h.SetMinimum(0.)
    h.GetYaxis().SetTitle(ytitle)
    h.Draw("hist")
    gPad.SetLogx(logx); gPad.SetLogy(logy)
    if isinstance(histos[0], HistView):
        for hv in histos[1:]:
            hv.h.Draw("same hist")
    else:
        for h in histos[1:]:
            h.Draw("same hist")
    CMS_label()
    return

def draw2D(histos, logx=False, logy=False, logz=False, palette=True, stats=True):
    if isinstance(histos[0], HistView2D):
        h = histos[0].h
    else:
        h = histos[0]
    if not stats:
        h.SetStats(0)
    h.Draw("colz")
    gPad.SetLogx(logx); gPad.SetLogy(logy); gPad.SetLogz(logz)
    gPad.SetRightMargin(0.10)
    gPad.Modified(); gPad.Update()
    if palette:
        xy = (0.91, 0.13, 0.95, 0.95)
        paletteObj = h.FindObject("palette")
        paletteObj.SetX1NDC(xy[0]); paletteObj.SetY1NDC(xy[1]); paletteObj.SetX2NDC(xy[2]); paletteObj.SetY2NDC(xy[3])
        paletteObj.SetTitleSize(0.024); paletteObj.SetLabelSize(0.024)
    if stats:
        xy = (0.64, 0.70, 0.88, 0.93)
        statsObj = h.FindObject("stats")
        statsObj.SetX1NDC(xy[0]); statsObj.SetY1NDC(xy[1]); statsObj.SetX2NDC(xy[2]); statsObj.SetY2NDC(xy[3])
    gPad.Modified(); gPad.Update()
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
# I/O

def get_infiles(txtfile, fast=False):
    infiles = []
    with open(txtfile) as f:
        for line in f:
            infiles.append(line.strip())
    if fast:
        half = len(infiles)/2
        infiles = [infiles[0], infiles[half]]
    return infiles
