#include "FaserPhysicsList.hh"
#include "FaserPhysicsMessenger.hh"

FaserPhysicsList::FaserPhysicsList() : G4VModularPhysicsList(), fDarkPhotonMass(default_darkPhoton_mass)
{ 
  fPhysicsMessenger = new FaserPhysicsMessenger(this);
}

FaserPhysicsList::~FaserPhysicsList()
{
  if (fPhysicsMessenger != nullptr) delete fPhysicsMessenger;
}

void FaserPhysicsList::ConstructParticle()
{
  G4cout << "FaserPhysicsList::ConstructParticle() called" << G4endl;
  FaserDarkPhoton::DarkPhotonDefinition(fDarkPhotonMass);
  G4VModularPhysicsList::ConstructParticle();
}

void FaserPhysicsList::SetCuts()
{
  G4VModularPhysicsList::SetCuts();
}

void FaserPhysicsList::ConstructProcess()
{
  G4VModularPhysicsList::ConstructProcess();
}
