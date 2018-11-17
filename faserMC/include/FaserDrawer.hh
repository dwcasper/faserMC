#ifndef FASER_DRAWER_HH
#define FASER_DRAWER_HH 1

#include "FaserEvent.hh"
#include "G4Point3D.hh"

#include <memory>
#include <vector>
#include <string>


//------------------------------------------------------------------------------

struct FaserDrawer {

  /**
   * Struct for holding helix trajectory parameters.
   * See here for the definitions of each parameter:
   * http://www.physics.iitm.ac.in/~sercehep2013/track2_Gagan_Mohanty.pdf
   */
  struct HelixTrajectory {
    int     h;
    double  x0;
    double  y0;
    double  z0;
    double  R;
    double  phi0;
    double  lambda;
  };

  TVector3     bField;

  FaserDrawer(TVector3 bField_)
    : bField {bField_}
  {
  }

  virtual ~FaserDrawer() {
  }

  void DrawSpacePoints(FaserEvent * event);
  void DrawPropagatedTrajectory(FaserEvent * event);


private:

  G4Point3D TrackPosAtS(const HelixTrajectory & tr, double s);
  G4Point3D TrackPosAtZ(const HelixTrajectory & tr, double z);

};

#endif
