//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Mar 30 13:20:12 2015 by ROOT version 5.34/07
// from TTree newtree/newtree
// found on file: Neutrino_E2023TTI_PU140_TuneCUEP6S1_ntuple.2.8BX.root
//////////////////////////////////////////////////////////

#ifndef MakeHists_h
#define MakeHists_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>

// Fixed size dimensions of array or collections stored in the TTree if any.

class MakeHists {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           TTStubs_size_new;
   vector<unsigned int> *TTStubs_modId_new;
   vector<float>   *TTStubs_trigBend_new;

   // List of branches
   TBranch        *b_TTStubs_size_new;   //!
   TBranch        *b_TTStubs_modId_new;   //!
   TBranch        *b_TTStubs_trigBend_new;   //!

   MakeHists(TTree *tree=0);
   virtual ~MakeHists();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(bool requirePtCut3);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef MakeHists_cxx
MakeHists::MakeHists(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("Neutrino_E2023TTI_PU140_TuneCUEP6S1_ntuple.2.8BX.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("Neutrino_E2023TTI_PU140_TuneCUEP6S1_ntuple.2.8BX.root");
      }
      f->GetObject("newtree",tree);

   }
   Init(tree);
}

MakeHists::~MakeHists()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t MakeHists::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t MakeHists::LoadTree(Long64_t entry)
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

void MakeHists::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   TTStubs_modId_new = 0;
   TTStubs_trigBend_new = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("TTStubs_size_new", &TTStubs_size_new, &b_TTStubs_size_new);
   fChain->SetBranchAddress("TTStubs_modId_new", &TTStubs_modId_new, &b_TTStubs_modId_new);
   fChain->SetBranchAddress("TTStubs_trigBend_new", &TTStubs_trigBend_new, &b_TTStubs_trigBend_new);
   Notify();
}

Bool_t MakeHists::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void MakeHists::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t MakeHists::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef MakeHists_cxx
