import argparse
from StringIO import StringIO
from ROOT import TFileCollection, gROOT, gSystem
from math import pi

def add_drawer_arguments(parser):
    outdir = (parser.prog.replace("drawer_", "figures_"))[:-3]
    parser.add_argument("infile", help="input file (either .root file or .txt file listing .root files)")
    parser.add_argument("--tower", type=int, default=27, help="trigger tower (default: %(default)s)")
    parser.add_argument("--pu", type=int, default=140, help="number of pileup interactions (default: %(default)s)")
    parser.add_argument("--outdir", default=outdir, help="output directory (default: %(default)s)")
    parser.add_argument("-n", "--nentries", type=int, default=-1, help="number of entries (default: %(default)s)")
    parser.add_argument("-b", "--batch", action="store_true", help="batch mode without graphics (default: %(default)s)")
    parser.add_argument("-v", "--verbosity", action="count", default=0, help="verbosity (default: %(default)s)")

def parse_drawer_options(options):
    # Create outdir if necessary
    if not options.outdir.endswith("/"):
        options.outdir += "/"
    if gSystem.AccessPathName(options.outdir):
        gSystem.mkdir(options.outdir)

    # Make input file list
    if not options.infile.endswith(".root") and not options.infile.endswith(".txt"):
        raise ValueError("infile must be .root file or .txt file")
    if options.infile.endswith(".root"):
        options.tfilecoll = TFileCollection("fc", "")
        options.tfilecoll.Add(options.infile)
    else:
        options.tfilecoll = TFileCollection("fc", "", options.infile)

    # Batch mode
    if options.batch:
        gROOT.SetBatch(True)

    # Trigger tower parameter space
    if options.tower != 99:
        ieta = options.tower/8
        iphi = options.tower%8
        options.etamin = -2.2 + (4.4/6) * ieta
        options.etamax = -2.2 + (4.4/6) * (ieta+1)
        if iphi < 6:
            options.phimin = -pi/2 + (2*pi/8) * iphi
            options.phimax = -pi/2 + (2*pi/8) * (iphi+1)
        else:
            options.phimin = -2*pi -pi/2 + (2*pi/8) * iphi
            options.phimax = -2*pi -pi/2 + (2*pi/8) * (iphi+1)

