// adapted from Geant4 example

#ifndef FaserRunAction_h
#define FaserRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;
class FaserGeometry;

/// Run action class
///

class FaserRunAction : public G4UserRunAction
{
  public:
    FaserRunAction();
    virtual ~FaserRunAction();

    // virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

  private:
  FaserGeometry* fGeometry;
};

#endif

