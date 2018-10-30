#include "FaserDigiMessenger.hh"
#include "FaserDigitizer.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

FaserDigiMessenger::FaserDigiMessenger(FaserDigitizer* digitizer)
  : fDigitizer(digitizer)
{
  new G4UnitDefinition("femtoCoulomb", "fC", "Electric charge", coulomb/1e15);

  digiDirectory = new G4UIdirectory("/faser/digi/");
  digiDirectory->SetGuidance("Sensor parameters for charge-spreading and conversion to ADC");

  cmd_digi_chargeSpreadSigma = new G4UIcmdWithADoubleAndUnit("/faser/digi/chargeSpreadSigma", this);
  cmd_digi_chargeSpreadSigma->SetGuidance("Standard deviation of Gaussian charge sharing in um");
  cmd_digi_chargeSpreadSigma->SetDefaultUnit("um");
  cmd_digi_chargeSpreadSigma->SetUnitCandidates("um mm");
  cmd_digi_chargeSpreadSigma->SetParameterName("chargeSpreadSigma", true, true);
  cmd_digi_chargeSpreadSigma->AvailableForStates(G4State_PreInit, G4State_Idle);
  
  cmd_digi_threshold = new G4UIcmdWithADoubleAndUnit("/faser/digi/threshold", this);
  cmd_digi_threshold->SetGuidance("Charge threshold in fC (events must be strictly greater)");
  cmd_digi_threshold->SetDefaultUnit("fC");
  cmd_digi_threshold->SetParameterName("threshold", true, true);
  cmd_digi_threshold->AvailableForStates(G4State_PreInit, G4State_Idle);

  cmd_digi_chargeSmear = new G4UIcmdWithADoubleAndUnit("/faser/digi/chargeSmear", this);
  cmd_digi_chargeSmear->SetDefaultUnit("eplus");
  cmd_digi_chargeSmear->SetUnitCandidates("eplus fC");
  cmd_digi_chargeSmear->SetGuidance("Average value of noise per channel");
  cmd_digi_chargeSmear->SetParameterName("chargeSmear", true, true);
  cmd_digi_chargeSmear->AvailableForStates(G4State_PreInit, G4State_Idle);

  cmd_digi_chargeNorm = new G4UIcmdWithADouble("/faser/digi/chargeNorm", this);
  cmd_digi_chargeNorm->SetGuidance("Fudge factor to control over charge level");
  cmd_digi_chargeNorm->SetParameterName("chargeNorm", true, true);
  cmd_digi_chargeNorm->AvailableForStates(G4State_PreInit, G4State_Idle);
}

FaserDigiMessenger::~FaserDigiMessenger()
{
  if (cmd_digi_chargeSpreadSigma) delete cmd_digi_chargeSpreadSigma;
  if (cmd_digi_threshold) delete cmd_digi_threshold;
  if (cmd_digi_chargeSmear) delete cmd_digi_chargeSmear;
  if (cmd_digi_chargeNorm) delete cmd_digi_chargeNorm;
}

void FaserDigiMessenger::SetNewValue(G4UIcommand* command, G4String newValues)
{
  if (command == cmd_digi_chargeSpreadSigma)
  {
    fDigitizer->SetChargeSpreadSigma(cmd_digi_chargeSpreadSigma->GetNewDoubleValue(newValues));
  }
  else if (command == cmd_digi_threshold)
  {
    fDigitizer->SetThreshold(cmd_digi_threshold->GetNewDoubleValue(newValues));
  }
  else if (command == cmd_digi_chargeSmear)
  {
    fDigitizer->SetChargeSmear(cmd_digi_chargeSmear->GetNewDoubleValue(newValues));
  }
  else if (command == cmd_digi_chargeNorm)
  {
    fDigitizer->SetChargeNorm(cmd_digi_chargeNorm->GetNewDoubleValue(newValues));    
  }
}

G4String FaserDigiMessenger::GetCurrentValue(G4UIcommand* command)
{
  G4String cv;

  if (command == cmd_digi_chargeSpreadSigma)
  {
    cv = cmd_digi_chargeSpreadSigma->ConvertToString(fDigitizer->GetChargeSpreadSigma(), "um");
  }
  else if (command == cmd_digi_threshold)
  {
    cv = cmd_digi_threshold->ConvertToString(fDigitizer->GetThreshold(), "fC");
  }
  else if (command == cmd_digi_chargeSmear)
  {
    cv = cmd_digi_chargeSmear->ConvertToString(fDigitizer->GetChargeSmear(), "eplus");
  }
  else if (command == cmd_digi_chargeNorm)
  {
    cv = cmd_digi_chargeNorm->ConvertToString(fDigitizer->GetChargeNorm());
  }
  return cv;
}
