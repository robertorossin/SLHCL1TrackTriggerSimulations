//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Mar  2 10:52:28 2016 by ROOT version 5.34/34
// from TTree tree/
// found on file: /data/rossin/EOS/SingleMuonTest_PU0_tt27_sf1_nz4_pt3_ml5_20150511/tracks_LTF_SingleMuonTest_PU0_tt27_sf1_nz4_pt3_5oo6_20150511.root
//////////////////////////////////////////////////////////

#ifndef roadAnalysis_h
#define roadAnalysis_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>
#include <vector>
#include <vector>
#include <vector>

// Fixed size dimensions of array or collections stored in the TTree if any.
using namespace std;

class roadAnalysis {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   vector<bool>    *trkParts_intime;
   vector<bool>    *trkParts_primary;
   vector<bool>    *trkParts_signal;
   vector<float>   *genParts_eta;
   vector<float>   *genParts_phi;
   vector<float>   *genParts_pt;
   vector<float>   *genParts_vx;
   vector<float>   *genParts_vy;
   vector<float>   *genParts_vz;
   vector<float>   *TTStubs_coordx;
   vector<float>   *TTStubs_coordy;
   vector<float>   *TTStubs_eta;
   vector<float>   *TTStubs_phi;
   vector<float>   *TTStubs_r;
   vector<float>   *TTStubs_roughPt;
   vector<float>   *TTStubs_trigBend;
   vector<float>   *TTStubs_x;
   vector<float>   *TTStubs_y;
   vector<float>   *TTStubs_z;
   vector<float>   *trkParts_eta;
   vector<float>   *trkParts_phi;
   vector<float>   *trkParts_pt;
   vector<float>   *trkParts_vx;
   vector<float>   *trkParts_vy;
   vector<float>   *trkParts_vz;
   vector<int>     *genParts_charge;
   vector<int>     *TTStubs_tpId;
   vector<int>     *trkParts_charge;
   vector<int>     *trkParts_pdgId;
   vector<unsigned int> *TTStubs_clusWidth0;
   vector<unsigned int> *TTStubs_clusWidth1;
   vector<unsigned int> *TTStubs_modId;
   vector<unsigned int> *AMTTRoads_patternRef;
   vector<unsigned int> *AMTTRoads_tower;
   vector<unsigned int> *AMTTRoads_nstubs;
   vector<float>   *AMTTRoads_patternInvPt;
   vector<vector<unsigned int> > *AMTTRoads_superstripIds;
   vector<vector<vector<unsigned int> > > *AMTTRoads_stubRefs;
   vector<float>   *AMTTTracks_pt;
   vector<float>   *AMTTTracks_eta;
   vector<float>   *AMTTTracks_rinv;
   vector<float>   *AMTTTracks_invPt;
   vector<float>   *AMTTTracks_phi0;
   vector<float>   *AMTTTracks_cottheta;
   vector<float>   *AMTTTracks_z0;
   vector<float>   *AMTTTracks_d0;
   vector<float>   *AMTTTracks_chi2;
   vector<int>     *AMTTTracks_ndof;
   vector<float>   *AMTTTracks_chi2_phi;
   vector<float>   *AMTTTracks_chi2_z;
   vector<bool>    *AMTTTracks_isGhost;
   vector<int>     *AMTTTracks_tpId;
   vector<int>     *AMTTTracks_synTpId;
   vector<unsigned int> *AMTTTracks_tower;
   vector<unsigned int> *AMTTTracks_hitBits;
   vector<unsigned int> *AMTTTracks_ptSegment;
   vector<unsigned int> *AMTTTracks_roadRef;
   vector<unsigned int> *AMTTTracks_combRef;
   vector<unsigned int> *AMTTTracks_patternRef;
   vector<vector<unsigned int> > *AMTTTracks_stubRefs;
   vector<vector<float> > *AMTTTracks_principals;

