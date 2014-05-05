import unittest
import sys
from ROOT import TFile, TTree, gROOT, vector


class TestAMSim(unittest.TestCase):
    """Test the pattern bank generated by PatternGenerator"""

    infile = "patternBank.root"

    def setUp(self):
        gROOT.SetBatch(True)
        self.tfile = TFile.Open(self.infile)
        self.ttree = self.tfile.Get("patternBank")
        self.nevents = self.ttree.GetEntries()
        self.tfile_tmp = TFile.Open(self.infile.replace(".root","_tmp.root"))
        self.ttree_tmp = self.tfile_tmp.Get("ntupler/tree")
        self.nevents_tmp = self.ttree_tmp.GetEntries()

    def tearDown(self):
        pass

    def test_nevents(self):
        self.assertEqual(self.nevents, 99)  # 1 event has zero stub
        self.assertEqual(self.nevents_tmp, 100)

    def test_nstubs(self):
        tree = self.ttree_tmp
        for ievt in tree:
            # Check for layer uniqueness
            layers = []
            for modId in ievt.TTStubs_modId:
                layer = modId/10000
                self.assertTrue(layer not in layers)
                layers.append(layer)

    def test_nhits(self):
        tree = self.ttree_tmp
        for ievt in tree:
            # Check for hit singleness
            for hit in ievt.TTStubs_hitCols:
                self.assertEqual(hit.size(), 1)
            for hit in ievt.TTStubs_hitRows:
                self.assertEqual(hit.size(), 1)


if __name__ == "__main__":
    if len(sys.argv) > 1:
        TestAMSim.infile = sys.argv.pop()

    unittest.main()