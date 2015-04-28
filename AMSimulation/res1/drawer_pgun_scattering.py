#!/usr/bin/env python

from rootdrawing import *
from parser import *
from math import sin, cos, tan, sinh, asin, exp, atan

# Load trigger tower map
ttmap = json.load(open("../data/trigger_sector_map.json"), object_pairs_hook=convert_key_to_int)
ttrmap = get_reverse_map(ttmap)

# Load module vertices
vertexmap = json.load(open("../data/module_vertices.json"), object_pairs_hook=convert_key_to_int)

nbinsx, xmin, xmax = 1500/5, -40., 110.
nbinsy, ymin, ymax = 1500/5,   0., 150.
nbinsz, zmin, zmax = 1500/5, -30., 120.

tlatex2 = TLatex()
tlatex2.SetNDC(0)
tlatex2.SetTextFont(42)
tlatex2.SetTextSize(0.02)

tline2 = TLine()
tline2.SetLineColor(kGray+2)
tline2.SetLineStyle(1)
tline2.SetLineWidth(2)


# ______________________________________________________________________________
def bookTrack():
    histos = {}

    hname = "xy"
    histos[hname] = TH2F(hname, "; x [cm]; y [cm]", nbinsx, xmin, xmax, nbinsy, ymin, ymax)

    hname = "rz"
    histos[hname] = TH2F(hname, "; z [cm]; r [cm]", nbinsz, zmin, zmax, nbinsy, ymin, ymax)

    # Style
    for hname, h in histos.iteritems():
        h.SetMarkerSize(1); h.SetMarkerStyle(5)
    donotdelete.append(histos)
    return histos

def make_tpolyline(l, view):
    xy = set()
    for i in xrange(4):
        c = l[3*i:3*(i+1)]
        if view == "XY":
            c = (c[0], c[1])
        elif view == "RZ":
            c = (c[2], sqrt(c[0]*c[0] + c[1]*c[1]))
        xy.add(c)
    n, x, y = 0, [], []
    for c in xy:
        x.append(c[0])
        y.append(c[1])
        n += 1
    if n == 4:  # close the poly line in case it's made of 4 distinct points
        x.append(x[0])
        y.append(y[0])
        n += 1
    tpolyline = TPolyLine(n, array('d', x), array('d', y))
    return tpolyline

def get_tpolylines(view):
    tpolylines = []

    if view == "XY":
        towers = [28, 29, 30, 31, 24, 25, 26, 27]  # put 27 last
    elif view == "RZ":
        towers = [35, 43, 3, 11, 19, 27]  # put 27 last

    def style_tpolyline(l, module, tower):
        l.SetLineStyle(1)
        l.SetLineWidth(2)
        if tower == towers[-1]:
            if isPSModule(module):
                l.SetLineColor(palette[0])
            else:
                l.SetLineColor(palette[1])
        else:
            if isPSModule(module):
                l.SetLineColor(kGray)
            else:
                l.SetLineColor(kGray)

    for tower in towers:
        for module in ttmap[tower]:
            for module_i in [module, -module]:
                tpolyline = make_tpolyline(vertexmap[module_i], view)
                style_tpolyline(tpolyline, module, tower)
                tpolylines.append(tpolyline)
    return tpolylines

def get_tgraphs(pt, eta, phi, vz):
    n = 50

    xp, xm, yp, ym, rr, zz = [], [], [], [], [], []

    for i in xrange(n+1):
        r = (ymax - ymin) / float(n) * i
        cc = 0.3*3.8*1e-2/2.0
        #deltaPhi = - cc*r*1.0/float(pt)
        deltaPhi = - asin(cc*r*1.0/float(pt))

        xp.append(r * cos(phi + deltaPhi))
        xm.append(r * cos(phi - deltaPhi))
        yp.append(r * sin(phi + deltaPhi))
        ym.append(r * sin(phi - deltaPhi))
        rr.append(r)
        zz.append(r * sinh(eta))
    n += 1

    gxyp = TGraph(n, array('d', xp), array('d', yp))
    gxym = TGraph(n, array('d', xm), array('d', ym))
    grz = TGraph(n, array('d', zz), array('d', rr))
    tgraphs = [gxyp, gxym, grz]

    def style_tgraph(g):
        g.SetLineStyle(1)
        g.SetLineWidth(2)
        g.SetLineColor(palette[3])

    for g in tgraphs:
        style_tgraph(g)
    return tgraphs


