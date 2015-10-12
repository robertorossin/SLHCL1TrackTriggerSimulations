#!/usr/bin/env python

import json
from math import pi, sqrt, sinh, asin, atan2

# tklayout version:
#   https://github.com/tkLayout/tkLayout/blob/master/src/AnalyzerVisitors/TriggerProcessorBandwidth.cpp

# ______________________________________________________________________________
# Functions
convert_key_to_int = lambda pairs: dict([(int(k),v) for (k,v) in pairs])

quadsum = lambda x,y: sqrt(x*x + y*y)

average = lambda x: sum(x, 0.0) / len(x)

def convert_xyz_to_cyl(xyz):
    cyl = []
    for i in xrange(4):
        r = quadsum(xyz[3*i], xyz[3*i+1])
        phi = atan2(xyz[3*i+1], xyz[3*i])
        z = xyz[3*i+2]
        cyl += [r, phi, z]
    return cyl

def get_delta_phi(phi1, phi2):
    result = phi1 - phi2
    while result > pi: result -= 2*pi
    while result <= -pi: result += 2*pi
    return result


# Load existing maps
vertexmap = json.load(open("../data/module_vertices.json"), object_pairs_hook=convert_key_to_int)

# Get layer number from moduleId
def decodeLayer(moduleId):
    return moduleId / 10000

# Get the parameter space of a trigger tower
def get_parameter_space(tower):
    ieta = tower/8
    iphi = tower%8
    etamin = -2.2 + (4.4/6) * ieta
    etamax = -2.2 + (4.4/6) * (ieta+1)
    if iphi < 6:
        phimin = -pi/2 + (2*pi/8) * iphi
        phimax = -pi/2 + (2*pi/8) * (iphi+1)
    else:
        phimin = -2*pi -pi/2 + (2*pi/8) * iphi
        phimax = -2*pi -pi/2 + (2*pi/8) * (iphi+1)
    return (phimin, phimax, etamin, etamax)

# Get the trajectories in the physical space of a trigger tower
def get_trajectories(tower, min_pt=2., max_vz=5., max_rho=110.0950, debug=False):
    phimin, phimax, etamin, etamax = get_parameter_space(tower)
    cotmin = sinh(etamin)
    cotmax = sinh(etamax)

    # Magic numbers from tklayout
    tklayout_phi_magic = 0.18
#     tklayout_phi_magic = 0.05
    tklayout_z_magic = max_vz/max_rho

    if debug:  print "min_pt={0} max_vz={1} max_rho={2:.4f}".format(min_pt, max_vz, max_rho)
    if debug:  print "phimin={0:.4f} phimax={1:.4f} etamin={2:.4f} etamax={3:.4f} cotmin={4:.4f} cotmax={5:.4f}".format(phimin, phimax, etamin, etamax, cotmin, cotmax)

    mPtFactor = 0.3*3.8*1e-2/2.0
    invPt = 1.0/float(min_pt)

    # Define the trajectories
    def traj_phimin(r):
        #deltaPhi = - mPtFactor * r * invPt
        deltaPhi = - asin(mPtFactor * r * invPt)
        phi = phimin + tklayout_phi_magic + deltaPhi
        if phi < -pi: phi += 2*pi
        return phi

    def traj_phimax(r):
        #deltaPhi = - mPtFactor * r * invPt
        deltaPhi = - asin(mPtFactor * r * invPt)
        phi = phimax - tklayout_phi_magic - deltaPhi
        if phi > +pi: phi -= 2*pi
        return phi

    def traj_zmin(r):
        deltaZ = r * cotmin
        #deltaZ = (1.0 / (mPtFactor * invPt) * asin(mPtFactor * r * invPt)) * cotmin
        return -max_vz + r*tklayout_z_magic + deltaZ

    def traj_zmax(r):
        deltaZ = r * cotmax
        #deltaZ = (1.0 / (mPtFactor * invPt) * asin(mPtFactor * r * invPt)) * cotmax
        return +max_vz - r*tklayout_z_magic + deltaZ

    def traj_rmin(z):
        if cotmax == 0:
            return 0
        r = (z - max_vz) / (cotmax - tklayout_z_magic)
        return r

    def traj_rmax(z):
        if cotmin == 0:
            return 0
        r = (z + max_vz) / (cotmin + tklayout_z_magic)
        return r
    return (traj_phimin, traj_phimax, traj_zmin, traj_zmax, traj_rmin, traj_rmax)

