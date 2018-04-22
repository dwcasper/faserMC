#ifndef FaserDigitizer_h
#define FaserDigitizer_h 1

#include "G4VDigitizerModule.hh"
#include "FaserDigi.hh"
#include "globals.hh"

class FaserDigitizer : public G4VDigitizerModule
{
  public:
    FaserDigitizer(G4String name);
    virtual ~FaserDigitizer();

    void Digitize();

  private:
    FaserDigiCollection* fDigiCollection;

};

#endif
