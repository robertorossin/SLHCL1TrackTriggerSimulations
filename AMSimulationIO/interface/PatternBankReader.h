#ifndef AMSimulationIO_PatternBankReader_h_
#define AMSimulationIO_PatternBankReader_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulationDataFormats/interface/Pattern.h"

#include "TFile.h"
#include "TROOT.h"
#include "TString.h"
#include "TTree.h"
#include <memory>
#include <string>
#include <vector>

namespace slhcl1tt {


// _____________________________________________________________________________
class PatternBankReader {
  public:
    PatternBankReader(int verbose=1);
    ~PatternBankReader();

    int init(TString src);

    void getPatternBankInfo(float& coverage, unsigned& count, unsigned& tower, std::string& superstrip,
                            unsigned& superstrip_nx, unsigned& superstrip_nz);
    void getPatternInvPt(Long64_t entry, float& invPt_mean);

    Int_t getPattern(Long64_t entry) { return ttree->GetEntry(entry); }

    Long64_t getPatterns() const { return ttree->GetEntries(); }

    // Pattern attributes
    float                          pb_invPt_mean;
    float                          pb_invPt_sigma;
    float                          pb_cotTheta_mean;
    float                          pb_cotTheta_sigma;
    float                          pb_phi_mean;
    float                          pb_phi_sigma;
    float                          pb_z0_mean;
    float                          pb_z0_sigma;

    // Pattern bank statistics
    float                          pb_coverage;
    unsigned                       pb_count;
    unsigned                       pb_tower;
    std::string *                  pb_superstrip;
    unsigned                       pb_superstrip_nx;
    unsigned                       pb_superstrip_nz;

    // Pattern bank
    frequency_type                 pb_frequency;
    std::vector<superstrip_type> * pb_superstripIds;

  protected:
    TFile* tfile;
    TTree* ttree;   // for pattern bank
    TTree* ttree2;  // for pattern bank statistics
    TTree* ttree3;  // for pattern attributes
    const int verbose_;
};


// _____________________________________________________________________________
class PatternBankWriter {
  public:
    PatternBankWriter(int verbose=1);
    ~PatternBankWriter();

    int init(TString out);

    void fillPatternAttributes();

    void fillPatternBankInfo();  // must be called before fillPatternBank()

    void fillPatternBank();

    Long64_t writeTree();

    // Pattern attributes
    std::auto_ptr<float>                         pb_invPt_mean;
    std::auto_ptr<float>                         pb_invPt_sigma;
    std::auto_ptr<float>                         pb_cotTheta_mean;
    std::auto_ptr<float>                         pb_cotTheta_sigma;
    std::auto_ptr<float>                         pb_phi_mean;
    std::auto_ptr<float>                         pb_phi_sigma;
    std::auto_ptr<float>                         pb_z0_mean;
    std::auto_ptr<float>                         pb_z0_sigma;

    // Pattern bank statistics
    std::auto_ptr<float>                         pb_coverage;
    std::auto_ptr<unsigned>                      pb_count;
    std::auto_ptr<unsigned>                      pb_tower;
    std::auto_ptr<std::string>                   pb_superstrip;
    std::auto_ptr<unsigned>                      pb_superstrip_nx;
    std::auto_ptr<unsigned>                      pb_superstrip_nz;

    // Pattern bank
    std::auto_ptr<frequency_type>                pb_frequency;
    std::auto_ptr<std::vector<superstrip_type> > pb_superstripIds;

  protected:
    TFile* tfile;
    TTree* ttree;   // for pattern bank
    TTree* ttree2;  // for pattern bank statistics
    TTree* ttree3;  // for pattern attributes
    const int verbose_;
};

}  // namespace slhcl1tt

#endif