# Filter the modules based on the trajectories
def filter_modules(trajs, moduleIds, debug=False):
    results = []

    traj_phimin, traj_phimax, traj_zmin, traj_zmax, traj_rmin, traj_rmax = trajs

    def is_phi_good(phis, phimin, phimax):
        return any(get_delta_phi(phimin, phimin) <= get_delta_phi(phi, phimin) <= get_delta_phi(phimax, phimin) for phi in phis)

    def is_z_good(zees, zmin, zmax):
        return any(zmin <= zee <= zmax for zee in zees)

    for moduleId in moduleIds:
        cyl = vertexmap2[moduleId]

        lay = decodeLayer(moduleId)
        if 5 <= lay <= 10:  # Barrel
            r = average(cyl[0::3])
            phimin = traj_phimin(r)
            phimax = traj_phimax(r)
            zmin = traj_zmin(r)
            zmax = traj_zmax(r)

            if debug:  print "{0} {1:.4f} {2:.4f} {3:.4f} {4:.4f} {5:.4f}".format(moduleId, r, phimin, phimax, zmin, zmax)
            if debug:  print "{0} {1:.4f} {2:.4f} {3:.4f} {4:.4f} {5:.4f} {6:.4f} {7:.4f} {8:.4f} {9:.4f}".format(moduleId, r, *(cyl[1::3] + cyl[2::3]))

            assert(get_delta_phi(phimax, phimin) > 0)
            assert(zmax - zmin > 0)

            if is_phi_good(cyl[1::3], phimin, phimax) and is_z_good(cyl[2::3], zmin, zmax):
                results.append(moduleId)

        elif 11 <= lay <= 15 or 18 <= lay <= 22:  # Endcap
            z = average(cyl[2::3])
            if z > 0:
                rmin = traj_rmin(z)
                rmax = traj_rmax(z)
            else:
                rmin = traj_rmax(z)
                rmax = traj_rmin(z)
            r = average(cyl[0::3])
            phimin = traj_phimin(r)
            phimax = traj_phimax(r)

            if debug:  print "{0} {1:.4f} {2:.4f} {3:.4f} {4:.4f} {5:.4f}".format(moduleId, z, phimin, phimax, rmin, rmax)
            if debug:  print "{0} {1:.4f} {2:.4f} {3:.4f} {4:.4f} {5:.4f} {6:.4f} {7:.4f} {8:.4f} {9:.4f}".format(moduleId, z, *(cyl[1::3] + cyl[0::3]))

            assert(get_delta_phi(phimax, phimin) > 0)
            assert((16 <= tower <= 31) or (rmax - rmin > 0))

            if is_phi_good(cyl[1::3], phimin, phimax) and is_z_good(cyl[0::3], rmin, rmax):
                results.append(moduleId)

    return set(results)

# ______________________________________________________________________________
# Benchmark
tt27_moduleIds = set([50230, 50231, 50232, 50233, 50234, 50235, 50236, 50237,
50330, 50331, 50332, 50333, 50334, 50335, 50336, 50337, 50430, 50431, 50432,
50433, 50434, 50435, 50436, 50437, 60326, 60327, 60328, 60329, 60330, 60331,
60332, 60333, 60334, 60335, 60426, 60427, 60428, 60429, 60430, 60431, 60432,
60433, 60434, 60435, 60526, 60527, 60528, 60529, 60530, 60531, 60532, 60533,
60534, 60535, 60626, 60627, 60628, 60629, 60630, 60631, 60632, 60633, 60634,
60635, 70426, 70427, 70428, 70429, 70430, 70431, 70432, 70433, 70434, 70435,
70436, 70437, 70526, 70527, 70528, 70529, 70530, 70531, 70532, 70533, 70534,
70535, 70536, 70537, 70626, 70627, 70628, 70629, 70630, 70631, 70632, 70633,
70634, 70635, 70636, 70637, 70726, 70727, 70728, 70729, 70730, 70731, 70732,
70733, 70734, 70735, 70736, 70737, 70826, 70827, 70828, 70829, 70830, 70831,
70832, 70833, 70834, 70835, 70836, 70837, 70926, 70927, 70928, 70929, 70930,
70931, 70932, 70933, 70934, 70935, 70936, 70937, 80611, 80612, 80613, 80614,
80615, 80616, 80617, 80711, 80712, 80713, 80714, 80715, 80716, 80717, 80811,
80812, 80813, 80814, 80815, 80816, 80817, 80911, 80912, 80913, 80914, 80915,
80916, 80917, 81011, 81012, 81013, 81014, 81015, 81016, 81017, 81111, 81112,
81113, 81114, 81115, 81116, 81117, 81211, 81212, 81213, 81214, 81215, 81216,
81217, 90711, 90712, 90713, 90714, 90715, 90716, 90717, 90718, 90719, 90811,
90812, 90813, 90814, 90815, 90816, 90817, 90818, 90819, 90911, 90912, 90913,
90914, 90915, 90916, 90917, 90918, 90919, 91011, 91012, 91013, 91014, 91015,
91016, 91017, 91018, 91019, 91111, 91112, 91113, 91114, 91115, 91116, 91117,
91118, 91119, 91211, 91212, 91213, 91214, 91215, 91216, 91217, 91218, 91219,
91311, 91312, 91313, 91314, 91315, 91316, 91317, 91318, 91319, 91411, 91412,
91413, 91414, 91415, 91416, 91417, 91418, 91419, 91511, 91512, 91513, 91514,
91515, 91516, 91517, 91518, 91519, 91611, 91612, 91613, 91614, 91615, 91616,
91617, 91618, 91619, 100811, 100812, 100813, 100814, 100815, 100816, 100817,
100818, 100819, 100820, 100911, 100912, 100913, 100914, 100915, 100916, 100917,
100918, 100919, 100920, 101011, 101012, 101013, 101014, 101015, 101016, 101017,
101018, 101019, 101020, 101111, 101112, 101113, 101114, 101115, 101116, 101117,
101118, 101119, 101120, 101211, 101212, 101213, 101214, 101215, 101216, 101217,
101218, 101219, 101220, 101311, 101312, 101313, 101314, 101315, 101316, 101317,
101318, 101319, 101320, 101411, 101412, 101413, 101414, 101415, 101416, 101417,
101418, 101419, 101420, 101511, 101512, 101513, 101514, 101515, 101516, 101517,
101518, 101519, 101520, 101611, 101612, 101613, 101614, 101615, 101616, 101617,
101618, 101619, 101620, 101711, 101712, 101713, 101714, 101715, 101716, 101717,
101718, 101719, 101720, 101811, 101812, 101813, 101814, 101815, 101816, 101817,
101818, 101819, 101820, 101911, 101912, 101913, 101914, 101915, 101916, 101917,
101918, 101919, 101920, 102011, 102012, 102013, 102014, 102015, 102016, 102017,
102018, 102019, 102020, 102111, 102112, 102113, 102114, 102115, 102116, 102117,
102118, 102119, 102120])


