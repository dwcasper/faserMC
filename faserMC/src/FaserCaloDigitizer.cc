#include "FaserCaloDigitizer.hh"
#include "FaserCaloDigi.hh"

#include "G4DigiManager.hh"
#include "Randomize.hh"

#include <math.h>
#include <map>

FaserCaloDigitizer::FaserCaloDigitizer(G4String name, G4String input, G4String output)
  : G4VDigitizerModule(name), fModuleName(name), fInputName(input), fOutputName(output)
{
  collectionName.push_back(fOutputName);
}

FaserCaloDigitizer::~FaserCaloDigitizer()
{ }

void FaserCaloDigitizer::Digitize()
{
  fDigiCollection = new FaserCaloDigiCollection
	  (fModuleName, fOutputName);
  
  // get tracker hits
  G4DigiManager* digiMan = G4DigiManager::GetDMpointer();
  G4int caloID = digiMan->GetHitsCollectionID( fInputName );
  FaserCaloHitsCollection* FCHC = (FaserCaloHitsCollection*)
	  (digiMan->GetHitsCollection(caloID));

  if (FCHC) {

    std::map<G4int, FaserCaloDigi*> digiMap;

    G4int nHits = FCHC->entries();

    // loop through hits and accumulate energies
    for (G4int i=0; i<nHits; i++) 
    {
      FaserCaloHit* hit = (*FCHC)[i];

      G4int module = hit->Module();
      G4int tower = hit->Tower();
      G4int towerID = tower + 9 * module;
      G4double eDepTotal = hit->Edep();
      G4AffineTransform transform = hit->Transform(); // local->global
      G4int track = hit->SourceTrack();

      if (digiMap.count(towerID) == 0)
      {
        FaserCaloDigi* thisDigi = new FaserCaloDigi();
        thisDigi->SetModule(module);
        thisDigi->SetTower(tower);
        thisDigi->SetTransform(transform);
        thisDigi->AddTrack(track, eDepTotal);
        digiMap[towerID] = thisDigi;
      }
      else
      {
        digiMap[towerID]->AddTrack(track, eDepTotal);
        if (transform.NetTranslation().z() < digiMap[towerID]->Transform().NetTranslation().z())
          digiMap[towerID]->SetTransform(transform);
      }
    } // end of loop over hits

    // store digits
    for (auto& d : digiMap)
    {                    
        fDigiCollection->insert(d.second);
    }
  }

  StoreDigiCollection(fDigiCollection);

}
