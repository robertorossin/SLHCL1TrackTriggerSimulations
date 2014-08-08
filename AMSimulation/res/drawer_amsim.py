#!/usr/bin/env python

from rootdrawing import *
import os

# ______________________________________________________________________________
# Global

latex.SetTextSize(0.036)

line1 = TLine()
line1.SetLineColor(40)
line1.SetLineStyle(2)

# ______________________________________________________________________________
# Configurations

sections = {}
sections["coverage_vs_pt"     ] = True
sections["coverage_vs_eta"    ] = True
sections["coverage_vs_eta_phi"] = True

drawerInit = DrawerInit()
chain = TChain("ntupler/tree", "")
chain_val = TChain("ntupler/tree", "")

EOS = "/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1_results/"
RES = "SingleMuPlusMinus_sp16_%s"
settings = [
    ("ss1024_20140714"    ,   8246923),
    ("ss512_20140714"     ,  17990289),
    ("ss256_20140714"     ,  39435020),
    ("ss128_20140714"     ,  76364339),
    ("ss64_20140714"      , 125888937),
    ("ss32_20140714"      , 178122282),
]

RES_barrel = "SingleMuMinus_Barrel_sp16_%s"
settings_barrel = [
    ("ss1024_20140714"    ,   404111),
    ("ss512_20140714"     ,   868186),
    ("ss256_20140714"     ,  1991642),
    ("ss128_20140714"     ,  4679628),
    ("ss64_20140714"      , 10033316),
    ("ss32_20140714"      , 19229845),
]

RES = RES_barrel
settings = settings_barrel

imgdir = "figures_amsim/"
if not imgdir.endswith("/"):  imgdir += "/"
if gSystem.AccessPathName(imgdir):
    gSystem.mkdir(imgdir)


# ______________________________________________________________________________
# Profiles

histos = {}

tricolors = [kBlack, kRed, kBlue]
def bookProf(xtitle="", nbinsx=50, xlow=0, xup=50, ylow=0, yup=1, option=""):
    for i, j in enumerate(["prof0", "prof1", "prof2", "prof00"]):
        h = TProfile(j, xtitle, nbinsx, xlow, xup, ylow, yup, option)
        h.SetFillStyle(0)
        h.SetMarkerStyle(24)
        h.SetMarkerSize(0.9)
        h.SetLineWidth(2)
        h.SetMarkerColor(tricolors[i % 3])
        h.SetLineColor(tricolors[i % 3])
        histos[j] = h

def drawProf():
    h = histos["prof00"]
    h.SetStats(0)
    h.SetMaximum(1.2)
    h.SetMinimum(0)

    g = TGraphAsymmErrors(h)
    for i in xrange(g.GetN()):
        g.SetPointError(i, 1, 1, 0, 10)
    g.SetFillStyle(1001)
    g.SetFillColor(kGray2)
    histos["gprof00"] = g
    h.Reset()

    h.Draw()
    g.Draw("3")
    histos["prof0"].Draw("same")
    histos["prof1"].Draw("same")
    histos["prof2"].Draw("same")

    xmin, xmax = h.GetXaxis().GetXmin(), h.GetXaxis().GetXmax()
    line.DrawLine(xmin, 1.0, xmax, 1.0)
    line1.DrawLine(xmin, 0.4, xmax, 0.4)

    CMS_label()

def bookProf2D(xtitle="", nbinsx=50, xlow=0, xup=50, nbinsy=50, ylow=0, yup=50, zlow=0, zup=1, option=""):
    for i, j in enumerate(["prof0", "prof1", "prof2"]):
        h = TProfile2D(j, xtitle, nbinsx, xlow, xup, nbinsy, ylow, yup, zlow, zup, option)
        histos[j] = h


# ______________________________________________________________________________
# Coverage

if sections["coverage_vs_pt"]:

    for sett, sett1 in settings[0:]:
        chain.Reset()
        chain_val.Reset()

        bookProf("; p_{T} [GeV]; coverage", 100, 0., 100., 0., 1., "")

        result = RES % sett
        infiles = map(lambda x: EOS+"/"+result+"/"+x, os.listdir(EOS+"/"+result))
        print sett, len(infiles)

        for f in infiles:
            chain.Add(f)
        tree = chain

        tree.Project("prof0", "(@AMTTRoads_patternIds.size()!=0 && Alt$(AMTTRoads_patternIds[0],-1)>0):genParts_pt[0]", "abs(genParts_eta[0])<0.8", "prof goff")
        tree.Project("prof1", "(@AMTTRoads_patternIds.size()!=0 && Alt$(AMTTRoads_patternIds[0],-1)>0):genParts_pt[0]", "0.8<=abs(genParts_eta[0]) && abs(genParts_eta[0])<1.5", "prof goff")
        tree.Project("prof2", "(@AMTTRoads_patternIds.size()!=0 && Alt$(AMTTRoads_patternIds[0],-1)>0):genParts_pt[0]", "1.5<=abs(genParts_eta[0]) && abs(genParts_eta[0])<2.2", "prof goff")

        # Validation
        EOS_val = EOS.replace("_results","_results_val")
        infiles_val = map(lambda x: EOS_val+"/"+result+"/"+x, os.listdir(EOS_val+"/"+result))
        for f in infiles_val:
            chain_val.Add(f)
        tree_val = chain_val
        tree_val.Project("prof00", "(@AMTTRoads_patternIds.size()!=0 && Alt$(AMTTRoads_patternIds[0],-1)>0):genParts_pt[0]", "abs(genParts_eta[0])<0.8", "prof goff")

        drawProf()

        leg1 = TLegend(0.42,0.22,0.93,0.39)
        leg1.SetFillStyle(0); leg1.SetLineColor(0); leg1.SetShadowColor(0); leg1.SetBorderSize(0)
        leg1.AddEntry(histos["prof0"], "Single muon 0.0 #leq |#eta| < 0.8", "lp")
        leg1.AddEntry(histos["prof1"], "Single muon 0.8 #leq |#eta| < 1.5", "lp")
        leg1.AddEntry(histos["prof2"], "Single muon 1.5 #leq |#eta| < 2.2", "lp")
        leg1.Draw()

        latex.DrawLatex(0.55, 0.185, "%s [%.1fM bank]" % (sett[:-9].replace("_", " "),1e-6*sett1))

        save(imgdir, "coverage_vs_pt_%s" % (RES % sett))


