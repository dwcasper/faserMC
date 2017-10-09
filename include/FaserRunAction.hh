// adapted from Geant4 example

#ifndef FaserRunAction_h
#define FaserRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume 
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.

class FaserRunAction : public G4UserRunAction
{
  public:
    FaserRunAction();
    virtual ~FaserRunAction();

    // virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

  private:

};

#endif

