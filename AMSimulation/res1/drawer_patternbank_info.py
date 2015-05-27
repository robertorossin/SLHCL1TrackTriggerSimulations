#!/usr/bin/env python

from rootdrawing import *
from parser import *

cov_stop = 0.95
palette_offset = 0


# ______________________________________________________________________________
def drawer_book():
    superstrips = []
    graphs = {}

    for bank in options.banks:
        m = re.search("(sf.+?_nz.+?)_\w", bank)
        if m:
            ss = m.group(1)
        else:
            raise "Failed to find the superstrip definition from filename"

        tfile = TFile.Open(bank)
        ttree1 = tfile.Get("patternBank")
        ttree2 = tfile.Get("patternBankInfo")
        assert(ttree1.GetEntries() > 0 and ttree2.GetEntries() == 1)

        # Pattern bank info
        ttree2.GetEntry(0)
        bank_coverage = ttree2.coverage
        bank_count = ttree2.count

        # Pattern bank
        ttree1.SetBranchStatus("superstripIds", 0)
        npatterns = ttree1.GetEntries()
        npoints = 1000
        pointwidth = npatterns / npoints

        x, cov, freq = [], [], []
        x_i, integral_i = 0, 0

        for i in xrange(npatterns):
            ttree1.GetEntry(i)

            freq_i = ttree1.frequency

            if i == x_i:
                cov_i = float(integral_i) / float(bank_count) * bank_coverage

                x.append(x_i)
                cov.append(cov_i)
                freq.append(freq_i)
                if options.verbose:  print "..", x_i, cov_i, freq_i

                x_i += pointwidth

            elif i == npatterns - 1:
                cov_i = float(integral_i) / float(bank_count) * bank_coverage

                x.append(x_i)
                cov.append(cov_i)
                freq.append(freq_i)
                if options.verbose:  print "..", x_i, cov_i, freq_i

                assert((integral_i + freq_i) == bank_count)

            integral_i += freq_i

        npatterns_stop = 0
        for x_i, cov_i in izip(x, cov):
            if cov_i > cov_stop:
                npatterns_stop = x_i
                break

        superstrips.append((ss, npatterns, bank_coverage, bank_count, npatterns_stop))
        if options.verbose:  print superstrips[-1]

        npoints = len(x)
        gname = "gr_%s" % ss
        graphs[gname] = TGraph(npoints, array('d', x), array('d', cov))
        graphs[gname].SetName(gname)

        gname = "gr_freq_%s" % ss
        graphs[gname] = TGraph(npoints, array('d', x), array('d', freq))
        graphs[gname].SetName(gname)

    donotdelete.append(superstrips)
    donotdelete.append(graphs)
    return (superstrips, graphs)

def parse_ss(ss):
    if "lu" in ss:
        ss = ss.replace("lu", "")
    elif "sf" in ss:
        ss = ss.replace("sf", "sf=").replace("_nz", ",nz=")
        ss = ss.replace("0p", "0.")
    ss = ss.replace("_", " ")
    return ss

def drawer_draw_cov(superstrips, graphs, options):
    options.logy = False

    hframe = TH1F("hframe", "; # of patterns; running estimate for coverage", 1000, options.xmin, options.xmax)
    hframe.SetStats(0); hframe.SetMinimum(0); hframe.SetMaximum(1.2)

    # Style
    for i, ss in enumerate(superstrips):
        gr = graphs["gr_%s" % ss[0]]
        gr.SetLineWidth(2); gr.SetLineStyle(1); gr.SetMarkerSize(0)
        gr.SetLineColor(paletteSet1[i+palette_offset])

    # Legend
    moveLegend(0.6,0.15,0.9,0.15+len(superstrips)*0.05); tlegend.Clear()
    for i, ss in enumerate(superstrips):
        gr = graphs["gr_%s" % ss[0]]
        tlegend.AddEntry(gr, parse_ss(ss[0]), "l")

    # Draw
    hframe.Draw()
    gPad.SetLogy(options.logy)
    for y in [0.5, 0.8, 0.9, 0.95, 1.0]:
        tline.DrawLine(options.xmin, y, options.xmax, y)
    for i, ss in enumerate(superstrips):
        gr = graphs["gr_%s" % ss[0]]
        gr.Draw("C")
    tlegend.Draw()
    CMS_label()
    save(options.outdir, "coverage_%s" % (superstrips[0][0]), dot_root=True)

    # Zoom in
    hframe.Draw()
    hframe.GetXaxis().SetRangeUser(0, options.xmax/50)
    for y in [0.5, 0.8, 0.9, 0.95, 1.0]:
        tline.DrawLine(options.xmin, y, options.xmax/50, y)
    for i, ss in enumerate(superstrips):
        gr = graphs["gr_%s" % ss[0]]
        gr.Draw("C")
    tlegend.Draw()
    CMS_label()
    save(options.outdir, "coverage_zoom_%s" % (superstrips[0][0]), dot_root=True)

    donotdelete.append(hframe)
    return donotdelete

