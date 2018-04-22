#ifndef INCLUDE_ROOTIO_HH
#define INCLUDE_ROOTIO_HH 1

#include "TROOT.h"
#include "TFile.h"
#include "TSystem.h"
#include "TTree.h"

#include "FaserDigi.hh"

class RootIO
{
public:
  virtual ~RootIO();

  static RootIO* GetInstance();
  static void SetFileName(G4String name);
  void Write(std::vector<FaserDigi*>*);
  void Close();

protected:
  RootIO();

private:
  TFile* fFile;
  TTree* fTree;
  int fNevents;
  std::vector<FaserDigi*>* fBranchAdx;
};

#endif
