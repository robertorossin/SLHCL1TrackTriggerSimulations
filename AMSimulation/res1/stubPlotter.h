//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Nov 10 13:33:16 2015 by ROOT version 5.34/30
// from TTree tree/
// found on file: /data/rossin/EOS/Neutrino_PU140_tt27_sf1_nz4_pt3_20151107/roads_Neutrino_PU140_tt27_sf1_nz4_pt3_5oo6_95c_14k.root
//////////////////////////////////////////////////////////

#ifndef stubPlotter_h
#define stubPlotter_h

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

class stubPlotter {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   vector<bool>    *TTClusters_barrel;
   vector<bool>    *TTClusters_isCombinatoric;
   vector<bool>    *TTClusters_isGenuine;
   vector<bool>    *TTClusters_isUnknown;
   vector<bool>    *TTClusters_psmodule;
   vector<bool>    *TTClusters_stack;
   vector<bool>    *TTStubs_barrel;
   vector<bool>    *TTStubs_isCombinatoric;
   vector<bool>    *TTStubs_isGenuine;
   vector<bool>    *TTStubs_isUnknown;
   vector<bool>    *TTStubs_psmodule;
   vector<bool>    *trkParts_intime;
   vector<bool>    *trkParts_primary;
   vector<bool>    *trkParts_signal;
   vector<bool>    *trkVertices_inVolume;
   Float_t         beamSpot_beamWidthX;
   Float_t         beamSpot_beamWidthY;
   Float_t         beamSpot_bx;
   Float_t         beamSpot_by;
   Float_t         beamSpot_bz;
   Float_t         beamSpot_sigmaZ;
   Float_t         gen_trueNPV;
   Float_t         gen_weightMC;
   Float_t         gen_weightPDF;
   Float_t         gen_weightPU;
   vector<float>   *gen_puZpositions;
   vector<float>   *genParts_E;
   vector<float>   *genParts_M;
   vector<float>   *genParts_eta;
   vector<float>   *genParts_phi;
   vector<float>   *genParts_pt;
   vector<float>   *genParts_px;
   vector<float>   *genParts_py;
   vector<float>   *genParts_pz;
   vector<float>   *genParts_vx;
   vector<float>   *genParts_vy;
   vector<float>   *genParts_vz;
   vector<float>   *genParts_cotTheta;
   vector<float>   *genParts_d0;
   vector<float>   *genParts_dz;
   vector<float>   *genParts_invPt;
   vector<float>   *simTracks_charge;
   vector<float>   *simTracks_eta;
   vector<float>   *simTracks_phi;
   vector<float>   *simTracks_pt;
   vector<float>   *simTracks_px;
   vector<float>   *simTracks_py;
   vector<float>   *simTracks_pz;
   vector<float>   *simVertices_tof;
   vector<float>   *simVertices_vx;
   vector<float>   *simVertices_vy;
   vector<float>   *simVertices_vz;
   vector<float>   *TTClusters_coordx;
   vector<float>   *TTClusters_coordy;
   vector<float>   *TTClusters_eta;
   vector<float>   *TTClusters_phi;
   vector<float>   *TTClusters_r;
   vector<float>   *TTClusters_simEta;
   vector<float>   *TTClusters_simPhi;
   vector<float>   *TTClusters_simPt;
   vector<float>   *TTClusters_x;
   vector<float>   *TTClusters_y;
   vector<float>   *TTClusters_z;
   vector<float>   *TTStubs_coordx;
   vector<float>   *TTStubs_coordy;
   vector<float>   *TTStubs_dirx;
   vector<float>   *TTStubs_diry;
   vector<float>   *TTStubs_dirz;
   vector<float>   *TTStubs_eta;
   vector<float>   *TTStubs_phi;
   vector<float>   *TTStubs_r;
   vector<float>   *TTStubs_roughPt;
   vector<float>   *TTStubs_separation;
   vector<float>   *TTStubs_simEta;
   vector<float>   *TTStubs_simPhi;
   vector<float>   *TTStubs_simPt;
   vector<float>   *TTStubs_trigBend;
   vector<float>   *TTStubs_trigDist;
   vector<float>   *TTStubs_trigOffset;
   vector<float>   *TTStubs_trigPos;
   vector<float>   *TTStubs_x;
   vector<float>   *TTStubs_y;
   vector<float>   *TTStubs_z;
   vector<float>   *simPixelDigis_eta;
   vector<float>   *simPixelDigis_frac;
   vector<float>   *simPixelDigis_phi;
   vector<float>   *simPixelDigis_r;
   vector<float>   *simPixelDigis_x;
   vector<float>   *simPixelDigis_y;
   vector<float>   *simPixelDigis_z;
   vector<float>   *trkParts_eta;
   vector<float>   *trkParts_phi;
   vector<float>   *trkParts_pt;
   vector<float>   *trkParts_px;
   vector<float>   *trkParts_py;
   vector<float>   *trkParts_pz;
   vector<float>   *trkParts_vx;
   vector<float>   *trkParts_vy;
   vector<float>   *trkParts_vz;
   vector<float>   *trkVertices_tof;
   vector<float>   *trkVertices_vx;
   vector<float>   *trkVertices_vy;
   vector<float>   *trkVertices_vz;
   vector<vector<float> > *TTStubs_hitXs;
   vector<vector<float> > *TTStubs_hitYs;
   vector<vector<float> > *TTStubs_hitZs;
   Int_t           bx;
   Int_t           orbit;
   Int_t           gen_nPV;
   vector<int>     *gen_puNtrks;
   vector<int>     *genParts_charge;
   vector<int>     *genParts_pdgId;
   vector<int>     *genParts_status;
   vector<int>     *simTracks_genpId;
   vector<int>     *simTracks_pdgId;
   vector<int>     *simTracks_vtxId;
   vector<int>     *simVertices_trkId;
   vector<int>     *TTClusters_pdgId;
   vector<int>     *TTClusters_tpId;
   vector<int>     *TTClusters_trkId;
   vector<int>     *TTStubs_detWindow;
   vector<int>     *TTStubs_pdgId;
   vector<int>     *TTStubs_tpId;
   vector<int>     *TTStubs_trkId;
   vector<int>     *simPixelDigis_adc;
   vector<int>     *simPixelDigis_chan;
   vector<int>     *simPixelDigis_trkId;
   vector<int>     *trkParts_charge;
   vector<int>     *trkParts_genpId;
   vector<int>     *trkParts_nhits;
   vector<int>     *trkParts_ntkhits;
   vector<int>     *trkParts_ntklayers;
   vector<int>     *trkParts_pdgId;
   vector<int>     *trkParts_trkId;
   vector<int>     *trkVertices_vtxId;
   vector<vector<int> > *TTClusters_hitChans;
   vector<vector<int> > *TTClusters_hitTrkIds;
   vector<vector<int> > *TTStubs_hitChans;
   vector<vector<int> > *TTStubs_hitTrkIds;
   UInt_t          event;
   UInt_t          lumi;
   UInt_t          run;
   UInt_t          genParts_size;
   UInt_t          simTracks_size;
   UInt_t          simVertices_size;
   UInt_t          TTClusters_size;
   UInt_t          TTStubs_size;
   UInt_t          simPixelDigis_size;
   UInt_t          trkParts_size;
   UInt_t          trkVertices_size;
   vector<unsigned int> *gen_randomSeeds;
   vector<unsigned int> *simTracks_evtId;
   vector<unsigned int> *simTracks_trkId;
   vector<unsigned int> *simVertices_vtxId;
   vector<unsigned int> *TTClusters_geoId;
   vector<unsigned int> *TTClusters_modId;
   vector<unsigned int> *TTClusters_nhits;
   vector<unsigned int> *TTClusters_stackId;
   vector<unsigned int> *TTClusters_width;
   vector<unsigned int> *TTStubs_clusId0;
   vector<unsigned int> *TTStubs_clusId1;
   vector<unsigned int> *TTStubs_clusWidth0;
   vector<unsigned int> *TTStubs_clusWidth1;
   vector<unsigned int> *TTStubs_geoId0;
   vector<unsigned int> *TTStubs_geoId1;
   vector<unsigned int> *TTStubs_modId;
   vector<unsigned int> *TTStubs_nhits;
   vector<unsigned int> *TTStubs_stackId;
   vector<unsigned int> *simPixelDigis_cfpos;
   vector<unsigned int> *simPixelDigis_evtId;
   vector<unsigned int> *simPixelDigis_geoId;
   vector<unsigned int> *simPixelDigis_modId;
   vector<unsigned int> *trkParts_evtId;
   vector<unsigned int> *trkVertices_evtId;
   vector<vector<unsigned int> > *TTClusters_hitEvtIds;
   vector<vector<unsigned int> > *TTStubs_hitEvtIds;
   vector<vector<unsigned int> > *trkParts_trkIds;
   vector<vector<unsigned int> > *trkVertices_vtxIds;
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
   TBranch        *b_TTClusters_barrel;   //!
   TBranch        *b_TTClusters_isCombinatoric;   //!
   TBranch        *b_TTClusters_isGenuine;   //!
   TBranch        *b_TTClusters_isUnknown;   //!
   TBranch        *b_TTClusters_psmodule;   //!
   TBranch        *b_TTClusters_stack;   //!
   TBranch        *b_TTStubs_barrel;   //!
   TBranch        *b_TTStubs_isCombinatoric;   //!
   TBranch        *b_TTStubs_isGenuine;   //!
   TBranch        *b_TTStubs_isUnknown;   //!
   TBranch        *b_TTStubs_psmodule;   //!
   TBranch        *b_trkParts_intime;   //!
   TBranch        *b_trkParts_primary;   //!
   TBranch        *b_trkParts_signal;   //!
   TBranch        *b_trkVertices_inVolume;   //!
   TBranch        *b_beamSpot_beamWidthX;   //!
   TBranch        *b_beamSpot_beamWidthY;   //!
   TBranch        *b_beamSpot_bx;   //!
   TBranch        *b_beamSpot_by;   //!
   TBranch        *b_beamSpot_bz;   //!
   TBranch        *b_beamSpot_sigmaZ;   //!
   TBranch        *b_gen_trueNPV;   //!
   TBranch        *b_gen_weightMC;   //!
   TBranch        *b_gen_weightPDF;   //!
   TBranch        *b_gen_weightPU;   //!
   TBranch        *b_gen_puZpositions;   //!
   TBranch        *b_genParts_E;   //!
   TBranch        *b_genParts_M;   //!
   TBranch        *b_genParts_eta;   //!
   TBranch        *b_genParts_phi;   //!
   TBranch        *b_genParts_pt;   //!
   TBranch        *b_genParts_px;   //!
   TBranch        *b_genParts_py;   //!
   TBranch        *b_genParts_pz;   //!
   TBranch        *b_genParts_vx;   //!
   TBranch        *b_genParts_vy;   //!
   TBranch        *b_genParts_vz;   //!
   TBranch        *b_genParts_cotTheta;   //!
   TBranch        *b_genParts_d0;   //!
   TBranch        *b_genParts_dz;   //!
   TBranch        *b_genParts_invPt;   //!
   TBranch        *b_simTracks_charge;   //!
   TBranch        *b_simTracks_eta;   //!
   TBranch        *b_simTracks_phi;   //!
   TBranch        *b_simTracks_pt;   //!
   TBranch        *b_simTracks_px;   //!
   TBranch        *b_simTracks_py;   //!
   TBranch        *b_simTracks_pz;   //!
   TBranch        *b_simVertices_tof;   //!
   TBranch        *b_simVertices_vx;   //!
   TBranch        *b_simVertices_vy;   //!
   TBranch        *b_simVertices_vz;   //!
   TBranch        *b_TTClusters_coordx;   //!
   TBranch        *b_TTClusters_coordy;   //!
   TBranch        *b_TTClusters_eta;   //!
   TBranch        *b_TTClusters_phi;   //!
   TBranch        *b_TTClusters_r;   //!
   TBranch        *b_TTClusters_simEta;   //!
   TBranch        *b_TTClusters_simPhi;   //!
   TBranch        *b_TTClusters_simPt;   //!
   TBranch        *b_TTClusters_x;   //!
   TBranch        *b_TTClusters_y;   //!
   TBranch        *b_TTClusters_z;   //!
   TBranch        *b_TTStubs_coordx;   //!
   TBranch        *b_TTStubs_coordy;   //!
   TBranch        *b_TTStubs_dirx;   //!
   TBranch        *b_TTStubs_diry;   //!
   TBranch        *b_TTStubs_dirz;   //!
   TBranch        *b_TTStubs_eta;   //!
   TBranch        *b_TTStubs_phi;   //!
   TBranch        *b_TTStubs_r;   //!
   TBranch        *b_TTStubs_roughPt;   //!
   TBranch        *b_TTStubs_separation;   //!
   TBranch        *b_TTStubs_simEta;   //!
   TBranch        *b_TTStubs_simPhi;   //!
   TBranch        *b_TTStubs_simPt;   //!
   TBranch        *b_TTStubs_trigBend;   //!
   TBranch        *b_TTStubs_trigDist;   //!
   TBranch        *b_TTStubs_trigOffset;   //!
   TBranch        *b_TTStubs_trigPos;   //!
   TBranch        *b_TTStubs_x;   //!
   TBranch        *b_TTStubs_y;   //!
   TBranch        *b_TTStubs_z;   //!
   TBranch        *b_simPixelDigis_eta;   //!
   TBranch        *b_simPixelDigis_frac;   //!
   TBranch        *b_simPixelDigis_phi;   //!
   TBranch        *b_simPixelDigis_r;   //!
   TBranch        *b_simPixelDigis_x;   //!
   TBranch        *b_simPixelDigis_y;   //!
   TBranch        *b_simPixelDigis_z;   //!
   TBranch        *b_trkParts_eta;   //!
   TBranch        *b_trkParts_phi;   //!
   TBranch        *b_trkParts_pt;   //!
   TBranch        *b_trkParts_px;   //!
   TBranch        *b_trkParts_py;   //!
   TBranch        *b_trkParts_pz;   //!
   TBranch        *b_trkParts_vx;   //!
   TBranch        *b_trkParts_vy;   //!
   TBranch        *b_trkParts_vz;   //!
   TBranch        *b_trkVertices_tof;   //!
   TBranch        *b_trkVertices_vx;   //!
   TBranch        *b_trkVertices_vy;   //!
   TBranch        *b_trkVertices_vz;   //!
   TBranch        *b_TTStubs_hitXs;   //!
   TBranch        *b_TTStubs_hitYs;   //!
   TBranch        *b_TTStubs_hitZs;   //!
   TBranch        *b_bx;   //!
   TBranch        *b_orbit;   //!
   TBranch        *b_gen_nPV;   //!
   TBranch        *b_gen_puNtrks;   //!
   TBranch        *b_genParts_charge;   //!
   TBranch        *b_genParts_pdgId;   //!
   TBranch        *b_genParts_status;   //!
   TBranch        *b_simTracks_genpId;   //!
   TBranch        *b_simTracks_pdgId;   //!
   TBranch        *b_simTracks_vtxId;   //!
   TBranch        *b_simVertices_trkId;   //!
   TBranch        *b_TTClusters_pdgId;   //!
   TBranch        *b_TTClusters_tpId;   //!
   TBranch        *b_TTClusters_trkId;   //!
   TBranch        *b_TTStubs_detWindow;   //!
   TBranch        *b_TTStubs_pdgId;   //!
   TBranch        *b_TTStubs_tpId;   //!
   TBranch        *b_TTStubs_trkId;   //!
   TBranch        *b_simPixelDigis_adc;   //!
   TBranch        *b_simPixelDigis_chan;   //!
   TBranch        *b_simPixelDigis_trkId;   //!
   TBranch        *b_trkParts_charge;   //!
   TBranch        *b_trkParts_genpId;   //!
   TBranch        *b_trkParts_nhits;   //!
   TBranch        *b_trkParts_ntkhits;   //!
   TBranch        *b_trkParts_ntklayers;   //!
   TBranch        *b_trkParts_pdgId;   //!
   TBranch        *b_trkParts_trkId;   //!
   TBranch        *b_trkVertices_vtxId;   //!
   TBranch        *b_TTClusters_hitChans;   //!
   TBranch        *b_TTClusters_hitTrkIds;   //!
   TBranch        *b_TTStubs_hitChans;   //!
   TBranch        *b_TTStubs_hitTrkIds;   //!
   TBranch        *b_event;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_run;   //!
   TBranch        *b_genParts_size;   //!
   TBranch        *b_simTracks_size;   //!
   TBranch        *b_simVertices_size;   //!
   TBranch        *b_TTClusters_size;   //!
   TBranch        *b_TTStubs_size;   //!
   TBranch        *b_simPixelDigis_size;   //!
   TBranch        *b_trkParts_size;   //!
   TBranch        *b_trkVertices_size;   //!
   TBranch        *b_gen_randomSeeds;   //!
   TBranch        *b_simTracks_evtId;   //!
   TBranch        *b_simTracks_trkId;   //!
   TBranch        *b_simVertices_vtxId;   //!
   TBranch        *b_TTClusters_geoId;   //!
   TBranch        *b_TTClusters_modId;   //!
   TBranch        *b_TTClusters_nhits;   //!
   TBranch        *b_TTClusters_stackId;   //!
   TBranch        *b_TTClusters_width;   //!
   TBranch        *b_TTStubs_clusId0;   //!
   TBranch        *b_TTStubs_clusId1;   //!
   TBranch        *b_TTStubs_clusWidth0;   //!
   TBranch        *b_TTStubs_clusWidth1;   //!
   TBranch        *b_TTStubs_geoId0;   //!
   TBranch        *b_TTStubs_geoId1;   //!
   TBranch        *b_TTStubs_modId;   //!
   TBranch        *b_TTStubs_nhits;   //!
   TBranch        *b_TTStubs_stackId;   //!
   TBranch        *b_simPixelDigis_cfpos;   //!
   TBranch        *b_simPixelDigis_evtId;   //!
   TBranch        *b_simPixelDigis_geoId;   //!
   TBranch        *b_simPixelDigis_modId;   //!
   TBranch        *b_trkParts_evtId;   //!
   TBranch        *b_trkVertices_evtId;   //!
   TBranch        *b_TTClusters_hitEvtIds;   //!
   TBranch        *b_TTStubs_hitEvtIds;   //!
   TBranch        *b_trkParts_trkIds;   //!
   TBranch        *b_trkVertices_vtxIds;   //!
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

   stubPlotter(TString fName="",TTree *tree=0);
   virtual ~stubPlotter();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(unsigned int iEv,TString pName);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef stubPlotter_cxx
stubPlotter::stubPlotter(TString fName,TTree *tree) : fChain(0)
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

stubPlotter::~stubPlotter()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t stubPlotter::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t stubPlotter::LoadTree(Long64_t entry)
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

void stubPlotter::Init(TTree *tree)
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
   Notify();
}

Bool_t stubPlotter::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void stubPlotter::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t stubPlotter::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef stubPlotter_cxx
