#!/usr/bin/env python

from rootdrawing import *
from parser import *
import pdgName

col = TColor.GetColor("#BA0000")

c_pdgName = pdgName.pdgName()

# ______________________________________________________________________________
parnames = ["#phi", "cot #theta", "z_{0}", "1/p_{T}"]

def use_TEfficiency():
    del ROOT.TEfficiency
    gROOT.ProcessLine(".L ./fixes/TEfficiency.cxx+")
    #gSystem.Load("./fixes/TEfficiency_cxx.so")

    global TEfficiency
    TEfficiency = ROOT.TEfficiency


# ______________________________________________________________________________
def drawer_book(options):
    histos = {}

    prefix = "efficiency_"
    ytitle = "#varepsilon_{synthetic}"

    hname = prefix + "pt"
    histos[hname] = TEfficiency(hname, "; p_{T} [GeV]; %s" % ytitle, 100, 0., 500.)

    hname = prefix + "ppt"  # pt, but zoomed in to 0-50 GeV
    histos[hname] = TEfficiency(hname, "; p_{T} [GeV]; %s" % ytitle, 100, 0., 50.)

    hname = prefix + "pppt"  # pt, but zoomed in to 0-10 GeV
    histos[hname] = TEfficiency(hname, "; p_{T} [GeV]; %s" % ytitle, 100, 0., 10.)

    hname = prefix + "eta"
    histos[hname] = TEfficiency(hname, "; #eta; %s" % ytitle,        120, -3.0, 3.0)

    hname = prefix + "phi"
    histos[hname] = TEfficiency(hname, "; #phi; %s" % ytitle,        128, -3.2, 3.2)

    hname = prefix + "vz"
    histos[hname] = TEfficiency(hname, "; v_{z} [cm]; %s" % ytitle,  120, -30, 30)

    hname = prefix + "charge"
    histos[hname] = TEfficiency(hname, "; charge; %s" % ytitle,      5, -2.5, 2.5)

    hname = prefix + "pdgName"
    histos[hname] = TProfile   (hname, "; pdgName; %s" % ytitle,     50, -25, 25, 0., 2., "")

    if options.low_stat:
        histos[prefix + "pt"  ].SetBins(50, 0., 500.)
        histos[prefix + "ppt" ].SetBins(50, 0., 50.)
        histos[prefix + "pppt"].SetBins(50, 0., 10.)

    if options.low_low_stat:
        histos[prefix + "pt"  ].SetBins(20, 0., 500.)
        histos[prefix + "ppt" ].SetBins(20, 0., 50.)
        histos[prefix + "pppt"].SetBins(20, 0., 10.)

    # Error statistics
    for hname, h in histos.iteritems():
        if h.ClassName() == "TEfficiency":
            h.SetConfidenceLevel(0.682689492137)  # 1-sigma
            h.SetStatisticOption(0)  # kFCP
            #h.SetStatisticOption(6)  # kBUniform

    # Style
    for hname, h in histos.iteritems():
        h.SetLineWidth(2); h.SetMarkerSize(0)
        h.SetMarkerStyle(24); h.SetMarkerSize(0.9)
        h.SetMarkerColor(col); h.SetLineColor(col)
    donotdelete.append(histos)
    return histos

def parse_parname(s):
    s = s.replace("#","")
    s = s.replace(" ","")
    s = s.replace("_","")
    s = s.replace("{","")
    s = s.replace("}","")
    return s

def repr_cppvector(v):
    s = ",".join([str(x) for x in v])
    s = "[" + s + "]"
    return s

