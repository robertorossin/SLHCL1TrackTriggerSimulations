#!/usr/bin/env python

from rootdrawing import *
import json

# ______________________________________________________________________________
# Global

latex.SetTextSize(0.06)

# ______________________________________________________________________________
# Configurations

sections = {}
sections["etaphi"               ] = True

drawerInit = DrawerInit()

chain = TChain("ntupler/tree", "")
chain.Add("stubs_40M.1.root")
tree = chain

triggerTowerMap = json.load(open("../data/trigger_sector_map.json"))
for i in xrange(6*8):
    print "Tower %i has %i modules." % (i, len(triggerTowerMap[str(i)]))

imgdir = "figures_triggertower/"
if not imgdir.endswith("/"):  imgdir += "/"
if gSystem.AccessPathName(imgdir):
    gSystem.mkdir(imgdir)


# ______________________________________________________________________________
class TriggerTower:
    def __init__(self, tt):
        self.minEta = 999.
        self.maxEta = -999.
        self.minPhi = 999.
        self.maxPhi = -999.
        self.h_eta = TH1F("tt_eta_%i" % tt, "; #eta", 50, -2.5, 2.5)
        self.h_phi = TH1F("tt_phi_%i" % tt, "; #phi", 64, -3.2, 3.2)

    def fill(self, eta, phi):
        if self.minEta > eta:
            self.minEta = eta
        if self.maxEta < eta:
            self.maxEta = eta
        if self.minPhi > phi:
            self.minPhi = phi
        if self.maxPhi < phi:
            self.maxPhi = phi
        self.h_eta.Fill(eta)
        self.h_phi.Fill(phi)

# ______________________________________________________________________________
def getReverseMap(directMap):
    reverseMap = {}
    for i in xrange(6*8):
        for m in directMap[str(i)]:
            if m in reverseMap:
                reverseMap[m].append(i)
            else:
                reverseMap[m] = [i]
    return reverseMap

triggerTowerReverseMap = getReverseMap(triggerTowerMap)


# ______________________________________________________________________________
# Eta/Phi

if sections["etaphi"]:

    triggerTowers = {}
    for i in xrange(6*8):
        triggerTowers[i] = TriggerTower(i)
    triggerTowers[99] = TriggerTower(99)

    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("TTStubs_modId"      , 1)
    tree.SetBranchStatus("TTStubs_simPt"      , 1)
    tree.SetBranchStatus("TTStubs_simEta"     , 1)
    tree.SetBranchStatus("TTStubs_simPhi"     , 1)

    i = 0
    maxEvents = 1000000
    for ievt in tree:
        if i > maxEvents:  break

        # Loop over stubs
        countMap = {}
        for moduleId, pt, eta, phi in zip(ievt.TTStubs_modId, ievt.TTStubs_simPt, ievt.TTStubs_simEta, ievt.TTStubs_simPhi):
            if moduleId in triggerTowerReverseMap:
                for tt in triggerTowerReverseMap[moduleId]:
                    if tt in countMap:
                        countMap[tt] += 1
                    else:
                        countMap[tt] = 1
            else:
                tt = 99
                if tt in countMap:
                    countMap[tt] += 1
                else:
                    countMap[tt] = 1

        if countMap:
            counts = sorted(countMap.items(), key=lambda x: x[1], reverse=True)
            if counts[0][1] >= 4:
                t = triggerTowers[counts[0][0]]
                t.fill(eta, phi)

        i += 1

    # Print
    for tt, triggerTower in triggerTowers.iteritems():
        triggerTower.h_eta.Scale(1.0 / triggerTower.h_eta.Integral())
        triggerTower.h_phi.Scale(1.0 / triggerTower.h_phi.Integral())

        print "tt %2i: % 5.3f % 5.3f  % 5.3f  % 5.3f" % (tt, triggerTower.minEta, triggerTower.maxEta, triggerTower.minPhi, triggerTower.maxPhi)

    # As a function of eta
    for iphi in xrange(8):
        imgname = "eta_iphi%i" % iphi
        imgcanvas = TH1F(imgname, "; #eta", 50, -2.5, 2.5)
        imgcanvas.SetStats(0)
        imgcanvas.SetMaximum(1e0)
        imgcanvas.SetMinimum(100./8 * maxEvents)
        imgcanvas.Draw()
        gPad.SetLogy()

        for ieta in xrange(6):
            tt = ieta*8 + iphi
            h = triggerTowers[tt].h_eta

            h.SetLineWidth(2)
            h.SetLineColor(palette[ieta])
            h.Draw("hist same")

        latex.SetTextSize(0.05)
        latex.DrawLatex(0.2, 0.86, "iphi = %i" % iphi)
        CMS_label()
        save(imgdir, imgname)

    # As a function of phi
    for ieta in xrange(6):
        imgname = "phi_ieta%i" % ieta
        imgcanvas = TH1F(imgname, "; #phi", 64, -3.2, 3.2)
        imgcanvas.SetStats(0)
        imgcanvas.SetMaximum(1e0)
        imgcanvas.SetMinimum(100./6 * maxEvents)
        imgcanvas.Draw()
        gPad.SetLogy()

        for iphi in xrange(8):
            tt = ieta*8 + iphi
            h = triggerTowers[tt].h_phi

            h.SetLineWidth(2)
            h.SetLineColor(palette[iphi])
            h.Draw("hist same")

        latex.SetTextSize(0.05)
        latex.DrawLatex(0.2, 0.86, "ieta = %i" % ieta)
        CMS_label()
        save(imgdir, imgname)


# Use these?
eta_bins = [
    (-2.50,-1.38), (-1.54,-0.54), (-0.93,0.30),
    (-0.30,0.93), (0.54,1.54), (1.38,2.50)
    ]

phi_bins = [
    (-1.8,-0.8), (-1.0,0.0), (-0.2,0.8), (0.6,1.6),
    (1.4,2.4), (2.2,3.2), (3.0,-2.4), (-2.6,-1.6)
    ]
