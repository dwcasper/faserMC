#pragma once

#include "TVector3.h"
#include <vector>

//------------------------------------------------------------------------------

class FaserTrackerDigit
{
public:

  int       plane;
  int       module;
  int       sensor;
  int       row;
  int       strip;
  double    charge;
  TVector3  globalPos;
  std::vector<uint> truthHitIndices;

  FaserTrackerDigit()
    : plane {-1}
    , module {-1}
    , sensor {-1}
    , row {-1}
    , strip {-1}
    , charge {0}
    , globalPos {0., 0., 0.}
  {
  }

  FaserTrackerDigit(
    int plane_,
    int module_,
    int sensor_,
    int row_,
    int strip_,
    double charge_,
    TVector3 globalPos_
  ) : plane {plane_}
    , module {module_}
    , sensor {sensor_}
    , row {row_}
    , strip {strip_}
    , charge {charge_}
    , globalPos {globalPos_}
  {
  }

  virtual ~FaserTrackerDigit() {
  }

  void print() const;

};

