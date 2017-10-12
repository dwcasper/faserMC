#include "FaserSensorHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<FaserSensorHit>* FaserSensorHitAllocator = 0;

FaserSensorHit::FaserSensorHit()
  : G4VHit(),
    fTrackID(-1),
    fPlaneID(-1),
    fModuleID(-1),
    fSensorID(-1),
    fRowID(-1),
    fStripID(-1),
    fEdep(0.0),
    fGlobalPos(G4ThreeVector()),
    fLocalPos(G4ThreeVector())
{}

FaserSensorHit::~FaserSensorHit() {}

FaserSensorHit::FaserSensorHit(const FaserSensorHit& right)
  : G4VHit()
{
  fTrackID = right.fTrackID;
  fPlaneID = right.fPlaneID;
  fModuleID = right.fModuleID;
  fSensorID = right.fSensorID;
  fRowID = right.fRowID;
  fStripID = right.fStripID;
  fEdep = right.fEdep;
  fGlobalPos = right.fGlobalPos;
  fLocalPos = right.fLocalPos;
}

const FaserSensorHit& FaserSensorHit::operator=(const FaserSensorHit& right)
{
  fTrackID = right.fTrackID;
  fPlaneID = right.fPlaneID;
  fModuleID = right.fModuleID;
  fSensorID = right.fSensorID;
  fRowID = right.fRowID;
  fStripID = right.fStripID;
  fEdep =right.fEdep;
  fGlobalPos = right.fGlobalPos;
  fLocalPos = right.fLocalPos;
  
  return *this;
}

G4int FaserSensorHit::operator==(const FaserSensorHit& right)
{
  return ( this == &right) ? 1 : 0;
}

void FaserSensorHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if (pVVisManager)
  {
    G4Circle circle(fGlobalPos);
    circle.SetScreenSize(4.);
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
