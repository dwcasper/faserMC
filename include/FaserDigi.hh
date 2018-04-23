#ifndef FaserDigi_h
#define FaserDigi_h 1

#include "G4VDigi.hh"
#include "G4TDigiCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class FaserDigi: public G4VDigi
{
  public:
    FaserDigi();
    virtual ~FaserDigi();

    FaserDigi(const FaserDigi&);
    const FaserDigi& operator=(const FaserDigi&);
    int operator==(const FaserDigi&) const;

    inline void* operator new(size_t);
    inline void operator delete(void*);

    virtual void Draw();
    virtual void Print();

  private:

    G4int fPlaneID;
    G4int fModuleID;
    G4int fSensorID;
    G4int fRowID;
    G4int fStripID;
    G4int fEdep;

  public:

    inline void SetPlaneID(G4int plane)		{ fPlaneID = plane; };
    inline void SetModuleID(G4int module) 	{ fModuleID = module; };
    inline void SetSensorID(G4int sensor)	{ fSensorID = sensor; };
    inline void SetRowID(G4int row)		{ fRowID = row; };
    inline void SetStripID(G4int strip)		{ fStripID = strip; };
    inline void SetEdep(G4double edep)		{ fEdep = edep; };

    inline G4int GetPlaneID() 			{ return fPlaneID; };
    inline G4int GetModuleID()			{ return fModuleID; };
    inline G4int GetSensorID()			{ return fSensorID; };
    inline G4int GetRowID()			{ return fRowID; };
    inline G4int GetStripID()			{ return fStripID; };
    inline G4double GetEdep()			{ return fEdep; };

};

typedef G4TDigiCollection<FaserDigi> FaserDigiCollection;

extern G4ThreadLocal G4Allocator<FaserDigi>* faserDigiAllocator;

inline void* FaserDigi::operator new(size_t)
{
  if (!faserDigiAllocator)
    faserDigiAllocator = new G4Allocator<FaserDigi>;
  return (void *) faserDigiAllocator->MallocSingle();
}

inline void FaserDigi::operator delete(void* aDigi)
{
  faserDigiAllocator->FreeSingle((FaserDigi*) aDigi);
}

#endif
