//

#ifndef FaserPhysicsMessenger_h
#define FaserPhysicsMessenger_h 1

#include "G4UImessenger.hh"

class FaserPhysicsList;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class FaserPhysicsMessenger: public G4UImessenger
{
  public:
    FaserPhysicsMessenger(FaserPhysicsList* );
    virtual ~FaserPhysicsMessenger();
 
    virtual void SetNewValue(G4UIcommand*, G4String);
 
  private:

    FaserPhysicsList*          fPhysicsList;

    G4UIdirectory*             fPhysicsDir;

    G4UIcmdWithADoubleAndUnit* fDarkPhotonMassCmd;

};

#endif
