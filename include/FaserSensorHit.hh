#ifndef FaserSensorHit_h
#define FaserSensorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4AffineTransform.hh"
#include "FaserDetectorConstruction.hh"
#include "tls.hh"

// Hit class for the Faser silicon sensor
//

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
    void SetPlane(G4int plane)               		{ fPlaneID = plane; };
    void SetModule(G4int module) 	            	{ fModuleID = module; };
    void SetSensor(G4int sensor)      	       	{ fSensorID = sensor; };
    void SetRow(G4int row)                   		{ fRowID = row; };
    void SetStrip(G4int strip)	               	{ fStripID = strip; };
    void SetEdep(G4double de)           	       	{ fEdep = de; };
    void SetGlobalPos(G4ThreeVector globalXYZ) 		{ fGlobalPos = globalXYZ; };
    void SetLocalPos(G4ThreeVector localXYZ)   		{ fLocalPos = localXYZ; };
    void SetTransform(G4AffineTransform transform)      { fTransform = transform; };  
  
    void SetTrack(G4int track)			        { fTrackID = track; };
    void SetEnergy(G4double te)   		        { fEnergy = te; };

    void SetOriginTrack(G4int track)                    { fOriginTrackID = track; }

    // getters
    G4int Plane() const    	       		        { return fPlaneID; };
    G4int Module() const          			{ return fModuleID; };
    G4int Sensor() const          			{ return fSensorID; };
    G4int Row() const	             		        { return fRowID; };
    G4int Strip() const    	       		        { return fStripID; };
    G4double Edep() const           			{ return fEdep; };
    G4ThreeVector GlobalPos() const 			{ return fGlobalPos; };
    G4ThreeVector LocalPos() const	  		{ return fLocalPos; };
    G4AffineTransform Transform() const                 { return fTransform; }

    G4int Track() const				        { return fTrackID; };
    G4double Energy() const                             { return fEnergy; }
  
    G4int OriginTrack() const			        { return fOriginTrackID; };

   private:

    G4int fPlaneID;
    G4int fModuleID;
    G4int fSensorID;
    G4int fRowID;
    G4int fStripID;
    G4double fEdep;
    G4ThreeVector fGlobalPos;
    G4ThreeVector fLocalPos;
    G4AffineTransform fTransform;

    G4int fTrackID;
    G4double fEnergy;

    G4int fOriginTrackID;

    static const FaserDetectorConstruction* fDetectorConstruction;
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
