import unittest
import sys
from ROOT import TFile, TTree, gROOT, gSystem, vector


class TestAMSim(unittest.TestCase):
    """Test the matrices produced by MatrixBuilder"""

    infile = "matrices.txt"

    def setUp(self):
        pass

    def tearDown(self):
        pass

    def test_nevents(self):
        pass


if __name__ == "__main__":
    if len(sys.argv) > 1:
        TestAMSim.infile = sys.argv.pop()

    unittest.main()
