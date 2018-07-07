#ifndef FASER_EVENT_HH
#define FASER_EVENT_HH 1

#include "globals.hh"
#include "FaserTruthParticle.hh"
#include "FaserSensorHit.hh"
#include "FaserSamplerHit.hh"
#include "FaserDigi.hh"
#include "FaserCluster.hh"
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

  std::vector<FaserSensorHit*>& Hits() { return fHits; }
  void SetHits(const FaserSensorHitsCollection* hits) { for (G4int i=0; i < hits->entries(); i++) fHits.push_back((*hits)[i]); }

  std::vector<FaserSamplerHit*>& Samples() { return fSamples; }
  void SetSamples(const FaserSamplerHitsCollection* hits) { for (G4int i=0; i < hits->entries(); i++) fSamples.push_back((*hits)[i]); }

  std::vector<FaserDigi*> Digis()& { return fDigis; }
  void SetDigis(const FaserDigiCollection* digis) { for (G4int i=0; i < digis->entries(); i++) fDigis.push_back((*digis)[i]); }

  std::vector<FaserCluster*>& Clusters() { return fClusters; }
  void SetClusters();


private:
  G4int fEventNumber;
  std::vector<FaserTruthParticle*> fParticles;
  std::vector<FaserSensorHit*> fHits;
  std::vector<FaserSamplerHit*> fSamples;
  std::vector<FaserDigi*> fDigis;
  std::vector<FaserCluster*> fClusters;

  ////////////////////////////////////////////////////////////////////////////////
  // Utility methods for cluster finding                                        //

  int rowID(FaserDigi* d);
  vector<vector<FaserDigi*>> clusterOneRow(vector<FaserDigi*> digits);
  void sortDigits(vector<FaserDigi*>& v);
  map<int, vector<FaserDigi*>> mapDigitsByPlane(FaserEvent & evt);
  map<int, vector<FaserDigi*>> mapDigitsByRow(vector<FaserDigi*> v);

};

#endif
