#include "FaserSamplerSD.hh"
#include "FaserTrackInformation.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

//#include "RootIO.hh"

FaserSamplerSD::FaserSamplerSD(const G4String& name,
			     const G4String& hitsCollectionName)
  : G4VSensitiveDetector(name),
    fHitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
}

FaserSamplerSD::~FaserSamplerSD()
{ 
  //RootIO::GetInstance()->Close();
}

G4bool FaserSamplerSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  G4double edep = aStep->GetTotalEnergyDeposit();
  if (edep == 0.0) return false;

  FaserSamplerHit* newHit = new FaserSamplerHit();

  G4TouchableHandle h = aStep->GetPreStepPoint()->GetTouchableHandle();
  //
  // zero-based identifiers
  G4int plane = h->GetCopyNumber(1);     // 0 - (nPlanes - 1)

  newHit->SetPlane( plane );

  newHit->SetEdep( edep );
  G4ThreeVector worldPosition = aStep->GetPostStepPoint()->GetPosition();
  newHit->SetGlobalPos( worldPosition );
  newHit->SetLocalPos( h->GetHistory()->GetTopTransform().TransformPoint( worldPosition ) );

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

void FaserSamplerSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection =
    new FaserSamplerHitsCollection(SensitiveDetectorName, collectionName[0]);  

  // add hits collection to the collection of all hit collections for event
  G4SDManager* sdMan = G4SDManager::GetSDMpointer();
  
  G4int hcID = sdMan->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection);
}

void FaserSamplerSD::EndOfEvent(G4HCofThisEvent*)
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
