#include "FaserCaloDigi.hh"
#include "FaserDetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"
#include "G4Box.hh"
#include <cmath>

const FaserDetectorConstruction* FaserCaloDigi::fDetectorConstruction = nullptr;

G4ThreadLocal G4Allocator<FaserCaloDigi> *faserCaloDigiAllocator = 0;

FaserCaloDigi::FaserCaloDigi()
  : G4VDigi(),
    fModuleID(-1),
    fTowerID(-1),
    fTransform(G4AffineTransform()),
    fEnergy(0.0)
{ }

FaserCaloDigi::~FaserCaloDigi()
{ }

FaserCaloDigi::FaserCaloDigi(const FaserCaloDigi& right)
  : G4VDigi()
{
  fTowerID = right.fTowerID;
  fModuleID = right.fModuleID;
  fTransform = right.fTransform;
  fEnergy = right.fEnergy;
  fTracks = right.fTracks;
}

const FaserCaloDigi& FaserCaloDigi::operator=(const FaserCaloDigi& right)
{
  fTowerID = right.fTowerID;
  fModuleID = right.fModuleID;
  fTransform = right.fTransform;
  fEnergy = right.fEnergy;
  fTracks = right.fTracks;

  return *this;
}

int FaserCaloDigi::operator==(const FaserCaloDigi& right) const
{
  return ( fTowerID == right.fTowerID 
	&& fModuleID == right.fModuleID 
	&& fEnergy == right.fEnergy ) ? 1 : 0;
}

G4Colour FaserCaloDigi::QtoC()
{
  if (fEnergy <=0) return G4Colour(0,0,0);
  G4double q = std::min(1.0, 0.05 + 0.95 * log10(std::max(1.0,fEnergy/100.0))/log10(3500000/100.0));
  G4double h = 300.0 * (1 - q); // from 300 at q=0 to 0 at q=1 (purple->blue->green->...->red)
  G4double x = 1.0 - fabs(std::fmod(h/60.0, 2.0)-1.0);
  if (h < 60) return G4Colour(1.0,x,0);
  if (h < 120) return G4Colour(x,1.0,0);
  if (h < 180) return G4Colour(0,1.0,x);
  if (h < 240) return G4Colour(0,x,1.0);
  if (h < 300) return G4Colour(x,0,1.0);
  return G4Colour(1.0,0,x);
}

void FaserCaloDigi::Draw()
{ 
  if (fEnergy <= 0) return;
  if (fDetectorConstruction == nullptr)
  {
    fDetectorConstruction = static_cast<const FaserDetectorConstruction*>(
					  G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  }
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if (pVVisManager != nullptr)
  {
    // wasted a couple days figuring out that the rotation (but not translation!) needs to be inverted for correct display...

    G4double logScale = std::min(1.0, 0.05 + 0.95 * log10(std::max(1.0,fEnergy/100.0))/log10(3500000/100.0));
    G4double maxHeight = fDetectorConstruction->GetCaloMaxZ() - fDetectorConstruction->GetCaloMinZ();
    G4double height = logScale * maxHeight;
    // G4cout << "energy = " << fEnergy << " logScale = " << logScale << " maxH = " << maxHeight << " height = " << height << G4endl;
    const G4LogicalVolume* tower = fDetectorConstruction->GetCaloTower();
    const G4Box* b = (const G4Box*) tower->GetSolid();
    G4Box* bin = new G4Box("caloDigit", b->GetXHalfLength(), b->GetYHalfLength(), height/2.0);
    G4ThreeVector translation = fTransform.NetTranslation();
    translation.setZ(fDetectorConstruction->GetCaloMinZ() + height/2.0);
    G4Transform3D trans(fTransform.NetRotation().inverse(), translation);
    G4VisAttributes attribs;
    const G4VisAttributes* pVA = tower->GetVisAttributes();
    if (pVA != nullptr) attribs = *pVA;
    attribs.SetColour(QtoC());
    attribs.SetForceSolid(true);
    pVVisManager->Draw(*bin, attribs, trans);
    delete bin;
  }

}

void FaserCaloDigi::Print()
{ }
