#ifndef FaserDigitizer_h
#define FaserDigitizer_h 1

#include "G4VDigitizerModule.hh"
#include "FaserDigi.hh"
#include "FaserDigiMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

class FaserDigitizer : public G4VDigitizerModule
{
  public:
    FaserDigitizer(G4String name);
    virtual ~FaserDigitizer();

    void Digitize();

    virtual void SetChargeSpreadSigma(G4double value)	{ fChargeSpreadSigma = value; }
    virtual void SetThreshold(G4double value)		{ fThreshold = value; }

    virtual G4double GetChargeSpreadSigma() const	{ return fChargeSpreadSigma; }
    virtual G4int GetThreshold() const			{ return fThreshold; }

    static constexpr G4double defaultChargeSpreadSigma = 100*um;
    static constexpr G4double defaultThreshold = 0;

  private:
    FaserDigiMessenger* fDigiMessenger;

    FaserDigiCollection* fDigiCollection;
    G4double* fStripEnergies;
    G4int fNPlanes;
    G4int fNModules;
    G4int fNSensors;
    G4int fNRows;
    G4int fNStrips;
    
    G4double fThreshold;
    G4double fChargeSpreadSigma;
    G4double fStripPitch;
    
    const G4double fBandGap = 1.1*eV;

};

#endif
