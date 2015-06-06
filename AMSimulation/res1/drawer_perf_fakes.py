#!/usr/bin/env python

from rootdrawing import *
from parser import *

col  = TColor.GetColor("#1f78b4")  # mu0
fcol = TColor.GetColor("#a6cee3")  # mu0

#col  = TColor.GetColor("#e31a1c")  # nu140
#fcol = TColor.GetColor("#fb9a99")  # nu140

#col  = TColor.GetColor("#6a3d9a")  # tttt140
#fcol = TColor.GetColor("#cab2d6")  # tttt140


# ______________________________________________________________________________
parnames = ["#phi", "cot #theta", "z_{0}", "1/p_{T}"]


# ______________________________________________________________________________
def drawer_book():
    histos = {}

    hname = "nroads_per_event"
    nbins, xmin, xmax = 200, 0., 200.*options.xscale
    histos[hname] = TH1F(hname, "; # roads/tower/BX"                , nbins, xmin, xmax)

    hname = "ncombinations_per_event"
    nbins, xmin, xmax = 800, 0., 1600.*options.xscale
    histos[hname] = TH1F(hname, "; # combinations/tower/BX"         , nbins, xmin, xmax)

    hname = "ntracks_per_event"
    nbins, xmin, xmax = 80, 0., 80.*options.xscale
    histos[hname] = TH1F(hname, "; # tracks/tower/BX"               , nbins, xmin, xmax)

    hname = "ngoods_per_event"
    nbins, xmin, xmax = 80, 0., 80.*options.xscale
    histos[hname] = TH1F(hname, "; # good tracks/tower/BX"          , nbins, xmin, xmax)

    hname = "nduplicates_per_event"
    nbins, xmin, xmax = 80, 0., 80.*options.xscale
    histos[hname] = TH1F(hname, "; # duplicate tracks/tower/BX"     , nbins, xmin, xmax)

    hname = "nfakes_per_event"
    nbins, xmin, xmax = 80, 0., 80.*options.xscale
    histos[hname] = TH1F(hname, "; # fake tracks/tower/BX"          , nbins, xmin, xmax)

    # Change binning
    if options.pu == 0:
        histos["nroads_per_event"       ].SetBins(40, 0., 40.)
        histos["ncombinations_per_event"].SetBins(40, 0., 40.)
        histos["ntracks_per_event"      ].SetBins(40, 0., 40.)
        histos["ngoods_per_event"       ].SetBins(40, 0., 40.)
        histos["nduplicates_per_event"  ].SetBins(40, 0., 40.)
        histos["nfakes_per_event"       ].SetBins(40, 0., 40.)

    # Style
    for hname, h in histos.iteritems():
        h.SetLineWidth(2); h.SetMarkerSize(0)
        h.SetLineColor(col); h.SetFillColor(fcol)
    donotdelete.append(histos)
    return histos

def parse_parname(s):
    s = s.replace("#","")
    s = s.replace(" ","")
    s = s.replace("_","")
    s = s.replace("{","")
    s = s.replace("}","")
    return s

