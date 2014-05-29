#!/usr/bin/env python

from rootdrawing import *
from array import array

# ______________________________________________________________________________
# Globals

latex.SetTextSize(0.06)

# ______________________________________________________________________________
# Configurations

sections = {}
sections["ZR"] = True
sections["XY"] = True
sections["UV"] = False  # FIXME

drawerInit = DrawerInit()
fname = "patternGeomInfo_Extended2023TTI_fib35.root"
nentries = 35

tfile = TFile.Open(fname)
tfiledir = tfile.Get("geominfo")

imgdir = "figures_pattern/"
if not imgdir.endswith("/"):  imgdir += "/"
if gSystem.AccessPathName(imgdir):
    gSystem.mkdir(imgdir)

# ______________________________________________________________________________
# Functions

def getGeomHist():
    global hPXB_PS_ZR, hPXB_2S_ZR, hPXF_PS_ZR, hPXF_2S_ZR, \
           hPXB_PS_XY, hPXB_2S_XY, hPXF_PS_XY, hPXF_2S_XY, \
           hPXB_PS_UV, hPXB_2S_UV, hPXF_PS_UV, hPXF_2S_UV

    markerstyle = 21
    markersize = 0.5
    hPXB_PS_ZR = tfiledir.Get("hPXB_PS_ZR")
    hPXB_PS_ZR.SetMarkerColor(palette[0])
    hPXB_PS_ZR.SetMarkerStyle(markerstyle)
    hPXB_PS_ZR.SetMarkerSize(markersize)

    hPXB_2S_ZR = tfiledir.Get("hPXB_2S_ZR")
    hPXB_2S_ZR.SetMarkerColor(palette[1])
    hPXB_2S_ZR.SetMarkerStyle(markerstyle)
    hPXB_2S_ZR.SetMarkerSize(markersize)

    hPXF_PS_ZR = tfiledir.Get("hPXF_PS_ZR")
    hPXF_PS_ZR.SetMarkerColor(palette[2])
    hPXF_PS_ZR.SetMarkerStyle(markerstyle)
    hPXF_PS_ZR.SetMarkerSize(markersize)

    hPXF_2S_ZR = tfiledir.Get("hPXF_2S_ZR")
    hPXF_2S_ZR.SetMarkerColor(palette[3])
    hPXF_2S_ZR.SetMarkerStyle(markerstyle)
    hPXF_2S_ZR.SetMarkerSize(markersize)

    hPXB_PS_XY = tfiledir.Get("hPXB_PS_XY")
    hPXB_PS_XY.SetMarkerColor(palette[0])
    hPXB_PS_XY.SetMarkerStyle(markerstyle)
    hPXB_PS_XY.SetMarkerSize(markersize)

    hPXB_2S_XY = tfiledir.Get("hPXB_2S_XY")
    hPXB_2S_XY.SetMarkerColor(palette[1])
    hPXB_2S_XY.SetMarkerStyle(markerstyle)
    hPXB_2S_XY.SetMarkerSize(markersize)

    hPXF_PS_XY = tfiledir.Get("hPXF_PS_XY")
    hPXF_PS_XY.SetMarkerColor(palette[2])
    hPXF_PS_XY.SetMarkerStyle(markerstyle)
    hPXF_PS_XY.SetMarkerSize(markersize)

    hPXF_2S_XY = tfiledir.Get("hPXF_2S_XY")
    hPXF_2S_XY.SetMarkerColor(palette[3])
    hPXF_2S_XY.SetMarkerStyle(markerstyle)
    hPXF_2S_XY.SetMarkerSize(markersize)

    hPXB_PS_UV = tfiledir.Get("hPXB_PS_UV")
    hPXB_PS_UV.SetMarkerColor(palette[0])
    hPXB_PS_UV.SetMarkerStyle(markerstyle)
    hPXB_PS_UV.SetMarkerSize(markersize)

    hPXB_2S_UV = tfiledir.Get("hPXB_2S_UV")
    hPXB_2S_UV.SetMarkerColor(palette[1])
    hPXB_2S_UV.SetMarkerStyle(markerstyle)
    hPXB_2S_UV.SetMarkerSize(markersize)

    hPXF_PS_UV = tfiledir.Get("hPXF_PS_UV")
    hPXF_PS_UV.SetMarkerColor(palette[2])
    hPXF_PS_UV.SetMarkerStyle(markerstyle)
    hPXF_PS_UV.SetMarkerSize(markersize)

    hPXF_2S_UV = tfiledir.Get("hPXF_2S_UV")
    hPXF_2S_UV.SetMarkerColor(palette[3])
    hPXF_2S_UV.SetMarkerStyle(markerstyle)
    hPXF_2S_UV.SetMarkerSize(markersize)

