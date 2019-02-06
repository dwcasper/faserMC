// adapted from Geant4 example

#include "FaserEventAction.hh"
#include "FaserRunAction.hh"

#include "G4Event.hh"
#include "FaserEvent.hh"
#include "FaserEventInformation.hh"

#include "G4RunManager.hh"

#include "FaserSensorHit.hh"
#include "FaserCaloHit.hh"

#include "G4DigiManager.hh"
#include "FaserDigitizer.hh"
#include "FaserCaloDigitizer.hh"

#include "FaserDigi.hh"
#include "FaserCaloDigi.hh"

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

void FaserEventAction::EndOfEventAction(const G4Event* g4Evt)
{ 
  FaserEvent* faserEvent = new FaserEvent();

  G4DigiManager* digiMan = G4DigiManager::GetDMpointer();

  FaserDigitizer* trackerDigiModule = (FaserDigitizer*) digiMan->FindDigitizerModule("FaserTrackerDigitizer");
  trackerDigiModule->Digitize();

  FaserDigitizer* samplerDigiModule = (FaserDigitizer*) digiMan->FindDigitizerModule("FaserSamplerDigitizer");
  samplerDigiModule->Digitize();

  FaserCaloDigitizer* caloDigiModule = (FaserCaloDigitizer*) digiMan->FindDigitizerModule("FaserCaloDigitizer");
  caloDigiModule->Digitize();

  G4int trackerDigiID = digiMan->GetDigiCollectionID("FaserTrackerDigiCollection");
  FaserDigiCollection* tdc = (FaserDigiCollection*)
    digiMan->GetDigiCollection(trackerDigiID);

  G4int samplerDigiID = digiMan->GetDigiCollectionID("FaserSamplerDigiCollection");
  FaserDigiCollection* sdc = (FaserDigiCollection*)
    digiMan->GetDigiCollection(samplerDigiID);

  G4int caloDigiID = digiMan->GetDigiCollectionID("FaserCaloDigiCollection");
  FaserCaloDigiCollection* cdc = (FaserCaloDigiCollection*)
    digiMan->GetDigiCollection(caloDigiID);

  G4int trackerID = digiMan->GetHitsCollectionID("FaserTrackerHitsCollection");
  FaserSensorHitsCollection* hc = (FaserSensorHitsCollection*)
    digiMan->GetHitsCollection(trackerID);

  G4int samplerID = digiMan->GetHitsCollectionID("FaserSamplerHitsCollection");
  FaserSensorHitsCollection* sc = (FaserSensorHitsCollection*)
    digiMan->GetHitsCollection(samplerID);

  G4int caloID = digiMan->GetHitsCollectionID("FaserCaloHitsCollection");
  FaserCaloHitsCollection* cc = (FaserCaloHitsCollection*)
    digiMan->GetHitsCollection(caloID);
  

  faserEvent->SetParticles(g4Evt->GetTrajectoryContainer());
  faserEvent->SetTrackerHits(hc);
  faserEvent->SetSamplerHits(sc);
  faserEvent->SetCaloHits(cc);                                                                                                  
  faserEvent->SetTrackerDigis(tdc);
  faserEvent->SetSamplerDigis(sdc);
  faserEvent->SetCaloDigis(cdc);
  faserEvent->SetClusters();
  faserEvent->SetSpacePoints();

  // Attach our event to the G4Event so it can be retrieved elsewhere (for I/O and visualization)
  // G4 will manage the memory after this call
  G4EventManager::GetEventManager()->SetUserInformation(new FaserEventInformation(faserEvent));
  g4Evt->GetUserInformation()->Print();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
