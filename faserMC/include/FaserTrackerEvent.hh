#ifndef FASER_TRACKER_EVENT_HH
#define FASER_TRACKER_EVENT_HH 1

#include "FaserTrackerSpacePoint.hh"
#include "FaserTrackerTruthParticle.hh"
#include <vector>

//------------------------------------------------------------------------------

class FaserTrackerEvent
{
public:

  int eventNumber;
  std::vector<FaserTrackerSpacePoint*> spacePoints;
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

#endif
