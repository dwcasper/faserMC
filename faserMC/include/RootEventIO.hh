#pragma once

#include "G4String.hh"

class FaserEvent;
class TFile;
class TTree;

class RootEventIO
{
private:
  static RootEventIO* sInstance;
  RootEventIO();

  TFile* fFile;
  TTree* fTree;
  int fNevents;
  FaserEvent* fBranchAdx;
  static G4String sFileName;
  static G4String sTreeName;

public:
  ~RootEventIO();
  static RootEventIO* GetInstance();
  static void SetFileName(G4String name);
  void Write(FaserEvent*);
  void Close();
};

