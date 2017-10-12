#ifndef FaserSensorHit_h
#define FaserSensorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

// Hit class for the Faser silicon sensor
//
// Stores:
//
// trackID
// planeID
// moduleID
// sensorID
// energy deposit
// global position
// local position

class FaserSensorHit : public G4VHit
{
   public:

    FaserSensorHit();
    FaserSensorHit(const FaserSensorHit&);
    virtual ~FaserSensorHit();

    const FaserSensorHit& operator=(const FaserSensorHit&);
    G4int operator==(const FaserSensorHit&);

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // over-ridden methods
    virtual void Draw();
    virtual void Print();

    // setters
    void SetTrackID(G4int track)               { fTrackID = track; };
    void SetPlaneID(G4int plane)               { fPlaneID = plane; };
    void SetModuleID(G4int module)             { fModuleID = module; };
    void SetSensorID(G4int sensor)             { fSensorID = sensor; };
    void SetEdep(G4double de)                  { fEdep = de; };
    void SetGlobalPos(G4ThreeVector globalXYZ) { fGlobalPos = globalXYZ; };
    void SetLocalPos(G4ThreeVector localXYZ)   { fLocalPos = localXYZ; };

    // getters
    G4int GetTrackID() const           { return fTrackID; };
    G4int GetPlaneID() const           { return fPlaneID; };
    G4int GetModuleID() const          { return fModuleID; };
    G4int GetSensorID() const          { return fSensorID; };
    G4double GetEdep() const           { return fEdep; };
    G4ThreeVector GetGlobalPos() const { return fGlobalPos; };
    G4ThreeVector GetLocalPos() const  { return fLocalPos; };


   private:

    G4int fTrackID;
    G4int fPlaneID;
    G4int fModuleID;
    G4int fSensorID;
    G4double fEdep;
    G4ThreeVector fGlobalPos;
    G4ThreeVector fLocalPos;

};

typedef G4THitsCollection<FaserSensorHit> FaserSensorHitsCollection;

extern G4ThreadLocal G4Allocator<FaserSensorHit>* FaserSensorHitAllocator;

inline void* FaserSensorHit::operator new(size_t)
{
  if (!FaserSensorHitAllocator)
    FaserSensorHitAllocator = new G4Allocator<FaserSensorHit>;
  return (void *) FaserSensorHitAllocator->MallocSingle();
}

inline void FaserSensorHit::operator delete(void* hit)
{
  FaserSensorHitAllocator->FreeSingle((FaserSensorHit*) hit);
}

#endif
