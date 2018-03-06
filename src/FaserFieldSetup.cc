// copied and adapted from geant4 extended/field03 example

#include "FaserFieldSetup.hh"
#include "FaserFieldMessenger.hh"

#include "G4MagneticField.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ChordFinder.hh"

#include "G4ExplicitEuler.hh"
#include "G4ImplicitEuler.hh"
#include "G4SimpleRunge.hh"
#include "G4SimpleHeum.hh"
#include "G4ClassicalRK4.hh"
#include "G4HelixExplicitEuler.hh"
#include "G4HelixImplicitEuler.hh"
#include "G4HelixSimpleRunge.hh"
#include "G4CashKarpRKF45.hh"
#include "G4RKG3_Stepper.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FaserFieldSetup::FaserFieldSetup()
 : fFieldManager(0),
   fLocalFieldManager(0),
   fChordFinder(0),
   fLocalChordFinder(0),
   fEquation(0),
   fLocalEquation(0),
   fMagneticField(0),
   fLocalMagneticField(0),
   fStepper(0),
   fLocalStepper(0),
   fFieldMessenger(0)
{
  fMagneticField = new G4UniformMagField(G4ThreeVector(0.0*tesla,
                                                       0.0, 
                                                       0.0));
  fLocalMagneticField = new G4UniformMagField(G4ThreeVector(0.0*tesla,
                                                            0.0, 
                                                            0.0));

  fFieldMessenger = new FaserFieldMessenger(this);
 
  fEquation = new G4Mag_UsualEqRhs(fMagneticField);
  fLocalEquation = new G4Mag_UsualEqRhs(fLocalMagneticField);
 
  fMinStep     = 0.25*mm ; // minimal step of 1 mm is default
  fStepperType = 4 ;       // ClassicalRK4 is default stepper

  fFieldManager = GetGlobalFieldManager();
  fLocalFieldManager = new G4FieldManager();

  UpdateField();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FaserFieldSetup::~FaserFieldSetup()
{
  delete fMagneticField;
  delete fChordFinder;
  delete fStepper;
  delete fFieldMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserFieldSetup::UpdateField()
{
  // It must be possible to call 'again' - e.g. to choose an alternative stepper
  //   has been chosen, or in case other changes have been made.

  // 1. First clean up previous state.
  delete fChordFinder;
  fChordFinder= nullptr;
  delete fLocalChordFinder;
  fLocalChordFinder= nullptr;

  G4cout<<"FaserFieldSetup::UpdateField> The minimal step is equal to "
        << fMinStep/mm <<" mm"<<G4endl;
  G4cout<<"                            Stepper Type chosen = " << fStepperType
        << G4endl;

  // 2. Create the steppers ( Note: this also deletes the previous ones. )
  SetStepper();

  // 3. Create the chord finder(s)
  fChordFinder = new G4ChordFinder(fMagneticField, fMinStep, fStepper);
  fLocalChordFinder = new G4ChordFinder(fLocalMagneticField,
                                        fMinStep,fLocalStepper);

  fFieldManager->SetChordFinder(fChordFinder);
  fLocalFieldManager->SetChordFinder(fLocalChordFinder);

  // 4. Ensure that the field is updated (in Field manager & equation)
  fFieldManager->SetDetectorField(fMagneticField);
  fLocalFieldManager->SetDetectorField(fLocalMagneticField);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// this is unnecessary for FASER, but it presumably works, so keep it...
void FaserFieldSetup::SetStepper()
{
  delete fStepper;
  fStepper= nullptr;

  delete fLocalStepper; 
  fLocalStepper= nullptr;

  switch ( fStepperType )
  {
    case 0:
      fStepper = new G4ExplicitEuler( fEquation );
      fLocalStepper = new G4ExplicitEuler( fLocalEquation );
      G4cout<<"G4ExplicitEuler is called"<<G4endl;
      break;
    case 1:
      fStepper = new G4ImplicitEuler( fEquation );
      fLocalStepper = new G4ImplicitEuler( fLocalEquation );
      G4cout<<"G4ImplicitEuler is called"<<G4endl;
      break;
    case 2:
      fStepper = new G4SimpleRunge( fEquation );
      fLocalStepper = new G4SimpleRunge( fLocalEquation );
      G4cout<<"G4SimpleRunge is called"<<G4endl;
      break;
    case 3:
      fStepper = new G4SimpleHeum( fEquation );
      fLocalStepper = new G4SimpleHeum( fLocalEquation );
      G4cout<<"G4SimpleHeum is called"<<G4endl;
      break;
    case 4:
      fStepper = new G4ClassicalRK4( fEquation );
      fLocalStepper = new G4ClassicalRK4( fLocalEquation );
      G4cout<<"G4ClassicalRK4 (default) is called"<<G4endl;
      break;
    case 5:
      fStepper = new G4HelixExplicitEuler( fEquation );
      fLocalStepper = new G4HelixExplicitEuler( fLocalEquation );
      G4cout<<"G4HelixExplicitEuler is called"<<G4endl;
      break;
    case 6:
      fStepper = new G4HelixImplicitEuler( fEquation );
      fLocalStepper = new G4HelixImplicitEuler( fLocalEquation );
      G4cout<<"G4HelixImplicitEuler is called"<<G4endl;
      break;
    case 7:
      fStepper = new G4HelixSimpleRunge( fEquation );
      fLocalStepper = new G4HelixSimpleRunge( fLocalEquation );
      G4cout<<"G4HelixSimpleRunge is called"<<G4endl;
      break;
    case 8:
      fStepper = new G4CashKarpRKF45( fEquation );
      fLocalStepper = new G4CashKarpRKF45( fLocalEquation );
      G4cout<<"G4CashKarpRKF45 is called"<<G4endl;
      break;
    case 9:
      fStepper = new G4RKG3_Stepper( fEquation );
      fLocalStepper = new G4RKG3_Stepper( fLocalEquation );
      G4cout<<"G4RKG3_Stepper is called"<<G4endl;
      break;
    default: fStepper = 0;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserFieldSetup::SetGlobalFieldValue(G4double fieldStrength)
{
  G4ThreeVector fieldSetVec(0.0, fieldStrength, 0.0);
  SetGlobalFieldValue( fieldSetVec );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserFieldSetup::SetGlobalFieldValue(G4ThreeVector fieldVector)
{
  if(fMagneticField) delete fMagneticField;

  if(fieldVector != G4ThreeVector(0.,0.,0.))
  {
    fMagneticField = new  G4UniformMagField(fieldVector);
  }
  else
  {
    // If the new field's value is Zero, then
    // setting the pointer to zero ensures
    // that it is not used for propagation.
    fMagneticField = 0;
  }

  // Either
  //   - UpdateField() to reset all (ChordFinder, Equation);
  // UpdateField();
  //     or simply update the field manager & equation of motion
  //     with pointer to new field
  GetGlobalFieldManager()->SetDetectorField(fMagneticField);
  fEquation->SetFieldObj( fMagneticField );

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserFieldSetup::SetLocalFieldValue(G4double fieldStrength)
{
  G4ThreeVector fieldSetVec(0.0, fieldStrength, 0.0);
  SetLocalFieldValue( fieldSetVec );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserFieldSetup::SetLocalFieldValue(G4ThreeVector fieldVector)
{
  if(fLocalMagneticField) delete fLocalMagneticField;

  if(fieldVector != G4ThreeVector(0.,0.,0.))
  {
    fLocalMagneticField = new  G4UniformMagField(fieldVector);
  }
  else
  {
    // If the new field's value is Zero, then
    // setting the pointer to zero ensures
    // that it is not used for propagation.
    fLocalMagneticField = 0;
  }

  // Either
  //   - UpdateField() to reset all (ChordFinder, Equation);
  // UpdateField();
  //     or simply update the field manager & equation of motion
  //     with pointer to new field
  GetLocalFieldManager()->SetDetectorField(fLocalMagneticField);
  fLocalEquation->SetFieldObj( fLocalMagneticField );

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4FieldManager* FaserFieldSetup::GetGlobalFieldManager()
{
  return G4TransportationManager::GetTransportationManager()
                                  ->GetFieldManager();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector FaserFieldSetup::GetGlobalFieldValue()
{
  static G4double fieldValue[6],  position[4];
  position[0] = position[1] = position[2] = position[3] = 0.0;

  if (fMagneticField)
  {
    fMagneticField->GetFieldValue( position, fieldValue);
  }
  else
  {
    fieldValue[0] = fieldValue[1] = fieldValue[2] = 0.0;
  }
  G4ThreeVector fieldVec(fieldValue[0], fieldValue[1], fieldValue[2]);

  return fieldVec;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector FaserFieldSetup::GetLocalFieldValue()
{
  static G4double fieldValue[6],  position[4];
  position[0] = position[1] = position[2] = position[3] = 0.0;

  if (fLocalMagneticField)
  {
    fLocalMagneticField->GetFieldValue( position, fieldValue);
  }
  else
  {
    fieldValue[0] = fieldValue[1] = fieldValue[2] = 0.0;
  }
  G4ThreeVector fieldVec(fieldValue[0], fieldValue[1], fieldValue[2]);

  return fieldVec;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
