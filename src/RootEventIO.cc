#include <sstream>

#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"

#include "RootEventIO.hh"
#include "FaserGeometry.hh"
#include "FaserEvent.hh"

#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

static G4String fileName = "FaserMCEvent_Default.root";
static G4String geoFileName = "faserGeo.mac";
static RootEventIO* instance = nullptr;

RootEventIO::RootEventIO()
  : fNevents(0), fBranchAdx(nullptr)
{
  // initialize ROOT
  TSystem ts;
  gSystem->Load("libFaserRootClassesDict");

  fFile = new TFile(fileName, "RECREATE");
  fTree = new TTree("faser","an event tree");
  fGeoTree = new TTree("faserGeo", "FASER simulation conditions");
}

RootEventIO::~RootEventIO()
{

}

RootEventIO* RootEventIO::GetInstance()
{
  if (instance == nullptr)
    {
      instance = new RootEventIO();
    }
  return instance;
}

void RootEventIO::SetFileName(G4String name)
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

void RootEventIO::Write(FaserEvent* hcont)
{
  fNevents++;

  std::ostringstream os;
  os << fNevents;
  std::string stevt = "Event_" + os.str();
  G4cout << "Writing " << stevt << G4endl;

  if (fBranchAdx == nullptr)
  {
    fBranchAdx = hcont;
    fTree->Branch("event", "FaserEvent", &fBranchAdx, 3200, 99);
  }
  else
  {
    fBranchAdx = hcont;
  }
  fTree->Fill();

}

void RootEventIO::Write(FaserGeometry* hcont)
{
  G4cout << "Saving run conditions" << G4endl;

  if (fGeoBranchAdx == nullptr)
  {
    fGeoBranchAdx = hcont;
    fGeoTree->Branch("conditions", "FaserGeometry", &fGeoBranchAdx, 3200, 99);
  }
  else
  {
    fGeoBranchAdx = hcont;
  }
  fGeoTree->Fill();
}

void RootEventIO::Close()
{
  fFile->cd();
  fGeoTree->Write();
  G4cout << "Wrote geometry data to " << fileName << G4endl;
  fTree->Write();
  if (fNevents > 0) G4cout << "Wrote a total of " << fNevents << " events to " << fileName << G4endl;
  fFile->Close();
  fNevents = 0;
}
