#include "FaserDigi.hh"

G4ThreadLocal G4Allocator<FaserDigi> *faserDigiAllocator = 0;

FaserDigi::FaserDigi()
  : G4VDigi(),
    fTrackID(-1),
    fPlaneID(-1),
    fModuleID(-1),
    fSensorID(-1),
    fRowID(-1),
    fStripID(-1),
    fEdep(0.0),
    fGlobalPos(G4ThreeVector()),
    fLocalPos(G4ThreeVector())
{ }

FaserDigi::~FaserDigi()
{ }

FaserDigi::FaserDigi(const FaserDigi& right)
  : G4VDigi()
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

const FaserDigi& FaserDigi::operator=(const FaserDigi& right)
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

  return *this;
}

int FaserDigi::operator==(const FaserDigi& right) const
{
  return ( this == &right ) ? 1 : 0;
}

void FaserDigi::Draw()
{ }

void FaserDigi::Print()
{ }