def drawer_project(tree, histos, options):
    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("trkParts_pt"     , 1)
    tree.SetBranchStatus("trkParts_eta"    , 1)
    tree.SetBranchStatus("trkParts_phi"    , 1)
    tree.SetBranchStatus("trkParts_vx"     , 1)
    tree.SetBranchStatus("trkParts_vy"     , 1)
    tree.SetBranchStatus("trkParts_vz"     , 1)
    tree.SetBranchStatus("trkParts_charge" , 1)
    tree.SetBranchStatus("trkParts_primary", 1)
    tree.SetBranchStatus("trkParts_signal" , 1)
    tree.SetBranchStatus("trkParts_pdgId"  , 1)
    tree.SetBranchStatus("AMTTTracks_invPt"     , 1)
    tree.SetBranchStatus("AMTTTracks_phi0"      , 1)
    tree.SetBranchStatus("AMTTTracks_cottheta"  , 1)
    tree.SetBranchStatus("AMTTTracks_z0"        , 1)
    tree.SetBranchStatus("AMTTTracks_chi2"      , 1)
    tree.SetBranchStatus("AMTTTracks_ndof"      , 1)
    tree.SetBranchStatus("AMTTTracks_synTpId"   , 1)
    tree.SetBranchStatus("AMTTTracks_roadRef"   , 1)
    tree.SetBranchStatus("AMTTTracks_patternRef", 1)
    tree.SetBranchStatus("AMTTTracks_stubRefs"  , 1)

    # Loop over events
    numEntries, denomEntries = 0, 0

    for ievt, evt in enumerate(tree):
        if (ievt == options.nentries):  break

        if (ievt % 100 == 0):  print "Processing event: %i" % ievt

        nparts_all = evt.trkParts_primary.size()
        trkparts = {}

        for ipart in xrange(nparts_all):
            if options.pu == 0:  # single-track events
                if ipart > 0:
                    break

            charge  = evt.trkParts_charge [ipart]
            primary = evt.trkParts_primary[ipart]
            signal  = evt.trkParts_signal [ipart]

            if not (charge!=0 and primary):
                continue

            if options.signal and not signal:
                continue

            pt      = evt.trkParts_pt     [ipart]
            eta     = evt.trkParts_eta    [ipart]
            phi     = evt.trkParts_phi    [ipart]
            vz      = evt.trkParts_vz     [ipart]
            pdgId   = evt.trkParts_pdgId  [ipart]

            trkparts[ipart] = (pt, eta, phi, vz, charge, pdgId)
            if options.verbose:  print ievt, "part ", ipart, 1.0/pt


        ntracks_all = evt.AMTTTracks_patternRef.size()
        trkparts_trigger = {}

        for itrack in xrange(ntracks_all):
            trigger = False

            synTpId  = evt.AMTTTracks_synTpId[itrack]
            if synTpId >= 0:
                trigger = True

            patternRef = evt.AMTTTracks_patternRef[itrack]
            if not (patternRef < options.npatterns):
                trigger = False

            chi2     = evt.AMTTTracks_chi2[itrack]
            ndof     = evt.AMTTTracks_ndof[itrack]
            chi2Red  = chi2/ndof

            if not (chi2Red < options.maxChi2):
                trigger = False

            invPt      = evt.AMTTTracks_invPt   [itrack]
            if not (1.0/abs(invPt) > options.minPt):
                trigger = False

            if trigger:
                trkparts_trigger[synTpId] = True

                if options.verbose and (synTpId in trkparts):  print ievt, "track", itrack, invPt, repr_cppvector(evt.AMTTTracks_stubRefs[itrack])

        #for k, v in trkparts_trigger.iteritems():
        #    assert k in trkparts

        # ______________________________________________________________________
        # N-1 conditions

        for k, v in trkparts.iteritems():
            pt, eta, phi, vz, charge, pdgId = v
            trigger = k in trkparts_trigger

            prefix = "efficiency_"
            if options.etamin < eta < options.etamax and options.phimin < phi < options.phimax:
                histos[prefix + "pt"      ].Fill(trigger, pt)
                histos[prefix + "ppt"     ].Fill(trigger, pt)
                histos[prefix + "pppt"    ].Fill(trigger, pt)
                denomEntries += 1
                if trigger:  numEntries += 1
            if options.ptmin < pt < options.ptmax and options.phimin < phi < options.phimax:
                histos[prefix + "eta"     ].Fill(trigger, eta)
            if options.ptmin < pt < options.ptmax and options.etamin < eta < options.etamax:
                histos[prefix + "phi"     ].Fill(trigger, phi)
            if options.ptmin < pt < options.ptmax and options.etamin < eta < options.etamax and options.phimin < phi < options.phimax:
                histos[prefix + "vz"      ].Fill(trigger, vz)
                histos[prefix + "charge"  ].Fill(trigger, charge)
                histos[prefix + "pdgName" ].Fill(c_pdgName.get(pdgId), trigger)
                #histos[prefix + "pdgName" ].Fill(trigger, c_pdgName.get(pdgId))

    histos[prefix + "pt"      ].numEntries = numEntries
    histos[prefix + "pt"      ].denomEntries = denomEntries

    tree.SetBranchStatus("*", 1)
    return

