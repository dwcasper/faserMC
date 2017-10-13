//
// Declaration of classes that need dictionaries
//

#include "FaserSensorHit.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"

std::vector<FaserSensorHit*> aGarbage;
std::map<G4String, G4AttDef> bGarbage;
std::vector<G4AttValue> cGarbage;

#undef __G4String
