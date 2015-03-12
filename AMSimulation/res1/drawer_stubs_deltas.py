#!/usr/bin/env python

from rootdrawing import *
from parser import *
from helper import *
from math import asin

# Configurations
col  = TColor.GetColor("#1f78b4")  # mu0
fcol = TColor.GetColor("#a6cee3")

donotdelete = []  # persist in memory


# ______________________________________________________________________________
def calcIdealPhi(phi, qoverpt, r):
    const = 0.3*3.8*1e-2/2.0;
    #return phi - const * r * qoverpt
    return phi - asin(const * r * qoverpt)

# Calculate constants
rMeans = b_rcoord_cmssw  # cm
pitches = [100., 100., 100., 90., 90., 90.]  # um
thicknesses = [0.26, 0.16, 0.16, 0.18, 0.18, 0.18]  # cm
drCorrs = []
for i in xrange(6):
    drCorr = pitches[i] / thicknesses[i] / rMeans[i] * 1e-4
    drCorrs.append(drCorr)


# ______________________________________________________________________________
def bookStubs():
    histos = {}

    # TH1F
    hname = "stub_ds_all"
    nbinsx, xmin, xmax = modify_binning(40, -10., 10.)
    histos[hname] = TH1F(hname, "; stub #Delta s_{all}", nbinsx, xmin, xmax)

    for i in xrange(6):
        hname = "stub_ds_%i" % i
        nbinsx, xmin, xmax = modify_binning(40, -10., 10.)
        histos[hname] = TH1F(hname, "; stub #Delta s_{L%i}" % i, nbinsx, xmin, xmax)

        hname = "stub_dphi_%i" % i
        nbinsx, xmin, xmax = modify_binning(1000, -0.01, 0.01)
        histos[hname] = TH1F(hname, "; stub #delta #phi_{L%i}" % i, nbinsx, xmin, xmax)

        hname = "stub_dphi_corr_%i" % i
        nbinsx, xmin, xmax = modify_binning(1000, -0.01, 0.01)
        histos[hname] = TH1F(hname, "; stub #delta #phi_{L%i} (corr)" % i, nbinsx, xmin, xmax)

        hname = "stub_dphi_4r_%i" % i
        nbinsx, xmin, xmax = modify_binning(1000, -0.01, 0.01)
        histos[hname] = TH1F(hname, "; stub #delta #phi_{L%i} (@stub r)" % i, nbinsx, xmin, xmax)

        hname = "stub_absdphi_%i" % i
        nbinsx, xmin, xmax = modify_binning(1000, -0.01, 0.01)
        histos[hname] = TH1F(hname, "; stub |#delta #phi|_{L%i}" % i, nbinsx, xmin, xmax)

        hname = "stub_absdphi_corr_%i" % i
        nbinsx, xmin, xmax = modify_binning(1000, -0.01, 0.01)
        histos[hname] = TH1F(hname, "; stub |#delta #phi|_{L%i} (corr)" % i, nbinsx, xmin, xmax)

        hname = "stub_absdphi_4r_%i" % i
        nbinsx, xmin, xmax = modify_binning(1000, -0.01, 0.01)
        histos[hname] = TH1F(hname, "; stub |#delta #phi|_{L%i} (@stub r)" % i, nbinsx, xmin, xmax)

    # TH2F
    hname = "stub_qoverpt_vs_ds_all"
    nbinsx, xmin, xmax = modify_binning(40, -10., 10.)
    nbinsy, ymin, ymax = 1000, -0.6, 0.6
    histos[hname] = TH2F(hname, "; stub #Delta s_{all}; signed 1/p_{T} [1/GeV]", nbinsx, xmin, xmax, nbinsy, ymin, ymax)

    for i in xrange(6):
        hname = "stub_qoverpt_vs_ds_%i" % i
        nbinsx, xmin, xmax = modify_binning(40, -10., 10.)
        nbinsy, ymin, ymax = 1000, -0.6, 0.6
        histos[hname] = TH2F(hname, "; stub #Delta s_{L%i}; signed 1/p_{T} [1/GeV]" % i, nbinsx, xmin, xmax, nbinsy, ymin, ymax)

    # Style
    for hname, h in histos.iteritems():
        if "vs" not in hname:
            h.SetLineWidth(2); h.SetMarkerSize(0)
            h.SetLineColor(col); h.SetFillColor(fcol)
        else:
            h.SetMarkerStyle(1)
    donotdelete.append(histos)
    return histos

