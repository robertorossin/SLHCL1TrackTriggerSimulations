#!/usr/bin/env python

from ROOT import TGraph, TFile
from array import array
import sys
verbose = 0


# ______________________________________________________________________________
def parse(infile):
    data = []

    with open(infile) as f:
        for line in f.readlines():
            data_i = line.strip().split()
            data_f = data_i[0:3]
            data_f = map(int, data_f)
            data.append(tuple(data_f))
    return data

def make_graph(data, npoints=1000):
    bank_count = data[-1][1]
    bank_coverage = 1.0

    npatterns = len(data)
    pointwidth = npatterns / npoints

    x, cov, freq = [], [], []
    x_i, integral_i = 0, 0

    for i in xrange(npatterns):

        freq_i = data[i][2]

        if i == x_i:
            cov_i = float(integral_i) / float(bank_count) * bank_coverage

            x.append(x_i)
            cov.append(cov_i)
            freq.append(freq_i)
            if verbose:  print "..", x_i, cov_i, freq_i

            x_i += pointwidth

        elif i == npatterns - 1:
            cov_i = float(integral_i) / float(bank_count) * bank_coverage

            x.append(x_i)
            cov.append(cov_i)
            freq.append(freq_i)
            if verbose:  print "..", x_i, cov_i, freq_i

            assert((integral_i + freq_i) == bank_count)

        integral_i += freq_i

    npoints = len(x)
    gname = "gr"
    gr = TGraph(npoints, array('d', x), array('d', cov))
    gr.SetName(gname)
    return gr

def write_graph(outfile, gr):
    tfile = TFile.Open(outfile, "RECREATE")
    gr.Write()
    tfile.Close()


# ______________________________________________________________________________
def main():
    if len(sys.argv) < 2:
        raise "Please provide the pattern bank."

    if "-h" in sys.argv or "--help" in sys.argv:
        print "usage: %s %s" % (sys.argv[0], "patternBank")
        sys.exit(0)

    if not sys.argv[1].endswith(".txt"):
        raise "Please provide the pattern bank as .txt"

    # Process
    data = parse(sys.argv[1])

    gr = make_graph(data)

    write_graph(sys.argv[1].replace(".txt",".root"), gr)


# ______________________________________________________________________________
if __name__ == '__main__':

    main()
