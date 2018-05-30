#ifndef FaserDigitizer_h
#define FaserDigitizer_h 1

#include "G4VDigitizerModule.hh"
#include "FaserDigi.hh"
#include "FaserSensorHit.hh"
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
    virtual void SetChargeSmear(G4double value)         { fChargeSmear = value; }
    virtual void SetChargeSmearNorm(G4double value)         { fChargeSmearNorm = value; }
   
    virtual G4double GetChargeSpreadSigma() const	{ return fChargeSpreadSigma; }
    virtual G4int GetThreshold() const			{ return fThreshold; }
    virtual G4double GetChargeSmear() const              { return fChargeSmear; }
    virtual G4double GetChargeSmearNorm() const          { return fChargeSmearNorm; } 

    static constexpr G4double defaultChargeSpreadSigma = 50*um;
    static constexpr G4double defaultThreshold = 0;
    static constexpr G4double defaultChargeSmear = 0.05;
    static constexpr G4double defaultChargeSmearNorm = 50000.0;

  private:
    FaserDigiMessenger* fDigiMessenger;
    FaserDigiCollection* fDigiCollection;
    G4int fNPlanes;
    G4int fNModules;
    G4int fNSensors;
    G4int fNRows;
    G4int fNStrips;
    
    G4double fThreshold;
    G4double fChargeSpreadSigma;
    G4double fStripPitch;

    G4double fChargeSmear;
    G4double fChargeSmearNorm;
    
    const G4double fBandGap = 1.1*eV;

};

#endif
