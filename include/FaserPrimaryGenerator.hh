#ifndef FaserPrimaryGenerator_hh
#define FaserPrimaryGenerator_hh 1

#include "G4VPrimaryGenerator.hh"

#include "G4SystemOfUnits.hh"
#include "globals.hh"

class G4Event;
class FaserGeneratorMessenger;
class FaserDetectorConstruction;
class G4Box;

class FaserPrimaryGenerator : public G4VPrimaryGenerator
{
public:
  FaserPrimaryGenerator();
  virtual ~FaserPrimaryGenerator();

public:
  virtual void GeneratePrimaryVertex(G4Event*);

  void setMinPrimaryMomentum(G4double pMin) { minPrimaryMomentum = pMin; }
  void setMaxPrimaryMomentum(G4double pMax) { maxPrimaryMomentum = pMax; }
  void setSourcePosition(const G4ThreeVector& pos) { sourcePosition = pos; }

  G4double getMinPrimaryMomentum() const { return minPrimaryMomentum; }
  G4double getMaxPrimaryMomentum() const { return maxPrimaryMomentum; }
  G4ThreeVector getSourcePosition() const { return sourcePosition; }

  static constexpr G4double default_minPrimaryMomentum = 100.0*GeV;
  static constexpr G4double default_maxPrimaryMomentum = 7.0*TeV;
  static constexpr G4double default_sourceDistance = 450.0*m;

private:
  FaserGeneratorMessenger* fGeneratorMessenger;
  const G4Box* fEnvelope;
  const FaserDetectorConstruction* fDetectorConstruction; 

  double minPrimaryMomentum;
  double maxPrimaryMomentum;
  G4ThreeVector sourcePosition;
};
#endif
