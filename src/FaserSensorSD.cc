#include "FaserSensorSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "RootIO.hh"

FaserSensorSD::FaserSensorSD(const G4String& name,
			     const G4String& hitsCollectionName)
  : G4VSensitiveDetector(name),
    fHitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
}

FaserSensorSD::~FaserSensorSD()
{ 
  RootIO::GetInstance()->Close();
}

G4bool FaserSensorSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  G4double edep = aStep->GetTotalEnergyDeposit();
  if (edep == 0.0) return false;

  FaserSensorHit* newHit = new FaserSensorHit();

  newHit->SetTrackID ( aStep->GetTrack()->GetTrackID() );
  G4TouchableHandle h = aStep->GetPreStepPoint()->GetTouchableHandle();
  //
  // zero-based identifiers
  G4int strip = h->GetReplicaNumber(0);  // 0 - (sensor_readoutStrips - 1)
  G4int row = h->GetCopyNumber(1);       // 0 - 1
  G4int sensor = h->GetCopyNumber(2);    // 0 - 3
  G4int module = h->GetCopyNumber(3);    // 0 - 1
  G4int plane = h->GetCopyNumber(4);     // 0 - (nPlanes - 1)

  newHit->SetPlaneID( plane );
  newHit->SetModuleID( module );
  newHit->SetSensorID( sensor );
  newHit->SetRowID( row );
  newHit->SetStripID( strip );

  newHit->SetEdep( edep );
  G4ThreeVector worldPosition = aStep->GetPostStepPoint()->GetPosition();
  newHit->SetGlobalPos( worldPosition );
  newHit->SetLocalPos( h->GetHistory()->GetTopTransform().TransformPoint( worldPosition ) );

  fHitsCollection->insert( newHit );

  //newHit->Print();

  return true;
}

void FaserSensorSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection =
    new FaserSensorHitsCollection(SensitiveDetectorName, collectionName[0]);  

    // add hits collection to the collection of all hit collections for event
    G4int hcID =
    G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection( hcID, fHitsCollection);
}

void FaserSensorSD::EndOfEvent(G4HCofThisEvent*)
{
  G4int nofHits = fHitsCollection->entries();
  if (verboseLevel > 1)
  {
      G4cout << G4endl <<
	"-----> Hits Collection: " << nofHits <<
	" hits in the silicon sensors:" << G4endl;
      //for (G4int i = 0; i < nofHits; i++) (*fHitsCollection)[i]->Print(); 
  }

  /*
  std::vector<FaserSensorHit*> hitsVector;

  for (G4int i = 0; i < nofHits; i++)
  {
    hitsVector.push_back((*fHitsCollection)[i]);
  }
  RootIO::GetInstance()->Write(&hitsVector);
  */
}
