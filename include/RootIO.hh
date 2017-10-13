#ifndef INCLUDE_ROOTIO_HH
#define INCLUDE_ROOTIO_HH 1

#include "TROOT.h"
#include "TFile.h"
#include "TSystem.h"
#include "TTree.h"

#include "FaserSensorHit.hh"

class RootIO
{
public:
  virtual ~RootIO();

  static RootIO* GetInstance();
  static void SetFileName(G4String name);
  void Write(std::vector<FaserSensorHit*>*);
  void Close();

protected:
  RootIO();

private:
  TFile* fFile;
  TTree* fTree;
  int fNevents;
  std::vector<FaserSensorHit*>* fBranchAdx;
};

#endif
