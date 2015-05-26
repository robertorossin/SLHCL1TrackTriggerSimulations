#!/usr/bin/env python

import sys

filename = "stubs_tt27_100M.root"
ver = 0
outdir = "/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC25p3_ntuple/stubs_tt27_100M/"

# ______________________________________________________________________________

print "*********************************************"
print "*** Remember to create your GRID proxy! *****\n"

old = "/eos/uscms"
new = "root://cmseos:1094/"

outdir = outdir if outdir.endswith("/") else outdir + "/"
outdir1 = outdir.replace(old, new)

old = ".root"
new = ".%i.root" % ver
filename1 = filename.replace(old, new)

print "xrdcp -np %s %s" % (filename, outdir1+filename1)
print "mv %s %s" % (filename.replace(".root",".log"), outdir+(filename1.replace(".root",".log")))

print "\nAfter transfer:"
print "ls -l %s" % filename
print "ls -l %s" % (outdir+filename1)
print "rm %s" % filename
print "ln -s %s" % (outdir+filename1)

print "*********************************************"

