#include "FaserEvent.hh"

#include "G4ParticleTable.hh"
#include "G4VTrajectoryPoint.hh"

FaserEvent::FaserEvent(G4int eventNumber)
  : fEventNumber(eventNumber)
{}

FaserEvent::FaserEvent()
  : FaserEvent(0)
{ }

FaserEvent::~FaserEvent()
{ 
  for (auto p : fParticles)
  {
    delete p;
  }
  fParticles.clear();
}

void FaserEvent::SetParticles(G4TrajectoryContainer* particles)
{
  if (particles == nullptr) return;
  for (size_t i = 0; i < particles->size(); i++)
  {
    G4VTrajectory& t = *((*particles)[i]);
    G4int pdgCode = t.GetPDGEncoding();
    G4double mass = G4ParticleTable::GetParticleTable()->FindParticle(pdgCode)->GetPDGMass();
    G4ThreeVector momentum = t.GetInitialMomentum();
    G4double energy = sqrt(momentum.mag2() + mass*mass);
    FaserTruthParticle* p = new FaserTruthParticle(t.GetTrackID(), t.GetParentID(), pdgCode,
						   (*(t.GetPoint(0))).GetPosition(), momentum, energy); 
    fParticles.push_back(p);
  }
}
