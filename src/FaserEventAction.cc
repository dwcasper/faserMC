// adapted from Geant4 example

#include "FaserEventAction.hh"
#include "FaserRunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "FaserSensorHit.hh"
#include "FaserSamplerHit.hh"

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
  FaserDigitizer* fd = new FaserDigitizer("FaserDigitizer");
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

  FaserDigitizer* digiModule = (FaserDigitizer*) digiMan->FindDigitizerModule("FaserDigitizer");
  digiModule->Digitize();  
  
  G4int digiID = digiMan->GetDigiCollectionID("FaserDigitizer/FaserDigiCollection");
  FaserDigiCollection* dc = (FaserDigiCollection*) 
	  digiMan->GetDigiCollection(digiID);
  
  G4int truthID = digiMan->GetHitsCollectionID("FaserSensorHitsCollection");
  FaserSensorHitsCollection* hc = (FaserSensorHitsCollection*) 
	  digiMan->GetHitsCollection(truthID);

  G4int caloID = digiMan->GetHitsCollectionID("FaserSamplerHitsCollection");
  FaserSamplerHitsCollection* sc = (FaserSamplerHitsCollection*)
    digiMan->GetHitsCollection(caloID);

  G4cout << "Nhits: " << hc->entries() << ", Ndigits: " << dc->entries() << ", Samples: " << sc->entries() << G4endl;

 
  fFaserEvent->SetParticles(g4event->GetTrajectoryContainer());
  fFaserEvent->SetHits(hc);
  fFaserEvent->SetSamples(sc);
  fFaserEvent->SetDigis(dc);
  RootEventIO* rootEventIO = RootEventIO::GetInstance();
  rootEventIO->Write(fFaserEvent);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
