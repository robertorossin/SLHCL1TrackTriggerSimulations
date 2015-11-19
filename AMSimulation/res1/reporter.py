#!/usr/bin/env python

import os
import sys
from math import sqrt, pi
from itertools import izip

from ROOT import TTree, TFile, gROOT, gSystem

# ______________________________________________________________________________
# Options

class Options:
    def __init__(self):
        pass

options = Options()

options.nevents = 1000           # analyze first nevents
options.threshold_nroads = 80   # analyze event if nroads > threshold (when event_list is empty)
options.threshold_ntracks = 1   # analyze event if ntracks > threshold (when event_list is empty)

options.event_list = [939]
#options.event_list = [7, 43, 59, 152, 202, 221, 235, 375, 439,]

options.use_color = True


# ______________________________________________________________________________
# Various printing/formatting functions

def apply_color(s):
    lightred = '\033[91m'
    lightgreen = '\033[92m'
    end = '\033[0m'
    s = lightgreen + s + end
    return s

def print_superstrips(superstripIdsA, stubRefsA):
    s = ""
    for x, y in izip(superstripIdsA, stubRefsA):
        if options.use_color and y.size():
            s += apply_color("%4i" % x)
        else:
            s += "%4i" % x
        s += " "
    return s

def print_matching(stubRefsA):
    #return " ".join(["%1i" % x.size() for x in stubRefsA])
    s = ""
    n = 0
    for x in stubRefsA:
        s += "%1i" % x.size()
        s += " "
        if x.size():
            n += 1
    s += "[%1i/6]" % n
    return s

def print_matching_for_tracks(stubRefs):
    n = 0
    for x in stubRefs:
        if x != 999999999:
            n += 1
    s = "[%1i/6]" % n
    return s

def print_layered_superstrips(superstrips, superstrips_hit, d=4):
    formatter = lambda x: ("%"+str(d)+"i") % x

    strings = [""] * 6
    for layer, s in enumerate(strings):
        strings[layer] += "(L%i) " % layer

    for i, (layer, x) in enumerate(superstrips):
        assert(layer < 6)
        if options.use_color and (layer, x) in superstrips_hit:
            strings[layer] += apply_color(formatter(x))
        else:
            strings[layer] += formatter(x)
        strings[layer] += " "

    for layer, s in enumerate(strings):
        strings[layer] = s[:-1]

    s = "\n".join(strings)
    return s

def print_delta(lhs, rhs):
    diff = float(lhs) - float(rhs)
    diff_p = diff/rhs * 100
    #s = "% .3f (% .2f%%)" % (diff, diff_p)
    s = "% .2f%%" % (diff_p)
    return s

# ______________________________________________________________________________
# Speed up TTree loading

def set_branch_status(tree):
    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("AMTTRoads_patternRef"     , 1)
    tree.SetBranchStatus("AMTTRoads_patternInvPt"   , 1)
    tree.SetBranchStatus("AMTTRoads_superstripIds"  , 1)
    tree.SetBranchStatus("AMTTRoads_stubRefs"       , 1)
    tree.SetBranchStatus("AMTTTracks_pt"            , 1)
    tree.SetBranchStatus("AMTTTracks_eta"           , 1)
    tree.SetBranchStatus("AMTTTracks_phi0"          , 1)
    tree.SetBranchStatus("AMTTTracks_z0"            , 1)
    tree.SetBranchStatus("AMTTTracks_invPt"         , 1)
    tree.SetBranchStatus("AMTTTracks_chi2"          , 1)
    tree.SetBranchStatus("AMTTTracks_ndof"          , 1)
    tree.SetBranchStatus("AMTTTracks_patternRef"    , 1)
    tree.SetBranchStatus("AMTTTracks_roadRef"       , 1)
    tree.SetBranchStatus("AMTTTracks_combRef"       , 1)
    tree.SetBranchStatus("AMTTTracks_stubRefs"      , 1)
    tree.SetBranchStatus("AMTTTracks_tpId"          , 1)
    tree.SetBranchStatus("AMTTTracks_synTpId"       , 1)
    tree.SetBranchStatus("trkParts_primary"         , 1)
    tree.SetBranchStatus("trkParts_pt"              , 1)
    tree.SetBranchStatus("trkParts_eta"             , 1)
    tree.SetBranchStatus("trkParts_phi"             , 1)
    tree.SetBranchStatus("trkParts_vz"              , 1)
    tree.SetBranchStatus("trkParts_charge"          , 1)

