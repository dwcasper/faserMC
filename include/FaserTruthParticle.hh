#ifndef FASERTRUTHPARTICLE_HH
#define FASERTRUTHPARTICLE_HH 1

#include "G4ThreeVector.hh"

class FaserTruthParticle
{
public:
  FaserTruthParticle();
  FaserTruthParticle(G4int trackID, G4int parentID, G4int pdgCode, G4ThreeVector vertex, G4ThreeVector momentum, G4double energy);
  virtual ~FaserTruthParticle();

  G4int TrackID() { return fTrackID; }
  G4int ParentID() { return fParentID; }
  G4int PdgCode() { return fPdgCode; }
  G4ThreeVector Vertex() { return fVertex; }
  G4ThreeVector Momentum() { return fMomentum; }
  G4double Energy() { return fEnergy; }

private:
  G4int fTrackID;
  G4int fParentID;
  G4int fPdgCode;
  G4ThreeVector fVertex;
  G4ThreeVector fMomentum;
  G4double fEnergy;
};


#endif