   // List of branches
   TBranch        *b_trkParts_intime;   //!
   TBranch        *b_trkParts_primary;   //!
   TBranch        *b_trkParts_signal;   //!
   TBranch        *b_genParts_eta;   //!
   TBranch        *b_genParts_phi;   //!
   TBranch        *b_genParts_pt;   //!
   TBranch        *b_genParts_vx;   //!
   TBranch        *b_genParts_vy;   //!
   TBranch        *b_genParts_vz;   //!
   TBranch        *b_TTStubs_coordx;   //!
   TBranch        *b_TTStubs_coordy;   //!
   TBranch        *b_TTStubs_eta;   //!
   TBranch        *b_TTStubs_phi;   //!
   TBranch        *b_TTStubs_r;   //!
   TBranch        *b_TTStubs_roughPt;   //!
   TBranch        *b_TTStubs_trigBend;   //!
   TBranch        *b_TTStubs_x;   //!
   TBranch        *b_TTStubs_y;   //!
   TBranch        *b_TTStubs_z;   //!
   TBranch        *b_trkParts_eta;   //!
   TBranch        *b_trkParts_phi;   //!
   TBranch        *b_trkParts_pt;   //!
   TBranch        *b_trkParts_vx;   //!
   TBranch        *b_trkParts_vy;   //!
   TBranch        *b_trkParts_vz;   //!
   TBranch        *b_genParts_charge;   //!
   TBranch        *b_TTStubs_tpId;   //!
   TBranch        *b_trkParts_charge;   //!
   TBranch        *b_trkParts_pdgId;   //!
   TBranch        *b_TTStubs_clusWidth0;   //!
   TBranch        *b_TTStubs_clusWidth1;   //!
   TBranch        *b_TTStubs_modId;   //!
   TBranch        *b_AMTTRoads_patternRef;   //!
   TBranch        *b_AMTTRoads_tower;   //!
   TBranch        *b_AMTTRoads_nstubs;   //!
   TBranch        *b_AMTTRoads_patternInvPt;   //!
   TBranch        *b_AMTTRoads_superstripIds;   //!
   TBranch        *b_AMTTRoads_stubRefs;   //!
   TBranch        *b_AMTTTracks_pt;   //!
   TBranch        *b_AMTTTracks_eta;   //!
   TBranch        *b_AMTTTracks_rinv;   //!
   TBranch        *b_AMTTTracks_invPt;   //!
   TBranch        *b_AMTTTracks_phi0;   //!
   TBranch        *b_AMTTTracks_cottheta;   //!
   TBranch        *b_AMTTTracks_z0;   //!
   TBranch        *b_AMTTTracks_d0;   //!
   TBranch        *b_AMTTTracks_chi2;   //!
   TBranch        *b_AMTTTracks_ndof;   //!
   TBranch        *b_AMTTTracks_chi2_phi;   //!
   TBranch        *b_AMTTTracks_chi2_z;   //!
   TBranch        *b_AMTTTracks_isGhost;   //!
   TBranch        *b_AMTTTracks_tpId;   //!
   TBranch        *b_AMTTTracks_synTpId;   //!
   TBranch        *b_AMTTTracks_tower;   //!
   TBranch        *b_AMTTTracks_hitBits;   //!
   TBranch        *b_AMTTTracks_ptSegment;   //!
   TBranch        *b_AMTTTracks_roadRef;   //!
   TBranch        *b_AMTTTracks_combRef;   //!
   TBranch        *b_AMTTTracks_patternRef;   //!
   TBranch        *b_AMTTTracks_stubRefs;   //!
   TBranch        *b_AMTTTracks_principals;   //!

   roadAnalysis(TString fName="",TTree *tree=0);
   virtual ~roadAnalysis();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(unsigned int iEv,TString pName, bool makePlots);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef roadAnalysis_cxx
roadAnalysis::roadAnalysis(TString fName,TTree *tree) : fChain(0)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(fName);
      if (!f || !f->IsOpen()) {
         f = new TFile(fName);
      }
      TDirectory * dir = (TDirectory*)f->Get(fName+TString(":/ntupler"));
      dir->GetObject("tree",tree);

   }
   Init(tree);
}

roadAnalysis::~roadAnalysis()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t roadAnalysis::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t roadAnalysis::LoadTree(Long64_t entry)
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

