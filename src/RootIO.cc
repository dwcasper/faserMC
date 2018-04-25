
#include <sstream>

#include "RootIO.hh"

#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

static G4String fileName = "FaserMC_Default.root";
static RootIO* instance = nullptr;

RootIO::RootIO()
{
  // initialize ROOT
  fAnalysisManager = G4AnalysisManager::Instance();
  fAnalysisManager->OpenFile(fileName);
  fAnalysisManager->CreateNtuple("hits", "Faser tracker digits"); 

  fAnalysisManager->CreateNtupleIColumn("digi_plane", fPlaneVector);
  fAnalysisManager->CreateNtupleIColumn("digi_module", fModuleVector);
  fAnalysisManager->CreateNtupleIColumn("digi_sensor", fSensorVector);
  fAnalysisManager->CreateNtupleIColumn("digi_row", fRowVector);
  fAnalysisManager->CreateNtupleIColumn("digi_strip", fStripVector);
  fAnalysisManager->CreateNtupleIColumn("digi_ADC", fADCVector);
  
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

void RootIO::Write(FaserDigiCollection* dc)
{

  G4int nDigi = dc->entries();

  for(G4int i=0; i<nDigi; i++)
  {
    FaserDigi* digi = (*dc)[i];
    
    fPlaneVector.push_back(digi->GetPlaneID());
    fModuleVector.push_back(digi->GetModuleID());
    fSensorVector.push_back(digi->GetSensorID());
    fRowVector.push_back(digi->GetRowID());
    fStripVector.push_back(digi->GetStripID());
    fADCVector.push_back(digi->GetADC());
  }

  fAnalysisManager->AddNtupleRow();

  fPlaneVector.clear();
  fModuleVector.clear();
  fSensorVector.clear();
  fRowVector.clear();
  fStripVector.clear();
  fADCVector.clear();
}

void RootIO::Close()
{
  fAnalysisManager->Write();
  fAnalysisManager->CloseFile();  
}
