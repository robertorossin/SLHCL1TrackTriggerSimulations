import unittest
import sys
from ROOT import TFile, TTree, gROOT, gSystem, vector


class TestAMSim(unittest.TestCase):
    """Test the pattern recognition results using PatternMatcher"""

    infile = "roads.root"

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

    def test_nroads(self):
        tree = self.ttree
        n = 0
        for ievt in tree:
            for iroad in ievt.AMTTRoads_hitXs:
                n += 1
            self.assertTrue(ievt.AMTTRoads_hitXs.size() == 0 or ievt.AMTTRoads_hitXs.size() == 1)
        self.assertEqual(n, 62)

    def test_nhits(self):
        tree = self.ttree
        n = 0
        for ievt in tree:
            for iroad in ievt.AMTTRoads_hitXs:
                for ihit in iroad:
                    n += 1
                self.assertTrue(iroad.size() == 0 or iroad.size() >= 6)
        self.assertEqual(n, 372)

    def test_charge(self):
        tree = self.ttree
        for ievt in tree:
            for iroad in ievt.AMTTRoads_hitCharges:
                for ihit in iroad:
                    self.assertEqual(ihit, -1)

    def test_patternId(self):
        tree = self.ttree
        for ievt in tree:
            for iroad in ievt.AMTTRoads_patternIds:
                for ihit in iroad[:6]:
                    self.assertNotEqual(ihit, 0)


if __name__ == "__main__":
    if len(sys.argv) > 1:
        TestAMSim.infile = sys.argv.pop()

    unittest.main()
