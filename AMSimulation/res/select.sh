INDIR="/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results"
INFILE="SingleMuPlusMinus_OneOverPt_vz0_20140420.root"
OUTDIR="$INDIR"
OUTFILE="SingleMuPlusMinus_OneOverPt_vz0_20140420_cut.root"

root -l -b -q tcut.C+\(\"root://cmseos:1094/$OUTDIR/$OUTFILE\",\"root://cmseos:1094/$INDIR/$INFILE\"\)
