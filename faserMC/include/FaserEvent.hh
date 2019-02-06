#ifndef FASER_EVENT_HH
#define FASER_EVENT_HH 1

#include "globals.hh"
#include "FaserTruthParticle.hh"
#include "FaserSensorHit.hh"
#include "FaserCaloHit.hh"
#include "FaserDigi.hh"
#include "FaserCaloDigi.hh"
#include "FaserCluster.hh"
#include "FaserSpacePoint.hh"
#include "G4TrajectoryContainer.hh"

#include <map>
#include <vector>

using std::map;
using std::vector;

class FaserEvent
{
public:
  FaserEvent();
  FaserEvent(G4int eventNumber);
  virtual ~FaserEvent();

  G4int GetEventNumber() { return fEventNumber; }
  void  SetEventNumber(G4int eventNumber) { fEventNumber = eventNumber; }

  std::vector<FaserTruthParticle*>& Particles() { return fParticles; }
  void SetParticles(G4TrajectoryContainer* particles);

  // create owned copies of objects managed by G4
  std::vector<FaserSensorHit*>& TrackerHits() { return fTrackerHits; }
  void SetTrackerHits(const FaserSensorHitsCollection* hits) { for (G4int i=0; i < hits->entries(); i++) fTrackerHits.push_back(new FaserSensorHit(*(*hits)[i])); }

  std::vector<FaserSensorHit*>& SamplerHits() { return fSamplerHits; }
  void SetSamplerHits(const FaserSensorHitsCollection* hits) { for (G4int i=0; i < hits->entries(); i++) fSamplerHits.push_back(new FaserSensorHit(*(*hits)[i])); }

  std::vector<FaserCaloHit*>& CaloHits() { return fCaloHits; }
  void SetCaloHits(const FaserCaloHitsCollection* hits) { for (G4int i=0; i < hits->entries(); i++) fCaloHits.push_back(new FaserCaloHit(*(*hits)[i])); }

  std::vector<FaserDigi*> TrackerDigis()& { return fTrackerDigis; }
  void SetTrackerDigis(const FaserDigiCollection* digis) { for (G4int i=0; i < digis->entries(); i++) fTrackerDigis.push_back(new FaserDigi(*(*digis)[i])); }

  std::vector<FaserDigi*> SamplerDigis()& { return fSamplerDigis; }
  void SetSamplerDigis(const FaserDigiCollection* digis) { for (G4int i=0; i < digis->entries(); i++) fSamplerDigis.push_back(new FaserDigi(*(*digis)[i])); }

  std::vector<FaserCaloDigi*> CaloDigis()& { return fCaloDigis; }
  void SetCaloDigis(const FaserCaloDigiCollection* digis) { for (G4int i=0; i < digis->entries(); i++) fCaloDigis.push_back(new FaserCaloDigi(*(*digis)[i])); }

  std::vector<FaserCluster*>& Clusters() { return fClusters; }
  void SetClusters();

  std::vector<FaserSpacePoint*>& SpacePoints() { return fSpacePoints; }
  void SetSpacePoints();


private:
  G4int fEventNumber; 
  std::vector<FaserTruthParticle*> fParticles;
  std::vector<FaserSensorHit*> fTrackerHits;
  std::vector<FaserSensorHit*> fSamplerHits;
  std::vector<FaserCaloHit*> fCaloHits;
  std::vector<FaserDigi*> fTrackerDigis;
  std::vector<FaserDigi*> fSamplerDigis;
  std::vector<FaserCaloDigi*> fCaloDigis;
  std::vector<FaserCluster*> fClusters;
  std::vector<FaserSpacePoint*> fSpacePoints;

  ////////////////////////////////////////////////////////////////////////////////
  // Utility methods for cluster finding                                        //

  int rowID(FaserDigi* d);
  vector<vector<FaserDigi*>> clusterOneRow(vector<FaserDigi*> digits);
  void sortDigits(vector<FaserDigi*>& v);
  map<int, vector<FaserDigi*>> mapDigitsByPlane(FaserEvent & evt);
  map<int, vector<FaserDigi*>> mapDigitsByRow(vector<FaserDigi*> v);
};

#endif
