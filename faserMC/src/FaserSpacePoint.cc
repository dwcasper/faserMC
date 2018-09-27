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
  int nStrips = 1280;
  double stripPitch = 0.0755; // mm
  double stereoAngle = 0.026; // rad

  double moduleOffsetX = 48.5064; // mm, - for module 0, + for module 1
  double sensorOffsetY = 96.938/(2.*TMath::Cos(stereoAngle)); // mm, + for sensors 0/1, - for sensors 2/3
  double rowOffsetY = 0.5*(48.2 + 0.269); // mm, - for row 0, + for row 1

  std::map<int, double> planePosZ = { // mm
    { 0,    0.00 },
    { 1,   50.00 },
    { 2,  909.04 },
    { 3,  959.04 },
    { 4, 1009.04 },
    { 5, 1059.04 },
    { 6, 1942.02 },
    { 7, 1992.02 },
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
  int sensor= fClusters[0]->Sensor();

  // Group sensors 0/1 -> 0 and 2/3 -> 2.
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
  double sumAbsQ = 0.;
  static int iCluster = -1;
  for (const FaserCluster * clus : fClusters)
  {
    int sensor = clus->Sensor();
    double absCharge = fabs(clus->Charge());
    if (sensor==1 || sensor==3) stripFront += absCharge * clus->WeightedStrip();
    else if (sensor==0 || sensor==2) stripBack += absCharge * clus->WeightedStrip();
    sumAbsQ += absCharge;
    //cout << "DEBUG_CLUSTERS  " << ++iCluster
    //                    << "," << absCharge
    //                    << "," << clus->GlobalPos().x()
    //                    << "," << clus->GlobalPos().y()
    //                    << "," << clus->GlobalPos().z()
    //                    << "\n";
  }
  stripFront /= sumAbsQ;
  stripBack  /= sumAbsQ;

  // Local coordinates along front/back rows in mm:
  double uFront = (0.5*(Geo::nStrips - 1) - stripFront) * Geo::stripPitch;
  double uBack  = (0.5*(Geo::nStrips - 1) - stripBack) * Geo::stripPitch;

  // To get the global coordinates, start with respect to the row center.
  // Then shift by global positions of the centers of the plane, module, sensor, and row.
  double x = 0.5*(uFront + uBack)*TMath::Cos(Geo::stereoAngle);
  double y = 0.5*(uFront - uBack)*TMath::Sin(Geo::stereoAngle);
  double z = 0.;

  // Shift z to center of plane
  z = Geo::planePosZ.count(Plane()) < 1 ? -10000. : Geo::planePosZ[Plane()];

  // Shift to center of module
  if (Module()==0) x += -Geo::moduleOffsetX;
  else if (Module()==1) x += Geo::moduleOffsetX;
  else throw runtime_error {"FaserSpacePoint::GlobalPos: invalid module: "+to_string(Module())};

  // Shift to center of sensor
  if (Sensor()==0 || Sensor()==1) y += Geo::sensorOffsetY;
  else if (Sensor()==2 || Sensor()==3) y += -Geo::sensorOffsetY;
  else throw runtime_error {"FaserSpacePoint::GlobalPos: invalid sensor: "+to_string(Sensor())};

  // Shift to center of row
  if (Row()==0) y += -Geo::rowOffsetY;
  else if (Row()==1) y += Geo::rowOffsetY;
  else throw runtime_error {"FaserSpacePoint::GlobalPos: invalid row: "+to_string(Row())};

  //cout << "DEBUG_SP  " << x << "," << y << "," << z << "\n";

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
  // Group sensors 0/1 -> 0 and 2/3 -> 2.
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

