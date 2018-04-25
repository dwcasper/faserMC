#include "FaserDigitizer.hh"
#include "FaserDigi.hh"
#include "FaserSensorHit.hh"
#include "FaserDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4DigiManager.hh"

FaserDigitizer::FaserDigitizer(G4String name)
  : G4VDigitizerModule(name),
    fNModules(2),
    fNSensors(4),
    fNRows(2),
    fThreshold(0),
    fElectronsPerADC(5.0),
    fChargeSpreadSigma(0.0)

{
  G4String colName = "FaserDigiCollection";
  collectionName.push_back(colName);

  G4RunManager* runMan = G4RunManager::GetRunManager();
  FaserDetectorConstruction* dc = (FaserDetectorConstruction*)
	  runMan->GetUserDetectorConstruction();
  fNPlanes = dc->getSensorPlanes();
  fNStrips = dc->getReadoutStrips();
  fStripEnergies = new G4double[fNPlanes * fNModules * fNSensors * fNRows * fNStrips]();
}

FaserDigitizer::~FaserDigitizer()
{
  delete [] fStripEnergies;
}

void FaserDigitizer::Digitize()
{
  fDigiCollection = new FaserDigiCollection
	  ("FaserDigitizer", "FaserDigiCollection");
  
  // get tracker hits
  G4DigiManager* digiMan = G4DigiManager::GetDMpointer();
  G4int sensorID = digiMan->GetHitsCollectionID("FaserSensorHitsCollection");
  FaserSensorHitsCollection* FSHC = (FaserSensorHitsCollection*)
	  (digiMan->GetHitsCollection(sensorID));

  if (FSHC) {

    G4int nHits = FSHC->entries();

    // loop through hits and accumulate energies
    for (G4int i=0; i<nHits; i++) 
    {
      FaserSensorHit* hit = (*FSHC)[i];

      G4int plane = hit->GetPlaneID();
      G4int module = hit->GetModuleID();
      G4int sensor = hit->GetSensorID();
      G4int row = hit->GetRowID();
      G4int strip = hit->GetStripID();
      G4double edep = hit->GetEdep();

      G4int index = (((plane*fNModules + module)
	     * fNSensors + sensor)  
	     * fNRows + row)
	     * fNStrips + strip;

      G4ThreeVector localPos = hit->GetLocalPos();

      *(fStripEnergies + index) += edep;

    }

    // convert energies to digits
    G4int nStripsTotal = fNPlanes * fNModules * fNSensors * fNRows * fNStrips;
    for (G4int index=0; index<nStripsTotal; index++)
    {
      G4double eTotal = *(fStripEnergies + index);
      G4int ADC = eTotal/fBandGap/fElectronsPerADC;

      if (ADC > fThreshold)
      {
        FaserDigi* digi = new FaserDigi();
                
	G4int remainder = index;
	digi->SetStripID(remainder % fNStrips);
	remainder /= fNStrips;
	digi->SetRowID(remainder % fNRows);
	remainder /= fNRows;
	digi->SetSensorID(remainder % fNSensors);
	remainder /= fNSensors;
	digi->SetModuleID(remainder % fNModules);
	remainder /= fNModules;
	digi->SetPlaneID(remainder);
	
	digi->SetADC(ADC);
                
	fDigiCollection->insert(digi);
      }
      *(fStripEnergies + index) = 0;
      
    }
  }

  StoreDigiCollection(fDigiCollection);

}
