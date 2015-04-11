#!/usr/bin/env python

import subprocess

indir = "/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC25p3/SingleMuon_tt27_20150408/"
outfile = "input_SingleMuon_tt27_20150408.txt"

# ______________________________________________________________________________

print "*********************************************"
print "indir = %s" % indir
print "outfile = %s" % outfile

indir = indir if indir.endswith("/") else indir + "/"
p = subprocess.Popen("ls -v %s*" % indir, shell=True, stdout=subprocess.PIPE)
filenames = p.stdout.readlines()
filenames = filter(lambda x: x.strip().endswith(".root"), filenames)

print "\nFound %i .root files:" % len(filenames)
for i, filename in enumerate(filenames):
    if i < 10: 
        print filename,
    else:
        print "... (snipped) ..."
        break

old = "/eos/uscms"
new = "root://cmseos:1094/"
filenames = map(lambda x: x.replace(old, new), filenames)

with open(outfile, 'w') as f:
    f.write(''.join(filenames))

print "\nDONE!"
print "*********************************************"

