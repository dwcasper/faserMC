#ifndef FASER_SENSOR_PLANE_CONSTRUCTION_HH
#define FASER_SENSOR_PLANE_CONSTRUCTION_HH 1

#include "FaserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "globals.hh"

class FaserSensorPlaneConstruction
{
    public:

    FaserSensorPlaneConstruction(FaserDetectorConstruction* parent, G4String name);
    virtual ~FaserSensorPlaneConstruction();

    G4LogicalVolume* Construct();

    G4String GetName() const { return fName; }
    G4LogicalVolume* GetStrip() const { return fLogicStrip; }

    private:
    FaserSensorPlaneConstruction(const FaserSensorPlaneConstruction& right) = delete;
    FaserSensorPlaneConstruction& operator=(const FaserSensorPlaneConstruction& right) = delete;

    void ConstructModule();
    void ConstructSensor();

    FaserDetectorConstruction*  fParent;
    G4String                    fName;
    G4LogicalVolume*            fLogicStrip;

    G4LogicalVolume* fLogicSensor;
    G4LogicalVolume* fLogicRow;
    G4LogicalVolume* fLogicModule;

    G4RotationMatrix* fStereoPlus;
    G4RotationMatrix* fStereoMinus;
    G4RotationMatrix* fOverlapAngle;

    G4bool checkOverlaps;
};

#endif
