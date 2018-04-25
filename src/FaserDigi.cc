#include "FaserDigi.hh"

G4ThreadLocal G4Allocator<FaserDigi> *faserDigiAllocator = 0;

FaserDigi::FaserDigi()
  : G4VDigi(),
    fPlaneID(-1),
    fModuleID(-1),
    fSensorID(-1),
    fRowID(-1),
    fStripID(-1),
    fADC(0.0)
{ }

FaserDigi::~FaserDigi()
{ }

FaserDigi::FaserDigi(const FaserDigi& right)
  : G4VDigi()
{
  fPlaneID = right.fPlaneID;
  fModuleID = right.fModuleID;
  fSensorID = right.fSensorID;
  fRowID = right.fRowID;
  fStripID = right.fStripID;
  fADC = right.fADC;
}

const FaserDigi& FaserDigi::operator=(const FaserDigi& right)
{
  fPlaneID = right.fPlaneID;
  fModuleID = right.fModuleID;
  fSensorID = right.fSensorID;
  fRowID = right.fRowID;
  fStripID = right.fStripID;
  fADC = right.fADC;

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
