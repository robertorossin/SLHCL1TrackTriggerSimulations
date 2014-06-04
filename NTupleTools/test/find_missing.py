input = "input_SingleMuPlus_OneOverPt_vz0_20140522.txt"
fileprefix = "SingleMu_E2023TTI_%i_"
n = 500


files = []
with open(input) as f:
    for line in f:
        filename = line.strip().split("/")[-1]
        filename = filename[:-5]
        files.append(filename)

map_files = {}
for i in xrange(1,n):
    map_files[i] = 0
    for f in files:
        if (fileprefix % i) in f:
            map_files[i] += 1
            if map_files[i] != 1:
                print "DUPLICATED: ", f
    if not map_files[i]: 
        print "MISSING:", i
