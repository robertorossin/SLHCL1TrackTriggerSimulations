#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/TrackFitterAlgoLTF.h"
using namespace slhcl1tt;

int TrackFitterAlgoLTF::fit(const TTRoadComb& acomb, TTTrack2& atrack) {
    // Arrange the stub coordinates in the format expected by the fitter
    std::vector<double> vars;
    for (unsigned istub=0; istub<acomb.stubs_phi.size(); ++istub) {
        vars.push_back(acomb.stubs_phi.at(istub));
        vars.push_back(acomb.stubs_r.at(istub));
        vars.push_back(acomb.stubs_z.at(istub));
    }

    double normChi2 = linearizedTrackFitter_.fit(vars);
    const std::vector<double>& pars = linearizedTrackFitter_.estimatedPars();
    // TODO: This should be returned by the LinearizedTrackFitter
    int nDof = 8;

    atrack.setTrackParams(pars[0], pars[1], pars[2], pars[3], 0, normChi2*nDof, nDof, 0, 0);

    return 0;
}
