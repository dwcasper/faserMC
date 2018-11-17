
#include "FaserPhysicsMessenger.hh"

#include "FaserPhysicsList.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FaserPhysicsMessenger::FaserPhysicsMessenger(FaserPhysicsList* physicsList)
 : G4UImessenger(),
   fPhysicsList(physicsList),
   fPhysicsDir(nullptr),
   fDarkPhotonMassCmd(nullptr)
{
  fPhysicsDir = new G4UIdirectory("/faser/physics/");
  fPhysicsDir->SetGuidance("Faser physics list control.");

  fDarkPhotonMassCmd = new G4UIcmdWithADoubleAndUnit("/faser/physics/setDarkPhotonMass",this);
  fDarkPhotonMassCmd->SetGuidance("Define the dark photon mass.");
  fDarkPhotonMassCmd->SetParameterName("mAprime",false,false);
  fDarkPhotonMassCmd->SetDefaultUnit("MeV");
  //fDarkPhotonMassCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FaserPhysicsMessenger::~FaserPhysicsMessenger()
{
  if (fDarkPhotonMassCmd) delete fDarkPhotonMassCmd;
  if (fPhysicsDir) delete fPhysicsDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserPhysicsMessenger::SetNewValue( G4UIcommand* command, G4String newValue)
{
  if( command == fDarkPhotonMassCmd )
  {
    fPhysicsList->setDarkPhotonMass(fDarkPhotonMassCmd->GetNewDoubleValue(newValue));
    // Check the value
    G4cout << "Set dark photon mass to " <<
      fPhysicsList->getDarkPhotonMass() / MeV << " MeV " << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
