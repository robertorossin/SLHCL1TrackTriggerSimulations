import unittest
import sys
from ROOT import TFile, TTree, gROOT, gSystem, vector


class TestAMSim(unittest.TestCase):
    """Test the pattern attributes produced by PatternAnalyzer"""

    infile = "attribs.root"

    def setUp(self):
        gROOT.SetBatch(True)
        gROOT.SetMacroPath(gSystem.Getenv("CMSSW_BASE")+"/src/SLHCL1TrackTriggerSimulations/AMSimulation")
        gROOT.LoadMacro("python/test/loader.h+")
        self.tfile = TFile.Open(self.infile)
        self.ttree = self.tfile.Get("patternAttributes")
        self.nevents = self.ttree.GetEntries()

    def tearDown(self):
        pass

    def test_nevents(self):
        self.assertEqual(self.nevents, 2)


if __name__ == "__main__":
    if len(sys.argv) > 1:
        TestAMSim.infile = sys.argv.pop()

    unittest.main()
