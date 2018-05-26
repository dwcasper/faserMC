#include "FaserPrimaryGenerator.hh"
#include "FaserGeneratorMessenger.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Box.hh"
#include "FaserDetectorConstruction.hh"
#include "G4RunManager.hh"
#include "Randomize.hh"
#include "G4PrimaryParticle.hh"
#include "CLHEP/Units/PhysicalConstants.h"
#include "G4Event.hh"

FaserPrimaryGenerator::FaserPrimaryGenerator() 
  : G4VPrimaryGenerator(), fGeneratorMessenger(nullptr), fDecayVolume(nullptr), fDetectorConstruction(nullptr)
{
  fGeneratorMessenger = new FaserGeneratorMessenger(this);
}

FaserPrimaryGenerator::~FaserPrimaryGenerator()
{
  delete fGeneratorMessenger;
}

void FaserPrimaryGenerator::GeneratePrimaryVertex(G4Event* event)
{ 
  G4double decaySizeX = 0;
  G4double decaySizeY = 0;
  G4double decaySizeZ = 0;
  G4double decayLength = 0;

  if (fDecayVolume == nullptr)
  {
    G4LogicalVolume* decayLV = G4LogicalVolumeStore::GetInstance()->GetVolume("DecayVolume");
    if ( decayLV ) fDecayVolume = dynamic_cast<const G4Box*>(decayLV->GetSolid());
  }

  if ( fDecayVolume != nullptr ) {
    decaySizeX = fDecayVolume->GetXHalfLength();
    decaySizeY = fDecayVolume->GetYHalfLength();
    decayLength = fDecayVolume->GetZHalfLength()*2;
  }  
  else  
  {
    G4ExceptionDescription msg;
    msg << "Decay volume of box shape not found.\n"; 
    msg << "Perhaps you have changed geometry.\n";
    msg << "Primary particles will start at (x,y) = (0,0) and decay immediately.";
    G4Exception("FaserPrimaryGeneratorAction::GeneratePrimaries()",
		"MyCode0002",JustWarning,msg);
  }

  if ( fDetectorConstruction == nullptr )
  {
      fDetectorConstruction = static_cast<const FaserDetectorConstruction*>(
					    G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  }

  if ( fDetectorConstruction != nullptr )
  {
    decaySizeZ = std::max(fDetectorConstruction->getDecayVolumeLength(), fDetectorConstruction->getPlanePitch());
  }
  else 
  {
    G4ExceptionDescription msg;
    msg << "Detector length not found.\n"; 
    msg << "Perhaps you have changed geometry.\n";
    msg << "Primary particles will start at z = 0.";
    G4Exception("FaserPrimaryGeneratorAction::GeneratePrimaries()",
		"MyCode0003",JustWarning,msg);
  }

  G4double x0 = 2 * decaySizeX * (G4UniformRand() - 0.5);
  G4double y0 = 2 * decaySizeY * (G4UniformRand() - 0.5);
  G4double z0 = -decaySizeZ + 1.0*cm;

  G4ThreeVector position(x0, y0, z0);
  G4double time = 0.0 * s;
  G4PrimaryVertex* vertex = new G4PrimaryVertex(position, time);

  G4ThreeVector momentum = position - sourcePosition;
  G4double logP = log(minPrimaryMomentum) + log(maxPrimaryMomentum/minPrimaryMomentum)*G4UniformRand();
  momentum.setMag(exp(logP));
  G4ParticleDefinition* particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("darkphoton");
  G4PrimaryParticle* primary = new G4PrimaryParticle(particleDefinition, momentum.x(), momentum.y(), momentum.z());

  G4double decayProperTime = (1.0-G4UniformRand())*primary->GetMass()*decayLength/(primary->GetTotalMomentum() * CLHEP::c_light);
  //G4cout << "Proper time set for decay: " << decayProperTime / s << G4endl;
  primary->SetProperTime(decayProperTime);

  vertex->SetPrimary(primary);
  event->AddPrimaryVertex(vertex);


}
