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
        self.fvec3 = vector('float')()
        self.fvec4 = vector('float')()

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
            n = getattr(tree, "genParts_size")
            self.assertTrue(n > 0)
            for j in xrange(n):
                self.assertTrue(self.fvec1[j] > 0)
                self.assertTrue(abs(self.fvec2[j]) < pi+1e-6)
            with self.assertRaises(IndexError):
                print self.fvec1[n]

    def test_genJets(self):
        tree = self.ttree
        tree.SetBranchAddress("genJets_pt", self.fvec1)
        tree.SetBranchAddress("genJets_phi", self.fvec2)
        for i in xrange(self.nevents):
            tree.GetEntry(i)
            n = getattr(tree, "genJets_size")
            self.assertTrue(n >= 0)  # can be zero
            for j in xrange(n):
                self.assertTrue(self.fvec1[j] > 0)
                self.assertTrue(abs(self.fvec2[j]) < pi+1e-6)
            with self.assertRaises(IndexError):
                print self.fvec1[n]

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

    def test_simTracks(self):
        tree = self.ttree
        tree.SetBranchAddress("simTracks_pt", self.fvec1)
        tree.SetBranchAddress("simTracks_phi", self.fvec2)
        for i in xrange(self.nevents):
            tree.GetEntry(i)
            n = getattr(tree, "simTracks_size")
            self.assertTrue(n > 0)
            for j in xrange(n):
                self.assertTrue(self.fvec1[j] > 0)
                self.assertTrue(abs(self.fvec2[j]) < pi+1e-6)
            with self.assertRaises(IndexError):
                print self.fvec1[n]

    def test_simVertices(self):
        tree = self.ttree
        tree.SetBranchAddress("simVertices_vx", self.fvec1)
        tree.SetBranchAddress("simVertices_vz", self.fvec2)
        for i in xrange(self.nevents):
            tree.GetEntry(i)
            n = getattr(tree, "simVertices_size")
            self.assertTrue(n > 0)
            for j in xrange(n):
                self.assertTrue(abs(self.fvec1[j]) < 1000)
                self.assertTrue(abs(self.fvec2[j]) < 1500)
            with self.assertRaises(IndexError):
                print self.fvec1[n]

    def test_trackingParticles(self):
        tree = self.ttree
        tree.SetBranchAddress("trkParts_pt", self.fvec1)
        tree.SetBranchAddress("trkParts_phi", self.fvec2)
        for i in xrange(self.nevents):
            tree.GetEntry(i)
            n = getattr(tree, "trkParts_size")
            self.assertTrue(n > 0)
            for j in xrange(n):
                self.assertTrue(self.fvec1[j] > 0)
                self.assertTrue(abs(self.fvec2[j]) < pi+1e-6)
            with self.assertRaises(IndexError):
                print self.fvec1[n]


    #@unittest.skipIf(process != "particleGun", "Not particle gun")
    def test_particleGun(self):
        tree = self.ttree
        for i in xrange(self.nevents):
            tree.GetEntry(i)
            self.assertEqual(getattr(tree, "genParts_size"), 1)
            self.assertEqual(abs(getattr(tree, "genParts_pdgId")[0]), 13)
            
            nsimtracks = 0
            for j in xrange(getattr(tree, "simTracks_size")):
                if abs(getattr(tree, "simTracks_pdgId")[j]) == 13:
                    nsimtracks += 1
            self.assertEqual(nsimtracks, 1)

            nsimvertices = 0
            for j in xrange(getattr(tree, "simVertices_size")):
                if abs(getattr(tree, "simVertices_vz")[j]) < 30:
                    nsimvertices += 1
            self.assertEqual(nsimvertices, 1)

            ntrkparts = 0
            for j in xrange(getattr(tree, "trkParts_size")):
                if abs(getattr(tree, "trkParts_pdgId")[j]) == 13:
                    ntrkparts += 1
            self.assertEqual(ntrkparts, 1)


if __name__ == "__main__":
    if len(sys.argv) > 1:
        TestNTuple.infile = sys.argv.pop()

    unittest.main()

