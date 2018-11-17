// adapted from Geant4 example

#include "FaserPrimaryGeneratorAction.hh"
#include "FaserPrimaryGenerator.hh"
#include "FaserDetectorConstruction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FaserPrimaryGeneratorAction::FaserPrimaryGeneratorAction()
  : G4VUserPrimaryGeneratorAction(),
  fPrimaryGenerator(nullptr)
{
  fPrimaryGenerator = new FaserPrimaryGenerator();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FaserPrimaryGeneratorAction::~FaserPrimaryGeneratorAction()
{
  delete fPrimaryGenerator;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of each event

  fPrimaryGenerator->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

