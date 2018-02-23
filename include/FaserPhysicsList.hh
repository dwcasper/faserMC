#ifndef FaserPhysicsList_hh
#define FaserPhysicsList_hh 1

#include "G4VModularPhysicsList.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

#include "FaserDarkPhoton.hh"

class FaserPhysicsList : public G4VModularPhysicsList
{
public:
  FaserPhysicsList();
  virtual ~FaserPhysicsList();

  virtual void ConstructParticle();

  virtual void SetCuts();

  virtual void ConstructProcess();

  void setDarkPhotonMass(G4double mass) { if (FaserDarkPhoton::DarkPhoton() == nullptr) fDarkPhotonMass = mass; }
  G4double getDarkPhotonMass() { return (FaserDarkPhoton::DarkPhoton() != nullptr ? FaserDarkPhoton::DarkPhoton()->GetPDGMass() : fDarkPhotonMass); }

  static constexpr G4double default_darkPhoton_mass = 100.0 * MeV;

private:
  G4double fDarkPhotonMass;


};
#endif
