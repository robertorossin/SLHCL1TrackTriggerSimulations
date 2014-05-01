import unittest
import sys
from ROOT import TFile, TTree, gROOT, vector


class TestAMSim(unittest.TestCase):
    """Test the pattern recognition results using PatternMatcher"""

    infile = "results.root"

    def setUp(self):
        gROOT.SetBatch(True)
        self.tfile = TFile.Open(self.infile)
        self.ttree = self.tfile.Get("ntupler/tree")
        self.nevents = self.ttree.GetEntries()
        self.tfile_tmp = TFile.Open(self.infile.replace(".root","_tmp.root"))
        self.ttree_tmp = self.tfile_tmp.Get("ntupler/tree")
        self.nevents_tmp = self.ttree_tmp.GetEntries()

    def tearDown(self):
        pass

    def test_nevents(self):
        self.assertEqual(self.nevents, 100)
        self.assertEqual(self.nevents_tmp, 100)

    def test_npassed(self):
        tree = self.ttree_tmp
        n = 0
        for ievt in tree:
            if ievt.TTStubs_passed:
                n += 1
        self.assertEqual(n, 63)  # 63 events have >=6 good stubs


if __name__ == "__main__":
    if len(sys.argv) > 1:
        TestAMSim.infile = sys.argv.pop()

    unittest.main()
