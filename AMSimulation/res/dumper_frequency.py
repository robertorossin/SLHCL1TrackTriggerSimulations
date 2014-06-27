from ROOT import TFile, TH1F

eos = "/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results/"
bank = "patternBank_ss%s_40M.root"

nbins = 50
values = []
#for ssSize in [32]:
for ssSize in [8,16,32,64,128]:
    infilename = eos + bank % ssSize
    infile = TFile.Open(infilename)

    freq = TH1F("freq", "frequency", nbins, 0, nbins)
    infile.patternBank.Project("freq", "frequency", "", "goff")

    values_inner = []
    for i in xrange(nbins):
        values_inner.append(freq.GetBinContent(i+1))
    values.append(values_inner)

print values