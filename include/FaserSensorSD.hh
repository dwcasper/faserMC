#ifndef FaserSensorSD_h
#define FaserSensorSD_h 1

#include "G4VSensitiveDetector.hh"

#include "FaserSensorHit.hh"

#include <vector>
#include <set>

class G4Step;
class G4HCofThisEvent;

// sensitive detector class, which is notified of energy deposits in the sensor and creates hits

class FaserSensorSD : public G4VSensitiveDetector
{
  public:
  
    FaserSensorSD(const G4String& name,
 		  const G4String& hitsCollectionName);
    virtual ~FaserSensorSD();

    // methods from base class
    virtual void    Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool  ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void    EndOfEvent(G4HCofThisEvent* hitCollection);

  private:
  
    FaserSensorHitsCollection* fHitsCollection;
    FaserSensorHitsCollection* fTruthCollection;
    std::set<std::pair<G4int, G4int>> fTruthSet;

    G4int fNPlanes;
    G4int fNModules;
    G4int fNSensors;
    G4int fNRows;
};

#endif
