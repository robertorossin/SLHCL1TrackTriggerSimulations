 #include "ReadTree.C"
 void runJobs()
 {
	 TChain * chain = new TChain("ntupler/tree","");
	 //chain->Add("/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1_ntuple/PU140/Neutrino_E2023TTI_PU140_TuneCUEP6S1_ntuple.2.root");
	 chain->Add("/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1_ntuple/PU200/Neutrino_E2023TTI_PU200_TuneCUEP6S1_ntuple.2.root");
	 ReadTree a(chain);
	 a.Loop();
 }
