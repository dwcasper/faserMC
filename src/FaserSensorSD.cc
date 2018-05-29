#include "FaserSensorSD.hh"
#include "FaserTrackInformation.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

//#include "RootIO.hh"

FaserSensorSD::FaserSensorSD(const G4String& name,
			     const G4String& hitsCollectionName)
  : G4VSensitiveDetector(name),
    fHitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
}

FaserSensorSD::~FaserSensorSD()
{ 
  //RootIO::GetInstance()->Close();
}

G4bool FaserSensorSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  G4double edep = aStep->GetTotalEnergyDeposit();
  if (edep == 0.0) return false;

  FaserSensorHit* newHit = new FaserSensorHit();

  G4TouchableHandle h = aStep->GetPreStepPoint()->GetTouchableHandle();
  //
  // zero-based identifiers
  G4int strip = h->GetReplicaNumber(0);  // 0 - (sensor_readoutStrips - 1)
  G4int row = h->GetCopyNumber(1);       // 0 - 1
  G4int sensor = h->GetCopyNumber(2);    // 0 - 3
  G4int module = h->GetCopyNumber(3);    // 0 - 1
  G4int plane = h->GetCopyNumber(4);     // 0 - (nPlanes - 1)

  newHit->SetPlane( plane );
  newHit->SetModule( module );
  newHit->SetSensor( sensor );
  newHit->SetRow( row );
  newHit->SetStrip( strip );

  newHit->SetEdep( edep );
  G4ThreeVector worldPosition = aStep->GetPostStepPoint()->GetPosition();
  newHit->SetGlobalPos( worldPosition );
  newHit->SetLocalPos( h->GetHistory()->GetTopTransform().TransformPoint( worldPosition ) );
  newHit->SetTransform( h->GetHistory()->GetTopTransform().Inverse() );

  // truth information
  G4Track* track = aStep->GetTrack();
  newHit->SetTrack( track->GetTrackID() );
  newHit->SetEnergy( track->GetTotalEnergy() );

  FaserTrackInformation* info = (FaserTrackInformation*) track->GetUserInformation();
  if ( info != nullptr )
  {
    newHit->SetOriginTrack( info->GetOriginalTrackID() );
  }
  else
  {
    G4cout << "Warning: no user information for track producing hit" << G4endl;
  }

  fHitsCollection->insert( newHit );

  return true;
}

void FaserSensorSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection =
    new FaserSensorHitsCollection(SensitiveDetectorName, collectionName[0]);  

  // add hits collection to the collection of all hit collections for event
  G4SDManager* sdMan = G4SDManager::GetSDMpointer();
  
  G4int hcID = sdMan->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection);
}

void FaserSensorSD::EndOfEvent(G4HCofThisEvent*)
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
