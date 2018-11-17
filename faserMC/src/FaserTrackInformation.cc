#include "FaserTrackInformation.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

G4ThreadLocal G4Allocator<FaserTrackInformation>* 
                            aTrackInformationAllocator = 0;

FaserTrackInformation::FaserTrackInformation()
  : G4VUserTrackInformation()
{
  fOriginalTrackID = 0;
  fParticleDefinition = nullptr;
  fOriginalPosition = G4ThreeVector(0.,0.,0.);
  fOriginalMomentum = G4ThreeVector(0.,0.,0.);
  fOriginalEnergy = 0.;
  fOriginalTime = 0.;
  fTrackingStatus = 1;
  fSourceTrackID = -1;
  fSourceDefinition = nullptr;
  fSourcePosition = G4ThreeVector(0.,0.,0.);
  fSourceMomentum = G4ThreeVector(0.,0.,0.);
  fSourceEnergy = 0.;
  fSourceTime = 0.;
  fSuspendedStepID = -1;
}

FaserTrackInformation::FaserTrackInformation(const G4Track* aTrack)
  : G4VUserTrackInformation()
{
  fOriginalTrackID = aTrack->GetTrackID();
  fParticleDefinition = aTrack->GetDefinition();
  fOriginalPosition = aTrack->GetPosition();
  fOriginalMomentum = aTrack->GetMomentum();
  fOriginalEnergy = aTrack->GetTotalEnergy();
  fOriginalTime = aTrack->GetGlobalTime();
  fTrackingStatus = 1;

  fSourceTrackID = -1;
  fSourceDefinition = nullptr;
  fSourcePosition = G4ThreeVector(0.,0.,0.);
  fSourceMomentum = G4ThreeVector(0.,0.,0.);
  fSourceTime = 0.;
  fSourceEnergy = 0;
  fSuspendedStepID = -1;
}

FaserTrackInformation::FaserTrackInformation(const FaserTrackInformation* aTrackInfo)
  : G4VUserTrackInformation()
{
  fOriginalTrackID = aTrackInfo->fOriginalTrackID;
  fParticleDefinition = aTrackInfo->fParticleDefinition;
  fOriginalPosition = aTrackInfo->fOriginalPosition;
  fOriginalMomentum = aTrackInfo->fOriginalMomentum;
  fOriginalEnergy = aTrackInfo->fOriginalEnergy;
  fOriginalTime = aTrackInfo->fOriginalTime;
  fTrackingStatus = aTrackInfo->fTrackingStatus;
  fSourceTrackID = aTrackInfo->fSourceTrackID;
  fSourceDefinition = aTrackInfo->fSourceDefinition;
  fSourcePosition = aTrackInfo->fSourcePosition;
  fSourceMomentum = aTrackInfo->fSourceMomentum;
  fSourceEnergy = aTrackInfo->fSourceEnergy;
  fSourceTime = aTrackInfo->fSourceTime;
  fSuspendedStepID = -1;
}

FaserTrackInformation::~FaserTrackInformation()
{;}

FaserTrackInformation& FaserTrackInformation
::operator =(const FaserTrackInformation& aTrackInfo)
{
  fOriginalTrackID = aTrackInfo.fOriginalTrackID;
  fParticleDefinition = aTrackInfo.fParticleDefinition;
  fOriginalPosition = aTrackInfo.fOriginalPosition;
  fOriginalMomentum = aTrackInfo.fOriginalMomentum;
  fOriginalEnergy = aTrackInfo.fOriginalEnergy;
  fOriginalTime = aTrackInfo.fOriginalTime;
  fTrackingStatus = aTrackInfo.fTrackingStatus;
  fSourceTrackID = aTrackInfo.fSourceTrackID;
  fSourceDefinition = aTrackInfo.fSourceDefinition;
  fSourcePosition = aTrackInfo.fSourcePosition;
  fSourceMomentum = aTrackInfo.fSourceMomentum;
  fSourceEnergy = aTrackInfo.fSourceEnergy;
  fSourceTime = aTrackInfo.fSourceTime;
  fSuspendedStepID = -1;
  return *this;
}

void FaserTrackInformation::SetSourceTrackInformation(const G4Track* aTrack)
{
  fSourceTrackID = aTrack->GetTrackID();
  fSourceDefinition = aTrack->GetDefinition();
  fSourcePosition = aTrack->GetPosition();
  fSourceMomentum = aTrack->GetMomentum();
  fSourceEnergy = aTrack->GetTotalEnergy();
  fSourceTime = aTrack->GetGlobalTime();
}

void FaserTrackInformation::Print() const
{
  G4cout 
    << "Source track ID " << fSourceTrackID << " (" 
    << fSourceDefinition->GetParticleName() << ","
    << fSourceEnergy/GeV << "[GeV]) at " << fSourcePosition << G4endl;
  G4cout
    << "Original primary track ID " << fOriginalTrackID << " (" 
    << fParticleDefinition->GetParticleName() << ","
    << fOriginalEnergy/GeV << "[GeV])" << G4endl;
}
