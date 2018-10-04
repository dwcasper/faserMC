#ifndef FASER_TRACKER_GEOMETRY_HH
#define FASER_TRACKER_GEOMETRY_HH 1

#include <vector>
#include <string>

//------------------------------------------------------------------------------

class FaserTrackerGeometry {

public:

  //TODO: populate these in faserMC
  double  halfX = 10.;
  double  halfY = 10.;
  double  halfZ = 10.;
  double  sensorHalfX = -10000.;
  double  sensorHalfY = -10000.;
  double  sensorGap     = -10000.; // mm
  double  thickness = -10000.;
  double  stripLength   = -10000.;
  double  X0, L0, A, Z, rho;

  int     nStrips       = -10000;
  double  stripPitch    = -10000.; // mm
  double  stereoAngle   = -10000.; // rad
  double  moduleOffsetX = -10000.; // mm
  double  sensorOffsetY = -10000.; // mm
  double  rowOffsetY    = -10000.; // mm

  std::vector<int>   planeIndices_front;
  std::vector<int>   planeIndices_central;
  std::vector<int>   planeIndices_end;
  //std::vector<int> * p_planeIndices_front   = &planeIndices_front;
  //std::vector<int> * p_planeIndices_central = &planeIndices_central;
  //std::vector<int> * p_planeIndices_end     = &planeIndices_end;

  std::vector<double> planeZ; // mm
  //std::vector<double> * p_planeZ = &planeZ;


  FaserTrackerGeometry() {
  }

  virtual ~FaserTrackerGeometry() {
  }

  //void WriteToFile(const std::string & fileName);

};

#endif
