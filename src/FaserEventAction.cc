// adapted from Geant4 example

#include "FaserEventAction.hh"
#include "FaserRunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FaserEventAction::FaserEventAction(FaserRunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction)
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FaserEventAction::~FaserEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserEventAction::BeginOfEventAction(const G4Event*)
{    

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserEventAction::EndOfEventAction(const G4Event*)
{   

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
