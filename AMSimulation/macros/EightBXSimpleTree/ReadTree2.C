#define ReadTree2_cxx
#include "ReadTree2.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream> 

void ReadTree2::Loop(TString PUINPUT)
{
//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
	TFile *PileUpFile = TFile::Open(PUINPUT,"READ"); 
	TTree *PileUpTree;
	PileUpFile->GetObject("newtree",PileUpTree);
	vector<float>   *TTStubs_trigBend_pu;
	vector<unsigned int> *TTStubs_modId_pu;
	TBranch        *b_TTStubs_trigBend_pu;
	TBranch        *b_TTStubs_modId_pu;
	PileUpTree->SetBranchAddress("TTStubs_trigBend_new", &TTStubs_trigBend_pu, &b_TTStubs_trigBend_pu);
	PileUpTree->SetBranchAddress("TTStubs_modId_new", &TTStubs_modId_pu, &b_TTStubs_modId_pu);

   TFile *newfile = new TFile("NewTree.root", "RECREATE");
	TTree newtree("newtree","newtree");
   
	Int_t TTStubs_size_new = 0;
	std::vector<unsigned int> TTStubs_modId_new;
	std::vector<float> TTStubs_trigBend_new;

	newtree.Branch("TTStubs_size_new",&TTStubs_size_new,"TTStubs_size_new/I");
	newtree.Branch("TTStubs_modId_new",&TTStubs_modId_new);
	newtree.Branch("TTStubs_trigBend_new",&TTStubs_trigBend_new);

   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;

   for (Long64_t jentry=0; jentry<125;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      const unsigned nstubs = TTStubs_modId->size();
      if (jentry%100==0) std::cout<<"Processing event #"<<jentry<<", nstubs="<<nstubs<<std::endl;
      if (!nstubs) { // skip if no stub
      	continue;
      }    
      if (nstubs > 100000) {
      	std::cout << "Way too many stubs: " << nstubs << std::endl;
      	return;
      }

		Long64_t kentry = PileUpTree->LoadTree(jentry);
		if (kentry < 0) break;
		b_TTStubs_trigBend_pu->GetEntry(kentry);  
		b_TTStubs_modId_pu->GetEntry(kentry);
		const unsigned nstubs_pu = TTStubs_modId_pu->size();

      TTStubs_size_new = nstubs + nstubs_pu;
		std::cout<<nstubs<<" "<<nstubs_pu<<std::endl;
		for (unsigned l=0; l<nstubs; ++l) {    
			TTStubs_modId_new.push_back(TTStubs_modId->at(l));
			TTStubs_trigBend_new.push_back(TTStubs_trigBend->at(l));
		}
      for (unsigned l=0; l<nstubs_pu; ++l) {    
         TTStubs_modId_new.push_back(TTStubs_modId_pu->at(l));
         TTStubs_trigBend_new.push_back(TTStubs_trigBend_pu->at(l));
      }

		std::cout<<"Fill n_new="<<TTStubs_size_new<<std::endl;
		newtree.Fill();
		TTStubs_modId_new.clear();
		TTStubs_trigBend_new.clear();
		TTStubs_size_new=0;
   }
   newfile->Write();
}
