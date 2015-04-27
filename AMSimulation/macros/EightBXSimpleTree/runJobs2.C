 #include "ReadTree2.C"
 void runJobs2()
 {
	 TChain * chain = new TChain("ntupler/tree","");
	 //chain->Add("/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1_ntuple/PU140/TTbarTTbar_E2023TTI_PU140_TuneCUEP6S1_ntuple.2.root");
	 chain->Add("/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1_ntuple/PU200/TTbarTTbar_E2023TTI_PU200_TuneCUEP6S1_ntuple.2.root");
	 ReadTree2 a(chain);
	 //a.Loop("Neutrino_E2023TTI_PU140_TuneCUEP6S1_ntuple.2.7BX.root");
	 a.Loop("Neutrino_E2023TTI_PU200_TuneCUEP6S1_ntuple.2.7BX.root");
 }
