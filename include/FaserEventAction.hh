// adapted from Geant4 example

#ifndef FaserEventAction_h
#define FaserEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class FaserRunAction;
class FaserEvent;

/// Event action class
///

class FaserEventAction : public G4UserEventAction
{
  public:
    FaserEventAction(FaserRunAction* runAction);
    virtual ~FaserEventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

  private:
    FaserRunAction* fRunAction;
    FaserEvent*     fFaserEvent;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
