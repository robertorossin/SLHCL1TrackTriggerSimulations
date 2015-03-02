#!/usr/bin/env python

from rootdrawing import *
from helper import *
from array import array

# Configurations
#col  = TColor.GetColor("#e31a1c")  # nu140
#fcol = TColor.GetColor("#fb9a99")  # nu140

col  = TColor.GetColor("#6a3d9a")  # tttt140
fcol = TColor.GetColor("#cab2d6")  # tttt140

donotdelete = []  # persist in memory


# ______________________________________________________________________________
def bookRoads():
    def modify_binning(nbins, xmin, xmax):
        binsize = (xmax - xmin) / nbins
        return (nbins + 1, xmin - (binsize/2.), xmax + (binsize/2.))

    histos = {}

    hname = "nroads_per_event"
    nbins, xmin, xmax = modify_binning(150, 0., 300.)
    histos[hname] = TH1F(hname, "; # roads/tower/BX; Entries"                , nbins, xmin, xmax)

    hname = "nsuperstrips_per_road"
    nbins, xmin, xmax = modify_binning(20, 0., 20.)
    histos[hname] = TH1F(hname, "; # superstrips/road/tower/BX; Entries"     , nbins, xmin, xmax)

    hname = "nstubs_per_superstrip"
    nbins, xmin, xmax = modify_binning(50, 0., 50.)
    histos[hname] = TH1F(hname, "; # stubs/superstrip/road/tower/BX; Entries", nbins, xmin, xmax)

    hname = "nstubs_per_road"
    nbins, xmin, xmax = modify_binning(50, 0., 50.)
    histos[hname] = TH1F(hname, "; # stubs/road/tower/BX; Entries"           , nbins, xmin, xmax)

    hname = "nstubs_per_event"
    nbins, xmin, xmax = modify_binning(150, 0., 300.)
    histos[hname] = TH1F(hname, "; # stubs/tower/BX; Entries"                , nbins, xmin, xmax)

    hname = "ncombinations_per_road"
    nbins, xmin, xmax = modify_binning(100, 0., 500.)
    histos[hname] = TH1F(hname, "; # combinations/road/tower/BX; Entries"    , nbins, xmin, xmax)

    hname = "ncombinations_per_event"
    nbins, xmin, xmax = modify_binning(150, 0., 1500.)
    histos[hname] = TH1F(hname, "; # combinations/tower/BX; Entries"         , nbins, xmin, xmax)

    for i in xrange(6):
        hname = "nstubs_per_layer_%i" % i
        nbins, xmin, xmax = modify_binning(50, 0., 50.)
        histos[hname] = TH1F(hname, "; # stubs/layer/road/tower/BX; Entries", nbins, xmin, xmax)

    # Style
    for k, v in histos.iteritems():
        v.SetLineWidth(2); v.SetMarkerSize(0)
        v.SetLineColor(col); v.SetFillColor(fcol)
    donotdelete.append(histos)
    return histos


def projectRoads(tree, histos, options):
    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("AMTTRoads_bankIndex"        , 1)
    tree.SetBranchStatus("AMTTRoads_nsuperstrips"     , 1)
    tree.SetBranchStatus("AMTTRoads_stubSuperstripIds", 1)
    tree.SetBranchStatus("AMTTRoads_stubRefs"         , 1)

    # Loop over events
    for ievt, evt in enumerate(tree):
        if (ievt == options.nentries):  break

        nroads_per_event = 0
        ncombinations_per_event = 0

        stubmap = {}

        for bankIndex, nsuperstrips, superstripIds, stubRefs in izip(evt.AMTTRoads_bankIndex, evt.AMTTRoads_nsuperstrips, evt.AMTTRoads_stubSuperstripIds, evt.AMTTRoads_stubRefs):

            if bankIndex < options.npatterns:
                ssidmap = {}
                for ssid in superstripIds:
                    ssidmap[ssid] = ssidmap.get(ssid, 0) + 1
                for stub in stubRefs:
                    stubmap[stub] = stubmap.get(stub, 0) + 1

                nsuperstrips_per_road = len(ssidmap)
                assert(nsuperstrips_per_road == nsuperstrips)
                histos["nsuperstrips_per_road"].Fill(nsuperstrips_per_road)

                nstubs_per_road = 0
                ncombinations_per_road = 1

                for k, v in ssidmap.iteritems():
                    nstubs_per_superstrip = v
                    nstubs_per_road += v
                    ncombinations_per_road *= v

                    histos["nstubs_per_superstrip"].Fill(nstubs_per_superstrip)

                # Per layer
                # FIXME: is sorted correctly?
                for ik, k in enumerate(sorted(ssidmap.keys())):
                    nstubs_per_layer = ssidmap[k]
                    histos["nstubs_per_layer_%i" % ik].Fill(nstubs_per_layer)

                assert(nstubs_per_road == superstripIds.size())
                histos["nstubs_per_road"].Fill(nstubs_per_road)

                histos["ncombinations_per_road"].Fill(ncombinations_per_road)

                nroads_per_event += 1
                ncombinations_per_event += ncombinations_per_road

        assert(nroads_per_event <= evt.AMTTRoads_nsuperstrips.size())
        histos["nroads_per_event"].Fill(nroads_per_event)

        nstubs_per_event = len(stubmap)
        histos["nstubs_per_event"].Fill(nstubs_per_event)

        histos["ncombinations_per_event"].Fill(ncombinations_per_event)

    tree.SetBranchStatus("*", 1)
    return


