#include <sstream>

#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"

#include "RootEventIO.hh"
#include "FaserEvent.hh"
#include "FaserGeometry.hh"
#include "FaserTrackerEvent.hh"
#include "FaserTrackerGeometry.hh"

#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

static G4String fileName = "FaserMCEvent_Default.root";
static G4String treeName = "faser";
static G4String geoTreeName = "faserGeo";

static G4String trackerFileName = "FaserMCEvent_tracking.root";
static G4String trackerTreeName = "events";
static G4String trackerGeoTreeName = "geo";

static G4String geoFileName = "faserGeo.mac";
static RootEventIO* instance = nullptr;

RootEventIO::RootEventIO()
  : fNevents(0)
  , fBranchAdx(nullptr)
  , fGeoBranchAdx(nullptr)
  , fTrackerBranchAdx(nullptr)
  , fTrackerGeoBranchAdx(nullptr)
{
  // initialize ROOT
  TSystem ts;
  gSystem->Load("libFaserRootClassesDict");
  gSystem->Load("libFaserTrackerClassesDict");

  fFile = new TFile(fileName, "RECREATE");
  fTree = new TTree(treeName, "an event tree");
  fGeoTree = new TTree(geoTreeName, "FASER simulation conditions");

  fTrackerFile = new TFile(trackerFileName, "recreate");
  fTrackerTree = new TTree(trackerTreeName, "space points for tracking");
  fTrackerGeoTree = new TTree(trackerGeoTreeName, "FASER geometry info for tracking");
}

RootEventIO::~RootEventIO()
{
  //NB: `FaserDetectorConstruction` does not delete `fTrackerGeoBranchAdx`
  //    (must currently be deleted here)
  //TODO: Clean this up!
  if (fTrackerGeoBranchAdx) delete fTrackerGeoBranchAdx;
  fTrackerGeoBranchAdx = nullptr;
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

void RootEventIO::Write(FaserGeometry* hcont)
{
  G4cout << "Saving run conditions in " << geoTreeName << " in "
         << fileName << "...";

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

  G4cout << "done" << G4endl;
}

void RootEventIO::Write(FaserTrackerEvent* hcont)
{
  G4cout << "Writing space points to " << trackerTreeName << " in "
         << trackerFileName << "...";

  if (fTrackerBranchAdx == nullptr)
  {
    fTrackerBranchAdx = hcont;
    fTrackerTree->Branch("events", "FaserTrackerEvent", &fTrackerBranchAdx, 3200, 99);
  }
  else
  {
    fTrackerBranchAdx = hcont;
  }
  fTrackerTree->Fill();

  G4cout << "done" << G4endl;
}

void RootEventIO::WriteTrackerGeo()
{
  G4cout << "Saving geometry info to " << trackerGeoTreeName << " in "
         << trackerFileName << "...";

  fTrackerGeoTree->Branch("geo", "FaserTrackerGeometry", &fTrackerGeoBranchAdx, 3200, 99);
  fTrackerGeoTree->Fill();

  G4cout << "done" << G4endl;
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

  G4cout << "Writing space points and geometry to tracker trees...";
  fTrackerFile->cd();
  fTrackerGeoTree->Write();
  fTrackerTree->Write();
  fTrackerFile->Close();
  G4cout << "done\n"
         << "   Trees saved to " << trackerFileName << G4endl;
}
