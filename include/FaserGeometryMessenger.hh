#ifndef FaserGeometryMessenger_h
#define FaserGeometryMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class FaserDetectorConstruction;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;

class FaserGeometryMessenger : public G4UImessenger
{
public:
  FaserGeometryMessenger(FaserDetectorConstruction* detectorConstruction);
  ~FaserGeometryMessenger();

public:
  virtual void SetNewValue(G4UIcommand* command, G4String newValues);
  virtual G4String GetCurrentValue(G4UIcommand* command);

private:
  FaserDetectorConstruction* fDetectorConstruction;

private:
  G4UIdirectory*                 geometryDirectory;
  G4UIcmdWithAnInteger*          cmd_sensor_readoutStrips;
  G4UIcmdWithADoubleAndUnit*     cmd_sensor_stripPitch;
  G4UIcmdWithADoubleAndUnit*     cmd_sensor_stripLength;
  G4UIcmdWithADoubleAndUnit*     cmd_sensor_gap;
  G4UIcmdWithADoubleAndUnit*     cmd_sensor_sizeZ;
  G4UIcmdWithADoubleAndUnit*     cmd_sensor_stereoAngle;
  G4UIcmdWithADoubleAndUnit*     cmd_support_sizeZ;
  G4UIcmdWithAnInteger*          cmd_detector_sensorPlanes;
  G4UIcmdWithADoubleAndUnit*     cmd_detector_planePitch;
  G4UIcmdWithADoubleAndUnit*     cmd_detector_decayVolumeLength;
};

#endif
