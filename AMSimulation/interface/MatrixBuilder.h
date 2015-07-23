#ifndef AMSimulation_MatrixBuilder_h_
#define AMSimulation_MatrixBuilder_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/Helper.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/ProgramOption.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TrackFitterAlgoPCA.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TriggerTowerMap.h"
using namespace slhcl1tt;

#include "TH1F.h"
#include "TString.h"

namespace slhcl1tt {
class TTStubReader;
}

class MatrixBuilder {
  public:
    // Constructor
    MatrixBuilder(const ProgramOption& po)
    : po_(po),
      nEvents_(po.maxEvents), verbose_(po.verbose),
      view_(XYZ), nvariables_(12), nparameters_(4) {

        // Determine # of variables and # of parameters
        if (po.view == "XYZ") {
            view_ = XYZ;
            nvariables_ = 6 * 2;
            if (po.algo == "PCA4") {
                nparameters_ = 4;
            } else if (po.algo == "PCA5") {
                nparameters_ = 5;
            }

        } else if (po.view == "XY") {
            view_ = XY;
            nvariables_ = 6;
            if (po.algo == "PCA4") {
                nparameters_ = 2;
            } else if (po.algo == "PCA5") {
                nparameters_ = 3;
            }

        } else if (po.view == "RZ") {
            view_ = RZ;
            nvariables_ = 6;
            nparameters_ = 2;
        }

        meansR_ = Eigen::VectorXd::Zero(6);
        meansR_ << 22.5913, 35.4772, 50.5402, 68.3101, 88.5002, 107.71;

        // Initialize
        ttmap_ = new TriggerTowerMap();
        ttmap_->read(po_.datadir);
    }

    // Destructor
    ~MatrixBuilder() {
        if (ttmap_)     delete ttmap_;
    }

    // Main driver
    int run();


  private:
    // Member functions

    // Book histograms
    int bookHistograms();

    // Set certain variables to zero
    int setVariableToZero(Eigen::VectorXd& variables1, Eigen::VectorXd& variables2, Eigen::VectorXd& variables3, const unsigned hitBits);
    int setRotationToZero(Eigen::MatrixXd& rotation, const unsigned nvariables, const unsigned hitBits);
    int setCovarianceToUnit(Eigen::MatrixXd& covariances, const unsigned nvariables, const unsigned hitBits);

    // Build matrices
    int buildMatrices(TString src);

    int loopEventsAndFilter(TTStubReader& reader);

    int loopEventsAndSolveCT(TTStubReader& reader);

    int loopEventsAndSolveEigenvectors(TTStubReader& reader);

    int loopEventsAndSolveD(TTStubReader& reader);

    int loopEventsAndEval(TTStubReader& reader);

    // Write matrices
    int writeMatrices(TString out);
    int writeHistograms(TString out);

    // Program options
    const ProgramOption po_;
    long long nEvents_;
    int verbose_;

    // Operators
    TriggerTowerMap   * ttmap_;

    // Settings
    FitView view_;
    unsigned nvariables_;   // number of hit coordinates or principal components
    unsigned nparameters_;  // number of track parameters

    // Matrices
    Eigen::VectorXd meansR_;
    PCAMatrix mat_;

    // Event filter decisions
    std::vector<bool> keepEvents_;

    // Histograms
    std::map<TString, TH1F *>  histograms_;
};

#endif
