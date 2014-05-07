#ifndef ROOT_TMath
#include "TMath.h"
#endif

#ifndef ROOT_TRandom
#include "TRandom.h"
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


inline double quadsum(double a, double b)
{
    return TMath::Sqrt(a*a + b*b);
}

//______________________________________________________________________________
inline double deltaPhi(double phi1, double phi2)
{
  double result = phi1 - phi2;
  while (result > M_PI) result -= 2.0*M_PI;
  while (result <= -M_PI) result += 2.0*M_PI;
  return result;
}

inline float deltaPhi(float phi1, float phi2)
{
  float result = phi1 - phi2;
  while (result > float(M_PI)) result -= float(2.0*M_PI);
  while (result <= -float(M_PI)) result += float(2.0*M_PI);
  return result;
}

inline double deltaR(double eta1, double phi1, double eta2, double phi2)
{
  double deta = eta1 - eta2;
  double dphi = deltaPhi(phi1, phi2);
  return TMath::Sqrt(deta*deta + dphi*dphi);
}

inline float deltaR(float eta1, float phi1, float eta2, float phi2)
{
  float deta = eta1 - eta2;
  float dphi = deltaPhi(phi1, phi2);
  return TMath::Sqrt(deta*deta + dphi*dphi);
}

inline float alphaT(float pt1, float px1, float py1, float pt2, float px2, float py2)
{
  float sumpt = pt1 + pt2;
  float sumpx = px1 + px2;
  float sumpy = py1 + py2;
  return (pt2 / TMath::Sqrt(sumpt*sumpt - (sumpx*sumpx+sumpy*sumpy) ));
}


