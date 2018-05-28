
#include <sstream>

#include "RootIO.hh"

#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"

static G4String fileName = "FaserMC_Default.root";
static RootIO* instance = nullptr;

RootIO::RootIO()
{
  // initialize ROOT
  fAnalysisManager = G4AnalysisManager::Instance();
  fAnalysisManager->OpenFile(fileName);
  fAnalysisManager->CreateNtuple("hits", "Faser tracker digits"); 

  // tracker readout
  fAnalysisManager->CreateNtupleIColumn("digi_plane", fPlaneVector);
  fAnalysisManager->CreateNtupleIColumn("digi_module", fModuleVector);
  fAnalysisManager->CreateNtupleIColumn("digi_sensor", fSensorVector);
  fAnalysisManager->CreateNtupleIColumn("digi_row", fRowVector);
  fAnalysisManager->CreateNtupleIColumn("digi_strip", fStripVector);
  // not included in trackers, but useful for post hoc thresholding
  fAnalysisManager->CreateNtupleDColumn("digi_charge", fChargeVector);

  // truth values
  fAnalysisManager->CreateNtupleIColumn("truth_plane", fTruthPlaneVector);
  fAnalysisManager->CreateNtupleIColumn("truth_module", fTruthModuleVector);
  fAnalysisManager->CreateNtupleIColumn("truth_sensor", fTruthSensorVector);
  fAnalysisManager->CreateNtupleIColumn("truth_row", fTruthRowVector);
  fAnalysisManager->CreateNtupleIColumn("truth_strip", fTruthStripVector);

  fAnalysisManager->CreateNtupleIColumn("truth_track", fTruthTrackVector);
  fAnalysisManager->CreateNtupleIColumn("truth_particle", fTruthParticleVector);
  fAnalysisManager->CreateNtupleDColumn("truth_energy", fTruthEnergyVector);

  fAnalysisManager->CreateNtupleDColumn("truth_global_x", fTruthGlobalXVector);
  fAnalysisManager->CreateNtupleDColumn("truth_global_y", fTruthGlobalYVector);
  fAnalysisManager->CreateNtupleDColumn("truth_global_z", fTruthGlobalZVector);

  fAnalysisManager->CreateNtupleDColumn("truth_local_x", fTruthLocalXVector);
  fAnalysisManager->CreateNtupleDColumn("truth_local_y", fTruthLocalYVector);
  fAnalysisManager->CreateNtupleDColumn("truth_local_z", fTruthLocalZVector);

  fAnalysisManager->CreateNtupleIColumn("truth_origin_track", fTruthOriginTrackVector);
  fAnalysisManager->CreateNtupleIColumn("truth_origin_particle", fTruthOriginParticleVector);

  fAnalysisManager->CreateNtupleDColumn("truth_origin_x", fTruthOriginXVector);
  fAnalysisManager->CreateNtupleDColumn("truth_origin_y", fTruthOriginYVector);
  fAnalysisManager->CreateNtupleDColumn("truth_origin_z", fTruthOriginZVector);

  fAnalysisManager->CreateNtupleDColumn("truth_origin_px", fTruthOriginPXVector);
  fAnalysisManager->CreateNtupleDColumn("truth_origin_py", fTruthOriginPYVector);
  fAnalysisManager->CreateNtupleDColumn("truth_origin_pz", fTruthOriginPZVector);

  fAnalysisManager->CreateNtupleDColumn("truth_origin_energy", fTruthOriginEnergyVector);
  
  fAnalysisManager->FinishNtuple();
}

RootIO::~RootIO()
{
  delete fAnalysisManager;
}

RootIO* RootIO::GetInstance()
{
  if (instance == nullptr)
  {
    instance = new RootIO();
  }
  return instance;
}

void RootIO::SetFileName(G4String name)
{
  if (name == fileName) return;
  fileName = name;

  if (instance == nullptr) return;

  G4cout << "Closing previous ROOT file" << G4endl;

  instance->Close();
  delete instance;
  instance = nullptr;

  return;
}

