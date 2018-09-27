#ifndef FaserDarkPhoton_hh
#define FaserDarkPhoton_hh 1

#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

class FaserDarkPhoton : public G4ParticleDefinition
{
private:
  static FaserDarkPhoton* theDarkPhoton;

  FaserDarkPhoton(
	           const G4String&     aName,        G4double            mass,
	           G4double            width,        G4double            charge,   
	           G4int               iSpin,        G4int               iParity,    
	           G4int               iConjugation, G4int               iIsospin,   
	           G4int               iIsospin3,    G4int               gParity,
	           const G4String&     pType,        G4int               lepton,      
	           G4int               baryon,       G4int               encoding,
	           G4bool              stable,       G4double            lifetime,
	           G4DecayTable        *decaytable );

  virtual ~FaserDarkPhoton();

public:

  static FaserDarkPhoton* DarkPhotonDefinition( G4double mass );

  static FaserDarkPhoton* DarkPhoton( );

};
#endif
