#include "FaserCaloHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4RunManager.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<FaserCaloHit>* FaserCaloHitAllocator = 0;

const FaserDetectorConstruction* FaserCaloHit::fDetectorConstruction = nullptr;

FaserCaloHit::FaserCaloHit()
  : G4VHit(),
    fPlaneID(-1),
    fTowerID(-1),
    fModuleID(-1),
    fEdep(0.0),
    fGlobalPos(G4ThreeVector()),
    fLocalPos(G4ThreeVector()),
    fOriginTrackID(-1),
    fSourceTrackID(-1)
{}

FaserCaloHit::~FaserCaloHit() 
{ }

FaserCaloHit::FaserCaloHit(const FaserCaloHit& right)
  : G4VHit()
{
  fPlaneID = right.fPlaneID;
  fTowerID = right.fTowerID;
  fModuleID = right.fModuleID;
  fEdep = right.fEdep;
  fGlobalPos = right.fGlobalPos;
  fLocalPos = right.fLocalPos;
  
  fOriginTrackID = right.fOriginTrackID;
  fSourceTrackID = right.fSourceTrackID;
}

const FaserCaloHit& FaserCaloHit::operator=(const FaserCaloHit& right)
{
  fPlaneID = right.fPlaneID;
  fTowerID = right.fTowerID;
  fModuleID = right.fModuleID;
  fEdep =right.fEdep;
  fGlobalPos = right.fGlobalPos;
  fLocalPos = right.fLocalPos;

  fOriginTrackID = right.fOriginTrackID;
  fSourceTrackID = right.fSourceTrackID;

  return *this;
}

G4int FaserCaloHit::operator==(const FaserCaloHit& right)
{
  return ( this == &right) ? 1 : 0;
}

void FaserCaloHit::Draw()
{
  if (fDetectorConstruction == nullptr)
  {
    fDetectorConstruction = static_cast<const FaserDetectorConstruction*>(
					G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  }

  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if (pVVisManager)
  {
    /*
    G4Transform3D trans(fTransform.NetRotation(), fTransform.NetTranslation());
    G4VisAttributes attribs;
    const G4LogicalVolume* strip = fDetectorConstruction->GetTrackerStrip();
    const G4VisAttributes* pVA = strip->GetVisAttributes();
    if(pVA) attribs = *pVA;
    G4Colour colour(1.,0.3,0.);
    attribs.SetColour(colour);
    attribs.SetForceSolid(true);
    pVVisManager->Draw(*strip,attribs,trans);
    */

    G4Circle circle(fGlobalPos);
    circle.SetScreenSize(8.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.0, 0.0, 0.0);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);

  }
}

void FaserCaloHit::Print()
{
  G4cout 
    << "  trackID: " << fSourceTrackID << " planeID: " 
    << (fPlaneID) 
    << " Edep: " 
    << std::setw(7) << G4BestUnit(fEdep, "Energy")
    << " Global: " 
    << std::setw(7) << G4BestUnit(fGlobalPos, "Length")
    << " Local: " 
    << std::setw(7) << G4BestUnit(fLocalPos, "Length")
    << G4endl;
}