if sections["coverage_vs_eta"]:

    for sett, sett1 in settings[0:]:
        chain.Reset()
        chain_val.Reset()

        bookProf("; #eta; coverage", 100, -2.5, 2.5, 0., 1., "")

        result = RES % sett
        infiles = map(lambda x: EOS+"/"+result+"/"+x, os.listdir(EOS+"/"+result))
        print sett, len(infiles)

        for f in infiles:
            chain.Add(f)
        tree = chain

        tree.Project("prof0", "(@AMTTRoads_patternIds.size()!=0 && Alt$(AMTTRoads_patternIds[0],-1)>0):genParts_eta[0]", "20<=genParts_pt[0]", "prof goff")
        tree.Project("prof1", "(@AMTTRoads_patternIds.size()!=0 && Alt$(AMTTRoads_patternIds[0],-1)>0):genParts_eta[0]", "5<=genParts_pt[0] && genParts_pt[0]<20", "prof goff")
        tree.Project("prof2", "(@AMTTRoads_patternIds.size()!=0 && Alt$(AMTTRoads_patternIds[0],-1)>0):genParts_eta[0]", "2<=genParts_pt[0] && genParts_pt[0]<5", "prof goff")

        # Validation
        EOS_val = EOS.replace("_results","_results_val")
        infiles_val = map(lambda x: EOS_val+"/"+result+"/"+x, os.listdir(EOS_val+"/"+result))
        for f in infiles_val:
            chain_val.Add(f)
        tree_val = chain_val
        tree_val.Project("prof00", "(@AMTTRoads_patternIds.size()!=0 && Alt$(AMTTRoads_patternIds[0],-1)>0):genParts_eta[0]", "20<=genParts_pt[0]", "prof goff")

        drawProf()

        leg1 = TLegend(0.42,0.22,0.93,0.39)
        leg1.SetFillStyle(0); leg1.SetLineColor(0); leg1.SetShadowColor(0); leg1.SetBorderSize(0)
        leg1.AddEntry(histos["prof0"], "Single muon 20 #leq p_{T} < #infty GeV", "lp")
        leg1.AddEntry(histos["prof1"], "Single muon   5 #leq p_{T} < 20 GeV", "lp")
        leg1.AddEntry(histos["prof2"], "Single muon   2 #leq p_{T} <   5 GeV", "lp")
        leg1.Draw()

        latex.DrawLatex(0.55, 0.185, "%s [%.1fM bank]" % (sett[:-9].replace("_", " "),1e-6*sett1))

        save(imgdir, "coverage_vs_eta_%s" % (RES % sett))

# ______________________________________________________________________________
# Coverage (2D)

if sections["coverage_vs_eta_phi"]:

    for sett, sett1 in settings[0:]:
        chain.Reset()
        chain_val.Reset()

        bookProf2D("; #eta; #phi; coverage", 50, -2.5, 2.5, 64, -3.2, 3.2, 0., 1., "")

        result = RES % sett
        infiles = map(lambda x: EOS+"/"+result+"/"+x, os.listdir(EOS+"/"+result))
        print sett, len(infiles)

        for f in infiles:
            chain.Add(f)
        tree = chain

        tree.Project("prof0", "(@AMTTRoads_patternIds.size()!=0 && Alt$(AMTTRoads_patternIds[0],-1)>0):genParts_phi[0]:genParts_eta[0]", "genParts_pt[0]>20", "prof goff")
        #tree.Project("prof1", "(@AMTTRoads_patternIds.size()!=0 && Alt$(AMTTRoads_patternIds[0],-1)>0):genParts_phi[0]:genParts_eta[0]", "genParts_pt[0]>5", "prof goff")
        #tree.Project("prof2", "(@AMTTRoads_patternIds.size()!=0 && Alt$(AMTTRoads_patternIds[0],-1)>0):genParts_phi[0]:genParts_eta[0]", "genParts_pt[0]>2", "prof goff")

        h = histos["prof0"]
        h.SetStats(0)
        h.SetMaximum(1.2)
        h.SetMinimum(0)
        draw2D([h], stats=False)

        latex.DrawLatex(0.55, 0.22, "Single muon p_{T} > 20 GeV")
        latex.DrawLatex(0.55, 0.185, "%s [%.1fM bank]" % (sett[:-9].replace("_", " "),1e-6*sett1))

        save(imgdir, "coverage_vs_eta_phi_%s" % (RES % sett))
