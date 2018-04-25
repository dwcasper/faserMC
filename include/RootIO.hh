#ifndef INCLUDE_ROOTIO_HH
#define INCLUDE_ROOTIO_HH 1

#include "g4root.hh"

#include "FaserDigi.hh"

class RootIO
{
public:
  virtual ~RootIO();

  static RootIO* GetInstance();
  static void SetFileName(G4String name);
  void Write(FaserDigiCollection* dc);
  void Close();

protected:
  RootIO();

private:
  G4AnalysisManager* fAnalysisManager;

  std::vector<G4int> fPlaneVector;
  std::vector<G4int> fModuleVector;
  std::vector<G4int> fSensorVector;
  std::vector<G4int> fRowVector;
  std::vector<G4int> fStripVector;
  std::vector<G4int> fADCVector;
};

#endif
