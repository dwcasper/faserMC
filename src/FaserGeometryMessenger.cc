#include "FaserGeometryMessenger.hh"
#include "FaserDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"

FaserGeometryMessenger::FaserGeometryMessenger(FaserDetectorConstruction* detectorConstruction)
  : fDetectorConstruction(detectorConstruction)
{
  geometryDirectory = new G4UIdirectory("/faser/geo/");
  geometryDirectory->SetGuidance("Detector geometry initialization parameters.");

  cmd_sensor_readoutStrips = new G4UIcmdWithAnInteger("/faser/geo/readoutStrips", this);
  cmd_sensor_readoutStrips->SetGuidance("Number of active readout strips per sensor.");
  cmd_sensor_readoutStrips->SetParameterName("readoutStrips", true, true);
  cmd_sensor_readoutStrips->AvailableForStates(G4State_PreInit);

  cmd_sensor_stripPitch = new G4UIcmdWithADoubleAndUnit("/faser/geo/stripPitch", this);
  cmd_sensor_stripPitch->SetGuidance("Distance between strips in the silicon wafers.");
  cmd_sensor_stripPitch->SetParameterName("stripPitch", true, true);
  cmd_sensor_stripPitch->SetDefaultUnit("mm");
  cmd_sensor_stripPitch->SetUnitCandidates("mm micron cm m");
  cmd_sensor_stripPitch->AvailableForStates(G4State_PreInit);

  cmd_sensor_stripLength = new G4UIcmdWithADoubleAndUnit("/faser/geo/stripLength", this);
  cmd_sensor_stripLength->SetGuidance("Length of strips in the silicon wafers.");
  cmd_sensor_stripLength->SetParameterName("stripLength", true, true);
  cmd_sensor_stripLength->SetDefaultUnit("mm");
  cmd_sensor_stripLength->SetUnitCandidates("mm micron cm m");
  cmd_sensor_stripLength->AvailableForStates(G4State_PreInit);

  cmd_sensor_gap = new G4UIcmdWithADoubleAndUnit("/faser/geo/sensorGap", this);
  cmd_sensor_gap->SetGuidance("Gap between rows of strips in the silicon wafers.");
  cmd_sensor_gap->SetParameterName("sensorGap", true, true);
  cmd_sensor_gap->SetDefaultUnit("mm");
  cmd_sensor_gap->SetUnitCandidates("mm micron cm m");
  cmd_sensor_gap->AvailableForStates(G4State_PreInit);

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

  cmd_support_sizeZ = new G4UIcmdWithADoubleAndUnit("/faser/geo/supportSizeZ", this);
  cmd_support_sizeZ->SetGuidance("Set the thickness of the support strut.");
  cmd_support_sizeZ->SetParameterName("supportSizeZ", true, true);
  cmd_support_sizeZ->SetDefaultUnit("mm");
  cmd_support_sizeZ->SetUnitCandidates("mm micron cm m");
  cmd_support_sizeZ->AvailableForStates(G4State_PreInit);

  cmd_detector_sensorPlanes = new G4UIcmdWithAnInteger("/faser/geo/trackerPlanes", this);
  cmd_detector_sensorPlanes->SetGuidance("Number of silicon sensor planes in the tracker.");
  cmd_detector_sensorPlanes->SetParameterName("sensorPlanes", true, true);
  cmd_detector_sensorPlanes->AvailableForStates(G4State_PreInit);

  cmd_detector_planePitch = new G4UIcmdWithADoubleAndUnit("/faser/geo/planePitch", this);
  cmd_detector_planePitch->SetGuidance("Longitudinal spacing between sensor planes.");
  cmd_detector_planePitch->SetParameterName("planePitch", true, true);
  cmd_detector_planePitch->SetDefaultUnit("mm");
  cmd_detector_planePitch->SetUnitCandidates("mm micron cm m");
  cmd_detector_planePitch->AvailableForStates(G4State_PreInit);

  cmd_detector_caloPlanes = new G4UIcmdWithAnInteger("/faser/geo/caloPlanes", this);
  cmd_detector_caloPlanes->SetGuidance("Number of silicon sensor planes in the calorimeter.");
  cmd_detector_caloPlanes->SetParameterName("caloPlanes", true, true);
  cmd_detector_caloPlanes->AvailableForStates(G4State_PreInit);

  cmd_detector_absorberX0 = new G4UIcmdWithADouble("/faser/geo/absorberX0", this);
  cmd_detector_absorberX0->SetGuidance("Calorimeter absorber thickness in radiation lengths.");
  cmd_detector_absorberX0->SetParameterName("absorberX0", true, true);
  cmd_detector_absorberX0->AvailableForStates(G4State_PreInit);

  cmd_detector_samplerSizeZ = new G4UIcmdWithADoubleAndUnit("/faser/geo/samplerSizeZ", this);
  cmd_detector_samplerSizeZ->SetGuidance("Thickness of calorimeter sampling layer.");
  cmd_detector_samplerSizeZ->SetParameterName("samplerSizeZ", true, true);
  cmd_detector_samplerSizeZ->SetDefaultUnit("um");
  cmd_detector_samplerSizeZ->SetUnitCandidates("mm micron cm m");
  cmd_detector_samplerSizeZ->AvailableForStates(G4State_PreInit);

  cmd_detector_decayVolumeLength = new G4UIcmdWithADoubleAndUnit("/faser/geo/decayVolumeLength", this);
  cmd_detector_decayVolumeLength->SetGuidance("Air volume in front of first sensor.");
  cmd_detector_decayVolumeLength->SetParameterName("decayVolumeLength", true, true);
  cmd_detector_decayVolumeLength->SetDefaultUnit("m");
  cmd_detector_decayVolumeLength->SetUnitCandidates("mm micron cm m");
  cmd_detector_decayVolumeLength->AvailableForStates(G4State_PreInit);

  cmd_detector_trackerLength = new G4UIcmdWithADoubleAndUnit("/faser/geo/trackerLength", this);
  cmd_detector_trackerLength->SetGuidance("Length of the tracker region of the detector");
  cmd_detector_trackerLength->SetParameterName("trackerLength", true, true);
  cmd_detector_trackerLength->SetDefaultUnit("m");
  cmd_detector_trackerLength->SetUnitCandidates("mm micron cm m");
  cmd_detector_trackerLength->AvailableForStates(G4State_PreInit);

  cmd_detector_calorimeterLength = new G4UIcmdWithADoubleAndUnit("/faser/geo/calorimeterLength", this);
  cmd_detector_calorimeterLength->SetGuidance("Length of the calorimeter region of the detector");
  cmd_detector_calorimeterLength->SetParameterName("calorimeterLength", true, true);
  cmd_detector_calorimeterLength->SetDefaultUnit("m");
  cmd_detector_calorimeterLength->SetUnitCandidates("mm micron cm m");
  cmd_detector_calorimeterLength->AvailableForStates(G4State_PreInit);

  fDetectorConstruction->setReadoutStrips( FaserDetectorConstruction::default_sensor_readoutStrips );
  fDetectorConstruction->setStripPitch( FaserDetectorConstruction::default_sensor_stripPitch );
  fDetectorConstruction->setStripLength( FaserDetectorConstruction::default_sensor_stripLength );
  fDetectorConstruction->setSensorGap( FaserDetectorConstruction::default_sensor_gap );
  fDetectorConstruction->setSensorSizeZ ( FaserDetectorConstruction::default_sensor_sizeZ );
  fDetectorConstruction->setSensorStereoAngle( FaserDetectorConstruction::default_sensor_stereoAngle );
  fDetectorConstruction->setSupportSizeZ ( FaserDetectorConstruction::default_support_sizeZ );
  fDetectorConstruction->setSensorPlanes( FaserDetectorConstruction::default_detector_sensorPlanes );
  fDetectorConstruction->setCaloPlanes( FaserDetectorConstruction::default_detector_caloPlanes );
  fDetectorConstruction->setAbsorberX0( FaserDetectorConstruction::default_detector_absorberX0 );
  fDetectorConstruction->setSamplerSizeZ( FaserDetectorConstruction::default_detector_samplerSizeZ );
  fDetectorConstruction->setPlanePitch( FaserDetectorConstruction::default_detector_planePitch );
  fDetectorConstruction->setDecayVolumeLength( FaserDetectorConstruction::default_detector_decayVolumeLength );
  fDetectorConstruction->setTrackerLength( FaserDetectorConstruction::default_detector_trackerLength );
  fDetectorConstruction->setCalorimeterLength( FaserDetectorConstruction::default_detector_calorimeterLength );
}