def drawer_project(tree, histos, options):
    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("trkParts_pt"     , 1)
    tree.SetBranchStatus("trkParts_eta"    , 1)
    tree.SetBranchStatus("trkParts_phi"    , 1)
    #tree.SetBranchStatus("trkParts_vx"     , 1)
    #tree.SetBranchStatus("trkParts_vy"     , 1)
    tree.SetBranchStatus("trkParts_vz"     , 1)
    tree.SetBranchStatus("trkParts_charge" , 1)
    tree.SetBranchStatus("trkParts_primary", 1)
    tree.SetBranchStatus("AMTTRoads_patternRef" , 1)
    tree.SetBranchStatus("AMTTRoads_stubRefs"   , 1)
    tree.SetBranchStatus("AMTTTracks_invPt"     , 1)
    tree.SetBranchStatus("AMTTTracks_phi0"      , 1)
    tree.SetBranchStatus("AMTTTracks_cottheta"  , 1)
    tree.SetBranchStatus("AMTTTracks_z0"        , 1)
    tree.SetBranchStatus("AMTTTracks_chi2"      , 1)
    tree.SetBranchStatus("AMTTTracks_ndof"      , 1)
    tree.SetBranchStatus("AMTTTracks_synTpId"   , 1)
    tree.SetBranchStatus("AMTTTracks_roadRef"   , 1)
    tree.SetBranchStatus("AMTTTracks_patternRef", 1)

    # Loop over events
    for ievt, evt in enumerate(tree):
        if (ievt == options.nentries):  break

        if (ievt % 10 == 0):  print "Processing event: %i" % ievt

        if options.pu == 0:  # single-track events
            pt = evt.trkParts_pt[0]
            if not (pt > options.minPt):
                continue

        nroads_all = evt.AMTTRoads_patternRef.size()
        nroads, ncombs = 0, 0
        for iroad in xrange(nroads_all):
            patternRef = evt.AMTTRoads_patternRef[iroad]
            if not (patternRef < options.npatterns):
                continue

            nroads += 1

            stubRefs = evt.AMTTRoads_stubRefs[iroad]
            ncombs_per_road = 1
            for v in stubRefs:
                n = v.size()
                if n != 0:
                    ncombs_per_road *= n
            ncombs += ncombs_per_road

        nparts = evt.trkParts_pt.size()
        ntracks_all = evt.AMTTTracks_patternRef.size()
        ntracks, ngoods, nduplicates, nfakes = 0, 0, 0, 0

        for itrack in xrange(ntracks_all):
            patternRef = evt.AMTTTracks_patternRef[itrack]
            if not (patternRef < options.npatterns):
                continue

            chi2     = evt.AMTTTracks_chi2[itrack]
            ndof     = evt.AMTTTracks_ndof[itrack]
            chi2Red  = chi2/ndof

            if not (chi2Red < options.maxChi2):
                continue

            phi0       = evt.AMTTTracks_phi0    [itrack]
            cottheta   = evt.AMTTTracks_cottheta[itrack]
            z0         = evt.AMTTTracks_z0      [itrack]
            invPt      = evt.AMTTTracks_invPt   [itrack]
            synTpId    = evt.AMTTTracks_synTpId [itrack]

            if not (1.0/abs(invPt) > options.minPt):
                continue

            ntracks += 1

            if synTpId == -2:
                nfakes += 1
                continue

            if synTpId == -1:
                nduplicates += 1
                continue

            ngoods += 1

        if options.verbose:  print ievt, nroads, ncombs, ntracks, ngoods, nduplicates, nfakes

        assert(ntracks == ngoods + nduplicates + nfakes)
        histos["nroads_per_event"       ].Fill(nroads)
        histos["ncombinations_per_event"].Fill(ncombs)
        histos["ntracks_per_event"      ].Fill(ntracks)
        histos["ngoods_per_event"       ].Fill(ngoods)
        histos["nduplicates_per_event"  ].Fill(nduplicates)
        histos["nfakes_per_event"       ].Fill(nfakes)

    tree.SetBranchStatus("*", 1)
    return

def drawer_draw(histos, options):
    for hname, h in histos.iteritems():
        options.logy = True

        if options.logy:
            h.SetMaximum(h.GetMaximum() * 14); h.SetMinimum(0.5)
        else:
            h.SetMaximum(h.GetMaximum() * 1.4); h.SetMinimum(0.)
        h.SetStats(1); h.Draw("hist")
        gPad.SetLogy(options.logy)

        CMS_label()
        save(options.outdir, "%s_%s" % (hname, options.ss), dot_root=True)
    return

def drawer_sitrep(histos, options):
    print "--- SITREP ---------------------------------------------------------"


# ______________________________________________________________________________
# Main function
def main(options):

    # Init
    drawerInit = DrawerInit()
    tchain = TChain("ntupler/tree", "")
    tchain.AddFileInfoList(options.tfilecoll.GetList())

    # Process
    histos = drawer_book()
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
    parser.add_argument("--xscale", type=float, default=1, help="scale factor for the x-axis range (default: %(default)s)")

    # Parse default arguments
    options = parser.parse_args()
    parse_drawer_options(options)

    # Call the main function
    main(options)
