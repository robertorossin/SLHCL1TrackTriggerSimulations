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
            self.assertTrue(ievt.AMTTRoads_hitXs.size() in [0, 1, 7, 28])
        self.assertEqual(n, 63 + 15 * 7 + 0 * 28)

    def test_nhits(self):
        tree = self.ttree
        n = 0
        for ievt in tree:
            for iroad in ievt.AMTTRoads_hitXs:
                for ihit in iroad:
                    n += 1
                self.assertTrue(iroad.size() in [0, 6, 7])
        self.assertEqual(n, 1031)

    def test_nHitLayers(self):
        tree = self.ttree
        for ievt in tree:
            for iroad in ievt.AMTTRoads_nHitLayers:
                self.assertEqual(ord(iroad), 6)

    def test_bankIndex(self):
        tree = self.ttree
        bankIndices = {}
        for ievt in tree:
            for iroad in ievt.AMTTRoads_bankIndex:
                self.assertTrue(iroad not in bankIndices)
                bankIndices[iroad] = 0

    def test_patternId(self):
        tree = self.ttree
        for ievt in tree:
            for iroad in ievt.AMTTRoads_patternIds:
                for ihit in iroad[:6]:
                    self.assertNotEqual(ihit, 0)

    def test_charges(self):
        tree = self.ttree
        for ievt in tree:
            for iroad in ievt.AMTTRoads_hitCharges:
                for ihit in iroad:
                    self.assertEqual(ihit, -1)

    #def test_superstripIds(self):
    #    tree = self.ttree
    #    for ievt in tree:
    #        ssIds = []
    #        for iroad in ievt.AMTTRoads_patternIds:
    #            for ihit in iroad[:6]:
    #                ssIds.append(ihit)

    #        for iroad in ievt.AMTTRoads_hitSuperstripIds:
    #            for ihit in iroad:
    #                self.assertTrue(ihit in ssIds)


if __name__ == "__main__":
    if len(sys.argv) > 1:
        TestAMSim.infile = sys.argv.pop()

    unittest.main()
