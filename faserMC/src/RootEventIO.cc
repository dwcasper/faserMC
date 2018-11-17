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

static G4String fileName = "FaserMCEvent_sim.root";
static G4String treeName = "faser";
static RootEventIO* instance = nullptr;

RootEventIO::RootEventIO()
  : fNevents(0)
  , fBranchAdx(nullptr)
{
  // initialize ROOT
  TSystem ts;
  gSystem->Load("libFaserRootClassesDict");
  gSystem->Load("libFaserTrackerClassesDict");

  fFile = new TFile(fileName, "RECREATE");
  fTree = new TTree(treeName, "an event tree");
}

RootEventIO::~RootEventIO()
{
  Close();
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
  G4cout << "Writing " << stevt << " to " << treeName << " in "
         << fileName << "...";

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

  G4cout << "done" << G4endl;
}

void RootEventIO::Close()
{
  if (fFile != nullptr)
  {
    fFile->cd();
    if (fTree != nullptr)
    {
      fTree->Write();
      fTree = nullptr;
    }
    if (fNevents > 0) G4cout << "Wrote a total of " << fNevents << " events to " << fileName << G4endl;
    fFile->Close();
    fFile = nullptr;
  }
  fNevents = 0;
}
