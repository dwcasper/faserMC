#ifndef FaserSamplerSD_h
#define FaserSamplerSD_h 1

#include "G4VSensitiveDetector.hh"

#include "FaserSamplerHit.hh"

#include <vector>
#include <set>

class G4Step;
class G4HCofThisEvent;

// sensitive detector class, which is notified of energy deposits in the sensor and creates hits

class FaserSamplerSD : public G4VSensitiveDetector
{
  public:
  
    FaserSamplerSD(const G4String& name,
 		  const G4String& hitsCollectionName);
    virtual ~FaserSamplerSD();

    // methods from base class
    virtual void    Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool  ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void    EndOfEvent(G4HCofThisEvent* hitCollection);

  private:
  
    FaserSamplerHitsCollection* fHitsCollection;
};

#endif
