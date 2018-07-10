// adapted from Geant4 example

#include "FaserEventAction.hh"
#include "FaserRunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "FaserSensorHit.hh"
#include "FaserCaloHit.hh"

#include "G4DigiManager.hh"
#include "FaserDigitizer.hh"
#include "FaserDigi.hh"
#include "FaserEvent.hh"

//#include "RootIO.hh"
#include "RootEventIO.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FaserEventAction::FaserEventAction(FaserRunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction), fFaserEvent(nullptr)
{
  FaserDigitizer* fd = new FaserDigitizer("FaserTrackerDigitizer", "FaserTrackerHitsCollection", "FaserTrackerDigiCollection");
  G4DigiManager::GetDMpointer()->AddNewModule(fd);
  fd = new FaserDigitizer("FaserSamplerDigitizer", "FaserSamplerHitsCollection", "FaserSamplerDigiCollection");
  G4DigiManager::GetDMpointer()->AddNewModule(fd);

} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FaserEventAction::~FaserEventAction()
{
  if (fFaserEvent != nullptr) delete fFaserEvent;
  fFaserEvent = nullptr;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserEventAction::BeginOfEventAction(const G4Event* event)
{    
  if (fFaserEvent != nullptr) delete fFaserEvent;
  fFaserEvent = new FaserEvent(event->GetEventID());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserEventAction::EndOfEventAction(const G4Event* g4event)
{ 

  G4DigiManager* digiMan = G4DigiManager::GetDMpointer();

  FaserDigitizer* trackerDigiModule = (FaserDigitizer*) digiMan->FindDigitizerModule("FaserTrackerDigitizer");
  trackerDigiModule->Digitize();  

  FaserDigitizer* samplerDigiModule = (FaserDigitizer*) digiMan->FindDigitizerModule("FaserSamplerDigitizer");
  samplerDigiModule->Digitize();  

  
  G4int trackerDigiID = digiMan->GetDigiCollectionID("FaserTrackerDigiCollection");
  FaserDigiCollection* tdc = (FaserDigiCollection*) 
	  digiMan->GetDigiCollection(trackerDigiID);

  G4int samplerDigiID = digiMan->GetDigiCollectionID("FaserSamplerDigiCollection");
  FaserDigiCollection* sdc = (FaserDigiCollection*)
    digiMan->GetDigiCollection(samplerDigiID);
  
  G4int trackerID = digiMan->GetHitsCollectionID("FaserTrackerHitsCollection");
  FaserSensorHitsCollection* hc = (FaserSensorHitsCollection*) 
	  digiMan->GetHitsCollection(trackerID);

  G4int samplerID = digiMan->GetHitsCollectionID("FaserSamplerHitsCollection");
  FaserSensorHitsCollection* sc = (FaserSensorHitsCollection*)
    digiMan->GetHitsCollection(samplerID);

  G4int caloID = digiMan->GetHitsCollectionID("FaserCaloHitsCollection");
  FaserCaloHitsCollection* cc = (FaserCaloHitsCollection*)
    digiMan->GetHitsCollection(caloID);

  fFaserEvent->SetParticles(g4event->GetTrajectoryContainer());
  fFaserEvent->SetTrackerHits(hc);
  fFaserEvent->SetSamplerHits(sc);
  fFaserEvent->SetCaloHits(cc);
  fFaserEvent->SetTrackerDigis(tdc);
  fFaserEvent->SetSamplerDigis(sdc);
  fFaserEvent->SetClusters();

  G4cout << "Strip hits: " << fFaserEvent->TrackerHits().size() << ", Strip digits: " << fFaserEvent->TrackerDigis().size() << ", Strip clusters: " << fFaserEvent->Clusters().size() << G4endl;
  G4cout << "Sampler hits: " << fFaserEvent->SamplerHits().size() << ", Sampler digits: " << fFaserEvent->SamplerDigis().size() << G4endl;
  G4cout << "Calo hit: " << fFaserEvent->CaloHits().size() << G4endl; 

  RootEventIO* rootEventIO = RootEventIO::GetInstance();
  rootEventIO->Write(fFaserEvent);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
