amsim -R -i out.root -o roads.root -B $STORE/620_SLHC10_results/patternBank_ss8_22M.root   -n -1 --bank_nSubLadders 2 --bank_nSubModules 128 --bank_nMisses 1 --maxPatterns 3200000
amsim -R -i out.root -o roads.root -B $STORE/620_SLHC10_results/patternBank_ss16_22M.root  -n -1 --bank_nSubLadders 2 --bank_nSubModules 64  --bank_nMisses 1 --maxPatterns 3200000
amsim -R -i out.root -o roads.root -B $STORE/620_SLHC10_results/patternBank_ss32_22M.root  -n -1 --bank_nSubLadders 2 --bank_nSubModules 32  --bank_nMisses 1 --maxPatterns 3200000
amsim -R -i out.root -o roads.root -B $STORE/620_SLHC10_results/patternBank_ss64_22M.root  -n -1 --bank_nSubLadders 2 --bank_nSubModules 16  --bank_nMisses 1 --maxPatterns 3200000
amsim -R -i out.root -o roads.root -B $STORE/620_SLHC10_results/patternBank_ss128_22M.root -n -1 --bank_nSubLadders 2 --bank_nSubModules 8   --bank_nMisses 1 --maxPatterns 3200000

amsim -R -i out.root -o roads.root -B $STORE/620_SLHC10_results/patternBank_ss32_22M.root  -n -1 --bank_nSubLadders 2 --bank_nSubModules 32  --bank_nMisses 1 --maxPatterns 3200000
amsim -R -i out.root -o roads.root -B $STORE/620_SLHC10_results/patternBank_ss32_22M.root  -n -1 --bank_nSubLadders 2 --bank_nSubModules 32  --bank_nMisses 2 --maxPatterns 3200000
amsim -R -i out.root -o roads.root -B $STORE/620_SLHC10_results/patternBank_ss32_22M.root  -n -1 --bank_nSubLadders 2 --bank_nSubModules 32  --bank_nMisses 3 --maxPatterns 3200000

amsim -R -i out.root -o roads.root -B $STORE/620_SLHC10_results/patternBank_ss32_f1_22M.root  -n -1 --bank_nSubLadders 2 --bank_nSubModules 32  --bank_nMisses 1 --bank_nFakeSuperstrips 1 --maxPatterns 3200000
amsim -R -i out.root -o roads.root -B $STORE/620_SLHC10_results/patternBank_ss32_f2_22M.root  -n -1 --bank_nSubLadders 2 --bank_nSubModules 32  --bank_nMisses 1 --bank_nFakeSuperstrips 2 --maxPatterns 3200000

amsim -R -i out.root -o roads.root -B $STORE/620_SLHC10_results/patternBank_ss32_d1_22M.root  -n -1 --bank_nSubLadders 2 --bank_nSubModules 32  --bank_nMisses 1 --bank_nDCBits 1 --maxPatterns 3200000
amsim -R -i out.root -o roads.root -B $STORE/620_SLHC10_results/patternBank_ss32_d2_22M.root  -n -1 --bank_nSubLadders 2 --bank_nSubModules 32  --bank_nMisses 1 --bank_nDCBits 2 --maxPatterns 3200000
amsim -R -i out.root -o roads.root -B $STORE/620_SLHC10_results/patternBank_ss32_d3_22M.root  -n -1 --bank_nSubLadders 2 --bank_nSubModules 32  --bank_nMisses 1 --bank_nDCBits 3 --maxPatterns 3200000