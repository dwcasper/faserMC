#include "FaserRun.hh"
#include "G4SDManager.hh"
#include "G4DigiManager.hh"
#include "G4Event.hh"
#include "FaserDigitizer.hh"
#include "FaserCaloDigitizer.hh"
#include "FaserEvent.hh"
#include "RootEventIO.hh"


FaserRun::FaserRun() 
  : fFaserEvent(nullptr)
{ }

FaserRun::~FaserRun()
{ 
  if (fFaserEvent != nullptr) delete fFaserEvent;
  fFaserEvent = nullptr;
}

void FaserRun::RecordEvent(const G4Event* g4Evt)
{
  G4cout << "FaserRun::RecordEvent called" << G4endl;

  if (fFaserEvent != nullptr) delete fFaserEvent;
  fFaserEvent = new FaserEvent();

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

  /*
  G4int samplerID = digiMan->GetHitsCollectionID("FaserSamplerHitsCollection");
  FaserSensorHitsCollection* sc = (FaserSensorHitsCollection*)
    digiMan->GetHitsCollection(samplerID);

  G4int caloID = digiMan->GetHitsCollectionID("FaserCaloHitsCollection");
  FaserCaloHitsCollection* cc = (FaserCaloHitsCollection*)
    digiMan->GetHitsCollection(caloID);
  */

  fFaserEvent->SetParticles(g4Evt->GetTrajectoryContainer());
  fFaserEvent->SetTrackerHits(hc);
  // fFaserEvent->SetSamplerHits(sc);                                                                                                // fFaserEvent->SetCaloHits(cc);                                                                                                  
  fFaserEvent->SetTrackerDigis(tdc);
  fFaserEvent->SetSamplerDigis(sdc);
  fFaserEvent->SetCaloDigis(cdc);
  fFaserEvent->SetClusters();
  fFaserEvent->SetSpacePoints();


  G4cout << "Strip hits: " << fFaserEvent->TrackerHits().size() << 
    ", Strip digits: " << fFaserEvent->TrackerDigis().size() << 
    ", Strip clusters: " << fFaserEvent->Clusters().size() << 
    ", Space points: " << fFaserEvent->SpacePoints().size() << G4endl;
  
  G4cout << "Sampler hits: " << fFaserEvent->SamplerHits().size() << 
    ", Sampler digits: " << fFaserEvent->SamplerDigis().size() << G4endl;
  G4cout << "Calo hits: " << fFaserEvent->CaloHits().size() << 
    ", Calo digits: " << fFaserEvent->CaloDigis().size() << G4endl;
 
 //fDrawer->DrawSpacePoints(fFaserEvent);                                                                                           //fDrawer->DrawPropagatedTrajectory(fFaserEvent);                                                                                 

  RootEventIO* rootEventIO = RootEventIO::GetInstance();                                                                          
  rootEventIO->Write(fFaserEvent);                 

  G4Run::RecordEvent(g4Evt);
}

void FaserRun::Merge(const G4Run* aRun)
{
  G4cout << "FaserRun::Merge called" << G4endl;
  G4Run::Merge(aRun);
}
