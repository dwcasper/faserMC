#ifndef FaserCaloDigitizer_h
#define FaserCaloDigitizer_h 1

#include "G4VDigitizerModule.hh"
#include "FaserCaloDigi.hh"
#include "FaserCaloHit.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

class FaserCaloDigitizer : public G4VDigitizerModule
{
  public:
    FaserCaloDigitizer(G4String name, G4String hitCollectionName, G4String digiCollectionName);
    virtual ~FaserCaloDigitizer();

    void Digitize();

  private:
    G4String fModuleName;
    G4String fInputName;
    G4String fOutputName;
    FaserCaloDigiCollection* fDigiCollection;
};

#endif