def reset_branch_status(tree):
    tree.SetBranchStatus("*", 1)

# ______________________________________________________________________________
# Selectors

def select_particles(evt):
    """Select tracking particles in the trigger tower"""

    particles = []
    err = 0.002
    for ipart, (pt, eta, phi, vz, charge, primary) in enumerate(
        izip(evt.trkParts_pt, evt.trkParts_eta, evt.trkParts_phi, evt.trkParts_vz, evt.trkParts_charge, evt.trkParts_primary)):
        if primary:
            if (1.0/pt <= 1.0/3+err*10) and (0.-err*80 <= eta <= 4.4/6+err*80) and (pi/4-err <= phi <= pi/2+err):
                particle = (ipart, pt, charge*1.0/pt, eta, phi, vz)
                particles.append(particle)
    return particles

def select_tracks(evt):
    """Select all L1 tracks"""

    tracks = []
    for itrack, (pt, invPt, eta, phi, vz, chi2, ndof, patternRef, roadRef, combRef, stubRefs, tpId, synTpId) in enumerate(
        izip(evt.AMTTTracks_pt, evt.AMTTTracks_invPt, evt.AMTTTracks_eta, evt.AMTTTracks_phi0, evt.AMTTTracks_z0, evt.AMTTTracks_chi2, evt.AMTTTracks_ndof,
        evt.AMTTTracks_patternRef, evt.AMTTTracks_roadRef, evt.AMTTTracks_combRef, evt.AMTTTracks_stubRefs, evt.AMTTTracks_tpId, evt.AMTTTracks_synTpId)):

        itrack = 0  # reset itrack
        track = (itrack, pt, invPt, eta, phi, vz, chi2/ndof, patternRef, roadRef, combRef, stubRefs, tpId, synTpId)
        tracks.append(track)

    tracks.sort(key=lambda x: x[2])  # re-sort by invPt

    for itrack, track in enumerate(tracks):
        track = (itrack,) + track[1:]  # update itrack
        tracks[itrack] = track
    return tracks

def get_closest_particle(invPt, phi, particles):
    """Super simple L1 track -> tracking particle matching, using invPt and phi"""

    # from AMSimulation/src/MCTruthAssociator.cc
    rms_invPt = 0.001283
    rms_phi0  = 0.000453

    min_dist = 999999.
    for part in particles:
        part_invPt, part_phi = part[2], part[4]
        dist = sqrt(((invPt-part_invPt)/rms_invPt)**2 + ((phi-part_phi)/rms_phi0)**2)
        if min_dist > dist:
            min_dist = dist
            ret = part
    return ret

# ______________________________________________________________________________
# Main functions

def init():

    gROOT.LoadMacro(
    "/home/rossin/Dropbox/TT/CMSSW_6_2_0_SLHC25_patch3/src/SLHCL1TrackTriggerSimulations/AMSimulationIO/src/AMSimulationIOLinkDef.h")

