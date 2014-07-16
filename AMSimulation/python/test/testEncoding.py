import unittest
import sys
from itertools import izip
from ROOT import TFile, TTree, gROOT, gSystem, vector


class TestAMSim(unittest.TestCase):
    """Test the pattern recognition results using PatternMatcher"""

    infile1 = "stubs.root"
    infile2 = "roads.root"

    def setUp(self):
        gROOT.SetBatch(True)
        gROOT.SetMacroPath(gSystem.Getenv("CMSSW_BASE")+"/src/SLHCL1TrackTriggerSimulations/AMSimulation")
        gROOT.LoadMacro("python/test/loader.h+")
        self.tfile1 = TFile.Open(self.infile1)
        self.ttree1 = self.tfile1.Get("ntupler/tree")
        self.nevents1 = self.ttree1.GetEntries()
        self.tfile2 = TFile.Open(self.infile2)
        self.ttree2 = self.tfile2.Get("ntupler/tree")
        self.nevents2 = self.ttree2.GetEntries()

    def tearDown(self):
        pass

    def test_nevents(self):
        self.assertEqual(self.nevents1, 100)
        self.assertEqual(self.nevents2, 100)

    def test_encoding(self):
        tree = self.ttree1
        addresses1 = []
        for ievt in tree:
            addresses1_inner = []
            for moduleId, col, row in izip(ievt.TTStubs_modId, ievt.TTStubs_coordy, ievt.TTStubs_coordx):
                col, row = int(col), int(row)
                addresses1_inner.append((moduleId, col, row/2))
            addresses1.append(addresses1_inner)

        addresses2 = []
        tree = self.ttree2
        for ievt in tree:
            addresses2_inner = []
            for patternId in ievt.AMTTRoads_patternIds:
                for ssId in patternId:
                    addresses2_inner.append(ssId)
            addresses2.append(addresses2_inner)

        def encode(x):
            return (x[0]<<14) | (x[1]<<9) | (x[2])
        def decode(x):
            return ((x>>14) & (1<<18 - 1), (x>>9) & (1<<5 - 1), (x>>0) & (1<<9 - 1))

        n = 0
        for addr1, addr2 in izip(addresses1, addresses2):
            if n > 100:  break
            #print n, addr1, addr2
            if len(addr2):
                self.assertEqual(sorted([encode(a) for a in addr1]), sorted(addr2[:6]))
                self.assertEqual(sorted(addr1), sorted([decode(a) for a in addr2[:6]]))
            n += 1


if __name__ == "__main__":
    if len(sys.argv) > 2:
        TestAMSim.infile2 = sys.argv.pop()
        TestAMSim.infile1 = sys.argv.pop()

    unittest.main()
