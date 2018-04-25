#include "FaserDigi.hh"

G4ThreadLocal G4Allocator<FaserDigi> *faserDigiAllocator = 0;

FaserDigi::FaserDigi()
  : G4VDigi(),
    fPlaneID(-1),
    fModuleID(-1),
    fSensorID(-1),
    fRowID(-1),
    fStripID(-1),
    fCharge(0.0)
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
  fCharge = right.fCharge;
}

const FaserDigi& FaserDigi::operator=(const FaserDigi& right)
{
  fPlaneID = right.fPlaneID;
  fModuleID = right.fModuleID;
  fSensorID = right.fSensorID;
  fRowID = right.fRowID;
  fStripID = right.fStripID;
  fCharge = right.fCharge;

  return *this;
}

int FaserDigi::operator==(const FaserDigi& right) const
{
  return ( fPlaneID == right.fPlaneID 
	&& fModuleID == right.fModuleID 
	&& fSensorID == right.fSensorID
        && fRowID == right.fRowID
	&& fStripID == right.fStripID
	&& fCharge == right.fCharge ) ? 1 : 0;
}

void FaserDigi::Draw()
{ }

void FaserDigi::Print()
{ }
