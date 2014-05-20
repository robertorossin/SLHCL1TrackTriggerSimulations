#!/usr/bin/env python

from ROOT import TH1, TH1F, TH2F, TProfile, TFile, TChain, TCanvas, TLegend, TLatex, TLine, gROOT, gInterpreter, gStyle, gSystem, gPad
from rootcolors import *
from math import sqrt

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

        gStyle.SetOptStat(0)
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
sections["coverage1"] = True
sections["coverage2"] = True

drawerInit = DrawerInit()
chain = TChain("ntupler/tree", "")
infiles = [
"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_10_1_M5c.root",
"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_11_1_hSJ.root",
"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_12_1_c6j.root",
"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_13_1_5qi.root",
"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_14_1_uho.root",
"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_15_1_YVP.root",
"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_16_1_6Sy.root",
"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_17_1_qkx.root",
"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_18_1_HXB.root",
"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_19_1_3pI.root",
"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_20_1_Luk.root",
"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_1_1_ZCg.root",
"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_2_1_cqn.root",
"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_3_1_ZtL.root",
"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_4_1_TsL.root",
"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_5_1_uSw.root",
"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_6_1_LuW.root",
"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_7_1_nAO.root",
"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_8_1_L4a.root",
"/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/SingleMuPlusMinus_OneOverPt_vz0_20140519/results_9_1_rHD.root",
    ]

for f in infiles:
    chain.Add(f)
tree = chain

imgdir = "figures_patterngen/"
if not imgdir.endswith("/"):  imgdir += "/"
if gSystem.AccessPathName(imgdir):
    gSystem.mkdir(imgdir)


# ______________________________________________________________________________
# Functions


# ______________________________________________________________________________
# Coverage

if sections["coverage1"]:

    #h0 = TH1F("h0", "; p_{T} [GeV]; coverage", 100, 0, 100)

    nbins, xlow, xup, ylow, yup = 100, 0., 100., 0., 1.
    hP0 = TProfile("hP0", "; p_{T} [GeV]; coverage", nbins, xlow, xup, ylow, yup, "")
    hP1 = TProfile("hP1", "; p_{T} [GeV]; coverage", nbins, xlow, xup, ylow, yup, "")
    hP2 = TProfile("hP2", "; p_{T} [GeV]; coverage", nbins, xlow, xup, ylow, yup, "")

    tree.Project("hP0", "(@AMTTRoads_hash.size()!=0 && Alt$(AMTTRoads_hash[0],-1)>0):genParts_pt[0]", "abs(genParts_eta[0])<1.0", "prof")
    tree.Project("hP1", "(@AMTTRoads_hash.size()!=0 && Alt$(AMTTRoads_hash[0],-1)>0):genParts_pt[0]", "abs(genParts_eta[0])<2.2", "prof")
    tree.Project("hP2", "(@AMTTRoads_hash.size()!=0 && Alt$(AMTTRoads_hash[0],-1)>0):genParts_pt[0]", "abs(genParts_eta[0])<3.5", "prof")

    hP0.SetMaximum(0.6)
    hP0.SetMarkerColor(kRed)
    hP1.SetMarkerColor(kBlue)
    hP2.SetMarkerColor(kGreen)
    hP0.Draw()
    hP1.Draw("same")
    hP2.Draw("same")

    leg1 = TLegend(0.68,0.78,0.94,0.94)
    leg1.SetFillStyle(0); leg1.SetLineColor(0); leg1.SetShadowColor(0); leg1.SetBorderSize(0)
    leg1.AddEntry(hP0, "|#eta| < 1.0")
    leg1.AddEntry(hP1, "|#eta| < 2.2")
    leg1.AddEntry(hP2, "|#eta| < 3.5")
    leg1.Draw()

    gPad.Print("coverage_vs_pt_32SS_1M.png")
    gPad.Print("coverage_vs_pt_32SS_1M.pdf")


if sections["coverage2"]:

    nbins, xlow, xup, ylow, yup = 108, -2.7, 2.7, 0., 1.
    hP0 = TProfile("hP0", "; #eta; coverage", nbins, xlow, xup, ylow, yup, "")
    hP1 = TProfile("hP1", "; #eta; coverage", nbins, xlow, xup, ylow, yup, "")
    hP2 = TProfile("hP2", "; #eta; coverage", nbins, xlow, xup, ylow, yup, "")

    tree.Project("hP0", "(@AMTTRoads_hash.size()!=0 && Alt$(AMTTRoads_hash[0],-1)>0):genParts_eta[0]", "genParts_pt[0]>2", "prof")
    tree.Project("hP1", "(@AMTTRoads_hash.size()!=0 && Alt$(AMTTRoads_hash[0],-1)>0):genParts_eta[0]", "genParts_pt[0]>5", "prof")
    tree.Project("hP2", "(@AMTTRoads_hash.size()!=0 && Alt$(AMTTRoads_hash[0],-1)>0):genParts_eta[0]", "genParts_pt[0]>20", "prof")

    hP0.SetMaximum(0.6)
    hP0.SetMarkerColor(kRed)
    hP1.SetMarkerColor(kBlue)
    hP2.SetMarkerColor(kGreen)
    hP0.Draw()
    hP1.Draw("same")
    hP2.Draw("same")

    leg1 = TLegend(0.68,0.78,0.94,0.94)
    leg1.SetFillStyle(0); leg1.SetLineColor(0); leg1.SetShadowColor(0); leg1.SetBorderSize(0)
    leg1.AddEntry(hP0, "p_{T} > 2")
    leg1.AddEntry(hP1, "p_{T} > 5")
    leg1.AddEntry(hP2, "p_{T} > 20")
    leg1.Draw()

    gPad.Print("coverage_vs_eta_32SS_1M.png")
    gPad.Print("coverage_vs_eta_32SS_1M.pdf")



