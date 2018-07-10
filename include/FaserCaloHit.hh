#ifndef FaserCaloHit_h
#define FaserCaloHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4AffineTransform.hh"
#include "FaserDetectorConstruction.hh"
#include "tls.hh"

// Hit class for the Faser silicon sensor
//

class FaserCaloHit : public G4VHit
{
   public:

    FaserCaloHit();
    FaserCaloHit(const FaserCaloHit&);
    virtual ~FaserCaloHit();

    const FaserCaloHit& operator=(const FaserCaloHit&);
    G4int operator==(const FaserCaloHit&);

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // over-ridden methods
    virtual void Draw();
    virtual void Print();

    // setters
    void SetPlane(G4int plane)               		{ fPlaneID = plane; };
    void SetTower(G4int tower)                  { fTowerID = tower; }
    void SetModule(G4int module)                { fModuleID = module; }
    void SetEdep(G4double de)           	       	{ fEdep = de; };
    void SetGlobalPos(G4ThreeVector globalXYZ) 		{ fGlobalPos = globalXYZ; };
    void SetLocalPos(G4ThreeVector localXYZ)   		{ fLocalPos = localXYZ; };
  
    void SetOriginTrack(G4int track)            { fOriginTrackID = track; }
    void SetSourceTrack(G4int track)            { fSourceTrackID = track; }

    // getters
    G4int Plane() const    	       	      { return fPlaneID; };
    G4int Tower() const                   { return fTowerID; }
    G4int Module() const                  { return fModuleID; }
    G4double Edep() const           			{ return fEdep; };
    G4ThreeVector GlobalPos() const 			{ return fGlobalPos; };
    G4ThreeVector LocalPos() const	  		{ return fLocalPos; };


    G4int OriginTrack() const			        { return fOriginTrackID; }
    G4int SourceTrack() const             { return fSourceTrackID; }

   private:

    G4int fPlaneID;
    G4int fTowerID;
    G4int fModuleID;
    G4double fEdep;
    G4ThreeVector fGlobalPos;
    G4ThreeVector fLocalPos;

    G4int fOriginTrackID;
    G4int fSourceTrackID;

    static const FaserDetectorConstruction* fDetectorConstruction;
};

typedef G4THitsCollection<FaserCaloHit> FaserCaloHitsCollection;

extern G4ThreadLocal G4Allocator<FaserCaloHit>* FaserCaloHitAllocator;

inline void* FaserCaloHit::operator new(size_t)
{
  if (!FaserCaloHitAllocator)
    FaserCaloHitAllocator = new G4Allocator<FaserCaloHit>;
  return (void *) FaserCaloHitAllocator->MallocSingle();
}

inline void FaserCaloHit::operator delete(void* hit)
{
  FaserCaloHitAllocator->FreeSingle((FaserCaloHit*) hit);
}

#endif
