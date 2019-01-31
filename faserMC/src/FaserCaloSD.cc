#include "FaserCaloSD.hh"
#include "FaserTrackInformation.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

FaserCaloSD::FaserCaloSD(const G4String& name,
			     const G4String& hitsCollectionName)
  : G4VSensitiveDetector(name),
    fHitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
}

FaserCaloSD::~FaserCaloSD()
{ 

}

G4bool FaserCaloSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  G4double edep = aStep->GetTotalEnergyDeposit();
  // Disable hit generation in calorimeter due to memory issues
  if (edep == 0.0 || true) return false;

  FaserCaloHit* newHit = new FaserCaloHit();

  G4TouchableHandle h = aStep->GetPreStepPoint()->GetTouchableHandle();
  //
  // zero-based identifiers
  G4int tower = h->GetCopyNumber(0);     // 0, 0-3 or 0-8, depending on granularity
  G4int plane = h->GetCopyNumber(1);     // 0 - (nPlanes - 1)
  G4int module = h->GetCopyNumber(2);  

  newHit->SetPlane( plane );
  newHit->SetTower( tower);
  newHit->SetModule( module );

  newHit->SetEdep( edep );
  G4ThreeVector worldPosition = aStep->GetPostStepPoint()->GetPosition();
  newHit->SetGlobalPos( worldPosition );
  newHit->SetLocalPos( h->GetHistory()->GetTopTransform().TransformPoint( worldPosition ) );
  newHit->SetTransform( h->GetHistory()->GetTopTransform().Inverse()); // (local -> global)

  // truth information
  G4Track* track = aStep->GetTrack();

  FaserTrackInformation* info = (FaserTrackInformation*) track->GetUserInformation();
  if ( info != nullptr )
  {
    newHit->SetOriginTrack( info->GetOriginalTrackID() );
    newHit->SetSourceTrack( info->GetSourceTrackID() );
  }
  else
  {
    G4cout << "Warning: no user information for track producing hit" << G4endl;
  }

  fHitsCollection->insert( newHit );

  return true;
}

void FaserCaloSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection =
    new FaserCaloHitsCollection(SensitiveDetectorName, collectionName[0]);  

  // add hits collection to the collection of all hit collections for event
  G4SDManager* sdMan = G4SDManager::GetSDMpointer();
  
  G4int hcID = sdMan->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection);
}

void FaserCaloSD::EndOfEvent(G4HCofThisEvent*)
{
  if (verboseLevel > 1)
  {
    G4int nofHits = fHitsCollection->entries();

    G4cout << G4endl <<
	"-----> Hits Collection: " << nofHits <<
	" hits in the silicon sensors:" << G4endl;
      //for (G4int i = 0; i < nofHits; i++) (*fHitsCollection)[i]->Print(); 
  }
}
