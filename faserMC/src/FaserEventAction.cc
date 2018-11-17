// adapted from Geant4 example

#include "FaserEventAction.hh"
#include "FaserRunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "FaserSensorHit.hh"
#include "FaserCaloHit.hh"

#include "G4DigiManager.hh"
#include "FaserDigitizer.hh"
#include "FaserCaloDigitizer.hh"

#include "FaserDigi.hh"
#include "FaserCaloDigi.hh"

#include "FaserDrawer.hh"

#include "TVector3.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FaserEventAction::FaserEventAction(FaserRunAction* runAction)
  : G4UserEventAction()
  , fRunAction(runAction)
{
  FaserDigitizer* fd = new FaserDigitizer("FaserTrackerDigitizer", "FaserTrackerHitsCollection", "FaserTrackerDigiCollection");
  G4DigiManager::GetDMpointer()->AddNewModule(fd);
  fd = new FaserDigitizer("FaserSamplerDigitizer", "FaserSamplerHitsCollection", "FaserSamplerDigiCollection");
  G4DigiManager::GetDMpointer()->AddNewModule(fd);

  FaserCaloDigitizer* fcd = new FaserCaloDigitizer("FaserCaloDigitizer", "FaserCaloHitsCollection", "FaserCaloDigiCollection");
  G4DigiManager::GetDMpointer()->AddNewModule(fcd);
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FaserEventAction::~FaserEventAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserEventAction::BeginOfEventAction(const G4Event*)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserEventAction::EndOfEventAction(const G4Event*)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