void RootIO::AddDigits(FaserDigiCollection* dc)
{

  G4int nDigi = dc->entries();

  for (G4int i=0; i<nDigi; i++)
  {
    FaserDigi* digi = (*dc)[i];
    
    fPlaneVector.push_back(digi->GetPlaneID());
    fModuleVector.push_back(digi->GetModuleID());
    fSensorVector.push_back(digi->GetSensorID());
    fRowVector.push_back(digi->GetRowID());
    fStripVector.push_back(digi->GetStripID());
    fChargeVector.push_back(digi->GetCharge()/coulomb*1e15);
  }
}

void RootIO::AddTruth(FaserSensorHitsCollection* hc)
{
  G4int nHits = hc->entries();

  for (G4int i=0; i<nHits; i++)
  {
    FaserSensorHit* hit = (*hc)[i];

    fTruthPlaneVector.push_back(hit->GetPlaneID());
    fTruthModuleVector.push_back(hit->GetModuleID());
    fTruthSensorVector.push_back(hit->GetSensorID());
    fTruthRowVector.push_back(hit->GetRowID());
    fTruthStripVector.push_back(hit->GetStripID());

    fTruthTrackVector.push_back(hit->GetTrackID());
    fTruthParticleVector.push_back(hit->GetParticle());
    fTruthEnergyVector.push_back(hit->GetEnergy());
    
    fTruthGlobalXVector.push_back(hit->GetGlobalPos().x()/cm);
    fTruthGlobalYVector.push_back(hit->GetGlobalPos().y()/cm);
    fTruthGlobalZVector.push_back(hit->GetGlobalPos().z()/cm);
    
    fTruthLocalXVector.push_back(hit->GetLocalPos().x()/mm);
    fTruthLocalYVector.push_back(hit->GetLocalPos().y()/mm);
    fTruthLocalZVector.push_back(hit->GetLocalPos().z()/mm);

    fTruthOriginTrackVector.push_back(hit->GetOriginTrackID());
    fTruthOriginParticleVector.push_back(hit->GetOriginParticle());

    fTruthOriginXVector.push_back(hit->GetOriginPosition().x()/cm);
    fTruthOriginYVector.push_back(hit->GetOriginPosition().y()/cm);
    fTruthOriginZVector.push_back(hit->GetOriginPosition().z()/cm);

    fTruthOriginPXVector.push_back(hit->GetOriginMomentum().x());
    fTruthOriginPYVector.push_back(hit->GetOriginMomentum().y());
    fTruthOriginPZVector.push_back(hit->GetOriginMomentum().z());

    fTruthOriginEnergyVector.push_back(hit->GetOriginEnergy()/MeV);
  }
}

void RootIO::WriteEvent()
{
  fAnalysisManager->AddNtupleRow();

  fPlaneVector.clear();
  fModuleVector.clear();
  fSensorVector.clear();
  fRowVector.clear();
  fStripVector.clear();
  fChargeVector.clear();

  fTruthPlaneVector.clear();
  fTruthModuleVector.clear();
  fTruthSensorVector.clear();
  fTruthRowVector.clear();
  fTruthStripVector.clear();

  fTruthTrackVector.clear();
  fTruthParticleVector.clear();
  fTruthEnergyVector.clear();

  fTruthGlobalXVector.clear();
  fTruthGlobalYVector.clear();
  fTruthGlobalZVector.clear();

  fTruthLocalXVector.clear();
  fTruthLocalYVector.clear();
  fTruthLocalZVector.clear();

  fTruthOriginTrackVector.clear();
  fTruthOriginParticleVector.clear();

  fTruthOriginXVector.clear();
  fTruthOriginYVector.clear();
  fTruthOriginZVector.clear();

  fTruthOriginPXVector.clear();
  fTruthOriginPYVector.clear();
  fTruthOriginPZVector.clear();

  fTruthOriginEnergyVector.clear(); 
}

void RootIO::Close()
{
  fAnalysisManager->Write();
  fAnalysisManager->CloseFile();  
}
