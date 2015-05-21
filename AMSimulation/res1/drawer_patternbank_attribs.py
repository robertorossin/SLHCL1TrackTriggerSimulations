#!/usr/bin/env python

from rootdrawing import *
from parser import *


# ______________________________________________________________________________
def drawer_book():
    histos = {}

    hname = "npatterns"
    nbinsx, xmin, xmax = 1000, -0.5, 0.5
    h = histos[hname] = TH1F(hname, "; <signed 1/p_{T}> [1/GeV]; # patterns", nbinsx, xmin, xmax)
    h.SetLineColor(kBlue)

    donotdelete.append(histos)
    return histos

def drawer_checkout(tfile, histos, options):
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
    h = histos[hname] = style(tfile.Get(hname))

    hname = "diff_invPt_vs_invPt2"
    h = histos[hname] = style(tfile.Get(hname))

    hname = "diff_invPt_pr_invPt1"
    h = histos[hname] = style(tfile.Get(hname))
    h.SetMinimum(-0.05); h.SetMaximum(0.05)

    hname = "diff_invPt_pr_invPt2"
    h = histos[hname] = style(tfile.Get(hname))
    h.SetMinimum(-0.05); h.SetMaximum(0.05)

def drawer_project(tree, histos, options):
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

    # Rebin
    nbinsx = 50
    nelemsx = len(x)
    x_1, y_1, ex_1, ey_1, zeroes_1 = array('d'), array('d'), array('d'), array('d'), array('d')
    for ievt in xrange(nbinsx):
        binwidth = int(nelemsx / nbinsx)
        beg = binwidth * ievt
        end = binwidth * (ievt+1)

        x_1.append(ievt*binwidth)
        y_1.append(sum(y[beg:end]) / binwidth)
        ex_1.append(sum(ex[beg:end]) / binwidth)
        ey_1.append(sum(ey[beg:end]) / binwidth)
        zeroes_1.append(sum(zeroes[beg:end]) / binwidth)

    hname = "gr_invPt_meanrms_vs_id"
    #h = histos[hname] = style(TGraphErrors(len(x), x, y, zeroes, ey))
    h = histos[hname] = style(TGraphErrors(len(x_1), x_1, y_1, zeroes_1, ey_1))
    h.SetName(hname)
    h.SetTitle("; sorted pattern ID; <signed 1/p_{T}> [1/GeV]")

    hname = "gr_invPt_rms_vs_id"
    #h = histos[hname] = style(TGraphErrors(len(x), x, zeroes, zeroes, ey))
    h = histos[hname] = style(TGraphErrors(len(x_1), x_1, zeroes_1, zeroes_1, ey_1))
    h.SetName(hname)
    h.SetTitle("; sorted pattern ID; #sigma(signed 1/p_{T}) [1/GeV]")
    h.SetMinimum(-0.01); h.SetMaximum(0.01)

    hname = "gr_invPt_rms_vs_mean"
    #h = histos[hname] = style(TGraphErrors(len(x), y, zeroes, zeroes, ey))
    h = histos[hname] = style(TGraphErrors(len(x_1), y_1, zeroes_1, zeroes_1, ey_1))
    h.SetName(hname)
    h.SetTitle("; <signed 1/p_{T}> [1/GeV]; #sigma(signed 1/p_{T}) [1/GeV]")
    h.SetMinimum(-0.01); h.SetMaximum(0.01)

    tree.SetBranchStatus("*", 1)
    return

def drawer_draw(histos, options):
    for hname, h in histos.iteritems():
        if "gr_" not in hname:
            if "vs_" not in hname and "pr_" not in hname:
                # TH1F
                h.SetMaximum(h.GetMaximum() * 1.4); h.SetMinimum(0)
                h.SetStats(1); h.Draw("hist")
                CMS_label()
                save(options.outdir, hname, dot_root=True)
            elif "pr_" not in hname:
                # TH2F
                h.SetStats(0); h.Draw()
                CMS_label()
                save(options.outdir, hname, dot_root=True, dot_pdf=False)
            else:
                # TProfile
                h.SetStats(1); h.Draw()
                print hname, h.GetRMS(1), h.GetRMS(2)
                CMS_label()
                save(options.outdir, hname, dot_root=True, dot_pdf=False)
        else:
            # TGraph
            #h.Draw("A3")
            h.Draw("AP")
            CMS_label()
            save(options.outdir, hname, redraw_axis=False, dot_root=True, dot_pdf=False)

    # Specialized
    hnames = ["diff_invPt_vs_invPt1", "diff_invPt_vs_invPt2"]
    for hname in hnames:
        h = histos[hname]
        hname += "_colz"
        h.SetStats(0); h.Draw("colz")
        CMS_label()
        save(options.outdir, hname, dot_root=True, dot_pdf=False)

def drawer_sitrep(histos, options):
    print "--- SITREP ---------------------------------------------------------"


# ______________________________________________________________________________
# Main function
def main(options):

    # Init
    drawerInit = DrawerInit()
    tfile = TFile.Open(options.infile)
    ttree = tfile.Get("patternAttributes")

    # Process
    histos = drawer_book()
    drawer_checkout(tfile, histos, options)
    drawer_project(ttree, histos, options)
    drawer_draw(histos, options)
    drawer_sitrep(histos, options)


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
