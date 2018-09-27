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
#include "FaserCluster.hh"
#include "FaserSpacePoint.hh"
#include "TF1.h"
#include "FaserGeometry.hh"

std::vector<FaserSensorHit*> aGarbage;
std::vector<FaserDigi*> bGarbage;
std::vector<FaserTruthParticle*> cGarbage;
std::vector<FaserSamplerHit*> dGarbage;
std::vector<FaserCluster*> eGarbage;
std::vector<FaserSpacePoint*> fGarbage;

std::pair<G4String, G4AttDef> gGarbage;
std::map<G4String, G4AttDef> hGarbage;
std::vector<G4AttValue> iGarbage;
std::map<G4int, G4double> jGarbage;
std::map<G4String, G4String> kGarbage;
std::map<G4String, std::map<G4String, G4String>> lGarbage;

#undef __G4String
