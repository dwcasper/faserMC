#include "FaserSensorHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4RunManager.hh"
#include "G4MTRunManager.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<FaserSensorHit>* FaserSensorHitAllocator = 0;

const FaserDetectorConstruction* FaserSensorHit::fDetectorConstruction = nullptr;

FaserSensorHit::FaserSensorHit()
  : G4VHit(),
    fPlaneID(-1),
    fModuleID(-1),
    fSensorID(-1),
    fRowID(-1),
    fStripID(-1),
    fEdep(0.0),
    fGlobalPos(G4ThreeVector()),
    fLocalPos(G4ThreeVector()),
    fTransform(G4AffineTransform()),
    fTrackID(-1),
    fEnergy(0),
    fOriginTrackID(-1)
{}

FaserSensorHit::~FaserSensorHit() 
{ }

FaserSensorHit::FaserSensorHit(const FaserSensorHit& right)
  : G4VHit()
{
  fPlaneID = right.fPlaneID;
  fModuleID = right.fModuleID;
  fSensorID = right.fSensorID;
  fRowID = right.fRowID;
  fStripID = right.fStripID;
  fEdep = right.fEdep;
  fGlobalPos = right.fGlobalPos;
  fLocalPos = right.fLocalPos;
  fTransform = right.fTransform;
  
  fTrackID = right.fTrackID;
  fEnergy = right.fEnergy;
  fOriginTrackID = right.fOriginTrackID;
}

const FaserSensorHit& FaserSensorHit::operator=(const FaserSensorHit& right)
{
  fPlaneID = right.fPlaneID;
  fModuleID = right.fModuleID;
  fSensorID = right.fSensorID;
  fRowID = right.fRowID;
  fStripID = right.fStripID;
  fEdep =right.fEdep;
  fGlobalPos = right.fGlobalPos;
  fLocalPos = right.fLocalPos;
  fTransform = right.fTransform;

  fTrackID = right.fTrackID;
  fEnergy = right.fEnergy;
  fOriginTrackID = right.fOriginTrackID;

  return *this;
}

G4int FaserSensorHit::operator==(const FaserSensorHit& right)
{
  return ( this == &right) ? 1 : 0;
}

void FaserSensorHit::Draw()
{
  if (fDetectorConstruction == nullptr)
  {
    G4MTRunManager* master = G4MTRunManager::GetMasterRunManager();
    if (master != nullptr)
    {
      fDetectorConstruction = static_cast<const FaserDetectorConstruction*>(master->GetUserDetectorConstruction());
    }
    else
    {
      fDetectorConstruction = static_cast<const FaserDetectorConstruction*>(
					G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    }
  }
  if (fDetectorConstruction == nullptr)
  {
    G4cout << "FaserSensorHit::Draw unable to obtain detector construction pointer" << G4endl;
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

void FaserSensorHit::Print()
{
  G4cout 
    << "  trackID: " << fTrackID << " sensorID: " 
    << (10000000*fPlaneID + 1000000*fModuleID + 100000*fSensorID + 10000*fRowID + fStripID) 
    << " Edep: " 
    << std::setw(7) << G4BestUnit(fEdep, "Energy")
    << " Global: " 
    << std::setw(7) << G4BestUnit(fGlobalPos, "Length")
    << " Local: " 
    << std::setw(7) << G4BestUnit(fLocalPos, "Length")
    << G4endl;
}
