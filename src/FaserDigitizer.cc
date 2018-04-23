#include "FaserDigitizer.hh"
#include "FaserDigi.hh"
#include "FaserSensorHit.hh"
#include "FaserDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4DigiManager.hh"

#include "G4SystemOfUnits.hh"

FaserDigitizer::FaserDigitizer(G4String name)
  : G4VDigitizerModule(name),
    fNModules(2),
    fNSensors(4),
    fNRows(2),
    fThreshold(0.0)
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
    G4cout << "nHits = " << nHits << G4endl;

    G4int rowMult = fNStrips;
    G4int sensorMult = rowMult * fNRows;
    G4int moduleMult = sensorMult * fNSensors;
    G4int planeMult = moduleMult * fNModules;

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

      G4int index = plane*planeMult + module*moduleMult + sensor*sensorMult + row*rowMult + strip;
      *(fStripEnergies + index) += edep;

    }

    // convert energies to digits
    G4int nStripsTotal = fNPlanes * fNModules * fNSensors * fNRows * fNStrips;
    for (G4int index=0; index<nStripsTotal; index++)
    {
      G4double eTotal = *(fStripEnergies + index);
      if (eTotal > fThreshold)
      {
        FaserDigi* digi = new FaserDigi();
                
	G4int remainder = index;
	digi->SetPlaneID(remainder / planeMult);
	remainder %= planeMult;
	digi->SetModuleID(remainder / moduleMult);
	remainder %= moduleMult;
	digi->SetSensorID(remainder / sensorMult);
	remainder %= sensorMult;
	digi->SetRowID(remainder /= rowMult);
	remainder %= rowMult;
	digi->SetStripID(remainder);
	digi->SetEdep(eTotal);
                
	fDigiCollection->insert(digi);
      }
      *(fStripEnergies + index) = 0;
      
    }
  }
  G4cout << "nDigi = " << fDigiCollection->entries() << G4endl;

  StoreDigiCollection(fDigiCollection);

}
