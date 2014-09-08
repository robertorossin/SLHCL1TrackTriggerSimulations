from math import floor
import json

# ______________________________________________________________________________
# Constants

b_nlads = [16, 24, 34, 48, 62, 76]
b_nmods = [63, 55, 54, 24, 24, 24]

e_offset = 5
e_nrings = [15 + e_offset]
e_nmods  = [20, 24, 28, 28, 32, 36, 36, 40, 40, 52, 56, 64, 68, 76, 80]

tt_netas = 6
tt_nphis = 8

convert_key_to_int = lambda pairs: dict([(int(k),v) for (k,v) in pairs])

def get_reverse_map(direct_map):
    reverse_map = {}
    for i in xrange(6*8):
        for m in direct_map[i]:
            reverse_map.setdefault(m, []).append(i)
    return reverse_map

# ______________________________________________________________________________
# Binning

class EtaBinning:
    def __init__(self, xtitle, nbinsx, xlow, xup):
        self.xtitle = xtitle
        self.nbinsx, self.xlow, self.xup = nbinsx, xlow, xup
        self.binwidth = float(self.xup - self.xlow) / self.nbinsx

    def findBin(self, x):
        if x <  self.xlow: x = self.xlow
        if x >= self.xup : x = self.xup - 1e-6
        x = float(x - self.xlow) / self.binwidth
        return int(x)

    def getBinCenter(self, b):
        if b < 0           : b = 0
        if b >= self.nbinsx: b = self.nbinsx - 1
        b = self.xlow + (0.5+b) * self.binwidth
        return b

# ______________________________________________________________________________
# Encoding

def halfStripRound(x):
    p = 10.
    return int(floor((x*2)*p + 0.5)/p)

def decodeLayer(moduleId):
    return moduleId / 10000

def decodeLadder(moduleId):
    return (moduleId / 100) % 100

def decodeModule(moduleId):
    return moduleId % 100

def isPSModule(moduleId):
    lay = decodeLayer(moduleId)
    if 5 <= lay <= 7:
        return True
    lad = decodeLadder(moduleId)
    if 11 <= lay <= 22 and lad <= 8:
        return True
    return False

def isBarrelModule(moduleId):
    lay = decodeLayer(moduleId)
    if 5 <= lay <= 10:
        return True
    return False