FaserGeometryMessenger::~FaserGeometryMessenger()
{
  if (cmd_detector_calorimeterLength) delete cmd_detector_calorimeterLength;
  if (cmd_detector_trackerLength) delete cmd_detector_trackerLength;
  if (cmd_detector_decayVolumeLength) delete cmd_detector_decayVolumeLength;
  if (cmd_detector_planePitch) delete cmd_detector_planePitch;
  if (cmd_detector_sensorPlanes) delete cmd_detector_sensorPlanes;
  if (cmd_detector_caloPlanes) delete cmd_detector_caloPlanes;
  if (cmd_detector_absorberX0) delete cmd_detector_absorberX0;
  if (cmd_detector_samplerSizeZ) delete cmd_detector_samplerSizeZ;
  if (cmd_support_sizeZ) delete cmd_support_sizeZ;
  if (cmd_sensor_stereoAngle) delete cmd_sensor_stereoAngle;
  if (cmd_sensor_gap) delete cmd_sensor_gap;
  if (cmd_sensor_sizeZ) delete cmd_sensor_sizeZ;
  if (cmd_sensor_stripPitch) delete cmd_sensor_stripPitch;
  if (cmd_sensor_stripLength) delete cmd_sensor_stripLength;
  if (cmd_sensor_readoutStrips) delete cmd_sensor_readoutStrips;
  if (geometryDirectory) delete geometryDirectory;
}

