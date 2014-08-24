from ROOT import TFile

txt = "/uscms_data/d2/jiafu/L1TrackTrigger/CRAB_amsim/input_MinBias_20140821.txt"
treename = "ntupler/tree"
nfiles = 1000

with open(txt) as f:
    i = 0
    nentries = 0
    nentries_all = 0
    for line in f.readlines():
        if i == nfiles:
            break
        fname = line.strip()
        if not fname.startswith("#") and fname.endswith(".root"):
            tfile = TFile.Open(fname)
            ttree = tfile.Get(treename)
            nentries = ttree.GetEntries()
            nentries_all += nentries
            print "%d %d %d" % (i, nentries, nentries_all)
        i += 1
