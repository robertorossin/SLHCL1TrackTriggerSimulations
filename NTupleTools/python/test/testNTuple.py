import unittest
import sys
from math import pi
from ROOT import TFile, TTree, gROOT, vector


class TestNTuple(unittest.TestCase):
    """Test the ntuple produced by NTupleTools"""

    infile = "test_ntuple.root"

    def setUp(self):
        gROOT.SetBatch(True)
        self.runOnMC = True
        self.tfile = TFile.Open(self.infile)
        self.ttree = self.tfile.Get("ntupler/tree")
        self.nevents = self.ttree.GetEntriesFast()
        self.fvec1 = vector('float')()
        self.fvec2 = vector('float')()

    def tearDown(self):
        pass

    def test_nevents(self):
        self.assertEqual(self.nevents, 20)

    def test_eventInfo(self):
        tree = self.ttree
        for i in xrange(self.nevents):
            tree.GetEntry(i)
            run = getattr(tree, "run")
            event = getattr(tree, "event")
            lumi = getattr(tree, "lumi")
            print "Processing Run %i, Event %i, LumiSection %i." % (run, event, lumi)

    def test_genParticles(self):
        tree = self.ttree
        tree.SetBranchAddress("genParts_pt", self.fvec1)
        tree.SetBranchAddress("genParts_phi", self.fvec2)
        for i in xrange(self.nevents):
            tree.GetEntry(i)
            nparts = getattr(tree, "genParts_size")
            self.assertTrue(nparts > 0)
            for j in xrange(nparts):
                self.assertTrue(self.fvec1[j] > 0)
                self.assertTrue(abs(self.fvec2[j]) < pi+1e-6)
            with self.assertRaises(IndexError):
                print self.fvec1[nparts]

    def test_genJets(self):
        tree = self.ttree
        tree.SetBranchAddress("genJets_pt", self.fvec1)
        tree.SetBranchAddress("genJets_phi", self.fvec2)
        for i in xrange(self.nevents):
            tree.GetEntry(i)
            njets = getattr(tree, "genJets_size")
            self.assertTrue(njets >= 0)  # can be zero
            for j in xrange(njets):
                self.assertTrue(self.fvec1[j] > 0)
                self.assertTrue(abs(self.fvec2[j]) < pi+1e-6)
            with self.assertRaises(IndexError):
                print self.fvec1[njets]

    def test_genMET(self):
        tree = self.ttree
        for i in xrange(self.nevents):
            tree.GetEntry(i)
            fval1 = getattr(tree, "genMET_pt")
            fval2 = getattr(tree, "genMET_phi")
            self.assertTrue(fval1 > 0)
            self.assertTrue(abs(fval2) < pi+1e-6)

    def test_genEventInfo(self):
        tree = self.ttree
        for i in xrange(self.nevents):
            tree.GetEntry(i)
            fval1 = getattr(tree, "gen_trueNPV")
            fval2 = getattr(tree, "gen_weightMC")
            self.assertTrue(fval1 >= 0)
            self.assertTrue(fval2 >= 0)

    #@unittest.skipIf(process != "particleGun", "Not particle gun")
    def test_particleGun(self):
        tree = self.ttree
        for i in xrange(self.nevents):
            tree.GetEntry(i)
            self.assertEqual(tree.genParts_size, 1)
            self.assertEqual(abs(tree.genParts_pdgId[0]), 13)


if __name__ == "__main__":
    if len(sys.argv) > 1:
        TestNTuple.infile = sys.argv.pop()

    unittest.main()

