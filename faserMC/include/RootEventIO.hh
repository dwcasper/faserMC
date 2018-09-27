#ifndef INCLUDE_ROOTEVENTIO_HH
#define INCLUDE_ROOTEVENTIO_HH 1

#include "G4String.hh"

class FaserEvent;
class FaserGeometry;
class FaserTrackerEvent;
class FaserTrackerGeometry;
class TFile;
class TTree;

class RootEventIO
{
public:
  virtual ~RootEventIO();

  static RootEventIO* GetInstance();
  static void SetFileName(G4String name);
  void SetTrackerGeo(FaserTrackerGeometry * geo) { fTrackerGeoBranchAdx = geo; }
  void Write(FaserEvent*);
  void Write(FaserGeometry*);
  void Write(FaserTrackerEvent*);
  void WriteTrackerGeo();
  void Close();

protected:
  RootEventIO();

private:
  TFile* fFile;
  TTree* fTree;
  TTree* fGeoTree;
  TFile* fTrackerFile;
  TTree* fTrackerTree;
  TTree* fTrackerGeoTree;
  int fNevents;
  FaserEvent* fBranchAdx;
  FaserGeometry* fGeoBranchAdx;
  FaserTrackerEvent* fTrackerBranchAdx;
  FaserTrackerGeometry* fTrackerGeoBranchAdx;
};

#endif
