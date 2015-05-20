#!/usr/bin/env python

from rootdrawing import *
from parser import *

# Configurations
col  = TColor.GetColor("#1f78b4")  # mu0
fcol = TColor.GetColor("#a6cee3")  # mu0

# Load trigger tower map
ttmap = json.load(open("../data/trigger_sector_map.json"), object_pairs_hook=convert_key_to_int)
ttrmap = get_reverse_map(ttmap)


# ______________________________________________________________________________
def drawer_book():
    histos = {}

    hname = "nstubs_per_layer_%i" % 99
    histos[hname] = TH1F(hname, "; # stubs (all layers)" , 25, 0., 25.)

    for i in xrange(6):
        hname = "nstubs_per_layer_%i" % i
        histos[hname] = TH1F(hname, "; # stubs/layer {L%i}" % i , 10, 0., 10.)

    # Style
    for hname, h in histos.iteritems():
        h.SetLineWidth(2); h.SetMarkerSize(0)
        h.SetLineColor(col); h.SetFillColor(fcol)
    donotdelete.append(histos)
    return histos

def drawer_project(tree, histos, options):
    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("TTStubs_modId", 1)
    tree.SetBranchStatus("TTStubs_tpId" , 1)

    # Loop over events
    for ievt, evt in enumerate(tree):
        if (ievt == options.nentries):  break

        lay_map = {}

        for istub, (moduleId, tpId) in enumerate(izip(evt.TTStubs_modId, evt.TTStubs_tpId)):
            if tpId != 0:
                continue

            lay = decodeLayer(moduleId)
            if lay not in [5, 6, 7, 8, 9, 10]:
                continue

            lay_map[istub] = lay

        hname = "nstubs_per_layer_%i" % 99
        histos[hname].Fill(count_if(lay_map.values(), lambda x: 1))

        for i in xrange(6):
            hname = "nstubs_per_layer_%i" % i
            histos[hname].Fill(count_if(lay_map.values(), lambda x: (x-5) == i))

    tree.SetBranchStatus("*", 1)
    return

def drawer_draw(histos, options):
    for hname, h in histos.iteritems():
        options.logy = True

        h.Draw("hist")
        gPad.SetLogy(options.logy)

        CMS_label()
        save(options.outdir, hname)
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
    #parser.add_argument("ss", help="short name of superstrip definition (e.g. ss256)")
    #parser.add_argument("npatterns", type=int, help="number of patterns to reach the desired coverage")

    # Parse default arguments
    options = parser.parse_args()
    parse_drawer_options(options)

    # Call the main function
    main(options)
