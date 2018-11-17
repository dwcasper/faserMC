// adapted from RE01 Geant 4 example

#ifndef FaserStackingAction_H
#define FaserStackingAction_H 1

#include "globals.hh"
#include "G4UserStackingAction.hh"

class G4Track;

class FaserStackingAction : public G4UserStackingAction
{
public:
  FaserStackingAction();
  virtual ~FaserStackingAction();

public:
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
  virtual void NewStage();
  virtual void PrepareNewEvent();

private:
  G4int fStage;

};

#endif
