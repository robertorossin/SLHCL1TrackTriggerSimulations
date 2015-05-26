#!/usr/bin/env python

from rootdrawing import *
from parser import *
from math import asin, sinh

# Configurations
col  = TColor.GetColor("#1f78b4")  # mu0
fcol = TColor.GetColor("#a6cee3")  # mu0


# ______________________________________________________________________________
mPtFactor = 0.3*3.8*1e-2/2.0

def calcIdealPhi(phi, invPt, r):
    #return phi - mPtFactor * r * invPt
    return phi - asin(mPtFactor * r * invPt)

def calcIdealZ(eta, vz, invPt, r):
    cotTheta = sinh(eta)
    #return vz + r * cotTheta
    return vz + (1.0 / (mPtFactor * invPt) * asin(mPtFactor * r * invPt)) * cotTheta

# Calculate constants
rMeans = b_rcoord_cmssw  # cm
pitches = [100., 100., 100., 90., 90., 90.]  # um
thicknesses = [0.26, 0.16, 0.16, 0.18, 0.18, 0.18]  # cm
drCorrs = []
for i in xrange(6):
    drCorr = pitches[i] / thicknesses[i] / rMeans[i] * 1e-4
    drCorrs.append(drCorr)


# ______________________________________________________________________________
def drawer_book():
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
        histos[hname] = TH1F(hname, "; stub #delta #phi_{L%i} (corr)" % i, nbinsx, xmin, xmax)

        hname = "stub_dphi_four_%i" % i
        histos[hname] = TH1F(hname, "; stub #delta #phi_{L%i} (@stub r)" % i, nbinsx, xmin, xmax)

        hname = "stub_absdphi_%i" % i
        nbinsx, xmin, xmax = modify_binning(1000, -0.01, 0.01)
        histos[hname] = TH1F(hname, "; stub |#delta #phi|_{L%i}" % i, nbinsx, xmin, xmax)

        hname = "stub_absdphi_corr_%i" % i
        histos[hname] = TH1F(hname, "; stub |#delta #phi|_{L%i} (corr)" % i, nbinsx, xmin, xmax)

        hname = "stub_absdphi_four_%i" % i
        histos[hname] = TH1F(hname, "; stub |#delta #phi|_{L%i} (@stub r)" % i, nbinsx, xmin, xmax)

        hname = "stub_dz_%i" % i
        nbinsx, xmin, xmax = modify_binning(1000, -1., 1.) if i < 3 else modify_binning(1000, -10., 10.)
        histos[hname] = TH1F(hname, "; stub #delta z_{L%i}" % i, nbinsx, xmin, xmax)

        #hname = "stub_dz_corr_%i" % i
        #histos[hname] = TH1F(hname, "; stub #delta z_{L%i} (corr)" % i, nbinsx, xmin, xmax)

        hname = "stub_dz_four_%i" % i
        histos[hname] = TH1F(hname, "; stub #delta z_{L%i} (@stub r)" % i, nbinsx, xmin, xmax)

        hname = "stub_absdz_%i" % i
        nbinsx, xmin, xmax = modify_binning(1000, -1., 1.) if i < 3 else modify_binning(1000, -10., 10.)
        histos[hname] = TH1F(hname, "; stub |#delta z|_{L%i}" % i, nbinsx, xmin, xmax)

        #hname = "stub_absdz_corr_%i" % i
        #histos[hname] = TH1F(hname, "; stub |#delta z|_{L%i} (corr)" % i, nbinsx, xmin, xmax)

        hname = "stub_absdz_four_%i" % i
        histos[hname] = TH1F(hname, "; stub |#delta z|_{L%i} (@stub r)" % i, nbinsx, xmin, xmax)

    # TH2F
    hname = "stub_invPt_vs_ds_all"
    nbinsx, xmin, xmax = modify_binning(40, -10., 10.)
    nbinsy, ymin, ymax = 1000, -0.6, 0.6
    histos[hname] = TH2F(hname, "; stub #Delta s_{all}; signed 1/p_{T} [1/GeV]", nbinsx, xmin, xmax, nbinsy, ymin, ymax)

    for i in xrange(6):
        hname = "stub_invPt_vs_ds_%i" % i
        nbinsx, xmin, xmax = modify_binning(40, -10., 10.)
        nbinsy, ymin, ymax = 1000, -0.6, 0.6
        histos[hname] = TH2F(hname, "; stub #Delta s_{L%i}; signed 1/p_{T} [1/GeV]" % i, nbinsx, xmin, xmax, nbinsy, ymin, ymax)

    # Style
    for hname, h in histos.iteritems():
        if "vs_" not in hname:
            h.SetLineWidth(2); h.SetMarkerSize(0)
            h.SetLineColor(col); h.SetFillColor(fcol)
        else:
            h.SetMarkerStyle(1)
    donotdelete.append(histos)
    return histos

