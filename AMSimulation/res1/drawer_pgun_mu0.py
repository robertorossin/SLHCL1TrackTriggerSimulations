#!/usr/bin/env python

from rootdrawing import *
from parser import *

col  = TColor.GetColor("#1f78b4")  # mu0
fcol = TColor.GetColor("#a6cee3")  # mu0


# ______________________________________________________________________________
def drawer_book():
    histos = {}

    hname = "genParts_pt"
    histos[hname] = TH1F(hname, "; gen p_{T} [GeV]", 220, 0, 2200)

    hname = "genParts_invPt"
    histos[hname] = TH1F(hname, "; gen 1/p_{T} [1/GeV]", 220, -0.6, 0.6)

    hname = "genParts_eta"
    histos[hname] = TH1F(hname, "; gen #eta", 300, -3, 3)

    hname = "genParts_phi"
    histos[hname] = TH1F(hname, "; gen #phi [#pi rad]", 220, -1.1, 1.1)

    hname = "genParts_vx"
    histos[hname] = TH1F(hname, "; gen vertex x [cm]", 200, -0.01, 0.01)

    hname = "genParts_vy"
    histos[hname] = TH1F(hname, "; gen vertex y [cm]", 200, -0.01, 0.01)

    hname = "genParts_vz"
    histos[hname] = TH1F(hname, "; gen vertex z [cm]", 200, -30, 30)

    hname = "genParts_charge"
    histos[hname] = TH1F(hname, "; gen charge", 5, -2.5, 2.5)

    hname = "genParts_pdgId"
    histos[hname] = TH1F(hname, "; gen PDG ID", 51, -25.5, 25.5)

    # Style
    for hname, h in histos.iteritems():
        h.SetLineWidth(2); h.SetMarkerSize(0)
        h.SetLineColor(col); h.SetFillColor(fcol)
    donotdelete.append(histos)
    return histos

def drawer_project(tree, histos, options):
    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("genParts_pt"     , 1)
    tree.SetBranchStatus("genParts_eta"    , 1)
    tree.SetBranchStatus("genParts_phi"    , 1)
    tree.SetBranchStatus("genParts_vx"     , 1)
    tree.SetBranchStatus("genParts_vy"     , 1)
    tree.SetBranchStatus("genParts_vz"     , 1)
    tree.SetBranchStatus("genParts_charge" , 1)
    tree.SetBranchStatus("genParts_pdgId"  , 1)

    # Loop over events
    for ievt, evt in enumerate(tree):
        if (ievt == options.nentries):  break

        nparts = evt.genParts_pt.size()
        assert(nparts == 1)

        ipart = 0
        pt      = evt.genParts_pt     [ipart]
        eta     = evt.genParts_eta    [ipart]
        phi     = evt.genParts_phi    [ipart]
        vx      = evt.genParts_vx     [ipart]
        vy      = evt.genParts_vy     [ipart]
        vz      = evt.genParts_vz     [ipart]
        charge  = evt.genParts_charge [ipart]
        pdgId   = evt.genParts_pdgId  [ipart]

        histos["genParts_pt"     ].Fill(pt)
        histos["genParts_invPt"  ].Fill(charge/pt)
        histos["genParts_eta"    ].Fill(eta)
        histos["genParts_phi"    ].Fill(phi/pi)
        histos["genParts_vx"     ].Fill(vx)
        histos["genParts_vy"     ].Fill(vy)
        histos["genParts_vz"     ].Fill(vz)
        histos["genParts_charge" ].Fill(charge)
        histos["genParts_pdgId"  ].Fill(pdgId)

    tree.SetBranchStatus("*", 1)
    return

def drawer_draw(histos, options):
    for hname, h in histos.iteritems():
        if hname in ["genParts_pt"]:
            options.logy = True
        else:
            options.logy = False

        if options.logy:
            h.SetMaximum(h.GetMaximum() * 14); h.SetMinimum(0.5)
        else:
            h.SetMaximum(h.GetMaximum() * 1.4); h.SetMinimum(0.)

        if hname == "genParts_vz":
            h.Fit("gaus", "N")

        h.Draw("hist")
        gPad.SetLogy(options.logy)

        CMS_label()
        save(options.outdir, hname)

    return

def drawer_sitrep(histos, options):
    print "--- SITREP ---------------------------------------------------------"
    print "n/a"


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
    #parser.add_argument("ss", help="short name of superstrip definition (e.g. ss256)")
    #parser.add_argument("npatterns", type=int, help="number of patterns to reach the desired coverage")

    # Parse default arguments
    options = parser.parse_args()
    parse_drawer_options(options)

    # Call the main function
    main(options)
