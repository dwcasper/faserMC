#ifndef FASER_EVENT_HH
#define FASER_EVENT_HH 1

#include "globals.hh"
#include "FaserTruthParticle.hh"
#include "FaserSensorHit.hh"
#include "FaserSamplerHit.hh"
#include "FaserDigi.hh"
#include "G4TrajectoryContainer.hh"

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

private:
  G4int fEventNumber;
  std::vector<FaserTruthParticle*> fParticles;
  std::vector<FaserSensorHit*> fHits;
  std::vector<FaserSamplerHit*> fSamples;
  std::vector<FaserDigi*> fDigis;
};

#endif