def main():

    # Get input file name
    if len(sys.argv) != 2:
        print("Usage: %s %s" % (sys.argv[0], "tracks.root"))
        raise Exception("Cannot parse arguments")

    fname = sys.argv[1]

    tfile = TFile.Open(fname)
    ttree = tfile.Get("ntupler/tree")
    if not ttree:
        raise Exception("Cannot retrieve the tree")

    # Speed up TTree loading
    set_branch_status(ttree)

    # Loop over events
    for ievt, evt in enumerate(ttree):
        if ievt == options.nevents:
            break

        if not options.event_list:
            if evt.AMTTRoads_patternRef.size() < options.threshold_nroads:
                continue

            if evt.AMTTTracks_patternRef.size() < options.threshold_ntracks:
                continue
        else:
            if ievt not in options.event_list:
                continue

        # Tracking particles
        particles = select_particles(evt)
        print("evt {0:3} -- Found {1:3} particles:".format(ievt, len(particles)))
        if len(particles) == 0:
            raise Exception("No particle!")

        for (ipart, pt, invPt, eta, phi, vz) in particles:
            print("  part {0:5} pt: {1:6.3f}, invPt: {2: .3f}, eta: {3: .3f}, phi: {4: .3f}, vz: {5: .3f}".format(
                ipart, pt, invPt, eta, phi, vz))

        # AM roads
        print("evt {0:3} -- Found {1:3} roads:".format(ievt, evt.AMTTRoads_patternRef.size()))

        superstrips = []
        superstrips_hit = []
        stubs = []

        for iroad, (patternRef, patternInvPt, superstripIdsA, stubRefsA) in enumerate(
            izip(evt.AMTTRoads_patternRef, evt.AMTTRoads_patternInvPt, evt.AMTTRoads_superstripIds, evt.AMTTRoads_stubRefs)):

            # Collect superstrips and stubs
            for isuperstrip, (superstripIdsB, stubRefsB) in enumerate(
                izip(superstripIdsA, stubRefsA)):

                layer = isuperstrip
                ss = (layer, superstripIdsB)
                if ss not in superstrips:
                    superstrips.append(ss)

                if stubRefsB.size():
                    if ss not in superstrips_hit:
                        superstrips_hit.append(ss)

                for istub, (stubRefsC) in enumerate(stubRefsB):

                    stub = (layer, stubRefsC)
                    if stub not in stubs:
                        stubs.append(stub)

            print("  road {0:3} patternRef: {1:8}, patternInvPt: {2: .3f}, ss: {3}, nMatched: {4}".format(
                iroad, patternRef, patternInvPt, print_superstrips(superstripIdsA, stubRefsA), print_matching(stubRefsA)))

        superstrips.sort()
        print("evt {0:3} -- Found {1:3} superstrips:".format(ievt, len(superstrips)))
        print("  {0}".format(print_layered_superstrips(superstrips, superstrips_hit).replace("\n", "\n  ")))

        stubs.sort()
        print("evt {0:3} -- Found {1:3} stubs:".format(ievt, len(stubs)))
        print("  {0}".format(print_layered_superstrips(stubs, stubs, d=5).replace("\n", "\n  ")))

        # AM tracks
        tracks = select_tracks(evt)
        print("evt {0:3} -- Found {1:3} tracks:".format(ievt, len(tracks)))

        synthetics = []
        synParticles = []

        for (itrack, pt, invPt, eta, phi, vz, chi2Red, patternRef, roadRef, combRef, stubRefs, tpId, synTpId) in tracks:
            closest = get_closest_particle(invPt, phi, particles)
            closest_ipart, closest_invPt, closest_phi = closest[0], closest[2], closest[4]

            print("  track {0:3} pt: {1:6.3f}, invPt: {2: .3f}, eta: {3: .3f}, phi: {4: .3f}, vz: {5: .3f}, synTpId: {6:5}, myTpId: {7:5} d(invPt): {8}, d(phi): {9}, patternRef: {10:8}, roadRef: {11:3} {12}".format(
                itrack, pt, invPt, eta, phi, vz, synTpId, closest_ipart, print_delta(invPt, closest_invPt), print_delta(phi, closest_phi), patternRef, roadRef, print_matching_for_tracks(stubRefs)))

            # Collect reconstructed "particles"
            particle = (synTpId, pt, invPt, eta, phi, vz)

            if synTpId != -2 and synTpId != -1 and synTpId not in synthetics:
                synthetics.append(synTpId)
                synParticles.append(particle)

        # Synthetic MC matching
        print("evt {0:3} -- Reconstructed {1:3} particles:".format(ievt, len(synParticles)))

        synParticles.sort(key=lambda x: x[0])

        for (ipart, pt, invPt, eta, phi, vz) in synParticles:

            matchedParts = filter(lambda x: x[0] == ipart, particles)
            assert(len(matchedParts) == 1)  # make sure the tracking particle was selected
            (ipart, tp_pt, tp_invPt, tp_eta, tp_phi, tp_vz) = matchedParts[0]

            print("  part {0:5} pt: {1:6.3f}, invPt: {2: .3f}, eta: {3: .3f}, phi: {4: .3f}, vz: {5: .3f}, d(invPt): {6}, d(eta): {7}, d(phi): {8}, d(vz): {9}".format(
                ipart, pt, invPt, eta, phi, vz, print_delta(invPt, tp_invPt), print_delta(eta, tp_eta), print_delta(phi, tp_phi), print_delta(vz,tp_vz)))

        print("-" * 16)

    reset_branch_status(ttree)
    return 0


# ______________________________________________________________________________
if __name__ == "__main__":

    init()

    main()