void FaserGeometryMessenger::SetNewValue(G4UIcommand* command, G4String newValues)
{
  if (command == cmd_sensor_readoutStrips) 
    {
      fDetectorConstruction->setReadoutStrips( cmd_sensor_readoutStrips->GetNewIntValue(newValues) );
    }
  else if (command == cmd_sensor_stripPitch)
    {
      fDetectorConstruction->setStripPitch( cmd_sensor_stripPitch->GetNewDoubleValue(newValues) );
    }
  else if (command == cmd_sensor_stripLength)
    {
      fDetectorConstruction->setStripLength( cmd_sensor_stripLength->GetNewDoubleValue(newValues) );
    }
  else if (command == cmd_sensor_gap)
    {
      fDetectorConstruction->setSensorGap( cmd_sensor_gap->GetNewDoubleValue(newValues) );
    }
  else if (command == cmd_sensor_sizeZ)
    {
      fDetectorConstruction->setSensorSizeZ( cmd_sensor_sizeZ->GetNewDoubleValue(newValues) );
    }
  else if (command == cmd_sensor_stereoAngle)
    {
      fDetectorConstruction->setSensorStereoAngle( cmd_sensor_stereoAngle->GetNewDoubleValue(newValues) );
    }
  else if (command == cmd_support_sizeZ)
    {
      fDetectorConstruction->setSupportSizeZ( cmd_support_sizeZ->GetNewDoubleValue(newValues) );
    }
  else if (command == cmd_detector_sensorPlanes)
    {
      fDetectorConstruction->setSensorPlanes( cmd_detector_sensorPlanes->GetNewIntValue(newValues) );
    }
  else if (command == cmd_detector_caloPlanes)
  {
    fDetectorConstruction->setCaloPlanes( cmd_detector_caloPlanes->GetNewIntValue(newValues) );
  }
  else if (command == cmd_detector_absorberX0)
  {
    fDetectorConstruction->setAbsorberX0( cmd_detector_absorberX0->GetNewDoubleValue(newValues) );
  }
  else if (command == cmd_detector_samplerSizeZ)
  {
    fDetectorConstruction->setSamplerSizeZ( cmd_detector_samplerSizeZ->GetNewDoubleValue(newValues) );
  }
  else if (command == cmd_detector_planePitch)
    {
      fDetectorConstruction->setPlanePitch( cmd_detector_planePitch->GetNewDoubleValue(newValues) );
    }
  else if (command == cmd_detector_decayVolumeLength)
    {
      fDetectorConstruction->setDecayVolumeLength( cmd_detector_decayVolumeLength->GetNewDoubleValue(newValues) );
    }
  else if (command == cmd_detector_trackerLength)
    {
      fDetectorConstruction->setTrackerLength( cmd_detector_trackerLength->GetNewDoubleValue(newValues) );
    }
  else if (command == cmd_detector_calorimeterLength)
    {
      fDetectorConstruction->setCalorimeterLength( cmd_detector_calorimeterLength->GetNewDoubleValue(newValues) );
    }
}

