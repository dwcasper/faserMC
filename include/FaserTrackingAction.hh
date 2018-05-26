// adapted from RE01 Geant4 example

#ifndef FaserTrackingAction_H
#define FaserTrackingAction_H 1

#include "G4UserTrackingAction.hh"

class FaserTrackingAction : public G4UserTrackingAction
{
public:
  FaserTrackingAction();
  virtual ~FaserTrackingAction() {};

  virtual void PostUserTrackingAction(const G4Track*);

};

#endif
