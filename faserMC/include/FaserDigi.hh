#ifndef FaserDigi_h
#define FaserDigi_h 1

#include "G4VDigi.hh"
#include "G4TDigiCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4AffineTransform.hh"
#include "G4Colour.hh"

class FaserDetectorConstruction;

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
    virtual G4Colour QtoC();

  private:

    G4int fPlaneID;
    G4int fModuleID;
    G4int fSensorID;
    G4int fRowID;
    G4int fStripID;
    G4AffineTransform fTransform;
    G4double fCharge;
    std::map<G4int, G4double> fTracks;

    static const FaserDetectorConstruction* fDetectorConstruction;

  public:

    inline void SetPlane(G4int plane)		{ fPlaneID = plane; };
    inline void SetModule(G4int module) 	{ fModuleID = module; };
    inline void SetSensor(G4int sensor)	        { fSensorID = sensor; };
    inline void SetRow(G4int row)		{ fRowID = row; };
    inline void SetStrip(G4int strip)		{ fStripID = strip; };
    inline void SetTransform(G4AffineTransform transform) 
                                                { fTransform = transform; }
    inline void SetCharge(G4double charge)	{ fCharge = charge; };
    inline void AddTrack(G4int trackID, G4double charge)
                                                { fTracks[trackID] += charge; } 

    inline G4int Plane() const		        { return fPlaneID; };
    inline G4int Module() const	       	        { return fModuleID; };
    inline G4int Sensor() const		        { return fSensorID; };
    inline G4int Row() const		        { return fRowID; };
    inline G4int Strip() const 		        { return fStripID; };
    inline G4AffineTransform Transform() const     
                                                { return fTransform; }
    inline G4double Charge() const		{ return fCharge; };
    inline std::map<G4int, G4double> Tracks() const     
                                                { return fTracks; }
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
