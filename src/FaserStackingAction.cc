#include "FaserStackingAction.hh"
#include "FaserTrackInformation.hh"

#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

FaserStackingAction::FaserStackingAction()
  : G4UserStackingAction()
{

}

FaserStackingAction::~FaserStackingAction()
{;}

G4ClassificationOfNewTrack FaserStackingAction
::ClassifyNewTrack(const G4Track* aTrack)
{
  if (aTrack->GetParentID() == 0)
  {
    FaserTrackInformation* trackInfo = new FaserTrackInformation(aTrack);
    G4cout << "Created primary trackInfo: " << G4endl;
    trackInfo->Print();
    G4Track* theTrack = (G4Track*)aTrack;
    theTrack->SetUserInformation(trackInfo);
  }
  else if (aTrack->GetUserInformation() != nullptr && 
	   ((FaserTrackInformation*)aTrack->GetUserInformation())->GetOriginalTrackID() == 0)
    ((FaserTrackInformation*)aTrack->GetUserInformation())->SetOriginalTrackID(aTrack->GetTrackID());

  return fUrgent;
}
