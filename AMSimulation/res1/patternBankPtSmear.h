//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Mar 12 16:26:41 2015 by ROOT version 5.32/02
// from TTree patternBank/
// found on file: /data/rossin/EOS/patternBank_sf1_nz1_tt27_pt3_5.0M.root
//////////////////////////////////////////////////////////

#ifndef patternBankPtSmear_h
#define patternBankPtSmear_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>

// Fixed size dimensions of array or collections stored in the TTree if any.

class patternBankPtSmear {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain
   TTree          *tree;

   // Declaration of leaf types
   UShort_t        frequency;
   Double_t        meanPt ;
   Double_t        rmsPt  ;
   Double_t        meanPhi;
   Double_t        rmsPhi ;
   Int_t           chargeSum;
   std::vector<unsigned int> *superstripIds;

   // Other declarations
   int percentile;

   // List of branches
   TBranch        *b_frequency;   //!
   TBranch        *b_meanPt;   //!
   TBranch        *b_rmsPt;   //!
   TBranch        *b_meanPhi;   //!
   TBranch        *b_rmsPhi;   //!
   TBranch        *b_chargeSum;   //!
   TBranch        *b_superstripIds;   //!

   patternBankPtSmear(TString sTree="");
   virtual ~patternBankPtSmear();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(TString sTree, int mp=0);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   long unsigned int ReadPercentile(TString sTree);
};

#endif

#ifdef patternBankPtSmear_cxx
patternBankPtSmear::patternBankPtSmear(TString sTree) : fChain(0)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (sTree != "") {
  	 sTree=TString("/data/rossin/EOS/")+sTree;
  	 TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(sTree);
      if (!f || !f->IsOpen()) {
         f = new TFile(sTree);
      }
      f->GetObject("patternBank",tree);
   }
   else {
  	 return;
   }
   Init(tree);
}

patternBankPtSmear::~patternBankPtSmear()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t patternBankPtSmear::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t patternBankPtSmear::LoadTree(Long64_t entry)
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

void patternBankPtSmear::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   superstripIds = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("frequency", &frequency, &b_frequency);
   fChain->SetBranchAddress("meanPt", &meanPt, &b_meanPt);
   fChain->SetBranchAddress("rmsPt", &rmsPt, &b_rmsPt);
   fChain->SetBranchAddress("meanPhi", &meanPhi, &b_meanPhi);
   fChain->SetBranchAddress("rmsPhi", &rmsPhi, &b_rmsPhi);
   fChain->SetBranchAddress("chargeSum", &chargeSum, &b_chargeSum);
   fChain->SetBranchAddress("superstripIds", &superstripIds, &b_superstripIds);
   Notify();
}

Bool_t patternBankPtSmear::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void patternBankPtSmear::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t patternBankPtSmear::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef patternBankPtSmear_cxx
