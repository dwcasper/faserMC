#include "FaserGeneratorMessenger.hh"
#include "FaserPrimaryGenerator.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAString.hh"

FaserGeneratorMessenger::FaserGeneratorMessenger(FaserPrimaryGenerator* primaryGenerator)
  : fPrimaryGenerator(primaryGenerator)
{
  generatorDirectory = new G4UIdirectory("/faser/generator/");
  generatorDirectory->SetGuidance("Primary generator initialization parameters.");

  cmd_gen_minPrimaryMomentum = new G4UIcmdWithADoubleAndUnit("/faser/generator/minMomentum", this);
  cmd_gen_minPrimaryMomentum->SetGuidance("Minimum momentum for decaying primary particle.");
  cmd_gen_minPrimaryMomentum->SetParameterName("minPrimaryMomentum", true, true);
  cmd_gen_minPrimaryMomentum->SetDefaultUnit("GeV");
  cmd_gen_minPrimaryMomentum->SetUnitCandidates("GeV MeV TeV");
  cmd_gen_minPrimaryMomentum->AvailableForStates(G4State_PreInit, G4State_Idle);

  cmd_gen_maxPrimaryMomentum = new G4UIcmdWithADoubleAndUnit("/faser/generator/maxMomentum", this);
  cmd_gen_maxPrimaryMomentum->SetGuidance("Maximum momentum for decaying primary particle.");
  cmd_gen_maxPrimaryMomentum->SetParameterName("maxPrimaryMomentum", true, true);
  cmd_gen_maxPrimaryMomentum->SetDefaultUnit("GeV");
  cmd_gen_maxPrimaryMomentum->SetUnitCandidates("GeV MeV TeV");
  cmd_gen_maxPrimaryMomentum->AvailableForStates(G4State_PreInit, G4State_Idle);

  cmd_gen_sourcePosition = new G4UIcmdWith3VectorAndUnit("/faser/generator/sourcePosition", this);
  cmd_gen_sourcePosition->SetGuidance("Position of decaying particle source (in FASER coordinate system).");
  cmd_gen_sourcePosition->SetParameterName("sourcePositionX", "sourcePositionY", "sourcePositionZ", true);
  cmd_gen_sourcePosition->SetDefaultUnit("m");
  cmd_gen_sourcePosition->SetUnitCandidates("m cm km");
  cmd_gen_sourcePosition->AvailableForStates(G4State_PreInit, G4State_Idle);

  cmd_gen_particle = new G4UIcmdWithAString("/faser/generator/particleName", this);
  cmd_gen_particle->SetGuidance("Name of primary generator particle.");
  cmd_gen_particle->SetParameterName("particleName", true, true);
  cmd_gen_particle->AvailableForStates(G4State_PreInit, G4State_Idle);

  cmd_gen_dump = new G4UIcmdWithoutParameter("/faser/generator/dump", this);
 
  fPrimaryGenerator->setMinPrimaryMomentum( FaserPrimaryGenerator::default_minPrimaryMomentum );
  fPrimaryGenerator->setMaxPrimaryMomentum( FaserPrimaryGenerator::default_maxPrimaryMomentum );
  fPrimaryGenerator->setSourcePosition( G4ThreeVector(0.0, 0.0, -FaserPrimaryGenerator::default_sourceDistance) );
  fPrimaryGenerator->setParticleName( FaserPrimaryGenerator::default_particleName );
}

FaserGeneratorMessenger::~FaserGeneratorMessenger()
{
  if (cmd_gen_particle) delete cmd_gen_particle;
  if (cmd_gen_minPrimaryMomentum) delete cmd_gen_minPrimaryMomentum;
  if (cmd_gen_maxPrimaryMomentum) delete cmd_gen_maxPrimaryMomentum;
  if (cmd_gen_sourcePosition) delete cmd_gen_sourcePosition;
  if (generatorDirectory) delete generatorDirectory;
}

void FaserGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValues)
{
  if (command == cmd_gen_minPrimaryMomentum) 
    {
      fPrimaryGenerator->setMinPrimaryMomentum( cmd_gen_minPrimaryMomentum->GetNewDoubleValue(newValues) );
    }
  else if (command == cmd_gen_maxPrimaryMomentum)
    {
      fPrimaryGenerator->setMaxPrimaryMomentum( cmd_gen_maxPrimaryMomentum->GetNewDoubleValue(newValues) );
    }
  else if (command == cmd_gen_sourcePosition)
    {
      fPrimaryGenerator->setSourcePosition( cmd_gen_sourcePosition->GetNew3VectorValue(newValues) );
    }
  else if (command == cmd_gen_particle)
    {
      fPrimaryGenerator->setParticleName( newValues );
    }
  else if (command == cmd_gen_dump)
    {
      G4cout << "Generator Parameters:" << G4endl;
      G4cout << "Primary particle: " << GetCurrentValue(cmd_gen_particle) << G4endl;
      G4cout << "Minimum Primary Momentum: " << GetCurrentValue(cmd_gen_minPrimaryMomentum) << G4endl;
      G4cout << "Maximum Primary Momentum: " << GetCurrentValue(cmd_gen_maxPrimaryMomentum) << G4endl;
      G4cout << "Source position (in FASER coordinate system): " << GetCurrentValue(cmd_gen_sourcePosition) << G4endl;
    }
}

G4String FaserGeneratorMessenger::GetCurrentValue(G4UIcommand* command)
{
  G4String cv;
  
  if (command == cmd_gen_minPrimaryMomentum)
    {
      cv = cmd_gen_minPrimaryMomentum->ConvertToString(fPrimaryGenerator->getMinPrimaryMomentum(), "GeV");
    }
  else if (command == cmd_gen_maxPrimaryMomentum)
    {
      cv = cmd_gen_maxPrimaryMomentum->ConvertToString(fPrimaryGenerator->getMaxPrimaryMomentum(), "TeV");
    }
  else if (command == cmd_gen_sourcePosition)
    {
      cv = cmd_gen_sourcePosition->ConvertToString(fPrimaryGenerator->getSourcePosition(), "m");
    }
  else if (command == cmd_gen_particle)
    {
      cv = fPrimaryGenerator->getParticleName();
    }
  return cv;
}
