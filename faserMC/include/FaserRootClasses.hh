//
// Declaration of classes that need dictionaries
//

#include <map>
#include "FaserEvent.hh"
#include "FaserDigi.hh"
#include "FaserSensorHit.hh"
#include "FaserCaloHit.hh"
#include "FaserCaloDigi.hh"
#include "FaserTruthParticle.hh"
#include "G4String.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "FaserCluster.hh"
#include "FaserSpacePoint.hh"

std::vector<FaserSensorHit*> aGarbage;
std::vector<FaserCaloHit*> aaGarbage;
std::vector<FaserDigi*> bGarbage;
std::vector<FaserCaloDigi*> bbGarbage;
std::vector<FaserTruthParticle*> cGarbage;
std::vector<FaserCluster*> hGarbage;
std::vector<FaserSpacePoint*> iGarbage;

std::pair<G4String, G4AttDef> ddGarbage;
std::map<G4String, G4AttDef> dGarbage;
std::vector<G4AttValue> eGarbage;
std::map<G4int, G4double> fGarbage;
std::map<G4String, G4String> jjGarbage;
std::map<G4String, std::map<G4String, G4String>> jGarbage;


