#pragma once

#include "G4Run.hh"

class FaserEvent;

class FaserRun : public G4Run
{
public:
  FaserRun();
  virtual ~FaserRun();
  virtual void RecordEvent(const G4Event*);
  virtual void Merge(const G4Run*);
private:
  FaserEvent* fFaserEvent;
};
