#ifndef FASER_TRACKER_EVENT_HH
#define FASER_TRACKER_EVENT_HH 1

#include "FaserTrackerSpacePoint.hh"

//------------------------------------------------------------------------------

class FaserTrackerEvent
{
public:

  int eventNumber;
  vector<FaserTrackerSpacePoint*> spacePoints;

  FaserTrackerEvent()
    : eventNumber {-1}
  {
  }

  FaserTrackerEvent(int eventNumber_)
    : eventNumber {eventNumber_}
  {
  }

  virtual ~FaserTrackerEvent() {
  }

};

#endif
