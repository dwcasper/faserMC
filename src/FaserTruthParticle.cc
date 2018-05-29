#include "FaserTruthParticle.hh"

FaserTruthParticle::FaserTruthParticle()
  : FaserTruthParticle(0, 0, 0, G4ThreeVector(), G4ThreeVector(), 0.0)
{ }

FaserTruthParticle::FaserTruthParticle(G4int trackID, G4int parentID, G4int pdgCode, G4ThreeVector vertex, G4ThreeVector momentum,
				       G4double energy)
  : 
  fTrackID { trackID }, 
  fParentID { parentID },
  fPdgCode { pdgCode },
  fVertex { vertex },
  fMomentum { momentum },
  fEnergy { energy }
{ }

FaserTruthParticle::~FaserTruthParticle()
{ }
