// adapted from Geant4 example

#include "FaserRunAction.hh"
#include "FaserPrimaryGeneratorAction.hh"
#include "FaserDetectorConstruction.hh"
// #include "FaserGeometry.hh"
#include "FaserRun.hh"
#include "RootEventIO.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FaserRunAction::FaserRunAction()
  : G4UserRunAction()
{ 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FaserRunAction::~FaserRunAction()
{
  if (IsMaster()) 
  {
    RootEventIO* root = RootEventIO::GetInstance();
    root->Close();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Run* FaserRunAction::GenerateRun()
{
  return new FaserRun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserRunAction::BeginOfRunAction(const G4Run*)
{ 
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserRunAction::EndOfRunAction(const G4Run* run)
{
  
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  //const FaserPrimaryGeneratorAction* generatorAction
  // = static_cast<const FaserPrimaryGeneratorAction*>
  //   (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
        
  // Print
  //  
  if (IsMaster()) {
    G4cout
     << G4endl
     << "--------------------End of Global Run-----------------------";
  }
  else {
    G4cout
     << G4endl
     << "--------------------End of Local Run------------------------";
  }
  
  G4cout
     << G4endl
     << " The run consists of " << nofEvents << " events"
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



