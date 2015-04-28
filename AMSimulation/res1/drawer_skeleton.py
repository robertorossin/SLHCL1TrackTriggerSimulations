#!/usr/bin/env python

from rootdrawing import *
from parser import *


# ______________________________________________________________________________
def bookSpam():
    histos = {}

    donotdelete.append(histos)
    return histos

def projectSpam(tree, histos, options):
    tree.SetBranchStatus("*", 0)

    # Loop over events
    for ievt, evt in enumerate(tree):
        if (ievt == options.nentries):  break

    tree.SetBranchStatus("*", 1)
    return

def drawSpam(histos, options):
    return

def sitrepSpam(histos, options):
    print "--- SITREP ---------------------------------------------------------"


# ______________________________________________________________________________
# Main function
def main(options):

    # Init
    drawerInit = DrawerInit()
    tchain = TChain("ntupler/tree", "")
    tchain.AddFileInfoList(options.tfilecoll.GetList())

    # Process

    #
    # ... DO STUFF HERE ...
    #
    # e.g.
    #histos = bookSpam()
    #projectSpam(tchain, histos, options)
    #drawSpam(histos, options)
    #sitrepSpam(histos,options)


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
