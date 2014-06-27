set N=26000000
amsim -R -i out.root -o roads.root -B $STORE/620_SLHC10_results/patternBank_ss8_40M.root   -n -1 --bank_nSubLadders 2 --bank_nSubModules 128 --bank_nMisses 1 --maxPatterns $N --no-color -v 2
amsim -R -i out.root -o roads.root -B $STORE/620_SLHC10_results/patternBank_ss16_40M.root  -n -1 --bank_nSubLadders 2 --bank_nSubModules 64  --bank_nMisses 1 --maxPatterns $N --no-color -v 2
amsim -R -i out.root -o roads.root -B $STORE/620_SLHC10_results/patternBank_ss32_40M.root  -n -1 --bank_nSubLadders 2 --bank_nSubModules 32  --bank_nMisses 1 --maxPatterns $N --no-color -v 2
amsim -R -i out.root -o roads.root -B $STORE/620_SLHC10_results/patternBank_ss64_40M.root  -n -1 --bank_nSubLadders 2 --bank_nSubModules 16  --bank_nMisses 1 --maxPatterns $N --no-color -v 2
amsim -R -i out.root -o roads.root -B $STORE/620_SLHC10_results/patternBank_ss128_40M.root -n -1 --bank_nSubLadders 2 --bank_nSubModules 8   --bank_nMisses 1 --maxPatterns $N --no-color -v 2
#
#amsim -R -i out.root -o roads.root -B $STORE/620_SLHC10_results/patternBank_ss32_40M.root  -n -1 --bank_nSubLadders 2 --bank_nSubModules 32  --bank_nMisses 1 --maxPatterns $N --no-color -v 2
#amsim -R -i out.root -o roads.root -B $STORE/620_SLHC10_results/patternBank_ss32_40M.root  -n -1 --bank_nSubLadders 2 --bank_nSubModules 32  --bank_nMisses 2 --maxPatterns $N --no-color -v 2
#amsim -R -i out.root -o roads.root -B $STORE/620_SLHC10_results/patternBank_ss32_40M.root  -n -1 --bank_nSubLadders 2 --bank_nSubModules 32  --bank_nMisses 3 --maxPatterns $N --no-color -v 2
#
#amsim -R -i out.root -o roads.root -B $STORE/620_SLHC10_results/patternBank_ss32_f1_40M.root  -n -1 --bank_nSubLadders 2 --bank_nSubModules 32  --bank_nMisses 1 --bank_nFakeSuperstrips 1 --maxPatterns $N --no-color -v 2
#amsim -R -i out.root -o roads.root -B $STORE/620_SLHC10_results/patternBank_ss32_f2_40M.root  -n -1 --bank_nSubLadders 2 --bank_nSubModules 32  --bank_nMisses 1 --bank_nFakeSuperstrips 2 --maxPatterns $N --no-color -v 2
#
#amsim -R -i out.root -o roads.root -B $STORE/620_SLHC10_results/patternBank_ss32_d1_40M.root  -n -1 --bank_nSubLadders 2 --bank_nSubModules 32  --bank_nMisses 1 --bank_nDCBits 1 --maxPatterns $N --no-color -v 2
#amsim -R -i out.root -o roads.root -B $STORE/620_SLHC10_results/patternBank_ss32_d2_40M.root  -n -1 --bank_nSubLadders 2 --bank_nSubModules 32  --bank_nMisses 1 --bank_nDCBits 2 --maxPatterns $N --no-color -v 2
#amsim -R -i out.root -o roads.root -B $STORE/620_SLHC10_results/patternBank_ss32_d3_40M.root  -n -1 --bank_nSubLadders 2 --bank_nSubModules 32  --bank_nMisses 1 --bank_nDCBits 3 --maxPatterns $N --no-color -v 2
