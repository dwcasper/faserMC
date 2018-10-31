#include "FaserSpacePoint.hh"

#include "globals.hh"

#include <iostream>
#include <string>
#include <map>
#include <stdexcept>

using std::cout;
using std::to_string;
using std::runtime_error;

////////////////////////////////////////////////////////////////////////////////
// TODO: Read this info in from FaserGeometry instead of hardcoding here:

namespace Geo {
  // TODO: Remove this namespace & read directly from `faserGeo.mac` & `FaserDetectorConstruction`.
  //       (This is being developed on the `uniformGeoHandling` branch of `faserMC`.)

  // ITk geometry
  //int nStrips = 1280;
  //double stripPitch = 0.0755; // mm
  //double stereoAngle = 0.026; // rad

  //double moduleOffsetX = 48.5064; // mm, - for module 0, + for module 1
  //double sensorOffsetY = 96.938/(2.*TMath::Cos(stereoAngle)); // mm, + for sensors 0/1, - for sensors 2/3

  //std::map<int, double> planePosZ = { // mm
  //  { 0,    0.00 },
  //  { 1,   50.00 },
  //  { 2,  909.04 },
  //  { 3,  959.04 },
  //  { 4, 1009.04 },
  //  { 5, 1059.04 },
  //  { 6, 1942.02 },
  //  { 7, 1992.02 },
  //};
  //std::map<int, double> planePosZ = { // mm
  //  { 0, -0.000156387},
  //  { 1, 49.9998},
  //  { 2, 99.9998},
  //  { 3, 938.03},
  //  { 4, 988.03},
  //  { 5, 1038.03},
  //  { 6, 1892.02},
  //  { 7, 1942.02},
  //  { 8, 1992.02},
  //};

  // SCT geometry
  int nStrips = 768;
  double stripPitch = 0.08; // mm
  double stereoAngle = 0.026; // rad

  double offsetX(int iModule) {
    switch (iModule) { // mm
    case 0:
    case 4:
      return -3 * 28.865;
    case 1:
    case 5:
      return -1 * 28.865;
    case 2:
    case 6:
      return  1 * 28.865;
    case 3:
    case 7:
      return  3 * 28.865;
    default:
      return 0;
    }
  }

  double offsetY(int iModule) {
    switch (iModule) { // mm
    case 0:
    case 1:
    case 2:
    case 3:
      return -60.13;
    case 4:
    case 5:
    case 6:
    case 7:
      return  60.13;
    default:
      return 0;
    }
  }

  double offsetZ(int iModule) {
    switch (iModule) { // mm
    case 0:
    case 2:
      return -3 * 2.5;
    case 1:
    case 3:
      return  1 * 2.5;
    case 4:
    case 6:
      return  3 * 2.5;
    case 5:
    case 7:
      return -1 * 2.5;
    default:
      return 0;
    }
  }

  std::map<int, double> planePosZ = { // mm
    { 0,    4.01 },
    { 1,   54.01 },
    { 2,  104.01 },
    { 3,  930.01 },
    { 4,  980.01 },
    { 5, 1030.01 },
    { 6, 1888.01 },
    { 7, 1938.01 },
    { 8, 1988.01 },
  };

}


//------------------------------------------------------------------------------

FaserSpacePoint::FaserSpacePoint()
  : fClusters {}
{
  ;
}

//------------------------------------------------------------------------------

FaserSpacePoint::FaserSpacePoint(std::vector<FaserCluster*> & clusters)
  : fClusters {}
{
  for (FaserCluster * clus : clusters) {
    AddCluster(clus);
  }
}

//------------------------------------------------------------------------------

int FaserSpacePoint::Sensor() const
{
  if (fClusters.size()==0) return -1;
  int sensor = fClusters[0]->Sensor();

  // Group top sensors 0/1 -> 0 and bottom 2/3 -> 2.
  if (sensor==0 || sensor==1) return 0;
  else if (sensor==2 || sensor==3) return 2;

  return sensor;
}

//------------------------------------------------------------------------------

double FaserSpacePoint::Charge() const
{ double charge = 0.;
  for (const FaserCluster * clus : fClusters) charge += clus->Charge();
  return charge;
}

//------------------------------------------------------------------------------