void roadAnalysis::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   trkParts_intime = 0;
   trkParts_primary = 0;
   trkParts_signal = 0;
   genParts_eta = 0;
   genParts_phi = 0;
   genParts_pt = 0;
   genParts_vx = 0;
   genParts_vy = 0;
   genParts_vz = 0;
   TTStubs_coordx = 0;
   TTStubs_coordy = 0;
   TTStubs_eta = 0;
   TTStubs_phi = 0;
   TTStubs_r = 0;
   TTStubs_roughPt = 0;
   TTStubs_trigBend = 0;
   TTStubs_x = 0;
   TTStubs_y = 0;
   TTStubs_z = 0;
   trkParts_eta = 0;
   trkParts_phi = 0;
   trkParts_pt = 0;
   trkParts_vx = 0;
   trkParts_vy = 0;
   trkParts_vz = 0;
   genParts_charge = 0;
   TTStubs_tpId = 0;
   trkParts_charge = 0;
   trkParts_pdgId = 0;
   TTStubs_clusWidth0 = 0;
   TTStubs_clusWidth1 = 0;
   TTStubs_modId = 0;
   AMTTRoads_patternRef = 0;
   AMTTRoads_tower = 0;
   AMTTRoads_nstubs = 0;
   AMTTRoads_patternInvPt = 0;
   AMTTRoads_superstripIds = 0;
   AMTTRoads_stubRefs = 0;
   AMTTTracks_pt = 0;
   AMTTTracks_eta = 0;
   AMTTTracks_rinv = 0;
   AMTTTracks_invPt = 0;
   AMTTTracks_phi0 = 0;
   AMTTTracks_cottheta = 0;
   AMTTTracks_z0 = 0;
   AMTTTracks_d0 = 0;
   AMTTTracks_chi2 = 0;
   AMTTTracks_ndof = 0;
   AMTTTracks_chi2_phi = 0;
   AMTTTracks_chi2_z = 0;
   AMTTTracks_isGhost = 0;
   AMTTTracks_tpId = 0;
   AMTTTracks_synTpId = 0;
   AMTTTracks_tower = 0;
   AMTTTracks_hitBits = 0;
   AMTTTracks_ptSegment = 0;
   AMTTTracks_roadRef = 0;
   AMTTTracks_combRef = 0;
   AMTTTracks_patternRef = 0;
   AMTTTracks_stubRefs = 0;
   AMTTTracks_principals = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("trkParts_intime", &trkParts_intime, &b_trkParts_intime);
   fChain->SetBranchAddress("trkParts_primary", &trkParts_primary, &b_trkParts_primary);
   fChain->SetBranchAddress("trkParts_signal", &trkParts_signal, &b_trkParts_signal);
   fChain->SetBranchAddress("genParts_eta", &genParts_eta, &b_genParts_eta);
   fChain->SetBranchAddress("genParts_phi", &genParts_phi, &b_genParts_phi);
   fChain->SetBranchAddress("genParts_pt", &genParts_pt, &b_genParts_pt);
   fChain->SetBranchAddress("genParts_vx", &genParts_vx, &b_genParts_vx);
   fChain->SetBranchAddress("genParts_vy", &genParts_vy, &b_genParts_vy);
   fChain->SetBranchAddress("genParts_vz", &genParts_vz, &b_genParts_vz);
   fChain->SetBranchAddress("TTStubs_coordx", &TTStubs_coordx, &b_TTStubs_coordx);
   fChain->SetBranchAddress("TTStubs_coordy", &TTStubs_coordy, &b_TTStubs_coordy);
   fChain->SetBranchAddress("TTStubs_eta", &TTStubs_eta, &b_TTStubs_eta);
   fChain->SetBranchAddress("TTStubs_phi", &TTStubs_phi, &b_TTStubs_phi);
   fChain->SetBranchAddress("TTStubs_r", &TTStubs_r, &b_TTStubs_r);
   fChain->SetBranchAddress("TTStubs_roughPt", &TTStubs_roughPt, &b_TTStubs_roughPt);
   fChain->SetBranchAddress("TTStubs_trigBend", &TTStubs_trigBend, &b_TTStubs_trigBend);
   fChain->SetBranchAddress("TTStubs_x", &TTStubs_x, &b_TTStubs_x);
   fChain->SetBranchAddress("TTStubs_y", &TTStubs_y, &b_TTStubs_y);
   fChain->SetBranchAddress("TTStubs_z", &TTStubs_z, &b_TTStubs_z);
   fChain->SetBranchAddress("trkParts_eta", &trkParts_eta, &b_trkParts_eta);
   fChain->SetBranchAddress("trkParts_phi", &trkParts_phi, &b_trkParts_phi);
   fChain->SetBranchAddress("trkParts_pt", &trkParts_pt, &b_trkParts_pt);
   fChain->SetBranchAddress("trkParts_vx", &trkParts_vx, &b_trkParts_vx);
   fChain->SetBranchAddress("trkParts_vy", &trkParts_vy, &b_trkParts_vy);
   fChain->SetBranchAddress("trkParts_vz", &trkParts_vz, &b_trkParts_vz);
   fChain->SetBranchAddress("genParts_charge", &genParts_charge, &b_genParts_charge);
   fChain->SetBranchAddress("TTStubs_tpId", &TTStubs_tpId, &b_TTStubs_tpId);
   fChain->SetBranchAddress("trkParts_charge", &trkParts_charge, &b_trkParts_charge);
   fChain->SetBranchAddress("trkParts_pdgId", &trkParts_pdgId, &b_trkParts_pdgId);
   fChain->SetBranchAddress("TTStubs_clusWidth0", &TTStubs_clusWidth0, &b_TTStubs_clusWidth0);
   fChain->SetBranchAddress("TTStubs_clusWidth1", &TTStubs_clusWidth1, &b_TTStubs_clusWidth1);
   fChain->SetBranchAddress("TTStubs_modId", &TTStubs_modId, &b_TTStubs_modId);
   fChain->SetBranchAddress("AMTTRoads_patternRef", &AMTTRoads_patternRef, &b_AMTTRoads_patternRef);
   fChain->SetBranchAddress("AMTTRoads_tower", &AMTTRoads_tower, &b_AMTTRoads_tower);
   fChain->SetBranchAddress("AMTTRoads_nstubs", &AMTTRoads_nstubs, &b_AMTTRoads_nstubs);
   fChain->SetBranchAddress("AMTTRoads_patternInvPt", &AMTTRoads_patternInvPt, &b_AMTTRoads_patternInvPt);
   fChain->SetBranchAddress("AMTTRoads_superstripIds", &AMTTRoads_superstripIds, &b_AMTTRoads_superstripIds);
   fChain->SetBranchAddress("AMTTRoads_stubRefs", &AMTTRoads_stubRefs, &b_AMTTRoads_stubRefs);
   fChain->SetBranchAddress("AMTTTracks_pt", &AMTTTracks_pt, &b_AMTTTracks_pt);
   fChain->SetBranchAddress("AMTTTracks_eta", &AMTTTracks_eta, &b_AMTTTracks_eta);
   fChain->SetBranchAddress("AMTTTracks_rinv", &AMTTTracks_rinv, &b_AMTTTracks_rinv);
   fChain->SetBranchAddress("AMTTTracks_invPt", &AMTTTracks_invPt, &b_AMTTTracks_invPt);
   fChain->SetBranchAddress("AMTTTracks_phi0", &AMTTTracks_phi0, &b_AMTTTracks_phi0);
   fChain->SetBranchAddress("AMTTTracks_cottheta", &AMTTTracks_cottheta, &b_AMTTTracks_cottheta);
   fChain->SetBranchAddress("AMTTTracks_z0", &AMTTTracks_z0, &b_AMTTTracks_z0);
   fChain->SetBranchAddress("AMTTTracks_d0", &AMTTTracks_d0, &b_AMTTTracks_d0);
   fChain->SetBranchAddress("AMTTTracks_chi2", &AMTTTracks_chi2, &b_AMTTTracks_chi2);
   fChain->SetBranchAddress("AMTTTracks_ndof", &AMTTTracks_ndof, &b_AMTTTracks_ndof);
   fChain->SetBranchAddress("AMTTTracks_chi2_phi", &AMTTTracks_chi2_phi, &b_AMTTTracks_chi2_phi);
   fChain->SetBranchAddress("AMTTTracks_chi2_z", &AMTTTracks_chi2_z, &b_AMTTTracks_chi2_z);
   fChain->SetBranchAddress("AMTTTracks_isGhost", &AMTTTracks_isGhost, &b_AMTTTracks_isGhost);
   fChain->SetBranchAddress("AMTTTracks_tpId", &AMTTTracks_tpId, &b_AMTTTracks_tpId);
   fChain->SetBranchAddress("AMTTTracks_synTpId", &AMTTTracks_synTpId, &b_AMTTTracks_synTpId);
   fChain->SetBranchAddress("AMTTTracks_tower", &AMTTTracks_tower, &b_AMTTTracks_tower);
   fChain->SetBranchAddress("AMTTTracks_hitBits", &AMTTTracks_hitBits, &b_AMTTTracks_hitBits);
   fChain->SetBranchAddress("AMTTTracks_ptSegment", &AMTTTracks_ptSegment, &b_AMTTTracks_ptSegment);
   fChain->SetBranchAddress("AMTTTracks_roadRef", &AMTTTracks_roadRef, &b_AMTTTracks_roadRef);
   fChain->SetBranchAddress("AMTTTracks_combRef", &AMTTTracks_combRef, &b_AMTTTracks_combRef);
   fChain->SetBranchAddress("AMTTTracks_patternRef", &AMTTTracks_patternRef, &b_AMTTTracks_patternRef);
   fChain->SetBranchAddress("AMTTTracks_stubRefs", &AMTTTracks_stubRefs, &b_AMTTTracks_stubRefs);
   fChain->SetBranchAddress("AMTTTracks_principals", &AMTTTracks_principals, &b_AMTTTracks_principals);
   Notify();
}

Bool_t roadAnalysis::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void roadAnalysis::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t roadAnalysis::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef roadAnalysis_cxx
