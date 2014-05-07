RES="crab_0_140508_180531/res"
OUT="/store/user/l1upgrades/SLHC/GEN/620_SLHC10_results"
FNAME="SingleMuPlusMinus_OneOverPt_vz0_20140420.root"

mkdir -p "/eos/uscms/$OUT"
root -l -b -q tadd.C+\(\"root://cmseos:1094/$OUT/$FNAME\",\"$RES\",\".root\",\"results\"\)

