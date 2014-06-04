inputs = [
"input_SingleMuPlus_OneOverPt_vz0_20140429ext1.txt",
"input_SingleMuPlus_OneOverPt_vz0_20140522.txt",
"input_SingleMuPlus_OneOverPt_vz0_20140522ext1.txt",
]
eos = "/eos/uscms/"
branchname = "uints_randomEngineSeedKeeper__RAWSIM.obj"
nevents = 10000

files = []
for input in inputs:
    with open(input) as f:
        for line in f:
            filename = line.strip()
            filename = eos + filename
            files.append(filename)

from ROOT import gROOT
from DataFormats.FWLite import Events, Handle
gROOT.SetBatch()

seed_map = {}
events = Events(files)
handle = Handle('std::vector<unsigned int>')
i = 0
for f in files:
    events = Events(f)
    for event in events:
        event.getByLabel("randomEngineSeedKeeper", handle)
        prod = handle.product()
        seed = prod[0]  # generator seed
        if seed in seed_map:
            print "DUPLICATED: ", seed
        seed_map[seed] = 1
        print f, seed
        break
