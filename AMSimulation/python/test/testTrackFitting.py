import unittest
import sys
from ROOT import TFile, TTree, gROOT, vector


class TestAMSim(unittest.TestCase):
    """Test the track fitting results using TrackFitter"""

    infile = "tracks.root"

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
        pass
        #self.assertEqual(self.nevents, 100)
        #self.assertEqual(self.nevents_tmp, 100)


if __name__ == "__main__":
    if len(sys.argv) > 1:
        TestAMSim.infile = sys.argv.pop()

    unittest.main()
