#ifndef INCLUDE_ROOTIO_HH
#define INCLUDE_ROOTIO_HH 1

#include "g4root.hh"

#include "FaserSensorHit.hh"
#include "FaserDigi.hh"

class RootIO
{
public:
  virtual ~RootIO();

  static RootIO* GetInstance();
  static void SetFileName(G4String name);
  void AddDigits(FaserDigiCollection* dc);
  void AddTruth(FaserSensorHitsCollection* truth);
  void WriteEvent();
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
  std::vector<G4double> fChargeVector;

  std::vector<G4int> fTruthPlaneVector;
  std::vector<G4int> fTruthModuleVector;
  std::vector<G4int> fTruthSensorVector;
  std::vector<G4int> fTruthRowVector;
  std::vector<G4int> fTruthStripVector;

  std::vector<G4int> fTruthTrackVector;
  std::vector<G4String> fTruthParticleVector;
  
  std::vector<G4double> fTruthGlobalXVector;
  std::vector<G4double> fTruthGlobalYVector;
  std::vector<G4double> fTruthGlobalZVector;
  
  std::vector<G4double> fTruthLocalXVector;
  std::vector<G4double> fTruthLocalYVector;
  std::vector<G4double> fTruthLocalZVector;

  std::vector<G4double> fTruthVertexXVector;
  std::vector<G4double> fTruthVertexYVector;
  std::vector<G4double> fTruthVertexZVector;

  std::vector<G4double> fTruthVertexPXVector;
  std::vector<G4double> fTruthVertexPYVector;
  std::vector<G4double> fTruthVertexPZVector;

  std::vector<G4double> fTruthVertexKineticEnergyVector;
};

#endif
