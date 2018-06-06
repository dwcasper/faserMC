// adapted from Geant4 example

#include "FaserSteppingAction.hh"
#include "FaserEventAction.hh"
#include "FaserDetectorConstruction.hh"
#include "FaserTrackInformation.hh"

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

void FaserSteppingAction::UserSteppingAction(const G4Step* theStep)
{
  if (nullptr == theStep) return;
  G4Track* theTrack = theStep->GetTrack();
  if (theTrack->GetTrackStatus() != fAlive) { return; }
  
  // Get the region
  G4StepPoint* thePrePoint = theStep->GetPreStepPoint();
  G4LogicalVolume* thePreLV = thePrePoint->GetPhysicalVolume()->GetLogicalVolume();
  G4Region* thePreR = thePreLV->GetRegion();
  G4StepPoint* thePostPoint = theStep->GetPostStepPoint();
  G4LogicalVolume* thePostLV = thePostPoint->GetPhysicalVolume()->GetLogicalVolume();
  G4Region* thePostR = thePostLV->GetRegion();
  if (thePreR->GetName() != "DefaultRegionForTheWorld" &&
      thePostR->GetName() == "DefaultRegionForTheWorld")
    theTrack->SetTrackStatus(fStopAndKill);
  if (thePreR->GetName() != "Calorimeter" &&
      thePostR->GetName() == "Calorimeter")
  {
    auto info = dynamic_cast<FaserTrackInformation*>(theTrack->GetUserInformation());
    if (info->GetSourceTrackID() == 0) 
    {
      info->SetSourceTrackID(theTrack->GetTrackID());
      info->SetSourceEnergy(theTrack->GetTotalEnergy());
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

