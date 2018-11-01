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
    FaserDigitizer(G4String name, G4String hitCollectionName, G4String digiCollectionName);
    virtual ~FaserDigitizer();

    void Digitize();

    virtual void SetChargeSpreadSigma(G4double value)	{ fChargeSpreadSigma = value; }
    virtual void SetThreshold(G4double value)		{ fThreshold = value; }
    virtual void SetChargeSmear(G4double value)         { fChargeSmear = value; }
    virtual void SetChargeNorm(G4double value)          { fChargeNorm = value; }
   
    virtual G4double GetChargeSpreadSigma() const	{ return fChargeSpreadSigma; }
    virtual G4int GetThreshold() const			{ return fThreshold; }
    virtual G4double GetChargeSmear() const             { return fChargeSmear; }
    virtual G4double GetChargeNorm() const              { return fChargeNorm; } 

    static constexpr G4double defaultChargeSpreadSigma = 15*um;
    static constexpr G4double defaultThreshold = 0.6e-15*coulomb;
    static constexpr G4double defaultChargeSmear = 600*eplus;
    static constexpr G4double defaultChargeNorm = 0.4;

  private:
    G4String fModuleName;
    G4String fInputName;
    G4String fOutputName;
    FaserDigiMessenger* fDigiMessenger;
    FaserDigiCollection* fDigiCollection;
    // G4int fNPlanes;
    G4int fNModules;
    G4int fNSensors;
    G4int fNRows;
    G4int fNStrips;
    
    G4double fThreshold;
    G4double fChargeSpreadSigma;
    G4double fStripPitch;

    G4double fChargeSmear;
    G4double fChargeNorm;
    
    const G4double fBandGap = 1.1*eV;

};

#endif
