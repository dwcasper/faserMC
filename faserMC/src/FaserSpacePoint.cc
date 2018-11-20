#include "FaserSpacePoint.hh"
#include "G4MTRunManager.hh"
#include "G4RunManager.hh"
#include "FaserDetectorConstruction.hh"

#include "globals.hh"

#include <iostream>
#include <string>
#include <map>
#include <stdexcept>

using std::cout;
using std::to_string;
using std::runtime_error;

//------------------------------------------------------------------------------

FaserSpacePoint::FaserSpacePoint()
  : fClusters {}
  , fGlobalPosition {0., 0.,  0.}
{
  ;
}

//------------------------------------------------------------------------------

FaserSpacePoint::FaserSpacePoint(std::vector<FaserCluster*> & clusters)
  : fClusters {}
  , fGlobalPosition {0., 0., 0. }
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

void FaserSpacePoint::ComputePosition()
{
  // todo: make this a static member variable if possible (not sure whether it complicates I/O)
  const FaserDetectorConstruction* detectorConstruction = nullptr;
  G4MTRunManager* master = G4MTRunManager::GetMasterRunManager();
  if (master != nullptr)
  {
    detectorConstruction = static_cast<const FaserDetectorConstruction*>(master->GetUserDetectorConstruction());
  }
  else
  {
    detectorConstruction = static_cast<const FaserDetectorConstruction*>(
			  G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  }

  double stereoAngle = 20.0*mrad;
  if ( detectorConstruction != nullptr )
  {
    stereoAngle = detectorConstruction->getSensorStereoAngle();
  }
  else 
  {
    G4ExceptionDescription msg;
    msg << "Detector construction not found.\n"; 
    msg << "Perhaps you have changed geometry.\n";
    msg << "A default stereo angle of 20 mrad will be used.";
    G4Exception("FaserSpacePoint::ComputePosition()",
		"MyCode0004",JustWarning,msg);
  }

  G4ThreeVector frontPos;
  G4ThreeVector backPos;
  int frontCount = 0;
  int backCount = 0;
  for (const FaserCluster * clus : fClusters)
  {
    int sensor = clus->Sensor();
    double absCharge = fabs(clus->Charge());
    if (absCharge > 0)
    {
      if (sensor==1 || sensor==3) 
      {
        frontPos += clus->GlobalPosition();
        frontCount++;
      } 
      else if (sensor==0 || sensor==2)
      {
        backPos += clus->GlobalPosition();
        backCount++;
      }
    }
  }

  // Reject space point unless both front and back clusters are present
  if (frontCount == 0 || backCount == 0) 
  {
    return;
  }

  // Average of front and back; not clear if possible...
  if (frontCount > 1) frontPos *= (1.0/frontCount);
  if (backCount > 1 ) backPos  *= (1.0/backCount);

  double x = 0.5*(frontPos.x() + backPos.x() - (backPos.y() - frontPos.y())*tan(stereoAngle));
  double y = 0.5*(frontPos.y() + backPos.y() - (backPos.x() - frontPos.x())/tan(stereoAngle));
  double z = 0.5*(frontPos.z() + backPos.z());

  fGlobalPosition = TVector3(x, y, z);
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
  ComputePosition();
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
                           << ", globalPos=" << c->GlobalPosition() <<"\n";
  }

  TVector3 pos = GlobalPosition();
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
                               << ";" << c->GlobalPosition() << "\n";
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
    return (GlobalPosition() - other.GlobalPosition()).Mag();
}


