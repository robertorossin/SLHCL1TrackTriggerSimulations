#!/usr/bin/env python

from rootdrawing import *
from parser import *



# ______________________________________________________________________________
# Main function
def main(options):

    fname = options.infile
    
    f = open(fname)
    towers=[]
    tower_line=[]
    
    first_line=""
    
    for line in f:
        if (len(line)>100):
            print line[0:3]
            towerList = line[4:].split(",")
            ieta = int(line[0:1])
            iphi = int(line[2:3])
            tower=[]
            for module in towerList:
                moduleID=int(module)
                tower.append(moduleID)
            
            tower_line.append(tower)
            if (iphi==8):
                towers.append(tower_line)
                tower_line=[]
        else:
            first_line=line
            
    print towers
    print len(towers)
    print len(towers[0])

    new_towers=[]
    for ieta, tower_line in enumerate(towers):
        new_tower_line=[]
        for iphi, tower in enumerate(tower_line):
            print ieta+1, iphi+1, len(tower)
            
            new_tower=[]
            for moduleID in tower:
                if moduleID<100000:
                    new_tower.append(moduleID)
                else:
                    iphiRotate = (iphi+6)%8
                    towerRotate = towers[ieta][iphiRotate]
                    for moduleIDrotate in towerRotate:
                        if moduleIDrotate>100000:
                            new_tower.append(moduleIDrotate)
                    break
            new_tower_line.append(new_tower)
        new_towers.append(new_tower_line)

    print "\n"

    
    f = open(fname.replace("trigger","rotate_trigger"), 'w')
    f.write(first_line)


    for ieta, tower_line in enumerate(new_towers):
        for iphi, tower in enumerate(tower_line):
            print ieta+1, iphi+1, len(tower)
            
            f.write(str(ieta+1))
            f.write(",")
            f.write(str(iphi+1))

            for moduleID in tower:
                f.write(","+str(moduleID))
            f.write("\n")

    f.closed



# ______________________________________________________________________________
if __name__ == '__main__':

    # Setup argument parser
    parser = argparse.ArgumentParser()

    # Add default arguments
    add_drawer_arguments(parser)

    # Add more arguments
    parser.add_argument("ss", help="short name of superstrip definition (e.g. ss256)")
    parser.add_argument("npatterns", type=int, help="number of patterns to reach the desired coverage")
    parser.add_argument("--coverage", type=float, default=0.95, help="desired coverage (default: %(default)s)")
    parser.add_argument("--minPt", type=float, default=2, help="min pT for gen particle (default: %(default)s)")
    parser.add_argument("--maxChi2", type=float, default=5, help="max reduced chi-squared (default: %(default)s)")
    parser.add_argument("--minNLayers", type=int, default=5, help="min number of layers (default: %(default)s)")
    parser.add_argument("--minDigiFraction", type=float, default=0.3, help="min contribution to a digi (default: %(default)s)")

    # Parse default arguments
    options = parser.parse_args()
#     parse_drawer_options(options)
    options.ptmin = options.minPt

    # Call the main function
    main(options)
