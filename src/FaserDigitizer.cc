#include "FaserDigitizer.hh"
#include "FaserDigi.hh"
#include "FaserSensorHit.hh"
#include "FaserDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4DigiManager.hh"

#include "G4SystemOfUnits.hh"

#include <math.h>

FaserDigitizer::FaserDigitizer(G4String name)
  : G4VDigitizerModule(name),
    fNModules(2),
    fNSensors(4),
    fNRows(2),
    fThreshold(0),
    fElectronsPerADC(5.0),
    fChargeSpreadSigma(150*um)

{
  G4String colName = "FaserDigiCollection";
  collectionName.push_back(colName);

  G4RunManager* runMan = G4RunManager::GetRunManager();
  FaserDetectorConstruction* dc = (FaserDetectorConstruction*)
	  runMan->GetUserDetectorConstruction();
  fNPlanes = dc->getSensorPlanes();
  fNStrips = dc->getReadoutStrips();
  fStripEnergies = new G4double[fNPlanes * fNModules * fNSensors * fNRows * fNStrips]();
  fStripPitch = dc->getStripPitch();
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
    G4double erfNormalization = fChargeSpreadSigma/fStripPitch/sqrt(2);

    // loop through hits and accumulate energies
    for (G4int i=0; i<nHits; i++) 
    {
      FaserSensorHit* hit = (*FSHC)[i];

      G4int plane = hit->GetPlaneID();
      G4int module = hit->GetModuleID();
      G4int sensor = hit->GetSensorID();
      G4int row = hit->GetRowID();
      G4int strip = hit->GetStripID();
      G4double eDepTotal = hit->GetEdep();

      G4int index = (((plane*fNModules + module)
	     * fNSensors + sensor)  
	     * fNRows + row)
	     * fNStrips + strip;

      if (fChargeSpreadSigma > 0)
      {

	// find deposited energy in the strip of incidence      
        G4double hitXscaled = hit->GetLocalPos().x() / fStripPitch; // in interval [-0.5, 0.5]
        G4double erfLeft = erf((-0.5 - hitXscaled)/erfNormalization)/2;
	G4double erfRight = erf((0.5 - hitXscaled)/erfNormalization)/2;
	*(fStripEnergies + index) += eDepTotal * (erfRight - erfLeft);

	G4int dx;
	G4double edep;
	G4double erfBound;
	G4int indexBase = index / fNStrips * fNStrips; // just the row part
	
	// spread charge to the left
	for (G4int iStrip=index % fNStrips-1; iStrip>=0; iStrip--)
	{
	  dx = iStrip - strip;
	  erfBound = erf((-0.5 + dx - hitXscaled)/erfNormalization)/2;
	  edep = eDepTotal * (erfLeft-erfBound);

	  if (edep < fBandGap) break;

	  *(fStripEnergies + iStrip + indexBase) += edep;
	  erfLeft = erfBound;
	}

	// spread charge to the right
	for (G4int iStrip=index % fNStrips+1; iStrip<fNStrips; iStrip++)
	{
	  dx = iStrip - strip;
	  erfBound = erf((0.5 + dx - hitXscaled)/erfNormalization)/2;
	  edep = eDepTotal * (erfBound-erfRight);
	  
	  if (edep < fBandGap) break;
	  
	  *(fStripEnergies + iStrip + indexBase) += edep;
	  erfRight = erfBound;
	}
      }
      else 
      {
        *(fStripEnergies + index) += eDepTotal;
      }
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
