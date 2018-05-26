// Adapted from Geant4 example

#include "FaserActionInitialization.hh"
#include "FaserPrimaryGeneratorAction.hh"
#include "FaserRunAction.hh"
#include "FaserEventAction.hh"
#include "FaserSteppingAction.hh"
#include "FaserStackingAction.hh"
#include "FaserTrackingAction.hh"

#include "TROOT.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FaserActionInitialization::FaserActionInitialization()
 : G4VUserActionInitialization()
{
  // kludge to avoid Open-GL/ROOT collision
  gROOT->GetInterpreter();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FaserActionInitialization::~FaserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserActionInitialization::BuildForMaster() const
{
  FaserRunAction* runAction = new FaserRunAction;
  SetUserAction(runAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserActionInitialization::Build() const
{
  SetUserAction(new FaserPrimaryGeneratorAction);

  FaserRunAction* runAction = new FaserRunAction;
  SetUserAction(runAction);
  
  FaserEventAction* eventAction = new FaserEventAction(runAction);
  SetUserAction(eventAction);
  
  SetUserAction(new FaserSteppingAction(eventAction));

  SetUserAction(new FaserStackingAction);

  SetUserAction(new FaserTrackingAction);
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
