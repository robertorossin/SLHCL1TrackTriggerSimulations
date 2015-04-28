#!/usr/bin/env python

from rootdrawing import *
from parser import *
from helper import *

# Configurations
#col  = TColor.GetColor("#e31a1c")  # nu140
#fcol = TColor.GetColor("#fb9a99")  # nu140

col  = TColor.GetColor("#6a3d9a")  # tttt140
fcol = TColor.GetColor("#cab2d6")  # tttt140

# Load trigger tower map
ttmap = json.load(open("../data/trigger_sector_map.json"), object_pairs_hook=convert_key_to_int)
ttrmap = get_reverse_map(ttmap)


# ______________________________________________________________________________
def bookStubs():
    histos = {}

    # TH1F
    hname = "ntracks_pt0p5"
    nbinsx, xmin, xmax = 100, 0., 100.
    histos[hname] = TH1F(hname, "; # tracks/tower/BX {p_{T}>0.5}" , nbinsx, xmin, xmax)

    hname = "ntracks_pt1"
    nbinsx, xmin, xmax = 100, 0., 100.
    histos[hname] = TH1F(hname, "; # tracks/tower/BX {p_{T}>1}"   , nbinsx, xmin, xmax)

    hname = "ntracks_pt2"
    nbinsx, xmin, xmax = 100, 0., 100.
    histos[hname] = TH1F(hname, "; # tracks/tower/BX {p_{T}>2}"   , nbinsx, xmin, xmax)

    hname = "ntracks_pt3"
    nbinsx, xmin, xmax = 100, 0., 100.
    histos[hname] = TH1F(hname, "; # tracks/tower/BX {p_{T}>3}"   , nbinsx, xmin, xmax)

    hname = "nstubs_99"
    nbinsx, xmin, xmax = 1600, 0., 1600.
    histos[hname] = TH1F(hname, "; # stubs/tower/BX"              , nbinsx, xmin, xmax)

    for i in xrange(6):
        hname = "nstubs_%i" % i
        nbinsx, xmin, xmax = 400, 0., 400.
        histos[hname] = TH1F(hname, "; # stubs/tower/BX {L%i}" % i, nbinsx, xmin, xmax)

    # Style
    for hname, h in histos.iteritems():
        h.SetLineWidth(2); h.SetMarkerSize(0)
        h.SetLineColor(col); h.SetFillColor(fcol)
        if h.ClassName() == "TH1F":
            binwidth = (h.GetXaxis().GetXmax() - h.GetXaxis().GetXmin())/h.GetNbinsX()
            h.SetYTitle("Entries / %.1f" % binwidth)
    donotdelete.append(histos)
    return histos


def projectStubs(tree, histos, options):
    # Merge barrel and endcap layers
    layermap = {
        5:0, 6:1, 7:2, 8:3, 9:4, 10:5,
        11:5, 12:4, 13:3, 14:2, 15:1,
        18:5, 19:4, 20:3, 21:2, 22:1,
    }

    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("trkParts_pt"           , 1)
    tree.SetBranchStatus("trkParts_eta"          , 1)
    tree.SetBranchStatus("trkParts_phi"          , 1)
    tree.SetBranchStatus("trkParts_primary"      , 1)
    tree.SetBranchStatus("TTStubs_modId"         , 1)

    # Loop over events
    for ievt, evt in enumerate(tree):
        if (ievt == options.nentries):  break
        if (ievt % 100 == 0):  print "Processing event: %i" % ievt

        ntracks = {0.5: 0, 1: 0, 2: 0, 3: 0}
        nstubs = {0: 0, 1: 0, 2: 0, 3: 0, 4: 0, 5: 0, 99: 0}

        # ntracks
        for pt, eta, phi, primary in izip(evt.trkParts_pt, evt.trkParts_eta, evt.trkParts_phi, evt.trkParts_primary):
            if (options.etamin < eta < options.etamax and options.phimin < phi < options.phimax and primary):
                for x in ntracks.keys():
                    if pt > x:
                        ntracks[x] += 1

        for x in ntracks.keys():
            hname = "ntracks_pt" + str(x)
            hname = hname.replace(".", "p")
            histos[hname].Fill(ntracks[x])

        # nstubs
        for moduleId in evt.TTStubs_modId:
            lay = decodeLayer(moduleId)
            mlay = layermap[lay]

            if moduleId in ttrmap and options.tower in ttrmap[moduleId]:
                nstubs[mlay] += 1
                nstubs[99] += 1

        for x in nstubs.keys():
            hname = "nstubs_" + str(x)
            hname = hname.replace(".", "p")
            histos[hname].Fill(nstubs[x])

    tree.SetBranchStatus("*", 1)
    return


def drawStubs(histos, options):
    for hname, h in histos.iteritems():
        # TH1F
        h.SetMaximum(h.GetMaximum() * 1.4); h.SetMinimum(0)
        h.SetStats(1); h.Draw("hist")
        CMS_label()
        save(options.outdir, hname, dot_root=True)


def sitrepStubs(histos, options):
    pass


# ______________________________________________________________________________
# Main function
def main(options):

    # Init
    drawerInit = DrawerInit()
    tchain = TChain("ntupler/tree", "")
    tchain.AddFileInfoList(options.tfilecoll.GetList())

    # Process
    histos = bookStubs()
    projectStubs(tchain, histos, options)
    drawStubs(histos, options)
    sitrepStubs(histos, options)


# ______________________________________________________________________________
if __name__ == '__main__':

    # Setup argument parser
    parser = argparse.ArgumentParser()

    # Add default arguments
    add_drawer_arguments(parser)

    # Parse default arguments
    options = parser.parse_args()
    parse_drawer_options(options)

    # Call the main function
    main(options)
