#include <sstream>

#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"

#include "RootEventIO.hh"
#include "FaserEvent.hh"

#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

static G4String fileName = "FaserMCEvent_Default.root";
static RootEventIO* instance = nullptr;

RootEventIO::RootEventIO()
  : fNevents(0), fBranchAdx(nullptr)
{
  // initialize ROOT
  TSystem ts;
  gSystem->Load("libFaserRootClassesDict");

  fFile = new TFile(fileName, "RECREATE");
  fTree = new TTree("faser","an event tree");
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
  //const char* chevt = stevt.c_str();
  G4cout << "Writing " << stevt << G4endl;

  if (fBranchAdx == nullptr)
    {
      fBranchAdx = hcont;
      //fTree->Branch("Hits", "std::vector<FaserSensorHit*>", &fBranchAdx, 32000, 99);
      fTree->Branch("event", "FaserEvent", &fBranchAdx, 3200, 99);
    }
  else
    {
      fBranchAdx = hcont;
    }
  fTree->Fill();

}

void RootEventIO::Close()
{
  fTree->Write();
  if (fNevents > 0) G4cout << "Wrote a total of " << fNevents << " events to " << fileName << G4endl;
  fFile->Close();
  fNevents = 0;
}