def drawer_draw(histos, options):
    for hname, h in histos.iteritems():
        ymax = 1.2

        if hname.endswith("pdgName"):
            h.LabelsDeflate("X")
            h.LabelsOption("a")

        if h.ClassName() == "TEfficiency":
            h1 = h.GetCopyTotalHisto(); h1.Reset()
        else:
            h1 = h
        h1.SetMinimum(0); h1.SetMaximum(ymax)
        h1.SetStats(0); h1.Draw()

        # Reference lines for 0.9, 0.95 and 1.0
        xmin, xmax = h1.GetXaxis().GetXmin(), h1.GetXaxis().GetXmax()
        for y in [0.5, 0.8, 0.9, 0.95, 1.0]:
            tline.DrawLine(xmin, y, xmax, y)

        if h.ClassName() == "TEfficiency":
            h.gr = h.CreateGraph()
            h.gr.Draw("p")
        else:
            h.Draw("same")

        CMS_label()
        save(options.outdir, "%s_%s" % (hname, options.ss), dot_root=True)
    return


def drawer_sitrep(histos, options):
    print "--- SITREP ---------------------------------------------------------"

    prefix = "efficiency_"
    h = histos[prefix + "pt"]
    print "eff={0}/{1}={2:.3f}".format(h.numEntries, h.denomEntries, float(h.numEntries)/h.denomEntries)


    h = histos[prefix + "pdgName"]
    bincontents = {}
    for i in xrange(1,h.GetNbinsX()+1):
        #print i, h.GetXaxis().GetBinLabel(i), h.GetBinContent(i)
        binlabel = h.GetXaxis().GetBinLabel(i)
        bincontents[binlabel] = bincontents.get(binlabel,0) + h.GetBinContent(i)

    print "Primary tracking particle efficiency:"
    for k, v in sorted(bincontents.items(), key=lambda x: x[1], reverse=True):
        print "{0:8s} {1:.3f}".format(k, float(v))


# ______________________________________________________________________________
# Main function
def main(options):

    # Init
    drawerInit = DrawerInit()
    tchain = TChain("ntupler/tree", "")
    tchain.AddFileInfoList(options.tfilecoll.GetList())

    gStyle.SetNdivisions(510, "Y")
    gStyle.SetEndErrorSize(0)
    use_TEfficiency()

    # Process
    histos = drawer_book(options)
    drawer_project(tchain, histos, options)
    drawer_draw(histos, options)
    drawer_sitrep(histos, options)


# ______________________________________________________________________________
if __name__ == '__main__':

    # Setup argument parser
    parser = argparse.ArgumentParser()

    # Add default arguments
    add_drawer_arguments(parser)

    # Add more arguments
    parser.add_argument("ss", help="short name of superstrip definition (e.g. ss256)")
    parser.add_argument("npatterns", type=int, help="number of patterns to reach the desired coverage")
    parser.add_argument("--coverage", type=float, default=0.95, help="desired coverage (default: %(default)s)")
    parser.add_argument("--minPt", type=float, default=2, help="min pT (default: %(default)s)")
    parser.add_argument("--maxChi2", type=float, default=5, help="max reduced chi-squared (default: %(default)s)")
    parser.add_argument("--low-stat", action="store_true", help="low statistics (default: %(default)s)")
    parser.add_argument("--low-low-stat", action="store_true", help="low low statistics (default: %(default)s)")

    # Parse default arguments
    options = parser.parse_args()
    parse_drawer_options(options)
    options.ptmin = options.minPt

    # Call the main function
    main(options)
