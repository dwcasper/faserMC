// adapted from Geant4 example

#include "FaserEventAction.hh"
#include "FaserRunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "FaserEvent.hh"
#include "RootEventIO.hh"

#include "FaserSensorHit.hh"
#include "FaserCaloHit.hh"

#include "G4DigiManager.hh"
#include "FaserDigitizer.hh"
#include "FaserCaloDigitizer.hh"

#include "FaserDigi.hh"
#include "FaserCaloDigi.hh"

#include "FaserTrackerEvent.hh"
#include "FaserTrackerSpacePoint.hh"
#include "FaserTrackerCluster.hh"
#include "FaserTrackerDigit.hh"
#include "FaserTrackerTruthHit.hh"
#include "FaserTrackerTruthParticle.hh"
#include "FaserDrawer.hh"

#include "TVector3.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FaserEventAction::FaserEventAction(FaserRunAction* runAction)
  : G4UserEventAction()
  , fRunAction(runAction)
  , fFaserEvent(nullptr)
  , fFaserTrackerEvent(nullptr)
  , fDrawer(new FaserDrawer {TVector3{0.,0.5,0.}})
{
  FaserDigitizer* fd = new FaserDigitizer("FaserTrackerDigitizer", "FaserTrackerHitsCollection", "FaserTrackerDigiCollection");
  G4DigiManager::GetDMpointer()->AddNewModule(fd);
  fd = new FaserDigitizer("FaserSamplerDigitizer", "FaserSamplerHitsCollection", "FaserSamplerDigiCollection");
  G4DigiManager::GetDMpointer()->AddNewModule(fd);

  FaserCaloDigitizer* fcd = new FaserCaloDigitizer("FaserCaloDigitizer", "FaserCaloHitsCollection", "FaserCaloDigiCollection");
  G4DigiManager::GetDMpointer()->AddNewModule(fcd);
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

  FaserDigitizer* trackerDigiModule = (FaserDigitizer*) digiMan->FindDigitizerModule("FaserTrackerDigitizer");
  trackerDigiModule->Digitize();  

  FaserDigitizer* samplerDigiModule = (FaserDigitizer*) digiMan->FindDigitizerModule("FaserSamplerDigitizer");
  samplerDigiModule->Digitize();  

  FaserCaloDigitizer* caloDigiModule = (FaserCaloDigitizer*) digiMan->FindDigitizerModule("FaserCaloDigitizer");
  caloDigiModule->Digitize();
  
  G4int trackerDigiID = digiMan->GetDigiCollectionID("FaserTrackerDigiCollection");
  FaserDigiCollection* tdc = (FaserDigiCollection*) 
	  digiMan->GetDigiCollection(trackerDigiID);

  G4int samplerDigiID = digiMan->GetDigiCollectionID("FaserSamplerDigiCollection");
  FaserDigiCollection* sdc = (FaserDigiCollection*)
    digiMan->GetDigiCollection(samplerDigiID);

  G4int caloDigiID = digiMan->GetDigiCollectionID("FaserCaloDigiCollection");
  FaserCaloDigiCollection* cdc = (FaserCaloDigiCollection*)
    digiMan->GetDigiCollection(caloDigiID);
  
  G4int trackerID = digiMan->GetHitsCollectionID("FaserTrackerHitsCollection");
  FaserSensorHitsCollection* hc = (FaserSensorHitsCollection*) 
	  digiMan->GetHitsCollection(trackerID);

  G4int samplerID = digiMan->GetHitsCollectionID("FaserSamplerHitsCollection");
  FaserSensorHitsCollection* sc = (FaserSensorHitsCollection*)
    digiMan->GetHitsCollection(samplerID);

  G4int caloID = digiMan->GetHitsCollectionID("FaserCaloHitsCollection");
  FaserCaloHitsCollection* cc = (FaserCaloHitsCollection*)
    digiMan->GetHitsCollection(caloID);

  fFaserEvent->SetParticles(g4event->GetTrajectoryContainer());
  fFaserEvent->SetTrackerHits(hc);
  // fFaserEvent->SetSamplerHits(sc);
  // fFaserEvent->SetCaloHits(cc);
  fFaserEvent->SetTrackerDigis(tdc);
  fFaserEvent->SetSamplerDigis(sdc);
  fFaserEvent->SetCaloDigis(cdc);
  fFaserEvent->SetClusters();
  fFaserEvent->SetSpacePoints();

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

  for (FaserSensorHit * hit : fFaserEvent->TrackerHits()) {
    fFaserTrackerEvent->truthHits.push_back(new FaserTrackerTruthHit {
      hit->Track(),
      hit->Plane(),
      hit->Module(),
      hit->Sensor(),
      hit->Row(),
      hit->Strip(),
      hit->Edep(),
      {hit->GlobalPos().x(), hit->GlobalPos().y(), hit->GlobalPos().z()}
    });
  }

  for (FaserDigi * digi : fFaserEvent->TrackerDigis()) {
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
    for (uint i = 0; i < fFaserTrackerEvent->truthHits.size(); ++i) {
      FaserTrackerTruthHit * hit = fFaserTrackerEvent->truthHits[i];
      if (hit->plane  != trDigit->plane ) continue;
      if (hit->module != trDigit->module) continue;
      if (hit->sensor != trDigit->sensor) continue;
      if (hit->row    != trDigit->row   ) continue;
      if (hit->strip  != trDigit->strip ) continue;
      trDigit->truthHitIndices.push_back(i);
    }
    fFaserTrackerEvent->digits.push_back(trDigit);
  }

  for (FaserCluster * cl : fFaserEvent->Clusters()) {
    auto * trCluster = new FaserTrackerCluster {
      cl->Plane(),
      cl->Module(),
      cl->Sensor(),
      cl->Row(),
      cl->WeightedStrip(),
      cl->Charge(),
      {cl->GlobalPos().x(), cl->GlobalPos().y(), cl->GlobalPos().z()}
    };
    for (uint i = 0; i < fFaserEvent->TrackerDigis().size(); ++i) {
      FaserDigi * eventDigi = fFaserEvent->TrackerDigis().at(i);
      if (std::find(cl->Digis().begin(), cl->Digis().end(), eventDigi) != cl->Digis().end()) {
        trCluster->digitIndices.push_back(i);
      }
    }
    for (uint i = 0; i < trCluster->digitIndices.size(); ++i) {
      FaserTrackerDigit * trDigit = fFaserTrackerEvent->digits[i];
      for (uint j : trDigit->truthHitIndices) {
        if (std::find(trCluster->truthHitIndices.begin(), trCluster->truthHitIndices.end(), j) == trCluster->truthHitIndices.end()) {
            trCluster->truthHitIndices.push_back(j);
        }
      }
    }
    fFaserTrackerEvent->analogClusters.push_back(trCluster);
  }

  for (FaserSpacePoint * sp : fFaserEvent->SpacePoints()) {
    auto * trSP = new FaserTrackerSpacePoint {
      sp->Plane(),
      sp->Module(),
      sp->Sensor(),
      sp->Row(),
      {sp->GlobalPos().x(), sp->GlobalPos().y(), sp->GlobalPos().z()}
    };
    for (uint i = 0; i < fFaserEvent->Clusters().size(); ++i) {
      FaserCluster * eventCl = fFaserEvent->Clusters().at(i);
      if (std::find(sp->Clusters().begin(), sp->Clusters().end(), eventCl) != sp->Clusters().end()) {
        trSP->analogClusterIndices.push_back(i);
      }
    }
    for (uint i = 0; i < trSP->analogClusterIndices.size(); ++i) {
      FaserTrackerCluster * trCluster = fFaserTrackerEvent->analogClusters[i];
      for (uint j : trCluster->truthHitIndices) {
        if (std::find(trSP->truthHitIndices.begin(), trSP->truthHitIndices.end(), j) == trSP->truthHitIndices.end()) {
          trSP->truthHitIndices.push_back(j);
        }
      }
    }
    fFaserTrackerEvent->spacePoints.push_back(trSP);
  }

  G4cout << "Strip hits: " << fFaserEvent->TrackerHits().size() << ", Strip digits: " << fFaserEvent->TrackerDigis().size() << ", Strip clusters: " << fFaserEvent->Clusters().size() << G4endl;
  G4cout << "Sampler hits: " << fFaserEvent->SamplerHits().size() << ", Sampler digits: " << fFaserEvent->SamplerDigis().size() << G4endl;
  G4cout << "Calo hits: " << fFaserEvent->CaloHits().size() << ", Calo digits: " << fFaserEvent->CaloDigis().size() << G4endl; 
  //fDrawer->DrawSpacePoints(fFaserEvent);
  //fDrawer->DrawPropagatedTrajectory(fFaserEvent);

  RootEventIO* rootEventIO = RootEventIO::GetInstance();
  rootEventIO->Write(fFaserEvent);
  rootEventIO->Write(fFaserTrackerEvent);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
