// copy-pasta modification of the extended/Field03 example

#ifndef FaserFieldSetup_h
#define FaserFieldSetup_h 1

#include "G4MagneticField.hh"
#include "G4UniformMagField.hh"

class G4FieldManager;
class G4ChordFinder;
class G4Mag_UsualEqRhs;
class G4MagIntegratorStepper;
class FaserFieldMessenger;

///  A class for setting up the Magnetic Field
///
///  It also creates the necessary classes to control accuracy of propagation.
///  In this example
///    - There is a global field for most of the setup;
///    - A local field overides it for some volume(s)

class FaserFieldSetup
{
public:
  FaserFieldSetup();           //  A zero field
  virtual ~FaserFieldSetup();

  void SetStepperType( G4int i ) { fStepperType = i; }
  G4int GetStepperType() { return fStepperType; }

  void SetStepper();

  void SetMinStep(G4double newStep) { fMinStep = newStep; }
  G4double GetMinStep() { return fMinStep; }

  void SetGlobalFieldValue(G4ThreeVector fieldVector);
  void SetGlobalFieldValue(G4double      fieldValue);
  G4ThreeVector GetGlobalFieldValue();

  void SetLocalFieldValue(G4ThreeVector fieldVector);
  void SetLocalFieldValue(G4double      fieldValue);
  G4ThreeVector GetLocalFieldValue();

  void UpdateField();

  G4FieldManager* GetLocalFieldManager() { return fLocalFieldManager; }

protected:

  // Find the global Field Manager

  G4FieldManager*         GetGlobalFieldManager() ;

  G4FieldManager*         fFieldManager;
  G4FieldManager*         fLocalFieldManager;
  G4ChordFinder*          fChordFinder;
  G4ChordFinder*          fLocalChordFinder;
  G4Mag_UsualEqRhs*       fEquation;
  G4Mag_UsualEqRhs*       fLocalEquation;
  G4MagneticField*        fMagneticField;
  G4MagneticField*        fLocalMagneticField;

  G4MagIntegratorStepper* fStepper;
  G4MagIntegratorStepper* fLocalStepper;
  G4int                   fStepperType;

  G4double                fMinStep;
 
  FaserFieldMessenger*    fFieldMessenger;

};

#endif
