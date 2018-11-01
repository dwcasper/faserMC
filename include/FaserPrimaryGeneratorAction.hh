// adapted from Geant4 example

#ifndef FaserPrimaryGeneratorAction_h
#define FaserPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4Event;
class FaserPrimaryGenerator;


class FaserPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    FaserPrimaryGeneratorAction();    
    virtual ~FaserPrimaryGeneratorAction();

    // method from the base class
    virtual void GeneratePrimaries(G4Event*);         
  
  private:
    FaserPrimaryGenerator* fPrimaryGenerator;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
