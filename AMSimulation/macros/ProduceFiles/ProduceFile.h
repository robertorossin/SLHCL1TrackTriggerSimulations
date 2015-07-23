//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Jan 11 17:29:12 2015 by ROOT version 5.34/07
// from TTree tree/
// found on file: /eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1_ntuple/PU140/TTbarTTbar_E2023TTI_PU140_TuneCUEP6S1_ntuple.2.root
//////////////////////////////////////////////////////////

#ifndef ProduceFile_h
#define ProduceFile_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>
#include <vector>

//#include "utils.C"

// Fixed size dimensions of array or collections stored in the TTree if any.

class ProduceFile {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   vector<float>   *TTStubs_coordx;
   vector<float>   *TTStubs_coordy;
   vector<float>   *TTStubs_trigBend;
   vector<unsigned int> *TTStubs_bxId;
   vector<unsigned int> *TTStubs_modId;
   vector<float>   *genParts_pt;
   vector<float>   *genParts_vz;

   // List of branches
   TBranch        *b_TTStubs_coordx;   //!
   TBranch        *b_TTStubs_coordy;   //!
   TBranch        *b_TTStubs_trigBend;   //!
   TBranch        *b_TTStubs_bxId;   //!
   TBranch        *b_TTStubs_modId;   //!
   TBranch        *b_genParts_pt;   //! 
   TBranch        *b_genParts_vz;   //! 

   ProduceFile(TTree *tree=0);
   virtual ~ProduceFile();
   //virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef ProduceFile_cxx
ProduceFile::ProduceFile(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1_ntuple/PU140/TTbarTTbar_E2023TTI_PU140_TuneCUEP6S1_ntuple.2.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1_ntuple/PU140/TTbarTTbar_E2023TTI_PU140_TuneCUEP6S1_ntuple.2.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("/eos/uscms/store/user/l1upgrades/SLHC/GEN/620_SLHC12p1_ntuple/PU140/TTbarTTbar_E2023TTI_PU140_TuneCUEP6S1_ntuple.2.root:/ntupler");
      dir->GetObject("tree",tree);

   }
   Init(tree);
}

ProduceFile::~ProduceFile()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t ProduceFile::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t ProduceFile::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void ProduceFile::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   TTStubs_coordx = 0;
   TTStubs_coordy = 0;
   TTStubs_trigBend = 0;
   TTStubs_bxId = 0;
	TTStubs_modId = 0;
   genParts_pt = 0;
   genParts_vz = 0;

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("TTStubs_coordx", &TTStubs_coordx, &b_TTStubs_coordx);
   fChain->SetBranchAddress("TTStubs_coordy", &TTStubs_coordy, &b_TTStubs_coordy);
   fChain->SetBranchAddress("TTStubs_trigBend", &TTStubs_trigBend, &b_TTStubs_trigBend);
   //fChain->SetBranchAddress("TTStubs_bxId", &TTStubs_bxId, &b_TTStubs_bxId);
   fChain->SetBranchAddress("TTStubs_modId", &TTStubs_modId, &b_TTStubs_modId);
   //fChain->SetBranchAddress("genParts_pt", &genParts_pt, &b_genParts_pt);
   //fChain->SetBranchAddress("genParts_vz", &genParts_vz, &b_genParts_vz);

   Notify();
}

Bool_t ProduceFile::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void ProduceFile::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
/*Int_t ProduceFile::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}*/

#endif // #ifdef ProduceFile_cxx
