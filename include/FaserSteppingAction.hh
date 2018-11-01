// adapted from Geant4 example

#ifndef FaserSteppingAction_h
#define FaserSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class FaserEventAction;

class G4LogicalVolume;

/// Stepping action class
/// 

class FaserSteppingAction : public G4UserSteppingAction
{
  public:
    FaserSteppingAction(FaserEventAction* eventAction);
    virtual ~FaserSteppingAction();

    // method from the base class
    virtual void UserSteppingAction(const G4Step*);

  private:
    FaserEventAction*  fEventAction;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
