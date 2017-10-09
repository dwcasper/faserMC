// adapted from Geant4 example

#ifndef FaserActionInitialization_h
#define FaserActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class.

class FaserActionInitialization : public G4VUserActionInitialization
{
  public:
    FaserActionInitialization();
    virtual ~FaserActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
