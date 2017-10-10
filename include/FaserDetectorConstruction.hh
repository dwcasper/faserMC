// adapted from Geant4 example

#ifndef FaserDetectorConstruction_h
#define FaserDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4RotationMatrix.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

class FaserGeometryMessenger;
class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

class FaserDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    FaserDetectorConstruction();
    virtual ~FaserDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    
  virtual void setSensorSizeXY(G4double value) { sensor_sizeXY = value; }
  virtual G4double getSensorSizeXY() { return sensor_sizeXY; }

  virtual void setSensorSizeZ(G4double value) { sensor_sizeZ = value; }
  virtual G4double getSensorSizeZ() { return sensor_sizeZ; }

  virtual void setSensorStereoAngle(G4double value) { sensor_stereoAngle = value; }
  virtual G4double getSensorStereoAngle() { return sensor_stereoAngle; }

  static constexpr double default_sensor_sizeXY = 96.64*mm;
  static constexpr double default_sensor_sizeZ = 0.32*mm;
  static constexpr double default_sensor_stereoAngle = 26.0*mrad;

  protected:

  FaserGeometryMessenger* fGeometryMessenger;

  G4double sensor_sizeXY;
  G4double sensor_sizeZ;
  G4double sensor_stereoAngle;

  // these are not copied by the volumes that use them,
  // so they must not be changed

  G4RotationMatrix* fStereoPlus;
  G4RotationMatrix* fStereoMinus;
  G4RotationMatrix* fOverlapAngle;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