def drawer_draw_freq(superstrips, graphs, options):
    options.logy = True

    hframe = TH1F("hframe", "; # of patterns; frequency", 1000, options.xmin, options.xmax)
    hframe.SetStats(0); hframe.SetMinimum(0.5); hframe.SetMaximum(1e5)

    # Style
    for i, ss in enumerate(superstrips):
        gr = graphs["gr_freq_%s" % ss[0]]
        gr.SetLineWidth(2); gr.SetLineStyle(1); gr.SetMarkerSize(0)
        gr.SetLineColor(paletteSet1[i+palette_offset])

    # Legend
    moveLegend(0.6,0.15,0.9,0.15+len(superstrips)*0.05); tlegend.Clear()
    for i, ss in enumerate(superstrips):
        gr = graphs["gr_freq_%s" % ss[0]]
        tlegend.AddEntry(gr, parse_ss(ss[0]), "l")

    # Draw
    hframe.Draw()
    gPad.SetLogy(options.logy)
    for i, ss in enumerate(superstrips):
        gr = graphs["gr_freq_%s" % ss[0]]
        gr.Draw("C")
    tlegend.Draw()
    CMS_label()
    save(options.outdir, "frequency_%s" % (superstrips[0][0]), dot_root=True)

    # Zoom in
    hframe.Draw()
    hframe.GetXaxis().SetRangeUser(0, options.xmax/50)
    for i, ss in enumerate(superstrips):
        gr = graphs["gr_freq_%s" % ss[0]]
        gr.Draw("C")
    tlegend.Draw()
    CMS_label()
    save(options.outdir, "frequency_zoom_%s" % (superstrips[0][0]), dot_root=True)

    donotdelete.append(hframe)
    return donotdelete


def drawer_sitrep(superstrips, graphs, options):
    my_ceil = lambda x: int(ceil(float(x)/100.) * 100.)

    print "--- SITREP ---------------------------------------------------------"
    for i, ss in enumerate(superstrips):
        print '{0:9} {1:10d}  {2:5.4f}'.format(ss[0], ss[1], ss[2])
    print
    for i, ss in enumerate(superstrips):
        print '{0:9} {1:10d}  {2:5.4f}'.format(ss[0], my_ceil(ss[4]), cov_stop)



# ______________________________________________________________________________
# Main function
def main(options):

    # Init
    drawerInit = DrawerInit()
    gStyle.SetPadRightMargin(0.1)
    gStyle.SetTitleSize(0.05, "Y")
    gStyle.SetNdivisions(510, "Y")

    # Process
    (superstrips, graphs) = drawer_book()
    drawer_draw_cov(superstrips, graphs, options)
    drawer_draw_freq(superstrips, graphs, options)
    drawer_sitrep(superstrips, graphs, options)


# ______________________________________________________________________________
if __name__ == '__main__':

    # Setup argument parser
    parser = argparse.ArgumentParser()

    # Add default arguments
    add_drawer_arguments(parser)

    # Add more arguments
    #parser.add_argument("ss", help="short name of superstrip definition (e.g. ss256)")
    #parser.add_argument("npatterns", type=int, help="number of patterns to reach the desired coverage")
    parser.add_argument("infile1", nargs="?", default="", help="more input file")
    parser.add_argument("infile2", nargs="?", default="", help="more input file")
    parser.add_argument("infile3", nargs="?", default="", help="more input file")
    parser.add_argument("infile4", nargs="?", default="", help="more input file")
    parser.add_argument("infile5", nargs="?", default="", help="more input file")
    parser.add_argument("infile6", nargs="?", default="", help="more input file")
    parser.add_argument("infile7", nargs="?", default="", help="more input file")
    parser.add_argument("infile8", nargs="?", default="", help="more input file")
    parser.add_argument("infile9", nargs="?", default="", help="more input file")
    parser.add_argument("--xmin", type=float, default=0. , help="x range")
    parser.add_argument("--xmax", type=float, default=5e7, help="x range")
    parser.add_argument("--ymin", type=float, default=0. , help="y range")
    parser.add_argument("--ymax", type=float, default=5e7, help="y range")

    # Parse default arguments
    options = parser.parse_args()
    parse_drawer_options(options)

    # Parse more arguments
    options.infile0 = options.infile
    options.banks = []
    for i in xrange(10):
        if hasattr(options, "infile%i" % i):
            bank = getattr(options, "infile%i" % i)
            if bank and bank.endswith(".root"):
                options.banks.append(bank)

    print "Banks: "
    for bank in options.banks:
        print bank

    # Call the main function
    main(options)
