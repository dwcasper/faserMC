// adapted from Geant4 example

#ifndef FaserDetectorConstruction_h
#define FaserDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4RotationMatrix.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

class FaserDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    FaserDetectorConstruction();
    virtual ~FaserDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    

  protected:
  // these are not copied by the volumes that use them,
  // so they must not be changed

  G4RotationMatrix* fStereoPlus;
  G4RotationMatrix* fStereoMinus;
  G4RotationMatrix* fOverlapAngle;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