vertexmap2 = {}
moduleIds_set = set()
max_rho = 0
for moduleId, xyz in vertexmap.iteritems():
    cyl = convert_xyz_to_cyl(xyz)
    vertexmap2[moduleId] = cyl

    if moduleId > 0:
        moduleIds_set.add(moduleId)

    if max_rho < max(cyl[::3]):
        max_rho = max(cyl[::3])
#print "max_rho = %.4f" % max_rho

# ______________________________________________________________________________
# Main
ttmap = {}
for tower in xrange(48):

    # Get the trajectories
    trajs = get_trajectories(tower)

    # Get the moduleIds
    tower_moduleIds = filter_modules(trajs, moduleIds_set)

    ttmap[tower] = sorted(tower_moduleIds)

    if tower == 27:
        aset = tower_moduleIds
        print "[NEW]"
        print "tt=%i len=%i %s" % (tower, len(aset), repr(sorted(aset)))
        aset = tt27_moduleIds
        print "[BENCHMARK]"
        print "tt=%i len=%i %s" % (tower, len(aset), repr(sorted(aset)))
        aset = tt27_moduleIds.difference(tower_moduleIds)
        print "[in BENCHMARK but not in NEW]"
        print "tt=%i len=%i %s" % (tower, len(aset), repr(sorted(aset)))
        aset = tower_moduleIds.difference(tt27_moduleIds)
        print "[in NEW but not in BENCHMARK]"
        print "tt=%i len=%i %s" % (tower, len(aset), repr(sorted(aset)))

# Prepare for writing
writeme = []
for tower in xrange(48):
    if tower in ttmap:
        ieta = tower/8
        iphi = tower%8
        tower_moduleIds = ttmap[tower]
        s = ",".join("{0}".format(n) for n in [ieta+1,iphi+1]+tower_moduleIds)
        #print s
        writeme.append(s)

        print "Tower", tower, "has", len(tower_moduleIds), "modules."

# ______________________________________________________________________________
# Write .csv file
with open("../data/trigger_sector_map_new.csv", "w") as f:
    writeme = ["eta_idx, phi_idx, module_list"] + writeme
    f.write("\n".join(writeme))

# Write .json file
mymap = {}
with open("../data/trigger_sector_map_new.csv", "r") as f:
    for line in f:
        if not line[0].isdigit():
            continue
        values = line.split(",")

        # Convert to int
        values = [int(x) for x in values]

        key = (values[0]-1)*8 + (values[1]-1)
        values = sorted(values[2:])
        mymap[key] = values

assert(len(mymap) == 6*8)

json.dump(mymap, open("../data/trigger_sector_map_new.json", "w"), sort_keys=True)
