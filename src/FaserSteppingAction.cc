// adapted from Geant4 example

#include "FaserSteppingAction.hh"
#include "FaserEventAction.hh"
#include "FaserDetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FaserSteppingAction::FaserSteppingAction(FaserEventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FaserSteppingAction::~FaserSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserSteppingAction::UserSteppingAction(const G4Step* step)
{
  if (nullptr == step) return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

