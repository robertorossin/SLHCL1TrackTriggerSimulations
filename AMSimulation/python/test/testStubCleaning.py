import unittest
import sys
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
        tree = self.ttree
        for ievt in tree:
            for modId in ievt.TTStubs_modId:
                layer = modId/10000
                self.assertTrue(layer in [5,6,7,8,9,10,11,12,13,14,15,18,19,20,21,22])

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
            self.assertTrue(nstubs <= 6)

    def test_count(self):
        tree = self.ttree
        countA = 0
        count0 = 0
        count1 = 0
        count2 = 0
        count3 = 0
        count4 = 0
        count5 = 0
        count6 = 0
        for ievt in tree:
            for modId in ievt.TTStubs_modId:
                countA += 1
            nstubs = ievt.TTStubs_modId.size()
            if nstubs == 0:
                count0 += 1
            elif nstubs == 1:
                count1 += 1
            elif nstubs == 2:
                count2 += 1
            elif nstubs == 3:
                count3 += 1
            elif nstubs == 4:
                count4 += 1
            elif nstubs == 5:
                count5 += 1
            elif nstubs == 6:
                count6 += 1
        self.assertEqual(countA, 571)
        self.assertEqual(count0, 0)
        self.assertEqual(count1, 0)
        self.assertEqual(count2, 0)
        self.assertEqual(count3, 0)
        self.assertEqual(count4, 6)
        self.assertEqual(count5, 17)
        self.assertEqual(count6, 77)
        self.assertEqual(count0+count1+count2+count3+count4+count5+count6, 100)
        self.assertEqual(1*count1+2*count2+3*count3+4*count4+5*count5+6*count6, countA)


if __name__ == "__main__":
    if len(sys.argv) > 1:
        TestAMSim.infile = sys.argv.pop()

    unittest.main()
