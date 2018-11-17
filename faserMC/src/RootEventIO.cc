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
#include "G4AutoLock.hh"

namespace
{
  G4Mutex rootIoMutex = G4MUTEX_INITIALIZER;
  G4Mutex rootIoDataMutex = G4MUTEX_INITIALIZER;
}

G4String RootEventIO::sFileName = "FaserMCEvent_sim.root";
G4String RootEventIO::sTreeName = "faser";
RootEventIO* RootEventIO::sInstance = nullptr;

RootEventIO::RootEventIO()
  : fNevents(0)
  , fBranchAdx(nullptr)
{
  // initialize ROOT
  TSystem ts;
  gSystem->Load("libFaserRootClassesDict");

  fFile = new TFile(sFileName, "RECREATE");
  fTree = new TTree(sTreeName, "an event tree");
}

RootEventIO::~RootEventIO()
{
  G4cout << "Deleting RootEventIO instance" << G4endl;
}

RootEventIO* RootEventIO::GetInstance()
{
  G4AutoLock l(&rootIoMutex);

  if (sInstance == nullptr)
  {
      sInstance = new RootEventIO();
  }
  return sInstance;
}

void RootEventIO::SetFileName(G4String name)
{
  G4AutoLock l(&rootIoDataMutex);
  if (name == sFileName) return;
  sFileName = name;

  if (sInstance == nullptr) return;

  G4cout << "Closing previous ROOT file" << G4endl;

  sInstance->Close();
  delete sInstance;
  sInstance = nullptr;

  return;
}

void RootEventIO::Write(FaserEvent* hcont)
{
  G4AutoLock l(&rootIoDataMutex);
  fNevents++;

  std::ostringstream os;
  os << fNevents;
  std::string stevt = "Event_" + os.str();
  G4cout << "Writing " << stevt << " to " << sTreeName << " in "
         << sFileName << "...";

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
  G4AutoLock l(&rootIoDataMutex);
  if (fFile != nullptr)
  {
    fFile->cd();
    if (fTree != nullptr)
    {
      fTree->Write();
    }
    if (fNevents > 0) G4cout << "Wrote a total of " << fNevents << " events to " << sFileName << G4endl;
    fFile->Close();
  }
  sInstance = nullptr;
  fNevents = 0;
  delete this;
}