def drawer_project(tree, histos, options):
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
        gen_invPt  = float(gen_charge) / gen_pt

        for i in xrange(6):
            # Stub info
            stub_phi = evt.TTStubs_phi[i]
            stub_r   = evt.TTStubs_r[i]
            stub_z   = evt.TTStubs_z[i]
            stub_ds  = evt.TTStubs_trigBend[i]
            stub_dr  = rMeans[i] - stub_r

            histos["stub_ds_all"].Fill(stub_ds)
            histos["stub_ds_%i" % i].Fill(stub_ds)

            histos["stub_invPt_vs_ds_all"].Fill(stub_ds, gen_invPt)
            histos["stub_invPt_vs_ds_%i" % i].Fill(stub_ds, gen_invPt)

            ideal_phi_at_stub_r = calcIdealPhi(gen_phi, gen_invPt, stub_r)
            ideal_phi_at_mean_r = calcIdealPhi(gen_phi, gen_invPt, rMeans[i])
            stub_phi_corr = stub_phi - drCorrs[i] * stub_ds * stub_dr

            histos["stub_dphi_%i" % i].Fill(stub_phi - ideal_phi_at_mean_r)
            histos["stub_dphi_corr_%i" % i].Fill(stub_phi_corr - ideal_phi_at_mean_r)
            histos["stub_dphi_four_%i" % i].Fill(stub_phi - ideal_phi_at_stub_r)

            histos["stub_absdphi_%i" % i].Fill(abs(stub_phi - ideal_phi_at_mean_r))
            histos["stub_absdphi_corr_%i" % i].Fill(abs(stub_phi_corr - ideal_phi_at_mean_r))
            histos["stub_absdphi_four_%i" % i].Fill(abs(stub_phi - ideal_phi_at_stub_r))

            ideal_z_at_stub_r = calcIdealZ(gen_eta, gen_vz, gen_invPt, stub_r)
            ideal_z_at_mean_r = calcIdealZ(gen_eta, gen_vz, gen_invPt, rMeans[i])
            stub_z_corr = 0

            histos["stub_dz_%i" % i].Fill(stub_z - ideal_z_at_mean_r)
            #histos["stub_dz_corr_%i" % i].Fill(stub_z_corr - ideal_z_at_mean_r)
            histos["stub_dz_four_%i" % i].Fill(stub_z - ideal_z_at_stub_r)

            histos["stub_absdz_%i" % i].Fill(abs(stub_z - ideal_z_at_mean_r))
            #histos["stub_absdz_corr_%i" % i].Fill(abs(stub_z_corr - ideal_z_at_mean_r))
            histos["stub_absdz_four_%i" % i].Fill(abs(stub_z - ideal_z_at_stub_r))


    tree.SetBranchStatus("*", 1)
    return

def drawer_draw(histos, options):
    for hname, h in histos.iteritems():
        if "vs_" not in hname:
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

def drawer_sitrep(histos, options):
    # Get one-sided confidence interval
    def getCI(h, threshold=0.95):
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

    values1, values2 = [], []
    for i in xrange(6):
        h = histos["stub_dphi_%i" % i]
        values1.append(h.GetRMS())
        h = histos["stub_absdphi_%i" % i]
        values2.append(getCI(h))
    print "dphi RMS width             :", [round(x,6) for x in values1]
    print "absdphi 95% CI width       :", [round(x,6) for x in values2]

    values1, values2 = [], []
    for i in xrange(6):
        h = histos["stub_dphi_corr_%i" % i]
        values1.append(h.GetRMS())
        h = histos["stub_absdphi_corr_%i" % i]
        values2.append(getCI(h))
    print "dphi RMS width       (corr):", [round(x,6) for x in values1]
    print "absdphi 95% CI width (corr):", [round(x,6) for x in values2]

    values1, values2 = [], []
    for i in xrange(6):
        h = histos["stub_dphi_four_%i" % i]
        values1.append(h.GetRMS())
        h = histos["stub_absdphi_four_%i" % i]
        values2.append(getCI(h))
    print "dphi RMS width       (four):", [round(x,6) for x in values1]
    print "absdphi 95% CI width (four):", [round(x,6) for x in values2]

    values1, values2 = [], []
    for i in xrange(6):
        h = histos["stub_dz_%i" % i]
        values1.append(h.GetRMS())
        h = histos["stub_absdz_%i" % i]
        values2.append(getCI(h))
    print "dz RMS width             :", [round(x,6) for x in values1]
    print "absdz 95% CI width       :", [round(x,6) for x in values2]

    #values1, values2 = [], []
    #for i in xrange(6):
    #    h = histos["stub_dz_corr_%i" % i]
    #    values1.append(h.GetRMS())
    #    h = histos["stub_absdz_corr_%i" % i]
    #    values2.append(getCI(h))
    #print "dz RMS width       (corr):", [round(x,6) for x in values1]
    #print "absdz 95% CI width (corr):", [round(x,6) for x in values2]

    values1, values2 = [], []
    for i in xrange(6):
        h = histos["stub_dz_four_%i" % i]
        values1.append(h.GetRMS())
        h = histos["stub_absdz_four_%i" % i]
        values2.append(getCI(h))
    print "dz RMS width       (four):", [round(x,6) for x in values1]
    print "absdz 95% CI width (four):", [round(x,6) for x in values2]


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
