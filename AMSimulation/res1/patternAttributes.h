//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jun 29 14:22:12 2015 by ROOT version 5.34/30
// from TTree patternAttributes/
// found on file: /data/rossin/EOS/SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/patternBank_SingleMuonFlatOneOverPt0p0005To0p5_tt27_sf1_nz4_pt2_SLHC25p3_100M.root
//////////////////////////////////////////////////////////

#ifndef patternAttributes_h
#define patternAttributes_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class patternAttributes {
public :
	TTree          *fChain;   //!pointer to the analyzed TTree or TChain
	Int_t           fCurrent; //!current Tree number in a TChain
	TTree          *fChainAttr;   //!pointer to the analyzed TTree or TChain
	Int_t           fCurrentAttr; //!current Tree number in a TChain
  TTree          *tree;
  TTree          *treeAttr;

   // Declaration of leaf types
   Float_t         invPt_mean;
   Float_t         invPt_sigma;
   Float_t         cotTheta_mean;
   Float_t         cotTheta_sigma;
   Float_t         phi_mean;
   Float_t         phi_sigma;
   Float_t         z0_mean;
   Float_t         z0_sigma;
   // Declaration of leaf types in patternBank tree
   UShort_t        frequency;
   std::vector<unsigned int> *superstripIds;


   // List of branches
   TBranch        *b_invPt_mean;   //!
   TBranch        *b_invPt_sigma;   //!
   TBranch        *b_cotTheta_mean;   //!
   TBranch        *b_cotTheta_sigma;   //!
   TBranch        *b_phi_mean;   //!
   TBranch        *b_phi_sigma;   //!
   TBranch        *b_z0_mean;   //!
   TBranch        *b_z0_sigma;   //!

   // List of branches in patternBank tree
   TBranch        *b_frequency;   //!
   TBranch        *b_superstripIds;   //!
   // Other declarations
   int percentile;

   patternAttributes(TString sfile);
   virtual ~patternAttributes();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Int_t    GetEntryAttr(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual Long64_t LoadTreeAttr(Long64_t entry);
   virtual void     Init(TTree *treeAttr, TTree *tree);
   virtual void     Loop(TString sTree, int makePlots);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   long unsigned int ReadPercentile(TString sTree,TString &pName);
};

#endif

#ifdef patternAttributes_cxx
patternAttributes::patternAttributes(TString sfile) : fChain(0)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (sfile != "") {
  	 sfile=TString("/data/rossin/EOS/")+sfile;
  	 TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(sfile);
      if (!f || !f->IsOpen()) {
         f = new TFile(sfile);
      }
      f->GetObject("patternAttributes",treeAttr);
      f->GetObject("patternBank"      ,tree    );
   }
   else {
  	 return;
   }
   Init(treeAttr, tree);
}

//patternAttributes::patternAttributes(TTree *treeAttr, TTree *tree) : fChain(0)
//{
//// if parameter tree is not specified (or zero), connect the file
//// used to generate this class and read the Tree.
//   if (treeAttr == 0) {
//      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/data/rossin/EOS/SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/patternBank_SingleMuonFlatOneOverPt0p0005To0p5_tt27_sf1_nz4_pt2_SLHC25p3_100M.root");
//      if (!f || !f->IsOpen()) {
//         f = new TFile("/data/rossin/EOS/SingleMuonTest_tt27_PU0_140_20150408_SLHC25p3_NewPatterns/patternBank_SingleMuonFlatOneOverPt0p0005To0p5_tt27_sf1_nz4_pt2_SLHC25p3_100M.root");
//      }
//      f->GetObject("patternAttributes",treeAttr);
//      f->GetObject("patternBank"      ,tree    );
//
//   }
//   Init(treeAttr, tree);
//}

patternAttributes::~patternAttributes()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t patternAttributes::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Int_t patternAttributes::GetEntryAttr(Long64_t entry)
{
// Read contents of entry.
   if (!fChainAttr) return 0;
   return fChainAttr->GetEntry(entry);
}
Long64_t patternAttributes::LoadTree(Long64_t entry)
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
Long64_t patternAttributes::LoadTreeAttr(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChainAttr) return -5;
   Long64_t centry = fChainAttr->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChainAttr->GetTreeNumber() != fCurrentAttr) {
      fCurrentAttr = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void patternAttributes::Init(TTree *treeAttr, TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!treeAttr) return;
   fChainAttr = treeAttr;
   fCurrent = -1;
   fChainAttr->SetMakeClass(1);

   fChainAttr->SetBranchAddress("invPt_mean", &invPt_mean, &b_invPt_mean);
   fChainAttr->SetBranchAddress("invPt_sigma", &invPt_sigma, &b_invPt_sigma);
   fChainAttr->SetBranchAddress("cotTheta_mean", &cotTheta_mean, &b_cotTheta_mean);
   fChainAttr->SetBranchAddress("cotTheta_sigma", &cotTheta_sigma, &b_cotTheta_sigma);
   fChainAttr->SetBranchAddress("phi_mean", &phi_mean, &b_phi_mean);
   fChainAttr->SetBranchAddress("phi_sigma", &phi_sigma, &b_phi_sigma);
   fChainAttr->SetBranchAddress("z0_mean", &z0_mean, &b_z0_mean);
   fChainAttr->SetBranchAddress("z0_sigma", &z0_sigma, &b_z0_sigma);

   // Set object pointer
   superstripIds = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("frequency", &frequency, &b_frequency);
   fChain->SetBranchAddress("superstripIds", &superstripIds, &b_superstripIds);

   Notify();
}

Bool_t patternAttributes::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void patternAttributes::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t patternAttributes::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef patternAttributes_cxx
