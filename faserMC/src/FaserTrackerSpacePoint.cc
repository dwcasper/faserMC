#include "FaserTrackerSpacePoint.hh"
#include <iostream>

using std::cout;


void FaserTrackerSpacePoint::print() const {

    cout << "        SpacePoint  plane=" << plane
         <<                   "  globalPos=(" << globalPos.X()
         <<                              ", " << globalPos.Y()
         <<                              ", " << globalPos.Z() << ")\n";

}
