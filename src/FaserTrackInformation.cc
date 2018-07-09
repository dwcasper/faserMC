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
  fSourceTrackID = 0;
  fSourceEnergy = 0;
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
  fSourceTrackID = 0;
  fSourceEnergy = 0;
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
  fSourceTrackID = aTrackInfo->fSourceTrackID;
  fSourceEnergy = aTrackInfo->fSourceEnergy;
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
  fSourceTrackID = aTrackInfo.fSourceTrackID;
  fSourceEnergy = aTrackInfo.fSourceEnergy;
  return *this;
}

void FaserTrackInformation::Print() const
{
  G4cout
    << "Original primary track ID " << fOriginalTrackID << " ("
    << fParticleDefinition->GetParticleName() << ","
    << fOriginalEnergy/GeV << "[GeV])" 
    << " Source track ID = " << fSourceTrackID << " (" << fSourceEnergy/GeV << " GeV)" << G4endl;
}
