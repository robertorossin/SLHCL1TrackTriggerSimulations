#!/usr/bin/env python

from rootdrawing import *
from roothelper import *

# ______________________________________________________________________________
# Configurations

sections = {}
sections["residual"] = False
sections["interval"] = True


drawerInit = DrawerInit()

infiles = ["/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1_ntuple/stubs_barrel_400M/stubs_barrel_400M.1.root"]
#infiles = ["/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1_ntuple/stubs_barrel_400M/stubs_barrel_400M.2.root"]

col  = TColor.GetColor("#1f78b4")  # Paired
fcol = TColor.GetColor("#a6cee3")
imgdir = "figures_stubs/"


# Number of events
nentries = 1000000
#nentries = 10000

chain = TChain("ntupler/tree", "")
for f in infiles:
    chain.Add(f)
tree = chain

if not imgdir.endswith("/"):  imgdir += "/"
if gSystem.AccessPathName(imgdir):
    gSystem.mkdir(imgdir)


# ______________________________________________________________________________
# Load

ttmap = json.load(open("../data/trigger_sector_map.json"), object_pairs_hook=convert_key_to_int)
ttrmap = get_reverse_map(ttmap)


# ______________________________________________________________________________
if sections["residual"]:

    def doit(x, imgdir=None, logy=False):
        histos = book(x)
        project(tree, histos, nentries=nentries, drawOverflow=False, drawUnderflow=False)
        draw(histos, logy=logy)
        save(imgdir, x[0].name)
        return histos

    # Exactly 6 stubs
    cut = "@TTStubs_r.size()==6"

    # Barrel
    phi_xmaxs = [0.006, 0.008, 0.010, 0.014, 0.018, 0.024]
    z_xmaxs = [0.5,0.5,0.5,5,5,5]
    for i in xrange(5,11):
        layer = "b%i" % i
        ii = i-5

        var = "TTStubs_phi[%i] - (genParts_phi[0] - 0.3*3.8*TTStubs_r[%i]*1e-2 * (genParts_charge[0]/genParts_pt[0]) / 2.0)" % (ii,ii)
        xtitle = "Stub #phi_{%s} - Track #phi_{%s,exp}" % (layer,layer)
        p0 = struct("phiresidual_%s" % layer, var, cut, xtitle, 200, -phi_xmaxs[ii], phi_xmaxs[ii], col, fcol)
        histos = doit([p0], imgdir)
        print histos[0].h.GetRMS()

        var = "TTStubs_z[%i] - (genParts_vz[0] + TTStubs_r[%i] * sinh(genParts_eta[0]))" % (ii,ii)  # cot(theta) = sinh(eta) = pz/pt
        xtitle = "Stub z_{%s} - Track z_{%s,exp} [cm]" % (layer,layer)
        p0 = struct("zresidual_%s" % layer, var, cut, xtitle, 200, -z_xmaxs[ii], z_xmaxs[ii], col, fcol)
        histos = doit([p0], imgdir)
        print histos[0].h.GetRMS()