def projectTrack(tree, histos, options):
    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("TTStubs_r"      , 1)
    tree.SetBranchStatus("TTStubs_phi"    , 1)
    tree.SetBranchStatus("TTStubs_z"      , 1)
    #tree.SetBranchStatus("genParts_pt"    , 1)
    #tree.SetBranchStatus("genParts_eta"   , 1)
    #tree.SetBranchStatus("genParts_phi"   , 1)
    #tree.SetBranchStatus("genParts_vz"    , 1)
    tree.SetBranchStatus("genParts_charge", 1)

    # Loop over events
    for ievt, evt in enumerate(tree):
        if (ievt == options.nentries):  break

        if not options.randomCharge:
            charge = evt.genParts_charge[0]
            if charge > 0:  continue

        for r, phi, z in izip(evt.TTStubs_r, evt.TTStubs_phi, evt.TTStubs_z):
            histos["xy"].Fill(r * cos(phi), r * sin(phi))
            histos["rz"].Fill(z, r)

    tree.SetBranchStatus("*", 1)
    return

def drawTrack(histos, view, tpolylines, tgraph_p, tgraph_m, options):
    if view == "XY":
        hname = "xy"
    elif view == "RZ":
        hname = "rz"

    h = histos[hname]

    i_pt = int(round(options.pt))

    hframe = h.Clone("hframe")
    hframe.Reset()
    hframe.SetLineColor(kWhite)
    hframe.SetStats(0); hframe.Draw()

    for l in tpolylines:
        l.Draw()
    if not options.randomCharge:
        tgraph_m.Draw("l")
    else:
        tgraph_p.Draw("l")
        tgraph_m.Draw("l")

    if view == "XY":
        for phi in [options.phimin, options.phimax]:
            if phi < 1.0:
                tline2.DrawLine(0, 0, xmax, xmax*tan(phi))
            else:
                tline2.DrawLine(0, 0, ymax/tan(phi), ymax)
    elif view == "RZ":
        for eta in [options.etamin, options.etamax]:
            theta = 2.0 * atan(exp(-eta))
            if theta < 1.0:
                tline2.DrawLine(0, 0, zmax, zmax*tan(theta))
            else:
                tline2.DrawLine(0, 0, ymax/tan(theta), ymax)

    moveLegend(0.58,0.80,0.94,0.92); tlegend.Clear()
    tlegend.AddEntry(hframe, "p_{T} = %i GeV" % i_pt, "l")
    tlegend.AddEntry(hframe, "#eta  = %.1f/3" % (options.eta*3), "l")
    tlegend.AddEntry(hframe, "#phi  = %i#pi/8" % (options.phi*8/pi), "l")
    tlegend.Draw()
    h.Draw("same")
    CMS_label()
    save(options.outdir, "%s_pt%i" % (hname, i_pt))

    donotdelete.append(hframe)
    donotdelete.append([tpolylines, tgraph_p, tgraph_m])
    return

def sitrepTrack(histos, options):
    print "--- SITREP ---------------------------------------------------------"
    print "--- Using tt{0}, pt={1}, eta={2}, phi={3}, vz={4}".format(options.tower, options.pt, options.eta, options.phi, options.vz)


# ______________________________________________________________________________
# Main function
def main(options):

    # Init
    drawerInit = DrawerInit()
    tchain = TChain("ntupler/tree", "")
    tchain.AddFileInfoList(options.tfilecoll.GetList())

    # Process
    histos = bookTrack()
    projectTrack(tchain, histos, options)
    tpolylines_xy = get_tpolylines("XY")
    tpolylines_rz = get_tpolylines("RZ")
    tgraphs = get_tgraphs(options.pt, options.eta, options.phi, options.vz)
    drawTrack(histos, "XY", tpolylines_xy, tgraphs[0], tgraphs[1], options)
    drawTrack(histos, "RZ", tpolylines_rz, tgraphs[2], tgraphs[2], options)
    sitrepTrack(histos,options)


# ______________________________________________________________________________
if __name__ == '__main__':

    # Setup argument parser
    parser = argparse.ArgumentParser()

    # Add default arguments
    add_drawer_arguments(parser)

    # Add more arguments
    parser.add_argument("--pt", type=float, default=3.0, help="generated track pT (default: %(default)s)")
    parser.add_argument("--eta", type=float, default=1.1/3, help="generated track eta (default: %(default)s)")
    parser.add_argument("--phi", type=float, default=pi*3/8, help="generated track phi (default: %(default)s)")
    parser.add_argument("--vz", type=float, default=0., help="generated track vz (default: %(default)s)")
    parser.add_argument("--randomCharge", action="store_true", help="use tracks of both charges (default: %(default)s)")

    # Parse default arguments
    options = parser.parse_args()
    parse_drawer_options(options)

    # Call the main function
    main(options)
