#ifndef AMSimulation_HelperMath_h_
#define AMSimulation_HelperMath_h_

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


// All these functions are designed to be call-able by TTree
// As such, they are not namespaced
inline double quadsum(double a, double b) {
    return std::sqrt(a*a + b*b);
}

inline float quadsum(float a, float b) {
    return std::sqrt(a*a + b*b);
}


inline double deltaPhi(double phi1, double phi2) {
  double result = phi1 - phi2;
  while (result > M_PI) result -= 2.0*M_PI;
  while (result <= -M_PI) result += 2.0*M_PI;
  return result;
}

inline float deltaPhi(float phi1, float phi2) {
  float result = phi1 - phi2;
  while (result > M_PI) result -= 2.0*M_PI;
  while (result <= -M_PI) result += 2.0*M_PI;
  return result;
}


inline double deltaR(double eta1, double phi1, double eta2, double phi2) {
  double deta = eta1 - eta2;
  double dphi = deltaPhi(phi1, phi2);
  return std::sqrt(deta*deta + dphi*dphi);
}

inline float deltaR(float eta1, float phi1, float eta2, float phi2) {
  float deta = eta1 - eta2;
  float dphi = deltaPhi(phi1, phi2);
  return std::sqrt(deta*deta + dphi*dphi);
}


inline float etaFromTheta(float theta) {
  if (theta < 0.)  theta += M_PI;
  float tanThetaOver2 = std::tan(theta / 2.0);
  if (tanThetaOver2 == 0.0) return 999999.0;
  return -std::log(tanThetaOver2);
}

inline float etaFromRhoZ(float rho, float z) {
  if (rho == 0.0 && z == 0.0) return 0.0;
  if (rho == 0.0) return 999999.0;
  float theta = std::atan2(rho, z);
  return etaFromTheta(theta);
}

#endif

