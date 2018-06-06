//
// Declaration of classes that need dictionaries
//

#include <map>
#include "FaserEvent.hh"
#include "FaserDigi.hh"
#include "FaserSensorHit.hh"
#include "FaserSamplerHit.hh"
#include "FaserTruthParticle.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"

std::vector<FaserSensorHit*> aGarbage;
std::vector<FaserDigi*> bGarbage;
std::vector<FaserTruthParticle*> cGarbage;
std::pair<G4String, G4AttDef> ddGarbage;
std::map<G4String, G4AttDef> dGarbage;
std::vector<G4AttValue> eGarbage;
std::map<G4int, G4double> fGarbage;
std::vector<FaserSamplerHit*> gGarbage;

#undef __G4String