def projectStubs(tree, histos, options):
    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("genParts_pt"           , 1)
    tree.SetBranchStatus("genParts_eta"          , 1)
    tree.SetBranchStatus("genParts_phi"          , 1)
    tree.SetBranchStatus("genParts_vz"           , 1)
    tree.SetBranchStatus("genParts_charge"       , 1)
    tree.SetBranchStatus("TTStubs_modId"         , 1)
    tree.SetBranchStatus("TTStubs_r"             , 1)
    tree.SetBranchStatus("TTStubs_phi"           , 1)
    tree.SetBranchStatus("TTStubs_z"             , 1)
    tree.SetBranchStatus("TTStubs_trigBend"      , 1)

    # Loop over events
    for ievt, evt in enumerate(tree):
        if (ievt == options.nentries):  break

        # Only tracks with 6 stubs
        if evt.TTStubs_modId.size() != 6:
            continue

        # All 6 stubs in the barrel
        layers = []
        for moduleId in evt.TTStubs_modId:
            lay = decodeLayer(moduleId)
            layers.append(lay)
        if layers != [5,6,7,8,9,10]:
            continue

        # Track info
        gen_pt     = evt.genParts_pt[0]
        gen_eta    = evt.genParts_eta[0]
        gen_phi    = evt.genParts_phi[0]
        gen_vz     = evt.genParts_vz[0]
        gen_charge = evt.genParts_charge[0]
        gen_qoverpt = float(gen_charge) / gen_pt

        for i in xrange(6):
            # Stub info
            stub_phi = evt.TTStubs_phi[i]
            stub_r   = evt.TTStubs_r[i]
            stub_z   = evt.TTStubs_z[i]
            stub_ds  = evt.TTStubs_trigBend[i]
            stub_dr  = stub_r - rMeans[i]

            histos["stub_ds_all"].Fill(stub_ds)
            histos["stub_ds_%i" % i].Fill(stub_ds)

            histos["stub_qoverpt_vs_ds_all"].Fill(stub_ds, gen_qoverpt)
            histos["stub_qoverpt_vs_ds_%i" % i].Fill(stub_ds, gen_qoverpt)

            ideal_phi_at_stub_r = calcIdealPhi(gen_phi, gen_qoverpt, stub_r)
            ideal_phi_at_mean_r = calcIdealPhi(gen_phi, gen_qoverpt, rMeans[i])
            stub_phi_corr = stub_phi + drCorrs[i] * stub_ds * stub_dr

            histos["stub_dphi_%i" % i].Fill(stub_phi - ideal_phi_at_mean_r)
            histos["stub_dphi_corr_%i" % i].Fill(stub_phi_corr - ideal_phi_at_mean_r)
            histos["stub_dphi_4r_%i" % i].Fill(stub_phi - ideal_phi_at_stub_r)

            histos["stub_absdphi_%i" % i].Fill(abs(stub_phi - ideal_phi_at_mean_r))
            histos["stub_absdphi_corr_%i" % i].Fill(abs(stub_phi_corr - ideal_phi_at_mean_r))
            histos["stub_absdphi_4r_%i" % i].Fill(abs(stub_phi - ideal_phi_at_stub_r))

    tree.SetBranchStatus("*", 1)
    return

def drawStubs(histos, options):
    for hname, h in histos.iteritems():
        if "vs" not in hname:
            # TH1F
            h.SetMaximum(h.GetMaximum() * 1.4); h.SetMinimum(0)
            h.SetStats(1); h.Draw("hist")
            CMS_label()
            save(options.outdir, hname, dot_root=True)
        else:
            # TH2F
            h.SetStats(0); h.Draw()
            CMS_label()
            save(options.outdir, hname, dot_root=True, dot_pdf=False)

def sitrepStubs(histos, options):
    # Get one-sided confidence interval
    def getCI(h, threshold=0.9):
        nbinsx = h.GetNbinsX()
        integral = h.Integral(1,nbinsx)
        integral_i = 0.
        x = h.GetBinCenter(1)
        for i in xrange(1,nbinsx+1):
            integral_i += h.GetBinContent(i)
            if (integral_i / integral) <= threshold:
                x = h.GetBinCenter(i)
        assert(integral_i == integral)
        return x

    print "--- SITREP ---------------------------------------------------------"
    print "--- Using rMeans : ", [round(x,2) for x in rMeans]
    print "--- Using drCorrs: ", [round(x,6) for x in drCorrs]

    values1 = []
    values2 = []
    for i in xrange(6):
        h = histos["stub_absdphi_%i" % i]
        values1.append(h.GetRMS())
        values2.append(getCI(h))
    print "dphi RMS width      :", [round(x,6) for x in values1]
    print "absdphi 90% CI width:", [round(x,6) for x in values2]

    values1 = []
    values2 = []
    for i in xrange(6):
        h = histos["stub_absdphi_corr_%i" % i]
        values1.append(h.GetRMS())
        values2.append(getCI(h))
    print "dphi RMS width       (corr):", [round(x,6) for x in values1]
    print "absdphi 90% CI width (corr):", [round(x,6) for x in values2]


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
