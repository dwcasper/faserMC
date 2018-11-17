// adapted from Geant4 example
#pragma once

#include "G4UserEventAction.hh"
#include "globals.hh"

class FaserRunAction;
class FaserEvent;
class FaserDrawer;

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
    FaserRunAction*     fRunAction;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


    
