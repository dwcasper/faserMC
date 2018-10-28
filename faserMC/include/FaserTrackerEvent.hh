#pragma once

#include "FaserTrackerDigit.hh"
#include "FaserTrackerCluster.hh"
#include "FaserTrackerSpacePoint.hh"
#include "FaserTrackerTruthHit.hh"
#include "FaserTrackerTruthParticle.hh"
#include <vector>

//------------------------------------------------------------------------------

class FaserTrackerEvent
{
public:

  int eventNumber;
  std::vector<FaserTrackerDigit*> digits;
  std::vector<FaserTrackerCluster*> analogClusters;
  std::vector<FaserTrackerCluster*> digitalClusters;
  std::vector<FaserTrackerSpacePoint*> spacePoints;
  std::vector<FaserTrackerTruthHit*> truthHits;
  std::vector<FaserTrackerTruthParticle*> truthParticles;

  FaserTrackerEvent()
    : eventNumber {-1}
  {
  }

  FaserTrackerEvent(int eventNumber_)
    : eventNumber {eventNumber_}
  {
  }

  virtual ~FaserTrackerEvent() {
    //for (FaserTrackerDigit * digit : digits) {
    //    if (digit) delete digit;
    //}
    //for (FaserTrackerCluster * cl : analogClusters) {
    //    if (cl) delete cl;
    //}
    //for (FaserTrackerCluster * cl : digitalClusters) {
    //    if (cl) delete cl;
    //}
    //for (FaserTrackerSpacePoint * sp : spacePoints) {
    //    if (sp) delete sp;
    //}
    //for (FaserTrackerTruthParticle * tp : truthParticles) {
    //    if (tp) delete tp;
    //}
    //for (FaserTrackerTruthHit * th : truthHits) {
    //    if (th) delete th;
    //}
  }

};

