#include "FaserPhysicsList.hh"

FaserPhysicsList::FaserPhysicsList() : G4VModularPhysicsList(), fDarkPhotonMass(default_darkPhoton_mass)
{ }

FaserPhysicsList::~FaserPhysicsList()
{

}

void FaserPhysicsList::ConstructParticle()
{
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
