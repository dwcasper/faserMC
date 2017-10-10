#include "FaserGeometryMessenger.hh"
#include "FaserDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"

FaserGeometryMessenger::FaserGeometryMessenger(FaserDetectorConstruction* detectorConstruction)
  : fDetectorConstruction(detectorConstruction)
{
  geometryDirectory = new G4UIdirectory("/faser/geo/");
  geometryDirectory->SetGuidance("Detector geometry initialization parameters.");

  cmd_sensor_sizeXY = new G4UIcmdWithADoubleAndUnit("/faser/geo/sensorSizeXY", this);
  cmd_sensor_sizeXY->SetGuidance("Set the width of the square silicon wafers.");
  cmd_sensor_sizeXY->SetParameterName("sensorSizeXY", true, true);
  cmd_sensor_sizeXY->SetDefaultUnit("mm");
  cmd_sensor_sizeXY->SetUnitCandidates("mm micron cm m");
  cmd_sensor_sizeXY->AvailableForStates(G4State_PreInit);

  cmd_sensor_sizeZ = new G4UIcmdWithADoubleAndUnit("/faser/geo/sensorSizeZ", this);
  cmd_sensor_sizeZ->SetGuidance("Set the thickness of the silicon wafers.");
  cmd_sensor_sizeZ->SetParameterName("sensorSizeZ", true, true);
  cmd_sensor_sizeZ->SetDefaultUnit("mm");
  cmd_sensor_sizeZ->SetUnitCandidates("mm micron cm m");
  cmd_sensor_sizeZ->AvailableForStates(G4State_PreInit);

  cmd_sensor_stereoAngle = new G4UIcmdWithADoubleAndUnit("/faser/geo/stereoAngle", this);
  cmd_sensor_stereoAngle->SetGuidance("Set the angle wafers on each side are rotated.");
  cmd_sensor_stereoAngle->SetParameterName("sensorStereo", true, true);
  cmd_sensor_stereoAngle->SetDefaultUnit("mrad");
  cmd_sensor_stereoAngle->SetUnitCandidates("rad mrad deg");
  cmd_sensor_stereoAngle->AvailableForStates(G4State_PreInit);

  fDetectorConstruction->setSensorSizeXY( FaserDetectorConstruction::default_sensor_sizeXY );
  fDetectorConstruction->setSensorSizeZ ( FaserDetectorConstruction::default_sensor_sizeZ );
  fDetectorConstruction->setSensorStereoAngle( FaserDetectorConstruction::default_sensor_stereoAngle );
}

FaserGeometryMessenger::~FaserGeometryMessenger()
{
  if (cmd_sensor_stereoAngle) delete cmd_sensor_stereoAngle;
  if (cmd_sensor_sizeXY) delete cmd_sensor_sizeXY;
  if (cmd_sensor_sizeZ) delete cmd_sensor_sizeZ;

  if (geometryDirectory) delete geometryDirectory;
}

void FaserGeometryMessenger::SetNewValue(G4UIcommand* command, G4String newValues)
{
  if (command == cmd_sensor_sizeXY)
    {
      fDetectorConstruction->setSensorSizeXY( cmd_sensor_sizeXY->GetNewDoubleValue(newValues) );
    }
  else if (command == cmd_sensor_sizeZ)
    {
      fDetectorConstruction->setSensorSizeZ( cmd_sensor_sizeZ->GetNewDoubleValue(newValues) );
    }
  else if (command == cmd_sensor_stereoAngle)
    {
      fDetectorConstruction->setSensorStereoAngle( cmd_sensor_stereoAngle->GetNewDoubleValue(newValues) );
    }
}

G4String FaserGeometryMessenger::GetCurrentValue(G4UIcommand* command)
{
  G4String cv;
  
  if (command == cmd_sensor_sizeXY)
    {
      cv = cmd_sensor_sizeXY->ConvertToString(fDetectorConstruction->getSensorSizeXY(), "mm");
    }
  else if (command == cmd_sensor_sizeZ)
    {
      cv = cmd_sensor_sizeZ->ConvertToString(fDetectorConstruction->getSensorSizeZ(), "mm");
    }
  else if (command == cmd_sensor_stereoAngle)
    {
      cv = cmd_sensor_stereoAngle->ConvertToString(fDetectorConstruction->getSensorStereoAngle(), "mrad");
    }
  return cv;
}