def drawRoads(histos, options, logy=True):
    def parse_ss(ss):
        if "lu" in ss:
            ss = ss.replace("lu", "")
        elif "sf" in ss:
            ss = ss.replace("sf", "sf=").replace("_nz", ",nz=")
            ss = ss.replace("0p", "0.")
        ss = ss.replace("_", " ")
        return ss

    def displayQuantiles(h, in_quantiles=[0.95,0.99], scalebox=(1.,1.)):
        # Display one-sided confidence intervals, a.k.a quantiles
        n = len(in_quantiles)
        in_quantiles = array('d', in_quantiles)
        quantiles = array('d', [0. for i in xrange(n)])
        h.GetQuantiles(n, quantiles, in_quantiles)

        gPad.Modified(); gPad.Update()
        ps = h.FindObject("stats")
        ps.SetName("mystats")

        newX1NDC = ps.GetX2NDC() - (ps.GetX2NDC() - ps.GetX1NDC()) * scalebox[0]
        newY1NDC = ps.GetY2NDC() - ((ps.GetY2NDC() - ps.GetY1NDC()) / 5 * (5 + n)) * scalebox[1]
        ps.SetX1NDC(newX1NDC)
        ps.SetY1NDC(newY1NDC)

        for iq, q in enumerate(in_quantiles):
            ps.AddText("%i%% CI = %6.4g" % (int(q*100), quantiles[iq]))
        h.SetStats(0)
        #gPad.Modified(); gPad.Update()
        ps.Draw()

    for hname, h in histos.iteritems():
        if "per_layer" in hname:
            continue

        if logy:
            h.SetMaximum(h.GetMaximum() * 14); h.SetMinimum(0.5)
        else:
            h.SetMaximum(h.GetMaximum() * 1.4); h.SetMinimum(0.)
        h.SetStats(1); h.Draw("hist")
        gPad.SetLogy(logy)
        displayQuantiles(h)

        tlatex.DrawLatex(0.6, 0.185, "%s [%.2fM bank]" % (parse_ss(options.ss), options.npatterns*1e-6))
        CMS_label()
        save(options.outdir, "%s_%s" % (hname, options.ss), dot_root=True)

    # Specialized: nstubs_per_layer_%i
    if True:
        c1 = TCanvas("c1", "c1")
        lmargin, rmargin, bmargin, tmargin = (gStyle.GetPadLeftMargin(), gStyle.GetPadRightMargin(), gStyle.GetPadBottomMargin(), gStyle.GetPadTopMargin())
        width, height = (1.0 - lmargin - rmargin) * 0.5, (1.0 - bmargin - tmargin) * 0.333
        pad1 = TPad("pad1", "pad1", lmargin+width*0, 1.0-tmargin-height*1, lmargin+width*1, 1.0-tmargin-height*0)
        pad1.SetNumber(1); pad1.SetMargin(0.0, 0.0, 0.0, 0.0)
        pad1.Draw()
        pad2 = TPad("pad2", "pad2", lmargin+width*0, 1.0-tmargin-height*2, lmargin+width*1, 1.0-tmargin-height*1)
        pad2.SetNumber(2); pad2.SetMargin(0.0, 0.0, 0.0, 0.0)
        pad2.Draw()
        pad3 = TPad("pad3", "pad3", lmargin+width*0, 1.0-tmargin-height*3, lmargin+width*1, 1.0-tmargin-height*2)
        pad3.SetNumber(3); pad3.SetMargin(0.0, 0.0, 0.0, 0.0)
        pad3.Draw()
        pad4 = TPad("pad4", "pad4", lmargin+width*1, 1.0-tmargin-height*1, lmargin+width*2, 1.0-tmargin-height*0)
        pad4.SetNumber(4); pad4.SetMargin(0.0, 0.0, 0.0, 0.0)
        pad4.Draw()
        pad5 = TPad("pad5", "pad5", lmargin+width*1, 1.0-tmargin-height*2, lmargin+width*2, 1.0-tmargin-height*1)
        pad5.SetNumber(5); pad5.SetMargin(0.0, 0.0, 0.0, 0.0)
        pad5.Draw()
        pad6 = TPad("pad6", "pad6", lmargin+width*1, 1.0-tmargin-height*3, lmargin+width*2, 1.0-tmargin-height*2)
        pad6.SetNumber(6); pad6.SetMargin(0.0, 0.0, 0.0, 0.0)
        pad6.Draw()
        c1.cd()
        c1.Modified(); c1.Update()

        temps = (tlatex.GetTextSize(),)
        tlatex.SetTextSize(0.08)

        ymax = -1

        for i in xrange(6):
            hname = "nstubs_per_layer_%i" % i
            h = histos[hname]
            c1.cd(i+1)

            # Style
            #print h.GetXaxis().GetTitleOffset(), h.GetXaxis().GetTitleSize(), h.GetXaxis().GetTitleSize(), h.GetYaxis().GetTitleOffset(), h.GetYaxis().GetTitleSize(), h.GetYaxis().GetTitleSize()
            if i/3 == 0:
                #h.GetYaxis().SetTitleOffset(0.90)
                h.GetYaxis().SetTitleSize(h.GetYaxis().GetTitleSize() * 2)
                h.GetYaxis().SetLabelSize(h.GetYaxis().GetLabelSize() * 2)
            else:
                h.GetYaxis().SetTitle("")
                h.GetYaxis().SetLabelSize(0)

            if i%3 == 2:
                #h.GetXaxis().SetTitleOffset(1.10)
                h.GetXaxis().SetTitleSize(h.GetXaxis().GetTitleSize() * 2)
                h.GetXaxis().SetLabelSize(h.GetXaxis().GetLabelSize() * 2)
            else:
                h.GetXaxis().SetTitle("")
                h.GetXaxis().SetLabelSize(0)

            if ymax == -1:
                ymax = h.GetMaximum()

            if logy:
                h.SetMaximum(ymax * 14); h.SetMinimum(0.5)
            else:
                h.SetMaximum(ymax * 1.4); h.SetMinimum(0.)
            h.SetStats(1); h.Draw("hist")
            gPad.SetLogy(logy)
            displayQuantiles(h, scalebox=(2.,2.))

            tlatex.DrawLatex(0.5, 0.260, "Layer %i" % i)
            tlatex.DrawLatex(0.5, 0.185, "%s [%.2fM bank]" % (parse_ss(options.ss), options.npatterns*1e-6))

        c1.cd(0)
        CMS_label()
        hname = "nstubs_per_layer"
        save(options.outdir, "%s_%s" % (hname, options.ss), dot_root=True)

        tlatex.SetTextSize(temps[0])
    return


