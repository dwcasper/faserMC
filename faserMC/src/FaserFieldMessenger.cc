// copied and adapted from Geant4 extended/field03 example

#include "FaserFieldMessenger.hh"

#include "FaserFieldSetup.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FaserFieldMessenger::FaserFieldMessenger(FaserFieldSetup* fieldSetup)
 : G4UImessenger(),
   fEMfieldSetup(fieldSetup),
   fFieldDir(0),
   fStepperCmd(0),
   fMagFieldCmd(0),
   fLocalMagFieldCmd(0),
   fMinStepCmd(0),
   fUpdateCmd(0)
{
  fFieldDir = new G4UIdirectory("/faser/field/");
  fFieldDir->SetGuidance("Faser magnetic field control.");

  fStepperCmd = new G4UIcmdWithAnInteger("/faser/field/setStepperType",this);
  fStepperCmd->SetGuidance("Select stepper type for magnetic field");
  fStepperCmd->SetParameterName("choice",true);
  fStepperCmd->SetDefaultValue(4);
  fStepperCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fUpdateCmd = new G4UIcmdWithoutParameter("/faser/field/update",this);
  fUpdateCmd->SetGuidance("Update field setup.");
  fUpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  fUpdateCmd->SetGuidance("if you changed field value(s).");
  fUpdateCmd->AvailableForStates(G4State_Idle);
 
  fMagFieldCmd = new G4UIcmdWithADoubleAndUnit("/faser/field/setGlobalField",this);
  fMagFieldCmd->SetGuidance("Define global magnetic field.");
  fMagFieldCmd->SetGuidance("Magnetic field will be in Y direction.");
  fMagFieldCmd->SetParameterName("GlobalBy",false,false);
  fMagFieldCmd->SetDefaultUnit("tesla");
  fMagFieldCmd->AvailableForStates(G4State_Idle);
 
  fLocalMagFieldCmd = new G4UIcmdWithADoubleAndUnit("/faser/field/setTrackerField",this);
  fLocalMagFieldCmd->SetGuidance("Define tracker magnetic field.");
  fLocalMagFieldCmd->SetGuidance("Magnetic field will be in Y direction.");
  fLocalMagFieldCmd->SetGuidance("Overrides the global field in the tracker region only.");
  fLocalMagFieldCmd->SetParameterName("TrackerBy",false,false);
  fLocalMagFieldCmd->SetDefaultUnit("tesla");
  fLocalMagFieldCmd->AvailableForStates(G4State_Idle);
 
  fMinStepCmd = new G4UIcmdWithADoubleAndUnit("/faser/field/setMinStep",this);
  fMinStepCmd->SetGuidance("Define minimal step");
  fMinStepCmd->SetParameterName("min step",false,false);
  fMinStepCmd->SetDefaultUnit("mm");
  fMinStepCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FaserFieldMessenger::~FaserFieldMessenger()
{
  delete fStepperCmd;
  delete fMagFieldCmd;
  delete fLocalMagFieldCmd;
  delete fMinStepCmd;
  delete fFieldDir;
  delete fUpdateCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserFieldMessenger::SetNewValue( G4UIcommand* command, G4String newValue)
{
  if( command == fStepperCmd )
    fEMfieldSetup->SetStepperType(fStepperCmd->GetNewIntValue(newValue));
  if( command == fUpdateCmd )
    fEMfieldSetup->UpdateField();
  if( command == fMagFieldCmd )
  {
    fEMfieldSetup->SetGlobalFieldValue(fMagFieldCmd->GetNewDoubleValue(newValue));
    // Check the value
    G4cout << "Set global field value to " <<
      fEMfieldSetup->GetGlobalFieldValue() / gauss << " Gauss " << G4endl;
  }
  if( command == fLocalMagFieldCmd )
  {
    fEMfieldSetup->SetLocalFieldValue(fLocalMagFieldCmd->GetNewDoubleValue(newValue));
    // Check the value
    G4cout << "Set tracker field value to " <<
      fEMfieldSetup->GetLocalFieldValue() / gauss << " Gauss " << G4endl;
  }
  if( command == fMinStepCmd )
    fEMfieldSetup->SetMinStep(fMinStepCmd->GetNewDoubleValue(newValue));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4String FaserFieldMessenger::GetCurrentValue(G4UIcommand* command)
{
  G4String cv;
  if (command == fStepperCmd)
  {
    cv = fStepperCmd->ConvertToString(fEMfieldSetup->GetStepperType());
  }
  else if (command == fMinStepCmd)
  {
    cv = fMinStepCmd->ConvertToString(fEMfieldSetup->GetMinStep());
  }
  else if (command == fLocalMagFieldCmd)
  {
    cv = fLocalMagFieldCmd->ConvertToString(fEMfieldSetup->GetLocalFieldValue());
  }
  else if (command == fMagFieldCmd)
  {
    cv = fMagFieldCmd->ConvertToString(fEMfieldSetup->GetGlobalFieldValue());
  }
  else
  {
    cv = "";
  }
  return cv;
}