#ifndef FASER_RUN_HH
#define FASER_RUN_HH 1

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

};
#endif