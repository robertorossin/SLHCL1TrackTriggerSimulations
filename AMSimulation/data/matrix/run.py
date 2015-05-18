#!/usr/bin/env python

import subprocess
import sys

input   = "../stubs_tt27_5M.2.root"
output  = "matrices_tt27_pt{ptSegment}_hb{hitBits}.txt"

cmd = "amsim -M -i {input} -o {output} -v 2 --minInvPt {minInvPt:.6f} --maxInvPt {maxInvPt:.6f} --hitBits {hitBits} --no-color >& {log}"

ptSegment = 0
hitBits   = 0

INVPT   = 1./2.
NSEG    = 10

def doit():

    minInvPt = -INVPT + ptSegment * (INVPT / NSEG * 2)
    maxInvPt = -INVPT + (ptSegment+1) * (INVPT / NSEG * 2)

    this_output = output.format(ptSegment=ptSegment, hitBits=hitBits)
    this_log = this_output[:-4] + ".log"

    this_cmd = cmd.format(input=input, output=this_output, minInvPt=minInvPt, maxInvPt=maxInvPt, hitBits=hitBits, log=this_log)
    print this_cmd

    try:
        ret = subprocess.call(this_cmd, shell=True)
        if ret < 0:
            print >>sys.stderr, "Process terminated by signal", -ret
        elif ret != 0:
            print >>sys.stderr, "Process returned", ret
    except OSError as e:
        print >>sys.stderr, "Execution failed.", e


# ______________________________________________________________________________
if __name__ == '__main__':

    for i in xrange(NSEG):
        for j in xrange(7):
            ptSegment = i
            hitBits = j

            doit()

    print "DONE"
