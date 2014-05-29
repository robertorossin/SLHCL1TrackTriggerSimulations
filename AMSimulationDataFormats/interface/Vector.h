#ifndef AMSimulationDataFormats_Vector_h_
#define AMSimulationDataFormats_Vector_h_


// copied from DataFormats/Math/interface/Point3D.h to avoid dependency for now
#include "Math/Point3D.h"
#include "Math/GenVector/CoordinateSystemTags.h"
namespace slhcl1tt {
typedef ROOT::Math::PositionVector3D<ROOT::Math::Cartesian3D<float> > XYZPoint;
//typedef ROOT::Math::PositionVector3D<ROOT::Math::Cylindrical3D<float> > RhoZPhiPoint;
typedef ROOT::Math::PositionVector3D<ROOT::Math::Cartesian3D<float>, ROOT::Math::LocalCoordinateSystemTag> LocalPoint;
typedef ROOT::Math::PositionVector3D<ROOT::Math::Cartesian3D<float>, ROOT::Math::GlobalCoordinateSystemTag> GlobalPoint;
}  // namespace slh1l1tt

// copied from DataFormats/Math/interface/Vector3D.h to avoid dependency for now
#include "Math/Vector3D.h"
namespace slhcl1tt {
typedef ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<float> > XYZVector;
//typedef ROOT::Math::DisplacementVector3D<ROOT::Math::CylindricalEta3D<float> > RhoEtaPhiVector;
typedef ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<float>, ROOT::Math::LocalCoordinateSystemTag> LocalVector;
typedef ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<float>, ROOT::Math::GlobalCoordinateSystemTag> GlobalVector;

}  // namespace slhcl1tt

// copied from DataFormats/Math/interface/LorentzVector.h to avoid dependency for now
#include "Math/LorentzVector.h"
namespace slhcl1tt {
typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > XYZTLorentzVector;
}  // namespace slhcl1tt

#endif
