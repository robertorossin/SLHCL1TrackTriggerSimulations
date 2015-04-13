#ifndef AMSimulation_TrackFitterAlgoPCA_h_
#define AMSimulation_TrackFitterAlgoPCA_h_

#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TrackFitterAlgoBase.h"
using namespace slhcl1tt;

#include "SLHCL1TrackTriggerSimulations/AMSimulation/external/Eigen/Core"


class TrackFitterAlgoPCA : public TrackFitterAlgoBase {
  public:
    TrackFitterAlgoPCA(bool fiveParameters=false)
    : TrackFitterAlgoBase(), fiveParameters_(fiveParameters) {

        bookHistograms();
    }

    ~TrackFitterAlgoPCA() {}

    int bookHistograms();

    int loadConstants(TString txt);

    int fit(const std::vector<TTHit>& hits, TTTrack2& track);

    void print();

  private:
    bool fiveParameters_;
    Eigen::VectorXd sqrtEigenvalues_;
    Eigen::MatrixXd D_;
    Eigen::MatrixXd V_;
    Eigen::MatrixXd DV_;
};

#endif