# ______________________________________________________________________________
if sections["interval"]:

    def bookLayer():
        histos = {}
        for i in xrange(5,23):
            if i == 16 or i == 17:  continue
            if i < 11:
                layer = "b%i" % i
            else:
                layer = "e%i" % i
            histos["phiinterval_%s" % layer] = TH1F("phiinterval_%s" % layer, "; Stub #phi_{%s}; Entries" % layer, 640*2, -3.2, 3.2)
            histos["etainterval_%s" % layer] = TH1F("etainterval_%s" % layer, "; Stub #eta_{%s}; Entries" % layer, 600*2, -3.0, 3.0)
            histos["zinterval_%s" % layer] = TH1F("zinterval_%s" % layer, "; Stub z_{%s} [cm]; Entries" % layer, 600*2, -300.0, 300.0)
        return histos

    def projectLayer(tree, histos, tt, nentries=1000000000):
        tree.SetBranchStatus("*", 0)
        tree.SetBranchStatus("TTStubs_phi"    , 1)
        tree.SetBranchStatus("TTStubs_eta"    , 1)
        tree.SetBranchStatus("TTStubs_z"      , 1)
        tree.SetBranchStatus("TTStubs_modId"  , 1)

        ievt = tree
        for i_ievt in xrange(nentries):
            if i_ievt < nentries/2:
                tree.GetEntry(i_ievt)
            else:
                tree.GetEntry(200000000+i_ievt)

            for phi, eta, z, moduleId in izip(ievt.TTStubs_phi, ievt.TTStubs_eta, ievt.TTStubs_z, ievt.TTStubs_modId):
                if moduleId in ttmap[tt]:
                    lay = decodeLayer(moduleId)
                    if lay < 11:
                        layer = "b%i" % lay
                    else:
                        layer = "e%i" % lay
                    histos["phiinterval_%s" % layer].Fill(phi)
                    histos["etainterval_%s" % layer].Fill(eta)
                    histos["zinterval_%s" % layer].Fill(z)

        tree.SetBranchStatus("*", 1)
        return

    def find_xinterval(h):
        nbins = h.GetNbinsX()
        xmin, xmax = 0, nbins+2
        for i in xrange(nbins+2):
            if h.GetBinContent(i) > 0:
                xmin = h.GetBinLowEdge(i)
                break
        for i in xrange(nbins+2):
            if h.GetBinContent(nbins-1-i) > 0:
                xmax = h.GetBinLowEdge(nbins-i)
                break
        return xmin, xmax

    def drawLayer(histos, imgdir, tt):

        for i in xrange(5,23):
            if i == 16 or i == 17:  continue
            if i < 11:
                layer = "b%i" % i
            else:
                layer = "e%i" % i

            # phi interval
            h = histos["phiinterval_%s" % layer]
            h.SetStats(0); h.SetLineWidth(2); h.SetLineColor(col); h.SetFillColor(fcol)
            if h.GetEntries() == 0:  continue
            xmin, xmax = find_xinterval(h)
            h.Rebin(20); h.SetMaximum(h.GetMaximum()*1.3)

            h.Draw("hist")
            moveLegend(0.56,0.88,0.94,0.94); legend.Clear()
            legend.AddEntry(h, "%s #phi=[%.3f,%.3f]" % (layer,xmin,xmax), "f")
            legend.Draw()
            CMS_label()
            save(imgdir, "phiinterval_tt%i_%s" % (tt,layer))

            # eta interval
            h = histos["etainterval_%s" % layer]
            h.SetStats(0); h.SetLineWidth(2); h.SetLineColor(col); h.SetFillColor(fcol)
            if h.GetEntries() == 0:  continue
            xmin, xmax = find_xinterval(h)
            h.Rebin(20); h.SetMaximum(h.GetMaximum()*1.3)

            h.Draw("hist")
            moveLegend(0.56,0.88,0.94,0.94); legend.Clear()
            legend.AddEntry(h, "%s #eta=[%.3f,%.3f]" % (layer,xmin,xmax), "f")
            legend.Draw()
            CMS_label()
            save(imgdir, "etainterval_tt%i_%s" % (tt,layer))

            # z interval
            h = histos["zinterval_%s" % layer]
            h.SetStats(0); h.SetLineWidth(2); h.SetLineColor(col); h.SetFillColor(fcol)
            if h.GetEntries() == 0:  continue
            xmin, xmax = find_xinterval(h)
            h.Rebin(20); h.SetMaximum(h.GetMaximum()*1.3)

            h.Draw("hist")
            moveLegend(0.56,0.88,0.94,0.94); legend.Clear()
            legend.AddEntry(h, "%s z=[%.3f,%.3f]" % (layer,xmin,xmax), "f")
            legend.Draw()
            CMS_label()
            save(imgdir, "zinterval_tt%i_%s" % (tt,layer))

        donotdelete = []
        return donotdelete


    for tt in [27]:
        histos = bookLayer()
        projectLayer(tree, histos, tt, nentries=nentries)
        d = drawLayer(histos, imgdir, tt)
