#pragma once

#include "TVector3.h"
#include <vector>

//------------------------------------------------------------------------------

class FaserTrackerTruthHit
{
public:

  int       trackID;
  int       plane;
  int       module;
  int       sensor;
  int       row;
  int       strip;
  double    energy;
  TVector3  globalPos;

  FaserTrackerTruthHit()
    : trackID {-1}
    , plane {-1}
    , module {-1}
    , sensor {-1}
    , row {-1}
    , strip {-1}
    , energy {0.}
    , globalPos {0., 0., 0.}
  {
  }

  FaserTrackerTruthHit(
    int trackID_,
    int plane_,
    int module_,
    int sensor_,
    int row_,
    int strip_,
    double energy_,
    TVector3 globalPos_
  ) : trackID {trackID_}
    , plane {plane_}
    , module {module_}
    , sensor {sensor_}
    , row {row_}
    , strip {strip_}
    , energy {energy_}
    , globalPos {globalPos_}
  {
  }

  virtual ~FaserTrackerTruthHit() {
  }

  void print() const;

};