def getGeomCanvas():
    global img000, img001, img002, img003, img004
    fname_tk = "img000.root"
    tfile_tk = TFile.Open(fname_tk)
    img000 = tfile_tk.Get(fname_tk.replace(".root",""))
    tfile_tk.Close()

    fname_tk = "img001.root"
    tfile_tk = TFile.Open(fname_tk)
    img001 = tfile_tk.Get(fname_tk.replace(".root",""))
    tfile_tk.Close()

    fname_tk = "img002.root"
    tfile_tk = TFile.Open(fname_tk)
    img002 = tfile_tk.Get(fname_tk.replace(".root",""))
    tfile_tk.Close()

    fname_tk = "img003.root"
    tfile_tk = TFile.Open(fname_tk)
    img003 = tfile_tk.Get(fname_tk.replace(".root",""))
    tfile_tk.Close()

    fname_tk = "img004.root"
    tfile_tk = TFile.Open(fname_tk)
    img004 = tfile_tk.Get(fname_tk.replace(".root",""))
    tfile_tk.Close()

def makeBWPalette(scale=1.0, ncontours=50):
    stops = [0.00, 0.34, 0.61, 0.84, 1.00]
    red   = [1.00, 0.84, 0.61, 0.34, 0.00]
    green = [1.00, 0.84, 0.61, 0.34, 0.00]
    blue  = [1.00, 0.84, 0.61, 0.34, 0.00]

    if scale != 1.0:
        red   = [(col * scale) for col in red]
        green = [(col * scale) for col in green]
        blue  = [(col * scale) for col in blue]

    s = array('d', stops)
    r = array('d', red)
    g = array('d', green)
    b = array('d', blue)

    npoints = len(s)
    TColor.CreateGradientColorTable(npoints, s, r, g, b, ncontours)

def fib(x):
    if x <= 0:
        return 0
    if x == 1:
        return 1
    if x == 2:  # need this, otherwise '1' occurs twice
        return 2
    return fib(x-1) + fib(x-2)


# ______________________________________________________________________________
# Layout

getGeomCanvas()

# Colors have to be imported after opening the previous root file
from rootcolors import *
getGeomHist()

makeBWPalette(0.6)

markerstyle = 22
markersize = 1.5

useTklayout = True

# ______________________________________________________________________________
# ZR
if sections["ZR"]:
    h_old = None
    latex_old = None

    for i in xrange(nentries):
        if i==0 and not useTklayout:
            c1 = TCanvas("ZRCanvas", "ZRView Canvas", 1445, 600)
            hPXB_PS_ZR.Draw()
            hPXB_2S_ZR.Draw("same")
            hPXF_PS_ZR.Draw("same")
            hPXF_2S_ZR.Draw("same")

        elif i==0:
            img000.Draw()

        if h_old:
            h_old.Reset()
            h_sum.Reset()
            latex_old.Delete()

        h_sum = tfiledir.Get("hPXA_sum_ZR_%i" % fib(i))
        h_sum.Draw("same col")

        h = tfiledir.Get("hPXA_ind_ZR_%i" % fib(i))
        h.SetMarkerStyle(markerstyle)
        h.SetMarkerSize(markersize)
        h.Draw("same")
        latex_old = latex.DrawLatex(0.72, 0.5, "n = %i" % fib(i))

        h_old = h

        gPad.Print(imgdir+"ZRview_patt%i.png" % fib(i))


# ______________________________________________________________________________
# XY
if sections["XY"]:
    h_old = None
    h_oldEC = None
    latex_old = None
    latex_oldEC = None

    for i in xrange(nentries):
        if i==0 and not useTklayout:
            c1 = TCanvas("XYCanvas", "XYView Canvas", 600, 600)
            hPXB_PS_XY.Draw()
            hPXB_2S_XY.Draw("same")

            c2 = TCanvas("XYCanvasEC", "XYView Canvas (Endcap)", 600, 600)
            hPXF_PS_XY.Draw("")
            hPXF_2S_XY.Draw("same")

        elif i==0:
            img001.Draw()
            img002.Draw()
            c1 = img001
            c2 = img002

        if h_old:
            h_old.Reset()
            h_oldEC.Reset()
            h_sum.Reset()
            h_sumEC.Reset()
            latex_old.Delete()
            latex_oldEC.Delete()

        c1.cd()
        h_sum = tfiledir.Get("hPXB_sum_XY_%i" % fib(i))
        h_sum.Draw("same col")
        c2.cd()
        h_sumEC = tfiledir.Get("hPXF_sum_XY_%i" % fib(i))
        h_sumEC.Draw("same col")

        c1.cd()
        h = tfiledir.Get("hPXB_ind_XY_%i" % fib(i))
        h.SetMarkerStyle(markerstyle)
        h.SetMarkerSize(markersize)
        h.Draw("same")
        latex_old = latex.DrawLatex(0.7, 0.91, "n = %i" % fib(i))

        c2.cd()
        hEC = tfiledir.Get("hPXF_ind_XY_%i" % fib(i))
        hEC.SetMarkerStyle(markerstyle)
        hEC.SetMarkerSize(markersize)
        hEC.Draw("same")
        latex_oldEC = latex.DrawLatex(0.7, 0.91, "n = %i" % fib(i))

        h_old = h
        h_oldEC = hEC

        c1.cd()
        gPad.Print(imgdir+"XYview_patt%i.png" % fib(i))

        c2.cd()
        gPad.Print(imgdir+"XYviewEC_patt%i.png" % fib(i))

