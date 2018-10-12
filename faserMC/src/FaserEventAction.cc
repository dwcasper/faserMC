// adapted from Geant4 example

#include "FaserEventAction.hh"
#include "FaserRunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "FaserSensorHit.hh"
#include "FaserSamplerHit.hh"

#include "G4DigiManager.hh"
#include "FaserDigitizer.hh"
#include "FaserDigi.hh"
#include "FaserEvent.hh"
#include "FaserTrackerEvent.hh"
#include "FaserTrackerSpacePoint.hh"
#include "FaserTrackerTruthParticle.hh"
#include "FaserDrawer.hh"

//#include "RootIO.hh"
#include "RootEventIO.hh"
#include "TVector3.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FaserEventAction::FaserEventAction(FaserRunAction* runAction)
  : G4UserEventAction()
  , fRunAction(runAction)
  , fFaserEvent(nullptr)
  , fFaserTrackerEvent(nullptr)
  , fDrawer(new FaserDrawer {TVector3{0.,0.5,0.}})
{
  FaserDigitizer* fd = new FaserDigitizer("FaserDigitizer");
  G4DigiManager::GetDMpointer()->AddNewModule(fd);
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FaserEventAction::~FaserEventAction()
{
  if (fFaserEvent != nullptr) delete fFaserEvent;
  fFaserEvent = nullptr;
  if (fFaserTrackerEvent != nullptr) delete fFaserTrackerEvent;
  fFaserTrackerEvent = nullptr;
  if (fDrawer != nullptr) delete fDrawer;
  fDrawer = nullptr;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserEventAction::BeginOfEventAction(const G4Event* event)
{    
  if (fFaserEvent != nullptr) delete fFaserEvent;
  fFaserEvent = new FaserEvent(event->GetEventID());
  if (fFaserTrackerEvent != nullptr) delete fFaserTrackerEvent;
  fFaserTrackerEvent = new FaserTrackerEvent(event->GetEventID());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FaserEventAction::EndOfEventAction(const G4Event* g4event)
{ 

  G4DigiManager* digiMan = G4DigiManager::GetDMpointer();

  FaserDigitizer* digiModule = (FaserDigitizer*) digiMan->FindDigitizerModule("FaserDigitizer");
  digiModule->Digitize();  
  
  G4int digiID = digiMan->GetDigiCollectionID("FaserDigitizer/FaserDigiCollection");
  FaserDigiCollection* dc = (FaserDigiCollection*) 
	  digiMan->GetDigiCollection(digiID);
  
  G4int truthID = digiMan->GetHitsCollectionID("FaserSensorHitsCollection");
  FaserSensorHitsCollection* hc = (FaserSensorHitsCollection*) 
	  digiMan->GetHitsCollection(truthID);

  G4int caloID = digiMan->GetHitsCollectionID("FaserSamplerHitsCollection");
  FaserSamplerHitsCollection* sc = (FaserSamplerHitsCollection*)
    digiMan->GetHitsCollection(caloID);

  G4cout << "Nhits: " << hc->entries() << ", Ndigits: " << dc->entries() << ", Samples: " << sc->entries() << G4endl;

 
  fFaserEvent->SetParticles(g4event->GetTrajectoryContainer());
  fFaserEvent->SetHits(hc);
  fFaserEvent->SetSamples(sc);
  fFaserEvent->SetDigis(dc);
  fFaserEvent->SetClusters();
  fFaserEvent->SetSpacePoints();
  for (FaserSpacePoint * sp : fFaserEvent->SpacePoints()) {
    fFaserTrackerEvent->spacePoints.push_back(new FaserTrackerSpacePoint {
      sp->Plane(),
      sp->Module(),
      sp->Sensor(),
      sp->Row(),
      sp->GlobalPos()
    });
  }
  for (FaserTruthParticle * tp : fFaserEvent->Particles()) {
    const G4ThreeVector & vertex = tp->Vertex();
    const G4ThreeVector & momentum = tp->Momentum();
    fFaserTrackerEvent->truthParticles.push_back(new FaserTrackerTruthParticle {
      tp->TrackID(),
      tp->ParentID(),
      tp->PdgCode(),
      TVector3{vertex.x(), vertex.y(), vertex.z()},
      TLorentzVector{momentum.x(), momentum.y(), momentum.z(), tp->Energy()},
    });
  }
  fDrawer->DrawSpacePoints(fFaserEvent);
  fDrawer->DrawPropagatedTrajectory(fFaserEvent);
  RootEventIO* rootEventIO = RootEventIO::GetInstance();
  rootEventIO->Write(fFaserEvent);
  rootEventIO->Write(fFaserTrackerEvent);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
