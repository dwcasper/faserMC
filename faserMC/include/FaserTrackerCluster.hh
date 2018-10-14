#pragma once

#include "FaserTrackerDigit.hh"
#include "TVector3.h"
#include <vector>


//------------------------------------------------------------------------------

class FaserTrackerCluster
{
public:

  int       plane;
  int       module;
  int       sensor;
  int       row;
  double    weightedStrip;
  double    charge;
  TVector3  globalPos;
  std::vector<FaserTrackerDigit*> digits;

  FaserTrackerCluster()
    : plane {-1}
    , module {-1}
    , sensor {-1}
    , row {-1}
    , weightedStrip{-1}
    , charge {0.}
    , globalPos {0., 0., 0.}
  {
  }

  FaserTrackerCluster(
    int plane_,
    int module_,
    int sensor_,
    int row_,
    double weightedStrip_,
    double charge_,
    TVector3 globalPos_
  ) : plane {plane_}
    , module {module_}
    , sensor {sensor_}
    , row {row_}
    , weightedStrip {weightedStrip_}
    , charge {charge_}
    , globalPos {globalPos_}
  {
  }

  virtual ~FaserTrackerCluster() {
    for (FaserTrackerDigit * digit : digits) {
      if (digit) delete digit;
    }
  }

  void print() const;

};

