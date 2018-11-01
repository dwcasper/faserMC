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
  : G4UserStackingAction(), fStage(0)
{

}

FaserStackingAction::~FaserStackingAction()
{;}

G4ClassificationOfNewTrack FaserStackingAction
::ClassifyNewTrack(const G4Track* aTrack)
{
  G4ClassificationOfNewTrack classification = fUrgent;

  if (fStage == 0)
  {
    FaserTrackInformation* info;
    if (aTrack->GetTrackStatus() == fSuspend)
    {
      info = (FaserTrackInformation*)(aTrack->GetUserInformation());
      info->SetTrackingStatus(0);
      info->SetSourceTrackInformation(aTrack);
      classification = fWaiting;
    }
    else if (aTrack->GetParentID() == 0)
    {
      // G4cout << "Creating info from track " << aTrack->GetTrackID() << " in stacking action" << G4endl;
      info = new FaserTrackInformation(aTrack);
      info->SetTrackingStatus(1);
      G4Track* theTrack = (G4Track*)aTrack;
      theTrack->SetUserInformation(info);
      // info->Print();
    }
    else if (aTrack->GetUserInformation() != nullptr && 
	    ((FaserTrackInformation*)aTrack->GetUserInformation())->GetOriginalTrackID() == 0)
        ((FaserTrackInformation*)aTrack->GetUserInformation())->SetOriginalTrackID(aTrack->GetTrackID());
  }

  // if (aTrack->GetParentID() == 0)
  // {
  //   FaserTrackInformation* trackInfo = new FaserTrackInformation(aTrack);
  //   G4cout << "Created primary trackInfo: " << G4endl;
  //   trackInfo->Print();
  //   G4Track* theTrack = (G4Track*)aTrack;
  //   theTrack->SetUserInformation(trackInfo);
  // }

  // return fUrgent;

  return classification;
}

void FaserStackingAction::NewStage()
{
  if (fStage == 0)
  {
    G4cout << "Stage 0 is finished" << G4endl;
  }

  if (stackManager->GetNUrgentTrack() != 0)
  {
    stackManager->TransferStackedTracks(fUrgent, fWaiting);

    stackManager->TransferOneStackedTrack(fWaiting, fUrgent);

    fStage++;
  }
}

void FaserStackingAction::PrepareNewEvent()
{
  fStage = 0;
}