G4String FaserGeometryMessenger::GetCurrentValue(G4UIcommand* command)
{
  G4String cv;
  
  if (command == cmd_sensor_readoutStrips)
  {
    cv = cmd_sensor_readoutStrips->ConvertToString(fDetectorConstruction->getReadoutStrips());
  }
  else if (command == cmd_sensor_stripPitch)
  {
    cv = cmd_sensor_stripPitch->ConvertToString(fDetectorConstruction->getStripPitch(), "mm");
  }
  else if (command == cmd_sensor_stripLength)
  {
    cv = cmd_sensor_stripLength->ConvertToString(fDetectorConstruction->getStripLength(), "mm");
  }
  else if (command == cmd_sensor_gap)
  {
    cv = cmd_sensor_gap->ConvertToString(fDetectorConstruction->getSensorGap(), "mm");
  }
  else if (command == cmd_sensor_sizeZ)
  {
    cv = cmd_sensor_sizeZ->ConvertToString(fDetectorConstruction->getSensorSizeZ(), "mm");
  }
  else if (command == cmd_sensor_stereoAngle)
  {
    cv = cmd_sensor_stereoAngle->ConvertToString(fDetectorConstruction->getSensorStereoAngle(), "mrad");
  }
  else if (command == cmd_support_sizeZ)
  {
    cv = cmd_support_sizeZ->ConvertToString(fDetectorConstruction->getSupportSizeZ(), "mm");
  }
  else if (command == cmd_detector_sensorPlanes)
  {
    cv = cmd_detector_sensorPlanes->ConvertToString(fDetectorConstruction->getSensorPlanes());
  }
  else if (command == cmd_detector_caloPlanes)
  {
    cv = cmd_detector_caloPlanes->ConvertToString(fDetectorConstruction->getCaloPlanes());
  }
  else if (command == cmd_detector_absorberX0)
  {
    cv = cmd_detector_absorberX0->ConvertToString(fDetectorConstruction->getAbsorberX0());
  }
  else if (command == cmd_detector_samplerSizeZ)
  {
    cv = cmd_detector_samplerSizeZ->ConvertToString(fDetectorConstruction->getSamplerSizeZ(), "um");
  }
  else if (command == cmd_detector_planePitch)
  {
    cv = cmd_detector_planePitch->ConvertToString(fDetectorConstruction->getPlanePitch(), "m");
  }
  else if (command == cmd_detector_decayVolumeLength)
  {
    cv = cmd_detector_decayVolumeLength->ConvertToString(fDetectorConstruction->getDecayVolumeLength(), "m");
  }
  else if (command == cmd_detector_trackerLength)
  {
    cv = cmd_detector_trackerLength->ConvertToString(fDetectorConstruction->getTrackerLength(), "m");
  }
  else if (command == cmd_detector_calorimeterLength)
  {
    cv = cmd_detector_calorimeterLength->ConvertToString(fDetectorConstruction->getCalorimeterLength(), "m");
  }
  return cv;
}