# ______________________________________________________________________________
# Main function
def main(options):

    # Init
    # FIXME: read text file?
    drawerInit = DrawerInit()
    tchain = TChain("ntupler/tree", "")
    tchain.Add(options.infile)

    # Process
    histos = bookRoads()
    projectRoads(tchain, histos, options)
    drawRoads(histos, options)


# ______________________________________________________________________________
if __name__ == '__main__':

    # Get input arguments
    import argparse
    outdir = (sys.argv[0].replace("drawer_", "figures_"))[:-3]

    parser = argparse.ArgumentParser()
    parser.add_argument("infile", help="input .root file")
    parser.add_argument("ss", help="short name of superstrip definition (e.g. ss256)")
    parser.add_argument("npatterns", type=int, help="number of patterns to reach the desired coverage")
    parser.add_argument("--tower", type=int, default=27, help="trigger tower (default: %(default)s)")
    parser.add_argument("--outdir", default=outdir, help="output directory (default: %(default)s)")
    parser.add_argument("-n", "--nentries", type=int, default=100000, help="number of entries (default: %(default)s)")
    parser.add_argument("-b", "--batch", action="store_true", help="batch mode without graphics (default: %(default)s)")
    parser.add_argument("-v", "--verbosity", action="count", default=0, help="verbosity (default: %(default)s)")
    options = parser.parse_args()
    #print options

    # Create outdir if necessary
    if not options.outdir.endswith("/"):
        options.outdir += "/"
    if gSystem.AccessPathName(options.outdir):
        gSystem.mkdir(options.outdir)

    # Check input arguments
    if not options.infile.endswith(".root"):
        raise ValueError("infile must be a .root file")
    if options.batch:
        gROOT.SetBatch(True)

    # Call the main function
    main(options)
