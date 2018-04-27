#include "FaserDigiMessenger.hh"
#include "FaserDigitizer.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

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
}

FaserDigiMessenger::~FaserDigiMessenger()
{
  if (cmd_digi_chargeSpreadSigma) delete cmd_digi_chargeSpreadSigma;
  if (cmd_digi_threshold) delete cmd_digi_threshold;
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
  return cv;
}
