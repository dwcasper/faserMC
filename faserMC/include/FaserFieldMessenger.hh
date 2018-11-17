// copy-pasta adaptation of geant4 extended/field03 example

#ifndef FaserFieldMessenger_h
#define FaserFieldMessenger_h 1

#include "G4UImessenger.hh"

class FaserFieldSetup;
class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class FaserFieldMessenger: public G4UImessenger
{
  public:
    FaserFieldMessenger(FaserFieldSetup* );
    virtual ~FaserFieldMessenger();
 
    virtual void SetNewValue(G4UIcommand*, G4String);

    virtual G4String GetCurrentValue(G4UIcommand*);
 
  private:

    FaserFieldSetup*             fEMfieldSetup;

    G4UIdirectory*             fFieldDir;

    G4UIcmdWithAnInteger*      fStepperCmd;
    G4UIcmdWithADoubleAndUnit* fMagFieldCmd;
    G4UIcmdWithADoubleAndUnit* fLocalMagFieldCmd;
    G4UIcmdWithADoubleAndUnit* fMinStepCmd;
    G4UIcmdWithoutParameter*   fUpdateCmd;
};

#endif