TVector3 FaserSpacePoint::GlobalPos() const
{
  double stripFront = 0.;
  double stripBack = 0.;
  double sumAbsQ_front = 0.;
  double sumAbsQ_back = 0.;
  G4ThreeVector weightedClusterGlobalPos_front {0.,0.,0.};
  G4ThreeVector weightedClusterGlobalPos_back  {0.,0.,0.};

  for (const FaserCluster * clus : fClusters)
  {
    int sensor = clus->Sensor();
    double absCharge = fabs(clus->Charge());
    if (sensor==1 || sensor==3) {
      stripFront += absCharge * clus->WeightedStrip();
      weightedClusterGlobalPos_front += clus->GlobalPos();
      sumAbsQ_front += absCharge;
    } else if (sensor==0 || sensor==2) {
      stripBack += absCharge * clus->WeightedStrip();
      weightedClusterGlobalPos_back += clus->GlobalPos();
      sumAbsQ_back += absCharge;
    }
  }

  // Local coordinates along front/back rows in mm:
  bool frontPresent = false;
  bool backPresent = false;
  double uFront = 0.;
  double uBack  = 0.;
  if (sumAbsQ_front != 0) {
    frontPresent = true;
    stripFront /= sumAbsQ_front;
    uFront = (0.5*(Geo::nStrips - 1) - stripFront) * Geo::stripPitch;
  }
  if (sumAbsQ_back != 0) {
    backPresent = true;
    stripBack  /= sumAbsQ_back;
    uBack = (0.5*(Geo::nStrips - 1) - stripBack) * Geo::stripPitch;
  }

  //if (!frontPresent && !backPresent) return {-10000., -10000., -10000.};

  //if (!frontPresent) return {weightedClusterGlobalPos_back.x(),
  //                           weightedClusterGlobalPos_back.y(),
  //                           weightedClusterGlobalPos_back.z()};

  //if (!backPresent) return {weightedClusterGlobalPos_front.x(),
  //                          weightedClusterGlobalPos_front.y(),
  //                          weightedClusterGlobalPos_front.z()};

  // Reject space point unless both front and back clusters are present
  if (!frontPresent || !backPresent) {
    return {-10000., -10000., -10000.};
  }

  // Else both front & back clusters present => combine front/back pair, using
  // local to global coordinate transformation:

  // To get the global coordinates, start with respect to the row center.
  // Then shift by global positions of the centers of the plane, module, sensor, and row.
  double x = 0.5*(uFront + uBack)/TMath::Cos(Geo::stereoAngle);
  double y = 0.5*(uBack - uFront)/TMath::Sin(Geo::stereoAngle);
  double z = 0.;

  // Shift to the center of the module (origin of local coordinate system)
  x += Geo::offsetX(Module());
  y += Geo::offsetY(Module());
  z += Geo::planePosZ.count(Plane()) < 1 ? -10000. : Geo::planePosZ[Plane()] + Geo::offsetZ(Module());

  return {x, y, z};
}

//------------------------------------------------------------------------------

void FaserSpacePoint::AddCluster(FaserCluster * cluster)
{
  if (fClusters.size() == 0)
  {
    fClusters.push_back(cluster);
    return;
  }

  if (cluster->Plane()  != Plane() ) throw runtime_error {"FaserSpacePoint::AddCluster: incompatible planes"};
  if (cluster->Module() != Module()) throw runtime_error {"FaserSpacePoint::AddCluster: incompatible modules"};

  int sensor = fClusters[0]->Sensor();
  // Group top sensors 0/1 -> 0 and bottom 2/3 -> 2.
  if (sensor==1) sensor = 0;
  else if (sensor==3) sensor = 2;
  if (sensor!= Sensor()) throw runtime_error {"FaserSpacePoint::AddCluster: incompatible sensors"};

  if (cluster->Row() != Row()) throw runtime_error {"FaserSpacePoint::AddCluster: incompatible rows"};

  fClusters.push_back(cluster);
}

//------------------------------------------------------------------------------

void FaserSpacePoint::Print() const
{
  cout << "INFO  FaserSpacePoint::Print\n"
       << "  Clusters (nClusters=" << fClusters.size() << "):\n";

  for (size_t i=0; i<fClusters.size(); ++i)
  {
    FaserCluster * c = fClusters[i];
    cout << "    Cluster " << i << "  plane=" << c->Plane() << ", module=" << c->Module() << ", sensor=" << c->Sensor()
                           << ", row=" << c->Row() << ", charge=" << c->Charge() << ", weightedStrip=" << c->WeightedStrip()
                           << ", globalPos=" << c->GlobalPos() <<"\n";
  }

  TVector3 pos = GlobalPos();
  double x = pos.X();
  double y = pos.Y();
  double z = pos.Z();
  cout << "  Space point obtained:\n"
       << "    plane=" << Plane() << ", module=" << Module() << ", sensor=" << Sensor() << ", row=" << Row()
                       << ", charge=" << Charge() << ", globalPos=(" << x << ", " << y << ", " << z << ")\n";
}

//------------------------------------------------------------------------------

void FaserSpacePoint::Debug() const
{

  for (size_t i=0; i<fClusters.size(); ++i)
  {
    FaserCluster * c = fClusters[i];
    cout << "DEBUG_CLUSTERS  " << i << ";" << c->Plane() << ";" << c->Module() << ";" << c->Sensor()
                               << ";" << c->Row() << ";" << c->WeightedStrip() << ";" << c->Charge()
                               << ";" << c->GlobalPos() << "\n";
  }

  //TVector3 pos = GlobalPos();
  //double x = pos.X();
  //double y = pos.Y();
  //double z = pos.Z();
  //cout << "DEBUG_SPACEPOINT  " << Plane() << "," << Module() << "," << Sensor() << "," << Row()
  //                     << "," << x << "," << y << "," << z << "\n";
}

//------------------------------------------------------------------------------

double FaserSpacePoint::DistanceTo(const FaserSpacePoint & other) const {
    return (GlobalPos() - other.GlobalPos()).Mag();
}

//------------------------------------------------------------------------------

double FaserSpacePoint::DistanceTo(const FaserCircleFit & fit) const {
    TVector3 pos = GlobalPos();
    return fit.FitFunction()->DistancetoPrimitive(pos.Z(), pos.X());
}

