#ifndef FASER_TRACKER_SPACE_POINT_HH
#define FASER_TRACKER_SPACE_POINT_HH 1

#include "TVector3.h"
#include "TMath.h"
#include <vector>

//------------------------------------------------------------------------------

class FaserTrackerSpacePoint
{
public:

  int       plane;
  int       module;
  int       sensor;
  int       row;
  double    charge;
  TVector3  globalPos;
  TVector3  globalPosUncertainty;

  FaserTrackerSpacePoint()
    : plane {-1}
    , module {-1}
    , sensor {-1}
    , row {-1}
    , globalPos {0., 0., 0.}
    , globalPosUncertainty {0.08/TMath::Sqrt(2.), 0.08/(TMath::Sqrt(2.)*TMath::Sin(0.02)), 0.}
  {
  }

  FaserTrackerSpacePoint(
    int plane_,
    int module_,
    int sensor_,
    int row_,
    TVector3 globalPos_
  ) : plane {plane_}
    , module {module_}
    , sensor {sensor_}
    , row {row_}
    , globalPos {globalPos_}
    , globalPosUncertainty {0.08/TMath::Sqrt(2.), 0.08/(TMath::Sqrt(2.)*TMath::Sin(0.02)), 0.}
  {
  }

  virtual ~FaserTrackerSpacePoint() {
  }

  void print() const;

};

#endif
