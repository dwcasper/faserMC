#include "FaserDigitizer.hh"
#include "FaserDigi.hh"
#include "FaserDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4DigiManager.hh"
#include "Randomize.hh"

#include <math.h>

FaserDigitizer::FaserDigitizer(G4String name)
  : G4VDigitizerModule(name),
    fDigiMessenger(new FaserDigiMessenger(this)),
    fNModules(2),
    fNSensors(4),
    fNRows(2),
    fThreshold(defaultThreshold),
    fChargeSpreadSigma(defaultChargeSpreadSigma),
    fChargeSmear(defaultChargeSmear),
    fChargeSmearNorm(defaultChargeSmearNorm)
{
  G4String colName = "FaserDigiCollection";
  collectionName.push_back(colName);
}

FaserDigitizer::~FaserDigitizer()
{ }

void FaserDigitizer::Digitize()
{
  // Can't retrieve this in constructor - it still has only default values then
  G4RunManager* runMan = G4RunManager::GetRunManager();
  FaserDetectorConstruction* dc = (FaserDetectorConstruction*)
	  runMan->GetUserDetectorConstruction();
  fNPlanes = dc->getSensorPlanes();
  fNStrips = dc->getReadoutStrips();
  fStripPitch = dc->getStripPitch();

  std::map<G4int, G4AffineTransform> transforms;
  std::map<G4int, G4double> charges;
  std::map<G4int, std::map<G4int, G4double> > contributions;

  fDigiCollection = new FaserDigiCollection
	  ("FaserDigitizer", "FaserDigiCollection");
  
  // get tracker hits
  G4DigiManager* digiMan = G4DigiManager::GetDMpointer();
  G4int sensorID = digiMan->GetHitsCollectionID("FaserSensorHitsCollection");
  FaserSensorHitsCollection* FSHC = (FaserSensorHitsCollection*)
	  (digiMan->GetHitsCollection(sensorID));

  if (FSHC) {

    G4int nHits = FSHC->entries();
    G4double erfNormalization = (fChargeSpreadSigma/fStripPitch)*sqrt(2); // was divided by sqrt(2) = incorrect

    // loop through hits and accumulate energies
    for (G4int i=0; i<nHits; i++) 
    {
      FaserSensorHit* hit = (*FSHC)[i];

      G4int plane = hit->Plane();
      G4int module = hit->Module();
      G4int sensor = hit->Sensor();
      G4int row = hit->Row();
      G4int strip = hit->Strip();
      G4double eDepTotal = hit->Edep();
      G4AffineTransform transform = hit->Transform(); // local->global
      G4AffineTransform invTransform = transform.Inverse(); // global->local
      G4ThreeVector localTranslate = invTransform.NetTranslation();
      G4ThreeVector stripOffset(((strip+0.5) - fNStrips/2) * fStripPitch, 0., 0.); // x offset from center of row
      G4ThreeVector newTranslate = localTranslate + stripOffset;
      invTransform.SetNetTranslation(newTranslate);
      G4int rowIndex = ((plane*fNModules + module)*fNSensors + sensor)*fNRows + row;
      if (transforms.count(rowIndex) == 0) transforms[rowIndex] = invTransform;

      G4int index = (((plane*fNModules + module)
	     * fNSensors + sensor)  
	     * fNRows + row)
	     * fNStrips + strip;
      
      G4int track = hit->Track();

      if (fChargeSpreadSigma > 0)
      {
	// find deposited energy in the strip of incidence      
        G4double hitXscaled = hit->LocalPos().x() / fStripPitch; // in interval [-0.5, 0.5]
        G4double erfLeft = erf((-0.5 - hitXscaled)/erfNormalization)/2;
	G4double erfRight = erf((0.5 - hitXscaled)/erfNormalization)/2;
	G4double delta = eDepTotal * (erfRight - erfLeft);
	charges[index] += delta;
	contributions[index][track] += delta;

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

	  charges[iStrip + indexBase] += edep;
	  contributions[iStrip + indexBase][track] += edep;
	  erfLeft = erfBound;
	}

	// spread charge to the right
	for (G4int iStrip=index % fNStrips+1; iStrip<fNStrips; iStrip++)
	{
	  dx = iStrip - strip;
	  erfBound = erf((0.5 + dx - hitXscaled)/erfNormalization)/2;
	  edep = eDepTotal * (erfBound-erfRight);
	  
	  if (edep < fBandGap) break;
	  
	  charges[iStrip + indexBase] += edep;
	  contributions[iStrip + indexBase][track] += edep;
	  erfRight = erfBound;
	}
      }
      else 
      {
	charges[index] += eDepTotal;
	contributions[index][track] += eDepTotal;
      }
    }

    // convert energies to charge
    for (auto& p : charges)
    {
      G4int index = p.first;
      G4double eTotal = p.second;
      G4double q = eTotal/fBandGap*eplus;
      // smear the charge 
      q = std::max(0.0,G4RandGauss::shoot(q, fChargeSmear*sqrt(q*fChargeSmearNorm)));
      if (q > fThreshold)
      {
        FaserDigi* digi = new FaserDigi();
                
	G4int remainder = index;
	digi->SetStrip(remainder % fNStrips);
	remainder /= fNStrips;
	digi->SetRow(remainder % fNRows);
	remainder /= fNRows;
	digi->SetSensor(remainder % fNSensors);
	remainder /= fNSensors;
	digi->SetModule(remainder % fNModules);
	remainder /= fNModules;
	digi->SetPlane(remainder);
	
	digi->SetCharge(q);
	for (auto tq : contributions[index])
	{
	  digi->AddTrack(tq.first, tq.second);
	}

	G4int rowIndex = ((digi->Plane()*fNModules + 
			   digi->Module())*fNSensors + 
			   digi->Sensor())*fNRows + digi->Row();
	G4AffineTransform rowInv = transforms[rowIndex];
	G4ThreeVector localTranslate = rowInv.NetTranslation();
	G4ThreeVector stripOffset(((digi->Strip()+0.5) - fNStrips/2) * fStripPitch, 0., 0.); // x offset from center of row
	G4ThreeVector rowTranslate = localTranslate - stripOffset;
	rowInv.SetNetTranslation(rowTranslate);
	G4AffineTransform stripTransform = rowInv.Inverse(); // local to global
	digi->SetTransform(stripTransform);
                
	fDigiCollection->insert(digi);
      }
      
    }
  }

  StoreDigiCollection(fDigiCollection);

}
