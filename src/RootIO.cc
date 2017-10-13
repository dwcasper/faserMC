
#include <sstream>

#include "RootIO.hh"

#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

static G4String fileName = "FaserMC_Default.root";
static RootIO* instance = nullptr;

RootIO::RootIO()
  : fNevents(0), fBranchAdx(nullptr)
{
  // initialize ROOT
  TSystem ts;
  gSystem->Load("libFaserRootClassesDict");

  fFile = new TFile(fileName, "RECREATE");
  fTree = new TTree("faser","an event tree");
}

RootIO::~RootIO()
{}

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

void RootIO::Write(std::vector<FaserSensorHit*>* hcont)
{
  fNevents++;

  std::ostringstream os;
  os << fNevents;
  std::string stevt = "Event_" + os.str();
  //const char* chevt = stevt.c_str();
  G4cout << "Writing " << stevt << G4endl;

  //fFile->WriteObject(hcont, chevt);

  if (fBranchAdx == nullptr)
  {
    fBranchAdx = hcont;
    fTree->Branch("Hits", "std::vector<FaserSensorHit*>", &fBranchAdx, 32000, 99);
  }
  else
  {
    fBranchAdx = hcont;
  }
  fTree->Fill();

}

void RootIO::Close()
{
  fTree->Write();
  fFile->Close();
}
