#ifndef FaserDigiMessenger_h
#define FaserDigiMessenger_h

#include "G4UImessenger.hh"
#include "globals.hh"

class FaserDigitizer;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;

class FaserDigiMessenger : public G4UImessenger
{
  public:
    FaserDigiMessenger(FaserDigitizer* digitizer);
    ~FaserDigiMessenger();

  public:
    virtual void SetNewValue(G4UIcommand* command, G4String newValues);
    virtual G4String GetCurrentValue(G4UIcommand* command);

  private:
    FaserDigitizer* fDigitizer;

  private:
    G4UIdirectory*		digiDirectory;
    G4UIcmdWithADoubleAndUnit*	cmd_digi_chargeSpreadSigma;
    G4UIcmdWithADoubleAndUnit*	cmd_digi_threshold;
    G4UIcmdWithADouble*	        cmd_digi_chargeSmear;
    G4UIcmdWithADouble*	        cmd_digi_chargeSmearNorm;
};

#endif
