#ifndef FaserDigitizer_h
#define FaserDigitizer_h 1

#include "G4VDigitizerModule.hh"
#include "FaserDigi.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

class FaserDigitizer : public G4VDigitizerModule
{
  public:
    FaserDigitizer(G4String name);
    virtual ~FaserDigitizer();

    void Digitize();

  private:
    FaserDigiCollection* fDigiCollection;
    G4double* fStripEnergies;
    G4int fNPlanes;
    G4int fNModules;
    G4int fNSensors;
    G4int fNRows;
    G4int fNStrips;
    
    G4int fThreshold;
    G4double fElectronsPerADC;
    G4double fChargeSpreadSigma;
    
    const G4double fBandGap = 1.1*eV;

};

#endif
