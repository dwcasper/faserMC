#ifndef FaserCaloDigi_h
#define FaserCaloDigi_h 1

#include "G4VDigi.hh"
#include "G4TDigiCollection.hh"
#include "G4Allocator.hh"
#include "G4AffineTransform.hh"
#include "G4Colour.hh"

class FaserDetectorConstruction;

class FaserCaloDigi: public G4VDigi
{
  public:
    FaserCaloDigi();
    virtual ~FaserCaloDigi();

    FaserCaloDigi(const FaserCaloDigi&);
    const FaserCaloDigi& operator=(const FaserCaloDigi&);
    int operator==(const FaserCaloDigi&) const;

    inline void* operator new(size_t);
    inline void operator delete(void*);

    virtual void Draw();
    virtual void Print();
    virtual G4Colour QtoC();

  private:

    G4int fModuleID;
    G4int fTowerID;
    G4AffineTransform fTransform;
    G4double fEnergy;
    std::map<G4int, G4double> fTracks;

    static const FaserDetectorConstruction* fDetectorConstruction;

  public:

    inline void SetModule(G4int module)		{ fModuleID = module; };
    inline void SetTower(G4int tower)		  { fTowerID = tower; };
    inline void SetTransform(G4AffineTransform transform) 
                                          { fTransform = transform; }
    inline void SetEnergy(G4double energy){ fEnergy = energy; };
    inline void AddTrack(G4int trackID, G4double energy)
                                          { fTracks[trackID] += energy; fEnergy += energy; } 

    inline G4int Module() const		        { return fModuleID; };
    inline G4int Tower() const	       	  { return fTowerID; };
    inline G4AffineTransform Transform() const     
                                          { return fTransform; }
    inline G4double Energy() const	    	{ return fEnergy; };
    inline std::map<G4int, G4double> Tracks() const     
                                          { return fTracks; }
};

typedef G4TDigiCollection<FaserCaloDigi> FaserCaloDigiCollection;

extern G4ThreadLocal G4Allocator<FaserCaloDigi>* faserCaloDigiAllocator;

inline void* FaserCaloDigi::operator new(size_t)
{
  if (!faserCaloDigiAllocator)
    faserCaloDigiAllocator = new G4Allocator<FaserCaloDigi>;
  return (void *) faserCaloDigiAllocator->MallocSingle();
}

inline void FaserCaloDigi::operator delete(void* aDigi)
{
  faserCaloDigiAllocator->FreeSingle((FaserCaloDigi*) aDigi);
}

#endif
