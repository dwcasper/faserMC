#pragma once

#include "FaserCluster.hh"
#include "TVector3.h"
#include <vector>


// SpacePoint class for grouped Faser clusters
// (Clusters on front/back of a detection plane should be grouped.)
//

class FaserSpacePoint {
private:
  //long fIdentifier;
  std::vector<FaserCluster*> fClusters;
  TVector3 fGlobalPosition;
  void ComputePosition();

public:

  FaserSpacePoint();
  FaserSpacePoint(std::vector<FaserCluster*> & clusters);

  virtual ~FaserSpacePoint() {
  }

  void AddCluster(FaserCluster * cluster);
  std::vector<FaserCluster*> & Clusters() { return fClusters; }

  //long Identifier() { return fIdentifier; }

  int Plane()  const { return fClusters.size()==0 ? -1 : fClusters[0]->Plane(); }
  int Module() const { return fClusters.size()==0 ? -1 : fClusters[0]->Module(); }
  int Sensor() const;
  int Row()    const { return fClusters.size()==0 ? -1 : fClusters[0]->Row(); }

  double Charge() const;
  TVector3 GlobalPosition() const { return fGlobalPosition; }

  void Print() const;
  void Debug() const;

  double DistanceTo(const FaserSpacePoint & other) const;

};


