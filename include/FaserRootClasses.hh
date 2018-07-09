//
// Declaration of classes that need dictionaries
//

#include <map>
#include "FaserEvent.hh"
#include "FaserDigi.hh"
#include "FaserSensorHit.hh"
#include "FaserTruthParticle.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "FaserCluster.hh"
#include "TF1.h"
#include "FaserGeometry.hh"

std::vector<FaserSensorHit*> aGarbage;
std::vector<FaserDigi*> bGarbage;
std::vector<FaserTruthParticle*> cGarbage;
std::vector<FaserCluster*> hGarbage;

std::pair<G4String, G4AttDef> ddGarbage;
std::map<G4String, G4AttDef> dGarbage;
std::vector<G4AttValue> eGarbage;
std::map<G4int, G4double> fGarbage;
std::map<G4String, G4String> jjGarbage;
std::map<G4String, std::map<G4String, G4String>> jGarbage;

#undef __G4String
