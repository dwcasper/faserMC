//
// Declaration of classes that need dictionaries
//

#include "FaserEvent.hh"
#include "FaserDigi.hh"
#include "FaserSensorHit.hh"
#include "FaserTruthParticle.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include <map>

std::vector<FaserSensorHit*> aGarbage;
std::vector<FaserDigi*> bGarbage;
std::vector<FaserTruthParticle*> cGarbage;
std::map<G4String, G4AttDef> dGarbage;
std::vector<G4AttValue> eGarbage;
std::map<G4int, G4double> fGarbage;

#undef __G4String
