#ifndef AMSimulation_PCAMatrix_h_
#define AMSimulation_PCAMatrix_h_

#include "Eigen/Core"
#include "Eigen/Eigenvalues"
#include "Eigen/QR"
#include <string>

namespace slhcl1tt {

class PCAMatrix {
  public:
    Eigen::VectorXd meansR;
    Eigen::VectorXd meansC;
    Eigen::VectorXd meansT;
    Eigen::MatrixXd solutionsC;
    Eigen::MatrixXd solutionsT;

    Eigen::VectorXd sqrtEigenvalues;
    Eigen::VectorXd meansX;
    Eigen::VectorXd meansV;
    Eigen::VectorXd meansP;
    Eigen::MatrixXd V;
    Eigen::MatrixXd D;
    Eigen::MatrixXd DV;

    unsigned nvariables;
    unsigned nparameters;
    std::string filename;

    int read(const std::string txt);

    int write(const std::string txt);

    void print();
};

}  // namespace slhcl1tt

#endif

