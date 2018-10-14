#pragma once

#include "FaserTrackerSpacePoint.hh"
#include "FaserTrackerTruthHit.hh"
#include "FaserTrackerTruthParticle.hh"
#include <vector>

//------------------------------------------------------------------------------

class FaserTrackerEvent
{
public:

  int eventNumber;
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
    for (FaserTrackerSpacePoint * sp : spacePoints) {
        if (sp) delete sp;
    }
    for (FaserTrackerTruthParticle * tp : truthParticles) {
        if (tp) delete tp;
    }
  }

};

