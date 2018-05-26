#include "FaserSensorSD.hh"
#include "FaserDetectorConstruction.hh"
#include "FaserTrackInformation.hh"

#include "G4RunManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "RootIO.hh"

FaserSensorSD::FaserSensorSD(const G4String& name,
			     const G4String& hitsCollectionName)
  : G4VSensitiveDetector(name),
    fHitsCollection(NULL),
    fTruthCollection(NULL),
    fNModules(2),
    fNSensors(4),
    fNRows(2)
{
  collectionName.insert(hitsCollectionName);
  collectionName.insert("FaserSensorTruthCollection");

  G4RunManager* runMan = G4RunManager::GetRunManager();

  FaserDetectorConstruction* dc = (FaserDetectorConstruction*)
	  runMan->GetUserDetectorConstruction();
  fNPlanes = dc->getSensorPlanes();
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

  // truth information
  G4Track* track = aStep->GetTrack();
  //newHit->SetTrackID( track->GetTrackID() );
  //newHit->SetParticle( track->GetParticleDefinition()->GetParticleName() );
  //newHit->SetVertexPosition( track->GetVertexPosition() );
  //newHit->SetVertexMomentumDirection( track->GetVertexMomentumDirection() );
  //newHit->SetVertexKineticEnergy( track->GetVertexKineticEnergy() );


  FaserTrackInformation* info = (FaserTrackInformation*) track->GetUserInformation();
  if ( info != nullptr )
  {
    //G4cout << "Hit created from primary particle " << track->GetTrackID() << " ";
    //info->Print();
    newHit->SetTrackID( info->GetOriginalTrackID() );
    newHit->SetParticle( info->GetParticleDefinition()->GetPDGEncoding() );
    newHit->SetVertex( info->GetOriginalPosition() );
    newHit->SetMomentum( info->GetOriginalMomentum() );
    newHit->SetTotalEnergy( info->GetOriginalEnergy() );
  }
  else
  {
    G4cout << "Warning: no user information for track producing hit" << G4endl;
  }

  fHitsCollection->insert( newHit );

  // get index up to row for truth values
  G4int index = ((plane*fNModules + module) 
		  * fNSensors + sensor) 
	          * fNRows + row;
  
  // only need single representative hit per particle per row
  if ( fTruthSet.insert(std::make_pair(index, track->GetTrackID())).second )
  {
    // make a copy
    fTruthCollection->insert( new FaserSensorHit(*newHit) );
  }


  //newHit->Print();

  return true;
}

void FaserSensorSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection =
    new FaserSensorHitsCollection(SensitiveDetectorName, collectionName[0]);  
  fTruthCollection =
    new FaserSensorHitsCollection(SensitiveDetectorName, collectionName[1]);

  // add hits collection to the collection of all hit collections for event
  G4SDManager* sdMan = G4SDManager::GetSDMpointer();
  
  G4int hcID = sdMan->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection);
  
  G4int truthID = sdMan->GetCollectionID(collectionName[1]);
  hce->AddHitsCollection( truthID, fTruthCollection);
}

void FaserSensorSD::EndOfEvent(G4HCofThisEvent*)
{

  fTruthSet.clear();

  if (verboseLevel > 1)
  {
    G4int nofHits = fHitsCollection->entries();

    G4cout << G4endl <<
	"-----> Hits Collection: " << nofHits <<
	" hits in the silicon sensors:" << G4endl;
      //for (G4int i = 0; i < nofHits; i++) (*fHitsCollection)[i]->Print(); 
  }

}
