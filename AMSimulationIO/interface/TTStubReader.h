#ifndef AMSimulationIO_TTStubReader_h_
#define AMSimulationIO_TTStubReader_h_

#include "TChain.h"
#include "TFile.h"
#include "TFileCollection.h"
#include "TObjArray.h"
#include "TROOT.h"
#include "TString.h"
#include "TTreeFormula.h"
#include <memory>
#include <vector>

namespace slhcl1tt {


// _____________________________________________________________________________
class TTStubReader {
  public:
    TTStubReader(int verbose=1);
    ~TTStubReader();

    int init(TString src, bool full=true);

    TTreeFormula* addFormula(TString formula);

    Long64_t loadTree(Long64_t entry);

    Int_t getEntry(Long64_t entry);

    TChain* getChain() { return tchain; }

    // Stub information
    std::vector<float> *          vb_x;
    std::vector<float> *          vb_y;
    std::vector<float> *          vb_z;
    std::vector<float> *          vb_r;
    std::vector<float> *          vb_eta;
    std::vector<float> *          vb_phi;
    std::vector<float> *          vb_coordx;
    std::vector<float> *          vb_coordy;
    std::vector<float> *          vb_roughPt;
    std::vector<float> *          vb_trigBend;
    std::vector<unsigned> *       vb_modId;
    std::vector<int> *            vb_trkId;

    // genParticle information
    std::vector<float> *          vp_pt;
    std::vector<float> *          vp_eta;
    std::vector<float> *          vp_phi;
    std::vector<float> *          vp_vx;
    std::vector<float> *          vp_vy;
    std::vector<float> *          vp_vz;
    std::vector<int> *            vp_charge;

  private:
    TChain* tchain;
    int treenumber;
    TObjArray ttformulas;
    const int verbose_;
};


// _____________________________________________________________________________
class TTStubCloner {
  public:
    TTStubCloner(int verbose=1);
    ~TTStubCloner();

    int init(TChain* chain, TString out);

    void fill();

    Long64_t write();

  private:
    TFile* tfile;
    TTree* ttree;
    const int verbose_;
};

}  // namespace slhcl1tt

#endif

