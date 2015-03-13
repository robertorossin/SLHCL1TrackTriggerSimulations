#!/usr/bin/env python

from rootdrawing import *
from parser import *
from helper import *

donotdelete = []  # persist in memory


# ______________________________________________________________________________
def bookPatterns():
    histos = {}

    hname = "npatterns"
    nbinsx, xmin, xmax = 1000, -0.5, 0.5
    h = histos[hname] = TH1F(hname, "; signed 1/p_{T} [1/GeV]; # patterns", nbinsx, xmin, xmax)
    h.SetLineColor(kBlue)

    donotdelete.append(histos)
    return histos

def checkoutPatterns(tfile, histos, options):
    def style(h):
        h.SetTitleOffset(0.9, "X")
        h.SetTitleOffset(1.25, "Y")
        h.SetTitleSize(0.06, "XY")
        h.SetLabelSize(0.05, "X")
        h.SetLabelSize(0.04, "Y")
        h.SetNdivisions(505, "XY")
        h.SetMarkerStyle(1)
        return h

    hname = "diff_invPt_vs_invPt1"
    histos[hname] = style(tfile.Get(hname))

    hname = "diff_invPt_vs_invPt2"
    histos[hname] = style(tfile.Get(hname))

def projectPatterns(tree, histos, options):
    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("invPt_mean" , 1)
    tree.SetBranchStatus("invPt_sigma", 1)

    def style(h):
        h.SetMarkerStyle(1)
        return h

    # Loop over events
    x, y, ex, ey, zeroes = array('d'), array('d'), array('d'), array('d'), array('d')
    for ievt, evt in enumerate(tree):
        if (ievt == options.nentries):  break

        mean = evt.invPt_mean
        sigma = evt.invPt_sigma

        histos["npatterns"].Fill(mean)

        x.append(ievt)
        y.append(mean)
        ex.append(0)
        ey.append(sigma)
        zeroes.append(0)

    hname = "gr_invPt_meanrms_vs_id"
    h = histos[hname] = style(TGraphErrors(len(x), x, y, zeroes, ey))
    h.SetTitle("; sorted pattern ID; <signed 1/p_{T}> [1/GeV]")

    hname = "gr_invPt_rms_vs_id"
    h = histos[hname] = style(TGraphErrors(len(x), x, zeroes, zeroes, ey))
    h.SetTitle("; sorted pattern ID; #sigma(signed 1/p_{T}) [1/GeV]")

    tree.SetBranchStatus("*", 1)
    return

def drawPatterns(histos, options):
    for hname, h in histos.iteritems():
        if "vs_" not in hname:
            # TH1F
            h.SetMaximum(h.GetMaximum() * 1.4); h.SetMinimum(0)
            h.SetStats(1); h.Draw("hist")
            CMS_label()
            save(options.outdir, hname, dot_root=True)
        elif "gr_" not in hname:
            # TH2F
            h.SetStats(0); h.Draw()
            CMS_label()
            save(options.outdir, hname, dot_root=True, dot_pdf=False)
        else:
            # TGraph
            h.Draw("A3")
            CMS_label()
            save(options.outdir, hname, redraw_axis=False, dot_root=True, dot_pdf=False)

def sitrepPatterns(histos,options):
    pass


# ______________________________________________________________________________
# Main function
def main(options):

    # Init
    drawerInit = DrawerInit()
    tfile = TFile.Open(options.infile)
    ttree = tfile.Get("patternAttributes")

    # Process
    histos = bookPatterns()
    checkoutPatterns(tfile, histos, options)
    projectPatterns(ttree, histos, options)
    drawPatterns(histos, options)
    sitrepPatterns(histos,options)


# ______________________________________________________________________________
if __name__ == '__main__':

    # Setup argument parser
    parser = argparse.ArgumentParser()

    # Add default arguments
    add_drawer_arguments(parser)

    # Add more arguments
    #parser.add_argument("ss", help="short name of superstrip definition (e.g. ss256)")
    #parser.add_argument("npatterns", type=int, help="number of patterns to reach the desired coverage")

    # Parse default arguments
    options = parser.parse_args()
    parse_drawer_options(options)

    if not options.infile.endswith(".root"):
        raise ValueError("infile must be .root file")

    # Call the main function
    main(options)
