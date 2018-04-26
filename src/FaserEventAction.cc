// adapted from Geant4 example

#include "FaserEventAction.hh"
#include "FaserRunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "G4DigiManager.hh"
#include "FaserDigitizer.hh"
#include "FaserDigi.hh"

#include "RootIO.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FaserEventAction::FaserEventAction(FaserRunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction)
{
  FaserDigitizer* fd = new FaserDigitizer("FaserDigitizer");
  G4DigiManager::GetDMpointer()->AddNewModule(fd);
} 

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

  G4DigiManager* digiMan = G4DigiManager::GetDMpointer();

  FaserDigitizer* digiModule = (FaserDigitizer*) digiMan->FindDigitizerModule("FaserDigitizer");
  digiModule->Digitize();  
  
  G4int digiID = digiMan->GetDigiCollectionID("FaserDigitizer/FaserDigiCollection");
  FaserDigiCollection* dc = (FaserDigiCollection*) digiMan->GetDigiCollection(digiID);
  if ( dc ) 
  {
    RootIO::GetInstance()->Write(dc);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
