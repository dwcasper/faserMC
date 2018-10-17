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
#include "FaserTrackerCluster.hh"
#include "FaserTrackerDigit.hh"
#include "FaserTrackerTruthHit.hh"
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

  for (FaserSensorHit * hit : fFaserEvent->Hits()) {
    fFaserTrackerEvent->truthHits.push_back(new FaserTrackerTruthHit {
      hit->Plane(),
      hit->Module(),
      hit->Sensor(),
      hit->Row(),
      hit->Strip(),
      hit->Edep(),
      {hit->GlobalPos().x(), hit->GlobalPos().y(), hit->GlobalPos().z()}
    });
  }

  for (FaserSpacePoint * sp : fFaserEvent->SpacePoints()) {
    auto * trSP = new FaserTrackerSpacePoint {
      sp->Plane(),
      sp->Module(),
      sp->Sensor(),
      sp->Row(),
      {sp->GlobalPos().x(), sp->GlobalPos().y(), sp->GlobalPos().z()}
    };
    fFaserTrackerEvent->spacePoints.push_back(trSP);

    for (FaserCluster * cl : sp->Clusters()) {
      auto * trCluster = new FaserTrackerCluster {
        cl->Plane(),
        cl->Module(),
        cl->Sensor(),
        cl->Row(),
        cl->WeightedStrip(),
        cl->Charge(),
        {cl->GlobalPos().x(), cl->GlobalPos().y(), cl->GlobalPos().z()}
      };
      trSP->analogClusters.push_back(trCluster);

      for (FaserDigi * digi : cl->Digis()) {
        G4ThreeVector globalPos = digi->Transform().NetTranslation();
        auto * trDigit = new FaserTrackerDigit {
          digi->Plane(),
          digi->Module(),
          digi->Sensor(),
          digi->Row(),
          digi->Strip(),
          digi->Charge(),
          {globalPos.x(), globalPos.y(), globalPos.z()}
        };
        trCluster->digits.push_back(trDigit);

        for (uint i = 0; i < fFaserTrackerEvent->truthHits.size(); ++i) {
          FaserTrackerTruthHit * hit = fFaserTrackerEvent->truthHits[i];
          if (hit->plane  != trDigit->plane ) continue;
          if (hit->module != trDigit->module) continue;
          if (hit->sensor != trDigit->sensor) continue;
          if (hit->row    != trDigit->row   ) continue;
          if (hit->strip  != trDigit->strip ) continue;
          trDigit->truthHitIndices.push_back(i);
        }
      }
    }
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

  //fDrawer->DrawSpacePoints(fFaserEvent);
  //fDrawer->DrawPropagatedTrajectory(fFaserEvent);

  RootEventIO* rootEventIO = RootEventIO::GetInstance();
  rootEventIO->Write(fFaserEvent);
  rootEventIO->Write(fFaserTrackerEvent);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
