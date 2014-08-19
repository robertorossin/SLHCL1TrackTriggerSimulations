import unittest
import sys
from itertools import izip
from math import pi
from ROOT import TFile, TTree, gROOT, gSystem, vector


class TestAMSim(unittest.TestCase):
    """Test the stubs after cleaning by StubCleaner"""

    infile = "stubs.root"

    def setUp(self):
        gROOT.SetBatch(True)
        gROOT.SetMacroPath(gSystem.Getenv("CMSSW_BASE")+"/src/SLHCL1TrackTriggerSimulations/AMSimulation")
        gROOT.LoadMacro("python/test/loader.h+")
        self.tfile = TFile.Open(self.infile)
        self.ttree = self.tfile.Get("ntupler/tree")
        self.nevents = self.ttree.GetEntries()

    def tearDown(self):
        pass

    def test_nevents(self):
        self.assertEqual(self.nevents, 100)

    def test_moduleId(self):
        allowed_layers = [5,6,7,8,9,10,11,12,13,14,15,18,19,20,21,22]
        tree = self.ttree
        for ievt in tree:
            for modId in ievt.TTStubs_modId:
                lay = modId/10000
                self.assertTrue(lay in allowed_layers)

    def test_trkId(self):
        tree = self.ttree
        for ievt in tree:
            for trkId, simPt, simEta, simPhi in izip(ievt.TTStubs_trkId, ievt.TTStubs_simPt, ievt.TTStubs_simEta, ievt.TTStubs_simPhi):
                self.assertTrue(trkId == 1 or trkId == -1)
                if trkId == 1:
                    self.assertTrue(2.0 <= simPt and simPt <= 999999.0)
                    self.assertTrue(-2.5 <= simEta and simEta <= 2.5)
                    self.assertTrue(-pi <= simPhi and simPhi <= pi)

    def test_nhits(self):
        tree = self.ttree
        for ievt in tree:
            for nhits in ievt.TTStubs_nhits:
                self.assertTrue(nhits > 0)

    def test_nstubs(self):
        tree = self.ttree
        for ievt in tree:
            # Check for layer uniqueness
            layers = []
            for modId in ievt.TTStubs_modId:
                layer = modId/10000
                self.assertTrue(layer not in layers)
                layers.append(layer)

            for trkId in ievt.TTStubs_trkId:
                self.assertEqual(trkId, 1)

            nstubs = ievt.TTStubs_modId.size()
            self.assertTrue(nstubs <= 8)

    def test_count(self):
        tree = self.ttree
        countA = 0
        countB = 0
        countsA = [0] * 9
        for ievt in tree:
            for modId, trkId in izip(ievt.TTStubs_modId, ievt.TTStubs_trkId):
                countA += 1
                if trkId == 1:
                    countB += 1
            nstubs = ievt.TTStubs_modId.size()
            countsA[nstubs] += 1
        self.assertEqual(countA, 583)
        self.assertEqual(countB, 583)
        self.assertEqual(countsA, [1,0,0,0,5,16,63,15,0])
        self.assertEqual(sum(countsA), 100)
        self.assertEqual(sum([i * cnt for i,cnt in enumerate(countsA)]), countA)


if __name__ == "__main__":
    if len(sys.argv) > 1:
        TestAMSim.infile = sys.argv.pop()

    unittest.main()
