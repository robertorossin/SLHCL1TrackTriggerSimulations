#!/usr/bin/env python

from rootdrawing import *
from helper import *

donotdelete = []  # persist in memory


# ______________________________________________________________________________
# Main function
def main(options):

    # Init
    # FIXME: read text file?
    drawerInit = DrawerInit()
    tchain = TChain("ntupler/tree", "")
    tchain.Add(options.infile)

    # Process

    #
    # ... DO STUFF HERE ...
    #


# ______________________________________________________________________________
if __name__ == '__main__':

    # Get input arguments
    import argparse
    outdir = (sys.argv[0].replace("drawer_", "figures_"))[:-3]

    parser = argparse.ArgumentParser()
    parser.add_argument("infile", help="input .root file")
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
