#include "FaserDigitizer.hh"
#include "FaserDigi.hh"

#include "FaserSensorHit.hh"

#include "G4DigiManager.hh"

FaserDigitizer::FaserDigitizer(G4String name)
  : G4VDigitizerModule(name)
{
  G4String colName = "FaserDigiCollection";
  collectionName.push_back(colName);
}

FaserDigitizer::~FaserDigitizer()
{}

void FaserDigitizer::Digitize()
{
  fDigiCollection = new FaserDigiCollection
	  ("FaserDigitizer", "FaserDigiCollection");
	
  G4DigiManager* digiMan = G4DigiManager::GetDMpointer();
  G4int sensorID = digiMan->GetHitsCollectionID("FaserSensorHitsCollection");
  FaserSensorHitsCollection* FSHC = (FaserSensorHitsCollection*)
	  (digiMan->GetHitsCollection(sensorID));

  if (FSHC) {
    G4int nHits = FSHC->entries();

    for (G4int i=0; i<nHits; i++) {
      FaserSensorHit* hit = (*FSHC)[i];

      FaserDigi* digi = new FaserDigi();
      digi->SetTrackID(hit->GetTrackID());
      digi->SetPlaneID(hit->GetPlaneID());
      digi->SetModuleID(hit->GetModuleID());
      digi->SetSensorID(hit->GetSensorID());
      digi->SetRowID(hit->GetRowID());
      digi->SetStripID(hit->GetStripID());
      digi->SetEdep(hit->GetEdep());
      digi->SetGlobalPos(hit->GetGlobalPos());
      digi->SetLocalPos(hit->GetLocalPos());
      fDigiCollection->insert(digi);
    }
  }

  StoreDigiCollection(fDigiCollection);

}
