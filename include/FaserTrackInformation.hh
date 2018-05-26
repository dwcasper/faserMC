//
// Adapted from RE01 Geant4 example
// 
#ifndef FASERTrackInformation_h
#define FASERTrackInformation_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4Track.hh"
#include "G4Allocator.hh"
#include "G4VUserTrackInformation.hh"

class FaserTrackInformation : public G4VUserTrackInformation
{
public:
  FaserTrackInformation();
  FaserTrackInformation(const G4Track* aTrack);
  FaserTrackInformation(const FaserTrackInformation* aTrackInfo);
  virtual ~FaserTrackInformation();

  inline void* operator new(size_t);
  inline void operator delete(void* aTrackInfo);

  FaserTrackInformation& operator=(const FaserTrackInformation& right);

  G4int GetOriginalTrackID() { return fOriginalTrackID; }
  void  SetOriginalTrackID(G4int id) { fOriginalTrackID = id; }
  G4ParticleDefinition* GetParticleDefinition() { return fParticleDefinition; }
  G4ThreeVector GetOriginalPosition() { return fOriginalPosition; }
  G4ThreeVector GetOriginalMomentum() { return fOriginalMomentum; }
  G4double GetOriginalEnergy() { return fOriginalEnergy; }
  G4double GetOriginalTime() { return fOriginalTime; }

  virtual void Print() const;

private:
  G4int                 fOriginalTrackID;
  G4ParticleDefinition* fParticleDefinition;
  G4ThreeVector         fOriginalPosition;
  G4ThreeVector         fOriginalMomentum;
  G4double              fOriginalEnergy;
  G4double              fOriginalTime;

};

extern G4ThreadLocal
G4Allocator<FaserTrackInformation>* aTrackInformationAllocator;

inline void* FaserTrackInformation::operator new(size_t)
{
  if (!aTrackInformationAllocator)
    aTrackInformationAllocator = new G4Allocator<FaserTrackInformation>;
  return (void*)aTrackInformationAllocator->MallocSingle();
}

inline void FaserTrackInformation::operator delete(void* aTrackInfo)
{ aTrackInformationAllocator->FreeSingle((FaserTrackInformation*)aTrackInfo); }

#endif
